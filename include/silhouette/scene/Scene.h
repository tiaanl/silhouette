#ifndef SILHOUETTE_SCENE_SCENE_H_
#define SILHOUETTE_SCENE_SCENE_H_

#include "canvas/Math/Mat4.h"
#include "canvas/Math/Vec3.h"
#include "canvas/Utils/Color.h"
#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Text/StaticString.h"

namespace si {

struct Mesh {
  MemSize materialIndex;
  nu::DynamicArray<ca::Vec3> positions;
  nu::DynamicArray<ca::Vec2> texCoords;
  nu::DynamicArray<U16> indices;
};

struct Node {
  ca::Mat4 transform{ca::Mat4::identity};
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
