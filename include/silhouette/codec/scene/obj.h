#pragma once

#include <nucleus/function.h>
#include <nucleus/streams/input_stream.h>
#include <silhouette/scene.h>

#include <nucleus/optional.hpp>

namespace si {

using MaterialLoaderFunc = nu::Function<nu::ScopedPtr<nu::InputStream>(nu::StringView)>;

nu::Optional<Scene> load_scene_from_obj(nu::InputStream* stream,
                                        MaterialLoaderFunc&& material_loader_func);

}  // namespace si
