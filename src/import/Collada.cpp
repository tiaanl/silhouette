
#include "nucleus/FilePath.h"
#include "nucleus/Streams/FileInputStream.h"
#include "silhouette/scene/Scene.h"

#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "assimp/scene.h"

namespace si {

namespace {

void createMesh(Mesh* mesh, aiMesh* impMesh) {
  mesh->vertices.reserve(impMesh->mNumVertices);

  for (U32 i = 0; i < impMesh->mNumVertices; ++i) {
    aiVector3D& impVec = impMesh->mVertices[i];
    mesh->vertices.emplaceBack(impVec.x, impVec.y, impVec.z);
  }
}

}  // namespace

bool loadCollada(Scene* scene, nu::InputStream* stream) {
  Assimp::Importer importer;

  nu::InputStream::SizeType bytesRemaining = stream->getBytesRemaining();

  nu::DynamicArray<U8> buffer;
  buffer.reserve(bytesRemaining);
  stream->read(buffer.getData(), bytesRemaining);

  const aiScene* imp = importer.ReadFileFromMemory(buffer.getData(), buffer.getSize(), 0);

  for (U32 i = 0; i < imp->mRootNode->mNumMeshes; ++i) {
    aiMesh* impMesh = imp->mMeshes[imp->mRootNode->mMeshes[i]];

    scene->meshes.constructBack([impMesh](Mesh* mesh) { createMesh(mesh, impMesh); });
  }

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
