---
tag: EJ2B54
kind: feature
owner: javascript
slug: bitwise-operators
name: Bitwise operators
aliases: [bitwise AND, bitwise OR, bitwise XOR, bitwise NOT, "&", "|", "^", "~", "<<", ">>", ">>>", shift operators, zero-fill right shift]
area: languages/javascript/syntax
level: core
summary: >-
  & | ^ ~ << >> >>> convert numbers to 32-bit integers, work on the bits, and give a number. On two BigInts they work on BigInts, except >>>. Shift counts are taken modulo 32. & binds looser than ===.
grammar: "ShiftExpression : AdditiveExpression | ShiftExpression << AdditiveExpression | ShiftExpression >> AdditiveExpression | ShiftExpression >>> AdditiveExpression; BitwiseANDExpression, BitwiseXORExpression and BitwiseORExpression with & ^ | (ECMA-262, 13.9 and 13.12)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-binary-bitwise-operators", locator: "13.9 Bitwise Shift Operators; 13.12 Binary Bitwise Operators; 13.5.6 Bitwise NOT Operator"}
see_also: [javascript/arithmetic-operators, javascript/comparison-operators, javascript/assignment-operators, javascript/expressions-and-operators, python/bitwise-and-shift-operators]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Expressions_and_operators
    title: Expressions and operators, MDN JavaScript Guide
    locator: Bitwise operators
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-binary-bitwise-operators
    title: ECMAScript 2026 Language Specification
    locator: 13.9 Bitwise Shift Operators; 13.12 Binary Bitwise Operators
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Left_shift
    title: Left shift (<<), MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Bitwise_NOT
    title: Bitwise NOT (~), MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Unsigned_right_shift
    title: Unsigned right shift (>>>), MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Operator_precedence
    title: Operator precedence, MDN
    locator: Table
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.8 Shifting operations; 6.9 Binary bitwise operations; 6.10 Comparisons
    accessed: 2026-11-18
---

## Synopsis

| Operator | Name | `12 op 10`, or `op 12` |
|---|---|---|
| `a & b` | AND | 8 |
| `a \| b` | OR | 14 |
| `a ^ b` | XOR | 6 |
| `~a` | NOT | -13 |
| `a << b` | left shift | 12288 |
| `a >> b` | sign-propagating right shift | 0 |
| `a >>> b` | zero-fill right shift | 0 |

"A bitwise operator treats their operands as a set of 32 bits (zeros and ones), rather than as decimal, hexadecimal, or octal numbers."[^1] ECMA-262 defines the shifts in 13.9, `&`, `^` and `|` in 13.12, and `~` with the unary operators in 13.5.[^2] The operators "return standard JavaScript numerical values."[^1]

## Semantics

**r1. 32-bit integers.** "The operands are converted to thirty-two-bit integers and expressed by a series of bits (zeros and ones). Numbers with more than 32 bits get their most significant bits discarded."[^1] The fraction is dropped too, so `3.7 | 0` is 3. The result of `&`, `|`, `^`, `~`, `<<` and `>>` is a signed 32-bit integer: "values with the most significant (left-most) bit set to 1 represent negative numbers (two's-complement representation)."[^1] Example: [`bits`](#bits).

**r2. NOT.** "~x evaluates to the same value that -x - 1 evaluates to."[^1] So `~-1` is 0.

**r3. Shifts.** `<<` shifts in zeros from the right. `>>` copies the leftmost bit in from the left, so the sign stays: "-9>>2 yields -3, because the sign is preserved."[^1] `>>>` shifts in zeros, and its result is an unsigned 32-bit integer, so `-1 >>> 0` is 4294967295. The right operand "will be converted to an unsigned 32-bit integer and then taken modulo 32", so `100 << 32` is 100.[^3] Example: [`bits`](#bits).

**r4. BigInts.** On two BigInts, `&`, `|`, `^`, `~`, `<<` and `>>` give a BigInt, and do not cut the value to 32 bits.[^3] [^4] A BigInt and a number in one operation throw a TypeError.[^3] `>>>` "does not accept BigInt values", because "BigInts have an infinite number of leading sign bits".[^5] Examples: [`flags`](#flags), [`bigint-unsigned`](#bigint-unsigned).

**r5. Precedence.** In the MDN table, the equality operators are at level 8, and `&`, `^` and `|` are at levels 7, 6 and 5.[^6] So `x & 1 === 0` is `x & (1 === 0)`. The shift operators are at level 10, below `+` and `-`, so `1 << 2 + 1` is `1 << 3`.[^6] Example: [`flags`](#flags).

## Examples

The examples run with Node.js 24.

The operators on small numbers, with 8-bit views of the results, then shifts, unsigned values, the shift count modulo 32 and the cut to 32 bits (rules r1 and r3):

```javascript run id=bits
const bits = (n) => (n >>> 0).toString(2).padStart(8, "0").slice(-8);
console.log(15 & 9, 15 | 9, 15 ^ 9, ~15, ~9);
console.log(bits(12), bits(10), bits(12 & 10), bits(12 | 10), bits(12 ^ 10));
console.log(9 << 2, 9 >> 2, -9 >> 2, 19 >>> 2);
console.log(-9 >>> 0, (-1 >>> 0).toString(16));
console.log(100 << 32, 1 << 31, 2 ** 32 | 0, 3.7 | 0, -3.7 | 0);
```

```output for=bits
9 15 6 -16 -10
00001100 00001010 00001000 00001110 00000110
36 2 -3 4
4294967287 ffffffff
100 -2147483648 0 3 -3
```

Bit flags with the compound operators, a precedence trap, and BigInt operands (rules r4 and r5):

```javascript run id=flags
const READ = 1 << 0;
const WRITE = 1 << 1;
const EXEC = 1 << 2;

let mode = READ | WRITE;
console.log(mode, (mode & WRITE) !== 0, (mode & EXEC) !== 0);
mode |= EXEC;
mode &= ~WRITE;
mode ^= READ;
console.log(mode, mode.toString(2));

console.log(1n << 64n, ~5n, -9n >> 2n, 0xffn & 0x0fn);

const x = 4;
console.log(x & 1 === 0, (x & 1) === 0);
```

```output for=flags
3 true false
4 100
18446744073709551616n -6n -3n 15n
0 true
```

`>>>` on a BigInt (rule r4):

```javascript fails id=bigint-unsigned expect=exception:TypeError
console.log(-9n >>> 2n);
```

Node.js reports `TypeError: BigInts have no unsigned right shift, use >> instead`.

## Pitfalls

### Parentheses around a mask test

Write `(flags & MASK) !== 0`, not `flags & MASK !== 0` (rule r5). The second form compares first.

### `~~x` and `x | 0` to truncate

Both cut a number to 32 bits. MDN warns that `~~` "additionally removes leading bits for numbers outside the range -2147483648 to 2147483647. Use Math.trunc() instead."[^4]

### Large integers

`2 ** 32 | 0` is 0, and `1 << 31` is negative (rule r1). For values above 32 bits, use BigInts.

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| size of the operands | 32 bits for numbers[^1]; unlimited for BigInts | integers of any size; the operators "accept integers as arguments"[^7] |
| `~x` | `-x - 1`[^1] | "The bitwise inversion of x is defined as -(x+1)"[^7] |
| right shift | `>>` and `>>>`[^1] | `>>`: "A right shift by n bits is defined as floor division by pow(2,n)"[^7] |
| `&` and `==` | `==` binds tighter[^6] | comparisons have a priority "lower than that of any arithmetic, shifting or bitwise operation"[^7] |

So `x & 1 == 0` means `x & (1 == 0)` in JavaScript, and `(x & 1) == 0` in Python.[^6] [^7] See [[python/bitwise-and-shift-operators]].

## References
