#pragma once

#import <jsi/jsi.h>

namespace RNJsi {

namespace jsi = facebook::jsi;

template <typename S> class JsiNativeStateWrapper : public jsi::NativeState {
public:
  JsiNativeStateWrapper(const S &value) : _value(std::move(value)) {}
  template <class... _Args>
  JsiNativeStateWrapper(_Args &&...__args)
      : _value(std::forward<_Args>(__args)...) {}
  ~JsiNativeStateWrapper() {}
  S &getValue() { return _value; }

private:
  S _value;
};

} // namespace RNJsi
