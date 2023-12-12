#import "RNJsiModule.h"
#import <React-callinvoker/ReactCommon/CallInvoker.h>
#import <React/RCTBridge+Private.h>
#import <jsi/jsi.h>
#import <string>

@implementation Cpp
RCT_EXPORT_MODULE()

using namespace facebook;

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(install) {
  RCTBridge *bridge = [RCTBridge currentBridge];
  RCTCxxBridge *cxxBridge = (RCTCxxBridge *)bridge;
  if (!cxxBridge.runtime) {
    return @(false);
  }
  jsi::Runtime &rt = *(jsi::Runtime *)cxxBridge.runtime;

  try {
    RNJsi::JsiModuleRegistry::getInstance().install(rt);
  } catch (std::exception &exc) {
    NSLog(@"Failed to install C++ modules to Runtime! %s", exc.what());
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
