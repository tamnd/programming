---
tag: 6FT5M7
kind: feature
owner: javascript
slug: update-expressions
name: Increment and decrement
aliases: ["++", "--", increment, decrement, prefix increment, postfix increment, update expression]
area: languages/javascript/syntax
level: intro
summary: >-
  x++ and x-- add or subtract one and give the old value; ++x and --x give the new value. The operand must be a variable or a property. The value is converted to a number or a BigInt first.
grammar: "UpdateExpression : LeftHandSideExpression | LeftHandSideExpression [no LineTerminator here] ++ | LeftHandSideExpression [no LineTerminator here] -- | ++ UnaryExpression | -- UnaryExpression (ECMA-262, 13.4)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-update-expressions", locator: "13.4 Update Expressions"}
see_also: [side-effect, javascript/arithmetic-operators, javascript/assignment-operators, javascript/automatic-semicolon-insertion, javascript/expressions-and-operators, python/augmented-assignment]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Expressions_and_operators
    title: Expressions and operators, MDN JavaScript Guide
    locator: Arithmetic operators
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-update-expressions
    title: ECMAScript 2026 Language Specification
    locator: 13.4 Update Expressions; 13.4.1 Static Semantics, Early Errors
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Increment
    title: Increment (++), MDN
    locator: introduction; Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Decrement
    title: Decrement (--), MDN
    locator: introduction; Description
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.6 Unary arithmetic and bitwise operations
    accessed: 2026-11-18
---

## Synopsis

| Form | Effect on `x` | Value |
|---|---|---|
| `x++` | `x` becomes `x + 1` | the old value |
| `++x` | `x` becomes `x + 1` | the updated value |
| `x--` | `x` becomes `x - 1` | the old value |
| `--x` | `x` becomes `x - 1` | the updated value |

"If x is 3, then ++x sets x to 4 and returns 4, whereas x++ returns 3 and, only then, sets x to 4."[^1] ECMA-262 calls these forms update expressions.[^2]

## Semantics

**r1. Prefix and postfix.** "If used postfix, with operator after operand (for example, x++ ), the increment operator increments and returns the value before incrementing."[^3] "If used prefix, with operator before operand (for example, ++x ), the increment operator increments and returns the value after incrementing."[^3] `--` works the same way with subtraction.[^4] Example: [`forms`](#forms).

**r2. Numbers and BigInts.** "It first coerces the operand to a numeric value and tests the type of it. It performs BigInt increment if the operand becomes a BigInt; otherwise, it performs number increment."[^3] So a string variable that holds `5` becomes the number 6, and even the old value that `x++` returns is a number. `null` becomes 1, and a word becomes `NaN`. Example: [`forms`](#forms).

**r3. References only.** "The increment operator can only be applied on operands that are references (variables and object properties; i.e., valid assignment targets )."[^3] ECMA-262 makes an invalid target an early error, so `5++` fails before the code runs.[^2] "++x itself evaluates to a value, not a reference, so you cannot chain multiple increment operators together."[^3] Example: [`chained`](#chained).

**r4. No line break before postfix.** The grammar has "[no LineTerminator here]" before a postfix `++` or `--`.[^2] So a line break before `++` ends the statement, and `++` applies to the next line ([[javascript/automatic-semicolon-insertion]]). Example: [`uses`](#uses).

**r5. Side effects.** An update expression writes to its operand, like an assignment ([[javascript/assignment-operators]]). In `log[i++] = v`, the index is the old `i`, and `i` grows by one. Example: [`uses`](#uses).

## Examples

The examples run with Node.js 24.

Prefix and postfix forms, a BigInt, and conversion of a string, `null` and a word (rules r1 and r2):

```javascript run id=forms
let x = 3;
const post = x++;
console.log(post, x);
const pre = ++x;
console.log(pre, x);
console.log(x--, --x, x);

let big = 3n;
big++;
console.log(big);

let text = "5";
const old = text++;
console.log(typeof old, old, typeof text, text);

let empty = null, word = "five";
empty++;
word++;
console.log(empty, word);
```

```output for=forms
3 4
5 5
5 3 3
4n
number 5 number 6
1 NaN
```

A counter on a property, an index that moves forward, and a line break before `++` (rules r4 and r5). In the last part, `a` stays 1 and `b` becomes 3:

```javascript run id=uses
const counts = { a: 0 };
const items = ["a", "b", "a"];
for (const item of items) {
  counts[item] ??= 0;
  counts[item]++;
}
console.log(counts);

let i = 0;
const log = [];
log[i++] = "first";
log[i++] = "second";
console.log(log, i);

let a = 1
let b = 2
a
++b
console.log(a, b);
```

```output for=uses
{ a: 2, b: 1 }
[ 'first', 'second' ] 2
1 3
```

Two increments in a chain (rule r3):

```javascript fails id=chained expect=exception:SyntaxError
let x = 1;
console.log(++(++x));
```

Node.js reports `SyntaxError: Invalid left-hand side expression in prefix operation`.

## Pitfalls

### The same variable twice in one expression

`i++ + i++` is defined, because operands run from left to right ([[javascript/expressions-and-operators]]), but it is hard to read. Use one update per statement.

### `x++` as a value

`return count++;` returns the old count. If the updated count is meant, write `++count` or `count += 1`.

### A `const` counter

`const n = 0; n++;` throws a TypeError, like any assignment to a `const` ([[javascript/assignment-operators]]). Use `let` for a counter.

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| increment | `x++`, `++x`[^3] | `x += 1`, a statement |
| `++x` | adds one | two unary plus operators: `+(+x)`[^5] |

Python's unary operators are `-`, `+` and `~`, and it has no `++`.[^5] So `++x` in Python is valid but does not change `x`: "The unary + (plus) operator yields its numeric argument unchanged".[^5] See [[python/augmented-assignment]].

## References
