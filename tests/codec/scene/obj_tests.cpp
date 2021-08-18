#include <nucleus/streams/file_input_stream.h>
#include <silhouette/codec/scene/obj.h>

#include <catch2/catch.hpp>

namespace si {

TEST_CASE("obj") {
  SECTION("basic") {
    nu::FileInputStream stream{nu::FilePath{__FILE__}.dirName().dirName().dirName() / "fixtures" /
                               "asteroid.obj"};
    auto maybe_scene =
        load_scene_from_obj(&stream, [](nu::StringView name) -> nu::ScopedPtr<nu::InputStream> {
          return nu::make_scoped_ptr<nu::FileInputStream>(
              nu::FilePath{__FILE__}.dirName().dirName().dirName() / "fixtures" / name);
        });
    CHECK(maybe_scene.has_value());
  }
}

}  // namespace si
