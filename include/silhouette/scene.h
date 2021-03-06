#pragma once

#include "floats/mat4.h"
#include "floats/vec3.h"
#include "nucleus/containers/dynamic_array.h"
#include "nucleus/text/static_string.h"
#include "silhouette/color.h"

namespace nu {
class InputStream;
}  // namespace nu

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
    RGBA color;
    nu::StaticString<128> texture;
  } diffuse;
};

class Scene {
public:
  Scene() = default;

  nu::DynamicArray<Mesh>& meshes() {
    return meshes_;
  }

  nu::DynamicArray<Material>& materials() {
    return materials_;
  }

  Node& root_node() {
    return root_node_;
  }

  bool load_from_collada(nu::InputStream* stream);

private:
  nu::DynamicArray<Mesh> meshes_;
  nu::DynamicArray<Material> materials_;
  Node root_node_;
};

}  // namespace si
