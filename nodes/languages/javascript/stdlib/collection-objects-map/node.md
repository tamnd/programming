---
tag: NEW-javascript-collection-objects-map
kind: feature
owner: javascript
slug: collection-objects-map
name: Collection objects map
aliases: [Array, TypedArray, Uint8Array, Float16Array, Map, Set, WeakMap, WeakSet, indexed collections, keyed collections, which collection to use]
area: languages/javascript/stdlib
level: intro
summary: >-
  Clauses 23 and 24 of ECMA-262 define the collections: Array and the 12 typed arrays, which use an index, and Map, Set, WeakMap and WeakSet, which use keys. This page maps them.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/indexed-collections.html", locator: "23 Indexed Collections (23.1 to 23.3); 24 Keyed Collections (24.1 to 24.4)"}
see_also: [javascript/standard-built-in-objects, javascript/indexed-collections, javascript/keyed-collections, javascript/arrays, javascript/maps, hash-table]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects
  title: MDN Web Docs, JavaScript Reference, Standard built-in objects
  locator: Indexed collections; Keyed collections
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/indexed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 23
  locator: 23.1 Array Objects; 23.2 TypedArray Objects; 23.2.1 The %TypedArray% Intrinsic Object; 23.3 Uint8Array Objects
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/keyed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 24
  locator: 24.1 Map Objects; 24.1.3.7 Map.prototype.getOrInsert; 24.2 Set Objects; 24.3 WeakMap Objects; 24.4 WeakSet Objects
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2025/multipage/indexed-collections.html
  title: ECMAScript 2025 Language Specification (ECMA-262, 16th edition), clause 23
  locator: 23.2 TypedArray Objects
  accessed: 2026-11-18
- url: https://raw.githubusercontent.com/tc39/proposals/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Float16 on TypedArrays; Uint8Array to/from Base64; Upsert
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=kinds
const arr = [3, 1, 2];
const typed = new Uint8Array([3, 1, 256, -1]);
const map = new Map([["a", 1], [{}, 2]]);
const set = new Set([1, 1, 2]);
const wm = new WeakMap();
const key = {};
wm.set(key, "meta");
const ws = new WeakSet([key]);
console.log(arr.length, typed, map.size, set.size, wm.get(key), ws.has(key));
console.log(Array.isArray(typed), ArrayBuffer.isView(typed), typeof wm.size);
```

```output for=kinds
3 Uint8Array(4) [ 3, 1, 0, 255 ] 2 2 meta true
false true undefined
```

| Object | Clause | Use it for | Main page |
|---|---|---|---|
| `Array` | 23.1 | a list of any values in order, with an index | [[javascript/arrays]], [[javascript/array-methods]] |
| typed arrays, such as `Uint8Array` and `Float64Array` | 23.2 | numbers of one fixed type in a binary buffer | [[javascript/javascript-typed-arrays]], [[javascript/arraybuffer-and-views]] |
| `Uint8Array` base64 and hex | 23.3 | bytes to and from text | [[javascript/uint8array-base64-and-hex]] |
| `Map` | 24.1 | values by a key of any type, in insertion order | [[javascript/maps]] |
| `Set` | 24.2 | unique values, in insertion order | [[javascript/sets]], [[javascript/set-methods]] |
| `WeakMap` | 24.3 | data attached to an object or a symbol, with no hold on the key | [[javascript/weakmap-and-weakset]] |
| `WeakSet` | 24.4 | a mark on an object or a symbol, with no hold on it | [[javascript/weakmap-and-weakset]] |

## Semantics

**r1.** MDN has two groups. Indexed collections: "These objects represent collections of data which are ordered by an index value. This includes (typed) arrays and array-like constructs."[^1] Keyed collections: "These objects represent collections which use keys. The iterable collections (Map and Set) contain elements which are easily iterated in the order of insertion."[^1] See [[javascript/indexed-collections]] and [[javascript/keyed-collections]].

**r2.** An `Array` holds values of any type, and its `length` changes when elements are added. It is the default list. Example: [`kinds`](#kinds).

**r3.** "A TypedArray presents an array-like view of an underlying binary data buffer (25.1)."[^2] Each typed array has one element type, and a value converts to that type when it is stored, so `256` becomes `0` in a `Uint8Array`. The 12 constructors share a parent, the %TypedArray% intrinsic, which "does not have a global name or appear as a property of the global object."[^2] A typed array is not an `Array`. Examples: [`kinds`](#kinds), [`typed-list`](#typed-list).

**r4.** "Maps are collections of key/value pairs where both the keys and values may be arbitrary ECMAScript language values."[^3] "Distinct key values are discriminated using the semantics of the SameValueZero comparison algorithm."[^3] So `NaN` finds `NaN`, `-0` finds `0`, and two objects are different keys even when their contents are the same. Examples: [`choose`](#choose), [`set-of-objects`](#set-of-objects).

**r5.** "Set objects are collections of ECMAScript language values. A Set may contain each distinct value at most once."[^3] It uses the same SameValueZero test as `Map`. Example: [`choose`](#choose).

**r6.** "WeakMaps are collections of key/value pairs where the keys are objects and/or symbols and values may be arbitrary ECMAScript language values."[^3] A WeakMap can be queried for a key, "but no mechanism is provided for enumerating the values it holds as keys."[^3] So a weak collection has no `size` and no iteration, and the engine can remove an entry when the key is no longer reachable. Example: [`weakmap-primitive`](#weakmap-primitive).

**r7.** Lookup in a keyed collection is fast. "Maps must be implemented using either hash tables or other mechanisms that, on average, provide access times that are sublinear on the number of elements in the collection."[^3] A search in an `Array` with `includes` or `indexOf` reads the elements one by one. See [[hash-table]].

## Examples

```javascript run id=typed-list
const names = Object.getOwnPropertyNames(globalThis).filter((n) => /^(Big)?(Int|Uint|Float)\d+(Clamped)?Array$/.test(n)).sort();
console.log(names.length, names.join(" "));
const TypedArray = Object.getPrototypeOf(Int8Array);
console.log(TypedArray.name, typeof globalThis.TypedArray, new Float64Array(2) instanceof TypedArray);
console.log(typeof Uint8Array.fromBase64, typeof Map.prototype.getOrInsert);
```

```output for=typed-list
12 BigInt64Array BigUint64Array Float16Array Float32Array Float64Array Int16Array Int32Array Int8Array Uint16Array Uint32Array Uint8Array Uint8ClampedArray
TypedArray undefined true
undefined undefined
```

The parent of `Int8Array` has the name `TypedArray`, but it is not a global (rule r3). The last line shows two functions of the 17th edition that Node v24.10.0 does not have yet.

```javascript run id=choose
const m = new Map();
m.set(NaN, "nan").set(0, "zero");
console.log(m.get(NaN), m.get(-0), m.size);
const words = ["apple", "avocado", "banana", "blueberry", "cherry"];
console.log(Map.groupBy(words, (w) => w[0]));
console.log(new Set(words.map((w) => w[0])), words.toSorted((a, b) => b.length - a.length)[0]);
```

```output for=choose
nan zero 2
Map(3) {
  'a' => [ 'apple', 'avocado' ],
  'b' => [ 'banana', 'blueberry' ],
  'c' => [ 'cherry' ]
}
Set(3) { 'a', 'b', 'c' } blueberry
```

`Map` finds `NaN` and treats `-0` as `0` (rule r4). `Map.groupBy` puts the elements of an array in groups by a key. A `Set` keeps one copy of each first letter (rule r5). `toSorted` returns a new sorted array.

## Versions

The 16th edition (ECMAScript 2025) has `Float16Array` in 23.2.[^4] The 17th edition (ECMAScript 2026) adds "23.3 Uint8Array Objects", with "23.3.1.1 Uint8Array.fromBase64 ( string [ , options ] )".[^2] It also adds "24.1.3.7 Map.prototype.getOrInsert ( key, value )".[^3] The TC39 list of finished proposals gives 2025 for Float16 and 2026 for Uint8Array to/from Base64 and for Upsert.[^5] Node v24.10.0 has `Float16Array`, but not `Uint8Array.fromBase64` or `Map.prototype.getOrInsert` (see [`typed-list`](#typed-list)).

## Pitfalls

### Objects with the same contents in a Set

```javascript wrong id=set-of-objects
const points = new Set([{ x: 1 }, { x: 1 }]);
console.log(points.size);
const keys = new Set(["1,1", "1,1"]);
console.log(keys.size);
```

```output for=set-of-objects
2
1
```

SameValueZero compares objects by identity, not by contents (rule r4). Two object literals are two objects. To treat equal contents as one value, use a string or a number as the key.

### A primitive as a WeakMap key

```javascript fails id=weakmap-primitive expect=exception:TypeError
const wm = new WeakMap();
wm.set({}, 1);
wm.set("name", 1);
```

A WeakMap key must be an object or a symbol that is not registered (rule r6). A string can never be collected, so it cannot be a weak key. Use a `Map` for string keys.

## Comparison

Python has `list`, `dict` and `set` as built-in types, with syntax for each. JavaScript has syntax only for arrays and objects. `Map` and `Set` have no literal form, so code makes them with `new`. A plain object can also act as a map with string keys, but `Map` takes keys of any type and keeps their order (rule r4). Typed arrays have no direct match in Python's built-in types. The closest are `bytearray` and the module `array`.

## References
