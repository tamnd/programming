---
tag: 7Y560V
kind: feature
owner: javascript
slug: defining-methods
name: Defining methods
aliases: [method definitions, shorthand method syntax, object methods, this in methods, generator methods, async methods, HomeObject]
area: languages/javascript/user-types
level: intro
summary: >-
  A method is a property whose value is a function. The shorthand name() {} defines one in an object literal, and this inside it is the object the method was called on.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-functions-and-classes.html#sec-method-definitions", locator: "15.4 Method Definitions; 10.2.7 MakeMethod"}
see_also: [javascript/objects, javascript/object-initializers, method, method-concept]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Working_with_objects
  title: MDN Web Docs, JavaScript Guide, Working with objects
  locator: Defining methods; Using this for object references
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/Method_definitions
  title: MDN Web Docs, JavaScript Reference, Method definitions
  locator: the introduction; Description; Method definitions are not constructable; Using super in method definitions; Generator methods
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/this
  title: MDN Web Docs, JavaScript Reference, this
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 10
  locator: 10.2 ECMAScript Function Objects, Table 25; 10.2.7 MakeMethod
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/datamodel.html
  title: The Python Language Reference, Data model, Python 3.14
  locator: Instance methods
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html
  title: The Java Language Specification, Java SE 25 Edition, chapter 8
  locator: 8.4 Method Declarations
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const counter = {
  count: 0,
  increment() {
    this.count += 1;
    return this.count;
  },
  reset: function () {
    this.count = 0;
  },
};
counter.increment();
console.log(counter.increment(), typeof counter.increment);
counter.reset();
console.log(counter.count);
```

```output for=basics
2 function
0
```

| Form | Defines |
|---|---|
| `name() {}` | a method |
| `name: function () {}` | a property whose value is a function |
| `*name() {}` | a generator method |
| `async name() {}` | an async method |
| `[expression]() {}` | a method with a computed name |
| `obj.name = fn` | a method added after the object is made |

## Semantics

**r1.** "A method is a function associated with an object, or, put differently, a method is a property of an object that is a function."[^1] "Method definition is a shorter syntax for defining a function property in an object initializer. It can also be used in classes."[^2] Example: [`basics`](#basics).

**r2.** "Properties defined using this syntax are own properties of the created object, and they are configurable, enumerable, and writable, just like normal properties."[^2] Example: [`attrs`](#attrs).

**r3.** "The value of this in JavaScript depends on how a function is invoked (runtime binding), not how it is defined."[^3] For a call in the form `obj.f()`, the value of `this` "refers to obj".[^3] So one function can serve as a method of many objects. "Methods are typically defined on the prototype object of the constructor, so that all objects of the same type share the same method."[^1] Example: [`shared`](#shared).

**r4.** A method made with this syntax is not the same as a property whose value is a function expression; MDN notes "there are semantic differences".[^2] One difference: "Methods cannot be constructors! They will throw a TypeError if you try to instantiate them. On the other hand, a property created as a function can be used as a constructor."[^2] Example: [`construct`](#construct).

**r5.** "Only functions defined as methods have access to the super keyword. super.prop looks up the property on the prototype of the object that the method was initialized on."[^2] In ECMA-262, MakeMethod "configures F as a method" by setting its [[HomeObject]], the object whose prototype is where super lookups begin.[^4] Example: [`super`](#super).

**r6.** Generator and async functions have their own method forms: `*g() {}`, `async f() {}` and `async *f() {}`.[^2] "Note that the asterisk (* ) in the generator method syntax must be before the generator property name."[^2] Example: [`gen`](#gen).

**r7.** Arrow functions are not methods in this sense: "arrow functions do not have their own this binding. Therefore, their this value cannot be set by bind() , apply() or call() methods, nor does it point to the current object in object methods."[^3] Example: [`arrow`](#arrow).

## Examples

A method is a normal own property (rule r2).

```javascript run id=attrs
const obj = { m() {} };
const d = Object.getOwnPropertyDescriptor(obj, "m");
console.log(d.writable, d.enumerable, d.configurable, Object.keys(obj));
```

```output for=attrs
true true true [ 'm' ]
```

One function, two objects (rule r3).

```javascript run id=shared
function describe() {
  return `I am ${this.name}`;
}
const ada = { name: "Ada", describe };
const alan = { name: "Alan", describe };
console.log(ada.describe(), "/", alan.describe());
```

```output for=shared
I am Ada / I am Alan
```

`super` in an object literal method (rule r5).

```javascript run id=super
const base = {
  greet() {
    return "hello";
  },
};
const child = {
  __proto__: base,
  greet() {
    return `${super.greet()} from child`;
  },
};
console.log(child.greet());
```

```output for=super
hello from child
```

A generator method with a symbol name, and an async method (rule r6).

```javascript run id=gen
const pair = {
  *[Symbol.iterator]() {
    yield 1;
    yield 2;
  },
  async load() {
    return "done";
  },
};
console.log([...pair]);
pair.load().then((v) => console.log(v));
```

```output for=gen
[ 1, 2 ]
done
```

## Pitfalls

### Taking a method off its object

```javascript wrong id=extracted
"use strict";
const user = {
  name: "Ada",
  describe() {
    return `I am ${this.name}`;
  },
};
const describe = user.describe;
try {
  describe();
} catch (e) {
  console.log(e.name);
}
console.log(describe.call(user), "/", user.describe.bind(user)());
```

```output for=extracted
TypeError
I am Ada / I am Ada
```

Called on its own, the function gets no object. MDN: in that case `this` "typically refers to the global object (in non-strict mode) or undefined (in strict mode)".[^3] "The Function.prototype.bind() method can create a function whose this binding doesn't change", and `call` and `apply` set `this` for one call.[^3] Bind the method, or call it through its object (rule r3).

### Using an arrow function as a method

```javascript wrong id=arrow
const timer = {
  seconds: 5,
  arrow: () => this?.seconds,
  method() {
    return this.seconds;
  },
};
console.log(timer.arrow(), timer.method());
```

```output for=arrow
undefined 5
```

The arrow function takes `this` from the code around the object, not from `timer` (rule r7). Use the method syntax.

### Calling `new` on a method

```javascript wrong id=construct
const factory = {
  method() {},
  plain: function () {},
};
console.log(typeof new factory.plain());
try {
  new factory.method();
} catch (e) {
  console.log(e.name);
}
```

```output for=construct
object
TypeError
```

A function made with the method syntax is not a constructor (rule r4). Use a class or a constructor function to make objects with `new`.

## Comparison

In Python, a function looked up on an instance becomes a bound method, so "calling x.f(1) is equivalent to calling C.f(x, 1)", and the method keeps its instance when it is passed around.[^5] In Java, "A method declares executable code that can be invoked, passing a fixed number of values as arguments", and a method is always declared in a class.[^6] A JavaScript method is a property of any object, and `this` is set by each call (rule r3), so a method taken off its object loses it.

## References
