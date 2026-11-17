---
tag: DKGFC7
kind: feature
owner: javascript
slug: booleans-and-truthiness
name: Booleans and truthiness
aliases: [boolean, "true", "false", truthy, falsy, ToBoolean, "Boolean()", "!!x", boolean context]
area: languages/javascript/values-and-types
level: intro
summary: >-
  The Boolean type has true and false. In a condition, ToBoolean turns any value into one of them: false, 0, -0, 0n, NaN, the empty string, null and undefined are falsy, and all other values are truthy.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-toboolean", locator: "7.1.2 ToBoolean"}
see_also: [javascript/undefined-and-null, javascript/primitive-wrapper-objects, javascript/logical-operators, javascript/nullish-coalescing-operator, javascript/dynamic-and-weak-typing, boolean-type, python/truth-value-testing]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-toboolean
    title: ECMAScript 2026 Language Specification
    locator: 7.1.2 ToBoolean
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Falsy
    title: Falsy, MDN Glossary
    locator: introduction; Examples
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Boolean
    title: Boolean, MDN
    locator: introduction; Description; Boolean primitives and Boolean objects; Boolean coercion
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Truthy
    title: Truthy, MDN Glossary
    locator: introduction
    accessed: 2026-11-18
  - url: https://docs.python.org/3/library/stdtypes.html
    title: Built-in Types, The Python Standard Library
    locator: Truth Value Testing
    accessed: 2026-11-18
---

## Synopsis

| Falsy value | Type |
|---|---|
| `false` | Boolean |
| `0`, `-0` | Number |
| `0n` | BigInt |
| `NaN` | Number |
| `''` | String |
| `null` | Null |
| `undefined` | Undefined |

The ToBoolean steps: "If argument is a Boolean , return argument . If argument is one of undefined , null , +0 𝔽 , -0 𝔽 , NaN , 0 ℤ , or the empty String, return false ." Else "Return true ."[^1] The web host adds `document.all` as a falsy object.[^1] The types in the table are from the MDN list of falsy values.[^2]

```javascript run id=synopsis
const values = [false, 0, -0, 0n, NaN, "", null, undefined, "0", "false", [], {}, -1];
console.log(values.map((v) => Boolean(v)).join(" "));
```

```output for=synopsis
false false false false false false false false true true true true true
```

## Semantics

**r1. Two values.** "Boolean values can be one of two values: true or false , representing the truth value of a logical proposition."[^3] They "are typically produced by relational operators , equality operators , and logical NOT ( ! )".[^3]

**r2. Boolean contexts.** "JavaScript uses type conversion to coerce any value to a Boolean in contexts that require it, such as conditionals and loops ."[^2] So `if`, `while`, `? :`, `!`, `&&` and `||` all use ToBoolean on a value of any type. Example: [`contexts`](#contexts).

**r3. The falsy values.** "All values are truthy unless they are defined as falsy . That is, all values are truthy except false , 0 , -0 , 0n , "" , null , undefined , NaN , and document.all ."[^4] Example: [`synopsis`](#synopsis).

**r4. All objects are truthy.** In the Boolean coercion list, "All objects become true ."[^3] So `[]`, `{}` and `new Boolean(false)` are truthy. Strings with text, such as `'0'` and `'false'`, are also truthy.[^4] Example: [`synopsis`](#synopsis).

**r5. Explicit conversion.** "For converting non-boolean values to boolean, use Boolean as a function or use the double NOT operator. Do not use the Boolean() constructor with new ."[^3] Example: [`convert`](#convert).

**r6. `&&` and `||` keep the operands.** "Note that binary logical operators such as && and || return the values of the operands, which may or may not be boolean values."[^3] Example: [`contexts`](#contexts).

## Examples

The examples run with Node.js 24.

Conditions and logical operators (rules r2 and r6):

```javascript run id=contexts
const items = [];
if (items) {
  console.log("an empty array is truthy");
}
if (!items.length) {
  console.log("but its length is 0");
}
console.log(true && "dog", [] && "dog", 0 || "fallback", "" ? "yes" : "no");
```

```output for=contexts
an empty array is truthy
but its length is 0
dog dog fallback no
```

`Boolean()`, `!!` and `new Boolean()` (rule r5):

```javascript run id=convert
const x = "";
console.log(Boolean(x), !!x, typeof Boolean(x));
const wrapped = new Boolean(false);
console.log(typeof wrapped, Boolean(wrapped), wrapped ? "truthy" : "falsy");
```

```output for=convert
false false boolean
object true truthy
```

A falsy test drops a valid 0. `??` tests only for `null` and `undefined`:

```javascript run id=zero
function withDefault(count) {
  return [count || 10, count ?? 10];
}
console.log(withDefault(0), withDefault(undefined), withDefault(5));
```

```output for=zero
[ 10, 0 ] [ 10, 10 ] [ 5, 5 ]
```

## Pitfalls

### `0` and `''` are real values

`count || 10` replaces 0 with 10 ([`zero`](#zero)). Use `??` when 0 and `''` are valid values. See [[javascript/nullish-coalescing-operator]].

### Empty arrays and objects are truthy

`if ([])` runs its body (rule r4). Test `arr.length` for an empty array.

### `'false'` is truthy

Any string with text is truthy, including `'false'` and `'0'`.[^4]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| boolean values | `true`, `false`[^3] | `True`, `False`[^5] |
| zero numbers | falsy[^4] | "zero of any numeric type" is false[^5] |
| empty string | falsy[^4] | false[^5] |
| empty array or list | truthy (rule r4) | "empty sequences and collections" are false[^5] |
| custom rule | none, all objects are truthy[^3] | `__bool__()` or `__len__()`[^5] |

In Python, "an object is considered true unless its class defines either a __bool__() method that returns False or a __len__() method that returns zero".[^5] So an empty list is false in Python and an empty array is true in JavaScript. See [[python/truth-value-testing]].

## References
