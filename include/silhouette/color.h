#pragma once

namespace si {

struct RGB {
  U8 red;
  U8 green;
  U8 blue;
};

struct RGBA {
  U8 red;
  U8 green;
  U8 blue;
  U8 alpha;
};

inline bool operator==(const RGBA& left, const RGBA& right) {
  return left.red == right.red && left.green == right.green && left.blue == right.blue &&
         left.alpha == right.alpha;
}

}  // namespace si
