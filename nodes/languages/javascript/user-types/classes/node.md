---
tag: 250ZFN
kind: feature
owner: javascript
slug: classes
name: Classes
aliases: [class, class declaration, class expression, class syntax, ES6 classes, class body]
area: languages/javascript/user-types
level: intro
summary: >-
  A JavaScript class is a constructor function made with class syntax. Its methods go on the prototype, its static members on the class. It must be called with new, and its body is strict code.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-functions-and-classes.html#sec-class-definitions", locator: "15.7 Class Definitions"}
see_also: [javascript/constructors, javascript/defining-methods, javascript/inheritance-and-the-prototype-chain, javascript/strict-mode, object-oriented-programming, python/class-definition-syntax]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes
  title: MDN Web Docs, JavaScript Reference, Classes
  locator: the introduction; Defining classes; Class body; Methods; Static methods and fields; Binding this with instance and static methods
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-functions-and-classes.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 15
  locator: 15.7.13 ClassElementEvaluation; 15.7.14 ClassDefinitionEvaluation
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Using_classes
  title: MDN Web Docs, JavaScript Guide, Using classes
  locator: the introduction; Constructing instances; Class declaration hoisting; Static properties
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 10
  locator: 10.2.1 [[Call]]; 10.2.6 MakeClassConstructor; 10.2.8 DefineMethodProperty
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-source-code.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 11
  locator: 11.2.2 Strict Mode Code
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html
  title: The Java Language Specification, Java SE 25 Edition, chapter 8
  locator: 8.4.3.2 static Methods
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/datamodel.html
  title: The Python Language Reference, Data model, Python 3.14
  locator: 3.2.8.8 Classes
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
class Point {
  constructor(x, y) {
    this.x = x;
    this.y = y;
  }
  norm() {
    return Math.hypot(this.x, this.y);
  }
  get label() {
    return `(${this.x}, ${this.y})`;
  }
  static origin() {
    return new Point(0, 0);
  }
}
const p = new Point(3, 4);
console.log(p.norm(), p.label, Point.origin().label);
console.log(typeof Point, Object.getPrototypeOf(p) === Point.prototype);
console.log(Object.keys(p), Object.hasOwn(Point.prototype, "norm"), Object.hasOwn(Point, "origin"));
```

```output for=basics
5 (3, 4) (0, 0)
function true
[ 'x', 'y' ] true true
```

| Part | Where it ends up |
|---|---|
| `constructor(...) { ... }` | the class itself, a function called by `new` |
| `name() { ... }`, `get name() { ... }` | non-enumerable properties of `C.prototype` |
| `static name() { ... }` | a property of the class `C` |
| `this.x = ...` in the constructor | an own property of each instance |

## Semantics

**r1.** "Classes are a template for creating objects. They encapsulate data with code to work on that data. Classes in JS are built on prototypes but also have some syntax and semantics that are unique to classes."[^1] A class evaluates to a function object: ECMA-262 builds a prototype object, "Let proto be OrdinaryObjectCreate(protoParent).", and then a constructor `F` for it.[^2] So `typeof C` is `'function'`, and `new C()` makes an object whose prototype is `C.prototype`. "Classes themselves are normal JavaScript values as well, and have their own prototype chains."[^3] Example: [`basics`](#basics).

**r2.** "Methods are defined on the prototype of each class instance and are shared by all instances."[^1] ECMA-262 evaluates each non-static element against the prototype: "Let element be Completion(ClassElementEvaluation of e with argument proto)."[^2] A static element gets `F` instead, so "Static properties (fields and methods) are defined on the class itself instead of each instance."[^1] Example: [`statics`](#statics).

**r3.** Class methods are not enumerable. For a method in a class body, ECMA-262 says "Return ? MethodDefinitionEvaluation of MethodDefinition with arguments object and false."[^2] The last argument becomes the enumerable flag of the property: "Let desc be the PropertyDescriptor { [[Value]]: closure, [[Writable]]: true, [[Enumerable]]: enumerable, [[Configurable]]: true }."[^4] So `Object.keys` and `for...in` skip them, unlike methods assigned to `F.prototype` by hand. Example: [`enumerable`](#enumerable).

**r4.** A class can only be constructed. "Typical function constructors can both be constructed with new and called without new ."[^3] A class constructor is marked with "Set F.[[IsClassConstructor]] to true.", and its [[Call]] method then has "Let error be a newly created TypeError object."[^4] Example: [`nonew`](#nonew).

**r5.** A class can be a declaration or an expression. "Like function expressions, class expressions may be anonymous, or have a name that's different from the variable that it's assigned to. However, unlike function declarations, class declarations have the same temporal dead zone restrictions as let or const and behave as if they are not hoisted."[^1] Example: [`expressions`](#expressions).

**r6.** Class code is strict: "All parts of a ClassDeclaration or a ClassExpression are strict mode code."[^5] So when a method is called without an object, "the this value will be undefined inside the method".[^1] Example: [`detached`](#detached).

## Examples

Instance methods and static methods (rule r2).

```javascript run id=statics
class Temperature {
  constructor(celsius) {
    this.celsius = celsius;
  }
  static fromFahrenheit(f) {
    return new Temperature(((f - 32) * 5) / 9);
  }
  describe() {
    return `${this.celsius} C`;
  }
}
const t = Temperature.fromFahrenheit(212);
console.log(t.describe(), typeof t.fromFahrenheit, typeof Temperature.describe);
console.log(new Temperature(1).describe === t.describe);
```

```output for=statics
100 C undefined undefined
true
```

All instances share one `describe` function, found through the prototype.

Class methods against methods assigned by hand (rule r3).

```javascript run id=enumerable
class A {
  m() {}
}
function B() {}
B.prototype.m = function () {};
console.log(Object.keys(A.prototype), Object.keys(B.prototype));
for (const key in new A()) console.log("A instance has", key);
for (const key in new B()) console.log("B instance has", key);
```

```output for=enumerable
[] [ 'm' ]
B instance has m
```

Calling a class without `new` (rule r4).

```javascript run id=nonew
class Box {}
function OldBox() {}
try {
  Box();
} catch (e) {
  console.log(e.name);
}
console.log(OldBox(), new Box() instanceof Box);
```

```output for=nonew
TypeError
undefined true
```

Class expressions and their names (rule r5).

```javascript run id=expressions
const Anonymous = class {};
const Named = class Inner {
  who() {
    return Inner.name;
  }
};
console.log(Anonymous.name, Named.name, new Named().who(), typeof Inner);
```

```output for=expressions
Anonymous Inner Inner undefined
```

The name `Inner` is bound only inside the class body.

## Pitfalls

### Using a class before its declaration

```javascript wrong id=early
try {
  new Early();
} catch (e) {
  console.log(e.name);
}
class Early {}
console.log(typeof new Early());
```

```output for=early
ReferenceError
object
```

A function declaration could be called here, but a class declaration cannot (rule r5). MDN: class declarations "are not hoisted (or, in some interpretations, hoisted but with the temporal dead zone restriction), which means you cannot use a class before it is declared."[^3] Declare the class first.

### Passing a method as a callback

```javascript wrong id=detached
class Counter {
  constructor() {
    this.count = 0;
  }
  increment() {
    this.count += 1;
    return this.count;
  }
}
const c = new Counter();
const loose = c.increment;
try {
  loose();
} catch (e) {
  console.log(e.name);
}
const bound = c.increment.bind(c);
console.log(bound(), [1, 2].map(() => c.increment()));
```

```output for=detached
TypeError
1 [ 2, 3 ]
```

`loose()` runs the method with `this` undefined (rule r6), so reading `this.count` throws. Bind the method, or call it from an arrow function.

## Comparison

A Java method that is not static is an instance method, and "An instance method is always invoked with respect to an object, which becomes the current object to which the keywords this and super refer during execution of the method body."[^6] A JavaScript method has no fixed object: `this` comes from the call, and is undefined when there is none (rule r6). Python's classes "are callable. These objects normally act as factories for new instances of themselves", so `Point(3, 4)` makes an instance.[^7] A JavaScript class must be called with `new` (rule r4).

## References
