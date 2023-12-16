#include "RNJsi.h"

namespace Example {

using namespace RNJsi;

/*class TestObject : public JsiNativeObject<TestObject> {
public:
  JSI_FUNCTION(add) { return args[0].asNumber() + args[1].asNumber(); }
};

JSI_EXPORT_MODULE(TestObject, "TestObject")

class JsiTestObject : public JsiNativeObject<JsiTestObject> {
  JSI_FUNCTION(getX) { return 22; }
  JSI_FUNCTION(getY) { return 10; }
};

JSI_EXPORT_MODULE(JsiTestObject, "JsiTestObject")

struct StateTestObject {
  StateTestObject(double x, double y) : x(x), y(y) {}
  double x;
  double y;
};

class JsiStateTestObject
    : public JsiNativeObject<JsiStateTestObject, StateTestObject> {
  JSI_INITIALIZE(JsiStateTestObject) {
    auto x = args[0].asNumber();
    auto y = args[1].asNumber();
    make_state(rt, thisValue, x, y);
    return jsi::Value::undefined();
  }

  JSI_FUNCTION(getX) {
    auto testObject = getState(rt, thisValue);
    return testObject->x;
  }

  JSI_FUNCTION(getY) {
    auto testObject = getState(rt, thisValue);
    return testObject->y;
  }
};

JSI_EXPORT_MODULE(JsiStateTestObject, "JsiStateTestObject")
*/
} // namespace Example
