---
tag: NEW-javascript-hoisting
kind: feature
owner: javascript
slug: hoisting
name: Hoisting
aliases: [hoisting, hoisted, declaration hoisting, function hoisting, var hoisting, HoistableDeclaration]
area: languages/javascript/variables-and-scope
level: core
summary: >-
  Hoisting is how a declaration takes effect in its whole scope before its line runs. Functions are usable early, a var reads undefined, and let, const and class throw until declared.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#prod-HoistableDeclaration", locator: "14 ECMAScript Language: Statements and Declarations, HoistableDeclaration"}
see_also: [javascript/var-declarations, javascript/let-and-const, javascript/the-temporal-dead-zone, javascript/block-scope, javascript/declarations, declaration, uninitialized-variables, python/naming-and-binding]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Hoisting
    title: Hoisting, MDN Glossary
    locator: definition
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Grammar_and_types
    title: Grammar and types, MDN
    locator: Variable hoisting
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/function
    title: function, MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/class
    title: class, MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/import
    title: import, MDN
    locator: Hoisting
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/executionmodel.html
    title: Execution model, The Python Language Reference
    locator: 4.2.2 Resolution of names
    accessed: 2026-11-18
---

## Synopsis

| Declaration | Before its line |
|---|---|
| `function` | usable with its value |
| `var` | reads `undefined` |
| `let`, `const`, `class` | ReferenceError |
| `import` | binding usable; the module ran first |

"JavaScript Hoisting refers to the process whereby the interpreter appears to move the declaration of functions, variables, classes, or imports to the top of their scope, prior to execution of the code."[^1]

```javascript run id=synopsis
console.log(square(3));
console.log(total);
function square(n) {
  return n * n;
}
var total = 10;
```

```output for=synopsis
9
undefined
```

## Semantics

**r1. Not a spec term.** "Hoisting is not a term normatively defined in the ECMAScript specification." The specification has a HoistableDeclaration, "but this only includes function, function*, async function, and async function* declarations."[^1]

**r2. Four behaviors.** MDN lists four kinds of hoisting. The first is to use a value before its line ("Value hoisting"). The second is to read the name before its line with the value `undefined` ("Declaration hoisting"). In the third, the declaration changes the behavior of its scope before its line. In the fourth, the side effects of the declaration come before the rest of the code.[^1]

**r3. Which declaration has which.** "The four function declarations above are hoisted with type 1 behavior; var declaration is hoisted with type 2 behavior; let, const, and class declarations (also collectively called lexical declarations) are hoisted with type 3 behavior; import declarations are hoisted with type 1 and type 4 behavior."[^1]

**r4. var.** A read before the line gives `undefined`, "because only its declaration and default initialization (with undefined) is hoisted, but not its value assignment."[^2] See [[javascript/var-declarations]].

**r5. function.** Function declarations "are hoisted together with its value and can be called anywhere in its scope."[^3]

**r6. class.** "class declarations can only be accessed after the place of declaration is reached".[^4] Example: [`kinds`](#kinds).

**r7. import.** "Import declarations are hoisted. In this case, that means that the bindings the imports introduce are available in the entire module scope, and the imported modules' side effects are produced before the rest of the module's code runs."[^5]

## Examples

The examples run with Node.js 24.

A `var`, a `let` and a `class` read before their lines (rules r4 and r6):

```javascript run id=kinds
console.log(v);
try { l; } catch (e) { console.log("let:", e.name); }
try { new C(); } catch (e) { console.log("class:", e.name); }
var v = 1;
let l = 2;
class C {}
```

```output for=kinds
undefined
let: ReferenceError
class: ReferenceError
```

An inner `const` hides the outer one before its line (type 3 behavior):

```javascript fails id=taint expect=exception:ReferenceError
const x = 1;
{
  console.log(x);
  const x = 2;
}
```

## Pitfalls

### The inner name wins early

In example [`taint`](#taint), the read does not see the outer `x`. MDN says the inner `const` taints its whole block. The read finds the inner `x`, "which is not yet initialized, and throws a ReferenceError".[^1]

### A function and its call order

A function declaration can be called above its line (rule r5), but a function expression in a `var` cannot. The `var` holds `undefined` until the assignment runs (rule r4).

### Declare at the top

"Because of hoisting, all var statements in a function should be placed as near to the top of the function as possible."[^2]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| local in the whole block | yes, for each declaration kind[^1] | yes, for each bound name[^6] |
| read before the binding | `undefined` for `var`, ReferenceError for `let`[^2] | UnboundLocalError[^6] |
| call a function above its `def` | works for a declaration[^3] | the `def` must run first |

Python has a similar rule. "This can lead to errors when a name is used within a block before it is bound."[^6] See [[python/naming-and-binding]].

## References
