---
tag: 8BZKF8
kind: feature
owner: javascript
slug: numbers
name: Numbers
aliases: [Number, number type, number literals, Math, "parseInt", "parseFloat", "toFixed", numeric separators]
area: languages/javascript/values-and-types
level: intro
summary: >-
  JavaScript numbers are IEEE 754 doubles. Literals can be decimal, binary, octal or hex. Number has constants and tests, Math has functions, and Number() and parseInt read strings.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-language-types-number-type", locator: "6.1.6.1 The Number Type"}
see_also: [javascript/floating-point-numbers, javascript/nan-and-infinity, javascript/bigints, javascript/arithmetic-operators, javascript/javascript-data-types-and-data-structures, numeric-type, python/numeric-literals, python/float-type]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Numbers_and_strings
    title: Numbers and strings, MDN Guide
    locator: Numbers; Decimal numbers; Binary numbers; Octal numbers; Hexadecimal numbers; Numeric separators; Number object; Math object
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number
    title: Number, MDN
    locator: Number coercion
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/parseInt
    title: parseInt(), MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://docs.python.org/3/library/stdtypes.html
    title: Built-in Types, The Python Standard Library
    locator: Numeric Types (int, float, complex)
    accessed: 2026-11-18
---

## Synopsis

| Form | Example | Value |
|---|---|---|
| decimal | `42`, `3.5` | 42, 3.5 |
| exponent | `175e-2` | 1.75 |
| binary | `0b1010` | 10 |
| octal | `0o644` | 420 |
| hexadecimal | `0xFF` | 255 |
| with separators | `1_000_000` | 1000000 |

"You can use four types of number literals: decimal, binary, octal, and hexadecimal."[^1] An underscore "can be inserted between digits to improve readability."[^1]

```javascript run id=synopsis
console.log(42, 3.5, 175e-2, 0b1010, 0o644, 0xff, 1_000_000);
console.log(typeof 42, typeof 3.5, Number.isInteger(42.0));
```

```output for=synopsis
42 3.5 1.75 10 420 255 1000000
number number true
```

## Semantics

**r1. One number type.** "In JavaScript, numbers are implemented in double-precision 64-bit binary format IEEE 754". "Integer values up to ±2^53 − 1 can be represented exactly."[^1] Integers and fractions share this type, so `42` and `42.0` are the same value. Example: [`synopsis`](#synopsis). See [[javascript/floating-point-numbers]].

**r2. Three special values.** Also, "the number type has three symbolic values: Infinity , -Infinity , and NaN (not-a-number)."[^1] See [[javascript/nan-and-infinity]].

**r3. Legacy octal.** A literal such as `0644` is a legacy octal. MDN says that "number literals prefixed with 0 , whether interpreted as octal or decimal, cause a syntax error in strict mode".[^1] Use the `0o` prefix. Example: [`legacy-octal`](#legacy-octal).

**r4. The Number object.** "The built-in Number object has properties for numerical constants, such as maximum value, not-a-number, and infinity." It also has the tests `Number.isFinite()`, `Number.isInteger()`, `Number.isNaN()` and `Number.isSafeInteger()`.[^1] `toFixed()`, `toPrecision()` and `toExponential()` return strings.[^1] Example: [`number-object`](#number-object).

**r5. The Math object.** The MDN guide says that "standard mathematical functions are methods of Math ", and that "all trigonometric methods of Math take arguments in radians."[^1] Example: [`math`](#math).

**r6. `Number()` reads the whole string.** Strings "are converted by parsing them as if they contain a number literal . Parsing failure results in NaN ." "Empty or whitespace-only strings are converted to 0 ."[^2] Example: [`parse`](#parse).

**r7. `parseInt` reads a prefix.** "The parseInt function converts its first argument to a string , parses that string, then returns an integer or NaN ." It stops at the first character that is not a digit. Without a radix, it reads the `0x` prefix as hexadecimal, and "everything else is parsed as a decimal value".[^3] Example: [`parse`](#parse).

## Examples

The examples run with Node.js 24.

Constants, tests and formatting (rule r4). The constant values are those in the MDN table:[^1]

```javascript run id=number-object
console.log(Number.MAX_VALUE, Number.MIN_VALUE, Number.MAX_SAFE_INTEGER);
console.log(Number.isInteger(5.0), Number.isInteger(5.5), Number.isFinite(1 / 0));
const x = 1234.5678;
console.log(x.toFixed(2), x.toPrecision(3), x.toExponential(2), (255).toString(16));
```

```output for=number-object
1.7976931348623157e+308 5e-324 9007199254740991
true false false
1234.57 1.23e+3 1.23e+3 ff
```

Math functions (rule r5):

```javascript run id=math
console.log(Math.round(2.5), Math.round(-2.5), Math.floor(-1.5), Math.trunc(-1.5));
console.log(Math.max(1, 3, 2), Math.max(), Math.sqrt(2), Math.PI);
```

```output for=math
3 -2 -2 -1
3 -Infinity 1.4142135623730951 3.141592653589793
```

`Number()` against `parseInt` and `parseFloat` (rules r6 and r7):

```javascript run id=parse
console.log(Number("42px"), parseInt("42px"), parseFloat("3.14abc"));
console.log(Number(""), Number(" 12 "), Number("0b11"), parseInt("0b11"));
console.log(parseInt("0x1F"), parseInt("ff", 16), parseInt("08"), parseInt("101", 2));
```

```output for=parse
NaN 42 3.14
0 12 3 0
31 255 8 5
```

A legacy octal literal is a SyntaxError in strict mode (rule r3):

```javascript fails id=legacy-octal expect=exception:SyntaxError
"use strict";
const n = 0755;
```

## Pitfalls

### `Number('')` is 0

An empty input field gives `''`, and `Number('')` is 0, not NaN (rule r6).[^2] Test for the empty string first.

### `parseInt` stops early

`parseInt('42px')` is 42, and `parseInt('0b11')` is 0, because it stops at the first character it cannot read (rule r7).[^3] Use `Number()` when the whole string must be a number.

### `toFixed` returns a string

`(1.5).toFixed(2)` is the string `'1.50'`.[^1] `+` on that string joins strings.

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| number types | one, a binary64 double[^1] | "three distinct numeric types: integers , floating-point numbers , and complex numbers"[^4] |
| big integers | BigInt, a separate type | "Integers have unlimited precision."[^4] |
| fractions | the same Number type[^1] | `float`, "usually implemented using double in C"[^4] |

In Python, `42` and `42.0` have different types. In JavaScript they are one value (rule r1). See [[python/float-type]] and [[javascript/bigints]].

## References
