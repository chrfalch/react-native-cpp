#pragma once

#include "RNJsi.h"

namespace Example {

using namespace RNJsi;

class SimpleJsiHostObject : public jsi::HostObject {
public:
  static void install(jsi::Runtime &rt, const std::string &moduleName,
                      const jsi::Value &parent = jsi::Value::undefined()) {
    rt.global().setProperty(
        rt, moduleName.c_str(),
        jsi::Function::createFromHostFunction(
            rt, jsi::PropNameID::forUtf8(rt, "create"), 0,
            [](jsi::Runtime &rt, const jsi::Value &thisValue,
               const jsi::Value *args, size_t count) {
              return jsi::Object::createFromHostObject(
                  rt, std::make_shared<SimpleJsiHostObject>());
            }));
  }

  SimpleJsiHostObject() {}
  virtual ~SimpleJsiHostObject() {}

  jsi::Value get(jsi::Runtime &rt, const jsi::PropNameID &propName) override {
    if (propName.utf8(rt) == "getX") {
      return jsi::Function::createFromHostFunction(
          rt, jsi::PropNameID::forUtf8(rt, "getX"), 0,
          [](jsi::Runtime &rt, const jsi::Value &thisValue,
             const jsi::Value *args, size_t count) { return 22.0; });
    }
    return jsi::Value::undefined();
  }
  void set(jsi::Runtime &rt, const jsi::PropNameID &propName,
           const jsi::Value &value) override {
    // do nothing
  }
  std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime &rt) override {
    std::vector<jsi::PropNameID> names;
    names.push_back(jsi::PropNameID::forUtf8(rt, "add"));
    return names;
  }
};

JSI_EXPORT_MODULE(SimpleJsiHostObject)

} // namespace Example
