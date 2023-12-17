import { type Test } from '../testrunner';

export const createTest = (
  name: string,
  test: (args?: unknown) => void,
  params: Partial<Omit<Test, 'name' | 'test'>> = {}
): Test => ({
  name,
  test,
  ...params,
});
