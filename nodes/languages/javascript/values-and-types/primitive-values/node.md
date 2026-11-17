---
tag: 4F8KFR
kind: feature
owner: javascript
slug: primitive-values
name: Primitive values
aliases: [primitives, primitive types, primitive data types, immutable values]
area: languages/javascript/values-and-types
level: intro
summary: >-
  A primitive is a value that is not an object: a string, number, bigint, boolean, undefined, symbol or null. Primitives are immutable, and === compares them by value.
see_also: [javascript/javascript-data-types-and-data-structures, javascript/primitive-wrapper-objects, javascript/undefined-and-null, javascript/strings, javascript/strict-mode, immutability, value-types-and-reference-types, python/objects-values-and-types]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Primitive
    title: Primitive, MDN Glossary
    locator: introduction
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Data_structures
    title: JavaScript data types and data structures, MDN
    locator: Primitive values; String type; Objects
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String
    title: String, MDN
    locator: Character access
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Strict_equality
    title: Strict equality (===), MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/datamodel.html
    title: Data model, The Python Language Reference
    locator: 3.1 Objects, values and types; 3.2.4 numbers.Number; 3.2.5.1 Immutable sequences
    accessed: 2026-11-18
---

## Synopsis

| Primitive type | Example | Wrapper object |
|---|---|---|
| string | `'cat'` | `String` |
| number | `42` | `Number` |
| bigint | `42n` | `BigInt` |
| boolean | `true` | `Boolean` |
| undefined | `undefined` | none |
| symbol | `Symbol('id')` | `Symbol` |
| null | `null` | none |

"In JavaScript , a primitive (primitive value, primitive data type) is data that is not an object and has no methods or properties . There are 7 primitive data types".[^1] The wrapper column is from MDN.[^2]

```javascript run id=synopsis
let s = "cat";
s.toUpperCase();
console.log(s);
s = s.toUpperCase();
console.log(s, "abc" === "a" + "bc", {} === {});
```

```output for=synopsis
cat
CAT true false
```

## Semantics

**r1. Seven primitive types.** "All types except Object define immutable values represented directly at the lowest level of the language. We refer to values of these types as primitive values."[^2] Example: [`synopsis`](#synopsis).

**r2. Immutable.** "All primitives are immutable ; that is, they cannot be altered." "The language does not offer utilities to mutate primitive values."[^1] A string method such as `toUpperCase()` returns a different string, and the first string stays as it was. "JavaScript strings are immutable."[^2] Example: [`synopsis`](#synopsis).

**r3. The variable is not the value.** "It is important not to confuse a primitive itself with a variable assigned a primitive value. The variable may be reassigned to a new value, but the existing value cannot be changed".[^1] Example: [`copy`](#copy).

**r4. Methods through wrappers.** "Primitives have no methods but still behave as if they do. When properties are accessed on primitives, JavaScript auto-boxes the value into a wrapper object and accesses the property on that object instead."[^1] `null` and `undefined` have no wrapper.[^2] See [[javascript/primitive-wrapper-objects]].

**r5. Writes to a primitive are lost.** `str.foo = 1` "is not assigning to the property foo of str itself, but to an ephemeral wrapper object".[^1] For a string index, an attempt "to delete or assign a value to these properties will not succeed. The properties involved are neither writable nor configurable."[^3] In sloppy mode the write fails with no error. In strict mode it throws a TypeError. Example: [`lost-write`](#lost-write) and [`strict-write`](#strict-write).

**r6. Comparison by value.** For `===`, "Numbers must have the same numeric values", "Strings must have the same characters in the same order" and "Booleans must be both true or both false". For objects, it returns true "only if they refer to the same object."[^4] Example: [`compare`](#compare).

## Examples

The examples run with Node.js 24.

A primitive copies as a value. An object is shared between two variables (rule r3):

```javascript run id=copy
let a = 1;
let b = a;
b += 1;
console.log(a, b);

const o = { n: 1 };
const p = o;
p.n += 1;
console.log(o.n, p.n);
```

```output for=copy
1 2
2 2
```

A write to a primitive has no effect in sloppy mode (rule r5):

```javascript run id=lost-write
const str = "cat";
str.foo = 1;
str[0] = "b";
console.log(str, str.foo, str[0]);
```

```output for=lost-write
cat undefined c
```

The same write to a string index throws in strict mode (rule r5):

```javascript fails id=strict-write expect=exception:TypeError
"use strict";
const str = "cat";
str[0] = "b";
```

Primitives compare by value, objects by identity (rule r6):

```javascript run id=compare
console.log("hello" === "hello", 3 === 3, 1n === 1n, true === true);
console.log([1] === [1], { a: 1 } === { a: 1 }, Symbol("x") === Symbol("x"));
```

```output for=compare
true true true true
false false false
```

Two calls to `Symbol('x')` give two different symbols, so a symbol is equal only to itself.

## Pitfalls

### Calling a method does not change the string

`s.trim()`, `s.toUpperCase()` and `s.replace()` return a value. Assign the result, as in `s = s.toUpperCase()` (rule r2).

### Silent failed writes

In sloppy mode, `str[0] = 'b'` does nothing and gives no error ([`lost-write`](#lost-write)). Use strict mode to get the TypeError (rule r5).

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| values that are not objects | seven primitive types[^1] | none, "All data in a Python program is represented by objects"[^5] |
| immutable numbers | yes[^1] | "Numeric objects are immutable"[^5] |
| immutable strings | yes[^2] | "An object of an immutable sequence type cannot change once it is created."[^5] |
| methods on a value | through an auto-boxed wrapper[^1] | on the object itself[^5] |

In Python a number and a string are objects, and each has a type and an identity.[^5] JavaScript has no object behind a primitive, so it creates a wrapper for each property read (rule r4). See [[python/objects-values-and-types]].

## References
