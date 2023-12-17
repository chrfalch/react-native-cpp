#import "JsiModuleRegistry.h"
#import "JsiLogger.h"
#import "JsiNativeClass.h"

namespace RNJsi {

namespace jsi = facebook::jsi;

JsiModuleRegistry &JsiModuleRegistry::getInstance() {
  static JsiModuleRegistry instance;
  return instance;
}

void JsiModuleRegistry::registerModule(
    std::string name, std::function<void(jsi::Runtime &)> installer) {
  JsiLogger::logToConsole(
      "JsiModuleRegistry: Registering module installer \"%s\".", name.c_str());
  _installers.emplace(name, std::move(installer));
}

void JsiModuleRegistry::install(jsi::Runtime &rt) {
  JsiLogger::logToConsole("JsiModuleRegistry: Running module installers...");
  for (auto &installer : _installers) {
    installer.second(rt);
  }
  JsiLogger::logToConsole("JsiModuleRegistry: Done running module installers.");
}

} // namespace RNJsi
