export interface TestModule {
  getX(): number;
}

export interface JsiTestClass {
  getX(): number;
  getY(): number;
}

export interface JsiStateTestClass {
  getX(): number;
  getY(): number;
}

export interface SimpleJsiHostObject {
  getX(): number;
}

declare global {
  var TestModule: TestModule;
  var SimpleJsiHostObject: () => SimpleJsiHostObject;
  var JsiTestClass: { create(): JsiTestClass };
  var JsiStateTestClass: { create(x: number, y: number): JsiStateTestClass };
}
