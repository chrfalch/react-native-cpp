#pragma once

#import <jsi/jsi.h>
#import <map>

namespace RNJsi {

namespace jsi = facebook::jsi;

class JsiModuleRegistry {
public:
  static JsiModuleRegistry &getInstance() {
    static JsiModuleRegistry instance;
    return instance;
  }

  void registerModule(std::string name,
                      std::function<void(jsi::Runtime &)> installer) {
    _installers.emplace(name, std::move(installer));
  }

  void install(jsi::Runtime &rt) {
    for (auto &installer : _installers) {
      installer.second(rt);
    }
  }

private:
  std::map<std::string, std::function<void(jsi::Runtime &)>> _installers;
};

#define JSI_EXPORT_MODULE(CLASS, EXPORT_NAME)                                  \
  _Pragma("clang diagnostic ignored \"-Wunused-variable\"") static struct      \
      CLASS##Registrar {                                                       \
    CLASS##Registrar() {                                                       \
      RNJsi::JsiModuleRegistry::getInstance().registerModule(                  \
          "#EXPORT_NAME", std::bind(&CLASS::install, std::placeholders::_1,    \
                                    EXPORT_NAME, nullptr));                    \
    }                                                                          \
  } CLASS##_registrar;                                                         \
  static CLASS CLASS##_instance;                                               \
  _Pragma("clang diagnostic pop")

} // namespace RNJsi
