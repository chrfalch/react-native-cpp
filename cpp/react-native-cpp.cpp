#include "JsiNativeObject.h"

namespace RNJsi {
class TestObject : public JsiNativeObject<TestObject> {
public:
  JSI_EXPORT_FUNCTION(TestObject, add) {
    return args[0].asNumber() + args[1].asNumber();
  }
};

JSI_EXPORT_MODULE(TestObject, "TestObject")

} // namespace RNJsi
