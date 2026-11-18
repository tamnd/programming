---
tag: NEW-javascript-the-temporal-dead-zone
kind: feature
owner: javascript
slug: the-temporal-dead-zone
name: The temporal dead zone
aliases: [temporal dead zone, TDZ, Cannot access before initialization, uninitialized binding]
area: languages/javascript/variables-and-scope
level: core
summary: >-
  A let, const or class variable is in the temporal dead zone from the start of its block until its declaration runs. Any access there, typeof too, throws a ReferenceError. A var has no dead zone.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/executable-code-and-execution-contexts.html#sec-declarative-environment-records-getbindingvalue-n-s", locator: "9.1.1.1.6 GetBindingValue ( N, S )"}
see_also: [javascript/let-and-const, javascript/hoisting, javascript/var-declarations, javascript/the-typeof-operator, javascript/block-scope, javascript/environment-records, uninitialized-variables, python/naming-and-binding]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/let
    title: let, MDN
    locator: Temporal dead zone (TDZ); TDZ combined with lexical scoping
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/executable-code-and-execution-contexts.html#sec-declarative-environment-records-getbindingvalue-n-s
    title: ECMAScript 2026 Language Specification
    locator: 9.1.1.1.6 GetBindingValue ( N, S )
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-let-and-const-declarations
    title: ECMAScript 2026 Language Specification
    locator: 14.3.1 Let and Const Declarations, Note
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/executionmodel.html
    title: Execution model, The Python Language Reference
    locator: 4.2.2 Resolution of names
    accessed: 2026-11-18
---

## Synopsis

```text
{
  // dead zone for x starts here
  let x = value;   // the dead zone ends here
}
```

A variable declared with `let`, `const` or `class` is in a temporal dead zone (TDZ) "from the start of the block until code execution reaches the place where the variable is declared and initialized."[^1]

```javascript run id=synopsis
{
  console.log(typeof bar, bar);
  try { foo; } catch (e) { console.log(e.name + ": " + e.message); }
  var bar = 1;
  let foo = 2;
}
```

```output for=synopsis
undefined undefined
ReferenceError: Cannot access 'foo' before initialization
```

## Semantics

**r1. The error.** "While inside the TDZ, the variable has not been initialized with a value, and any attempt to access it will result in a ReferenceError."[^1] In the specification, GetBindingValue says: "If the binding for N in envRec is an uninitialized binding, throw a ReferenceError exception."[^2]

**r2. When the zone ends.** "The variable is initialized with a value when execution reaches the place in the code where it was declared. If no initial value was specified with the variable declaration, it will be initialized with a value of undefined."[^1] The specification says such a variable "may not be accessed in any way until the variable's LexicalBinding is evaluated."[^3]

**r3. var has no dead zone.** "This differs from var variables, which will return a value of undefined if they are accessed before they are declared."[^1] Example: [`synopsis`](#synopsis).

**r4. Time, not position.** The zone "depends on the order of execution (time) rather than the order in which the code is written (position)."[^1] Example: [`later`](#later).

**r5. typeof.** "Using the typeof operator for a variable in its TDZ will throw a ReferenceError". This "differs from using typeof for undeclared variables".[^1] Example: [`typeof`](#typeof).

## Examples

The examples run with Node.js 24.

A function that reads a `let` runs after the dead zone ends (rule r4):

```javascript run id=later
{
  const func = () => console.log(letVar);
  let letVar = 3;
  func();
}
```

```output for=later
3
```

typeof on an undeclared name and on a name in its dead zone (rule r5):

```javascript fails id=typeof expect=exception:ReferenceError
console.log(typeof undeclaredVariable);
{
  typeof i;
  let i = 10;
}
```

```output for=typeof
undefined
```

## Pitfalls

### An initializer that reads its own name

In `let foo = foo + 55`, the right side reads the inner `foo`. "The expression foo + 55 throws a ReferenceError because initialization of let foo has not completed".[^1] Example: [`self`](#self).

```javascript fails id=self expect=exception:ReferenceError
function test() {
  var foo = 33;
  if (foo) {
    let foo = foo + 55;
  }
}
test();
```

### typeof is not a safe test

A `typeof x === ''undefined''` test is safe for an undeclared name, but it throws in a dead zone (rule r5).

### One block for each switch

"You may encounter errors in switch statements because there is only one block." "To avoid the error, wrap each case in a new block statement."[^1]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| read before the binding | ReferenceError for `let`, `const`, `class`[^1] | UnboundLocalError in a function[^4] |
| a `var`-like read | `undefined`[^1] | no such case |
| error kind | ReferenceError[^2] | "UnboundLocalError is a subclass of NameError."[^4] |

Python gives the same kind of error for a local name with no value yet: "an UnboundLocalError exception is raised."[^4] See [[python/naming-and-binding]] and [[uninitialized-variables]].

## References
