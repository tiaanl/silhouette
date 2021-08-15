#pragma once

#include <floats/mat4.h>
#include <floats/vec3.h>
#include <nucleus/containers/dynamic_array.h>
#include <nucleus/text/dynamic_string.h>

#include "silhouette/color.h"

namespace nu {
class InputStream;
}  // namespace nu

namespace si {

struct Mesh {
  nu::DynamicString name;
  MemSize material_index = 0;
  nu::DynamicArray<fl::Vec3> positions;
  nu::DynamicArray<fl::Vec3> normals;
  nu::DynamicArray<fl::Vec2> tex_coords;

  explicit Mesh(nu::StringView name) : name{name} {}
};

struct Node {
  fl::Mat4 transform{fl::Mat4::identity};
  nu::DynamicArray<MemSize> meshIndices;
  nu::DynamicArray<Node> children;
};

struct Material {
  struct {
    RGBA color;
    nu::DynamicString texture;
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

private:
  nu::DynamicArray<Mesh> meshes_;
  nu::DynamicArray<Material> materials_;
  Node root_node_;
};

}  // namespace si
