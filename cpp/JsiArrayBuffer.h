#pragma once

#import <jsi/jsi.h>

namespace RNJsi {

namespace jsi = facebook::jsi;

/**
 Implements a simple memory buffer that can be used as the buffer when creating
 a jsi::ArrayBuffer.
 */
class JsiMemoryBuffer : public jsi::MutableBuffer {
public:
  JsiMemoryBuffer(size_t size) {
    _size = size;
    _data = (uint8_t *)malloc(size);
  }

  ~JsiMemoryBuffer() {
    free(_data);
    _size = 0;
  }

  size_t size() const { return _size; }
  uint8_t *data() { return _data; };

private:
  uint8_t *_data;
  size_t _size;
};

/**
 Impements a jsi::MutableBuffer that works as a std::unique_ptr and can be used
 as the memory buffer for a jsi::ArrayBuffer
 */
template <typename T> class JsiInstanceBuffer : public jsi::MutableBuffer {
public:
  /**
   Creates a new instance of a UniquePtrBuffer that instantiates and wraps a
   given type.
   */
  template <class... _Args> JsiInstanceBuffer(_Args &&...__args) {
    _size = sizeof(T);
    _data = std::make_unique<T>(std::forward<_Args>(__args)...);
  }

  size_t size() const { return _size; }
  uint8_t *data() { return reinterpret_cast<uint8_t *>(_data.get()); };

private:
  std::unique_ptr<T> _data;
  size_t _size;
};

/**
 Creates an array buffer containing the given type T.
 */
template <typename T, class... _Args>
jsi::Value makeAsArrayBuffer(jsi::Runtime &rt, _Args &&...__args) {
  return jsi::ArrayBuffer(rt, std::make_shared<JsiInstanceBuffer<T>>(
                                  std::forward<_Args>(__args)...));
}

struct MyRect {
  MyRect(size_t x, size_t y) : x(x), y(y) {}
  size_t x;
  size_t y;
  size_t width;
  size_t height;
};

/**
 Creates a new JS ArrayBuffer from a given data pointer.
 */
static jsi::Value createArrayBuffer(jsi::Runtime &rt, size_t size) {
  return jsi::ArrayBuffer(rt, std::make_shared<JsiMemoryBuffer>(size));
}

/**
 Creates a new JS ArrayBuffer wrapping a MyRect
 */
static jsi::Value createArrayBufferWrappingMyRect(jsi::Runtime &rt, size_t x,
                                                  size_t y) {
  return makeAsArrayBuffer<MyRect>(rt, x, y);
}

} // namespace RNJsi
