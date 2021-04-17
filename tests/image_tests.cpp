#include <nucleus/Streams/FileInputStream.h>

#include <catch2/catch.hpp>

#include "silhouette/image.h"

namespace si {

TEST_CASE("Image") {
  SECTION("we can load a png from disk") {
    nu::FileInputStream stream{nu::FilePath{__FILE__}.dirName() / "fixtures" / "checker.png"};
    Image image;
    REQUIRE(image.load_from_png(&stream) == true);
    CHECK(image.size() == fl::Size{32, 32});
    CHECK(image.format() == ImageFormat::RedGreenBlueAlpha);

    RGBA pixel{0, 0, 0, 0};

    CHECK(image.pixel(0, 0, &pixel));
    CHECK(pixel == RGBA{255, 0, 0, 255});

    CHECK(image.pixel(1, 0, &pixel));
    CHECK(pixel == RGBA{0, 0, 0, 0});

    CHECK(image.pixel(0, 1, &pixel));
    CHECK(pixel == RGBA{0, 0, 0, 0});

    CHECK(image.pixel(1, 1, &pixel));
    CHECK(pixel == RGBA{255, 0, 0, 255});
  }
}

}  // namespace si
