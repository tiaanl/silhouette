#include <nucleus/Streams/FileInputStream.h>

#include <catch2/catch.hpp>

#include "silhouette/scene.h"

namespace si {

TEST_CASE("Scene") {
  SECTION("we can load a collada scene from disk") {
    nu::FileInputStream stream{nu::FilePath{__FILE__}.dirName() / "fixtures" / "box.dae"};
    Scene scene;
    REQUIRE(scene.load_from_collada(&stream) == true);
    CHECK(scene.meshes().size() == 2);
    CHECK(scene.materials().size() == 1);
    CHECK(scene.root_node().children.size() == 1);
  }
}

}  // namespace si
