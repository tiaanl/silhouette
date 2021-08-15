#pragma once

#include <nucleus/optional.h>
#include <nucleus/streams/input_stream.h>

#include "silhouette/scene.h"

namespace si {

nu::Optional<Scene> load_scene_from_collada(nu::InputStream* stream);

}  // namespace si
