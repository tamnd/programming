---
tag: ZTYF4S
kind: feature
owner: javascript
slug: the-ecmascript-language-types
name: The ECMAScript language types
aliases: [ECMAScript language types, language types, language value, Object type, property attributes, data property, accessor property, property key, internal methods, internal slots]
area: languages/javascript/values-and-types
level: advanced
summary: >-
  Clause 6.1 of ECMA-262 defines the eight language types: Undefined, Null, Boolean, String, Symbol, Number, BigInt and Object. An Object is a collection of properties with attributes.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-language-types", locator: "6.1 ECMAScript Language Types"}
see_also: [javascript/javascript-data-types-and-data-structures, javascript/specification-types, javascript/property-descriptors, javascript/primitive-values, javascript/symbols, javascript/the-typeof-operator, python/objects-values-and-types, python/the-standard-type-hierarchy]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-language-types
    title: ECMAScript 2026 Language Specification
    locator: 6.1 ECMAScript Language Types; 6.1.4 The String Type; 6.1.6 Numeric Types; 6.1.7 The Object Type; 6.1.7.1 Property Attributes; 6.1.7.2 Object Internal Methods and Internal Slots
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-typeof-operator
    title: ECMAScript 2026 Language Specification
    locator: 13.5.3 The typeof Operator
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/datamodel.html
    title: Data model, The Python Language Reference
    locator: 3.1 Objects, values and types
    accessed: 2026-11-18
---

## Synopsis

| Type | Values | Clause |
|---|---|---|
| Undefined | `undefined` | 6.1.1 |
| Null | `null` | 6.1.2 |
| Boolean | `true`, `false` | 6.1.3 |
| String | sequences of 16-bit code units | 6.1.4 |
| Symbol | unique property keys that are not Strings | 6.1.5 |
| Number | IEEE 754 binary64 values, with one NaN | 6.1.6.1 |
| BigInt | integers of any size | 6.1.6.2 |
| Object | collections of properties | 6.1.7 |

"The ECMAScript language types are Undefined, Null, Boolean, String, Symbol, Number, BigInt, and Object. An ECMAScript language value is a value that is characterized by an ECMAScript language type."[^1]

```javascript run id=synopsis
const values = [undefined, null, true, "s", Symbol(), 1, 1n, {}];
console.log(values.map((v) => typeof v).join(" "));
```

```output for=synopsis
undefined object boolean string symbol number bigint object
```

## Semantics

**r1. Eight types.** "An ECMAScript language type corresponds to values that are directly manipulated by an ECMAScript programmer using the ECMAScript language."[^1] Functions are not a type of their own: "All functions in ECMAScript are members of the Object type."[^1]

**r2. typeof is not the type.** typeof gives `'object'` for null and `'function'` for an object with a `[[Call]]` internal method.[^2] So its result names the language type in most cases, but not in these two.[^1][^2] Example: [`synopsis`](#synopsis).

**r3. String.** "The String type is the set of all ordered sequences of zero or more 16-bit unsigned integer values" up to a length of 2^53 - 1. Each element is usually a UTF-16 code unit.[^1]

**r4. Number and BigInt.** The Number type has "exactly 18,437,736,874,454,810,627" values, the binary64 values with the NaN values of IEEE 754 seen as "a single special NaN value".[^1] "The BigInt type represents an integer value. The value may be any size and is not limited to a particular bit-width."[^1]

**r5. Object.** "Each instance of the Object type, also referred to simply as an Object, represents a collection of properties." A property is a data property or an accessor property. "A property key is either a String or a Symbol."[^1]

**r6. Property attributes.** A data property has `[[Value]]` and `[[Writable]]`. An accessor property has `[[Get]]` and `[[Set]]`. Both have `[[Enumerable]]` and `[[Configurable]]`. Each attribute's default value is `undefined` or false.[^1] Example: [`attributes`](#attributes).

**r7. Internal methods and slots.** "The actual semantics of objects, in ECMAScript, are specified via algorithms called internal methods." They "are not part of the ECMAScript language". "Internal slots are not object properties and they are not inherited."[^1]

## Examples

The examples run with Node.js 24.

`Object.getOwnPropertyDescriptor` shows the attributes (rule r6):

```javascript run id=attributes
const obj = { x: 1, get y() { return 2; } };
Object.defineProperty(obj, "z", { value: 3 });
console.log(Object.getOwnPropertyDescriptor(obj, "x"));
console.log(Object.getOwnPropertyDescriptor(obj, "y"));
console.log(Object.getOwnPropertyDescriptor(obj, "z"));
```

```output for=attributes
{ value: 1, writable: true, enumerable: true, configurable: true }
{
  get: [Function: get y],
  set: undefined,
  enumerable: true,
  configurable: true
}
{ value: 3, writable: false, enumerable: false, configurable: false }
```

An array index key is a String (rule r5):

```javascript run id=keys
const arr = ["a", "b"];
console.log(Reflect.ownKeys(arr), typeof Reflect.ownKeys(arr)[0]);
console.log(arr["1"] === arr[1]);
```

```output for=keys
[ '0', '1', 'length' ] string
true
```

A non-writable property in strict mode (rule r6):

```javascript fails id=strict-write expect=exception:TypeError
"use strict";
const o = {};
Object.defineProperty(o, "fixed", { value: 1 });
o.fixed = 2;
```

## Pitfalls

### defineProperty defaults to false

`Object.defineProperty(o, 'z', { value: 3 })` makes `z` non-writable, non-enumerable and non-configurable, since the attribute defaults are false ([`attributes`](#attributes)).[^1]

### Number keys become Strings

`arr[1]` and `arr['1']` read the same property, because a key is a String or a Symbol (rule r5).

## Comparison

| Topic | ECMAScript | Python |
|---|---|---|
| what a value is | a value of one of eight language types[^1] | "Every object has an identity, a type and a value."[^3] |
| primitives | seven types that are not Object[^1] | each value is an object[^3] |
| type of a type | a language type is not a value[^1] | "The type() function returns an object’s type (which is an object itself)."[^3] |

Python says "All data in a Python program is represented by objects or by relations between objects."[^3] ECMAScript keeps seven primitive types apart from Object.[^1] See [[python/objects-values-and-types]].

## References
