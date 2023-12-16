#pragma once

#import <jsi/jsi.h>

#import <functional>
#import <map>

namespace RNJsi {

namespace jsi = facebook::jsi;

class JsiModuleRegistry {
public:
  /**
   * @return Singleton instance of the JsiModuleRegistry
   */
  static JsiModuleRegistry &getInstance();

  /**
   * Registers a callback that will be call at install time with the given
   * name as one of its parameters.
   * @param name Name that installer will be called with on installation
   * @param installer Callback that performs the installation
   */
  void registerModule(std::string name,
                      std::function<void(jsi::Runtime &)> installer);

  /**
   * Installs all registered modules
   * @param rt Runtime to install into
   */
  void install(jsi::Runtime &rt);

private:
  std::map<std::string, std::function<void(jsi::Runtime &)>> _installers;
};

/**
 Implements a simple class for creating static registrars for modules
 */
template <typename T> struct JsiModuleRegistrar {
  JsiModuleRegistrar(std::string exportName) {
    // Register module with module registry
    RNJsi::JsiModuleRegistry::getInstance().registerModule(
        exportName,
        std::bind(&T::install, std::placeholders::_1, exportName, nullptr));
  }
};

} // namespace RNJsi
