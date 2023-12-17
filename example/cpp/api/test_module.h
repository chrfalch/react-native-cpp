#pragma once

#include "RNJsi.h"

namespace Example {

using namespace RNJsi;

JSI_MODULE(TestModule) {
public:
  static jsi::Value getX_function(jsi::Runtime & rt, const jsi::Value &thisValue,
                                  const jsi::Value *args, size_t count);

  JSI_HOST_FUNCTION(TestModule, getX, &TestModule::getX_function);
};

} // namespace Example
