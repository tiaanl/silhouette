#include "silhouette/image.h"

#include "nucleus/logging.h"

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

bool Image::load_from_png(nu::InputStream* stream) {
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
