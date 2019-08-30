#ifndef SILHOUETTE_SCENE_SCENE_H_
#define SILHOUETTE_SCENE_SCENE_H_

#include "nucleus/containers/DynamicArray.h"
#include "silhouette/scene/Mesh.h"

namespace si {

struct Scene {
  nu::DynamicArray<Mesh> meshes;
};

} // namespace si

#endif // SILHOUETTE_SCENE_SCENE_H_
