---
tag: TK288W
kind: feature
owner: javascript
slug: decorators-in-javascript
name: Decorators in JavaScript
aliases: [decorators, "@decorator", TC39 decorators proposal, class decorators, method decorators, field decorators, accessor keyword, auto-accessors, experimentalDecorators]
area: languages/javascript/metaprogramming
level: research
summary: >-
  Decorators are a TC39 proposal, at Stage 2.7, not a part of ECMAScript 2026. A decorator is a function that gets a class or a class element and a context object, and can replace it.
grammar: null
spec: {standard: "TC39 proposal, Stage 2.7 (not in ECMA-262, 17th edition)", url: "https://github.com/tc39/proposal-decorators", locator: "README: Detailed Design; Class Auto-Accessors"}
see_also: [javascript/tc39-and-the-proposal-process, javascript/meta-programming, decorator-pattern, python/decorators]
sources:
- url: https://raw.githubusercontent.com/tc39/proposals/main/README.md
  title: TC39, ECMAScript proposals, README
  locator: Active proposals; Stage 2.7
  accessed: 2026-11-18
- url: https://raw.githubusercontent.com/tc39/proposal-decorators/master/README.md
  title: TC39, Decorators proposal, README
  locator: Introduction; Detailed Design; Class Auto-Accessors
  accessed: 2026-11-18
- url: https://www.typescriptlang.org/docs/handbook/decorators.html
  title: TypeScript Handbook, Decorators
  locator: the note at the top; Introduction
  accessed: 2026-11-18
- url: https://www.typescriptlang.org/docs/handbook/release-notes/typescript-5-0.html
  title: TypeScript 5.0 release notes
  locator: Decorators
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=by-hand
function logged(value, context) {
  if (context.kind !== "method") return;
  return function (...args) {
    const result = value.call(this, ...args);
    console.log(`${String(context.name)}(${args.join(", ")}) = ${result}`);
    return result;
  };
}

class Calc {
  add(a, b) {
    return a + b;
  }
}
// What @logged on add would do, written by hand:
const replaced = logged(Calc.prototype.add, { kind: "method", name: "add", static: false, private: false });
if (replaced !== undefined) Calc.prototype.add = replaced;

new Calc().add(2, 3);
```

```output for=by-hand
add(2, 3) = 5
```

With the proposal, the code writes `@logged add(a, b) { ... }` in the class. Node v24 does not parse that syntax, so this example calls the decorator by hand with a context object of the form that the proposal gives. The decorator returns a new method, and the new method replaces `add`.

```text
@decorator class C {}
class C {
  @decorator method() {}
  @decorator field = value;
  @decorator accessor name = value;
  @decorator get x() {}
}
```

| Kind | First argument | The decorator can return |
|---|---|---|
| `"class"` | the class | a new class |
| `"method"` | the method | a new method |
| `"getter"`, `"setter"` | the function | a new function |
| `"field"` | `undefined` | a function that changes the start value |
| `"accessor"` | `{ get, set }` | `{ get, set, init }` |

## Semantics

**r1.** Decorators are not in the standard. The TC39 list of active proposals "contains only stage 2 proposals and higher that have not yet been withdrawn/rejected, or become finished", and it has Decorators in its Stage 2.7 table.[^1] The README of the proposal says: "This README describes the current decorators proposal, which is a work in progress."[^2] See [[javascript/tc39-and-the-proposal-process]] for the stages.

**r2.** A decorator is a plain function. "In this proposal, decorators are plain functions, and are accessible and easy to write."[^2] It can go on a class, a field, a method, a getter, a setter or an auto-accessor, and each of them can be public, private or static.[^2] Example: [`by-hand`](#by-hand).

**r3.** "When decorators are called, they receive two parameters".[^2] The first is the value, or `undefined` for a field. The second is a context object with `kind`, `name`, `access`, `static`, `private` and `addInitializer`.[^2] A decorator returns a value of the same kind, or `undefined` to keep the old value. Example: [`by-hand`](#by-hand).

**r4.** The decorator expressions run first, and the calls come later. "Decorators are evaluated as expressions, being ordered along with computed property names. This goes left to right, top to bottom."[^2] The results are called "after the class definition initially finishes executing."[^2]

**r5.** `addInitializer` adds code that runs after the definition. "In cases where the value is a member of class, then initialization occurs once per instance."[^2] For a class decorator, the code runs when the class is complete. Example: [`register`](#register).

**r6.** The proposal adds the keyword `accessor` for a class field. "Auto-accessors, unlike regular fields, define a getter and setter on the class prototype. This getter and setter default to getting and setting a value on a private slot."[^2] A decorator on an auto-accessor can replace the getter and the setter, which it cannot do for a plain field. Example: [`accessor-by-hand`](#accessor-by-hand).

**r7.** TypeScript has two kinds of decorators. The handbook says that TypeScript 5.0 has support for the Stage 3 form of the proposal.[^3] The older kind needs a flag: "To enable experimental support for decorators, you must enable the experimentalDecorators compiler option either on the command line or in your tsconfig.json".[^3] TypeScript 5.0 says: "--experimentalDecorators will continue to exist for the foreseeable future; however, without the flag, decorators will now be valid syntax for all new code."[^4] The new kind "is not compatible with --emitDecoratorMetadata, and it does not allow decorating parameters."[^4] Example: [`legacy-signature`](#legacy-signature).

## Examples

```javascript run id=register
const registry = new Map();
function register(value, context) {
  if (context.kind === "class") {
    context.addInitializer(function () {
      registry.set(this.name, this);
    });
  }
}
let Point = class Point {};
const initializers = [];
const result = register(Point, { kind: "class", name: "Point", addInitializer: (f) => initializers.push(f) });
Point = result ?? Point;
for (const f of initializers) f.call(Point);
console.log([...registry.keys()], registry.get("Point") === Point);
```

```output for=register
[ 'Point' ] true
```

This is the work of `@register class Point {}`, by hand. The decorator returns nothing, so the class stays the same, and the initializer adds the class to a registry when the definition is complete (rule r5).

```javascript run id=accessor-by-hand
class Example {
  #x = 1;
  get x() {
    return this.#x;
  }
  set x(v) {
    this.#x = v;
  }
}
const e = new Example();
e.x = 5;
console.log(e.x, Object.keys(e), Object.getOwnPropertyNames(Example.prototype));
```

```output for=accessor-by-hand
5 [] [ 'constructor', 'x' ]
```

The proposal makes `accessor x = 1;` mean about the same as this class: a private slot, and a getter and a setter on the prototype (rule r6). So the object has no own property `x`.

```javascript run id=detect
try {
  new Function("function d(v) { return v; } return class { @d m() {} };");
  console.log("decorators: supported");
} catch (e) {
  console.log("decorators:", e.name);
}
```

```output for=detect
decorators: SyntaxError
```

`new Function` parses the string when it runs, so a program can test for the syntax without a parse error in its own file. Node v24 gives a `SyntaxError`.

## Versions

No edition of ECMA-262 has decorators, up to the 17th edition (ECMAScript 2026) (rule r1). The TC39 list has the proposal at Stage 2.7.[^1] The list links the notes of the TC39 meeting of May 2026 on decorators for Stage 2.7.[^1] TypeScript 5.0 added the proposal form, in the words of its handbook "Stage 3 decorator support".[^3] Node v24.10.0, which runs the examples, does not parse the syntax (see [`detect`](#detect)). Code that uses decorators today goes through a compiler, such as TypeScript.

## Pitfalls

### Decorator syntax in Node v24

```javascript fails id=no-syntax expect=exception:SyntaxError
function logged(value) {
  return value;
}
@logged class A {}
```

The engine does not know the `@` syntax, so the whole file fails to parse, and no line runs. Compile the file with a tool that supports the proposal, or call the decorator by hand as in [`by-hand`](#by-hand).

### A decorator for the old TypeScript form

```javascript fails id=legacy-signature expect=exception:TypeError
function readonly(target, key, descriptor) {
  descriptor.writable = false;
  return descriptor;
}
class Box {
  size() {
    return 1;
  }
}
readonly(Box.prototype.size, { kind: "method", name: "size" });
```

The function `readonly` is written for `experimentalDecorators`, which passes a target, a key and a property descriptor. The proposal passes a value and a context object, so `descriptor` is `undefined` (rule r7). A decorator library must say which form it supports.

## Comparison

In Python, decorators are in the language. The README of the proposal says that "In some other languages, like Python, decorators are syntactic sugar for this pattern".[^2] A Python decorator can go on any function or class. A JavaScript decorator goes only on a class or a class element, and it must return a value of the same kind (rule r3). For Python, see [[python/decorators]]. For the design pattern, see [[decorator-pattern]].

## References
