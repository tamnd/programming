---
tag: JQ3KDP
kind: feature
owner: javascript
slug: sets
name: Sets
aliases: [Set, new Set, Set.prototype.add, Set.prototype.has, unique values, remove duplicates]
area: languages/javascript/collections
level: intro
summary: >-
  A Set holds unique values of any type, compared with SameValueZero, in insertion order. add, has and delete work on values, and new Set(array) removes duplicates from an array.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/keyed-collections.html#sec-set-objects", locator: "24.2 Set Objects"}
see_also: [javascript/keyed-collections, javascript/maps, set-concept, remove-duplicates-from-a-list, do-set-operations]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Set
  title: MDN Web Docs, JavaScript Reference, Set
  locator: the introduction; Description; Set composition; Instance methods; Examples
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/keyed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 24
  locator: 24.2 Set Objects; 24.2.4.1 Set.prototype.add
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Keyed_collections
  title: MDN Web Docs, JavaScript Guide, Keyed collections
  locator: Set object; Array and Set compared
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: New Set methods, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/stdtypes.html#set-types-set-frozenset
  title: The Python Standard Library, Python 3.14, Built-in Types
  locator: Set Types, set, frozenset
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Set.html
  title: Java SE 25 API, Interface Set
  locator: the interface description
  accessed: 2026-11-18
- url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.collections/set-of.html
  title: Kotlin Standard Library API, setOf
  locator: setOf(vararg elements), samples
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const colors = new Set(["red", "green"]);
colors.add("blue").add("red");

console.log(colors.size, colors.has("red"), colors.has("pink"));
console.log(colors.delete("green"), colors.delete("green"));
console.log([...colors]);

const numbers = [2, 13, 4, 4, 2, 13, 5];
console.log([...new Set(numbers)]);
```

```output for=basics
3 true false
true false
[ 'red', 'blue' ]
[ 2, 13, 4, 5 ]
```

| Task | Code |
|---|---|
| make a set | `new Set()`, `new Set(iterable)` |
| add a value | `s.add(v)`, which returns `s` |
| test, remove | `s.has(v)`, `s.delete(v)` |
| count, empty | `s.size`, `s.clear()` |
| loop | `for (const v of s)` or `s.forEach((v) => ...)` |
| remove duplicates from an array | `[...new Set(array)]` |

## Semantics

**r1.** "The Set object lets you store unique values of any type, whether primitive values or object references."[^1] The specification: "A Set may contain each distinct value at most once. Distinct values are discriminated using the semantics of the SameValueZero comparison algorithm."[^2]

**r2.** `add` inserts a value "if it is not already present."[^1] In the specification it scans the set, returns the set at once if the value is there, and otherwise appends it and returns the set.[^2] So `add` chains, and adding a value twice changes nothing. Example: [`basics`](#basics).

**r3.** Iteration follows insertion order: "The insertion order corresponds to the order in which each element was inserted into the set by the add() method successfully".[^1] A value that is deleted and added again goes to the end: it "will be added as a new item, it will not retain its original position before deletion".[^1] Example: [`order`](#order).

**r4.** Equality is SameValueZero: "NaN is considered the same as NaN (even though NaN !== NaN ) and all other values are considered equal according to the semantics of the === operator." Objects "are compared by reference, not by value."[^1] Example: [`order`](#order).

**r5.** `has` is fast: "it is, on average, faster than the Array.prototype.includes method when an array has a length equal to a set's size ."[^1] The specification asks for "access times that are sublinear on the number of elements in the collection".[^2] The Guide adds that a Set can delete by value, and that "The value NaN cannot be found with indexOf in an array."[^3]

**r6.** The constructor takes any iterable and calls `add` for each value.[^2] Spread or `Array.from` turns a set back into an array, and "any duplicate elements from an Array are deleted when converting".[^3] `keys` is "An alias for Set.prototype.values() ", and `entries` gives `[value, value]` pairs "similar to the Map object".[^1]

**r7.** Sets have composition methods: `union`, `intersection`, `difference`, `symmetricDifference`, `isSubsetOf`, `isSupersetOf` and `isDisjointFrom`.[^1] Their argument can be any set-like object, one with "A size property that contains a number", "A has() method" and "A keys() method".[^1] Example: [`compose`](#compose).

## Examples

Insertion order and SameValueZero (rules r3 and r4).

```javascript run id=order
const s = new Set([1, 2, 3]);
s.delete(1);
s.add(1);
console.log([...s]);

const odd = new Set([NaN, NaN, 0, -0, { a: 1 }, { a: 1 }]);
console.log(odd.size, odd.has(NaN), odd.has(-0), odd.has({ a: 1 }));
```

```output for=order
[ 2, 3, 1 ]
4 true true false
```

The two `{ a: 1 }` objects are different values, and a third one built for `has` is not in the set.

Composition methods, with a `Map` as a set-like argument (rule r7).

```javascript run id=compose
const a = new Set([1, 2, 3]);
const b = new Set([2, 3, 4]);
console.log([...a.union(b)], [...a.intersection(b)], [...a.difference(b)]);
console.log(a.isSubsetOf(new Set([1, 2, 3, 4])), a.isDisjointFrom(new Set([9])));

const names = new Map([[1, "one"], [4, "four"]]);
console.log([...a.union(names)]);
```

```output for=compose
[ 1, 2, 3, 4 ] [ 2, 3 ] [ 1 ]
true true
[ 1, 2, 3, 4 ]
```

## Versions

The TC39 list of finished proposals gives New Set methods the expected publication year 2025, so `union`, `intersection` and the other composition methods are part of ECMAScript 2025.[^4]

## Pitfalls

### Comparing two sets with `===`

Two sets with the same values are two objects, so `===` is false.

```javascript wrong id=equal
const x = new Set([1, 2]);
const y = new Set([2, 1]);
console.log(x === y);
console.log(x.size === y.size && x.isSubsetOf(y));
```

```output for=equal
false
true
```

Compare the sizes and test one set against the other with `isSubsetOf` (rule r7).

### Removing duplicate objects

`new Set` removes values that are the same under SameValueZero, and two objects with the same contents are not (rule r4).

```javascript wrong id=objects
const rows = [{ id: 1 }, { id: 1 }, { id: 2 }];
console.log(new Set(rows).size);

const seen = new Set();
const unique = rows.filter((r) => !seen.has(r.id) && seen.add(r.id));
console.log(unique);
```

```output for=objects
3
[ { id: 1 }, { id: 2 } ]
```

Keep a set of a primitive key, such as `id`, and filter on it.

## Comparison

Python's set has no order: "A set object is an unordered collection of distinct hashable objects."[^5] Java's `Set` is "A collection that contains no duplicate elements", where duplicates are decided by `e1.equals(e2)`.[^6] Kotlin's `setOf` keeps the order of its arguments, "Elements of the set are iterated in the order they were specified", and its sample shows that sets with the same elements are equal "no matter of order".[^7] A JavaScript `Set` keeps order like Kotlin's, but compares objects by reference (rule r4), and `===` on two sets compares the two objects, as the [`equal`](#equal) example shows. See [[set-concept]] and [[remove-duplicates-from-a-list]].

## References
