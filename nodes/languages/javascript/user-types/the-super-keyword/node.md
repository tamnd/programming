---
tag: A27V0D
kind: feature
owner: javascript
slug: the-super-keyword
name: The super keyword
aliases: [super, super call, super property, calling the parent constructor, calling an overridden method, super in object literals]
area: languages/javascript/user-types
level: core
summary: >-
  In JavaScript, super(...) calls the parent constructor from a derived constructor, and super.name reads a property from the prototype of the class or object literal that defines the method.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-super-keyword", locator: "13.3.7 The super Keyword"}
see_also: [javascript/extends-and-inheritance, javascript/constructor, javascript/instance-methods, javascript/static-properties, javascript/public-fields, python/super-and-cooperative-inheritance]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/super
  title: MDN Web Docs, JavaScript Reference, super
  locator: the introduction; Description; Examples
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 13
  locator: 13.3.7.1 Runtime Semantics, Evaluation; 13.3.7.2 GetSuperConstructor; 13.3.7.3 MakeSuperPropertyReference
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-functions-and-classes.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 15
  locator: 15.2.1 Static Semantics, Early Errors
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/functions.html
  title: The Python Standard Library, Python 3.14, Built-in Functions
  locator: super
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
class Shape {
  constructor(name) {
    this.name = name;
  }
  describe() {
    return `a ${this.name}`;
  }
  static unit() {
    return "cm";
  }
}
class Square extends Shape {
  constructor(side) {
    super("square");
    this.side = side;
  }
  describe() {
    return `${super.describe()} with side ${this.side}`;
  }
  static unit() {
    return `square ${super.unit()}`;
  }
}
console.log(new Square(2).describe());
console.log(Square.unit());
```

```output for=basics
a square with side 2
square cm
```

| Form | Where it is allowed | What it does |
|---|---|---|
| `super(...args)` | constructor of a class with `extends` | calls the parent constructor and binds `this` |
| `super.name`, `super[expr]` | any method, in a class or an object literal; field initializers | reads from the home object's prototype, with the current `this` |
| `super.name = v` | the same places | sets `name` on `this` |
| `super` alone | nowhere | SyntaxError |

## Semantics

**r1.** "The super keyword is used to access properties on an object literal or class's [[Prototype]], or invoke a superclass's constructor. The super.prop and super[expr] expressions are valid in any method definition in both classes and object literals. The super(...args) expression is valid in class constructors."[^1] Example: [`basics`](#basics), [`literal`](#literal).

**r2.** In a derived constructor, `super(...)` "must be called before the this keyword is used, and before the constructor returns."[^1] ECMA-262 finds the parent through the prototype of the running class, "Let superConstructor be ! activeFunction.[[GetPrototypeOf]]().", then runs "Let result be ? Construct(func, argList, newTarget)." and "Perform ? BindThisValue(thisER, result)."[^2] After that it adds the derived class's own fields: "Perform ? InitializeInstanceElements(result, F)."[^2] Example: [`basics`](#basics).

**r3.** "Within a class's body, the reference of super can be either the superclass's constructor itself, or the constructor's prototype , depending on whether the execution context is instance creation or class initialization."[^1] In instance methods and instance field initializers, `super.x` looks on the parent's `prototype`. In static methods and static field initializers, it looks on the parent class. Example: [`basics`](#basics), [`fields`](#fields).

**r4.** "Note that the reference of super is determined by the class or object literal super was declared in, not the object the method is called on. Therefore, unbinding or re-binding a method doesn't change the reference of super in it (although they do change the reference of this )."[^1] ECMA-262 builds the reference from the method's home object and keeps `this` apart: "Let baseValue be GetSuperBase(env)." and "Return the Reference Record { [[Base]]: baseValue, [[ReferencedName]]: propertyKey, [[Strict]]: strict, [[ThisValue]]: actualThis }."[^2] Example: [`homeobject`](#homeobject).

**r5.** "When calling super.prop as a function, the this value inside the prop function is the current this , not the object that super points to."[^1] Example: [`homeobject`](#homeobject).

**r6.** "super can also be accessed during class field initialization. The reference of super depends on whether the current field is an instance field or a static field."[^1] A parent's instance field is an own property of the instance, not of the parent's prototype, so "you can't use super to access the instance field of a superclass."[^1] Example: [`fields`](#fields).

**r7.** "super is not a variable that points to the prototype object. Attempting to read super itself is a SyntaxError ."[^1] A plain function cannot use it either: "It is a Syntax Error if FunctionBody Contains SuperProperty is true."[^3] Deleting `super.prop` throws a ReferenceError at run time.[^1] Example: [`syntax`](#syntax).

## Examples

`super` in an object literal (rule r1).

```javascript run id=literal
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

```output for=literal
hello from child
```

What `super` and `this` refer to (rules r4 and r5).

```javascript run id=homeobject
class Base {
  who() {
    return "Base.prototype.who";
  }
  static getName() {
    return this.name;
  }
}
class Extended extends Base {
  getX() {
    return super.who();
  }
  static getName() {
    return `called on ${super.getName()}`;
  }
}
const { getX } = new Extended();
console.log(getX());
const other = {
  __proto__: { who: () => "other parent" },
  getX,
};
console.log(other.getX());
console.log(Extended.getName());
```

```output for=homeobject
Base.prototype.who
Base.prototype.who
called on Extended
```

The detached and the borrowed `getX` still reach `Base.prototype`. The static call runs the parent method with `this` set to `Extended`.

`super` in field initializers (rule r6).

```javascript run id=fields
class Base {
  static baseStatic = 90;
  baseField = 10;
  baseMethod() {
    return 10;
  }
}
class Extended extends Base {
  fromMethod = super.baseMethod();
  fromField = super.baseField;
  static fromStatic = super.baseStatic;
}
const e = new Extended();
console.log(e.fromMethod, e.fromField, Extended.fromStatic, e.baseField);
```

```output for=fields
10 undefined 90 10
```

Forms that are refused (rule r7). The first two are compiled with `eval` so that the error can be caught.

```javascript run id=syntax
const sources = ["({ m() { return super; } })", "function f() { return super.x; }"];
for (const src of sources) {
  try {
    eval(src);
  } catch (e) {
    console.log(e.name);
  }
}
class Base {
  foo() {}
}
class Derived extends Base {
  remove() {
    delete super.foo;
  }
}
try {
  new Derived().remove();
} catch (e) {
  console.log(e.name);
}
```

```output for=syntax
SyntaxError
SyntaxError
ReferenceError
```

## Pitfalls

### Expecting super.x = v to change the parent

```javascript wrong id=setsuper
class A {}
class B extends A {
  setX() {
    super.x = 1;
  }
}
const b = new B();
b.setX();
console.log(Object.hasOwn(b, "x"), Object.hasOwn(A.prototype, "x"));
```

```output for=setsuper
true false
```

MDN: "When setting properties through super , the property is set on this instead."[^1] To change the shared prototype, assign to `A.prototype.x` by name.

### Using this before super()

```javascript wrong id=beforesuper
class Base {
  constructor(name) {
    this.name = name;
  }
}
class Child extends Base {
  constructor(name) {
    try {
      this.tag = "child";
    } catch (e) {
      console.log(e.name);
    }
    super(name);
    this.tag = "child";
  }
}
const c = new Child("c");
console.log(c.name, c.tag);
```

```output for=beforesuper
ReferenceError
c child
```

Until `super(...)` returns, a derived constructor has no `this` (rule r2). Compute what you need from the arguments, call `super(...)`, then touch `this`.

## Comparison

Python's `super()` is a call that returns an object: it will "Return a proxy object that delegates method calls to a parent or sibling class of type."[^4] Its zero-argument form "will not work as expected within nested functions, including generator expressions".[^4] JavaScript's `super` is syntax, not a value, so it cannot be stored or passed around (rule r7), and the object it searches is fixed by the method's home object (rule r4).

## References
