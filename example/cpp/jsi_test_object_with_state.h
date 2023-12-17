#include "RNJsi.h"

namespace Example {

using namespace RNJsi;

struct StateTestObject {
  StateTestObject(double x, double y) : x(x), y(y) {}
  double x;
  double y;
};

class JsiStateTestObject
    : public JsiNativeObject<JsiStateTestObject, StateTestObject> {
    JSI_INITIALIZER(JsiStateTestObject, {
      make_state(rt, thisValue, args[0].asNumber(), args[1].asNumber());
      return jsi::Value::undefined();
    });

  JSI_HOST_FUNCTION(JsiStateTestObject, getX, {
    auto testObject = getState(rt, thisValue);
    return testObject->x;
  });

  JSI_HOST_FUNCTION(JsiStateTestObject, getY, {
    auto testObject = getState(rt, thisValue);
    return testObject->y;
  });
};

JSI_EXPORT_OBJECT(JsiStateTestObject, "JsiStateTestObject")

} // namespace Example
