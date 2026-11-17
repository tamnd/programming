---
tag: S6XZR8
kind: feature
owner: javascript
slug: maps
name: Maps
aliases: [Map, new Map, Map.prototype.set, Map.prototype.get, hash map in JavaScript, dictionary in JavaScript]
area: languages/javascript/collections
level: intro
summary: >-
  A Map holds key and value pairs with keys of any type, compared with SameValueZero, and iterates them in insertion order. set adds or replaces, get reads, has tests, delete removes.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/keyed-collections.html#sec-map-objects", locator: "24.1 Map Objects"}
see_also: [javascript/keyed-collections, javascript/sets, javascript/grouping-data, map-and-dictionary, insertion-ordered-map]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Map
  title: MDN Web Docs, JavaScript Reference, Map
  locator: the introduction; Description; Instance methods; Examples
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Keyed_collections
  title: MDN Web Docs, JavaScript Guide, Keyed collections
  locator: Map object
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/keyed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 24
  locator: 24.1.1.1 Map; 24.1.3 Properties of the Map Prototype Object
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Upsert, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/stdtypes.html#mapping-types-dict
  title: The Python Standard Library, Python 3.14, Built-in Types
  locator: Mapping Types, dict
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/LinkedHashMap.html
  title: Java SE 25 API, Class LinkedHashMap
  locator: the class description
  accessed: 2026-11-18
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements with range clause
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const stock = new Map([
  ["apples", 3],
  ["pears", 0],
]);
stock.set("plums", 7).set("apples", 5);

console.log(stock.get("apples"), stock.get("kiwis"), stock.has("pears"));
console.log(stock.size, stock.delete("pears"), stock.delete("pears"));

for (const [fruit, count] of stock) {
  console.log(`${fruit}: ${count}`);
}
console.log([...stock.keys()], [...stock.values()]);
```

```output for=basics
5 undefined true
3 true false
apples: 5
plums: 7
[ 'apples', 'plums' ] [ 5, 7 ]
```

| Task | Code |
|---|---|
| make a map from pairs | `new Map([[k1, v1], [k2, v2]])` |
| add or replace | `m.set(key, value)`, which returns `m` |
| read, test, remove | `m.get(key)`, `m.has(key)`, `m.delete(key)` |
| count, empty | `m.size`, `m.clear()` |
| loop over the pairs | `for (const [k, v] of m)` or `m.forEach((v, k) => ...)` |
| to an array, to an object | `[...m]`, `Object.fromEntries(m)` |

## Semantics

**r1.** "The Map object holds key-value pairs and remembers the original insertion order of the keys. Any value (both objects and primitive values) may be used as either a key or a value."[^1] "A key in the Map may only occur once; it is unique in the Map 's collection."[^1] The Guide puts it in one line: "A Map object is a key/value map that can iterate its elements in insertion order."[^2]

**r2.** The constructor takes an iterable of pairs. The specification's `Map` constructor reads each entry with `AddEntriesFromIterable` and calls `set` for it.[^3] MDN shows the pattern: "Use the regular Map constructor to transform a 2D key-value Array into a map".[^1] Example: [`convert`](#convert).

**r3.** `set` "Adds a new entry with a specified key and value to this Map , or updates an existing entry if the key already exists."[^1] It returns the map, so calls chain.[^3] `get` returns "the value corresponding to the key in this Map , or undefined if there is none."[^1] `delete` returns a boolean, true if an entry was removed.[^3] Example: [`basics`](#basics).

**r4.** "Iteration happens in insertion order, which corresponds to the order in which each key-value pair was first inserted into the map by the set() method".[^1] `set` on a key that is present changes the value in its record and appends nothing, so the key keeps its place,[^3] as `apples` shows in the [`basics`](#basics) example. `keys`, `values` and `entries` each return "a new Iterator object", and the map itself iterates as `entries` does.[^1] `forEach` passes the value first and the key second.[^1]

**r5.** Keys are compared with SameValueZero: "NaN is considered the same as NaN (even though NaN !== NaN ) and all other values are considered equal according to the semantics of the === operator." For object keys, "They are compared by reference, not by value."[^1] See [[javascript/keyed-collections]].

**r6.** Copying a map is shallow: `new Map(original)` makes a new map, but "the data itself is not cloned."[^1] Merging with spread keeps one entry per key, and "The last repeated key wins."[^1] Example: [`merge`](#merge).

## Examples

Between maps, arrays and objects (rule r2).

```javascript run id=convert
const pairs = [["key1", "value1"], ["key2", "value2"]];
const m = new Map(pairs);
console.log(m.get("key1"), Array.from(m.keys()));

const fromObject = new Map(Object.entries({ a: 1, b: 2 }));
console.log(fromObject.get("b"), Object.fromEntries(fromObject));
```

```output for=convert
value1 [ 'key1', 'key2' ]
2 { a: 1, b: 2 }
```

Cloning and merging (rule r6).

```javascript run id=merge
const first = new Map([[1, "one"], [2, "two"], [3, "three"]]);
const second = new Map([[1, "uno"], [2, "dos"]]);
const merged = new Map([...first, ...second]);
console.log([...merged.values()]);

const original = new Map([["list", [1, 2]]]);
const clone = new Map(original);
clone.get("list").push(3);
console.log(clone === original, original.get("list"));
```

```output for=merge
[ 'uno', 'dos', 'three' ]
false [ 1, 2, 3 ]
```

Counting with `get` and `set`.

```javascript run id=count
const counts = new Map();
for (const word of "the cat and the hat".split(" ")) {
  counts.set(word, (counts.get(word) ?? 0) + 1);
}
console.log(counts);
```

```output for=count
Map(4) { 'the' => 2, 'cat' => 1, 'and' => 1, 'hat' => 1 }
```

## Versions

ECMAScript 2026 adds `getOrInsert` and `getOrInsertComputed`: the TC39 list gives the Upsert proposal the expected publication year 2026.[^4] MDN describes `getOrInsert` as returning the value for the key, and "If the key is not present, it inserts a new entry with the key and a given default value, and returns the inserted value."[^1] Node v24.10.0, which runs the examples of this guide, does not have them, so the counting example uses `get` with `??` instead.

```javascript run id=upsert
console.log(typeof Map.prototype.getOrInsert, typeof Map.prototype.get);
```

```output for=upsert
undefined function
```

## Pitfalls

### Object keys with the same contents

A key object built again is a different key (rule r5). MDN's example: `myMap.get({})` gives `undefined`, "because keyObj !== {}".[^1]

```javascript wrong id=object-key
const visits = new Map();
visits.set({ x: 1, y: 2 }, "seen");
console.log(visits.get({ x: 1, y: 2 }), visits.size);

const byString = new Map();
byString.set(JSON.stringify({ x: 1, y: 2 }), "seen");
console.log(byString.get(JSON.stringify({ x: 1, y: 2 })));
```

```output for=object-key
undefined 1
seen
```

Keep a reference to the key object, or build a primitive key such as a string.

### `forEach` arguments in the wrong order

`forEach` calls the callback with the value first (rule r4).

```javascript wrong id=foreach
const m = new Map([["a", 1]]);
m.forEach((key, value) => console.log(key, value));
m.forEach((value, key) => console.log(key, value));
```

```output for=foreach
1 a
a 1
```

## Comparison

A Python dict keeps insertion order too: "Dictionaries preserve insertion order. Note that updating a key does not affect the order."[^5] In Java the order depends on the class. `LinkedHashMap` is the "Hash table and linked list implementation of the Map interface, with well-defined encounter order", and "This implementation differs from HashMap in that it maintains a doubly-linked list running through all of its entries."[^6] Go goes the other way: "The iteration order over maps is not specified and is not guaranteed to be the same from one iteration to the next."[^7] A JavaScript `Map` behaves like Python's dict and Java's `LinkedHashMap` here (rule r4). See [[map-and-dictionary]] and [[insertion-ordered-map]].

## References
