---
tag: NEW-javascript-lexical-scoping
kind: feature
owner: javascript
slug: lexical-scoping
name: Lexical scoping
aliases: [lexical scope, static scope, scope chain, nested scopes, outer environment, OuterEnv]
area: languages/javascript/variables-and-scope
level: core
summary: >-
  JavaScript resolves a name by where the code is written, not by who calls it. An inner function sees the names of the functions and blocks around it, through a chain of outer environments.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/executable-code-and-execution-contexts.html#sec-environment-records", locator: "9.1 Environment Records"}
see_also: [javascript/function-scopes-and-closures, javascript/block-scope, javascript/environment-records, javascript/the-global-scope-and-globalthis, javascript/hoisting, lexical-scope, closures, python/name-resolution]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Closures
    title: Closures, MDN
    locator: Lexical scoping
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Scope
    title: Scope, MDN Glossary
    locator: definition
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/executable-code-and-execution-contexts.html#sec-environment-records
    title: ECMAScript 2026 Language Specification
    locator: 9.1 Environment Records
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/executionmodel.html
    title: Execution model, The Python Language Reference
    locator: 4.2.2 Resolution of names
    accessed: 2026-11-18
---

## Synopsis

```text
outer scope
  function scope
    block scope
      a name is looked up here, then in each outer scope
```

Lexical scoping "describes how a parser resolves variable names when functions are nested. The word lexical refers to the fact that lexical scoping uses the location where a variable is declared within the source code to determine where that variable is available."[^1]

```javascript run id=synopsis
function init() {
  const name = "Mozilla";
  function displayName() {
    console.log(name);
  }
  displayName();
}
init();
```

```output for=synopsis
Mozilla
```

## Semantics

**r1. Nested functions.** "Nested functions have access to variables declared in their outer scope."[^1] Example: [`synopsis`](#synopsis).

**r2. One way only.** "Scopes can also be layered in a hierarchy, so that child scopes have access to parent scopes, but not vice versa."[^2] Example: [`inside`](#inside).

**r3. The kinds of scope.** MDN lists the global scope, the module scope, the function scope and the block scope. The block scope holds names "declared with certain syntaxes, including let, const, class, or (in strict mode) function".[^2]

**r4. The chain in the specification.** An Environment Record links names to variables "based upon the lexical nesting structure of ECMAScript code." "Every Environment Record has an `[[OuterEnv]]` field, which is either null or a reference to an outer Environment Record."[^3] A lookup that fails in one record goes on to the outer one.

**r5. The outer record comes from the source.** "For example, if a FunctionDeclaration contains two nested FunctionDeclarations then the Environment Records of each of the nested functions will have as their outer Environment Record the Environment Record of the current evaluation of the surrounding function."[^3] The caller of a function does not change its outer record. Example: [`caller`](#caller).

## Examples

The examples run with Node.js 24.

The caller's local name does not reach the called function (rule r5):

```javascript run id=caller
const x = "global";
function show() {
  return x;
}
function call() {
  const x = "local in call";
  return show();
}
console.log(call());
```

```output for=caller
global
```

A name from an inner scope is not visible outside (rule r2):

```javascript fails id=inside expect=exception:ReferenceError
function exampleFunction() {
  const x = "declared inside function";
}
exampleFunction();
console.log(x);
```

The nearest declaration wins:

```javascript run id=shadow
const level = "outer";
function f() {
  const level = "f";
  {
    const level = "block";
    console.log(level);
  }
  console.log(level);
}
f();
console.log(level);
```

```output for=shadow
block
f
outer
```

## Pitfalls

### Expecting the caller's scope

In example [`caller`](#caller), `show` reads the global `x`, not the `x` of its caller. The position of `show` in the source fixes its outer scope (rule r5).

### var ignores blocks

For `var`, "blocks with curly braces do not create scopes".[^1] Only `let`, `const` and `class` get block scope (rule r3). See [[javascript/block-scope]].

### Shadowing hides a name

An inner declaration with the same name hides the outer one in its whole scope (example [`shadow`](#shadow)). The outer one is not reachable by that name there.

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| lookup | the nearest outer scope in the source[^3] | "it is resolved using the nearest enclosing scope"[^4] |
| block scope | `let`, `const`, `class`[^2] | none; the blocks are modules, functions and classes[^4] |
| name not found | ReferenceError, example [`inside`](#inside) | NameError[^4] |

Python also uses lexical scoping. "When a name is used in a code block, it is resolved using the nearest enclosing scope."[^4] See [[python/name-resolution]] and [[lexical-scope]].

## References
