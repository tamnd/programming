---
tag: 0NR2CH
kind: feature
owner: javascript
slug: loose-equality
name: Loose equality
aliases: ["==", "!=", double equals, abstract equality, IsLooselyEqual, loose inequality]
area: languages/javascript/values-and-types
level: core
summary: >-
  x == y converts operands of different types before it compares them. null and undefined equal only each other, Booleans become numbers, and objects become primitives.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-islooselyequal", locator: "7.2.13 IsLooselyEqual"}
see_also: [javascript/equality-comparisons-and-sameness, javascript/type-coercion, javascript/undefined-and-null, javascript/booleans-and-truthiness, javascript/primitive-wrapper-objects, javascript/comparison-operators, python/comparing-sequences-and-other-types]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Equality
    title: Equality (==), MDN
    locator: Description; Examples
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-islooselyequal
    title: ECMAScript 2026 Language Specification
    locator: 7.2.13 IsLooselyEqual
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Equality_comparisons_and_sameness
    title: Equality comparisons and sameness, MDN
    locator: Loose equality using ==
    accessed: 2026-11-18
  - url: https://docs.python.org/3/library/stdtypes.html
    title: Built-in Types, The Python Standard Library
    locator: Comparisons; Boolean Type - bool
    accessed: 2026-11-18
---

## Synopsis

```text
x == y
x != y
```

| Operands | Step |
|---|---|
| same type | compare as `===` |
| `null` and `undefined` | true; each is equal only to the other |
| number and string | convert the string to a number |
| BigInt and string | convert the string to a BigInt; false if that fails |
| a Boolean and any other value | convert the Boolean to a number |
| an object and a primitive | convert the object to a primitive |
| number and BigInt | compare the math values; false for `NaN` and infinities |
| anything else | false |

"The equality (==) operator checks whether its two operands are equal, returning a Boolean result. Unlike the strict equality operator, it attempts to convert and compare operands that are of different types."[^1]

```javascript run id=synopsis
console.log("1" == 1, 0 == false, null == undefined, 0 == null);
console.log(1n == 1, "2" == 2n, [1] == 1, new String("a") == "a");
console.log(NaN == NaN, "" == 0, "0" == false, [] == false);
```

```output for=synopsis
true true true false
true true true true
false true true true
```

## Semantics

**r1. Same type.** "If SameType ( x , y ) is true , then Return IsStrictlyEqual ( x , y )."[^2] Two objects are equal only when they are the same object.[^1]

**r2. null and undefined.** "If one of the operands is null or undefined, the other must also be null or undefined to return true."[^1] So `0 == null` is false.[^1]

**r3. Numbers and strings.** The string becomes a number. "Conversion failure results in NaN, which will guarantee the equality to be false."[^1]

**r4. Booleans.** "If one of the operands is a Boolean but the other is not, convert the boolean to a number: true is converted to 1, and false is converted to 0. Then compare the two operands loosely again."[^1] Example: [`boolean`](#boolean).

**r5. Objects.** An object compared with a string, number, BigInt or symbol goes through ToPrimitive.[^2] See [[javascript/type-coercion]].

**r6. BigInt.** For a number and a BigInt, "If x is not finite or y is not finite, return false." Else the math values are compared.[^2] A string is converted with StringToBigInt, and failure gives false.[^2]

**r7. Symmetry.** "Loose equality is symmetric: A == B always has identical semantics to B == A for any values of A and B (except for the order of applied conversions)."[^3]

**r8. Not transitive.** `'' == 0` and `0 == '0'` are true, but `'' == '0'` is false, because two strings compare as strings (rule r1). Example: [`transitive`](#transitive).

## Examples

The examples run with Node.js 24.

A Boolean is a number in `==` (rule r4):

```javascript run id=boolean
console.log("1" == true, "2" == true, "true" == true);
console.log(Boolean("2"), Boolean("true"));
```

```output for=boolean
true false false
true true
```

`==` is not transitive (rule r8):

```javascript run id=transitive
console.log("" == 0, 0 == "0", "" == "0");
```

```output for=transitive
true true false
```

One comparison tests for both null and undefined (rule r2):

```javascript run id=nullish
for (const v of [null, undefined, 0, "", false]) {
  const name = v === "" ? "(empty)" : String(v);
  console.log(name.padEnd(9), v == null);
}
```

```output for=nullish
null      true
undefined true
0         false
(empty)   false
false     false
```

## Pitfalls

### == true is not truthiness

`'2' == true` is false, but `'2'` is truthy (rule r4). Write `if (x)` to test truthiness. See [[javascript/booleans-and-truthiness]].

### Two wrapper objects

`new Number(3) == new Number(3)` is false, since both operands are objects (rule r1).[^1]

### Prefer ===

"In most cases, using loose equality is discouraged. The result of a comparison using strict equality is easier to predict, and may evaluate more quickly due to the lack of type coercion."[^3] `x == null` is one common use, as in [`nullish`](#nullish).

## Comparison

| Topic | JavaScript `==` | Python `==` |
|---|---|---|
| different types | converts, then compares[^1] | "Unless stated otherwise, objects of different types never compare equal."[^4] |
| `'1'` and `1` | true[^1] | false; a string and a number are different types[^4] |
| Booleans and numbers | `true == 1` is true[^1] | "bool is a subclass of int", so `True == 1`[^4] |

Python has one equality operator with no string to number step.[^4] JavaScript keeps `==` for this conversion and `===` without it.[^1] See [[python/comparing-sequences-and-other-types]].

## References
