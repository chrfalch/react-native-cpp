import React from 'react';
import { StyleSheet, View } from 'react-native';

export const Horizontal: React.FC<React.PropsWithChildren<{}>> = ({
  children,
}) => <View style={styles.container}>{children}</View>;

const styles = StyleSheet.create({
  container: {
    flexDirection: 'row',
    alignItems: 'center',
  },
});
