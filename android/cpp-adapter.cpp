#include <jni.h>
#include <jsi/jsi.h>
#include "JsiModuleRegistry.h"

namespace jsi = facebook::jsi;

 extern "C"
 JNIEXPORT jboolean JNICALL
 Java_com_cpp_CppModule_nativeInstall(JNIEnv *env, jclass type, jlong runtimePtr) {
   auto rt = reinterpret_cast<jsi::Runtime*>(runtimePtr);
   if (rt == nullptr) {
     // Runtime was null!
     return false;
   }

   RNJsi::JsiModuleRegistry::getInstance().install(*rt);
   return true;
 }
