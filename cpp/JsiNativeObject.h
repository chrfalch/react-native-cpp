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
class JsiNativeObject : public JsiNativeModule<JsiNativeObject<T, S>> {
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

    // Call initializer if it exists
    if (*getInitialiser()) {
      (*getInitialiser())(rt, jsi::Value(rt, result), args, count);
    }

    // Return our new object
    return jsi::Value(rt, result);
  }

  /**
   * Sets the jsi::HostFunctionType that should be called for newly created
   * objects.
   * @param initialiserFunction Function to store as initialiser for this
   * specalisation.
   */
  static void
  setInitialiserFunction(const jsi::HostFunctionType &initialiserFunction) {
    *getInitialiser() = initialiserFunction;
  }

  static S *getState(jsi::Runtime &rt, const jsi::Value &thisValue) {
    auto thisObj = thisValue.asObject(rt);
    if (thisObj.hasNativeState(rt)) {
      return &thisObj.getNativeState<JsiNativeState<S>>(rt)->getValue();
    }
    return nullptr;
  }

protected:
  template <class... _Args>
  static S *make_state(jsi::Runtime &rt, const jsi::Value &thisValue,
                       _Args &&...__args) {
    auto thisObj = thisValue.asObject(rt);
    auto state =
        std::make_shared<JsiNativeState<S>>(std::forward<_Args>(__args)...);
    thisObj.setNativeState(rt, state);
    return &state->getValue();
  }

  static jsi::HostFunctionType *getInitialiser() {
    static jsi::HostFunctionType initialiser = nullptr;
    return &initialiser;
  }
};

/**
 Implements a utility for creating static registrars for objects
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

#define JSI_EXPORT_OBJECT(CLASS, EXPORT_NAME)                                  \
  static JsiNativeObjectRegistrar<CLASS> CLASS##_METHOD##_registrar(           \
      EXPORT_NAME);

#define JSI_INITIALISER(CLASS, FUNC)                                           \
  static inline struct initialiser_registrar {                                 \
    initialiser_registrar() {                                                  \
      CLASS::setInitialiserFunction(                                           \
          [](jsi::Runtime & rt, const jsi::Value &thisValue,                   \
             const jsi::Value *args, size_t count) FUNC);                      \
    }                                                                          \
  } initialiser_registrar__;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// TESTS
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

class MyJsiTestClass : public JsiNativeObject<MyJsiTestClass> {
public:
  JSI_HOST_FUNCTION(MyJsiTestClass, return_100, { return 100; });
};

JSI_EXPORT_OBJECT(MyJsiTestClass, "__JsiTestObject")

struct TestState {
  TestState(double x, double y) : x(x), y(y) {}
  double x = 0;
  double y = 0;
};

class MyJsiStateTestClass
    : public JsiNativeObject<MyJsiStateTestClass, TestState> {
public:
  JSI_INITIALISER(MyJsiStateTestClass, {
    make_state(rt, thisValue, args[0].asNumber(), args[1].asNumber());
    return jsi::Value::undefined();
  });

  JSI_HOST_FUNCTION(MyJsiStateTestClass, area, {
    auto s = getState(rt, thisValue);
    if (s != nullptr) {
      return s->x * s->y;
    }
    return 0.0;
  });
};

JSI_EXPORT_OBJECT(MyJsiStateTestClass, "__JsiStateObject")

} // namespace RNJsi
