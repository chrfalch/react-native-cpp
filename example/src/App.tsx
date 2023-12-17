import * as React from 'react';
import { StyleSheet, Text, SafeAreaView } from 'react-native';

import 'react-native-cpp';
import '../cpp/types.ts';
import { createTest } from './tests';
import { TestSection } from './components';

const add = (a: number, b: number) => a + b;
const REPEAT = 1000;
export default function App() {
  return (
    <SafeAreaView style={styles.container}>
      <TestSection
        title="Create Objects"
        repeat={REPEAT}
        example="new SometClass()"
        tests={[
          createTest('c++', () => {
            globalThis.JsiTestClass.create();
          }),
          createTest('js', () => {
            const state = { x: 0 };
            return {
              getX() {
                return state.x;
              },
            };
          }),
        ]}
      />

      <TestSection
        title="Call Method"
        example="add(1, 1)"
        repeat={REPEAT}
        tests={[
          createTest('c++', () => {
            globalThis.TestModule.add(1, 1);
          }),
          createTest('js', () => {
            add(1, 1);
          }),
        ]}
      />

      <TestSection
        title="Call Method on object"
        example="obj.getX()"
        repeat={REPEAT}
        tests={[
          createTest(
            'c++',
            (obj: any) => {
              return obj.getX();
            },
            {
              expect: 22,
              before: () => {
                return JsiTestClass.create();
              },
            }
          ),
          createTest(
            'js',
            (obj: any) => {
              return obj.getX();
            },
            {
              expect: 10,
              before: () => {
                return { getX: () => 10 };
              },
            }
          ),
        ]}
      />

      <Text>Installed:</Text>
      <Text>
        {'TestModule: ' + ('TestModule' in globalThis ? true : false)}
      </Text>
      <Text>
        {'JsiTestClass: ' + ('JsiTestClass' in globalThis ? true : false)}
      </Text>
      <Text>
        {'JsiStateTestClass: ' +
          ('JsiStateTestClass' in globalThis ? true : false)}
      </Text>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
  },
});
