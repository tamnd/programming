---
tag: NEW-javascript-declarations
kind: feature
owner: javascript
slug: declarations
name: Declarations
aliases: [variable declarations, var let const, declaring variables, global variables, constants, const declaration]
area: languages/javascript/variables-and-scope
level: intro
summary: >-
  JavaScript declares variables with var, let and const. A script-level var becomes a property of the global object. A const binding cannot be assigned again, but its object can change.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-declarations-and-the-variable-statement", locator: "14.3 Declarations and the Variable Statement"}
see_also: [javascript/variables, javascript/var-declarations, javascript/let-and-const, javascript/the-global-scope-and-globalthis, javascript/destructuring, declaration, declare-a-constant, python/global-statement]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Grammar_and_types
    title: Grammar and types, MDN
    locator: Declarations; Declaring variables; Declaration and initialization; Global variables; Constants
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/var
    title: var, MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/executable-code-and-execution-contexts.html#sec-global-environment-records
    title: ECMAScript 2026 Language Specification
    locator: 9.1.1.4 Global Environment Records
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/executionmodel.html
    title: Execution model, The Python Language Reference
    locator: 4.2.1 Binding of names
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/simple_stmts.html
    title: Simple statements, The Python Language Reference
    locator: 7.12 The global statement
    accessed: 2026-11-18
---

## Synopsis

```text
var name [= value] [, name2 [= value2]] ...;
let name [= value] [, name2 [= value2]] ...;
const name = value [, name2 = value2] ...;
const { key } = object;
```

"JavaScript has five kinds of variable declarations."[^1] This page covers `var`, `let` and `const`. The other two, `using` and `await using`, declare a variable "like const that is synchronously disposed" or asynchronously disposed.[^1]

```javascript run id=synopsis
var a = 1, b = a + 1;
let c;
const d = 4;
const { key } = { key: "k" };
console.log(a, b, c, d, key);
```

```output for=synopsis
1 2 undefined 4 k
```

## Semantics

**r1. The three keywords.** `var` "Declares a variable, optionally initializing it to a value." `let` "Declares a block-scoped variable, optionally initializing it to a value." `const` "Declares a block-scoped variable that cannot be re-assigned, which must be initialized at declaration."[^1]

**r2. No initializer.** "If a variable is declared without an initializer, it is assigned the value undefined." "In essence, let x = 42 is equivalent to let x; x = 42."[^1]

**r3. Binding lists.** "The list that follows the var keyword is called a binding list and is separated by commas". "Initializers of later variables can refer to earlier variables in the list and get the initialized value."[^2] Example: [`synopsis`](#synopsis).

**r4. Globals in a script.** "Global variables are in fact properties of the global object."[^1] "In a script, a variable declared using var is added as a non-configurable property of the global object."[^2] In the specification, the object part of the global record holds the bindings of a FunctionDeclaration or VariableStatement in global code. "The bindings for all other ECMAScript declarations in global code are contained in the Declarative Environment Record component of the Global Environment Record."[^3] So a script-level `let` or `const` is global, but it is not a property. Example: [`global`](#global).

**r5. Globals in a module.** "In both Node.js CommonJS modules and native ECMAScript modules, top-level variable declarations are scoped to the module, and are not added as properties to the global object."[^2] Example: [`global`](#global).

**r6. Constants.** "A constant cannot change value through assignment or be re-declared while the script is running. It must be initialized to a value." "You cannot declare a constant with the same name as a function or variable in the same scope."[^1] Example: [`same-name`](#same-name).

## Examples

The examples run with Node.js 24.

A script against a module (rules r4 and r5). The `vm` module runs the string as a script. The file itself is a CommonJS module:

```javascript run id=global
const vm = require("node:vm");
vm.runInThisContext("var s1 = 1; let s2 = 2;");
console.log(Object.hasOwn(globalThis, "s1"), Object.hasOwn(globalThis, "s2"));
console.log(vm.runInThisContext("typeof s2"));
var m1 = 3;
console.log(Object.hasOwn(globalThis, "m1"));
```

```output for=global
true false
number
false
```

A constant with the name of a function (rule r6):

```javascript fails id=same-name expect=compile:SyntaxError
function f() {}
const f = 5;
```

The object of a constant can change:

```javascript run id=mutate
const list = ["HTML", "CSS"];
list.push("JS");
console.log(list);
```

```output for=mutate
[ 'HTML', 'CSS', 'JS' ]
```

## Pitfalls

### const does not freeze

"However, const only prevents re-assignments, but doesn't prevent mutations."[^1] See [[javascript/let-and-const]].

### Script globals and module globals differ

The same top-level `var` makes a global property in a script and a module-local name in a module (rules r4 and r5). Code that reads `globalThis.name` works in one and not in the other.

### No declaration

"Variables should always be declared before they are used."[^1] An assignment to a name with no declaration makes a global outside strict mode. See [[javascript/strict-mode]].

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| declaration keyword | `var`, `let`, `const`[^1] | none; "Python lacks declarations"[^4] |
| constant | `const`[^1] | no keyword |
| write a global from a function | assign to the outer name[^2] | `global` statement[^5] |

In Python, "The global statement causes the listed identifiers to be interpreted as globals."[^5] JavaScript has no such statement. A function that assigns to an outer name with no declaration of its own changes the outer variable. In the MDN example, the line `x = 3` "Assigns 3 to existing file scoped x."[^2] See [[python/global-statement]] and [[declaration]].

## References
