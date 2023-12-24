#import "RNJsiModule.h"
#import <React-callinvoker/ReactCommon/CallInvoker.h>
#import <React/RCTBridge+Private.h>
#import <jsi/jsi.h>
#import <string>

#if __has_include("react-native-cpp.h")
#import "react-native-cpp.h"
#endif

@implementation Cpp
RCT_EXPORT_MODULE()

using namespace facebook;

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(install) {
  RCTBridge *bridge = [RCTBridge currentBridge];
  RCTCxxBridge *cxxBridge = (RCTCxxBridge *)bridge;
  if (!cxxBridge.runtime) {
    return @(false);
  }
  try {
    RNJsi::JsiModuleRegistry::getInstance().install(
        *(jsi::Runtime *)cxxBridge.runtime);
  } catch (std::exception &exc) {
    NSLog(@"Failed to install JSI Modules in JS runtime: %s", exc.what());
    return @(false);
  }

  return @(true);
}

// Don't compile this code when we build for the old architecture.
#ifdef RCT_NEW_ARCH_ENABLED
- (std::shared_ptr<facebook::react::TurboModule>)getTurboModule:
    (const facebook::react::ObjCTurboModule::InitParams &)params {
  return std::make_shared<facebook::react::NativeCppSpecJSI>(params);
}
#endif

@end
