---
tag: SFE917
kind: feature
owner: javascript
slug: equality-comparisons-and-sameness
name: Equality comparisons and sameness
aliases: [equality, "===", "==", "Object.is", SameValue, SameValueZero, IsStrictlyEqual, IsLooselyEqual, strict equality, same-value equality, same-value-zero equality]
area: languages/javascript/values-and-types
level: core
summary: >-
  JavaScript has four equality algorithms: == (IsLooselyEqual), === (IsStrictlyEqual), Object.is (SameValue) and SameValueZero. They differ on type conversion, NaN and -0.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-testing-and-comparison-operations", locator: "7.2.9 SameValue; 7.2.10 SameValueZero; 7.2.13 IsLooselyEqual; 7.2.14 IsStrictlyEqual"}
see_also: [javascript/loose-equality, javascript/nan-and-infinity, javascript/floating-point-numbers, javascript/key-and-value-equality-of-map-and-set, javascript/comparison-operators, javascript/type-coercion, python/rich-comparison-methods]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Equality_comparisons_and_sameness
    title: Equality comparisons and sameness, MDN
    locator: introduction; Strict equality using ===; Same-value equality using Object.is(); Same-value-zero equality
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-testing-and-comparison-operations
    title: ECMAScript 2026 Language Specification
    locator: 7.2.9 SameValue; 7.2.10 SameValueZero; 7.2.13 IsLooselyEqual; 7.2.14 IsStrictlyEqual
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.10.1 Value comparisons; 6.10.3 Identity comparisons
    accessed: 2026-11-18
---

## Synopsis

| Algorithm | Used by | Converts types | `NaN` and `NaN` | `+0` and `-0` |
|---|---|---|---|---|
| IsLooselyEqual | `==`, `!=` | yes | not equal | equal |
| IsStrictlyEqual | `===`, `!==`, `indexOf`, `switch` | no | not equal | equal |
| SameValueZero | `includes`, `Map`, `Set` | no | equal | equal |
| SameValue | `Object.is` | no | equal | not equal |

MDN lists these four algorithms and the operations that use them.[^1] None of them compares the structure of two objects: "For any non-primitive objects x and y which have the same structure but are distinct objects themselves, all of the above forms will evaluate to false."[^1]

```javascript run id=synopsis
console.log(1 == "1", 1 === "1", NaN === NaN, Object.is(NaN, NaN));
console.log(0 === -0, Object.is(0, -0), [NaN].indexOf(NaN), [NaN].includes(NaN));
console.log({ a: 1 } === { a: 1 }, new Set([0, -0, NaN, NaN]).size);
```

```output for=synopsis
true false false true
true false -1 true
false 2
```

## Semantics

**r1. Strict equality.** IsStrictlyEqual "provides the semantics for the === operator". If the types differ, it returns false. For numbers it uses Number::equal, and for other values SameValueNonNumber.[^2] "Neither value is implicitly converted to some other value before being compared."[^1]

**r2. Loose equality.** IsLooselyEqual "provides the semantics for the == operator". If the types are the same, it returns IsStrictlyEqual. Else it handles null and undefined, then converts.[^2] See [[javascript/loose-equality]].

**r3. SameValue.** SameValue "determines whether or not the two arguments are the same value". It "differs from the IsStrictlyEqual Algorithm by treating all NaN values as equivalent and by differentiating +0 𝔽 from -0 𝔽."[^2] `Object.is` gives it to code.[^1]

**r4. SameValueZero.** "SameValueZero differs from SameValue only in that it treats +0 𝔽 and -0 𝔽 as equivalent."[^2] MDN says it is "used by Array.prototype.includes(), TypedArray.prototype.includes(), as well as Map and Set methods for comparing key equality."[^1] Example: [`synopsis`](#synopsis).

**r5. Index search uses strict equality.** Strict equality "is also used by array index-finding methods" and `case` matching, so "you cannot use indexOf(NaN) to find the index of a NaN value in an array".[^1] Example: [`switch-nan`](#switch-nan).

**r6. Objects compare by identity.** Each algorithm treats two objects as equal only when they are the same object.[^1][^2] "JavaScript does not provide a general deep comparison operator".[^1]

## Examples

The examples run with Node.js 24.

A `switch` never matches `NaN` (rule r5):

```javascript run id=switch-nan
function label(x) {
  switch (x) {
    case NaN: return "NaN case";
    default: return "default";
  }
}
console.log(label(NaN), Number.isNaN(NaN));
```

```output for=switch-nan
default true
```

SameValueZero by hand, from MDN (rule r4):

```javascript run id=same-value-zero
function sameValueZero(x, y) {
  if (typeof x === "number" && typeof y === "number") {
    return x === y || (x !== x && y !== y);
  }
  return x === y;
}
console.log(sameValueZero(NaN, NaN), sameValueZero(0, -0), sameValueZero(1, "1"));
```

```output for=same-value-zero
true true false
```

Same content, two objects (rule r6):

```javascript run id=identity
const a = [1, 2];
const b = [1, 2];
const c = a;
console.log(a === b, a == b, Object.is(a, b), a === c);
console.log(JSON.stringify(a) === JSON.stringify(b));
```

```output for=identity
false false false true
true
```

## Pitfalls

### indexOf and includes disagree on NaN

`[NaN].indexOf(NaN)` is `-1`, but `[NaN].includes(NaN)` is `true` (rules r4 and r5).

### Object.is splits zeros

`Object.is(0, -0)` is false, but `0 === -0` is true (rule r3). Use `Object.is` when the sign of zero matters.

### Use === by default

MDN says "Strict equality is almost always the correct comparison operation to use."[^1]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| value equality | `===`; `==` with conversion[^1] | `==` "compare the values of two objects"[^3] |
| identity | `===` for objects[^1] | `x is y` "is true if and only if x and y are the same object"[^3] |
| NaN | `NaN === NaN` is false[^1] | "not-a-number values are not equal to themselves"[^3] |
| user hook | none | rich comparison methods, such as `__eq__`[^3] |
| default for objects | identity[^1] | "based on the identity of the objects"[^3] |

Python `==` compares values and a class can change it, but JavaScript has no hook for `===`.[^1][^3] See [[python/rich-comparison-methods]].

## References
