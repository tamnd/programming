---
tag: NEW-javascript-the-math-object
kind: feature
owner: javascript
slug: the-math-object
name: The Math object
aliases: [Math, Math.round, Math.floor, Math.random, Math.max, Math.PI, Math.sumPrecise, Math.f16round, Math.trunc, Math.hypot]
area: languages/javascript/stdlib
level: intro
summary: >-
  Math is a namespace object with 8 constants and functions for rounding, powers, logarithms and trigonometry, plus Math.random. It works only on Number values and has no integer math.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/numbers-and-dates.html#sec-math-object", locator: "21.3 The Math Object (21.3.1 Value Properties; 21.3.2 Function Properties)"}
see_also: [javascript/numbers, javascript/floating-point-numbers, javascript/bigints, javascript/text-and-numbers-objects-map, floating-point-arithmetic, python/mathematics]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math
  title: MDN Web Docs, JavaScript Reference, Math
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/numbers-and-dates.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 21
  locator: 21.3 The Math Object; 21.3.2.29 Math.round; 21.3.2.34 Math.sumPrecise
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math/random
  title: MDN Web Docs, JavaScript Reference, Math.random()
  locator: the introduction; Examples
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math/f16round
  title: MDN Web Docs, JavaScript Reference, Math.f16round()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math/sumPrecise
  title: MDN Web Docs, JavaScript Reference, Math.sumPrecise()
  locator: the introduction; Return value
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2025/multipage/numbers-and-dates.html
  title: ECMAScript 2025 Language Specification (ECMA-262, 16th edition), clause 21
  locator: 21.3.2 Function Properties of the Math Object
  accessed: 2026-11-18
- url: https://raw.githubusercontent.com/tc39/proposals/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Math.sumPrecise; Float16 on TypedArrays, DataView, Math.f16round
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
console.log(Math.PI, Math.E, Math.SQRT2);
console.log(Math.sqrt(16), Math.cbrt(27), Math.pow(2, 10), 2 ** 10);
console.log(Math.max(3, 7, 1), Math.min(3, 7, 1), Math.abs(-5), Math.sign(-5));
console.log(Math.hypot(3, 4), Math.log10(1000), Math.log2(8));
```

```output for=basics
3.141592653589793 2.718281828459045 1.4142135623730951
4 3 1024 1024
7 1 5 -1
5 3 3
```

| Group | Members |
|---|---|
| Constants | `E`, `LN10`, `LN2`, `LOG10E`, `LOG2E`, `PI`, `SQRT1_2`, `SQRT2` |
| Rounding | `round`, `floor`, `ceil`, `trunc`, `fround`, `f16round` |
| Sign and size | `abs`, `sign`, `max`, `min` |
| Powers and roots | `pow`, `sqrt`, `cbrt`, `exp`, `expm1`, `hypot` |
| Logarithms | `log`, `log1p`, `log10`, `log2` |
| Trigonometry | `sin`, `cos`, `tan`, `asin`, `acos`, `atan`, `atan2`, and the hyperbolic forms `sinh`, `cosh`, `tanh`, `asinh`, `acosh`, `atanh` |
| 32-bit integers | `clz32`, `imul` |
| Random numbers | `random` |
| Sums | `sumPrecise` |

## Semantics

**r1.** "The Math namespace object contains static properties and methods for mathematical constants and functions."[^1] "Unlike most global objects, Math is not a constructor. You cannot use it with the new operator or invoke the Math object as a function. All properties and methods of Math are static."[^1] Example: [`math-call`](#math-call).

**r2.** The 8 constants are in 21.3.1, from "21.3.1.1 Math.E" to "21.3.1.8 Math.SQRT2".[^2] They are not writable, so `Math.PI = 3` has no effect. The functions are in 21.3.2, from "21.3.2.1 Math.abs ( x )" to "21.3.2.37 Math.trunc ( x )".[^2] Example: [`basics`](#basics).

**r3.** There are four ways to round to an integer. `Math.round` "returns the Number value that is closest to x and is integral."[^2] When `x` is halfway between two integers, `Math.round` goes up, toward positive infinity.[^2] `Math.floor` goes down, `Math.ceil` goes up, and `Math.trunc` removes the fraction. Example: [`rounding`](#rounding).

**r4.** The results of many functions can differ by a small amount between engines. MDN: "Many Math functions have a precision that's implementation-dependent."[^1] "This means that different browsers can give a different result. Even the same JavaScript engine on a different OS or architecture can give different results!"[^1] Example: [`trig`](#trig).

**r5.** `Math.random()` "returns a floating-point, pseudo-random number that's greater than or equal to 0 and less than 1".[^3] "The implementation selects the initial seed to the random number generation algorithm; it cannot be chosen or reset by the user."[^3] "Math.random() does not provide cryptographically secure random numbers. Do not use them for anything related to security."[^3] Example: [`random-int`](#random-int).

**r6.** `Math.f16round` "returns the nearest 16-bit half precision float representation of a number."[^4] `Math.sumPrecise` "takes an iterable of numbers and returns the sum of them. It is more precise than summing them up in a loop, because it avoids floating point precision loss in intermediate results."[^5] Example: [`float16`](#float16).

**r7.** `Math` has no integer type and no integer division. Each function takes and returns a Number, which is a 64-bit float. For a quotient, use `Math.trunc(a / b)`. For exact integers above `2 ** 53`, use BigInt and its operators. See [[javascript/bigints]]. Example: [`integer-math`](#integer-math).

## Examples

```javascript run id=rounding
for (const x of [2.5, -2.5, 2.4, -0.5]) {
  console.log(x, Math.round(x), Math.floor(x), Math.ceil(x), Math.trunc(x));
}
```

```output for=rounding
2.5 3 2 3 2
-2.5 -2 -3 -2 -2
2.4 2 2 3 2
-0.5 -0 -1 -0 -0
```

`Math.round(-2.5)` is `-2`, because a half goes toward positive infinity (rule r3). Some results are `-0`, which is equal to `0` with `===`.

```javascript run id=trig
console.log(Math.sin(Math.PI), Math.cos(Math.PI), Math.atan2(1, 1) * 4);
const degrees = 30;
console.log(Math.sin((degrees * Math.PI) / 180).toFixed(3));
```

```output for=trig
1.2246467991473532e-16 -1 3.141592653589793
0.500
```

`Math.PI` is not exactly pi, so the sine of it is a small number and not `0`. The trigonometric functions take radians. Round the result for display (rule r4).

```javascript run id=random-int
function randomInt(min, max) {
  return min + Math.floor(Math.random() * (max - min + 1));
}
const seen = new Set();
for (let i = 0; i < 1000; i++) seen.add(randomInt(1, 6));
console.log([...seen].sort());
const r = Math.random();
console.log(r >= 0 && r < 1);
```

```output for=random-int
[ 1, 2, 3, 4, 5, 6 ]
true
```

`randomInt` gives an integer from `min` to `max`, with both ends included. A program cannot set the seed, so the values change on each run (rule r5). For keys and tokens, use `crypto.getRandomValues`.

```javascript run id=float16
console.log(Math.f16round(1.337), Math.fround(1.337));
console.log(0.1 + 0.2 + 0.3);
console.log(typeof Math.sumPrecise);
```

```output for=float16
1.3369140625 1.3370000123977661
0.6000000000000001
undefined
```

`Math.f16round` and `Math.fround` round to the nearest 16-bit and 32-bit float (rule r6). A loop of `+` rounds at each step. `Math.sumPrecise([0.1, 0.2, 0.3])` rounds only once, but Node v24.10.0 does not have it.

## Versions

The 16th edition (ECMAScript 2025) has "21.3.2.18 Math.f16round ( x )".[^6] The 17th edition (ECMAScript 2026) has it too, and adds "21.3.2.34 Math.sumPrecise ( items )".[^2] The TC39 list of finished proposals gives 2025 for Float16 and 2026 for `Math.sumPrecise`.[^7] Node v24.10.0 has `Math.f16round` but not `Math.sumPrecise` (see [`float16`](#float16)).

## Pitfalls

### An array passed to Math.max

```javascript wrong id=max-array
const xs = [3, 7, 1];
console.log(Math.max(xs), Math.max(...xs));
console.log(Math.max(), Math.min());
```

```output for=max-array
NaN 7
-Infinity Infinity
```

`Math.max` takes numbers, not an array. The array converts to `NaN`. Spread the array into the call. With no arguments, `Math.max` gives `-Infinity` and `Math.min` gives `Infinity`, so test for an empty array first.

### Integer math with Number

```javascript wrong id=integer-math
console.log(2 ** 53, 2 ** 53 + 1, 2n ** 53n + 1n);
console.log(7 / 2, Math.trunc(7 / 2), 7n / 2n, -7 % 3);
```

```output for=integer-math
9007199254740992 9007199254740992 9007199254740993n
3.5 3 3n -1
```

`2 ** 53 + 1` cannot be a Number, so it rounds to `2 ** 53`. The BigInt result is exact. `/` gives a fraction, so use `Math.trunc` or BigInt division for a quotient. The remainder `%` has the sign of the left side (rule r7).

### Math called as a function

```javascript fails id=math-call expect=exception:TypeError
console.log(typeof Math);
Math(2);
```

`Math` is an object, not a function (rule r1). Call its functions, such as `Math.abs(2)`.

## Comparison

Python has a module `math` with about the same functions, and its integers have no size limit, so integer math is exact. See [[python/mathematics]]. In JavaScript, `Math` is always present with no import, but it works only on Number values (rule r7). C and Java have the same functions in `math.h` and `java.lang.Math`, with the same IEEE 754 doubles, so the same rounding errors occur. See [[floating-point-arithmetic]].

## References
