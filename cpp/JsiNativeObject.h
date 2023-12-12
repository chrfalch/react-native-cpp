#pragma once

#import <jsi/jsi.h>

#import "JsiArrayBuffer.h"
#import "JsiNativeModule.h"
#import "JsiNativeState.h"

namespace RNJsi {

namespace jsi = facebook::jsi;

/**
 Defines the base class for an object with state that can be installed in a
 javascript runtime.

 The object will be installed in the runtime with the name given by the module
 name - and can thereafter be instantied from javascript in the following way:
 ```cpp

 class MyCalculator: public JsiNativeObject {
 public:
   JSI_EXPORT_FUNCTION(MyCalculator, add) {
     return args[0].asNumber() + args[1].asNumber();
   }

   JSI_EXPORT_FUNCTION(MyCalculator, sub) {
     return args[0].asNumber() - args[1].asNumber();
   };
 };
 ```

 ```js
 const myNewObject = NewObject.create();
 const result = myNewObject.add(2, 2); // <- 4
 ```
 */
template <typename T> class JsiNativeObject : public JsiNativeModule {
public:
  JSI_EXPORT_FUNCTION(JsiNativeObject, create) {
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

    // Call initializer if it is registered
    if (initializerFunc) {
      initializerFunc(rt, result, args, count);
    }

    // Return our new object
    return jsi::Value(rt, result);
  }

  static void setInitializerFunction(jsi::HostFunctionType func) {
    initializerFunc = func;
  }

private:
  static inline jsi::HostFunctionType initializerFunc = nullptr;
};

// Macro for declaring and registering jsi state initialization.
// The initializer function will be called when the create method is
// called - if the initializer is set - then it will be called with
// the new object as the thisValue, and with the same args as the create
// method.
#define JSI_INITIALIZE(CLASS)                                                  \
  static struct initialize_object_Registrar {                                  \
    initialize_object_Registrar() {                                            \
      CLASS::setInitializerFunction(initialize_object);                        \
    }                                                                          \
  } initialize_object__registrar;                                              \
  static jsi::Value initialize_object(jsi::Runtime &rt,                        \
                                      const jsi::Value &thisValue,             \
                                      const jsi::Value *args, size_t count)

/**
 Implements a JsiNativeObject with state of a given type T. State is stored in a
 TypeArray with a c++ ArrayBuffer with the length and memory needed to store T.
 */
template <typename S>
class JsiNativeObjectWithState
    : public JsiNativeObject<JsiNativeObjectWithState<S>> {
public:
  static S *getState(jsi::Runtime &rt, const jsi::Value &thisValue) {
    // Get this as object
    auto thisObj = thisValue.asObject(rt);

    // Get pointer to data
    auto memAddr = thisObj.getProperty(
        rt, jsi::PropNameID::forAscii(rt, PropNameStateMemAddr,
                                      PropNameStateMemAddrLen));

    return reinterpret_cast<S *>(static_cast<uintptr_t>(memAddr.asNumber()));
  }

  template <class... _Args>
  static S *make_state(jsi::Runtime &rt, const jsi::Value &thisValue,
                       _Args &&...__args) {
    // Get this as object
    auto thisObj = thisValue.asObject(rt);

    // Create state
    auto state =
        std::make_shared<JsiInstanceBuffer<S>>(std::forward<_Args>(__args)...);

    // Set state and pointer to state on thisValue
    thisObj.setProperty(
        rt, jsi::PropNameID::forAscii(rt, PropNameState, PropNameStateLen),
        jsi::ArrayBuffer(rt, state));

    // Set pointer
    auto memAddr = reinterpret_cast<uintptr_t>(state.get());
    thisObj.setProperty(rt,
                        jsi::PropNameID::forAscii(rt, PropNameStateMemAddr,
                                                  PropNameStateMemAddrLen),
                        static_cast<double>(memAddr));

    return reinterpret_cast<S *>(static_cast<uintptr_t>(memAddr));
  }

private:
  // Property names
  static inline const char *PropNameState = "__state__";
  static inline size_t PropNameStateLen = strlen(PropNameState);

  static inline const char *PropNameStateMemAddr = "__state_addr__";
  static inline size_t PropNameStateMemAddrLen = strlen(PropNameStateMemAddr);
};

/**
 Implements a JsiNativeObject with state of a given type T. State is stored
 using the set/getNativeState method of the jsi::Object.
 */
template <typename S>
class JsiNativeObjectWithNativeState
    : public JsiNativeObject<JsiNativeObjectWithNativeState<S>> {
public:
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

struct TestObject {
  TestObject(double x, double y) : x(x), y(y) {}
  double x;
  double y;
};

class JsiStateTestObject : public JsiNativeObjectWithState<TestObject> {
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

class JsiNativeStateTestObject
    : public JsiNativeObjectWithNativeState<TestObject> {
  JSI_INITIALIZE(JsiNativeStateTestObject) {
    auto x = args[0].asNumber();
    auto y = args[1].asNumber();
    make_state(rt, thisValue, x, y);
    return jsi::Value::undefined();
  }

  JSI_EXPORT_FUNCTION(JsiNativeStateTestObject, getX) {
    auto testObject = getState(rt, thisValue);
    return testObject->x;
  }

  JSI_EXPORT_FUNCTION(JsiNativeStateTestObject, getY) {
    auto testObject = getState(rt, thisValue);
    return testObject->y;
  }
};

} // namespace RNJsi
