#include <jni.h>
#include "react-native-cpp.h"

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_cpp_CppModule_nativeMultiply(JNIEnv *env, jclass type, jdouble a, jdouble b) {
    return RNJsi::multiply(a, b);
}
