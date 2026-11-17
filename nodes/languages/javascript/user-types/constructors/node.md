---
tag: YGFFN9
kind: feature
owner: javascript
slug: constructors
name: Constructors
aliases: [constructor function, constructor functions, new operator, new.target, prototype property, function prototype, instanceof]
area: languages/javascript/user-types
level: core
summary: >-
  A JavaScript constructor is a function called with new. new makes an object whose prototype is the function's prototype property, runs the function with this bound to it, and returns it.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html#sec-ecmascript-function-objects-construct-argumentslist-newtarget", locator: "10.2.2 [[Construct]]; 10.1.14 GetPrototypeFromConstructor"}
see_also: [javascript/inheritance-and-the-prototype-chain, javascript/objects, javascript/defining-methods, classes-and-instances]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/new
  title: MDN Web Docs, JavaScript Reference, new
  locator: Description
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 10
  locator: 10.1.14 GetPrototypeFromConstructor; 10.2.2 [[Construct]]
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Function/prototype
  title: MDN Web Docs, JavaScript Reference, Function prototype
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Inheritance_and_the_prototype_chain
  title: MDN Web Docs, JavaScript Guide, Inheritance and the prototype chain
  locator: Constructors
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 13
  locator: 13.3.5.1.1 EvaluateNew
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/constructor
  title: MDN Web Docs, JavaScript Reference, Object.prototype.constructor
  locator: the introduction; Changing the constructor of a constructor function's prototype
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/instanceof
  title: MDN Web Docs, JavaScript Reference, instanceof
  locator: Description
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html
  title: The Java Language Specification, Java SE 25 Edition, chapter 8
  locator: 8.8 Constructor Declarations
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
function Point(x, y) {
  this.x = x;
  this.y = y;
}
Point.prototype.norm = function () {
  return Math.hypot(this.x, this.y);
};
const p = new Point(3, 4);
console.log(p.norm(), Object.getPrototypeOf(p) === Point.prototype);
console.log(p.constructor === Point, p instanceof Point, Object.keys(p));
```

```output for=basics
5 true
true true [ 'x', 'y' ]
```

| Step of `new F(args)` | Result |
|---|---|
| 1 | a new object, with `F.prototype` as its prototype if that is an object, else `Object.prototype` |
| 2 | `F` runs with `this` set to the new object |
| 3 | if `F` returns an object, that object is the result; else the new object is |

## Semantics

**r1.** `new` does these things. It "Creates a blank, plain JavaScript object." It "Points newInstance 's [[Prototype]] to the constructor function's prototype property, if the prototype is an Object . Otherwise, newInstance stays as a plain object with Object.prototype as its [[Prototype]]." It "Executes the constructor function with the given arguments, binding newInstance as the this context".[^1] ECMA-262 gets the prototype in GetPrototypeFromConstructor, which "determines the [[Prototype]] value that should be used to create an object corresponding to a specific constructor."[^2] Example: [`basics`](#basics).

**r2.** The result: "If the constructor function returns a non-primitive, this return value becomes the result of the whole new expression. Otherwise, if the constructor function doesn't return anything or returns a primitive, newInstance is returned instead."[^1] In [[Construct]]: "If result.[[Value]] is an Object, return result.[[Value]]." "If kind is base, return thisArgument."[^2] Example: [`returns`](#returns).

**r3.** "A function's prototype property, by default, is a plain object with one property: constructor , which is a reference to the function itself. The constructor property is writable, non-enumerable, and configurable."[^3] Example: [`basics`](#basics).

**r4.** Methods go on the prototype so that all instances share them. "To add the new property to all objects of the same type, you must add the property to the constructor's prototype property. This defines a property that is shared by all objects created with that function, rather than by just one instance of the object type."[^1] MDN: "Classes are syntax sugar over constructor functions, which means you can still manipulate Box.prototype to change the behavior of all instances."[^4] Example: [`shared`](#shared).

**r5.** Not every function is a constructor. EvaluateNew does this: "If IsConstructor(constructor) is false, throw a TypeError exception."[^5] Methods, arrow functions and async functions "do not have prototype , and are therefore ineligible as constructors, even if a prototype property is later manually assigned".[^3] Example: [`notctor`](#notctor).

**r6.** "A function can know whether it is invoked with new by checking new.target . new.target is only undefined when the function is invoked without new ."[^1] Example: [`target`](#target).

**r7.** The `constructor` property "is created by default on the prototype property of every constructor function and is inherited by all objects created by that constructor."[^6] `instanceof` "tests the presence of constructor.prototype in object 's prototype chain. This usually (though not always) means object was constructed with constructor ."[^7] Example: [`basics`](#basics).

## Examples

The value a constructor returns (rule r2).

```javascript run id=returns
function Plain() {
  this.a = 1;
  return 5;
}
function Replaced() {
  this.a = 1;
  return { b: 2 };
}
const r = new Replaced();
console.log(new Plain(), r, r instanceof Replaced);
```

```output for=returns
Plain { a: 1 } { b: 2 } false
```

A method on the prototype is one function for all instances (rule r4).

```javascript run id=shared
function A() {
  this.f = function () {};
}
function B() {}
B.prototype.f = function () {};
console.log(new A().f === new A().f, new B().f === new B().f);
console.log(Object.hasOwn(new A(), "f"), Object.hasOwn(new B(), "f"));
```

```output for=shared
false true
true false
```

Functions that are not constructors (rule r5).

```javascript run id=notctor
const arrow = () => {};
const method = { m() {} }.m;
for (const f of [arrow, method]) {
  try {
    new f();
  } catch (e) {
    console.log(e.name, f.prototype);
  }
}
```

```output for=notctor
TypeError undefined
TypeError undefined
```

Checking for `new` with `new.target` (rule r6).

```javascript run id=target
function Temp(c) {
  if (!new.target) return new Temp(c);
  this.celsius = c;
}
console.log(new Temp(20), Temp(30));
```

```output for=target
Temp { celsius: 20 } Temp { celsius: 30 }
```

## Pitfalls

### Calling a constructor without `new`

```javascript wrong id=nonew
"use strict";
function Point(x, y) {
  this.x = x;
  this.y = y;
}
try {
  Point(1, 2);
} catch (e) {
  console.log(e.name);
}
console.log(new Point(1, 2).x);
```

```output for=nonew
TypeError
1
```

"While the constructor function can be invoked like any regular function (i.e., without the new operator), in this case a new object is not created and the value of this is also different."[^1] In strict mode `this` is undefined, so the first assignment throws. Use `new`, check `new.target` (rule r6), or use a class, which cannot be called without `new`.

### Replacing the prototype object

```javascript wrong id=reassign
function Box(v) {
  this.v = v;
}
const old = new Box(1);
Box.prototype = {
  get() {
    return this.v;
  },
};
const fresh = new Box(2);
console.log(fresh.get(), typeof old.get, old instanceof Box);
console.log(fresh.constructor === Box, fresh.constructor === Object);
```

```output for=reassign
2 undefined false
false true
```

Objects made before the change keep the old prototype, and the new prototype has no `constructor` of its own. MDN: "if ConstructorFunction.prototype is re-assigned, the constructor property will be lost."[^6] "Note that the value of an instanceof test can change if constructor.prototype is re-assigned after creating the object (which is usually discouraged)."[^7] Add methods to the prototype that is there, as in [`basics`](#basics).

## Comparison

In Java a constructor is part of a class: "A constructor is used in the creation of an object that is an instance of a class".[^8] "Constructor declarations are not members. They are never inherited and therefore are not subject to hiding or overriding."[^8] A JavaScript constructor is an ordinary function object with a `prototype` property (rule r3). Any function that is a constructor can be called with `new` (rule r5), and class syntax makes the same kind of function (rule r4).

## References
