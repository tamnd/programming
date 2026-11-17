---
tag: 47M4KN
kind: feature
owner: javascript
slug: key-and-value-equality-of-map-and-set
name: Key and value equality of Map and Set
aliases: [SameValueZero, Map key equality, Set value equality, NaN as a Map key, -0 as a Map key, object keys by reference]
area: languages/javascript/collections
level: core
summary: >-
  Map and Set compare keys with SameValueZero: like === but NaN equals NaN, and -0 is stored as +0. Objects match only themselves, so a key with the same contents is a different key.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-samevaluezero", locator: "7.2.10 SameValueZero"}
see_also: [javascript/maps, javascript/sets, javascript/keyed-collections, use-a-custom-key-in-a-map]
sources:
- url: https://tc39.es/ecma262/2026/multipage/abstract-operations.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 7
  locator: 7.2.9 SameValue; 7.2.10 SameValueZero
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Equality_comparisons_and_sameness
  title: MDN Web Docs, JavaScript Guide, Equality comparisons and sameness
  locator: the introduction; Same-value-zero equality
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/keyed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 24
  locator: 24.1 Map Objects; 24.2 Set Objects; 24.5.1 CanonicalizeKeyedCollectionKey
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Keyed_collections
  title: MDN Web Docs, JavaScript Guide, Keyed collections
  locator: Key and value equality of Map and Set
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Map/groupBy
  title: MDN Web Docs, JavaScript Reference, Map.groupBy()
  locator: Description
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/stdtypes.html#mapping-types-dict
  title: The Python Standard Library, Python 3.14, Built-in Types
  locator: Mapping Types, dict
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Map.html
  title: Java SE 25 API, Interface Map
  locator: the interface description
  accessed: 2026-11-18
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Map types
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=rules
const m = new Map([[NaN, "nan"], [0, "zero"], ["1", "string one"]]);
console.log(m.get(NaN), m.get(-0), m.get(1), m.get("1"));

const s = new Set(["a", "a", 1, 1n, { k: 1 }, { k: 1 }]);
console.log(s.size);

const keys = [...new Map([[-0, "x"]]).keys()];
console.log(keys, Object.is(keys[0], -0));
```

```output for=rules
nan zero undefined string one
5
[ 0 ] false
```

| Pair | `===` | `Object.is` (SameValue) | Map and Set (SameValueZero) |
|---|---|---|---|
| `NaN`, `NaN` | false | true | same key |
| `0`, `-0` | true | false | same key |
| `1`, `'1'` | false | false | different keys |
| `1`, `1n` | false | false | different keys |
| two objects with the same contents | false | false | different keys |

## Semantics

**r1.** SameValueZero "determines whether or not the two arguments are the same value (ignoring the difference between +0𝔽 and -0𝔽)."[^1] Its first step is "If SameType(x, y) is false, return false."[^1] MDN: "Same-value-zero only differs from strict equality by treating NaN as equivalent, and only differs from same-value equality by treating -0 as equivalent to 0 ."[^2]

**r2.** Map and Set use it. For Map, "Distinct key values are discriminated using the semantics of the SameValueZero comparison algorithm", and for Set, "Distinct values are discriminated using the semantics of the SameValueZero comparison algorithm."[^3] The Guide sums it up: "-0 and +0 are considered equal. NaN is considered equal to itself (contrary to === )."[^4] Example: [`rules`](#rules).

**r3.** A key of -0 is stored as +0. `set`, `get`, `has`, `delete` and `add` first call `CanonicalizeKeyedCollectionKey`, whose step is "If key is -0𝔽, return +0𝔽."[^3] So `keys()` gives back 0, not -0, as the [`rules`](#rules) example shows with `Object.is`.

**r4.** Objects are compared by identity. None of the equality operations "compares whether the parameters are conceptually similar in structure. For any non-primitive objects x and y which have the same structure but are distinct objects themselves, all of the above forms will evaluate to false ."[^2] Primitives compare by value, so two strings with the same characters are one key. Example: [`identity`](#identity).

**r5.** The same comparison is used elsewhere: "It's used by Array.prototype.includes() , TypedArray.prototype.includes() , as well as Map and Set methods for comparing key equality."[^2] Strict equality "is also used by array index-finding methods including Array.prototype.indexOf()", so "you cannot use indexOf(NaN) to find the index of a NaN value in an array".[^2] Example: [`includes`](#includes).

**r6.** To use a value with structure as a key, a program keeps the object itself, or derives a primitive key from it. MDN names the cost of the second way: "you must maintain the mapping between the original object and its representation as the object changes."[^5] Example: [`compound`](#compound).

## Examples

One object is one key, wherever it is used (rule r4).

```javascript run id=identity
const point = { x: 1, y: 2 };
const labels = new Map([[point, "A"]]);
point.x = 10;
console.log(labels.get(point), labels.get({ x: 10, y: 2 }));

const first = "ab";
const second = "a" + "b";
console.log(new Set([first, second]).size);
```

```output for=identity
A undefined
1
```

Changing `point` does not move its entry: the map holds the object, not its contents.

`includes` and `indexOf` on the same array (rule r5).

```javascript run id=includes
const values = [1, NaN, -0];
console.log(values.includes(NaN), values.indexOf(NaN));
console.log(new Set(values).has(NaN), values.includes(0));
```

```output for=includes
true -1
true true
```

A string key for a pair of numbers, and a map of maps (rule r6).

```javascript run id=compound
const grid = new Map();
grid.set(`${3},${4}`, "tree");
console.log(grid.get(`${3},${4}`), grid.get([3, 4]));

const nested = new Map();
const row = nested.get(3) ?? new Map();
nested.set(3, row.set(4, "tree"));
console.log(nested.get(3)?.get(4), nested.get(5)?.get(4));
```

```output for=compound
tree undefined
tree undefined
```

## Pitfalls

### An array as a key

An array literal is a new object each time, so it never finds an entry made with another array (rule r4).

```javascript wrong id=array-key
const seen = new Set();
seen.add([1, 2]);
console.log(seen.has([1, 2]), seen.size);
seen.add([1, 2]);
console.log(seen.size);
```

```output for=array-key
false 1
2
```

Use a string key such as `'1,2'`, or keep the array in a variable and pass the same array each time.

### Numbers and numeric strings as keys

An object turns the key 1 into `'1'`, and a `Map` does not (rule r1). Code that moves from objects to maps can miss entries.

```javascript wrong id=number-string
const obj = {};
obj[1] = "one";
console.log(obj["1"]);

const map = new Map();
map.set(1, "one");
console.log(map.get("1"), map.get(Number("1")));
```

```output for=number-string
one
undefined one
```

## Comparison

Python compares dict keys by value, through hashing: "Values that compare equal (such as 1 , 1.0 , and True ) can be used interchangeably to index the same dictionary entry."[^6] In JavaScript, 1 and `'1'` or 1 and `1n` are different keys (rule r1). Java maps use `equals`, so a class decides what an equal key is: `containsKey` is specified as "returns true if and only if this map contains a mapping for a key k such that (key==null ? k==null : key.equals(k)) ."[^7] Go requires that "The comparison operators == and != must be fully defined for operands of the key type".[^8] A JavaScript `Map` has no hook like `equals`: SameValueZero is fixed (rule r2). See [[use-a-custom-key-in-a-map]].

## References
