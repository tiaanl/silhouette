#ifndef SILHOUETTE_IMAGE_IMAGE_H_
#define SILHOUETTE_IMAGE_IMAGE_H_

#include "canvas/Renderer/Types.h"
#include "canvas/Utils/Color.h"
#include "floats/Pos.h"
#include "floats/Size.h"
#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Streams/InputStream.h"

namespace ca {

class Renderer;

}  // namespace ca

namespace si {

enum class ImageFormat {
  Alpha,
  RedGreenBlue,
  RedGreenBlueAlpha,
};

class Image {
public:
  NU_DELETE_COPY(Image);

  // Create a blank alpha image.
  static Image createAlpha(const fl::Size& size, U8 intensity = 0);

  // Create an image with a solid color.
  static Image createSolid(const fl::Size& size, const ca::Color& color);

  Image();
  Image(Image&& other) noexcept;
  ~Image();

  Image& operator=(Image&& other) noexcept;

  NU_NO_DISCARD ImageFormat format() const {
    return m_format;
  }

  NU_NO_DISCARD fl::Size size() const {
    return m_size;
  }

  NU_NO_DISCARD nu::ArrayView<U8> data() const {
    return m_data.view();
  }

  // Load the image Scene from a stream.
  bool loadFromStream(nu::InputStream* stream);

  // Set the color of a single pixel in the image.
  void setPixel(const fl::Pos& pos, const ca::Color& color);

private:
  friend ca::TextureId createTextureFromImage(ca::Renderer*, const Image&, bool);

  // The format that the data is stored in.
  ImageFormat m_format;

  // The dimensions of the image.
  fl::Size m_size;

  // The buffer that holds the pixel data.
  nu::DynamicArray<U8> m_data;
};

ca::TextureId createTextureFromImage(ca::Renderer* renderer, const Image& image,
                                     bool smooth = false);

}  // namespace si

#endif  // SILHOUETTE_IMAGE_IMAGE_H_
