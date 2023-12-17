export type Test = {
  name: string;
  group?: string;
  test: (data?: unknown) => unknown;
  before?: () => void;
  before_each?: () => unknown;
  data?: unknown;
  expect?: unknown;
};
