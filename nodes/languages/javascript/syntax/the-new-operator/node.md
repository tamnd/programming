---
tag: C2H9TY
kind: feature
owner: javascript
slug: the-new-operator
name: The new operator
aliases: ["new", new expression, constructor call, "new.target"]
area: languages/javascript/syntax
level: core
summary: >-
  new C(args) calls a constructor and gives the object it makes. Without parentheses, new C calls C with no arguments. A value that is not a constructor gives a TypeError.
grammar: "NewExpression : MemberExpression | new NewExpression; MemberExpression : new MemberExpression Arguments (ECMA-262, 13.3)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-new-operator", locator: "13.3.5 The new Operator"}
see_also: [javascript/property-accessors, javascript/operator-precedence, javascript/optional-chaining, javascript/unary-operators, python/callable-objects]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/new
    title: new, MDN
    locator: introduction; Syntax; Description
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-new-operator
    title: ECMAScript 2026 Language Specification
    locator: 13.3.5 The new Operator; 13.3.5.1.1 EvaluateNew
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Operator_precedence
    title: Operator precedence, MDN
    locator: Table; precedence 17 and 16
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Strict_mode
    title: Strict mode, MDN
    locator: No this substitution
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.3.3 Calls
    accessed: 2026-11-18
---

## Synopsis

```javascript run id=synopsis
function Car(make, model, year) {
  this.make = make;
  this.model = model;
  this.year = year;
}
const car1 = new Car("Eagle", "Talon TSi", 1993);
console.log(car1.make, car1 instanceof Car);
```

```output for=synopsis
Eagle true
```

MDN: "The new operator lets developers create an instance of a user-defined object type or of one of the built-in object types that has a constructor function."[^1] The operand is "A class or function that specifies the type of the object instance."[^1]

## Semantics

**r1. Two forms.** The grammar has `new MemberExpression Arguments` and `new NewExpression`.[^2] MDN: "new Foo is equivalent to new Foo() , i.e., if no argument list is specified, Foo is called without arguments."[^1]

**r2. Evaluation order.** EvaluateNew first evaluates the constructor expression, then the arguments. Then: "If IsConstructor ( constructor ) is false , throw a TypeError exception." If the value is a constructor, the result is Construct with the argument list.[^2] Example: [`not-constructor`](#not-constructor).

**r3. The four steps.** For a constructor function, `new` does these things:[^1]

1. It "Creates a blank, plain JavaScript object."
2. It sets the [[Prototype]] of the object to the `prototype` property of the constructor, "if the prototype is an Object".
3. It runs the constructor with the arguments, with the object as `this`.
4. "If the constructor function returns a non-primitive , this return value becomes the result of the whole new expression." If it returns a primitive or nothing, the result is the object from step 1.

Example: [`steps`](#steps).

**r4. Classes need `new`.** "Classes can only be instantiated with the new operator" and a call without `new` "will throw a TypeError".[^1] Example: [`class-call`](#class-call).

**r5. `new.target`.** "A function can know whether it is invoked with new by checking new.target ." Also, "new.target is only undefined when the function is invoked without new ."[^1] Example: [`target`](#target).

**r6. Built-ins differ.** MDN: "Symbol() and BigInt() can only be called without new" and "Proxy and Map can only be constructed with new". The wrong form gives a TypeError. `Boolean()`, `Number()` and `String()` "coerce their argument to the respective primitive type when called, and return wrapper objects when constructed."[^1] Example: [`builtins`](#builtins).

**r7. Precedence.** `new x(y)`, with an argument list, is at precedence 17, with member access and calls. `new x`, without an argument list, is at precedence 16.[^3] MDN lists "new with arguments ( new a().b )" as a left side of member access, and "a member access ( new a.b() )" as a left side of `new` with arguments.[^3] The operand can not use optional chaining.[^1] Example: [`precedence`](#precedence).

## Examples

The examples run with Node.js 24.

A non-constructor value gives a TypeError (rule r2):

```javascript fails id=not-constructor expect=exception:TypeError
const notAConstructor = 42;
const made = new notAConstructor();
```

Node.js reports `TypeError: notAConstructor is not a constructor`.

The prototype link and the return value (rule r3):

```javascript run id=steps
function Point(x) {
  this.x = x;
}
Point.prototype.show = function () {
  return "x=" + this.x;
};
const p = new Point(3);
console.log(Object.getPrototypeOf(p) === Point.prototype, p.show());

function ReturnsObject() {
  this.lost = true;
  return { replaced: true };
}
function ReturnsPrimitive() {
  this.kept = true;
  return 5;
}
console.log(new ReturnsObject(), new ReturnsPrimitive());
```

```output for=steps
true x=3
{ replaced: true } ReturnsPrimitive { kept: true }
```

A class called without `new` (rule r4):

```javascript fails id=class-call expect=exception:TypeError
class Account {}
const account = Account();
```

Node.js reports `TypeError: Class constructor Account cannot be invoked without 'new'`.

A function that checks `new.target` (rule r5). This is the MDN example:[^1]

```javascript run id=target
function Car(color) {
  if (!new.target) {
    return `${color} car`;
  }
  this.color = color;
}
const a = Car("red");
const b = new Car("red");
console.log(a, b);
```

```output for=target
red car Car { color: 'red' }
```

Built-in constructors (rule r6):

```javascript run id=builtins
console.log(typeof Number("5"), typeof new Number("5"));
console.log(new Map([["a", 1]]).size, typeof Symbol("s"));
for (const attempt of [() => new Symbol("s"), () => Map()]) {
  try {
    attempt();
  } catch (err) {
    console.log(err.name);
  }
}
```

```output for=builtins
number object
1 symbol
TypeError
TypeError
```

Precedence with member access (rule r7):

```javascript run id=precedence
const ns = {
  Box: function (v) {
    this.v = v;
  },
};
ns.Box.prototype.get = function () {
  return this.v;
};
console.log(new ns.Box(7).get());
console.log((new ns.Box).v, new Date(0).getTime());
```

```output for=precedence
7
undefined 0
```

`new ns.Box(7).get()` reads as `(new ns.Box(7)).get()`. The member access `ns.Box` is the operand of `new`, and `.get()` applies to the result.

## Pitfalls

### Missing `new` on a function

A constructor function called without `new` does not make an object. MDN: "in this case a new object is not created and the value of this is also different."[^1] For a sloppy mode function, `this` is "the global object, if called with undefined or null as this".[^4] So the assignments make properties on the global object:

```javascript run id=missing-new
function Person(name) {
  this.name = name;
}
const result = Person("Ada");
console.log(result, globalThis.name);
```

```output for=missing-new
undefined Ada
```

Use classes, or check `new.target` (rule r5).

### `new` on a call result

`new f()()` is `(new f())()`, and `new f().g()` calls `g` on the object. To construct the result of a call, use parentheses: `new (f())()`.[^3]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| make an instance | `new C(args)`[^1] | `C(args)`; for a class object, "A new instance of that class is returned."[^5] |
| class called as a function | TypeError[^1] | the normal way to make an instance[^5] |
| operator form | `new` is part of the expression grammar[^2] | a normal call expression[^5] |

In Python, a plain call on a class makes the instance. See [[python/callable-objects]].

## References
