#import "JsiModuleRegistry.h"
#import "JsiLogger.h"
#import "JsiNativeClass.h"

#define LOG_MODULE_REGISTRY 0

namespace RNJsi {

namespace jsi = facebook::jsi;

JsiModuleRegistry &JsiModuleRegistry::getInstance() {
  static JsiModuleRegistry instance;
  return instance;
}

void JsiModuleRegistry::registerModule(
    std::string name, std::function<void(jsi::Runtime &)> installer) {
#if LOG_MODULE_REGISTRY
  JsiLogger::logToConsole(
      "JsiModuleRegistry: Registering module installer \"%s\".", name.c_str());
#endif
  _installers.emplace(name, std::move(installer));
}

void JsiModuleRegistry::install(jsi::Runtime &rt) {
#if LOG_MODULE_REGISTRY
  JsiLogger::logToConsole("JsiModuleRegistry: Running module installers...");
#endif
  for (auto &installer : _installers) {
    installer.second(rt);
  }
#if LOG_MODULE_REGISTRY
  JsiLogger::logToConsole("JsiModuleRegistry: Done running module installers.");
#endif
}

} // namespace RNJsi
