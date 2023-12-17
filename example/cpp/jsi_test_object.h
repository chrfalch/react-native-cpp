#include "RNJsi.h"

namespace Example {

using namespace RNJsi;

class JsiTestObject : public JsiNativeObject<JsiTestObject> {
  JSI_HOST_FUNCTION(
      JsiTestObject, getX, JSI_FUNCTION { return 22; });
  JSI_HOST_FUNCTION(
      JsiTestObject, getY, JSI_FUNCTION { return 10; });
};

JSI_EXPORT_OBJECT(JsiTestObject, "JsiTestObject")

} // namespace Example
