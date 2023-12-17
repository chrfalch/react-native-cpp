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
template <typename T, typename S = uintptr_t>
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
    if (*getInitializer()) {
      (*getInitializer())(rt, jsi::Value(rt, result), args, count);
    }

    // Return our new object
    return jsi::Value(rt, result);
  }

  /**
   * Sets the jsi::HostFunctionType that should be called for newly created
   * objects.
   * @param initializerFunction Function to store as initializer for this
   * specalisation.
   */
  static void
  setInitializerFunction(const jsi::HostFunctionType &initializerFunction) {
    *getInitializer() = initializerFunction;
  }

  static S *getState(jsi::Runtime &rt, const jsi::Value &thisValue) {
    auto thisObj = thisValue.asObject(rt);
    if (thisObj.hasNativeState(rt)) {
      return &thisObj.getNativeState<JsiNativeState<S>>(rt)->getValue();
    }
    return nullptr;
  }

protected:
  static S *setState(jsi::Runtime &rt, const jsi::Value &thisValue,
                     const S &state) {
    auto thisObj = thisValue.asObject(rt);
    thisObj.setNativeState(
        rt, std::make_shared<JsiNativeState<S>>(std::move(state)));
    // TODO: when hermes supports setExternalMemoryPressure we need
    // to add sizeof(T) + any internal stuff - maybe create some kind of
    // api for this.
    return &state->getValue();
  }

  template <class... _Args>
  static S *make_state(jsi::Runtime &rt, const jsi::Value &thisValue,
                       _Args &&...__args) {
    auto thisObj = thisValue.asObject(rt);
    auto state =
        std::make_shared<JsiNativeState<S>>(std::forward<_Args>(__args)...);
    thisObj.setNativeState(rt, state);
    // TODO: when hermes supports setExternalMemoryPressure we need
    // to add sizeof(T) + any internal stuff - maybe create some kind of
    // api for this.
    return &state->getValue();
  }

  static jsi::HostFunctionType *getInitializer() {
    static jsi::HostFunctionType initializer = nullptr;
    return &initializer;
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

/**
 * Implements a simple utility struct for creating static registrars for
 * initializer function
 * @tparam T Class to register initializer for
 */
template <typename T> struct JsiInitializerRegistrar {
  JsiInitializerRegistrar(const jsi::HostFunctionType &func) {
    T::setInitializerFunction(func);
  }
};

#define JSI_INITIALIZER(CLASS, FUNC)                                           \
  [[maybe_unused]] static inline JsiInitializerRegistrar<CLASS>                \
      initializer_registrar__ = JsiInitializerRegistrar<CLASS>(FUNC);

} // namespace RNJsi
