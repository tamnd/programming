---
tag: 8FGNYS
kind: feature
owner: javascript
slug: constructor
name: Constructor
aliases: [constructor method, class constructor, default constructor, new.target, constructor return value]
area: languages/javascript/user-types
level: intro
summary: >-
  The constructor method of a JavaScript class sets up each new instance. A class has at most one, gets a default one if none is written, and a derived class must call super() before using this.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html#sec-ecmascript-function-objects-construct-argumentslist-newtarget", locator: "10.2.2 [[Construct]]"}
see_also: [javascript/classes, javascript/constructors, javascript/inheritance-and-the-prototype-chain, python/class-definition-syntax]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes/constructor
  title: MDN Web Docs, JavaScript Reference, constructor
  locator: the introduction; Syntax; Description; Examples
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-functions-and-classes.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 15
  locator: 15.7.1 Static Semantics, Early Errors
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 10
  locator: 10.2.2 [[Construct]]
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/new.target
  title: MDN Web Docs, JavaScript Reference, new.target
  locator: Value
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html
  title: The Java Language Specification, Java SE 25 Edition, chapter 8
  locator: 8.8.8 Constructor Overloading; 8.8.9 Default Constructor
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/datamodel.html
  title: The Python Language Reference, Data model, Python 3.14
  locator: 3.3.1 Basic customization
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
class Account {
  constructor(owner, balance = 0) {
    this.owner = owner;
    this.balance = balance;
  }
  deposit(amount) {
    this.balance += amount;
    return this.balance;
  }
}
const a = new Account("Ada");
console.log(a.owner, a.balance, a.deposit(50));
console.log(Object.keys(a));
```

```output for=basics
Ada 0 50
[ 'owner', 'balance' ]
```

| Case | What happens |
|---|---|
| no `constructor` in a base class | a default `constructor() {}` |
| no `constructor` in a derived class | a default that passes all arguments to `super(...args)` |
| `return` of an object | `new` gives that object instead of `this` |
| `return` of a primitive | ignored in a base class, TypeError in a derived class |
| `this` before `super()` in a derived class | ReferenceError |

## Semantics

**r1.** "The constructor method is a special method of a class for creating and initializing an object instance of that class."[^1] "A constructor enables you to provide any custom initialization that must be done before any other methods can be called on an instantiated object."[^1] It "follows normal method syntax, so parameter default values, rest parameters, etc. can all be used."[^1] Example: [`basics`](#basics).

**r2.** "If you don't provide your own constructor, then a default constructor will be supplied for you. If your class is a base class, the default constructor is empty".[^1] "If your class is a derived class, the default constructor calls the parent constructor, passing along any arguments that were provided".[^1] Example: [`defaults`](#defaults).

**r3.** "A class cannot have more than one constructor method."[^1] ECMA-262 makes a second one an early error: "It is a Syntax Error if the PrototypePropertyNameList of ClassElementList contains more than one occurrence of" the name.[^2] Also, "A non-static class method with the non-computed name constructor cannot be a getter, setter, async, or generator."[^1] Example: [`restrictions`](#restrictions).

**r4.** "The constructor method may have a return value. While the base class may return anything from its constructor, the derived class must return an object or undefined , or a TypeError will be thrown."[^1] ECMA-262, at the end of [[Construct]]: "If result.[[Value]] is an Object, return result.[[Value]]. If kind is base, return thisArgument. If result.[[Value]] is not undefined, throw a TypeError exception."[^3] Example: [`returns`](#returns).

**r5.** "Within the constructor body, you can access the object being created through this and access the class that is called with new through new.target ."[^1] In class constructors, `new.target` "refers to the class that new was called upon, which may be a subclass of the current constructor, because subclasses transitively call the superclass's constructor through super() ."[^4] Example: [`newtarget`](#newtarget).

**r6.** In a derived class, "if you provide your own constructor, and your class derives from some parent class, then you must explicitly call the parent class constructor using super() ."[^1] MDN's example notes that `super()` must come before any use of `this`: "Leaving this out will cause a ReferenceError."[^1] Example: [`nosuper`](#nosuper).

## Examples

Default constructors (rule r2).

```javascript run id=defaults
class Empty {}
class Base {
  constructor(a, b) {
    this.args = [a, b];
  }
}
class Derived extends Base {}
console.log(new Empty(), new Derived(1, 2).args);
```

```output for=defaults
Empty {} [ 1, 2 ]
```

Constructors that the grammar refuses (rule r3). Each class is compiled with `eval` so that the error can be caught.

```javascript run id=restrictions
const sources = [
  "class A { constructor() {} constructor() {} }",
  "class B { get constructor() { return 1; } }",
  "class C { async constructor() {} }",
];
for (const src of sources) {
  try {
    eval(src);
  } catch (e) {
    console.log(e.name);
  }
}
```

```output for=restrictions
SyntaxError
SyntaxError
SyntaxError
```

Return values (rule r4).

```javascript run id=returns
class Plain {
  constructor() {
    this.made = true;
    return 42;
  }
}
const shared = { shared: true };
class Cached {
  constructor() {
    return shared;
  }
}
class Child extends Plain {
  constructor() {
    super();
    return 1;
  }
}
console.log(new Plain(), new Cached() === shared);
try {
  new Child();
} catch (e) {
  console.log(e.name);
}
```

```output for=returns
Plain { made: true } true
TypeError
```

Using `new.target` to refuse direct construction of a base class (rule r5).

```javascript run id=newtarget
class Shape {
  constructor() {
    if (new.target === Shape) throw new TypeError("Shape needs a subclass");
    this.kind = new.target.name;
  }
}
class Circle extends Shape {}
console.log(new Circle().kind);
try {
  new Shape();
} catch (e) {
  console.log(e.message);
}
```

```output for=newtarget
Circle
Shape needs a subclass
```

## Pitfalls

### Using this before super()

```javascript wrong id=nosuper
class Animal {
  constructor(name) {
    this.name = name;
  }
}
class Dog extends Animal {
  constructor(name) {
    this.tricks = [];
    super(name);
  }
}
try {
  new Dog("Rex");
} catch (e) {
  console.log(e.name);
}
class GoodDog extends Animal {
  constructor(name) {
    super(name);
    this.tricks = [];
  }
}
console.log(new GoodDog("Rex"));
```

```output for=nosuper
ReferenceError
GoodDog { name: 'Rex', tricks: [] }
```

In a derived class, `this` exists only after `super()` returns (rule r6). Call `super()` first.

### A computed constructor name

```javascript wrong id=computed
class Foo {
  ["constructor"]() {
    this.ready = true;
  }
}
const foo = new Foo();
console.log(foo.ready, foo.constructor === Foo);
```

```output for=computed
undefined false
```

MDN: "The constructor must be a literal name. Computed properties cannot become constructors."[^1] The computed method is an ordinary method, so `new Foo()` does not run it, and it replaces the `constructor` property of `Foo.prototype`. Write `constructor() { ... }`.

## Comparison

A Java class can have several constructors: "Overloading of constructors is identical in behavior to overloading of methods (§8.4.9)."[^5] A JavaScript class has one (rule r3), and uses default or rest parameters for other call shapes. Both languages supply a default: in Java, "If a class contains no constructor declarations, then a default constructor is implicitly declared."[^5] Python's `__init__()` must not return a value other than None, because "doing so will cause a TypeError to be raised at runtime."[^6] A base JavaScript constructor may return an object, and `new` then gives that object (rule r4).

## References
