#include "test_module.h"

namespace Example {

using namespace RNJsi;

jsi::Value TestModule::add_function(jsi::Runtime &rt,
                                    const jsi::Value &thisValue,
                                    const jsi::Value *args, size_t count) {
  return args[0].asNumber() + args[1].asNumber();
}

} // namespace Example
