#pragma once

#include <nucleus/streams/input_stream.h>
#include <silhouette/image.h>

#include <nucleus/optional.hpp>

namespace si {

nu::Optional<Image> load_image_from_png(nu::InputStream* stream);

}  // namespace si
