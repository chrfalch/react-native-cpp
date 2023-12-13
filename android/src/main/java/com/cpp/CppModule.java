package com.cpp;

import android.util.Log;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.JavaScriptContextHolder;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactMethod;

public class CppModule extends com.cpp.CppSpec {
  public static final String NAME = "Cpp";

  CppModule(ReactApplicationContext context) {
    super(context);
  }

  @Override
  @NonNull
  public String getName() {
    return NAME;
  }

  static {
    System.loadLibrary("react-native-cpp");
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  public boolean install() {
    try {
      JavaScriptContextHolder jsContext = getReactApplicationContext().getJavaScriptContextHolder();

      Log.i(NAME, "Installing JSI Bindings for react-native-cpp plugin...");
      boolean successful = nativeInstall(jsContext.get());
      if (successful) {
        Log.i(NAME, "Successfully installed JSI Bindings!");
        return true;
      } else {
        Log.e(NAME, "Failed to install JSI Bindings for react-native-cpp");
        return false;
      }
    } catch (Exception exception) {
      Log.e(NAME, "Failed to install JSI Bindings!", exception);
      return false;
    }
  }

  private static native boolean nativeInstall(long jsiPtr);
}
