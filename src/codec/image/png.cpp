#include "silhouette/codec/image/png.h"

#include "../../ext/stb_image.h"

namespace si {

nu::Optional<Image> load_image_from_png(nu::InputStream* stream) {
  DCHECK(stream);

  nu::InputStream::SizeType bytes_remaining = stream->getBytesRemaining();

  nu::DynamicArray<U8> buffer;
  buffer.resize(bytes_remaining);
  stream->read(buffer.data(), buffer.size());

  I32 x = 0, y = 0, channels = 0;
  stbi_uc* result =
      stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size()), &x, &y, &channels, 4);

  MemSize data_size = x * y * channels;
  auto data = nu::DynamicArray<U8>::withInitialSize(data_size);
  std::memcpy(data.data(), result, data_size);

  stbi_image_free(result);

  ImageFormat format = ImageFormat::RedGreenBlueAlpha;
  if (channels == 4) {
  } else if (channels == 3) {
    format = ImageFormat::RedGreenBlue;
  } else if (channels == 1) {
    format = ImageFormat::Alpha;
  } else {
    LOG(Error) << "Invalid channel count for image.";
    NOTREACHED();
    return {};
  }

  return Image::create_from_raw({x, y}, format, std::move(data));
}

}  // namespace si
