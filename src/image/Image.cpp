#include "silhouette/image/Image.h"

#include "nucleus/Logging.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace si {

// static
Image Image::createAlpha(const ca::Size& size, U8 intensity) {
  Image result;

  result.m_format = ImageFormat::Alpha;
  result.m_size = size;
  result.m_data.resize(size.width * size.height, intensity);

  return result;
}

Image::Image() = default;

Image::~Image() = default;

Image::Image(Image&& other)
  : m_format{other.m_format}, m_size(other.m_size), m_data(std::move(other.m_data)) {}

Image& Image::operator=(Image&& other) {
  m_format = other.m_format;
  m_size = other.m_size;
  m_data = std::move(other.m_data);

  return *this;
}

void Image::create(const ca::Size& size, const ca::Color& col) {
  if (size.width && size.height) {
    // Store the size of the image.
    m_size = size;

    // Resize the buffer to hold the image.
    auto bufferSize = size.width * size.height * 4;
    m_data.resize(bufferSize);

    // Fill the image with the specified color.
    U8* data = m_data.getData();
    U8* end = data + m_data.getSize();
    while (data < end) {
      *data++ = static_cast<U8>(std::round(col.r * 255.0f));
      *data++ = static_cast<U8>(std::round(col.g * 255.0f));
      *data++ = static_cast<U8>(std::round(col.b * 255.0f));
      *data++ = static_cast<U8>(std::round(col.a * 255.0f));
    }
  } else {
    // Store the size and clear out the scene.
    m_size = {0, 0};
    m_data.clear();
  }
}

bool Image::loadFromStream(nu::InputStream* stream) {
  DCHECK(stream);

  nu::DynamicArray<U8> buffer;
  buffer.reserve(stream->getBytesRemaining());
  stream->read(buffer.getData(), buffer.getSize());

  I32 x = 0, y = 0, channels = 0;
  stbi_uc* result = stbi_load_from_memory(buffer.getData(), buffer.getSize(), &x, &y, &channels, 4);

  MemSize size = channels * x * y;
  m_data.reserve(size);
  std::memcpy(m_data.getData(), result, size);

  stbi_image_free(result);

  return false;
}

void Image::setPixel(const ca::Pos& pos, const ca::Color& color) {
  U8* ptr = &m_data[pos.y * (m_size.width * 4) + (pos.x * 4)];
  *ptr++ = static_cast<U8>(round(color.r));
  *ptr++ = static_cast<U8>(round(color.g));
  *ptr++ = static_cast<U8>(round(color.b));
  *ptr++ = static_cast<U8>(round(color.a));
}

}  // namespace si
