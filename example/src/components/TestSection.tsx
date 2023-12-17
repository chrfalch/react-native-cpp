import React from 'react';
import { StyleSheet, View } from 'react-native';
import { type Test, runTest } from '../testrunner';
import { Spacer } from './Spacer';
import { T } from './T';
import { Horizontal } from './Horizontal';
import { Flex } from './Flex';

type Props = {
  title: string;
  example?: string;
  repeat: number;
  tests: Array<Test>;
};

const DECIMALS = 5;

export const TestSection: React.FC<Props> = ({
  title,
  example,
  repeat,
  tests,
}) => {
  const results = tests
    .map((t) => runTest({ ...t, group: title }, repeat))
    .sort((a, b) => a.avg - b.avg);
  return (
    <View style={styles.container}>
      <Horizontal>
        <T.Header>{title}</T.Header>
        <Flex />
        <T.Small>🧪 </T.Small>
      </Horizontal>
      <T.Code>{example}</T.Code>
      <Spacer />

      <T.FixedBold>
        {'avg' +
          spaces(DECIMALS) +
          '    diff' +
          spaces(DECIMALS) +
          '   inc       type'}
      </T.FixedBold>
      {results.map((result, index) => (
        <React.Fragment key={result.name}>
          <T.Fixed>{`${result.avg.toFixed(DECIMALS)} ms  ${
            index > 0
              ? (result.avg - results[0]!.avg).toFixed(DECIMALS)
              : spaces(DECIMALS) + ' -'
          } ms  ${toFixed(
            ((result.avg - results[0]!.avg) / results[0]!.avg) * 100,
            1
          )}%    ${result.name}`}</T.Fixed>
        </React.Fragment>
      ))}
    </View>
  );
};

const spaces = (n: number) => Array(n + 1).join(' ');
const toFixed = (n: number, decimals: number) => {
  return n
    .toFixed(decimals)
    .toString()
    .padStart(decimals + 4, ' ');
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    borderRadius: 4,
    paddingHorizontal: 4,
    paddingBottom: 4,
    marginVertical: 4,
  },
});
