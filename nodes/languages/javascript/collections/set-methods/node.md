---
tag: MKNAVC
kind: feature
owner: javascript
slug: set-methods
name: Set methods
aliases: [Set.prototype.union, Set.prototype.intersection, Set.prototype.difference, symmetricDifference, isSubsetOf, isSupersetOf, isDisjointFrom, set-like object]
area: languages/javascript/collections
level: core
summary: >-
  ECMAScript 2025 gives Set seven composition methods: union, intersection, difference, symmetricDifference and three tests. Each returns a new Set or a boolean and accepts any set-like argument.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/keyed-collections.html#sec-properties-of-the-set-prototype-object", locator: "24.2.4 Properties of the Set Prototype Object"}
see_also: [javascript/sets, javascript/keyed-collections, do-set-operations, set-concept]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Set
  title: MDN Web Docs, JavaScript Reference, Set
  locator: Set composition; Set-like objects
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Set/union
  title: MDN Web Docs, JavaScript Reference, Set.prototype.union()
  locator: Description; Examples
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/keyed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 24
  locator: 24.2.1.2 GetSetRecord; 24.2.4 Properties of the Set Prototype Object
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Set/symmetricDifference
  title: MDN Web Docs, JavaScript Reference, Set.prototype.symmetricDifference()
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Set/intersection
  title: MDN Web Docs, JavaScript Reference, Set.prototype.intersection()
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Set/isSubsetOf
  title: MDN Web Docs, JavaScript Reference, Set.prototype.isSubsetOf()
  locator: Description
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
  locator: addAll; retainAll
  accessed: 2026-11-18
- url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.collections/union.html
  title: Kotlin Standard Library API, union
  locator: the description
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=all
const evens = new Set([2, 4, 6, 8]);
const squares = new Set([1, 4, 9]);

console.log(evens.union(squares));
console.log(evens.intersection(squares), evens.difference(squares));
console.log(evens.symmetricDifference(squares));

const small = new Set([4]);
console.log(small.isSubsetOf(evens), evens.isSupersetOf(small), evens.isDisjointFrom(new Set([3])));
```

```output for=all
Set(6) { 2, 4, 6, 8, 1, 9 }
Set(1) { 4 } Set(3) { 2, 6, 8 }
Set(5) { 2, 6, 8, 1, 9 }
true true true
```

| Method | Returns | In set notation |
|---|---|---|
| `a.union(b)` | a new Set | A ∪ B |
| `a.intersection(b)` | a new Set | A ∩ B |
| `a.difference(b)` | a new Set | A ∖ B |
| `a.symmetricDifference(b)` | a new Set | (A ∖ B) ∪ (B ∖ A) |
| `a.isSubsetOf(b)` | a boolean | A ⊆ B |
| `a.isSupersetOf(b)` | a boolean | A ⊇ B |
| `a.isDisjointFrom(b)` | a boolean | A ∩ B = ∅ |

## Semantics

**r1.** "The Set object provides some methods that allow you to compose sets like you would with mathematical operations."[^1] Four of them return a set, and three return a boolean.[^1] The set is a fresh one: `union` returns "A new Set object containing elements which are in either or both of this set and the other set."[^2] Example: [`all`](#all).

**r2.** The argument can be any set-like object. "All set composition methods require this to be an actual Set instance, but their arguments just need to be set-like": an object with "A size property that contains a number", "A has() method that takes an element and returns a boolean" and "A keys() method that returns an iterator of the elements in the set."[^1] The specification reads these with `GetSetRecord`, which throws a TypeError if `size` converts to NaN or if `has` or `keys` is not callable, and a RangeError if the size is negative.[^3] Example: [`set-like`](#set-like).

**r3.** The methods read the argument through `keys`, not through `Symbol.iterator`. MDN gives the reason: "This is to make maps valid set-like objects, because for maps, the iterator produces entries but the has() method takes keys."[^1] So a `Map` works as the set of its keys. Example: [`set-like`](#set-like).

**r4.** `union` and `symmetricDifference` keep a fixed order: "The order of elements in the returned set is first those in this followed by those in other ."[^2][^4] `intersection` does not. Its work "depends on the sizes of this and other", and "The order of elements in the returned set is the same as that of the smaller of this and other ."[^5] Example: [`order`](#order).

**r5.** The subset test is not a proper subset test: "isSubsetOf() returns true if this and other contain the same elements."[^6] If `this` has more elements than `other.size`, `isSubsetOf` "directly returns false" without looking at the elements.[^6]

## Examples

A `Map` and a custom object as the argument (rules r2 and r3).

```javascript run id=set-like
const ids = new Set([1, 2, 3]);
const names = new Map([[1, "one"], [4, "four"]]);
console.log([...ids.union(names)], [...ids.intersection(names)]);

const evensBelow10 = {
  size: 5,
  has: (n) => n % 2 === 0 && n >= 0 && n < 10,
  keys: () => [0, 2, 4, 6, 8][Symbol.iterator](),
};
console.log([...ids.difference(evensBelow10)], ids.isDisjointFrom(evensBelow10));
```

```output for=set-like
[ 1, 2, 3, 4 ] [ 1 ]
[ 1, 3 ] false
```

The order of an intersection depends on which set is smaller (rule r4).

```javascript run id=order
const other = new Set([3, 2, 1, 0]);
const three = new Set([1, 2, 3]);
const five = new Set([1, 2, 3, 4, 5]);
console.log([...three.intersection(other)], [...five.intersection(other)]);
console.log([...three.union(other)]);
```

```output for=order
[ 1, 2, 3 ] [ 3, 2, 1 ]
[ 1, 2, 3, 0 ]
```

`three` is smaller than `other`, so its intersection follows the order of `three`. `five` is larger, so its intersection walks `other` and follows the order of `other`. `union` always puts the elements of `this` first.

## Versions

The TC39 list of finished proposals gives New Set methods the expected publication year 2025, so the seven methods are part of ECMAScript 2025.[^7] Node v24.10.0 runs every example on this page.

## Pitfalls

### An array as the argument

"Arrays are not set-like because they don't have a has() method or the size property".[^1] `GetSetRecord` throws before any work is done (rule r2).

```javascript wrong id=array-arg
const tags = new Set(["js", "css"]);
try {
  tags.union(["html"]);
} catch (e) {
  console.log(e.name);
}
console.log(tags.union(new Set(["html"])));
```

```output for=array-arg
TypeError
Set(3) { 'js', 'css', 'html' }
```

Wrap the array in `new Set(...)` first.

### A set-like object whose `size` is wrong

The methods trust `size` to pick a strategy (rules r4 and r5), so an object with a wrong `size` gives a wrong answer.

```javascript wrong id=bad-size
const a = new Set([1, 2]);
const liar = { size: 1, has: () => true, keys: () => [1, 2][Symbol.iterator]() };
console.log(a.isSubsetOf(liar));

const honest = { size: 2, has: () => true, keys: () => [1, 2][Symbol.iterator]() };
console.log(a.isSubsetOf(honest));
```

```output for=bad-size
false
true
```

`a` has 2 elements and `liar.size` is 1, so `isSubsetOf` returns false without calling `has`.

## Comparison

Python has the same operations as methods and as operators, and "the non-operator versions of union() , intersection() , difference() , symmetric_difference() , issubset() , and issuperset() methods will accept any iterable as an argument."[^8] The JavaScript methods accept set-like objects, not any iterable (rule r2). Java changes the set in place: "If the specified collection is also a set, the addAll operation effectively modifies this set so that its value is the union of the two sets", and `retainAll` "Retains only the elements in this set that are contained in the specified collection".[^9] Kotlin's `union` "Returns a set containing all distinct elements from both collections."[^10] See [[do-set-operations]].

## References
