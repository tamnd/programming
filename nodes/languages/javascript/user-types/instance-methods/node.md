---
tag: NDGPJJ
kind: feature
owner: javascript
slug: instance-methods
name: Instance methods
aliases: [class methods, prototype methods, methods in a class body, bound methods, method borrowing]
area: languages/javascript/user-types
level: intro
summary: >-
  An instance method in a JavaScript class body is one non-enumerable function on the class prototype, shared by every instance. Its this comes from the call, and it cannot be used with new.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-functions-and-classes.html#sec-static-semantics-classelementevaluation", locator: "15.7.13 ClassElementEvaluation"}
see_also: [javascript/classes, javascript/defining-methods, javascript/inheritance-and-the-prototype-chain, javascript/constructor, python/method-objects]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes
  title: MDN Web Docs, JavaScript Reference, Classes
  locator: Methods
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Using_classes
  title: MDN Web Docs, JavaScript Guide, Using classes
  locator: Instance methods
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-functions-and-classes.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 15
  locator: 15.7.13 ClassElementEvaluation
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 10
  locator: 10.2.8 DefineMethodProperty
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/this
  title: MDN Web Docs, JavaScript Reference, this
  locator: Class context; Bound methods in classes
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/Method_definitions
  title: MDN Web Docs, JavaScript Reference, Method definitions
  locator: Method definitions are not constructable
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Inheritance_and_the_prototype_chain
  title: MDN Web Docs, JavaScript Guide, Inheritance and the prototype chain
  locator: Inheriting methods
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/datamodel.html
  title: The Python Language Reference, Data model, Python 3.14
  locator: 3.2.8.2 Instance methods
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
class Color {
  constructor(r, g, b) {
    this.values = [r, g, b];
  }
  getRed() {
    return this.values[0];
  }
  setRed(value) {
    this.values[0] = value;
    return this;
  }
}
const red = new Color(255, 0, 0);
const blue = new Color(0, 0, 255);
console.log(red.getRed(), blue.getRed(), red.setRed(10).getRed());
console.log(red.getRed === blue.getRed, Object.hasOwn(red, "getRed"));
```

```output for=basics
255 0 10
true false
```

| Property of the method | Value |
|---|---|
| where it lives | `C.prototype`, one function for all instances |
| enumerable | false |
| writable, configurable | true |
| `this` inside | the object it was called on, or undefined |
| `new obj.method()` | TypeError |

## Semantics

**r1.** "Methods are defined on the prototype of each class instance and are shared by all instances."[^1] The guide: "A function can be shared between all instances, but still have its behavior differ when different instances call it, because the value of this is different."[^2] Example: [`basics`](#basics).

**r2.** ECMA-262 evaluates a method in a class body with "Return ? MethodDefinitionEvaluation of MethodDefinition with arguments object and false."[^3] The false is the enumerable flag, used in "Let desc be the PropertyDescriptor { [[Value]]: closure, [[Writable]]: true, [[Enumerable]]: enumerable, [[Configurable]]: true }."[^4] So the method can be replaced or deleted, but `Object.keys` and `for...in` skip it. Example: [`attrs`](#attrs).

**r3.** Class methods take `this` from the call: "the this value is the object that the method was accessed on. If the method is not transferred to another object, this is generally an instance of the class."[^5] "Classes are always in strict mode. Calling methods with an undefined this will throw an error if the method tries to access properties on this ."[^5] To fix `this`, "bind the class methods in the constructor".[^5] Example: [`bound`](#bound).

**r4.** "Methods cannot be constructors! They will throw a TypeError if you try to instantiate them."[^6] Example: [`attrs`](#attrs).

**r5.** "Methods can be plain functions, async functions, generator functions, or async generator functions."[^1] A generator method with the key `Symbol.iterator` makes instances iterable. Example: [`kinds`](#kinds).

**r6.** An own property with the same name hides the method. "An inherited function acts just as any other property, including property shadowing as shown above (in this case, a form of method overriding)."[^7] Example: [`shadow`](#shadow).

## Examples

The descriptor of a method, and `new` on a method (rules r2 and r4).

```javascript run id=attrs
class Greeter {
  hello() {
    return "hello";
  }
}
const d = Object.getOwnPropertyDescriptor(Greeter.prototype, "hello");
console.log(typeof d.value, d.writable, d.enumerable, d.configurable);
console.log(Object.keys(Greeter.prototype), Object.getOwnPropertyNames(Greeter.prototype));
try {
  new Greeter.prototype.hello();
} catch (e) {
  console.log(e.name);
}
```

```output for=attrs
function true false true
[] [ 'constructor', 'hello' ]
TypeError
```

A borrowed method and a bound method (rule r3).

```javascript run id=bound
class Car {
  constructor() {
    this.sayBye = this.sayBye.bind(this);
  }
  get name() {
    return "Ferrari";
  }
  sayHi() {
    return `Hello from ${this.name}`;
  }
  sayBye() {
    return `Bye from ${this.name}`;
  }
}
const car = new Car();
const bird = { name: "Tweety", sayHi: car.sayHi, sayBye: car.sayBye };
console.log(bird.sayHi(), "/", bird.sayBye());
console.log(Object.hasOwn(car, "sayBye"), Object.hasOwn(car, "sayHi"));
```

```output for=bound
Hello from Tweety / Bye from Ferrari
true false
```

The bound copy is an own property of `car`, so it hides the prototype method (rule r6).

Generator and async methods (rule r5).

```javascript run id=kinds
class Range {
  constructor(from, to) {
    this.from = from;
    this.to = to;
  }
  *[Symbol.iterator]() {
    for (let i = this.from; i <= this.to; i++) yield i;
  }
  async total() {
    let sum = 0;
    for (const n of this) sum += n;
    return sum;
  }
}
const r = new Range(1, 4);
console.log([...r]);
r.total().then((sum) => console.log(sum));
```

```output for=kinds
[ 1, 2, 3, 4 ]
10
```

## Pitfalls

### Creating methods in the constructor

```javascript wrong id=perinstance
class Color {
  constructor(r) {
    this.r = r;
    this.getRed = function () {
      return this.r;
    };
  }
}
const a = new Color(1);
const b = new Color(2);
console.log(a.getRed === b.getRed, Object.keys(a));
```

```output for=perinstance
false [ 'r', 'getRed' ]
```

The guide: "this creates a new function every time a Color instance is created, even when they all do the same thing!"[^2] The function is also an enumerable own property. Declare it in the class body (rule r1). Binding in the constructor has the same cost, so MDN advises: "Only use it where absolutely necessary."[^5]

### Assigning over a method

```javascript wrong id=shadow
class Task {
  constructor(title) {
    this.title = title;
  }
  done() {
    return `${this.title} finished`;
  }
}
const t = new Task("report");
t.done = true;
console.log(typeof t.done, typeof Task.prototype.done);
delete t.done;
console.log(t.done());
```

```output for=shadow
boolean function
report finished
```

The assignment makes an own data property `done` on `t`, which hides the method for that instance only (rule r6). Use different names for data and methods.

## Comparison

In Python, reading a function through an instance makes a method object whose `__self__` is that instance, and "the method object is said to be bound."[^8] So `f = obj.method` followed by `f()` still works on `obj`. In JavaScript, `const f = obj.method` gives the plain function, and `f()` runs with `this` undefined (rule r3). Use `obj.method.bind(obj)` or an arrow function to keep the object.

## References
