---
tag: 3WRJ64
kind: feature
owner: javascript
slug: undefined-and-null
name: undefined and null
aliases: [undefined, "null", nullish, nullish values, "void 0", "typeof null"]
area: languages/javascript/values-and-types
level: intro
summary: >-
  undefined is the value of a missing value: an unset variable, a missing property or a function with no return value. null is the absent object. Both are nullish, and x == null tests for both.
see_also: [javascript/primitive-values, javascript/javascript-data-types-and-data-structures, javascript/booleans-and-truthiness, javascript/nullish-coalescing-operator, javascript/optional-chaining, null-references, python/none-notimplemented-and-ellipsis]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-language-types-undefined-type
    title: ECMAScript 2026 Language Specification
    locator: 6.1.1 The Undefined Type; 6.1.2 The Null Type
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/undefined
    title: undefined, MDN
    locator: Value; Description; Strict equality and undefined; typeof operator and undefined
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Data_structures
    title: JavaScript data types and data structures, MDN
    locator: Undefined type
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/null
    title: null, MDN
    locator: Description; Difference between null and undefined
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Nullish
    title: Nullish value, MDN Glossary
    locator: introduction
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/datamodel.html
    title: Data model, The Python Language Reference
    locator: 3.2.1 None
    accessed: 2026-11-18
---

## Synopsis

| Test | `undefined` | `null` |
|---|---|---|
| `typeof v` | `'undefined'` | `'object'` |
| `v === undefined` | true | false |
| `v === null` | false | true |
| `v == null` | true | true |
| `Boolean(v)` | false | false |
| `JSON.stringify([v])` | `'[null]'` | `'[null]'` |

"The Undefined type has exactly one value, called undefined ." "The Null type has exactly one value, called null ."[^1]

```javascript run id=synopsis
let x;
const obj = {};
function noReturn() {}
console.log(x, obj.missing, noReturn(), void 0, [1, 2].find((n) => n > 5));
console.log(Object.getPrototypeOf(Object.prototype), "sky".match(/[aeiou]/));
```

```output for=synopsis
undefined undefined undefined undefined undefined
null null
```

## Semantics

**r1. Where `undefined` comes from.** "A variable that has not been assigned a value is of type undefined . A function returns undefined if a value was not returned . Accessing a property that does not exist also returns undefined . The void operator always returns undefined ."[^2] Methods such as `Array.prototype.find()` and `Map.prototype.get()` "return undefined when no element is found."[^3] Example: [`synopsis`](#synopsis).

**r2. Where `null` comes from.** "undefined represents the absence of a value, while null represents the absence of an object ." For example, "the end of the prototype chain is null because the prototype chain is composed of objects".[^4] A method returns `null` where a match would be an object, as in `'sky'.match(/[aeiou]/)`. Example: [`synopsis`](#synopsis).

**r3. Keyword against identifier.** "The keyword null is a literal for the null value. Unlike undefined , which is a global variable, null is not an identifier but a syntax keyword."[^4] The global `undefined` is a non-writable, non-configurable property, but "undefined is not a reserved word", so a local variable can have that name.[^2] Example: [`shadow`](#shadow).

**r4. `typeof null`.** `typeof null` gives `'object'`. MDN says: "This is a bug in JavaScript that cannot be fixed due to backward compatibility."[^4] Example: [`compare`](#compare).

**r5. Nullish.** "In JavaScript , a nullish value is the value which is either null or undefined . Nullish values are always falsy ."[^5] `??` and `?.` treat both values the same.[^4] A property read on either one throws a TypeError.[^4] Example: [`nullish`](#nullish) and [`read-undefined`](#read-undefined).

**r6. `==` and `===`.** `null == undefined` is true, and `null === undefined` is false.[^4] So `x == null` tests for both nullish values. `x === undefined` must use strict equality, "because x == undefined also checks whether x is null".[^2] Example: [`compare`](#compare).

**r7. In numbers and JSON.** MDN shows that `1 + null` is not NaN and `1 + undefined` is NaN.[^4] "Unlike undefined , JSON.stringify() can represent null faithfully."[^4] Example: [`compare`](#compare).

## Examples

The examples run with Node.js 24.

`typeof`, equality, arithmetic and JSON (rules r4, r6 and r7):

```javascript run id=compare
console.log(typeof null, typeof undefined, null === undefined, null == undefined);
console.log(1 + null, 1 + undefined, !null, !undefined);
console.log(JSON.stringify({ a: undefined, b: null }), JSON.stringify([undefined, null]));
```

```output for=compare
object undefined false true
1 NaN true true
{"b":null} [null,null]
```

`JSON.stringify` drops a property with the value `undefined`, and writes `null` for it in an array.

The nullish tests (rule r5):

```javascript run id=nullish
function describe(v) {
  return `${v == null} ${v ?? "default"} ${v?.length}`;
}
console.log(describe(undefined));
console.log(describe(null));
console.log(describe(""));
```

```output for=nullish
true default undefined
true default undefined
false  0
```

The empty string is not nullish, so `??` keeps it and `?.` reads its length.

A property read on `undefined` throws (rule r5):

```javascript fails id=read-undefined expect=exception:TypeError
const settings = {};
console.log(settings.theme.color);
```

A local name can hide `undefined`. MDN calls this "a very bad idea" (rule r3):[^2]

```javascript run id=shadow
(() => {
  const undefined = "foo";
  console.log(undefined, typeof undefined);
})();
console.log(typeof undefined);
```

```output for=shadow
foo string
undefined
```

## Pitfalls

### `typeof v === 'object'` passes for `null`

Test `v !== null` as well, or test `v === null` first (rule r4).

### `== undefined` also matches `null`

Use `=== undefined` to test for `undefined` only, and `== null` to test for both (rule r6).[^2]

### Two empty values in an API

MDN says that an API "should likely accept null and undefined as equivalent inputs".[^4] The `??` and `?.` operators do this (rule r5).

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| empty values | `undefined` and `null`[^1] | `None`, "This type has a single value."[^6] |
| a function with no `return` | gives `undefined`[^2] | gives `None`, which "is returned from functions that don’t explicitly return anything"[^6] |
| truth value | falsy[^5] | "Its truth value is false."[^6] |
| a name or a keyword | `null` is a keyword, `undefined` is a global[^4] | `None` is "accessed through the built-in name None"[^6] |

Python has one empty value where JavaScript has two. See [[python/none-notimplemented-and-ellipsis]].

## References
