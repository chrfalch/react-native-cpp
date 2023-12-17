import * as React from 'react';
import { StyleSheet, SafeAreaView, Button, View } from 'react-native';

import 'react-native-cpp';
import '../cpp/types';

import { createTest } from './tests';
import { T, TestSection } from './components';

const getX = () => 22;

const REPEAT = 1000;
export default function App() {
  const [_, setCounter] = React.useState(0);

  return (
    <SafeAreaView style={styles.container}>
      <View style={styles.content}>
        <T.Small>{`Running in ${__DEV__ ? 'DEBUG' : 'RELEASE'} mode`}</T.Small>
        <Button title="▶️ Run" onPress={() => setCounter((p) => p + 1)} />
        <TestSection
          title="Call Method"
          example="() => return getX()"
          repeat={REPEAT}
          tests={[
            createTest('c++', () => globalThis.TestModule.getX(), {
              expect: 22,
            }),
            createTest('js', () => getX(), { expect: 22 }),
          ]}
        />

        <TestSection
          title="Create Objects"
          repeat={REPEAT}
          example="new SometClass()"
          tests={[
            createTest('c++', () => {
              globalThis.JsiTestClass.create();
            }),
            createTest('c++ jsi::HostObject', () => {
              globalThis.SimpleJsiHostObject();
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
              'c++ jsi::HostObject',
              (obj: any) => {
                return obj.getX();
              },
              {
                expect: 22,
                before: () => {
                  return SimpleJsiHostObject();
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

        <T.Small>Installed:</T.Small>
        <T.Note>
          {'TestModule: ' + ('TestModule' in globalThis ? true : false)}
        </T.Note>
        <T.Note>
          {'JsiTestClass: ' + ('JsiTestClass' in globalThis ? true : false)}
        </T.Note>
        <T.Note>
          {'SimpleJsiHostObject: ' +
            ('SimpleJsiHostObject' in globalThis ? true : false)}
        </T.Note>
        <T.Note>
          {'JsiStateTestClass: ' +
            ('JsiStateTestClass' in globalThis ? true : false)}
        </T.Note>
      </View>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#efefef',
  },
  content: {
    flex: 1,
    padding: 14,
  },
});
