import React from 'react';
import { Platform, StyleSheet, Text, View } from 'react-native';

const Header: React.FC<React.PropsWithChildren<{}>> = ({ children }) => (
  <Text style={styles.heading}>{children}</Text>
);

const Body: React.FC<React.PropsWithChildren<{}>> = ({ children }) => (
  <Text style={styles.body}>{children}</Text>
);

const Small: React.FC<React.PropsWithChildren<{}>> = ({ children }) => (
  <Text style={styles.small}>{children}</Text>
);

const Note: React.FC<React.PropsWithChildren<{}>> = ({ children }) => (
  <Text style={styles.note}>{children}</Text>
);

const Fixed: React.FC<React.PropsWithChildren<{}>> = ({ children }) => (
  <Text style={styles.fixed}>{children}</Text>
);

const FixedBold: React.FC<React.PropsWithChildren<{}>> = ({ children }) => (
  <Text style={styles.fixed_bold}>{children}</Text>
);

const Code: React.FC<React.PropsWithChildren<{}>> = ({ children }) => (
  <View style={styles.codeBlock}>
    <Text style={styles.code}>{children}</Text>
  </View>
);

export const T = {
  Header,
  Body,
  Small,
  Note,
  Fixed,
  FixedBold,
  Code,
};

const styles = StyleSheet.create({
  heading: {
    fontWeight: 'bold',
    paddingVertical: 4,
  },
  body: {
    fontSize: 14,
  },
  small: {
    fontSize: 12,
  },
  note: {
    color: '#666',
    fontSize: 12,
  },
  fixed: {
    fontSize: 12,
    fontFamily: Platform.select({ ios: 'Courier new', android: 'monospace' }),
  },
  fixed_bold: {
    fontSize: 12,
    fontWeight: 'bold',
    fontFamily: Platform.select({ ios: 'Courier new', android: 'monospace' }),
  },
  codeBlock: {
    backgroundColor: '#EAEAEA',
    padding: 8,
    borderRadius: 4,
  },
  code: {
    fontSize: 10,
    fontFamily: Platform.select({ ios: 'Courier new', android: 'monospace' }),
  },
});
