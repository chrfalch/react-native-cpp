#pragma once

#include "RNJsi.h"

namespace Example {

using namespace RNJsi;

JSI_CLASS(JsiTestObject) {
  JSI_HOST_FUNCTION(
      JsiTestObject, getX, JSI_FUNCTION { return 22; });
  JSI_HOST_FUNCTION(
      JsiTestObject, getY, JSI_FUNCTION { return 10; });
};

} // namespace Example
