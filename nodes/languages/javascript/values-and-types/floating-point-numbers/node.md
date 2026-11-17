---
tag: P0XYWR
kind: feature
owner: javascript
slug: floating-point-numbers
name: Floating-point numbers
aliases: [IEEE 754, binary64, double precision, "0.1 + 0.2", Number.EPSILON, safe integers, MAX_SAFE_INTEGER, negative zero, "-0"]
area: languages/javascript/values-and-types
level: core
summary: >-
  A Number is an IEEE 754 binary64 value. Decimal fractions such as 0.1 are rounded, integers are exact only up to 2^53 - 1, and there is a negative zero that === cannot see.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-language-types-number-type", locator: "6.1.6.1 The Number Type"}
see_also: [javascript/numbers, javascript/nan-and-infinity, javascript/bigints, javascript/arithmetic-operators, floating-point-arithmetic, floating-point-rounding-error, python/floating-point-arithmetic]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-language-types-number-type
    title: ECMAScript 2026 Language Specification
    locator: 6.1.6.1 The Number Type
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Data_structures
    title: JavaScript data types and data structures, MDN
    locator: Number type
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/EPSILON
    title: Number.EPSILON, MDN
    locator: Value; Description; Testing equality
    accessed: 2026-11-18
  - url: https://docs.python.org/3/tutorial/floatingpoint.html
    title: Floating-Point Arithmetic, Issues and Limitations, The Python Tutorial
    locator: introduction
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/MAX_SAFE_INTEGER
    title: Number.MAX_SAFE_INTEGER, MDN
    locator: Value; Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/isSafeInteger
    title: Number.isSafeInteger(), MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/is
    title: Object.is(), MDN
    locator: Description
    accessed: 2026-11-18
---

## Synopsis

| Quantity | Value |
|---|---|
| format | IEEE 754-2019 binary64 |
| largest finite, `Number.MAX_VALUE` | about 1.8e308 |
| smallest positive, `Number.MIN_VALUE` | 2^-1074, about 5e-324 |
| largest safe integer, `Number.MAX_SAFE_INTEGER` | 2^53 - 1 = 9007199254740991 |
| gap after 1, `Number.EPSILON` | 2^-52, about 2.22e-16 |
| zeros | `+0` and `-0` |

The Number type represents "the double-precision floating point IEEE 754-2019 binary64 values".[^1] The limits are from MDN.[^2] [^3]

```javascript run id=synopsis
console.log(0.1 + 0.2, 0.1 + 0.2 === 0.3);
console.log(2 ** 53, 2 ** 53 + 1, Number.MAX_SAFE_INTEGER === 2 ** 53 - 1);
console.log(42 / -0, -0 === 0, Object.is(-0, 0));
```

```output for=synopsis
0.30000000000000004 false
9007199254740992 9007199254740992 true
-Infinity true false
```

## Semantics

**r1. The values.** "The Number type has exactly 18,437,736,874,454,810,627 (that is, 2 64 - 2 53 + 3 ) values".[^1] All the IEEE NaN values "are represented in ECMAScript as a single special NaN value". There is a positive and a negative Infinity.[^1]

**r2. Decimal fractions are rounded.** MDN says that "a base-2 (binary) system cannot represent 0.1 exactly. Thus, for example, 0.1 + 0.2 is not exactly equal to 0.3".[^3] Python's tutorial says the same of its floats: they "are represented in computer hardware as base 2 (binary) fractions."[^4] Example: [`synopsis`](#synopsis).

**r3. Comparing with a tolerance.** MDN notes that "it is often advised that floating point numbers should never be compared with === ." `Number.EPSILON` "is usually a reasonable threshold for errors if the arithmetic is around the magnitude of 1", but it "is inappropriate for any arithmetic operating on a larger magnitude."[^3] Example: [`epsilon`](#epsilon).

**r4. Precision falls with size.** MDN notes that "the absolute accuracy of floating numbers decreases as the number gets larger, because the exponent grows while the mantissa's accuracy stays the same."[^3]

**r5. Safe integers.** The format "can only safely represent integers between -(2 53 – 1) and 2 53 – 1". "Safe" means "the ability to represent integers exactly and to compare them correctly."[^5] 2^53 can be represented, but it is not safe, because "the integer 2 53 + 1 can't be directly represented in IEEE-754 but instead rounds to 2 53".[^6] Example: [`safe`](#safe).

**r6. Overflow and underflow.** "Positive values greater than Number.MAX_VALUE are converted to Infinity . Positive values smaller than Number.MIN_VALUE are converted to 0 ." Negative values in the same ranges go to `-Infinity` and `-0`.[^2] Example: [`limits`](#limits).

**r7. Negative zero.** "Note that there is both a positive zero and a negative zero ."[^1] "+0 === -0 is true", but "you are able to notice this when you divide by zero".[^2] `Object.is()` tells them apart. "The only difference between Object.is() and === is in their treatment of signed zeros and NaN values."[^7] Example: [`synopsis`](#synopsis).

## Examples

The examples run with Node.js 24.

`Number.EPSILON` works near 1 and fails near 1000 (rule r3). MDN gives these cases:[^3]

```javascript run id=epsilon
function equal(x, y) {
  return Math.abs(x - y) < Number.EPSILON;
}
console.log(equal(0.2 + 0.1, 0.3), 1000.1 + 1000.2, equal(1000.1 + 1000.2, 2000.3));
console.log(Math.abs(1000.1 + 1000.2 - 2000.3) < 1e-9);
```

```output for=epsilon
true 2000.3000000000002 false
true
```

Above `MAX_SAFE_INTEGER`, distinct integers compare equal (rule r5):

```javascript run id=safe
const max = Number.MAX_SAFE_INTEGER;
console.log(max + 1 === max + 2, Number.isSafeInteger(max), Number.isSafeInteger(max + 1));
console.log(9007199254740993, BigInt(max) + 2n);
```

```output for=safe
true true false
9007199254740992 9007199254740993n
```

The literal `9007199254740993` rounds to the nearest double, 9007199254740992. A BigInt keeps the exact integer.

Overflow, underflow and a negative zero from a product (rules r6 and r7):

```javascript run id=limits
console.log(Number.MAX_VALUE * 2, Number.MIN_VALUE / 2, -Number.MIN_VALUE / 2);
console.log(Object.is(-Number.MIN_VALUE / 2, -0), -1 * 0, Math.round(-0.4), String(-0));
```

```output for=limits
Infinity 0 -0
true -0 -0 0
```

## Pitfalls

### `0.1 + 0.2 !== 0.3`

Do not test fractions with `===` (rule r3). Compare with a tolerance that fits the size of the values.

### Integer IDs above 2^53

A 64-bit ID from a server can be larger than `MAX_SAFE_INTEGER`. As a Number, two IDs can become equal (rule r5). Keep such IDs as strings or BigInts.

### `-0` prints as `0` in a string

`String(-0)` is `'0'` ([`limits`](#limits)), and `-0 === 0` is true. Use `Object.is(x, -0)` to find a negative zero (rule r7).

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| float format | binary64[^1] | base 2 fractions in hardware[^4] |
| `0.1 + 0.2` | `0.30000000000000004` ([`synopsis`](#synopsis)) | not equal to 0.3[^4] |
| close-enough test | `Math.abs(a - b) < tol`[^3] | `math.isclose()`, which "can be useful for comparing inexact values"[^4] |
| integers | the same type, exact to 2^53 - 1[^5] | a separate `int` type |

See [[python/floating-point-arithmetic]] and [[floating-point-rounding-error]].

## References
