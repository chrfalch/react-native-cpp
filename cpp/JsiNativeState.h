#pragma once

#import <jsi/jsi.h>

namespace RNJsi {

namespace jsi = facebook::jsi;

template <typename S> class JsiNativeState : public jsi::NativeState {
public:
  JsiNativeState(const S &value) : _value(std::move(value)) {}
  template <class... _Args>
  JsiNativeState(_Args &&...__args) : _value(std::forward<_Args>(__args)...) {}

  ~JsiNativeState() {}
  S &getValue() { return _value; }

private:
  S _value;
};

} // namespace RNJsi
