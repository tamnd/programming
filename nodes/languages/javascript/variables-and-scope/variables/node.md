---
tag: NEW-javascript-variables
kind: feature
owner: javascript
slug: variables
name: Variables
aliases: [variable, variable name, identifier, declaring a variable, assigning a variable, var, let, const]
area: languages/javascript/variables-and-scope
level: intro
summary: >-
  A variable is a named reference to a value. Declare it with let, const or var. A let or var variable with no initializer holds undefined, and a const needs one.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-declarations-and-the-variable-statement", locator: "14.3 Declarations and the Variable Statement"}
see_also: [javascript/declarations, javascript/var-declarations, javascript/let-and-const, javascript/hoisting, javascript/identifiers-and-reserved-words, javascript/undefined-and-null, variable-concept, python/naming-and-binding]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Variable
    title: Variable, MDN Glossary
    locator: definition
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Grammar_and_types
    title: Grammar and types, MDN
    locator: Variables; Declaring variables; Declaration and initialization; Variable scope
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-declarations-and-the-variable-statement
    title: ECMAScript 2026 Language Specification
    locator: 14.3.1 Let and Const Declarations; 14.3.2 Variable Statement
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/executionmodel.html
    title: Execution model, The Python Language Reference
    locator: 4.2.1 Binding of names; 4.2.2 Resolution of names
    accessed: 2026-11-18
---

## Synopsis

```text
let name = value;
const name = value;
var name = value;
name = value;
```

| Keyword | Scope | Assign again | Without an initializer |
|---|---|---|---|
| `let` | block | yes | `undefined` |
| `const` | block | no | SyntaxError |
| `var` | function or global | yes | `undefined` |

"A variable is a named reference to a value."[^1] "You use variables as symbolic names for values in your application. The names of variables, called identifiers, conform to certain rules."[^2]

```javascript run id=synopsis
let count = 1;
count = count + 1;
const label = "items";
var total;
console.log(count, label, total);
```

```output for=synopsis
2 items undefined
```

## Semantics

**r1. Names.** "A JavaScript identifier usually starts with a letter, underscore (_), or dollar sign (`$`). Subsequent characters can also be digits (0 – 9)." Names are case sensitive.[^2] See [[javascript/identifiers-and-reserved-words]].

**r2. Declaration and initializer.** "In a statement like let x = 42, the let x part is called a declaration, and the = 42 part is called an initializer."[^2] "In var and let declarations, the initializer is optional. If a variable is declared without an initializer, it is assigned the value undefined."[^2]

**r3. const needs a value.** "const declarations always need an initializer, because they forbid any kind of assignment after declaration".[^2] Example: [`const-init`](#const-init).

**r4. Scopes.** "let and const declarations can also be scoped to the block statement that they are declared in." A `var` variable is "only local to the function (or global scope) that the block resides within."[^2] Example: [`scope`](#scope).

**r5. When the value is set.** In the specification, a `var` variable is "initialized to undefined when created". A `let` or `const` variable "may not be accessed in any way until the variable's LexicalBinding is evaluated."[^3] See [[javascript/the-temporal-dead-zone]].

**r6. Declare before use.** "Variables should always be declared before they are used." An assignment to an undeclared name "creates an undeclared global variable", and "This is an error in strict mode".[^2] Example: [`undeclared`](#undeclared).

## Examples

The examples run with Node.js 24.

Block scope and function scope (rule r4):

```javascript run id=scope
if (true) {
  var a = "var";
  let b = "let";
}
console.log(a, typeof b);
```

```output for=scope
var undefined
```

A const with no initializer (rule r3):

```javascript fails id=const-init expect=compile:SyntaxError
const answer;
```

An assignment to an undeclared name in strict mode (rule r6):

```javascript fails id=undeclared expect=exception:ReferenceError
"use strict";
function setUp() {
  totl = 10;
}
setUp();
```

## Pitfalls

### A typo makes a global

Without strict mode, `totl = 10` makes a global variable instead of an error (rule r6). Use strict mode or modules. See [[javascript/strict-mode]].

### undefined and an undeclared name

A declared variable with no value reads as `undefined` (rule r2). "The declaration allows the variable to be accessed later in code without throwing a ReferenceError".[^2] A name with no declaration throws a ReferenceError when read.

### const is not a frozen value

A `const` variable cannot be assigned again, but the object it refers to can change. See [[javascript/let-and-const]].

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| declaration | `let`, `const` or `var`[^2] | "Python lacks declarations"[^4] |
| what makes a name | the declaration[^3] | "Names are introduced by name binding operations."[^4] |
| unknown name | ReferenceError[^2] | "When a name is not found at all, a NameError exception is raised."[^4] |
| constants | `const`[^2] | no keyword |

In Python, "Names refer to objects."[^4] Assignment both makes and sets a name, while JavaScript separates the declaration from the assignment (rule r2). See [[python/naming-and-binding]] and [[variable-concept]].

## References
