---
tag: C8V05F
kind: feature
owner: javascript
slug: private-fields
name: Private fields
aliases: [private class fields, private elements, private methods, hash names, private names, brand checks]
area: languages/javascript/user-types
level: core
summary: >-
  A JavaScript private field has a hash name such as #count. Only code inside the class body can use it. It is not a property, so keys, JSON, Proxy and Object functions never see it.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-privateget", locator: "7.3.27 PrivateFieldAdd; 7.3.30 PrivateGet"}
see_also: [javascript/public-fields, javascript/classes, javascript/instance-methods, javascript/object-integrity-levels, encapsulation-and-visibility]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes/Private_elements
  title: MDN Web Docs, JavaScript Reference, Private elements
  locator: the introduction; Syntax; Description; Examples
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/abstract-operations.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 7
  locator: 7.3.27 PrivateFieldAdd; 7.3.30 PrivateGet
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 13
  locator: 13.10.1 Runtime Semantics, Evaluation
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Class fields, Private methods, and Static class features; Ergonomic brand checks for Private Fields
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/expressions.html
  title: The Python Language Reference, Expressions, Python 3.14
  locator: 6.2.2.1 Private name mangling
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
class Counter {
  #count = 0;
  #check(n) {
    if (n < 0) throw new RangeError("negative step");
  }
  add(n) {
    this.#check(n);
    this.#count += n;
    return this.#count;
  }
}
const c = new Counter();
console.log(c.add(2), c.add(3));
console.log(Object.keys(c), JSON.stringify(c), c["#count"]);
```

```output for=basics
2 5
[] {} undefined
```

| Operation | Public field `x` | Private field `#x` |
|---|---|---|
| read from outside the class | `o.x` | SyntaxError |
| read where it is missing | undefined | TypeError |
| `Object.keys`, `JSON.stringify`, `for...in` | listed | not listed |
| `delete` | allowed | SyntaxError |
| test presence | `'x' in o` | `#x in o`, inside the class |
| usable in subclass code | yes | no |

## Semantics

**r1.** "Private elements get created by using a hash # prefix and cannot be legally referenced outside of the class. The privacy encapsulation of these class elements is enforced by JavaScript itself. The only way to access a private element is via dot notation, and you can only do so within the class that defines the private element."[^1] Fields, methods, getters and setters, static or not, all have private forms.[^1] Example: [`basics`](#basics).

**r2.** A private element is not a property. "A property in JavaScript has a string or symbol key, and has attributes like writable , enumerable , and configurable , but private elements have none. While private elements are accessed with the familiar dot notation, they cannot be proxied, enumerated, deleted, or interacted with using any Object method."[^1] Example: [`basics`](#basics).

**r3.** Wrong uses are caught when the code is parsed. "It is a syntax error to refer to # names from outside of the class. It is also a syntax error to refer to private elements that were not declared in the class body, or to attempt to remove declared elements with delete ."[^1] Example: [`syntax`](#syntax).

**r4.** "If you access a private element from an object that doesn't have the element, a TypeError is thrown, instead of returning undefined as normal properties do."[^1] ECMA-262, in PrivateGet: "If entry is empty, throw a TypeError exception."[^2] Adding a private field twice also throws: in PrivateFieldAdd, "If entry is not empty, throw a TypeError exception."[^2] Example: [`brand`](#brand).

**r5.** `#x in obj` tests for a private element without throwing. "This will return true if the private field or method exists, and false otherwise."[^1] ECMA-262: "If PrivateElementFind(rVal, privateName) is empty, return false. Return true."[^3] Example: [`brand`](#brand).

**r6.** "Private elements are not part of the prototypical inheritance model since they can only be accessed within the current class's body and aren't inherited by subclasses. Private elements with the same name within different classes are entirely different and do not interoperate with each other."[^1] Also, "Object.freeze() and Object.seal() have no effect on private elements."[^1] Example: [`frozen`](#frozen).

## Examples

Errors found when the code is parsed (rule r3). Each source is compiled with `eval` so that the error can be caught.

```javascript run id=syntax
const sources = [
  "class A { #x; m() { delete this.#x; } }",
  "class B { m() { return this.#y; } }",
  "class C { #z; } new C().#z;",
];
for (const src of sources) {
  try {
    eval(src);
  } catch (e) {
    console.log(e.name);
  }
}
```

```output for=syntax
SyntaxError
SyntaxError
SyntaxError
```

Brand checks with `in` and with a read (rules r4 and r5).

```javascript run id=brand
class Point {
  #x;
  constructor(x) {
    this.#x = x;
  }
  static isPoint(o) {
    return #x in o;
  }
  static getX(o) {
    return o.#x;
  }
}
console.log(Point.isPoint(new Point(1)), Point.isPoint({ x: 1 }));
console.log(Point.getX(new Point(7)));
try {
  Point.getX({ x: 1 });
} catch (e) {
  console.log(e.name);
}
```

```output for=brand
true false
7
TypeError
```

Freezing does not touch private fields (rule r6).

```javascript run id=frozen
class Box {
  #value = 1;
  bump() {
    this.#value += 1;
    return this.#value;
  }
}
const b = Object.freeze(new Box());
console.log(Object.isFrozen(b), b.bump(), b.bump());
```

```output for=frozen
true 2 3
```

## Versions

| Feature | Edition |
|---|---|
| private fields, methods and accessors | ECMAScript 2022[^4] |
| `#x in obj` | ECMAScript 2022[^4] |

The TC39 list of finished proposals gives 2022 for the Class fields, Private methods and Static class features row, and 2022 for Ergonomic brand checks for Private Fields.[^4]

## Pitfalls

### A private static field read through this

```javascript wrong id=statics
class Base {
  static #count = 0;
  static next() {
    this.#count += 1;
    return this.#count;
  }
}
class Derived extends Base {}
console.log(Base.next());
try {
  Derived.next();
} catch (e) {
  console.log(e.name);
}
```

```output for=statics
1
TypeError
```

`Derived.next()` runs with `this` set to `Derived`, which does not have `#count` (rule r6). MDN: "only the class which defines the private static field can access the field."[^1] Write `Base.#count` instead of `this.#count`.

### Wrapping an instance in a Proxy

```javascript wrong id=proxy
class Secret {
  #code = 42;
  reveal() {
    return this.#code;
  }
}
const s = new Secret();
const p = new Proxy(s, {});
console.log(s.reveal());
try {
  p.reveal();
} catch (e) {
  console.log(e.name);
}
```

```output for=proxy
42
TypeError
```

`p.reveal()` runs with `this` set to the proxy, and the proxy has no private elements (rule r2). Call such methods on the target object.

## Comparison

Python has no enforced privacy. A name in a class body that "begins with two or more underscore characters and does not end in two or more underscores" is "considered a private name of that class", and "private names are transformed to a longer form before code is generated for them."[^5] The longer name can still be used from outside the class. A JavaScript `#name` cannot be reached from outside the class at all (rule r1).

## References
