import * as React from 'react';
import { StyleSheet, SafeAreaView, Button, ScrollView } from 'react-native';

import 'react-native-cpp';
import '../cpp/types';

import {
  call_method_on_class_or_object,
  call_method_on_class_or_object_with_state,
  call_method_on_module,
  create_object,
} from './tests';
import { T, TestSection } from './components';

const REPEAT = 10000;
export default function App() {
  const [_, setCounter] = React.useState(0);

  return (
    <SafeAreaView style={styles.container}>
      <ScrollView style={styles.content}>
        <T.Small>{`Running in ${__DEV__ ? 'DEBUG' : 'RELEASE'} mode`}</T.Small>
        <Button title="▶️ Run" onPress={() => setCounter((p) => p + 1)} />
        <TestSection
          title="Call Method"
          example="() => return getX()"
          repeat={REPEAT}
          tests={call_method_on_module}
        />

        <TestSection
          title="Create Objects"
          repeat={REPEAT}
          example="new SometClass()"
          tests={create_object}
        />

        <TestSection
          title="Call Method on object"
          example="obj.getX()"
          repeat={REPEAT}
          tests={call_method_on_class_or_object}
        />

        <TestSection
          title="Call Method on state object"
          example="obj.getX(), where result value is stored in obj"
          repeat={REPEAT}
          tests={call_method_on_class_or_object_with_state}
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
      </ScrollView>
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
