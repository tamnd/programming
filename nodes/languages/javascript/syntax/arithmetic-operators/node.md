---
tag: BE19XX
kind: feature
owner: javascript
slug: arithmetic-operators
name: Arithmetic operators
aliases: [addition, subtraction, multiplication, division, remainder, "%", unary plus, unary negation, Infinity, NaN]
area: languages/javascript/syntax
level: intro
summary: >-
  + - * / % work on numbers or on two BigInts, never on one of each. Other operands are converted, + joins strings, / by zero gives Infinity, and % keeps the sign of the dividend.
grammar: "MultiplicativeExpression : ExponentiationExpression | MultiplicativeExpression MultiplicativeOperator ExponentiationExpression; MultiplicativeOperator : one of * / %; AdditiveExpression : MultiplicativeExpression | AdditiveExpression + MultiplicativeExpression | AdditiveExpression - MultiplicativeExpression (ECMA-262, 13.7 and 13.8)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-multiplicative-operators", locator: "13.7 Multiplicative Operators; 13.8 Additive Operators"}
see_also: [ieee-754, javascript/expressions-and-operators, javascript/exponentiation-operator, javascript/update-expressions, javascript/assignment-operators, javascript/comparison-operators, python/arithmetic-operators]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Expressions_and_operators
    title: Expressions and operators, MDN JavaScript Guide
    locator: Arithmetic operators
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-multiplicative-operators
    title: ECMAScript 2026 Language Specification
    locator: 13.7 Multiplicative Operators; 13.8 Additive Operators
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Division
    title: Division (/), MDN
    locator: Description; Examples
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Remainder
    title: Remainder (%), MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Addition
    title: Addition (+), MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.7 Binary arithmetic operations
    accessed: 2026-11-18
---

## Synopsis

| Operator | Name | `7 op 2` |
|---|---|---|
| `a + b` | addition, or string concatenation | 9 |
| `a - b` | subtraction | 5 |
| `a * b` | multiplication | 14 |
| `a / b` | division | 3.5 |
| `a % b` | remainder | 1 |
| `+a`, `-a` | unary plus, unary negation | 7, -7 |

"An arithmetic operator takes numerical values (either literals or variables) as their operands and returns a single numerical value."[^1] `**` has a page of its own ([[javascript/exponentiation-operator]]), and so do `++` and `--` ([[javascript/update-expressions]]).

## Semantics

**r1. Floating point.** The operators "work as they do in most other programming languages when used with floating point numbers (in particular, note that division by zero produces Infinity )."[^1] So `7 / 2` is 3.5, not 3, and `0.1 + 0.2` is not exactly 0.3. `0 / 0` is `NaN`. Example: [`numbers`](#numbers).

**r2. Precedence.** `*`, `/` and `%` are in 13.7 Multiplicative Operators, and bind tighter than `+` and `-` in 13.8 Additive Operators.[^2] Each level groups from the left, so `8 - 3 - 2` is 3.

**r3. Conversion.** `-`, `*`, `/` and `%` convert their operands to numeric values first.[^3] [^4] So `'6' * '7'` is 42, and `5 / 'foo'` is `NaN`.[^3] Unary plus "Attempts to convert the operand to a number, if it is not already."[^1] Example: [`numbers`](#numbers).

**r4. `+` is two operators.** "The + operator is overloaded for two distinct operations: numeric addition and string concatenation."[^5] It first converts both operands to primitives. "If one side is a string, the other operand is also converted to a string and they are concatenated."[^5] So `'5' + 2` is `'52'`, but `'5' - 2` is 3. Example: [`numbers`](#numbers).

**r5. Remainder.** `%` is a remainder, not a modulo. The result "has the same sign as the dividend".[^4] MDN: "while in most languages, '%' is a remainder operator, in some (e.g., Python, Perl ) it is a modulo operator."[^4] "The operation returns NaN if one of the operands is NaN, n is ±Infinity, or if d is ±0."[^4] Example: [`remainder`](#remainder).

**r6. BigInt.** If both operands become BigInts, the operator does BigInt arithmetic. "A TypeError is thrown if one operand becomes a BigInt but the other becomes a number."[^3] BigInt division truncates "towards zero, and the remainder is discarded", and `2n / 0n` throws a RangeError, because "BigInt has no concept of infinity."[^3] Examples: [`bigint`](#bigint), [`mixed`](#mixed).

## Examples

The examples run with Node.js 24.

The five binary operators, division by zero, rounding, conversion and unary plus and minus (rules r1, r3 and r4). The empty string and the empty array convert to 0:

```javascript run id=numbers
console.log(7 + 2, 7 - 2, 7 * 2, 7 / 2, 7 % 2);
console.log(1 / 0, -1 / 0, 0 / 0);
console.log(0.1 + 0.2, 0.1 + 0.2 === 0.3);
console.log(5 / "2", "6" * "7", "5" - 2, "5" + 2, 5 / "foo");
console.log(+"3", +true, -"4", +"", +[]);
```

```output for=numbers
9 5 14 3.5 1
Infinity -Infinity NaN
0.30000000000000004 false
2.5 42 3 52 NaN
3 1 -4 0 0
```

The sign of a remainder follows the dividend (rule r5). `mod` gives a modulo with the sign of the divisor:

```javascript run id=remainder
console.log(13 % 5, -13 % 5, 13 % -5, -13 % -5);
console.log(5.5 % 2, -0 % 5, 5 % Infinity, 5 % 0);
const mod = (n, d) => ((n % d) + d) % d;
console.log(mod(-13, 5), mod(13, -5));
```

```output for=remainder
3 -3 3 -3
1.5 -0 5 NaN
2 -2
```

BigInt arithmetic truncates toward zero, and an explicit conversion joins the two types (rule r6):

```javascript run id=bigint
console.log(7n + 2n, 7n / 2n, -7n / 2n, 7n % -2n);
console.log(7n * BigInt(3), Number(7n) / 2);
```

```output for=bigint
9n 3n -3n 1n
21n 3.5
```

A BigInt and a number in one operation (rule r6):

```javascript fails id=mixed expect=exception:TypeError
const big = 10n;
console.log(big + 1);
```

Node.js reports `TypeError: Cannot mix BigInt and other types, use explicit conversions`.

## Pitfalls

### `+` with input from a form

Values from a form, a URL or `process.argv` are strings, so `input + 1` joins strings. Convert first: `Number(input) + 1` (rule r4).

### Integer division

`/` does not truncate. Use `Math.trunc(a / b)` for a quotient toward zero, or `Math.floor(a / b)` for a floor quotient.[^3]

### `%` on negative numbers

`-1 % 3` is -1, not 2. Code that maps an index into a range, such as a ring buffer, needs `((n % d) + d) % d` ([`remainder`](#remainder)).

### Money in floating point

`0.1 + 0.2 !== 0.3` (rule r1). Count in the smallest unit, as integers or BigInts.

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| `7 / 2` | 3.5 | 3.5; "Division of integers yields a float"[^6] |
| floor division | `Math.floor(a / b)` | `a // b`[^6] |
| `%` with a negative operand | sign of the dividend[^4] | "same sign as its second operand (or zero)"[^6] |
| division by zero | `Infinity`, `-Infinity` or `NaN`[^1] | "Division by zero raises the ZeroDivisionError exception."[^6] |
| big integers | BigInt, a type of its own[^3] | the `int` type |
| `'5' + 2` | `'52'`[^5] | an error: "The arguments must either both be numbers or both be sequences of the same type."[^6] |

Python's `%` "yields the remainder from the division of the first argument by the second", with the sign rule above.[^6] See [[python/arithmetic-operators]].

## References
