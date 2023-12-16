import { NativeModules, Platform } from 'react-native';

const LINKING_ERROR =
  `The package 'react-native-cpp' doesn't seem to be linked. Make sure: \n\n` +
  Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
  '- You rebuilt the app after installing the package\n' +
  '- You are not using Expo Go\n';

// @ts-expect-error
const isTurboModuleEnabled = global.__turboModuleProxy != null;

const CppModule = isTurboModuleEnabled
  ? require('./NativeCpp').default
  : NativeModules.Cpp;

export const RNJsiModule = CppModule
  ? CppModule
  : new Proxy(
      {},
      {
        get() {
          throw new Error(LINKING_ERROR);
        },
      }
    );

// Installs the JSI bindings into the global namespace.
console.log('Installing C++ bindings...');
const result = RNJsiModule.install() as boolean;
if (result !== true) {
  console.error(`Failed to install JSI bindings`);
}
console.log('Successfully installed JSI Bindings');
