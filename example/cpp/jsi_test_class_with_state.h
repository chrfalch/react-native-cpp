#pragma once

#include "RNJsi.h"

namespace Example {

using namespace RNJsi;

struct StateTestObject {
  StateTestObject(double x, double y) : x(x), y(y) {}
  double x;
  double y;
};

JSI_CLASS(JsiStateTestClass, StateTestObject) {
  JSI_INITIALIZER(
      JsiStateTestClass, JSI_FUNCTION {
        make_state(rt, thisValue, 22, 22);
        return jsi::Value::undefined();
      });

  JSI_HOST_FUNCTION(
      JsiStateTestClass, getX, JSI_FUNCTION {
        auto testObject = getState(rt, thisValue);
        return testObject->x;
      });

  JSI_HOST_FUNCTION(
      JsiStateTestClass, getY, JSI_FUNCTION {
        auto testObject = getState(rt, thisValue);
        return testObject->y;
      });
};

} // namespace Example
