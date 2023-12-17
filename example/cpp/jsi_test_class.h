#pragma once

#include "RNJsi.h"

namespace Example {

using namespace RNJsi;

JSI_CLASS(JsiTestClass) {
  JSI_HOST_FUNCTION(
      JsiTestClass, getX, JSI_FUNCTION { return 22; });
  JSI_HOST_FUNCTION(
      JsiTestClass, getY, JSI_FUNCTION { return 10; });
};

} // namespace Example
