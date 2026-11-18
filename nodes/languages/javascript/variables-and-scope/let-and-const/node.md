---
tag: NEW-javascript-let-and-const
kind: feature
owner: javascript
slug: let-and-const
name: let and const
aliases: [let, const, lexical declaration, LexicalDeclaration, block-scoped variable, immutable binding]
area: languages/javascript/variables-and-scope
level: core
summary: >-
  let and const declare block-scoped variables that cannot be read before the declaration runs or declared again in the same scope. A const binding cannot be assigned again.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-let-and-const-declarations", locator: "14.3.1 Let and Const Declarations"}
see_also: [javascript/var-declarations, javascript/declarations, javascript/block-scope, javascript/the-temporal-dead-zone, javascript/hoisting, declare-a-constant, single-assignment, python/constants-by-convention]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/let
    title: let, MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/const
    title: const, MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-let-and-const-declarations
    title: ECMAScript 2026 Language Specification
    locator: 14.3.1 Let and Const Declarations, Note
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/executionmodel.html
    title: Execution model, The Python Language Reference
    locator: 4.2.1 Binding of names
    accessed: 2026-11-18
---

## Synopsis

```text
let name1 [= value1] [, name2 [= value2]] ...;
const name1 = value1 [, name2 = value2] ...;
```

"The let declaration declares re-assignable, block-scoped local variables, optionally initializing each to a value."[^1] "The const declaration declares block-scoped local variables. The value of a constant can't be changed through reassignment using the assignment operator, but if a constant is an object, its properties can be added, updated, or removed."[^2]

```javascript run id=synopsis
let x = 1;
{
  let x = 2;
  const y = 3;
  console.log(x, y);
}
console.log(x, typeof y);
```

```output for=synopsis
2 3
1 undefined
```

## Semantics

**r1. The scope.** The scope of a `let` is the closest block statement, `switch`, `try...catch`, the body of a `for` when the `let` is in its header, the function body or the static initialization block. Otherwise it is the module or the global scope.[^1]

**r2. The differences from var.** "let declarations are scoped to blocks as well as functions." "let declarations do not create properties on globalThis when declared at the top level of a script." "let declarations cannot be redeclared by any other declaration in the same scope."[^1] The same rules apply to `const`.[^2]

**r3. Access before the declaration.** "The variables are created when their containing Environment Record is instantiated but may not be accessed in any way until the variable's LexicalBinding is evaluated."[^3] Any access in that zone "will result in a ReferenceError".[^1] See [[javascript/the-temporal-dead-zone]].

**r4. A declaration is not a statement.** "That means you cannot use a lone let declaration as the body of a block".[^1] Example: [`lone`](#lone).

**r5. const needs an initializer.** "An initializer for a constant is required. You must specify its value in the same declaration."[^2]

**r6. Binding, not value.** "The const declaration creates an immutable reference to a value." MDN says to read const as "create immutable bindings", not "immutable values".[^2] Example: [`assign`](#assign).

## Examples

The examples run with Node.js 24.

An assignment to a const throws at run time (rule r6). A change to its object does not:

```javascript fails id=assign expect=exception:TypeError
const config = { debug: false };
config.debug = true;
console.log(config.debug);
config = {};
```

```output for=assign
true
```

A lone declaration as the body of `if` (rule r4):

```javascript fails id=lone expect=compile:SyntaxError
if (true) let a = 1;
```

The same name declared twice with `let` (rule r2):

```javascript fails id=twice expect=compile:SyntaxError
let count = 1;
let count = 2;
```

## Pitfalls

### const and Object.freeze

A const object can still change. "You would need to use Object.freeze() to make an object immutable."[^2]

### Shadowing in a block

A `let` in an inner block makes a second variable with the same name (example [`synopsis`](#synopsis)). The outer variable does not change.

### let or const

"Many style guides (including MDN's) recommend using const over let whenever a variable is not reassigned in its scope."[^1]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| block scope | `let` and `const`[^1] | no; blocks are modules, function bodies and class definitions[^4] |
| constant binding | `const`[^2] | no keyword |
| declare twice | SyntaxError[^1] | no declaration |
| read before the value | ReferenceError[^1] | UnboundLocalError in a function[^4] |

In Python, "If the current scope is a function scope, and the name refers to a local variable that has not yet been bound to a value at the point where the name is used, an UnboundLocalError exception is raised."[^4] That is close to the `let` dead zone. See [[python/constants-by-convention]] and [[single-assignment]].

## References
