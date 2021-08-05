#include "silhouette/image.h"

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
Image Image::createSolid(const fl::Size& size, const RGBA& color) {
  Image result;

  if (!size.area()) {
    return result;
  }

  result.m_format = ImageFormat::RedGreenBlueAlpha;
  result.m_size = size;
  result.m_data.resize(size.width * size.height * 4);

  RGBA* data = reinterpret_cast<RGBA*>(result.m_data.data());
  std::fill(data, data + (size.width * size.height), color);

  return result;
}

// static
Image Image::create_from_raw(const fl::Size& size, ImageFormat format, nu::DynamicArray<U8> data) {
  Image result;

  result.m_size = size;
  result.m_format = format;
  result.m_data = std::move(data);

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

bool Image::pixel(I32 x, I32 y, RGBA* output) {
  if (x >= 0 && x < m_size.width && y >= 0 && y < m_size.height) {
    *output = *(RGBA*)&m_data[(y * m_size.width + x) * sizeof(RGBA)];
    return true;
  }

  return false;
}

void Image::setPixel(const fl::Pos& pos, const RGBA& color) {
  U8* ptr = &m_data[pos.y * (m_size.width * 4) + (pos.x * 4)];
  *ptr++ = color.red;
  *ptr++ = color.green;
  *ptr++ = color.blue;
  *ptr = color.alpha;
}

#if 0
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
#endif  // 0

}  // namespace si
