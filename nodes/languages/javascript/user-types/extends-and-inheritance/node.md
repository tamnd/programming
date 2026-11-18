---
tag: 2C5383
kind: feature
owner: javascript
slug: extends-and-inheritance
name: Extends and inheritance
aliases: [extends, class inheritance, subclasses, derived classes, superclass, extends null, inheriting from a class]
area: languages/javascript/user-types
level: core
summary: >-
  class B extends A makes B a subclass of A. B.prototype inherits from A.prototype and B inherits from A, so instances get A's methods and B gets A's static members. A class has one superclass.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-functions-and-classes.html#sec-runtime-semantics-classdefinitionevaluation", locator: "15.7.14 ClassDefinitionEvaluation"}
see_also: [javascript/classes, javascript/constructor, javascript/the-super-keyword, javascript/inheritance-and-the-prototype-chain, python/inheritance, python/multiple-inheritance, class-inheritance, composition-over-inheritance]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes/extends
  title: MDN Web Docs, JavaScript Reference, extends
  locator: the introduction; Description; Extending null; Examples
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-functions-and-classes.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 15
  locator: 15.7.14 ClassDefinitionEvaluation
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/functions.html
  title: The Python Standard Library, Python 3.14, Built-in Functions
  locator: super
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
class Animal {
  constructor(name) {
    this.name = name;
  }
  speak() {
    return `${this.name} makes a sound`;
  }
  static create(name) {
    return new this(name);
  }
}
class Dog extends Animal {
  speak() {
    return `${this.name} barks`;
  }
}
const d = Dog.create("Rex");
console.log(d.speak(), d instanceof Dog, d instanceof Animal);
console.log(Object.getPrototypeOf(Dog) === Animal);
console.log(Object.getPrototypeOf(Dog.prototype) === Animal.prototype);
```

```output for=basics
Rex barks true true
true
true
```

| Class header | Prototype of `C` | Prototype of `C.prototype` |
|---|---|---|
| `class C` | `Function.prototype` | `Object.prototype` |
| `class C extends null` | `Function.prototype` | null |
| `class C extends P` | `P` | `P.prototype` |

## Semantics

**r1.** "The extends keyword is used in class declarations or class expressions to create a class that is a child of another class."[^1] "extends sets the prototype for both ChildClass and ChildClass.prototype ."[^1] ECMA-262 reads the parent's `prototype` property, uses it as the prototype of the new prototype object, and uses the parent as the prototype of the new class: "Let proto be OrdinaryObjectCreate(protoParent)." and "Let constructorParent be superclass."[^2] So instance lookups reach the parent's methods, and lookups on the class reach its static members. Example: [`basics`](#basics).

**r2.** "Any constructor that can be called with new and has the prototype property can be the candidate for the parent class."[^1] "The extends keyword can be used to subclass custom classes as well as built-in objects."[^1] That includes old-style constructor functions. The spec checks both: a value that is not a constructor gives a TypeError, and "If protoParent is not an Object and protoParent is not null, throw a TypeError exception."[^2] Example: [`expressions`](#expressions), [`heritage`](#heritage).

**r3.** "The right-hand side of extends does not have to be an identifier. You can use any expression that evaluates to a constructor."[^1] The expression runs once, when the class is defined: "Let superclassRef be Completion(Evaluation of ClassHeritage)."[^2] "The this value in the extends expression is the this surrounding the class definition, and referring to the class's name is a ReferenceError because the class is not initialized yet."[^1] Example: [`expressions`](#expressions), [`heritage`](#heritage).

**r4.** A class with `extends` has a derived constructor: "If ClassHeritage is present, set F.[[ConstructorKind]] to derived."[^2] A derived class with no `constructor` gets a default one that "behaves similarly to constructor(...args) { super(...args); }".[^2] A derived constructor that is written out must call `super()` before it uses `this`. "While the base class may return anything from its constructor, the derived class must return an object or undefined , or a TypeError will be thrown."[^1] Example: [`basics`](#basics), [`returns`](#returns).

**r5.** "A class can only have a single superclass, so multiple inheritance from tooling classes, for example, is not possible."[^1] A function that takes a base class and returns a subclass of it gives mix-ins instead.[^1] Example: [`expressions`](#expressions).

## Examples

Different kinds of parent (rules r2, r3 and r5).

```javascript run id=expressions
function OldStyle(x) {
  this.x = x;
}
OldStyle.prototype.double = function () {
  return this.x * 2;
};
class Modern extends OldStyle {}

const pick = (ordered) => (ordered ? Map : Set);
class Store extends pick(true) {}

const Named = (Base) =>
  class extends Base {
    label() {
      return `#${this.x}`;
    }
  };
class Tagged extends Named(Modern) {}

const t = new Tagged(21);
console.log(new Modern(21).double(), new Store() instanceof Map);
console.log(t.double(), t.label(), t instanceof OldStyle);
```

```output for=expressions
42 true
42 #21 true
```

What a derived constructor may return (rule r4).

```javascript run id=returns
class Base {
  constructor() {
    return 1;
  }
}
console.log(new Base() instanceof Base);
class Child extends Base {
  constructor() {
    super();
    return 1;
  }
}
try {
  new Child();
} catch (e) {
  console.log(e.name);
}
```

```output for=returns
true
TypeError
```

The base constructor's primitive return value is ignored. The same return in a derived constructor throws.

## Pitfalls

### Extending a value that is not a constructor

```javascript wrong id=heritage
const Animal = {
  speak() {
    return `${this.name} makes a noise`;
  },
};
try {
  class Pet extends Animal {}
} catch (e) {
  console.log(e.name);
}
const bound = function () {}.bind(null);
try {
  class Pet extends bound {}
} catch (e) {
  console.log(e.name);
}
try {
  class Pet extends Pet {}
} catch (e) {
  console.log(e.name);
}
class Dog {
  constructor(name) {
    this.name = name;
  }
}
Object.setPrototypeOf(Dog.prototype, Animal);
console.log(new Dog("Mitzie").speak());
```

```output for=heritage
TypeError
TypeError
ReferenceError
Mitzie makes a noise
```

A plain object is not a constructor, and a bound function has no `prototype` property, so both throw (rule r2). A class cannot extend itself (rule r3). MDN: "Classes cannot extend regular (non-constructible) objects."[^1] To inherit from a plain object, set it as the prototype of the class's `prototype`, as the last lines do.

### Constructing a class that extends null

```javascript wrong id=extendsnull
try {
  new (class extends null {})();
} catch (e) {
  console.log(e.name);
}
class NullClass extends null {
  constructor() {
    return Object.create(new.target.prototype);
  }
}
const proto = Object.getPrototypeOf;
console.log(proto(proto(new NullClass())));
```

```output for=extendsnull
TypeError
null
```

MDN: "due to unsettled decisions about whether super() should be called within the constructor, it's not possible to construct such a class in practice using any constructor implementation that doesn't return an object."[^1] Return an object made with `Object.create(new.target.prototype)`, as the second class does.

## Comparison

Python allows several base classes, and its `super()` exists in part "to support cooperative multiple inheritance in a dynamic execution environment. This use case is unique to Python and is not found in statically compiled languages or languages that only support single inheritance."[^3] JavaScript is one of those single-inheritance languages: each class has one superclass, and mix-ins are built by chaining `extends` through functions (rule r5).

## References
