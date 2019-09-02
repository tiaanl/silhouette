#ifndef SILHOUETTE_IMAGE_IMAGE_H_
#define SILHOUETTE_IMAGE_IMAGE_H_

#include "canvas/Renderer/Types.h"
#include "canvas/Utils/Color.h"
#include "canvas/Utils/Pos.h"
#include "canvas/Utils/Size.h"
#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Streams/InputStream.h"

namespace ca {

class Renderer;

}  // namespace ca

namespace si {

class Image {
public:
  // Create a blank alpha image.
  static Image createAlpha(const ca::Size& size, U8 intensity = 0);

  Image();
  Image(Image&& other);
  ~Image();

  Image& operator=(Image&& other);

  ca::TextureFormat format() const {
    return m_format;
  }

  // Get the size of the image.
  const ca::Size& size() const {
    return m_size;
  }

  // Get the pixel scene for the image.
  U8* data() const {
    return const_cast<U8*>(m_data.data());
  }

  // The size in bytes of the stored image data.
  MemSize dataSize() const {
    return m_data.size();
  }

  // Create a blank image with the specified color.
  void create(const ca::Size& size, const ca::Color& col = ca::Color::black);

  // Load the image scene from a stream.
  bool loadFromStream(nu::InputStream* stream);

  // Set the color of a single pixel in the image.
  void setPixel(const ca::Pos& pos, const ca::Color& color);

private:
  DELETE_COPY(Image);

  friend ca::TextureId createTextureFromImage(ca::Renderer*, const Image&, bool);

  // Format of the scene is stored in.
  ca::TextureFormat m_format = ca::TextureFormat::Unknown;

  // The dimensions of the image.
  ca::Size m_size;

  // The buffer that holds the pixel scene.
  nu::DynamicArray<U8> m_data;
};

ca::TextureId createTextureFromImage(ca::Renderer* renderer, const Image& image,
                                     bool smooth = false);

}  // namespace si

#endif  // SILHOUETTE_IMAGE_IMAGE_H_
