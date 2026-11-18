---
tag: 81R1VV
kind: feature
owner: javascript
slug: the-typeof-operator
name: The typeof operator
aliases: [typeof, typeof null, typeof operator]
area: languages/javascript/values-and-types
level: intro
summary: >-
  typeof x gives a string that names the type of x: "undefined", "boolean", "number", "bigint", "string", "symbol", "function" or "object". typeof null is "object".
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-typeof-operator", locator: "13.5.3 The typeof Operator"}
see_also: [javascript/javascript-data-types-and-data-structures, javascript/undefined-and-null, javascript/primitive-wrapper-objects, javascript/symbols, javascript/unary-operators, javascript/operator-precedence, python/objects-values-and-types, python/instance-and-subclass-checks]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/typeof
    title: typeof, MDN
    locator: Description; typeof null; Need for parentheses in syntax; Interaction with undeclared and uninitialized variables
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-typeof-operator
    title: ECMAScript 2026 Language Specification
    locator: 13.5.3.1 Runtime Semantics, Evaluation
    accessed: 2026-11-18
  - url: https://docs.python.org/3/library/functions.html
    title: Built-in Functions, The Python Standard Library
    locator: type
    accessed: 2026-11-18
---

## Synopsis

```text
typeof operand
```

| Operand | Result |
|---|---|
| `undefined`, an undeclared name | `'undefined'` |
| `null` | `'object'` |
| a Boolean, number, BigInt, string or symbol | `'boolean'`, `'number'`, `'bigint'`, `'string'`, `'symbol'` |
| a function or class | `'function'` |
| any other object | `'object'` |

"The typeof operator returns a string indicating the type of the operand's value."[^1] MDN says of its table of results: "This list of values is exhaustive."[^1]

```javascript run id=synopsis
console.log(typeof undefined, typeof null, typeof true, typeof 1, typeof 1n);
console.log(typeof "s", typeof Symbol(), typeof function () {}, typeof class {}, typeof []);
```

```output for=synopsis
undefined object boolean number bigint
string symbol function function object
```

## Semantics

**r1. The result is a string.** The specification returns one of the strings in the table, from the value of the operand.[^2] So `typeof typeof 1` is `'string'`.[^1]

**r2. typeof null is 'object'.** In the specification, the step for a null value returns `'object'`.[^2] MDN explains that "In the first implementation of JavaScript, JavaScript values were represented as a type tag and a value." The tag for objects was 0, and null was the NULL pointer, so it had the same tag.[^1] Example: [`synopsis`](#synopsis).

**r3. Callable objects give 'function'.** An object with a `[[Call]]` internal method gives `'function'`.[^2] Classes are functions too.[^1] All other objects, which include arrays, dates and regular expressions, give `'object'`.[^1]

**r4. An undeclared name is OK.** MDN says that typeof "works with undeclared identifiers", and returns `'undefined'` without an error.[^1] In the specification, an unresolvable reference gives `'undefined'`.[^2] Example: [`undeclared`](#undeclared).

**r5. The temporal dead zone still throws.** "using typeof on lexical declarations (let const, using await using, and class) in the same block before the place of declaration will throw a ReferenceError."[^1] Example: [`tdz`](#tdz).

**r6. Precedence.** "The typeof operator has higher precedence than binary operators like addition (+)."[^1] Example: [`precedence`](#precedence).

## Examples

The examples run with Node.js 24.

A test for a global name that may not exist (rule r4):

```javascript run id=undeclared
console.log(typeof notDeclaredAnywhere);
if (typeof notDeclaredAnywhere === "undefined") console.log("not there");
```

```output for=undeclared
undefined
not there
```

The temporal dead zone (rule r5):

```javascript fails id=tdz expect=exception:ReferenceError
{
  console.log(typeof later);
  let later = 1;
}
```

Use parentheses around an expression (rule r6):

```javascript run id=precedence
const n = 99;
console.log(typeof n + " foo");
console.log(typeof (n + " foo"));
```

```output for=precedence
number foo
string
```

A check that tells null, arrays and objects apart:

```javascript run id=kind
function kind(v) {
  if (v === null) return "null";
  if (Array.isArray(v)) return "array";
  return typeof v;
}
console.log([null, [], {}, new String("s"), "s"].map(kind).join(" "));
```

```output for=kind
null array object object string
```

## Pitfalls

### null is not an object

`typeof v === 'object'` is true for `null` (rule r2). Test `v !== null` too.

### Wrapper objects

`typeof new String('abc')` is `'object'`. MDN calls the wrapper constructors with `new` "confusing, dangerous, and wasteful".[^1] See [[javascript/primitive-wrapper-objects]].

### Arrays

`typeof [1, 2, 4]` is `'object'`. MDN says to "use Array.isArray or Object.prototype.toString.call" to tell arrays from other objects.[^1] Example: [`kind`](#kind).

## Comparison

| Topic | JavaScript `typeof` | Python `type()` |
|---|---|---|
| result | a string from a fixed list[^1] | "a type object"[^3] |
| subclasses | not shown; most objects give `'object'`[^1] | `isinstance()` "takes subclasses into account"[^3] |

Python `type(x)` with one argument returns "the type of an object".[^3] JavaScript `typeof` gives only a small set of names, so code uses `Array.isArray` or `instanceof` to learn more.[^1] See [[python/objects-values-and-types]] and [[python/instance-and-subclass-checks]].

## References
