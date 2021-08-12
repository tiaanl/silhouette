#include <nucleus/streams/file_input_stream.h>
#include <silhouette/codec/image/png.h>

#include <catch2/catch.hpp>

namespace si {

TEST_CASE("Image") {
  SECTION("we can load a png from disk") {
    nu::FileInputStream stream{nu::FilePath{__FILE__}.dirName() / "fixtures" / "checker.png"};

    auto maybe_image = load_image_from_png(&stream);
    REQUIRE(maybe_image.has_value());
    CHECK(maybe_image->size() == fl::Size{32, 32});
    CHECK(maybe_image->format() == ImageFormat::RedGreenBlueAlpha);

    RGBA pixel{0, 0, 0, 0};

    CHECK(maybe_image->pixel(0, 0, &pixel));
    CHECK(pixel == RGBA{255, 0, 0, 255});

    CHECK(maybe_image->pixel(1, 0, &pixel));
    CHECK(pixel == RGBA{0, 0, 0, 0});

    CHECK(maybe_image->pixel(0, 1, &pixel));
    CHECK(pixel == RGBA{0, 0, 0, 0});

    CHECK(maybe_image->pixel(1, 1, &pixel));
    CHECK(pixel == RGBA{255, 0, 0, 255});
  }
}

}  // namespace si
