---
tag: GMH96J
kind: feature
owner: javascript
slug: accessor-fields
name: Accessor fields
aliases: [getters and setters in classes, class accessors, get and set in a class body, computed properties of a class, read-only class properties]
area: languages/javascript/user-types
level: core
summary: >-
  An accessor field is a get or set method in a JavaScript class body. Code uses it like a property, but it runs a function. It lives on the prototype, is not enumerable, and can wrap a private field.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-functions-and-classes.html#sec-runtime-semantics-methoddefinitionevaluation", locator: "15.4.5 MethodDefinitionEvaluation"}
see_also: [javascript/defining-getters-and-setters, javascript/classes, javascript/private-fields, javascript/public-fields, javascript/property-descriptors]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Using_classes
  title: MDN Web Docs, JavaScript Guide, Using classes
  locator: Accessor fields
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/set
  title: MDN Web Docs, JavaScript Reference, set
  locator: the introduction; Using setters in classes
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/get
  title: MDN Web Docs, JavaScript Reference, get
  locator: Using getters in classes
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-functions-and-classes.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 15
  locator: 15.4.5 MethodDefinitionEvaluation
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/JSON/stringify
  title: MDN Web Docs, JavaScript Reference, JSON.stringify()
  locator: Description
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-source-code.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 11
  locator: 11.2.2 Strict Mode Code
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes/Public_class_fields
  title: MDN Web Docs, JavaScript Reference, Public class fields
  locator: Description
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/functions.html
  title: The Python Standard Library, Python 3.14, Built-in Functions
  locator: property
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
class Temperature {
  #celsius = 0;
  get celsius() {
    return this.#celsius;
  }
  get fahrenheit() {
    return (this.#celsius * 9) / 5 + 32;
  }
  set fahrenheit(f) {
    this.#celsius = ((f - 32) * 5) / 9;
  }
}
const t = new Temperature();
t.fahrenheit = 212;
console.log(t.celsius, t.fahrenheit);
console.log(Object.keys(t), Object.hasOwn(t, "fahrenheit"), "fahrenheit" in t);
```

```output for=basics
100 212
[] false true
```

| Declared | Reading `o.x` | Assigning `o.x = v` |
|---|---|---|
| `get x()` and `set x(v)` | runs the getter | runs the setter |
| `get x()` only | runs the getter | TypeError in strict code, ignored in sloppy code |
| `set x(v)` only | undefined | runs the setter |
| `static get x()` | on the class, `C.x` | on the class |

## Semantics

**r1.** The guide: "There are only two methods, but they are prefixed with get and set , which allows them to be manipulated as if they were properties."[^1] The getter runs on each read and the setter on each assignment: "The set syntax binds an object property to a function to be called when there is an attempt to set that property."[^2] Example: [`basics`](#basics).

**r2.** "Getter properties are defined on the prototype property of the class and are thus shared by all instances of the class. Unlike getter properties in object literals, getter properties in classes are not enumerable."[^3] ECMA-262 defines a getter with "Let desc be the PropertyDescriptor { [[Get]]: closure, [[Enumerable]]: enumerable, [[Configurable]]: true }."[^4] A get and a set with the same name make one accessor property. Since the accessor is not an own property, JSON.stringify skips it: "Only enumerable own properties are visited."[^5] Example: [`descriptor`](#descriptor).

**r3.** "If a field only has a getter but no setter, it will be effectively read-only."[^1] Assigning to it fails quietly in sloppy code and throws a TypeError in strict code.[^1] Class bodies are strict: "All parts of a ClassDeclaration or a ClassExpression are strict mode code."[^6] So an assignment inside a method of the class always throws. Example: [`readonly`](#readonly).

**r4.** A setter can check a value before storing it, for example in a private field that outside code cannot reach. MDN's own example stores the value in `#msg`.[^2] Example: [`validate`](#validate).

**r5.** "Static getters and private getters use similar syntaxes, which are described in the static and private elements pages."[^3] A static accessor is a property of the class, and a private accessor such as `get #x()` is read as `this.#x` inside the class. Example: [`kinds`](#kinds).

## Examples

The accessor property on the prototype (rule r2).

```javascript run id=descriptor
class Color {
  constructor(r) {
    this.values = [r];
  }
  get red() {
    return this.values[0];
  }
  set red(v) {
    this.values[0] = v;
  }
}
const d = Object.getOwnPropertyDescriptor(Color.prototype, "red");
console.log(typeof d.get, typeof d.set, d.enumerable, d.configurable);
const c = new Color(255);
c.red = 10;
console.log(JSON.stringify(c), c.red);
```

```output for=descriptor
function function false true
{"values":[10]} 10
```

A setter that checks its value (rule r4).

```javascript run id=validate
class Person {
  #age = 0;
  get age() {
    return this.#age;
  }
  set age(value) {
    if (!Number.isInteger(value) || value < 0) {
      throw new RangeError(`bad age: ${value}`);
    }
    this.#age = value;
  }
}
const p = new Person();
p.age = 30;
try {
  p.age = -1;
} catch (e) {
  console.log(e.message);
}
console.log(p.age);
```

```output for=validate
bad age: -1
30
```

Static and private accessors (rule r5).

```javascript run id=kinds
class Config {
  static #instances = 0;
  #parts = ["a", "b"];
  constructor() {
    Config.#instances += 1;
  }
  static get instances() {
    return Config.#instances;
  }
  get #joined() {
    return this.#parts.join("-");
  }
  describe() {
    return this.#joined;
  }
}
new Config();
console.log(new Config().describe(), Config.instances, new Config().instances);
```

```output for=kinds
a-b 2 undefined
```

## Pitfalls

### Assigning to a getter-only accessor

```javascript wrong id=readonly
class Circle {
  constructor(r) {
    this.r = r;
  }
  get area() {
    return 3 * this.r ** 2;
  }
  double() {
    this.area = this.area * 2;
  }
}
const c = new Circle(2);
try {
  c.double();
} catch (e) {
  console.log(e.name);
}
console.log(c.area);
```

```output for=readonly
TypeError
12
```

The assignment runs in class code, which is strict, so it throws (rule r3). Add a setter, or change the data the getter reads (here `this.r`).

### A field with the same name as an accessor

```javascript wrong id=shadow
class User {
  name = "anon";
  get name() {
    return "from the getter";
  }
}
const u = new User();
console.log(u.name, Object.hasOwn(u, "name"));
```

```output for=shadow
anon true
```

The field makes an own data property on each instance, which hides the accessor on the prototype. MDN: "class fields are added using the [[DefineOwnProperty]] semantic (which is essentially Object.defineProperty() )".[^7] Give the stored value a private name such as `#name` and keep the accessor public.

## Comparison

Python's `property` does the same job: "If c is an instance of C, c.x will invoke the getter, c.x = value will invoke the setter, and del c.x the deleter."[^8] With only a getter, the `@property` decorator makes "a read-only attribute with the same name".[^8] In JavaScript, assigning to a getter-only accessor throws only in strict code, which includes all class code (rule r3).

## References
