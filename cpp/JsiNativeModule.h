#pragma once

#import <map>

#import <jsi/jsi.h>

#import "JsiLogger.h"
#import "JsiModuleRegistry.h"

namespace RNJsi {

namespace jsi = facebook::jsi;

/**
 Defines the base class for a C++ backed module that can be installed in a
 Javascript runtime.
 */
template <typename T> class JsiNativeModule {
public:
  /**
   * Installs the module. Will try to install the module with the name given
   * when constructing the object.
   * @param rt Runtime to install into
   * @param moduleName Name of module
   * @param parent Optional parent module - if we want to create a tree of
   * modules
   */
  static void install(jsi::Runtime &rt, const std::string &moduleName,
                      const jsi::Value &parent = jsi::Value::undefined()) {
    JsiLogger::logToConsole("JsiNativeModule: Installing Native Module %s",
                            moduleName.c_str());

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

  /**
   * Registers a new host function with the prototype that all new objects
   * are created from.
   * @param name Name of function (name it will be exported as in Javascript)
   * @param func Function to export
   */
  static void registerHostFunction(const std::string &name,
                                   jsi::HostFunctionType func) {
    JsiLogger::logToConsole("JsiNativeModule: Installing host function %s",
                            name.c_str());
    getHostFunctions().emplace(std::move(name), std::move(func));
  }

private:
  /**
   * @return Map of name / function for exported functions
   */
  static std::map<std::string, jsi::HostFunctionType> &getHostFunctions() {
    static std::map<std::string, jsi::HostFunctionType> hostFunctions;
    return hostFunctions;
  }
};

/**
 Implements a simple class for creating static registrars for host functions
 */
template <typename T> struct JsiHostFunctionRegistrar {
  JsiHostFunctionRegistrar(const std::string &exportName,
                           jsi::HostFunctionType func) {
    T::registerHostFunction(std::move(exportName), func);
  }
};

} // namespace RNJsi
