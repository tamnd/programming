---
tag: 69GS3R
kind: feature
owner: javascript
slug: nan-and-infinity
name: NaN and Infinity
aliases: [NaN, not a number, Infinity, "-Infinity", "Number.isNaN", "isNaN", overflow, division by zero]
area: languages/javascript/values-and-types
level: core
summary: >-
  NaN is the Number value for a failed or undefined result, and it is not equal to itself. Infinity comes from overflow and from division by zero. Number.isNaN tests for NaN with no conversion.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-language-types-number-type", locator: "6.1.6.1 The Number Type"}
see_also: [javascript/numbers, javascript/floating-point-numbers, javascript/booleans-and-truthiness, javascript/arithmetic-operators, javascript/relational-operators, python/float-type]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/NaN
    title: NaN, MDN
    locator: Description; Testing against NaN
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Infinity
    title: Infinity, MDN
    locator: Description; Using Infinity
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-language-types-number-type
    title: ECMAScript 2026 Language Specification
    locator: 6.1.6.1 The Number Type
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/isNaN
    title: Number.isNaN(), MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/isNaN
    title: isNaN(), MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Equality_comparisons_and_sameness
    title: Equality comparisons and sameness, MDN
    locator: Strict equality using ===; Same-value-zero equality
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Data_structures
    title: JavaScript data types and data structures, MDN
    locator: Number type
    accessed: 2026-11-18
  - url: https://docs.python.org/3/library/functions.html
    title: Built-in Functions, The Python Standard Library
    locator: float()
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.7 Binary arithmetic operations
    accessed: 2026-11-18
---

## Synopsis

| Expression | Result |
|---|---|
| `Number('abc')`, `parseInt('blabla')` | `NaN` |
| `Math.sqrt(-1)` | `NaN` |
| `0 * Infinity`, `Infinity - Infinity` | `NaN` |
| `NaN === NaN` | `false` |
| `1 / 0`, `10 ** 1000` | `Infinity` |
| `-1 / 0`, `Math.log(0)` | `-Infinity` |
| `Number.isNaN('abc')` | `false` |
| `isNaN('abc')` | `true` |

The cases are from the MDN NaN and Infinity pages.[^1] [^2]

```javascript run id=synopsis
console.log(parseInt("blabla"), Math.sqrt(-1), 0 * Infinity, 7 * "blabla");
console.log(NaN === NaN, Number.isNaN(NaN), typeof NaN);
console.log(1 / 0, -1 / 0, 10 ** 1000, Math.log(0), 1 / Infinity);
```

```output for=synopsis
NaN NaN NaN NaN
false true number
Infinity -Infinity Infinity -Infinity 0
```

## Semantics

**r1. NaN is a Number.** ECMA-262 maps all the IEEE NaN values to "a single special NaN value". It adds that "to ECMAScript code, all NaN values are indistinguishable from each other."[^3] `typeof NaN` is `'number'`. Example: [`synopsis`](#synopsis).

**r2. Where NaN comes from.** MDN lists "five different types of operations that return NaN": a failed number conversion, a math result that "is not a real number", an indeterminate form such as `0 * Infinity`, an operand that "is or gets coerced to NaN", and other invalid values shown as numbers.[^1] NaN spreads: "this means NaN is contagious".[^1] Example: [`synopsis`](#synopsis).

**r3. NaN compares false.** "When NaN is one of the operands of any relational comparison ( > , < , >= , <= ), the result is always false . NaN compares unequal (via == , != , === , and !== ) to any other value", and that includes another NaN.[^1] "NaN is also one of the falsy values in JavaScript."[^1] Example: [`compare`](#compare).

**r4. `Number.isNaN` against `isNaN`.** "As opposed to the global isNaN() function, the Number.isNaN() method doesn't force-convert the parameter to a number."[^4] The global `isNaN()` converts first, so `isNaN('hello')` is true. With `null`, `''` and booleans, which convert to numbers, it is false. So it does not tell whether a value is the NaN value, and it does not tell whether a value is a number.[^5] Example: [`isnan`](#isnan).

**r5. Self-comparison.** "Since x !== x is only true for NaN among all possible JavaScript values, Number.isNaN(x) can also be replaced with a test for x !== x , despite the latter being less readable."[^4] Example: [`isnan`](#isnan).

**r6. Searching for NaN.** `indexOf()` uses strict equality, so "you cannot use indexOf(NaN) to find the index of a NaN value in an array".[^6] Same-value-zero treats NaN as equal to NaN, and "It's used by Array.prototype.includes() , TypedArray.prototype.includes() , as well as Map and Set methods".[^6] Example: [`compare`](#compare).

**r7. Infinity.** "The value Infinity (positive infinity) is greater than any other number."[^2] "Positive values greater than Number.MAX_VALUE are converted to Infinity ."[^7] A nonzero number divided by `0` gives `Infinity` or `-Infinity`, and a number divided by `Infinity` gives 0.[^2] Example: [`synopsis`](#synopsis).

## Examples

The examples run with Node.js 24.

NaN in comparisons and searches (rules r3 and r6):

```javascript run id=compare
console.log(NaN < 1, NaN >= 1, NaN == NaN, NaN != NaN, Boolean(NaN));
const list = [1, NaN, 3];
console.log(list.indexOf(NaN), list.includes(NaN), new Set([NaN, NaN]).size);
```

```output for=compare
false false false true false
-1 true 1
```

The two NaN tests (rules r4 and r5):

```javascript run id=isnan
const inputs = [NaN, "hello", "", null, true, "12", undefined];
console.log(inputs.map((v) => isNaN(v)).join(" "));
console.log(inputs.map((v) => Number.isNaN(v)).join(" "));
const x = 0 / 0;
console.log(x !== x);
```

```output for=isnan
true true false false false false true
true false false false false false false
true
```

## Pitfalls

### `x === NaN` is always false

Use `Number.isNaN(x)`. A test `x === NaN` never passes, even when `x` is NaN (rule r3).[^4]

### The global `isNaN` converts

`isNaN('hello')` is true and `isNaN('')` is false ([`isnan`](#isnan)). To test that a value is a number, MDN says to use `typeof x === 'number'`.[^5]

### One NaN spoils a sum

A single NaN in a list makes the total NaN (rule r2). Check the inputs, or the total, with `Number.isNaN`.

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| make NaN or infinity | `NaN`, `Infinity` globals[^1] [^2] | `float('nan')`, `float('inf')`, from the `nan` and `infinity` input forms[^8] |
| `1 / 0` | `Infinity`[^2] | "Division by zero raises the ZeroDivisionError exception."[^9] |

JavaScript gives Infinity where Python raises an error. See [[python/float-type]].

## References
