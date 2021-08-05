#pragma once

#include <nucleus/optional.h>
#include <nucleus/streams/input_stream.h>
#include <silhouette/image.h>

namespace si {

nu::Optional<Image> load_image_from_png(nu::InputStream* stream);

}  // namespace si
