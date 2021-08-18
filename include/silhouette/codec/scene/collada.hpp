#pragma once

#include <nucleus/streams/input_stream.h>

#include <nucleus/optional.hpp>

#include "silhouette/scene.h"

namespace si {

nu::Optional<Scene> load_scene_from_collada(nu::InputStream* stream);

}  // namespace si
