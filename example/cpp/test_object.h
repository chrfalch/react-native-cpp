#include "RNJsi.h"

namespace Example {

using namespace RNJsi;

class TestObject : public JsiNativeModule<TestObject> {
public:
  JSI_HOST_FUNCTION(TestObject, add,
                    { return args[0].asNumber() + args[1].asNumber(); });
};

JSI_EXPORT_MODULE(TestObject, "TestObject")
} // namespace Example
