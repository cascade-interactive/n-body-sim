#pragma once
#include "simMath.hpp"

struct Body {
  Vector3 position{};
  Vector3 velocity{};
  Vector3 acceleration{};
  double mass{};
  double radius{};
};