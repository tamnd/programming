---
tag: 3KP667
kind: feature
owner: javascript
slug: exponentiation-operator
name: Exponentiation operator
aliases: ["**", "**=", power operator, Math.pow, exponentiation]
area: languages/javascript/syntax
level: core
summary: >-
  a ** b raises a to the power b, like Math.pow, and also works on two BigInts. It groups from the right. A unary operator directly before the base, as in -2 ** 2, is a SyntaxError.
grammar: "ExponentiationExpression : UnaryExpression | UpdateExpression ** ExponentiationExpression (ECMA-262, 13.6)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-exp-operator", locator: "13.6 Exponentiation Operator"}
see_also: [javascript/arithmetic-operators, javascript/expressions-and-operators, javascript/assignment-operators, javascript/bitwise-operators, python/arithmetic-operators, python/operator-precedence-in-python]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Exponentiation
    title: Exponentiation (**), MDN
    locator: introduction; Description; Examples
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-exp-operator
    title: ECMAScript 2026 Language Specification
    locator: 13.6 Exponentiation Operator
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.5 The power operator
    accessed: 2026-11-18
---

## Synopsis

```javascript run id=synopsis
console.log(2 ** 3, 10 ** -1, 2 ** 3 ** 2, (-2) ** 2, -(2 ** 2));
```

```output for=synopsis
8 0.1 512 4 -4
```

The ** operator "returns the result of raising the first operand to the power of the second operand. It is equivalent to Math.pow(), except it also accepts BigInts as operands."[^1] `x **= y` is the assignment form ([[javascript/assignment-operators]]).

## Semantics

**r1. Numbers and BigInts.** The operator "first coerces both operands to numeric values and tests the types of them. It performs BigInt exponentiation if both operands become BigInts; otherwise, it performs number exponentiation."[^1] As with the other arithmetic operators, "A TypeError is thrown if one operand becomes a BigInt but the other becomes a number."[^1] Examples: [`numbers`](#numbers), [`bigint`](#bigint).

**r2. Right to left.** "The exponentiation operator is right-associative : a ** b ** c is equal to a ** (b ** c)."[^1] So `2 ** 3 ** 2` is `2 ** 9`, which is 512. The grammar shows it: the right operand is again an ExponentiationExpression.[^2]

**r3. No unary operator on the base.** The left operand is an UpdateExpression, not a UnaryExpression.[^2] So `-2 ** 2`, `typeof 2 ** 2` and `!a ** 2` are syntax errors. MDN: "In JavaScript, it is impossible to write an ambiguous exponentiation expression."[^1] Write `(-2) ** 2` or `-(2 ** 2)`.[^1] `++n ** 2` is valid, because `++n` is an UpdateExpression. Examples: [`numbers`](#numbers), [`unary-base`](#unary-base).

**r4. Special values.** "0 raised to a power of 0 returns 1."[^1] "For numbers, 0 raised to a negative number returns Infinity, while -0 raised to a negative number returns -Infinity."[^1] `NaN ** 0` is 1, but if the base is 1 and the exponent is `±Infinity` or `NaN`, the result is `NaN`, where IEEE 754 gives 1.[^1] A negative base with a fraction as the exponent gives `NaN`. Example: [`numbers`](#numbers).

**r5. BigInt limits.** "For BigInt exponentiation, a RangeError is thrown if the exponent y is negative."[^1] MDN calls `2n ** 1024n` "A very large number, but not Infinity", where `2 ** 1024` is `Infinity`.[^1] Example: [`bigint`](#bigint).

## Examples

The examples run with Node.js 24.

Powers of numbers, the grouping, the sign of the base, special values and string conversion (rules r1 to r4):

```javascript run id=numbers
console.log(2 ** 10, 3 ** 2.5, 10 ** -1, 2 ** 0.5 === Math.SQRT2);
console.log(2 ** 3 ** 2, (2 ** 3) ** 2);
console.log(-(2 ** 2), (-2) ** 2, (-8) ** (1 / 3));
console.log(2 ** 1024, 0 ** -1, (-0) ** -1);
console.log(NaN ** 0, 1 ** Infinity, Math.pow(1, NaN));
console.log(2 ** "3", 2 ** "hello");

let n = 2;
console.log(++n ** 2, n);
n **= 3;
console.log(n);
```

```output for=numbers
1024 15.588457268119896 0.1 true
512 64
-4 4 NaN
Infinity Infinity -Infinity
1 NaN NaN
8 NaN
9 3
27
```

BigInt powers are exact, and a negative BigInt exponent throws (rules r1 and r5). `2n ** 1024n` has 309 digits:

```javascript run id=bigint
console.log(2n ** 64n, String(2n ** 1024n).length);
console.log(2n ** BigInt(3), Number(2n) ** 0.5);
try {
  2n ** -1n;
} catch (error) {
  console.log(error.name);
}
```

```output for=bigint
18446744073709551616n 309
8n 1.4142135623730951
RangeError
```

A unary minus directly before the base (rule r3):

```javascript fails id=unary-base expect=exception:SyntaxError
console.log(-2 ** 2);
```

Node.js reports `SyntaxError: Unary operator used immediately before exponentiation expression. Parenthesis must be used to disambiguate operator precedence`.

## Pitfalls

### `^` is not a power

"JavaScript uses that symbol for the bitwise XOR operator."[^1] `2 ^ 3` is 1. Use `2 ** 3`.

### Code ported from Python

`-2 ** 2` is -4 in Python and a SyntaxError in JavaScript (rule r3).[^1] [^3] The error is good: it makes the intent explicit. Choose `-(2 ** 2)` for the Python meaning.

### Roots of negative numbers

`(-8) ** (1 / 3)` is `NaN`, not -2. Use `Math.cbrt(-8)` for a cube root.

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| operator | `**`, and `Math.pow`[^1] | `**`, and `pow()`[^3] |
| `-2 ** 2` | a SyntaxError[^1] | -4; "The power operator binds more tightly than unary operators on its left"[^3] |
| grouping | from the right[^1] | from the right[^3] |
| integer to a negative power | `10 ** -2` is 0.01; a BigInt throws[^1] | "10**-2 returns 0.01"[^3] |
| negative base, fraction exponent | `NaN` | "results in a complex number"[^3] |

Python groups `**` from the right too: "in an unparenthesized sequence of power and unary operators, the operators are evaluated from right to left".[^3] See [[python/arithmetic-operators]].

## References
