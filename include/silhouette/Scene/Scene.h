#ifndef SILHOUETTE_SCENE_SCENE_H_
#define SILHOUETTE_SCENE_SCENE_H_

#include "canvas/Utils/Color.h"
#include "floats/Mat4.h"
#include "floats/Vec3.h"
#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Text/StaticString.h"

namespace si {

struct Mesh {
  MemSize materialIndex;
  nu::DynamicArray<fl::Vec3> positions;
  nu::DynamicArray<fl::Vec2> texCoords;
};

struct Node {
  fl::Mat4 transform{fl::Mat4::identity};
  nu::DynamicArray<MemSize> meshIndices;
  nu::DynamicArray<Node> children;
};

struct Material {
  struct {
    ca::Color color;
    nu::StaticString<128> texture;
  } diffuse;
};

struct Scene {
  nu::DynamicArray<Mesh> meshes;
  nu::DynamicArray<Material> materials;
  Node rootNode;
};

}  // namespace si

#endif  // SILHOUETTE_SCENE_SCENE_H_
