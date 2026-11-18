---
tag: NEW-javascript-var-declarations
kind: feature
owner: javascript
slug: var-declarations
name: var declarations
aliases: [var, var statement, function-scoped variable, VariableStatement, var hoisting, var redeclaration]
area: languages/javascript/variables-and-scope
level: core
summary: >-
  The var statement declares function-scoped or global variables. A var is hoisted and holds undefined until its initializer runs. Blocks do not scope it, and it can be declared again.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-variable-statement", locator: "14.3.2 Variable Statement"}
see_also: [javascript/declarations, javascript/let-and-const, javascript/hoisting, javascript/block-scope, javascript/the-global-scope-and-globalthis, javascript/function-scopes-and-closures, variable-lifetime, python/naming-and-binding]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/var
    title: var, MDN
    locator: Description; Hoisting; Redeclarations
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-variable-statement
    title: ECMAScript 2026 Language Specification
    locator: 14.3.2 Variable Statement, Note
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Closures
    title: Closures, MDN
    locator: Creating closures in loops, a common mistake
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/executionmodel.html
    title: Execution model, The Python Language Reference
    locator: 4.2.1 Binding of names; 4.2.2 Resolution of names
    accessed: 2026-11-18
---

## Synopsis

```text
var name1;
var name1 = value1;
var name1 = value1, name2 = value2;
var { key } = object;
```

"The var statement declares function-scoped or globally-scoped variables, optionally initializing each to a value."[^1]

```javascript run id=synopsis
function f() {
  console.log(x);
  if (true) {
    var x = 1;
  }
  console.log(x);
}
f();
```

```output for=synopsis
undefined
1
```

## Semantics

**r1. The scope.** The scope of a `var` is the function body or the static initialization block that most closely contains it. Otherwise it is "The current module, for code running in module mode" or "The global scope, for code running in script mode."[^1]

**r2. Blocks do not scope it.** "Importantly, other block constructs, including block statements, try...catch, switch, headers of one of the for statements, do not create scopes for var".[^1] Example: [`synopsis`](#synopsis).

**r3. Hoisting.** "var declarations, wherever they occur in a script, are processed before any code within the script is executed." "Only a variable's declaration is hoisted, not its initialization."[^1] In the specification, "Var variables are created when their containing Environment Record is instantiated and are initialized to undefined when created."[^2]

**r4. Redeclaration.** "Duplicate variable declarations using var will not trigger an error, even in strict mode, and the variable will not lose its value, unless the declaration has an initializer."[^1] The specification says such declarations "collectively define only one variable."[^2] Example: [`redeclare`](#redeclare).

**r5. Conflicts.** "var declarations cannot be in the same scope as a let, const, class, or import declaration."[^1] Example: [`conflict`](#conflict).

**r6. Global property.** "In a script, a variable declared using var is added as a non-configurable property of the global object." In a CommonJS module or an ECMAScript module, top-level declarations "are not added as properties to the global object."[^1] See [[javascript/the-global-scope-and-globalthis]].

## Examples

The examples run with Node.js 24.

Redeclaration and a function with the same name (rule r4):

```javascript run id=redeclare
var a = 1;
var a;
console.log(a);
console.log(typeof g);
function g() {}
var g;
console.log(typeof g);
```

```output for=redeclare
1
function
function
```

A `var` and a `let` in the same scope (rule r5):

```javascript fails id=conflict expect=compile:SyntaxError
let a = 1;
{
  var a = 2;
}
```

A loop counter declared with `var` is one variable for all the closures:

```javascript run id=loop
const fs = [];
for (var i = 0; i < 3; i++) fs.push(() => i);
const gs = [];
for (let j = 0; j < 3; j++) gs.push(() => j);
console.log(fs.map((f) => f()), gs.map((g) => g()));
```

```output for=loop
[ 3, 3, 3 ] [ 0, 1, 2 ]
```

## Pitfalls

### Closures in loops

In the MDN example, the closures made in a loop share "the same single lexical environment". "This is because the variable item is declared with var and thus has function scope due to hoisting."[^3] Use `let` or `const` in the loop. Example: [`loop`](#loop).

### A var in a block leaks out

A `var` inside `if` or `for` is visible in the whole function (rule r2). Code after the block can read a value that the block meant to keep local.

### Read before the assignment

A read before the initializer gives `undefined`, not an error (rule r3). A `let` read at the same place throws. See [[javascript/the-temporal-dead-zone]].

## Comparison

| Topic | JavaScript `var` | Python |
|---|---|---|
| scope | function, module or global[^1] | the block; "The following are blocks: a module, a function body, and a class definition."[^4] |
| block statements | no scope[^1] | no block statement |
| read before the value | `undefined`[^1] | UnboundLocalError[^4] |
| declare twice | allowed[^1] | no declaration |

In Python, "If a name is bound in a block, it is a local variable of that block, unless declared as nonlocal or global."[^4] "The local variables of a code block can be determined by scanning the entire text of the block for name binding operations."[^4] That is close to `var` in a function, which is local in the whole body (rule r3). But a Python read before the binding raises UnboundLocalError, and a `var` read gives `undefined`. See [[python/naming-and-binding]].

## References
