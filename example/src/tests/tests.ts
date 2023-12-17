import { createTest } from './createTest';

const getX = () => 22;

export const call_method_on_module = [
  createTest('c++', () => globalThis.TestModule.getX(), {
    expect: 22,
  }),
  createTest('js', () => getX(), { expect: 22 }),
];

export const create_object = [
  createTest('c++', () => {
    globalThis.JsiTestClass.create();
  }),
  createTest('c++ state', () => {
    globalThis.JsiStateTestClass.create();
  }),
  createTest('jsi::HostObject', () => {
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
];

export const call_method_on_class_or_object = [
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
    'jsi::HostObject',
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
];

export const call_method_on_class_or_object_with_state = [
  createTest(
    'c++',
    (obj: any) => {
      return obj.getX();
    },
    {
      expect: 22,
      before: () => {
        return JsiStateTestClass.create();
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
        const state = { x: 10 };
        return { getX: () => state.x };
      },
    }
  ),
];
