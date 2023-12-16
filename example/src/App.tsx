import * as React from 'react';
import { StyleSheet, View, Text } from 'react-native';

import {} from 'react-native-cpp';

export default function App() {
  return (
    <View style={styles.container}>
      <Text>Installed:</Text>
      <Text>
        {'TestObject: ' +
          ('TestObject' in globalThis
            ? Object.keys(globalThis['TestObject'])
            : false)}
      </Text>
      <Text>
        {'JsiTestObject: ' +
          ('JsiTestObject' in globalThis
            ? Object.keys(globalThis['JsiTestObject'])
            : false)}
      </Text>
      <Text>
        {'JsiStateTestObject: ' +
          ('JsiStateTestObject' in globalThis
            ? Object.keys(globalThis['JsiStateTestObject'])
            : false)}
      </Text>

      <Text>
        {'__JsiTestModule: ' +
          ('__JsiTestModule' in globalThis
            ? '[' +
              Object.keys(globalThis['__JsiTestModule']) +
              ']' +
              ', ' +
              globalThis['__JsiTestModule'].return_200()
            : false)}
      </Text>

      <Text>
        {'__JsiTestObject: ' +
          ('__JsiTestObject' in globalThis
            ? '[' +
              Object.keys(globalThis['__JsiTestObject']) +
              ']' +
              ', ' +
              globalThis['__JsiTestObject'].create().return_100()
            : false)}
      </Text>

      <Text>
        {'__JsiStateObject: ' +
          ('__JsiStateObject' in globalThis
            ? '[' +
              Object.keys(globalThis['__JsiStateObject']) +
              ']' +
              ', ' +
              globalThis['__JsiStateObject'].create(50, 100).area()
            : false)}
      </Text>
      {/* <Text>
        {'TestObject add: ' +
          ('TestObject' in globalThis &&
            (globalThis as any).TestObject.add(10, 20))}
      </Text> */}
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
});
