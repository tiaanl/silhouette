#ifndef SILHOUETTE_SCENE_SCENE_H_
#define SILHOUETTE_SCENE_SCENE_H_

#include "canvas/Math/Mat4.h"
#include "canvas/Math/Vec3.h"
#include "nucleus/Containers/DynamicArray.h"

namespace si {

struct Mesh {
  nu::DynamicArray<ca::Vec3> vertices;
  nu::DynamicArray<U16> indices;
};

struct Node {
  ca::Mat4 transform{ca::Mat4::identity};
  nu::DynamicArray<MemSize> meshIndices;
  nu::DynamicArray<Node> children;
};

struct Scene {
  nu::DynamicArray<Mesh> meshes;
  Node rootNode;
};

}  // namespace si

#endif  // SILHOUETTE_SCENE_SCENE_H_
