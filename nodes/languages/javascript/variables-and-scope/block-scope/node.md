---
tag: NEW-javascript-block-scope
kind: feature
owner: javascript
slug: block-scope
name: Block scope
aliases: [block scope, block statement, block-scoped, block-level function declaration, per-iteration binding, loop scope]
area: languages/javascript/variables-and-scope
level: core
summary: >-
  A block in braces is a scope for let, const and class, and for function declarations in strict mode. A var ignores the block. A for loop with let gets a fresh binding for each iteration.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-block", locator: "14.2 Block; 14.7.4.4 CreatePerIterationEnvironment"}
see_also: [javascript/let-and-const, javascript/var-declarations, javascript/lexical-scoping, javascript/the-temporal-dead-zone, javascript/statements-and-declarations, javascript/strict-mode, lexical-scope, python/python-scopes-and-namespaces]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/block
    title: Block statement, MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-block
    title: ECMAScript 2026 Language Specification
    locator: 14.2.2 Runtime Semantics, Evaluation; 14.7.4.4 CreatePerIterationEnvironment
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/function
    title: function, MDN
    locator: Block-level function declaration
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/let
    title: let, MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/executionmodel.html
    title: Execution model, The Python Language Reference
    locator: 4.1 Structure of a program; 4.2.2 Resolution of names
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/compound_stmts.html
    title: Compound statements, The Python Language Reference
    locator: 8.3 The for statement
    accessed: 2026-11-18
---

## Synopsis

```text
{
  StatementList
}
```

"A block statement is used to group zero or more statements." A block "is delimited by a pair of braces" and "contains a list of zero or more statements and declarations."[^1]

```javascript run id=synopsis
var x = 1;
let y = 1;
if (true) {
  var x = 2;
  let y = 2;
}
console.log(x, y);
```

```output for=synopsis
2 1
```

## Semantics

**r1. A scope for each run.** When a block runs, the specification makes a fresh record: "Let blockEnv be NewDeclarativeEnvironment ( oldEnv )." After the statements, it sets the LexicalEnvironment back "to oldEnv".[^2]

**r2. let, const and class.** "By contrast, identifiers declared with let, const, and class do have block scope".[^1] An inner `const` with an outer name "can be declared uniquely within the block."[^1]

**r3. var.** "Variables declared with var or created by function declarations in non-strict mode do not have block scope."[^1] Example: [`synopsis`](#synopsis).

**r4. Functions in strict mode.** "In strict mode, function declarations inside blocks are scoped to that block and are hoisted to the top of the block."[^1] Example: [`strict-fn`](#strict-fn).

**r5. Functions in sloppy mode.** "In non-strict mode, function declarations inside blocks behave strangely. Only declare functions in blocks if you are in strict mode."[^3]

**r6. Loop bodies.** A `let` in a `for` header has the body of the loop as its scope.[^4] For each iteration, CreatePerIterationEnvironment does: "Let thisIterationEnv be NewDeclarativeEnvironment ( outer )." It copies the last value into that record.[^2] Example: [`loop`](#loop).

## Examples

The examples run with Node.js 24.

A function declared in a block, in strict mode (rule r4):

```javascript fails id=strict-fn expect=exception:ReferenceError
"use strict";
{
  foo();
  function foo() {
    console.log("foo");
  }
}
foo();
```

```output for=strict-fn
foo
```

A fresh binding for each iteration (rule r6):

```javascript run id=loop
const fs = [];
for (let i = 0; i < 3; i++) {
  fs.push(() => i);
}
console.log(fs.map((f) => f()));
```

```output for=loop
[ 0, 1, 2 ]
```

A block keeps a temporary name out of the outer scope:

```javascript run id=temp
let total = 0;
{
  const step = 5;
  total += step * 2;
}
console.log(total, typeof step);
```

```output for=temp
10 undefined
```

## Pitfalls

### One block for a whole switch

The cases of a `switch` share one block, so a `let` with the same name in two cases is a SyntaxError. "To avoid the error, wrap each case in a new block statement."[^4]

### Function declarations in sloppy blocks

Do not declare functions in blocks outside strict mode (rule r5). Use a `const` with a function expression, which has block scope in each mode.

### A block is not a scope for var

The `var` in a block changes the outer variable (example [`synopsis`](#synopsis)). Use `let` or `const` for names that the block owns.

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| does `if` or `for` make a scope | yes, for `let`, `const`, `class`[^1] | no[^5] |
| what makes a scope | a block, a function, a module[^1] | "a module, a function body, and a class definition"[^5] |
| loop variable after the loop | not visible for `let`[^4] | "Names in the target list are not deleted when the loop is finished"[^6] |

In Python, a block is "a piece of Python program text that is executed as a unit."[^5] An `if` or `for` body is not such a block, so a name bound there stays visible after it. See [[python/python-scopes-and-namespaces]] and [[lexical-scope]].

## References
