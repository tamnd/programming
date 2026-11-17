---
tag: MBNZK8
kind: feature
owner: javascript
slug: comparison-operators
name: Comparison operators
aliases: [equality, "==", "===", "!=", "!==", loose equality, strict equality, IsLooselyEqual, IsStrictlyEqual, less than, greater than]
area: languages/javascript/syntax
level: intro
summary: >-
  == and != convert types before they compare; === and !== do not. <, >, <= and >= compare numbers, or strings by code unit. Objects are equal only to themselves, and NaN is equal to nothing.
grammar: "EqualityExpression : EqualityExpression == RelationalExpression | ... != ... | ... === ... | ... !== ...; RelationalExpression : RelationalExpression < ShiftExpression | ... > ... | ... <= ... | ... >= ... (ECMA-262, 13.10 and 13.11)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-equality-operators", locator: "13.10 Relational Operators; 13.11 Equality Operators"}
see_also: [javascript/expressions-and-operators, javascript/assignment-operators, javascript/arithmetic-operators, python/comparison-operators, python/rich-comparison-methods]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Expressions_and_operators
    title: Expressions and operators, MDN JavaScript Guide
    locator: Comparison operators
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-equality-operators
    title: ECMAScript 2026 Language Specification
    locator: 13.10 Relational Operators; 13.11 Equality Operators
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Strict_equality
    title: Strict equality (===), MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Equality
    title: Equality (==), MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Equality_comparisons_and_sameness
    title: Equality comparisons and sameness, MDN JavaScript Guide
    locator: introduction
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.10 Comparisons; 6.10.1 Value comparisons
    accessed: 2026-11-18
---

## Synopsis

| Operator | Name | Converts types |
|---|---|---|
| `a == b`, `a != b` | equal, not equal | yes |
| `a === b`, `a !== b` | strict equal, strict not equal | no |
| `a < b`, `a > b`, `a <= b`, `a >= b` | less than, greater than, less or equal, greater or equal | yes |

"A comparison operator compares its operands and returns a logical value based on whether the comparison is true."[^1] ECMA-262 puts `<`, `>`, `<=` and `>=` in 13.10 Relational Operators, with `in` and `instanceof`, and the four equality operators in 13.11.[^2]

## Semantics

**r1. Strict equality.** `===` and `!==` "provide the IsStrictlyEqual semantic. If the operands are of different types, return false."[^3] Objects are equal "only if they refer to the same object."[^3] "If either operand is NaN, return false."[^3] Example: [`table`](#table).

**r2. Loose equality.** `==` and `!=` "provide the IsLooselyEqual semantic."[^4] Values of the same type compare as with `===`.[^4] "If one of the operands is null or undefined, the other must also be null or undefined to return true."[^4] Otherwise the operands are converted: an object to a primitive, a boolean to a number, and a string to a number when the other side is a number.[^4] So `3 == '3'`, `true == 1` and `[1, 2] == '1,2'` are true. Example: [`table`](#table).

**r3. Zero and NaN.** Both `==` and `===` treat `+0` and `-0` as equal, and `NaN` as unequal to itself.[^5] `Object.is()` is a third test, which "does no type conversion and no special handling for NaN, -0, and +0".[^5] Example: [`table`](#table).

**r4. Order.** The relational operators convert both sides to primitives. If both are strings, "Strings are compared based on standard lexicographical ordering, using Unicode values."[^1] Otherwise the comparison is "generally" numeric.[^1] So `'2' < '12'` is false, and `'2' < 12` is true. A comparison with `NaN` is always false. Example: [`order`](#order).

**r5. `null` in comparisons.** `null >= 0` converts `null` to the number 0 and is true. `null == 0` uses rule r2 and is false. Example: [`order`](#order).

**r6. No chains.** `a < b < c` is `(a < b) < c`: the first result, a boolean, becomes 0 or 1 for the second comparison. So `3 > 2 > 1` is false. Example: [`order`](#order).

**r7. Errors.** A Symbol cannot convert to a number, so `Symbol() < 1` throws a TypeError. With `==`, "If one of the operands is a Symbol but the other is not, return false."[^4] Example: [`symbol-order`](#symbol-order).

## Examples

The examples run with Node.js 24.

Pairs of values under `==`, `===` and `Object.is` (rules r1 to r3):

```javascript run id=table
const show = (v) =>
  typeof v === "string" ? JSON.stringify(v)
  : Object.is(v, -0) ? "-0"
  : Array.isArray(v) ? `[${v}]`
  : String(v);
const pairs = [
  [3, "3"], [0, ""], ["", "0"], [null, undefined], [null, 0],
  [true, 1], [true, "true"], [NaN, NaN], [0, -0], [[1, 2], "1,2"],
];
console.log("a, b".padEnd(20), "==    ===   Object.is");
for (const [a, b] of pairs) {
  const row = [a == b, a === b, Object.is(a, b)].map((r) => String(r).padEnd(6));
  console.log(`${show(a)}, ${show(b)}`.padEnd(20), row.join("").trimEnd());
}
```

```output for=table
a, b                 ==    ===   Object.is
3, "3"               true  false false
0, ""                true  false false
"", "0"              false false false
null, undefined      true  false false
null, 0              false false false
true, 1              true  false false
true, "true"         false false false
NaN, NaN             false false true
0, -0                true  true  false
[1,2], "1,2"         true  false false
```

`true == 'true'` is false: the boolean becomes 1, the string becomes `NaN`, and `1 == NaN` is false.[^4]

Objects, strings, mixed types, `null`, `NaN` and a chain (rules r1 and r4 to r6). `'B' < 'a'` because the code unit of B is smaller than that of a:

```javascript run id=order
const a = { n: 1 };
const b = { n: 1 };
const c = a;
console.log(a == b, a === b, a === c);
console.log([1] == [1], "abc" === "abc");

console.log(2 < 12, "2" < "12", "2" < 12);
console.log("B" < "a", "a" < "b", "\u00e9" > "z");
console.log(null >= 0, null > 0, null == 0);
console.log(NaN < 1, NaN >= 1, NaN != NaN);
console.log(1 < 2 < 3, 3 > 2 > 1);
```

```output for=order
false false true
false true
true false true
true true true
true false false
false false true
true false
```

A Symbol in an order comparison (rule r7):

```javascript fails id=symbol-order expect=exception:TypeError
const key = Symbol("id");
console.log(key < 1);
```

Node.js reports `TypeError: Cannot convert a Symbol value to a number`.

## Pitfalls

### Equal structure is not equality

`{ n: 1 } === { n: 1 }` is false. "JavaScript does not provide a general deep comparison operator".[^5] Compare the fields, or use a library.

### `== null`

`x == null` is true for both `null` and `undefined`, and for nothing else (rule r2). Some code uses it on purpose. Write `x === null || x === undefined` if the intent must be clear.

### Strings that hold numbers

Values from a form or a URL are strings. `'10' < '9'` is true, because the first code units compare. Convert with `Number()` before an order comparison.

### NaN

`x === NaN` is always false. Use `Number.isNaN(x)` or `Object.is(x, NaN)`.[^5]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| equality with conversion | `==`[^4] | none |
| equality without conversion | `===`[^3] | `==`, with `__eq__` for classes |
| default object equality | the same object[^3] | "based on the identity of the objects"[^6] |
| order of mixed types | converts, often to numbers[^1] | "A default order comparison ( < , > , <= , and >= ) is not provided; an attempt raises TypeError."[^6] |
| chains | `(a < b) < c` | "x < y <= z is equivalent to x < y and y <= z , except that y is evaluated only once"[^6] |

See [[python/comparison-operators]] and [[python/rich-comparison-methods]].

## References
