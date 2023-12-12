#pragma once

#import <map>

#import <jsi/jsi.h>

#import "JsiModuleRegistry.h"

namespace RNJsi {

namespace jsi = facebook::jsi;

/**
 Defines the base class for a C++ backed module that can be installed in a
 Javascript runtime.

 A native module is a module that can expose functions and values but does
 not have state - ie. you cannot call this inside of the function it exposes.
 This is done to make it possible to expose performant JSI modules.

  Creating subclasses of the JsiNativeModule is done in CPP like the following:

  ```cpp
  class CalculatorModule : public JsiNativeModule {
  public:
   JSI_EXPORT_FUNCTION(CalculatorModule, add) {
     return args[0].asNumber() + args[1].asNumber();
   }
  };
  ```
 */
class JsiNativeModule {
public:
  /**
   Installs the module. Will try to install the module with the name given when
   constructing the object.
   */
  static void install(jsi::Runtime &rt, const std::string &moduleName,
                      const jsi::Value &parent = jsi::Value::undefined()) {
    // Create module
    auto newModule = jsi::Object(rt);

    // Install in the root (global) object in the runtime - or in the parent
    // object
    if (parent.isObject()) {
      parent.asObject(rt).setProperty(rt, moduleName.c_str(), newModule);
    } else {
      rt.global().setProperty(rt, moduleName.c_str(), newModule);
    }

    // Install our methods
    for (auto &funcDef : getHostFunctions()) {
      newModule.setProperty(rt, funcDef.first.c_str(),
                            jsi::Function::createFromHostFunction(
                                rt, jsi::PropNameID::forUtf8(rt, funcDef.first),
                                0, funcDef.second));
    }
  }

  static std::map<std::string, jsi::HostFunctionType> &getHostFunctions() {
    static std::map<std::string, jsi::HostFunctionType> hostFunctions;
    return hostFunctions;
  }

  static void registerHostFunction(const std::string &name,
                                   jsi::HostFunctionType func) {
    getHostFunctions().emplace(name, std::move(func));
  }
};

// Macro for declaring and registering jsi host functions method
#define JSI_EXPORT_FUNCTION(CLASS, METHOD)                                     \
  static struct METHOD##Registrar {                                            \
    METHOD##Registrar() { CLASS::registerHostFunction("#METHOD", METHOD); }    \
  } METHOD##_registrar;                                                        \
  static jsi::Value METHOD(jsi::Runtime &rt, const jsi::Value &thisValue,      \
                           const jsi::Value *args, size_t count)

} // namespace RNJsi
