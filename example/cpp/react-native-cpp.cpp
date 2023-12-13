#include "JsiNativeObject.h"

namespace Example {

using namespace RNJsi;

class TestObject : public JsiNativeObject<TestObject> {
public:
  JSI_EXPORT_FUNCTION(TestObject, add) {
    return args[0].asNumber() + args[1].asNumber();
  }
};

class JsiTestObject : public JsiNativeObject<> {

  JSI_EXPORT_FUNCTION(JsiTestObject, getX) { return 22; }

  JSI_EXPORT_FUNCTION(JsiTestObject, getY) { return 10; }
};

struct TestObject2 {
  TestObject2(double x, double y) : x(x), y(y) {}
  double x;
  double y;
};

class JsiStateTestObject : public JsiNativeObject<TestObject2> {
  JSI_INITIALIZE(JsiStateTestObject) {
    auto x = args[0].asNumber();
    auto y = args[1].asNumber();
    make_state(rt, thisValue, x, y);
    return jsi::Value::undefined();
  }

  JSI_EXPORT_FUNCTION(JsiStateTestObject, getX) {
    auto testObject = getState(rt, thisValue);
    return testObject->x;
  }

  JSI_EXPORT_FUNCTION(JsiStateTestObject, getY) {
    auto testObject = getState(rt, thisValue);
    return testObject->y;
  }
};

JSI_EXPORT_MODULE(TestObject, "TestObject")
JSI_EXPORT_MODULE(JsiTestObject, "JsiTestObject")
JSI_EXPORT_MODULE(JsiStateTestObject, "JsiStateTestObject")

} // namespace RNJsi
