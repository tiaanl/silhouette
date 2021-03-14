#include "silhouette/Image.h"

#include "canvas/Renderer/Renderer.h"
#include "floats/Common.h"
#include "nucleus/Logging.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace si {

// static
Image Image::createAlpha(const fl::Size& size, U8 intensity) {
  Image result;

  result.m_format = ImageFormat::Alpha;
  result.m_size = size;
  result.m_data.resize(size.width * size.height, intensity);

  return result;
}

// static
Image Image::createSolid(const fl::Size& size, const ca::Color& color) {
  Image result;

  if (!size.area()) {
    return result;
  }

  result.m_format = ImageFormat::RedGreenBlueAlpha;
  result.m_size = size;
  result.m_data.resize(size.width * size.height * 4);

  struct C {
    U8 r;
    U8 g;
    U8 b;
    U8 a;
  } c = {
      static_cast<U8>(fl::round(255.0f * color.r)),
      static_cast<U8>(fl::round(255.0f * color.g)),
      static_cast<U8>(fl::round(255.0f * color.b)),
      static_cast<U8>(fl::round(255.0f * color.a)),
  };

  C* data = reinterpret_cast<C*>(result.m_data.data());
  std::fill(data, data + (size.width * size.height), c);

  return result;
}

Image::Image() = default;

Image::~Image() = default;

Image::Image(Image&& other) noexcept
  : m_format{other.m_format}, m_size(other.m_size), m_data(std::move(other.m_data)) {}

Image& Image::operator=(Image&& other) noexcept {
  m_format = other.m_format;
  m_size = other.m_size;
  m_data = std::move(other.m_data);

  return *this;
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
      m_format = ImageFormat::RedGreenBlueAlpha;
      break;

    case 3:
      m_format = ImageFormat::RedGreenBlue;
      break;

    case 1:
      m_format = ImageFormat::Alpha;
      break;

    default:
      NOTREACHED();
      break;
  }

  return true;
}

void Image::setPixel(const fl::Pos& pos, const ca::Color& color) {
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

  ca::TextureFormat format = ca::TextureFormat::Unknown;

  switch (image.m_format) {
    case ImageFormat::Alpha:
      format = ca::TextureFormat::Alpha;
      break;

    case ImageFormat::RedGreenBlue:
      format = ca::TextureFormat::RGB;
      break;

    case ImageFormat::RedGreenBlueAlpha:
      format = ca::TextureFormat::RGBA;
      break;
  }

  return renderer->createTexture(format, image.m_size, image.m_data.data(), image.m_data.size(),
                                 smooth);
}

}  // namespace si
