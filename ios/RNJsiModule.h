#ifdef __cplusplus
#import "JsiModuleRegistry.h"
#endif

#ifdef RCT_NEW_ARCH_ENABLED
#import "RNCppSpec.h"

@interface Cpp : NSObject <NativeCppSpec>
#else
#import <React/RCTBridgeModule.h>

@interface Cpp : NSObject <RCTBridgeModule>
#endif

@end
