export interface TestModule {
  add(x: number, y: number): number;
}

export interface JsiTestClass {
  getX(): number;
  getY(): number;
}

export interface JsiStateTestClass {
  getX(): number;
  getY(): number;
}

declare global {
  var TestModule: TestModule;
  var JsiTestClass: { create(): JsiTestClass };
  var JsiStateTestClass: { create(x: number, y: number): JsiStateTestClass };
}
