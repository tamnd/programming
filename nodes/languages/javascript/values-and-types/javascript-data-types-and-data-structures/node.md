---
tag: W0ADDZ
kind: feature
owner: javascript
slug: javascript-data-types-and-data-structures
name: JavaScript data types and data structures
aliases: [data types, language types, the eight types, built-in data structures, JavaScript types]
area: languages/javascript/values-and-types
level: intro
summary: >-
  JavaScript has eight language types: Undefined, Null, Boolean, String, Symbol, Number, BigInt and Object. Arrays, maps, sets, dates and JSON are all built from objects.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-language-types", locator: "6.1 ECMAScript Language Types"}
see_also: [javascript/primitive-values, javascript/dynamic-and-weak-typing, javascript/undefined-and-null, javascript/numbers, javascript/bigints, javascript/keyed-collections, javascript/indexed-collections, python/the-standard-type-hierarchy]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-language-types
    title: ECMAScript 2026 Language Specification
    locator: 6.1 ECMAScript Language Types
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Data_structures
    title: JavaScript data types and data structures, MDN
    locator: "introduction; Dynamic and weak typing; Primitive values; Objects; Indexed collections; Keyed collections; Structured data: JSON"
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/datamodel.html
    title: Data model, The Python Language Reference
    locator: 3.1 Objects, values and types; 3.2.1 None; 3.2.4 numbers.Number
    accessed: 2026-11-18
---

## Synopsis

| Type | Example value | `typeof` result | Kind |
|---|---|---|---|
| Undefined | `undefined` | `'undefined'` | primitive |
| Null | `null` | `'object'` | primitive |
| Boolean | `true` | `'boolean'` | primitive |
| Number | `42`, `3.5`, `NaN` | `'number'` | primitive |
| BigInt | `42n` | `'bigint'` | primitive |
| String | `'text'` | `'string'` | primitive |
| Symbol | `Symbol('id')` | `'symbol'` | primitive |
| Object | `{}`, `[]`, functions | `'object'` or `'function'` | object |

"The ECMAScript language types are Undefined, Null, Boolean, String, Symbol, Number, BigInt, and Object."[^1] The `typeof` column is from MDN.[^2]

```javascript run id=synopsis
const values = [undefined, null, true, 42, 42n, "text", Symbol("id"), {}, [], function () {}];
console.log(values.map((v) => typeof v).join(" "));
```

```output for=synopsis
undefined object boolean number bigint string symbol object object function
```

## Semantics

**r1. Eight types.** ECMA-262 lists eight language types. "An ECMAScript language value is a value that is characterized by an ECMAScript language type."[^1] Example: [`synopsis`](#synopsis).

**r2. Values have types, variables do not.** "Variables in JavaScript are not directly associated with any particular value type, and any variable can be assigned (and re-assigned) values of all types".[^2] See [[javascript/dynamic-and-weak-typing]].

**r3. Primitives.** "All types except Object define immutable values represented directly at the lowest level of the language. We refer to values of these types as primitive values."[^2] So seven of the eight types are primitive. See [[javascript/primitive-values]].

**r4. `typeof null` is `'object'`.** "All primitive types, except null , can be tested by the typeof operator." For `null`, "one has to use === null to test for null ."[^2] Example: [`empty`](#empty).

**r5. Two empty values.** "Conceptually, undefined indicates the absence of a value , while null indicates the absence of an object". "The language usually defaults to undefined when something is devoid of a value", for example a missing property or a `let x;` without a value.[^2] Example: [`empty`](#empty).

**r6. Wrappers.** "All primitive types, except null and undefined , have their corresponding object wrapper types". When code reads a property of a primitive, "JavaScript automatically wraps the value into the corresponding wrapper object". A property access on `null` or `undefined` "throws a TypeError exception".[^2] Example: [`wrapper`](#wrapper) and [`null-property`](#null-property).

**r7. Objects are the only mutable values.** "In JavaScript, objects are the only mutable values. Functions are, in fact, also objects with the additional capability of being callable."[^2] An object is a collection of properties. "Property keys are either strings or symbols." Other key values "are implicitly converted to strings."[^2] Example: [`properties`](#properties).

**r8. Two kinds of property.** A data property has the attributes value, writable, enumerable and configurable. An accessor property has get, set, enumerable and configurable. `Object.getOwnPropertyDescriptor()` reads them.[^2] Example: [`properties`](#properties).

**r9. Data structures from objects.** "Arrays are regular objects for which there is a particular relationship between integer-keyed properties and the length property." Map, Set, WeakMap and WeakSet are keyed collections, typed arrays view a binary buffer, and JSON is "a lightweight data-interchange format, derived from JavaScript".[^2] Example: [`structures`](#structures).

## Examples

The examples run with Node.js 24.

The two empty values (rules r4 and r5):

```javascript run id=empty
let x;
const obj = {};
console.log(x, obj.missing, typeof undefined, typeof null);
console.log(null === null, typeof null === "object", Object.getPrototypeOf(Object.prototype));
```

```output for=empty
undefined undefined undefined object
true true null
```

A property read on a primitive uses its wrapper, so methods work on primitives (rule r6):

```javascript run id=wrapper
console.log((1234.5).toFixed(1), "text".length, "text".toUpperCase(), true.toString());
```

```output for=wrapper
1234.5 4 TEXT true
```

`null` and `undefined` have no wrapper, so a property read on them throws (rule r6):

```javascript fails id=null-property expect=exception:TypeError
const nothing = null;
console.log(nothing.length);
```

Property keys and attributes (rules r7 and r8):

```javascript run id=properties
const obj = { a: 1 };
obj[2] = "two";
console.log(Object.keys(obj), typeof Object.keys(obj)[1]);
console.log(Object.getOwnPropertyDescriptor(obj, "a"));
```

```output for=properties
[ '2', 'a' ] string
{ value: 1, writable: true, enumerable: true, configurable: true }
```

Arrays, maps, sets and JSON are objects (rule r9):

```javascript run id=structures
const list = ["a", "b"];
const map = new Map([["k", 1]]);
const set = new Set([1, 1, 2]);
console.log(typeof list, Array.isArray(list), list.length);
console.log(typeof map, map.get("k"), typeof set, set.size);
console.log(JSON.stringify({ list, n: 1 }));
```

```output for=structures
object true 2
object 1 object 2
{"list":["a","b"],"n":1}
```

## Pitfalls

### `typeof` cannot find `null`

`typeof null` is `'object'`, so a `typeof v === 'object'` test also passes for `null`. Test `v === null` first (rule r4).[^2]

### Strings for complex data

MDN warns against "stringly-typing" code. A list kept in one string with a separator breaks when an item holds the separator. "Use strings for textual data. When representing complex data, parse strings, and use the appropriate abstraction."[^2]

### Plain objects as maps

Objects are often used as maps, "However, there can be ergonomics, security, and performance issues. Use a Map for storing arbitrary data instead."[^2]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| what has a type | each value has one of eight types[^1] | "Every object has an identity, a type and a value."[^3] |
| the empty value | `undefined` and `null`[^2] | `None`, "This type has a single value."[^3] |
| numbers | Number and BigInt[^1] | `numbers.Number`, "Numeric objects are immutable"[^3] |
| mutability | "objects are the only mutable values"[^2] | "The value of some objects can change."[^3] |

Python says "All data in a Python program is represented by objects or by relations between objects."[^3] JavaScript separates primitives from objects (rule r3). See [[python/the-standard-type-hierarchy]].

## References
