#pragma once

#include "RNJsi.h"

namespace Example {

using namespace RNJsi;

class TestObject : public JsiNativeModule<TestObject> {
public:
  static jsi::Value add_function(jsi::Runtime &rt, const jsi::Value &thisValue,
                          const jsi::Value *args, size_t count);

  JSI_HOST_FUNCTION(TestObject, add, &TestObject::add_function);
};

JSI_EXPORT_MODULE(TestObject, "TestObject")

} // namespace Example
