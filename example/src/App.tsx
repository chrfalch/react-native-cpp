import * as React from 'react';
import { StyleSheet, View, Text } from 'react-native';

import {} from 'react-native-cpp';

export default function App() {
  return (
    <View style={styles.container}>
      <Text>Installed:</Text>
      <Text>
        {'TestModule: ' +
          ('TestModule' in globalThis
            ? `[${Object.keys(globalThis['TestModule'])}] = ${globalThis[
                'TestModule'
              ].add(2, 2)}`
            : false)}
      </Text>
      <Text>
        {'JsiTestClass: ' +
          ('JsiTestClass' in globalThis
            ? `[${Object.keys(globalThis['JsiTestClass'])}] = ${globalThis[
                'JsiTestClass'
              ]
                .create()
                .getX()}`
            : false)}
      </Text>
      <Text>
        {'JsiStateTestClass: ' +
          ('JsiStateTestClass' in globalThis
            ? `[${Object.keys(globalThis['JsiStateTestClass'])}] = ${globalThis[
                'JsiStateTestClass'
              ]
                .create(10, 20)
                .getX()}`
            : false)}
      </Text>
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
