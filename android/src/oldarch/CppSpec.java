package com.cpp;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.Promise;

abstract class CppSpec extends ReactContextBaseJavaModule {
  CppSpec(ReactApplicationContext context) {
    super(context);
  }
}
