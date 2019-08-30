#ifndef SILHOUETTE_IMAGE_IMAGE_H_
#define SILHOUETTE_IMAGE_IMAGE_H_

#include "canvas/Utils/Color.h"
#include "canvas/Utils/Pos.h"
#include "canvas/Utils/Size.h"
#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Streams/InputStream.h"

namespace si {

enum class ImageFormat : U32 {
  Unknown,
  Alpha,
  RGB,
  RGBA,
};

class Image {
public:
  // Create a blank alpha image.
  static Image createAlpha(const ca::Size& size, U8 intensity = 0);

  Image();
  Image(Image&& other);
  ~Image();

  Image& operator=(Image&& other);

  ImageFormat getFormat() const {
    return m_format;
  }

  // Get the size of the image.
  const ca::Size& getSize() const {
    return m_size;
  }

  // Get the pixel scene for the image.
  U8* getData() const {
    return const_cast<U8*>(m_data.getData());
  }
  // Create a blank image with the specified color.
  void create(const ca::Size& size, const ca::Color& col = ca::Color::black);

  // Load the image scene from a stream.
  bool loadFromStream(nu::InputStream* stream);

  // Set the color of a single pixel in the image.
  void setPixel(const ca::Pos& pos, const ca::Color& color);

private:
  DELETE_COPY(Image);

  // Format of the scene is stored in.
  ImageFormat m_format = ImageFormat::Unknown;

  // The dimensions of the image.
  ca::Size m_size;

  // The buffer that holds the pixel scene.
  nu::DynamicArray<U8> m_data;
};

}  // namespace si

#endif  // SILHOUETTE_IMAGE_IMAGE_H_
