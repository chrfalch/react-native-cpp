#include "test_module.h"

namespace Example {

using namespace RNJsi;

jsi::Value TestModule::getX_function(jsi::Runtime &rt,
                                    const jsi::Value &thisValue,
                                    const jsi::Value *args, size_t count) {
  return 22;
}

} // namespace Example
