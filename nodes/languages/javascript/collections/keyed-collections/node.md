---
tag: P2VAD9
kind: feature
owner: javascript
slug: keyed-collections
name: Keyed collections
aliases: [Map, Set, WeakMap, WeakSet, keyed collection, Map vs Object]
area: languages/javascript/collections
level: intro
summary: >-
  JavaScript has four keyed collections: Map and Set, which iterate in insertion order, and WeakMap and WeakSet, which hold object or symbol keys weakly and cannot be iterated.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/keyed-collections.html", locator: "24 Keyed Collections"}
see_also: [javascript/maps, javascript/sets, map-and-dictionary, set-concept, hash-table]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Keyed_collections
  title: MDN Web Docs, JavaScript Guide, Keyed collections
  locator: the introduction; Object and Map compared; WeakMap object; WeakSet object; Key and value equality of Map and Set
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/keyed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 24
  locator: 24.1 Map Objects; 24.2 Set Objects; 24.3 WeakMap Objects; 24.4 WeakSet Objects
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Map
  title: MDN Web Docs, JavaScript Reference, Map
  locator: Key equality; Objects vs. Maps; Setting object properties
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Symbols as WeakMap keys, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/stdtypes.html#mapping-types-dict
  title: The Python Standard Library, Python 3.14, Built-in Types
  locator: Set Types; Mapping Types, dict
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/WeakHashMap.html
  title: Java SE 25 API, Class WeakHashMap
  locator: the class description
  accessed: 2026-11-18
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Map types
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=four
const ages = new Map([["Ana", 31], ["Bo", 25]]);
ages.set("Cy", 40);
console.log(ages.get("Bo"), ages.size, ages.has("Dee"));

const tags = new Set(["js", "web", "js"]);
console.log(tags.size, [...tags]);

const user = { id: 1 };
const seen = new WeakSet([user]);
const notes = new WeakMap([[user, { visits: 3 }]]);
console.log(seen.has(user), notes.get(user).visits);
```

```output for=four
25 3 false
2 [ 'js', 'web' ]
true 3
```

| Collection | Holds | Keys or values | Iterable, has `size` |
|---|---|---|---|
| `Map` | key and value pairs | any value | yes |
| `Set` | unique values | any value | yes |
| `WeakMap` | key and value pairs | objects and non-registered symbols as keys | no |
| `WeakSet` | unique values | objects and non-registered symbols | no |

## Semantics

**r1.** The Guide describes "collections of data which are indexed by a key; Map and Set objects contain elements which are iterable in the order of insertion."[^1] A `for...of` loop over a `Map` gives `[key, value]` pairs.[^1]

**r2.** "Maps are collections of key/value pairs where both the keys and values may be arbitrary ECMAScript language values. A distinct key value may only occur in one key/value pair within the Map's collection."[^2] Likewise, "A Set may contain each distinct value at most once."[^2] Setting a key that is already present replaces its value, and adding a value that is already present does nothing. See [[javascript/maps]] and [[javascript/sets]].

**r3.** Keys are compared with SameValueZero: "Distinct key values are discriminated using the semantics of the SameValueZero comparison algorithm."[^2] The Guide spells it out: "Equality works like the identity comparison operator === . -0 and +0 are considered equal. NaN is considered equal to itself (contrary to === )."[^1] Object keys are "compared by reference, not by value."[^3] Example: [`equality`](#equality).

**r4.** Lookups are fast on average. "Maps must be implemented using either hash tables or other mechanisms that, on average, provide access times that are sublinear on the number of elements in the collection."[^2] Set has the same rule.[^2]

**r5.** A `WeakMap` takes keys that are "objects or non-registered symbols", and it "does not create strong references to its keys. That is, an object's presence as a key in a WeakMap does not prevent the object from being garbage collected."[^1] For that reason "a WeakMap doesn't allow observing the liveness of its keys, which is why it doesn't allow enumeration."[^1] The specification says "no mechanism is provided for enumerating the values it holds".[^2] `WeakSet` is the same for values: "WeakSets are collections of objects or symbols only".[^1] Example: [`weak`](#weak).

**r6.** A plain object also maps keys to values, with differences. "The keys of an Object must be either a String or a Symbol", and "An Object has a prototype, so it contains default keys that could collide with your own keys if you're not careful."[^3] The Guide's advice: "Use maps over objects when keys are unknown until run time, especially if the keys come from external input", and "Use objects when the shape is known ahead of time and all keys are expressible as strings."[^1] Example: [`object-or-map`](#object-or-map).

## Examples

SameValueZero in a `Map` and a `Set` (rule r3).

```javascript run id=equality
const m = new Map();
m.set(NaN, "not a number").set(0, "zero");
console.log(m.get(NaN), m.get(-0));

const key = { id: 1 };
m.set(key, "first");
console.log(m.get(key), m.get({ id: 1 }));

const s = new Set([NaN, NaN, 0, -0, "0"]);
console.log(s.size, [...s]);
```

```output for=equality
not a number zero
first undefined
3 [ NaN, 0, '0' ]
```

An object against a `Map` (rule r6).

```javascript run id=object-or-map
const dict = {};
console.log("constructor" in dict, new Map().has("constructor"));

const byObject = { 1: "number", "1": "string" };
const byMap = new Map([[1, "number"], ["1", "string"]]);
console.log(Object.keys(byObject).length, byMap.size);
```

```output for=object-or-map
true false
1 2
```

The object has an inherited `constructor` key and turns the key 1 into `'1'`, so the two entries collapse into one.

What the weak collections accept (rule r5).

```javascript run id=weak
const wm = new WeakMap();
try {
  wm.set("name", 1);
} catch (e) {
  console.log(e.name);
}
const id = Symbol("id");
wm.set(id, 1);
console.log(wm.has(id), wm.size, typeof wm.keys);
try {
  wm.set(Symbol.for("shared"), 1);
} catch (e) {
  console.log(e.name);
}
```

```output for=weak
TypeError
true undefined undefined
TypeError
```

`Symbol.for` makes a registered symbol, so it is not a valid key.

## Versions

The TC39 list of finished proposals gives Symbols as WeakMap keys the expected publication year 2023, so ECMAScript 2023 is the edition that lets non-registered symbols be keys of a `WeakMap` and values of a `WeakSet`.[^4]

## Pitfalls

### Setting properties on a `Map`

Bracket assignment makes an ordinary property on the `Map` object, not an entry. MDN: "that way of setting a property does not interact with the Map data structure."[^3]

```javascript wrong id=brackets
const wrongMap = new Map();
wrongMap["bla"] = "blaa";
console.log(wrongMap.has("bla"), wrongMap.size);

const rightMap = new Map();
rightMap.set("bla", "blaa");
console.log(rightMap.has("bla"), rightMap.size);
```

```output for=brackets
false 0
true 1
```

Use `set`, `get`, `has` and `delete`.

### `JSON.stringify` on a `Map`

A `Map` has "No native support for serialization or parsing."[^3]

```javascript wrong id=json
const m = new Map([["a", 1]]);
console.log(JSON.stringify(m));
console.log(JSON.stringify(Object.fromEntries(m)));
```

```output for=json
{}
{"a":1}
```

Turn the `Map` into an object or an array of entries first, or pass a replacer to `JSON.stringify`.[^3]

## Comparison

Python's dict and set are the matching types. "Dictionaries preserve insertion order", while "A set object is an unordered collection of distinct hashable objects."[^5] Java has a weak map in its library: `WeakHashMap` is a "Hash table based implementation of the Map interface, with weak keys. An entry in a WeakHashMap will automatically be removed when its key is no longer in ordinary use."[^6] In Go, "A map is an unordered group of elements of one type, called the element type, indexed by a set of unique keys of another type".[^7] JavaScript's `Map` and `Set` keep insertion order, as Python's dict does (rule r1). See [[map-and-dictionary]] and [[set-concept]].

## References
