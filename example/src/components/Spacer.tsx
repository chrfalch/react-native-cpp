import React from 'react';
import { StyleSheet, View } from 'react-native';

export const Spacer = () => <View style={styles.spacer} />;

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#eee',
  },
  scrollview: {
    flex: 1,
    paddingHorizontal: 16,
  },
  spacer: {
    height: 8,
  },
});
