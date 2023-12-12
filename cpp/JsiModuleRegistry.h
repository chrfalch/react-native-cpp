#pragma once

#import <jsi/jsi.h>

namespace RNJsi {

namespace jsi = facebook::jsi;

class JsiModuleRegistry {
public:
  static JsiModuleRegistry &getInstance() {
    static JsiModuleRegistry instance;
    return instance;
  }

  void registerModule(std::function<void(jsi::Runtime &)> installer) {
    _installers.push_back(installer);
  }

  void install(jsi::Runtime &rt) {
    for (auto &installer : _installers) {
      installer(rt);
    }
  }

private:
  std::vector<std::function<void(jsi::Runtime &)>> _installers;
};

#define JSI_EXPORT_MODULE(CLASS, EXPORT_NAME)                                  \
  _Pragma("clang diagnostic ignored \"-Wunused-variable\"") static struct      \
      CLASS##Registrar {                                                       \
    CLASS##Registrar() {                                                       \
      JsiUtils::JsiModuleRegistry::getInstance().registerModule(std::bind(     \
          &CLASS::install, std::placeholders::_1, EXPORT_NAME, nullptr));      \
    }                                                                          \
  } CLASS##_registrar;                                                         \
  static CLASS CLASS##_instance;                                               \
  _Pragma("clang diagnostic pop")

} // namespace RNJsi
