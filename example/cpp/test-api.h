#include "RNJsi.h"

namespace Example {

using namespace RNJsi;

class TestObject : public JsiNativeModule<TestObject> {
public:
  JSI_HOST_FUNCTION(TestObject, add,
                    { return args[0].asNumber() + args[1].asNumber(); });
};

JSI_EXPORT_MODULE(TestObject, "TestObject")

class JsiTestObject : public JsiNativeObject<JsiTestObject> {
  JSI_HOST_FUNCTION(JsiTestObject, getX, { return 22; });
  JSI_HOST_FUNCTION(JsiTestObject, getY, { return 10; });
};

JSI_EXPORT_OBJECT(JsiTestObject, "JsiTestObject")

struct StateTestObject {
  StateTestObject(double x, double y) : x(x), y(y) {}
  double x;
  double y;
};

class JsiStateTestObject
    : public JsiNativeObject<JsiStateTestObject, StateTestObject> {
  JSI_INITIALISER(JsiStateTestObject, {
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
