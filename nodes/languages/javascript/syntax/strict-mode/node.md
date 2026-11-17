---
tag: MV05PG
kind: feature
owner: javascript
slug: strict-mode
name: Strict mode
aliases: ["use strict", strict mode code, sloppy mode, Use Strict Directive]
area: languages/javascript/syntax
level: core
summary: >-
  Strict mode is a restricted variant of JavaScript. A "use strict" directive turns it on for a script or a function. Modules and class bodies are always strict.
grammar: "A Use Strict Directive is an ExpressionStatement in a Directive Prologue whose StringLiteral is 'use strict' or \"use strict\" (ECMA-262, 11.2.1)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-source-code.html#sec-strict-mode-code", locator: "11.2.1 Directive Prologues and the Use Strict Directive; 11.2.2 Strict Mode Code; Annex C"}
see_also: [javascript/expression-and-empty-statements, javascript/identifiers-and-reserved-words, javascript/unary-operators, javascript/statements-and-declarations, python/future-statements]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Strict_mode
    title: Strict mode, MDN
    locator: introduction; Invoking strict mode; Changes in strict mode
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-source-code.html#sec-strict-mode-code
    title: ECMAScript 2026 Language Specification
    locator: 11.2.1 Directive Prologues and the Use Strict Directive; 11.2.2 Strict Mode Code
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/strict-mode-of-ecmascript.html
    title: ECMAScript 2026 Language Specification, Annex C
    locator: Annex C The Strict Mode of ECMAScript
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/simple_stmts.html
    title: Simple statements, The Python Language Reference
    locator: 7.11.2 Future statements
    accessed: 2026-11-18
---

## Synopsis

```javascript run id=synopsis
function sloppy() {
  return this === globalThis;
}
function strict() {
  "use strict";
  return this;
}
console.log(sloppy(), strict());
```

```output for=synopsis
true undefined
```

MDN: "JavaScript's strict mode is a way to opt in to a restricted variant of JavaScript, thereby implicitly opting-out of" sloppy mode. "Strict mode isn't just a subset: it intentionally has different semantics from normal code."[^1]

## Semantics

**r1. The directive.** A Use Strict Directive is an expression statement in the Directive Prologue "whose StringLiteral is either of the exact code point sequences" of `use strict` in double or single quotes.[^2] The prologue is the sequence of string literal statements at the start of a script, module or function body.[^2] A directive after another statement is only an expression statement. Example: [`placement`](#placement).

**r2. Where strict mode applies.** "Strict mode applies to entire scripts or to individual functions . It doesn't apply to block statements enclosed in {} braces".[^1] The spec says: "Module code is always strict mode code." Also, "All parts of a ClassDeclaration or a ClassExpression are strict mode code."[^2] A function inside strict code is strict too. Example: [`placement`](#placement).

**r3. Simple parameters only.** The directive "can only be applied to the body of functions with simple parameters." With rest, default or destructured parameters, it is a syntax error.[^1] Example: [`params`](#params).

**r4. Silent mistakes become errors.** "Strict mode makes it impossible to accidentally create global variables." An assignment to an undeclared name throws a ReferenceError.[^1] An assignment to a non-writable property, a getter-only property or a property of a non-extensible object throws a TypeError.[^3] Setting a property on a primitive throws a TypeError.[^1] Example: [`errors`](#errors).

**r5. Syntax errors.** In strict mode, these are syntax errors: the `with` statement, `delete` on a plain name, duplicate parameter names, legacy octal literals such as `015`, and octal escapes such as `\45`.[^1] Example: [`syntax`](#syntax).

**r6. No `this` substitution.** "For a sloppy mode function, this is always an object". In a strict function, `this` "is not boxed into an object, and if unspecified, this is undefined instead of globalThis".[^1] Example: [`this`](#this).

**r7. `arguments` and `eval`.** Arguments objects for strict functions "do not dynamically share their array-indexed property values with the corresponding formal parameter bindings".[^3] "In strict mode, eval does not introduce new variables into the surrounding scope ."[^1] Example: [`arguments`](#arguments).

**r8. Extra reserved words.** "implements , interface , let , package , private , protected , public , static , and yield are reserved words within strict mode code".[^3]

## Examples

The examples run with Node.js 24, as CommonJS files. The `sloppy` function in the synopsis shows that such a file is not strict without the directive.

The directive works only at the start, and it reaches nested functions (rules r1 and r2):

```javascript run id=placement
function late() {
  const x = 1;
  "use strict";
  return this === undefined;
}
function outer() {
  "use strict";
  function inner() {
    return this === undefined;
  }
  return inner();
}
class Box {
  check() {
    return (function () { return this; })();
  }
}
console.log(late(), outer(), new Box().check());
```

```output for=placement
false true undefined
```

In `late`, the string comes after a declaration, so it is not a directive. The method in `Box` is class code, so the inner function is strict.

The directive with a default parameter (rule r3):

```javascript fails id=params expect=exception:SyntaxError
function sum(a = 1, b = 2) {
  "use strict";
  return a + b;
}
```

Node.js reports `SyntaxError: Illegal 'use strict' directive in function with non-simple parameter list`.

Runtime errors in strict mode (rule r4):

```javascript run id=errors
"use strict";
const attempts = {
  undeclared: () => { mistypeVarible = 17; },
  readOnly: () => { const o = {}; Object.defineProperty(o, "x", { value: 42 }); o.x = 9; },
  getterOnly: () => { const o = { get x() { return 17; } }; o.x = 5; },
  frozen: () => { const o = Object.preventExtensions({}); o.added = 1; },
  primitive: () => { (14).sailing = "home"; },
  nan: () => { NaN = 5; },
};
for (const [name, attempt] of Object.entries(attempts)) {
  try {
    attempt();
    console.log(name, "no error");
  } catch (err) {
    console.log(name, err.name);
  }
}
```

```output for=errors
undeclared ReferenceError
readOnly TypeError
getterOnly TypeError
frozen TypeError
primitive TypeError
nan TypeError
```

Without the directive, each of these assignments fails with no error, and the first one makes a global variable.[^1]

A legacy octal literal (rule r5):

```javascript fails id=syntax expect=exception:SyntaxError
"use strict";
const sum = 015 + 197;
```

Node.js reports `SyntaxError: Octal literals are not allowed in strict mode.` Use `0o15` for an octal literal.[^1]

The value of `this` (rule r6), from the MDN example:[^1]

```javascript run id=this
"use strict";
function fun() {
  return this;
}
console.log(fun(), fun.call(2), fun.apply(null), fun.bind(true)());
```

```output for=this
undefined 2 null true
```

`arguments` does not track the parameter (rule r7):

```javascript run id=arguments
function strictF(a) {
  "use strict";
  a = 42;
  return [a, arguments[0]];
}
function sloppyF(a) {
  a = 42;
  return [a, arguments[0]];
}
console.log(strictF(17), sloppyF(17));
```

```output for=arguments
[ 42, 17 ] [ 42, 42 ]
```

## Pitfalls

### Joined files

The directive applies to the whole script only at the start. If a build step puts a sloppy file in front of a strict file, the string is no longer in the Directive Prologue, and the combined script is sloppy (rule r1). Function-level directives do not have this problem.

### Blocks do nothing

`{ 'use strict'; }` does not make a block strict. "attempting to apply it to such contexts does nothing."[^1]

### Modules and classes are already strict

Code in a module or a class body is strict with no directive (rule r2). Code moved from a sloppy script into a module can start to throw, for example on an assignment to an undeclared name.

## Comparison

Python has no strict mode. Its nearest form is a per-module opt-in. "A future statement is a directive to the compiler that a particular module should be compiled using syntax or semantics that will be available in a specified future release of Python".[^4] See [[python/future-statements]].

| Topic | JavaScript strict mode | Python future statement |
|---|---|---|
| form | `'use strict';`, a string statement[^2] | `from __future__ import name`[^4] |
| scope | a script or a function[^1] | a module[^4] |
| on by default | modules and classes[^2] | when the feature becomes standard[^4] |

## References
