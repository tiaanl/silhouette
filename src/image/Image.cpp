#include "silhouette/Image/Image.h"

#include "canvas/Math/Common.h"
#include "canvas/Renderer/Renderer.h"
#include "nucleus/Logging.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace si {

// static
Image Image::createAlpha(const ca::Size& size, U8 intensity) {
  Image result;

  result.m_format = ca::TextureFormat::Alpha;
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
    U8* data = m_data.data();
    U8* end = data + m_data.size();
    while (data < end) {
      *data++ = static_cast<U8>(ca::round(col.r * 255.0f));
      *data++ = static_cast<U8>(ca::round(col.g * 255.0f));
      *data++ = static_cast<U8>(ca::round(col.b * 255.0f));
      *data++ = static_cast<U8>(ca::round(col.a * 255.0f));
    }
  } else {
    // Store the size and clear out the scene.
    m_size = {0, 0};
    m_data.clear();
  }
}

bool Image::loadFromStream(nu::InputStream* stream) {
  DCHECK(stream);

  nu::InputStream::SizeType bytesRemaining = stream->getBytesRemaining();

  nu::DynamicArray<U8> buffer;
  buffer.resize(bytesRemaining);
  stream->read(buffer.data(), buffer.size());

  I32 x = 0, y = 0, channels = 0;
  stbi_uc* result =
      stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size()), &x, &y, &channels, 4);

  MemSize size = 4 * x * y;
  m_data.resize(size);
  std::memcpy(m_data.data(), result, size);

  stbi_image_free(result);

  m_size = {x, y};

  switch (channels) {
    case 4:
      m_format = ca::TextureFormat::RGBA;
      break;

    case 3:
      m_format = ca::TextureFormat::RGB;
      break;

    case 1:
      m_format = ca::TextureFormat::Alpha;
      break;

    default:
      NOTREACHED();
      break;
  }

  return true;
}

void Image::setPixel(const ca::Pos& pos, const ca::Color& color) {
  U8* ptr = &m_data[pos.y * (m_size.width * 4) + (pos.x * 4)];
  *ptr++ = static_cast<U8>(round(color.r));
  *ptr++ = static_cast<U8>(round(color.g));
  *ptr++ = static_cast<U8>(round(color.b));
  *ptr = static_cast<U8>(round(color.a));
}

ca::TextureId createTextureFromImage(ca::Renderer* renderer, const Image& image, bool smooth) {
#if 0
  LOG(Info) << "Creating texture: format = " << (U32)image.m_format << ", size = " << image.m_size;
#endif  // 0

  return renderer->createTexture(image.m_format, image.m_size, image.m_data.data(),
                                 image.m_data.size(), smooth);
}

}  // namespace si
