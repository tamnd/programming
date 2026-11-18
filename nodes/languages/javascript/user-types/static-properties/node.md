---
tag: MZ63WT
kind: feature
owner: javascript
slug: static-properties
name: Static properties
aliases: [static methods, static fields, static members, class methods, class properties, static keyword]
area: languages/javascript/user-types
level: core
summary: >-
  A static member of a JavaScript class is a property of the class itself, not of its instances. Static fields run once, when the class is defined. Subclasses inherit them through the prototype chain.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-functions-and-classes.html#sec-runtime-semantics-classdefinitionevaluation", locator: "15.7.14 ClassDefinitionEvaluation"}
see_also: [javascript/classes, javascript/instance-methods, javascript/public-fields, javascript/private-fields, javascript/inheritance-and-the-prototype-chain]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes/static
  title: MDN Web Docs, JavaScript Reference, static
  locator: the introduction; Syntax; Description; Examples
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-functions-and-classes.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 15
  locator: 15.7.14 ClassDefinitionEvaluation
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/this
  title: MDN Web Docs, JavaScript Reference, this
  locator: Class context
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Class fields, Private methods, and Static class features, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html
  title: The Java Language Specification, Java SE 25 Edition, chapter 8
  locator: 8.3.1.1 static Fields
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
class Temperature {
  static unit = "C";
  static #made = 0;
  static fromFahrenheit(f) {
    return new Temperature(((f - 32) * 5) / 9);
  }
  static get made() {
    return Temperature.#made;
  }
  constructor(value) {
    this.value = value;
    Temperature.#made += 1;
  }
}
const t = Temperature.fromFahrenheit(212);
console.log(t.value, Temperature.unit, Temperature.made);
console.log(t.unit, typeof t.fromFahrenheit, Object.hasOwn(Temperature, "unit"));
```

```output for=basics
100 C 1
undefined undefined true
```

| Member | Defined on | Reached from an instance method as |
|---|---|---|
| `static m() {}` | the class `C` | `C.m()` or `this.constructor.m()` |
| `static x = v;` | the class `C`, once | `C.x` or `this.constructor.x` |
| `static get x()` | the class `C` | `C.x` |
| `static #x` | the class `C`, private | `C.#x` inside the class body |

## Semantics

**r1.** "The static keyword defines a static method or field for a class".[^1] "Static properties cannot be directly accessed on instances of the class. Instead, they're accessed on the class itself."[^1] ECMA-262 evaluates a static element against the class, not the prototype: "Let element be Completion(ClassElementEvaluation of e with argument F)."[^2] Example: [`basics`](#basics).

**r2.** Static fields "are added to the class constructor at the time of class evaluation using the [[DefineOwnProperty]] semantic (which is essentially Object.defineProperty() )."[^1] "Static field initializers and static initialization blocks are evaluated one-by-one. Field initializers can refer to field values above them, but not below them."[^1] So an initializer runs once, when the class is defined, not when an instance is made. Example: [`timing`](#timing).

**r3.** In a static method, this "is the value of the class (or a subclass)".[^3] "In the field initializer, this refers to the current class (which you can also access through its name), and super refers to the base class constructor."[^1] So one static method can call another through `this`.[^1] Example: [`timing`](#timing).

**r4.** "Public static fields are not reinitialized on subclasses, but can be accessed via the prototype chain."[^1] With `extends`, ECMA-262 makes the superclass the prototype of the new class: "Let constructorParent be superclass."[^2] Example: [`inherit`](#inherit).

**r5.** From an instance method, "Static members are not directly accessible using the this keyword from non-static methods."[^1] Use the class name or `this.constructor`.[^1] Example: [`frominstance`](#frominstance).

**r6.** "The name of a static property (field or method) cannot be prototype ."[^1]

## Examples

When static initializers run (rules r2 and r3).

```javascript run id=timing
console.log("before the class");
class Registry {
  static items = [];
  static count = Registry.items.length;
  static add(x) {
    this.items.push(x);
    return this.size();
  }
  static size() {
    return this.items.length;
  }
}
console.log("after the class", Registry.count);
console.log(Registry.add("a"), Registry.add("b"));
```

```output for=timing
before the class
after the class 0
1 2
```

Static members and subclasses (rule r4).

```javascript run id=inherit
class Shape {
  static kind = "shape";
  static describe() {
    return `a ${this.kind} made by ${this.name}`;
  }
}
class Circle extends Shape {}
class Square extends Shape {
  static kind = "square";
}
console.log(Circle.describe(), "/", Square.describe());
console.log(Object.getPrototypeOf(Circle) === Shape, Object.hasOwn(Circle, "kind"));
```

```output for=inherit
a shape made by Circle / a square made by Square
true false
```

Reaching static members from an instance (rule r5).

```javascript run id=frominstance
class Unit {
  static symbol = "m";
  format(n) {
    return `${n} ${Unit.symbol} or ${n} ${this.constructor.symbol}`;
  }
}
class Kilometre extends Unit {
  static symbol = "km";
}
console.log(new Unit().format(1));
console.log(new Kilometre().format(2));
```

```output for=frominstance
1 m or 1 m
2 m or 2 km
```

`Unit.symbol` always names the base class, while `this.constructor.symbol` follows the class of the instance.

## Versions

| Feature | Edition |
|---|---|
| static fields, public and private | ECMAScript 2022[^4] |

The TC39 list of finished proposals gives 2022 for the row Class fields, Private methods, and Static class features.[^4]

## Pitfalls

### A static counter updated through this

```javascript wrong id=counter
class Base {
  static count = 0;
  static create() {
    this.count += 1;
    return new this();
  }
}
class Derived extends Base {}
Base.create();
Derived.create();
Derived.create();
console.log(Base.count, Derived.count, Object.hasOwn(Derived, "count"));
```

```output for=counter
1 2 true
```

`Derived.create()` runs with `this` set to `Derived`. The read finds `Base.count` through the prototype chain, but the assignment makes a new own property on `Derived` (rule r4). Write `Base.count += 1` for one shared counter.

### Calling a static method on an instance

```javascript wrong id=oninstance
class MathUtil {
  static square(n) {
    return n * n;
  }
}
const m = new MathUtil();
try {
  m.square(3);
} catch (e) {
  console.log(e.name);
}
console.log(MathUtil.square(3), m.constructor.square(3));
```

```output for=oninstance
TypeError
9 9
```

The instance does not have `square`, so `m.square` is undefined and the call throws (rule r1). Call it on the class.

## Comparison

In Java, "If a field is declared static , there exists exactly one incarnation of the field, no matter how many instances (possibly zero) of the class may eventually be created."[^5] A JavaScript static field is also stored once, as a property of the class. But a subclass can get its own copy by assignment through `this`, as the counter pitfall shows (rule r4).

## References
