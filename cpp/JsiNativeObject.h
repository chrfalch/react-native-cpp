#pragma once

#import <jsi/jsi.h>

#import "JsiModuleRegistry.h"
#import "JsiNativeModule.h"
#import "JsiNativeState.h"

namespace RNJsi {

namespace jsi = facebook::jsi;

/**
 Defines the base class for an object with state that can be installed in a
 javascript runtime.
 */
template <typename T, typename S = void>
class JsiNativeObject : public JsiNativeModule<JsiNativeObject<T>> {
public:
  /**
   * Factory HostFunction for creating new objects with the correct
   * prototype.
   * @param rt Runtime
   * @param thisValue This
   * @param args Arguments
   * @param count Number of arguments
   * @return A new jsi::Object with a valid prototype
   */
  static jsi::Value create(jsi::Runtime &rt, const jsi::Value &thisValue,
                           const jsi::Value *args, size_t count) {
    // Create our object
    auto result = jsi::Object(rt);

    // Property names
    static const char *PropNamePrototype = "__proto__";
    static size_t PropNamePrototypeLen = strlen(PropNamePrototype);

    // Set prototype - we'll set ourselves as the prototype since
    // we can then define methods on ourselves later
    result.setProperty(
        rt,
        jsi::PropNameID::forAscii(rt, PropNamePrototype, PropNamePrototypeLen),
        thisValue);

    // Return our new object
    return jsi::Value(rt, result);
  }

protected:
  static S *getState(jsi::Runtime &rt, const jsi::Value &thisValue) {
    auto thisObj = thisValue.asObject(rt);
    if (thisObj.hasNativeState(rt)) {
      return &thisObj.getNativeState<JsiNativeStateWrapper<S>>(rt)->getValue();
    }
    return nullptr;
  }

  template <class... _Args>
  static S *make_state(jsi::Runtime &rt, const jsi::Value &thisValue,
                       _Args &&...__args) {
    auto thisObj = thisValue.asObject(rt);
    auto state = std::make_shared<JsiNativeStateWrapper<S>>(
        std::forward<_Args>(__args)...);
    thisObj.setNativeState(rt, state);
    return &state->getValue();
  }
};

/**
 Implements a simple class for creating static registrars for objects
 */
template <typename T> struct JsiNativeObjectRegistrar {
  JsiNativeObjectRegistrar(std::string exportName) {
    // Register module with module registry
    RNJsi::JsiModuleRegistry::getInstance().registerModule(
        exportName,
        std::bind(&T::install, std::placeholders::_1, exportName, nullptr));

    // Register the factory method
    T::registerHostFunction("create", &T::create);
  }
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// TESTS
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

class MyJsiTestClass : public JsiNativeObject<MyJsiTestClass> {
public:
  static inline struct return_100
      : public JsiHostFunctionRegistrar<MyJsiTestClass> {
    return_100()
        : JsiHostFunctionRegistrar<MyJsiTestClass>(
              "return_100",
              [](jsi::Runtime &rt, const jsi::Value &thisValue,
                 const jsi::Value *args, size_t count) { return 100; }) {}
  } __return_100;
};

static JsiNativeObjectRegistrar<MyJsiTestClass>
    myJsiTestClass_registrar("__TEST__");

} // namespace RNJsi
