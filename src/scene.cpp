#include "silhouette/scene.h"

#include <assimp/mesh.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "nucleus/streams/input_stream.h"

namespace si {

namespace {

void create_mesh(Mesh* mesh, aiMesh* imp_mesh) {
  mesh->material_index = imp_mesh->mMaterialIndex;

  U32 num_vertices = imp_mesh->mNumVertices;

  mesh->positions.reserve(num_vertices);
  mesh->normals.reserve(num_vertices);
  mesh->tex_coords.reserve(num_vertices);

  for (U32 i = 0; i < num_vertices; ++i) {
    mesh->positions.emplaceBack(imp_mesh->mVertices[i].x, imp_mesh->mVertices[i].y,
                                imp_mesh->mVertices[i].z);
    mesh->normals.emplaceBack(imp_mesh->mNormals[i].x, imp_mesh->mNormals[i].y,
                              imp_mesh->mNormals[i].z);
    if (imp_mesh->mTextureCoords[0]) {
      mesh->tex_coords.emplaceBack(imp_mesh->mTextureCoords[0][i].x,
                                   imp_mesh->mTextureCoords[0][i].y);
    }
  }
}

void create_node(Node* node, aiNode* imp_node) {
  // LOG(Info) << "createNode (" << impNode->mName.data << ") >>>";

  node->transform.col[0] = fl::Vec4{imp_node->mTransformation.a1, imp_node->mTransformation.b1,
                                    imp_node->mTransformation.c1, imp_node->mTransformation.d1};
  node->transform.col[1] = fl::Vec4{imp_node->mTransformation.a2, imp_node->mTransformation.b2,
                                    imp_node->mTransformation.c2, imp_node->mTransformation.d2};
  node->transform.col[2] = fl::Vec4{imp_node->mTransformation.a3, imp_node->mTransformation.b3,
                                    imp_node->mTransformation.c3, imp_node->mTransformation.d3};
  node->transform.col[3] = fl::Vec4{imp_node->mTransformation.a4, imp_node->mTransformation.b4,
                                    imp_node->mTransformation.c4, imp_node->mTransformation.d4};

  // LOG(Info) << "transform.col[0] = " << node->transform.col[0];
  // LOG(Info) << "transform.col[1] = " << node->transform.col[1];
  // LOG(Info) << "transform.col[2] = " << node->transform.col[2];
  // LOG(Info) << "transform.col[3] = " << node->transform.col[3];

  for (U32 i = 0; i < imp_node->mNumMeshes; ++i) {
    node->meshIndices.emplaceBack(imp_node->mMeshes[i]);
  }

  // LOG(Info) << "creating " << impNode->mNumChildren << " child nodes";
  for (U32 i = 0; i < imp_node->mNumChildren; ++i) {
    auto result = node->children.emplaceBack();
    create_node(&result.element(), imp_node->mChildren[i]);
  }

  // LOG(Info) << "<<< createNode";
}

void create_material(Material* dst, aiMaterial* src) {
  aiColor4D color;
  if (src->Get(AI_MATKEY_COLOR_DIFFUSE, color) == aiReturn_SUCCESS) {
    dst->diffuse.color =
        RGBA{static_cast<U8>(round(color.r * 255.0f)), static_cast<U8>(round(color.g * 255.0f)),
             static_cast<U8>(round(color.b * 255.0f)), static_cast<U8>(round(color.a * 255.0f))};
  }

  unsigned int diffuse_texture_count = src->GetTextureCount(aiTextureType_DIFFUSE);
  if (diffuse_texture_count < 1) {
    LOG(Warning) << "No diffuse textures found on material.";
  } else {
    if (diffuse_texture_count > 1) {
      LOG(Warning) << "Only one diffuse texture is supported. " << diffuse_texture_count
                   << " found.";
    }

    aiString path;
    src->GetTexture(aiTextureType_DIFFUSE, 0, &path);
    dst->diffuse.texture.append(path.data, path.length);
  }
}

}  // namespace

bool Scene::load_from_collada(nu::InputStream* stream) {
  Assimp::Importer importer;

  nu::InputStream::SizeType bytes_remaining = stream->getBytesRemaining();

  nu::DynamicArray<U8> buffer;
  buffer.resize(bytes_remaining);
  stream->read(buffer.data(), bytes_remaining);

  const aiScene* imp = importer.ReadFileFromMemory(buffer.data(), buffer.size(), 0);

#if 0
  for (U32 i = 0; i < imp->mNumMaterials; ++i) {
    aiMaterial* material = imp->mMaterials[i];

    for (U32 j = 0; j < material->mNumProperties; ++j) {
      aiMaterialProperty* property = material->mProperties[j];

      LOG(Info) << "property = " << property->mKey.data;
      if (property->mType == aiPTI_String) {
        LOG(Info) << "  value = " << property->mData + 4;
      }
      LOG(Info) << "  semantic = " << property->mSemantic;
    }
  }
#endif  // 0

  for (U32 i = 0; i < imp->mNumMeshes; ++i) {
    aiMesh* imp_mesh = imp->mMeshes[i];

    // We only do triangle meshes right now.
    if (!(imp_mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE)) {
      LOG(Warning) << "Found non triangle mesh, skipping.";
      continue;
    }

    auto result = meshes_.emplaceBack(imp_mesh->mName.C_Str());
    create_mesh(&result.element(), imp_mesh);
  }

  for (U32 i = 0; i < imp->mNumMaterials; ++i) {
    auto* imp_material = imp->mMaterials[i];

    auto result = materials_.pushBack({{{0, 0, 0, 0}, {}}});
    create_material(&result.element(), imp_material);
  }

  create_node(&root_node_, imp->mRootNode);

  return true;
}

}  // namespace si
