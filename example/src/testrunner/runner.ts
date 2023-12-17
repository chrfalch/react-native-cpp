import type { Test } from './types';

export const runTest = (test: Test, timesToRun = 10_000) => {
  // Collect times for all runs
  const times: Array<number> = [];

  if (!test.test) {
    throw new Error(
      `Test '${test.group} - ${test.name}' does not have a test function`
    );
  }

  // Wrap calling the test
  const t = (test_args?: unknown) => {
    // Run the test
    const start = performance.now();
    const result = test.test(test_args);
    const end = performance.now();

    // Check the result
    if (test.expect != null && result !== test.expect) {
      console.log(test.name + ' - failed.');
      console.log('\texpected ' + test.expect + ', got ' + result);
    }

    times.push(end - start);
  };

  let args = test.before?.();

  for (let i = 0; i < timesToRun; i++) {
    const r = test.before_each?.() ?? undefined;
    t(r ?? args);
  }

  // Call garbage collection
  globalThis.gc && globalThis.gc();

  return {
    name: test.name,
    avg: calculateTrimmedAverageTime(times, 0.1),
  };
};

function calculateTrimmedAverageTime(
  times: Array<number>,
  trimPercentage: number
) {
  // Step 1: Sort the times in ascending order
  const sortedTimes = times.slice().sort((a, b) => a - b);

  // Step 2: Calculate the number of values to trim from both ends
  const trimCount = Math.round(sortedTimes.length * trimPercentage);

  // Step 3: Trim the specified percentage of lowest and highest values
  const trimmedTimes = sortedTimes.slice(
    trimCount,
    sortedTimes.length - trimCount
  );

  // Step 4: Calculate the average of the trimmed times
  const trimmedAverageTime =
    trimmedTimes.reduce((acc, time) => acc + time, 0) / trimmedTimes.length;

  return trimmedAverageTime;
}
