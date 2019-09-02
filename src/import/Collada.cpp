
#include "nucleus/FilePath.h"
#include "nucleus/Streams/FileInputStream.h"
#include "silhouette/Scene/Scene.h"

#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "assimp/scene.h"

namespace si {

namespace {

void createMesh(Mesh* mesh, aiMesh* impMesh) {
  mesh->vertices.reserve(impMesh->mNumVertices);

  U16 index = 0;
  for (U32 i = 0; i < impMesh->mNumVertices; ++i) {
    aiVector3D& impVec = impMesh->mVertices[i];
    mesh->vertices.emplaceBack(impVec.x, impVec.y, impVec.z);
    mesh->indices.emplaceBack(index++);
  }
}

void createNode(Node* node, aiNode* impNode) {
  LOG(Info) << "createNode (" << impNode->mName.data << ") >>>";

  node->transform.col[0] = ca::Vec4{impNode->mTransformation.a1, impNode->mTransformation.b1,
                                    impNode->mTransformation.c1, impNode->mTransformation.d1};
  node->transform.col[1] = ca::Vec4{impNode->mTransformation.a2, impNode->mTransformation.b2,
                                    impNode->mTransformation.c2, impNode->mTransformation.d2};
  node->transform.col[2] = ca::Vec4{impNode->mTransformation.a3, impNode->mTransformation.b3,
                                    impNode->mTransformation.c3, impNode->mTransformation.d3};
  node->transform.col[3] = ca::Vec4{impNode->mTransformation.a4, impNode->mTransformation.b4,
                                    impNode->mTransformation.c4, impNode->mTransformation.d4};

  LOG(Info) << "transform.col[0] = " << node->transform.col[0];
  LOG(Info) << "transform.col[1] = " << node->transform.col[1];
  LOG(Info) << "transform.col[2] = " << node->transform.col[2];
  LOG(Info) << "transform.col[3] = " << node->transform.col[3];

  for (U32 i = 0; i < impNode->mNumMeshes; ++i) {
    node->meshIndices.emplaceBack(impNode->mMeshes[i]);
  }

  LOG(Info) << "creating " << impNode->mNumChildren << " child nodes";
  for (U32 i = 0; i < impNode->mNumChildren; ++i) {
    auto result = node->children.emplaceBack();
    createNode(&result.element(), impNode->mChildren[i]);
  }

  LOG(Info) << "<<< createNode";
}

}  // namespace

bool loadCollada(Scene* scene, nu::InputStream* stream) {
  Assimp::Importer importer;

  nu::InputStream::SizeType bytesRemaining = stream->getBytesRemaining();

  nu::DynamicArray<U8> buffer;
  buffer.resize(bytesRemaining);
  stream->read(buffer.data(), bytesRemaining);

  const aiScene* imp = importer.ReadFileFromMemory(buffer.data(), buffer.size(), 0);

  for (U32 i = 0; i < imp->mNumMeshes; ++i) {
    aiMesh* impMesh = imp->mMeshes[i];

    // We only do triangle meshes right now.
    if (!(impMesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE)) {
      LOG(Warning) << "Found non triangle mesh, skipping.";
      continue;
    }

    auto result = scene->meshes.emplaceBack();
    createMesh(&result.element(), impMesh);
  }

  createNode(&scene->rootNode, imp->mRootNode);

  return true;
}

bool loadCollada(Scene* scene, const nu::FilePath& path) {
  nu::FileInputStream stream{path};
  if (!stream.openedOk()) {
    return false;
  }

  return loadCollada(scene, &stream);
}

}  // namespace si
