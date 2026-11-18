---
tag: NEW-javascript-function-scopes-and-closures
kind: feature
owner: javascript
slug: function-scopes-and-closures
name: Function scopes and closures
aliases: [function scope, closure, closures, scope chain, captured variable, free variable]
area: languages/javascript/variables-and-scope
level: core
summary: >-
  A function is a scope, and it can use the variables of each scope around it. A closure is a function together with the environment it was made in, which stays alive after the outer call returns.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/executable-code-and-execution-contexts.html#sec-environment-records", locator: "9.1 Environment Records"}
see_also: [javascript/lexical-scoping, javascript/block-scope, javascript/var-declarations, javascript/environment-records, closures, lexical-scope, python/nonlocal-statement, python/name-resolution]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Closures
    title: Closures, MDN
    locator: Closure; Emulating private methods with closures; Closure scope chain; Performance considerations
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Functions
    title: Functions, MDN
    locator: Function scopes and closures; Name conflicts
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/executable-code-and-execution-contexts.html#sec-environment-records
    title: ECMAScript 2026 Language Specification
    locator: 9.1 Environment Records
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/executionmodel.html
    title: Execution model, The Python Language Reference
    locator: 4.2.1 Binding of names; 4.2.2 Resolution of names
    accessed: 2026-11-18
---

## Synopsis

```javascript run id=synopsis
function makeFunc() {
  const name = "Mozilla";
  function displayName() {
    console.log(name);
  }
  return displayName;
}
const myFunc = makeFunc();
myFunc();
```

```output for=synopsis
Mozilla
```

"A closure is the combination of a function bundled together (enclosed) with references to its surrounding state (the lexical environment). In other words, a closure gives a function access to its outer scope. In JavaScript, closures are created every time a function is created, at function creation time."[^1]

## Semantics

**r1. A function is a scope.** "Functions form a scope for variables" and "variables defined inside a function cannot be accessed from anywhere outside the function."[^2]

**r2. Outer scopes.** "A function defined inside another function can also access all variables defined in its parent function, and any other variables to which the parent function has access."[^2] "In this context, we can say that closures have access to all outer scopes."[^1]

**r3. The scope chain.** "More nested scopes take precedence. So, the innermost scope takes the highest precedence, while the outermost scope takes the lowest. This is the scope chain."[^2] Example: [`conflict`](#conflict).

**r4. The environment lives on.** A closure is "the combination of a function and the lexical environment within which that function was declared. This environment consists of any variables that were in-scope at the time the closure was created."[^1] In the specification, "Each time such code is evaluated, a new Environment Record is created to record the identifier bindings that are created by that code."[^3]

**r5. One environment for each call.** "Each closure references a different version of the privateCounter variable through its own closure."[^1] Example: [`counter`](#counter).

**r6. Shared variables.** Closures made in the same call share that call's variables. Closures that "share the same lexical environment" see the changes that each one makes.[^1] Example: [`counter`](#counter).

## Examples

The examples run with Node.js 24.

Two counters with private state (rules r5 and r6):

```javascript run id=counter
function makeCounter() {
  let privateCounter = 0;
  function changeBy(val) {
    privateCounter += val;
  }
  return {
    increment() { changeBy(1); },
    value() { return privateCounter; },
  };
}
const counter1 = makeCounter();
const counter2 = makeCounter();
counter1.increment();
counter1.increment();
console.log(counter1.value(), counter2.value());
```

```output for=counter
2 0
```

A parameter hides an outer variable (rule r3):

```javascript run id=conflict
function outside() {
  const x = 5;
  function inside(x) {
    return x * 2;
  }
  return inside;
}
console.log(outside()(10));
```

```output for=conflict
20
```

A chain of four nested scopes and the global scope:

```javascript run id=chain
const e = 10;
function sum(a) {
  return (b) => (c) => (d) => a + b + c + d + e;
}
console.log(sum(1)(2)(3)(4));
```

```output for=chain
20
```

## Pitfalls

### Closures capture variables, not values

A closure reads the variable when it runs, so it sees later changes. A `var` loop counter is one variable for all the closures of the loop. See [[javascript/var-declarations]].

### Memory and speed

MDN says "it is unwise to unnecessarily create functions within other functions if closures are not needed for a particular task, as it will negatively affect script performance both in terms of processing speed and memory consumption."[^1]

### Private by scope only

The `privateCounter` in example [`counter`](#counter) has no name outside `makeCounter`. "You can't access either of these private members from outside the anonymous function."[^1]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| read an outer variable | yes[^2] | yes, a free variable[^4] |
| assign an outer variable | yes, if no inner declaration | needs `nonlocal`[^4] |
| block closures | `let` in a block[^1] | no block scope |

In Python, "If a variable is used in a code block but not defined there, it is a free variable." To assign it, "The nonlocal statement causes corresponding names to refer to previously bound variables in the nearest enclosing function scope."[^4] See [[python/nonlocal-statement]] and [[closures]].

## References
