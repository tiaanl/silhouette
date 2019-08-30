#ifndef SILHOUETTE_SCENE_MESH_H_
#define SILHOUETTE_SCENE_MESH_H_

#include "canvas/Math/Vec3.h"

namespace si {

struct Mesh {
  nu::DynamicArray<ca::Vec3> vertices;
};

}  // namespace si

#endif  // SILHOUETTE_SCENE_MESH_H_
