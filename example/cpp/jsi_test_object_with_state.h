#pragma once

#include "RNJsi.h"

namespace Example {

using namespace RNJsi;

struct StateTestObject {
  StateTestObject(double x, double y) : x(x), y(y) {}
  double x;
  double y;
};

JSI_CLASS(JsiStateTestObject, StateTestObject) {
  JSI_INITIALIZER(
      JsiStateTestObject, JSI_FUNCTION {
        make_state(rt, thisValue, args[0].asNumber(), args[1].asNumber());
        return jsi::Value::undefined();
      });

  JSI_HOST_FUNCTION(
      JsiStateTestObject, getX, JSI_FUNCTION {
        auto testObject = getState(rt, thisValue);
        return testObject->x;
      });

  JSI_HOST_FUNCTION(
      JsiStateTestObject, getY, JSI_FUNCTION {
        auto testObject = getState(rt, thisValue);
        return testObject->y;
      });
};

} // namespace Example
