import * as React from 'react';
import { StyleSheet, View, Text } from 'react-native';

import {} from 'react-native-cpp';

export default function App() {
  return (
    <View style={styles.container}>
      <Text>Installed?</Text>
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
