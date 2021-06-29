#include "silhouette/scene.h"

#include <assimp/mesh.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "nucleus/streams/input_stream.h"

namespace si {

namespace {

void create_mesh(Mesh* mesh, aiMesh* impMesh) {
  mesh->materialIndex = impMesh->mMaterialIndex;

#if 0
  mesh->positions.emplaceBack(-1.0f, 0.0f, -1.0f);
  mesh->positions.emplaceBack(-1.0f, 0.0f,  1.0f);
  mesh->positions.emplaceBack( 1.0f, 0.0f,  1.0f);
  mesh->positions.emplaceBack( 1.0f, 0.0f,  1.0f);
  mesh->positions.emplaceBack( 1.0f, 0.0f, -1.0f);
  mesh->positions.emplaceBack(-1.0f, 0.0f, -1.0f);

  mesh->texCoords.emplaceBack(0.0f, 0.0f);
  mesh->texCoords.emplaceBack(0.0f, 1.0f);
  mesh->texCoords.emplaceBack(1.0f, 1.0f);
  mesh->texCoords.emplaceBack(1.0f, 1.0f);
  mesh->texCoords.emplaceBack(1.0f, 0.0f);
  mesh->texCoords.emplaceBack(0.0f, 0.0f);

#else

  U32 numVertices = impMesh->mNumVertices;

  mesh->positions.reserve(numVertices);
  mesh->texCoords.reserve(numVertices);

  for (U32 i = 0; i < numVertices; ++i) {
    mesh->positions.emplaceBack(impMesh->mVertices[i].x, impMesh->mVertices[i].y,
                                impMesh->mVertices[i].z);
    if (impMesh->mTextureCoords[0]) {
      mesh->texCoords.emplaceBack(impMesh->mTextureCoords[0][i].x, impMesh->mTextureCoords[0][i].y);
    }
  }
#endif  // 0
}

void create_node(Node* node, aiNode* impNode) {
  // LOG(Info) << "createNode (" << impNode->mName.data << ") >>>";

  node->transform.col[0] = fl::Vec4{impNode->mTransformation.a1, impNode->mTransformation.b1,
                                    impNode->mTransformation.c1, impNode->mTransformation.d1};
  node->transform.col[1] = fl::Vec4{impNode->mTransformation.a2, impNode->mTransformation.b2,
                                    impNode->mTransformation.c2, impNode->mTransformation.d2};
  node->transform.col[2] = fl::Vec4{impNode->mTransformation.a3, impNode->mTransformation.b3,
                                    impNode->mTransformation.c3, impNode->mTransformation.d3};
  node->transform.col[3] = fl::Vec4{impNode->mTransformation.a4, impNode->mTransformation.b4,
                                    impNode->mTransformation.c4, impNode->mTransformation.d4};

  // LOG(Info) << "transform.col[0] = " << node->transform.col[0];
  // LOG(Info) << "transform.col[1] = " << node->transform.col[1];
  // LOG(Info) << "transform.col[2] = " << node->transform.col[2];
  // LOG(Info) << "transform.col[3] = " << node->transform.col[3];

  for (U32 i = 0; i < impNode->mNumMeshes; ++i) {
    node->meshIndices.emplaceBack(impNode->mMeshes[i]);
  }

  // LOG(Info) << "creating " << impNode->mNumChildren << " child nodes";
  for (U32 i = 0; i < impNode->mNumChildren; ++i) {
    auto result = node->children.emplaceBack();
    create_node(&result.element(), impNode->mChildren[i]);
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

  unsigned int diffuseTextureCount = src->GetTextureCount(aiTextureType_DIFFUSE);
  if (diffuseTextureCount < 1) {
    LOG(Warning) << "No diffuse textures found on material.";
  } else {
    if (diffuseTextureCount > 1) {
      LOG(Warning) << "Only one diffuse texture is supported. " << diffuseTextureCount << " found.";
    }

    aiString path;
    src->GetTexture(aiTextureType_DIFFUSE, 0, &path);
    dst->diffuse.texture.append(path.data, path.length);
  }
}

}  // namespace

bool Scene::load_from_collada(nu::InputStream* stream) {
  Assimp::Importer importer;

  nu::InputStream::SizeType bytesRemaining = stream->getBytesRemaining();

  nu::DynamicArray<U8> buffer;
  buffer.resize(bytesRemaining);
  stream->read(buffer.data(), bytesRemaining);

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
    aiMesh* impMesh = imp->mMeshes[i];

    // We only do triangle meshes right now.
    if (!(impMesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE)) {
      LOG(Warning) << "Found non triangle mesh, skipping.";
      continue;
    }

    auto result = meshes_.emplaceBack();
    create_mesh(&result.element(), impMesh);
  }

  for (U32 i = 0; i < imp->mNumMaterials; ++i) {
    auto* impMaterial = imp->mMaterials[i];

    auto result = materials_.pushBack({{{0, 0, 0, 0}, {}}});
    create_material(&result.element(), impMaterial);
  }

  create_node(&root_node_, imp->mRootNode);

  return true;
}

}  // namespace si
