---
tag: TBTTXP
kind: feature
owner: javascript
slug: grouping-data
name: Grouping data
aliases: [Object.groupBy, Map.groupBy, group by, groupBy, Array.prototype.group, group an array]
area: languages/javascript/collections
level: core
summary: >-
  Object.groupBy and Map.groupBy (ECMAScript 2024) call a function on each element of an iterable and collect the elements into one array per key, in an object or in a Map.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/fundamental-objects.html#sec-object.groupby", locator: "20.1.2.13 Object.groupBy"}
see_also: [group-items-by-a-key, reduce-a-list-to-a-value, count-the-items-of-each-kind, javascript/array-methods]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/groupBy
  title: MDN Web Docs, JavaScript Reference, Object.groupBy()
  locator: the introduction; Parameters; Return value; Description
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/fundamental-objects.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 20
  locator: 20.1.2.13 Object.groupBy, Note
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/keyed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 24
  locator: 24.1.2.1 Map.groupBy, Note; 24.5.1 CanonicalizeKeyedCollectionKey
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/abstract-operations.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 7
  locator: 7.3.34 AddValueToKeyedGroup; 7.3.35 GroupBy
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Map/groupBy
  title: MDN Web Docs, JavaScript Reference, Map.groupBy()
  locator: Description; Examples
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/reduce
  title: MDN Web Docs, JavaScript Reference, Array.prototype.reduce()
  locator: When to not use reduce(), Grouping objects by a property
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Array Grouping, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/itertools.html#itertools.groupby
  title: The Python Standard Library, Python 3.14, itertools
  locator: itertools.groupby
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Collectors.html
  title: Java SE 25 API, Class Collectors
  locator: groupingBy(Function)
  accessed: 2026-11-18
- url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.collections/group-by.html
  title: Kotlin Standard Library API, groupBy
  locator: the description
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const inventory = [
  { name: "asparagus", type: "vegetables", quantity: 5 },
  { name: "bananas", type: "fruit", quantity: 0 },
  { name: "goat", type: "meat", quantity: 23 },
  { name: "cherries", type: "fruit", quantity: 5 },
];

const byType = Object.groupBy(inventory, ({ type }) => type);
console.log(Object.keys(byType));
console.log(byType.fruit.map((f) => f.name));

const restock = { restock: true };
const enough = { restock: false };
const byStock = Map.groupBy(inventory, ({ quantity }) => (quantity < 6 ? restock : enough));
console.log(byStock.get(restock).length, byStock.get(enough).length);
```

```output for=basics
[ 'vegetables', 'fruit', 'meat' ]
[ 'bananas', 'cherries' ]
3 1
```

| Call | Keys | Result |
|---|---|---|
| `Object.groupBy(items, fn)` | coerced to strings or symbols | an object with no prototype, one array per key |
| `Map.groupBy(items, fn)` | any value, kept as it is | a `Map`, one array per key |

The callback gets the element and its index, `fn(element, index)`.

## Semantics

**r1.** `Object.groupBy` "groups the elements of a given iterable according to the string values returned by a provided callback function."[^1] It "calls callback once for each element in items, in ascending order", with "the value of the element and the index of the element."[^2] Each group is an array of the elements for which the callback returned that key. Example: [`basics`](#basics).

**r2.** The keys of `Object.groupBy` are property keys: "Each value returned by callback is coerced to a property key."[^2] A number such as 1 becomes the key `'1'`. The result "is an object that does not inherit from %Object.prototype%".[^2] MDN calls it "A null -prototype object".[^1] Example: [`keys`](#keys).

**r3.** The keys of `Map.groupBy` are not coerced: "Each value returned by callback is used as a key in the Map."[^3] `GroupBy` first changes -0 to +0 with `CanonicalizeKeyedCollectionKey`, and `AddValueToKeyedGroup` puts a value in the group whose key passes `SameValue`.[^3][^4] So an object key matches only the same object: "You can't use another object that just happens to have the same name and properties."[^5] Example: [`keys`](#keys).

**r4.** The input is any iterable, since `GroupBy` begins with `GetIterator(items, sync)`.[^4] MDN lists it as "An iterable (such as an Array )".[^1] A string or a `Set` works too. Example: [`iterables`](#iterables).

**r5.** The groups hold the elements themselves: "The elements in the returned object and the original iterable are the same (not deep copies)."[^1] A change to an element shows through the original array and through the groups. Example: [`same`](#same).

**r6.** Before these methods, grouping was written with `reduce`. MDN gives that pattern under "Grouping objects by a property. Use Object.groupBy() instead."[^6] Example: [`reduce`](#reduce).

## Examples

String keys from `Object.groupBy`, and kept keys from `Map.groupBy` (rules r2 and r3).

```javascript run id=keys
const byTens = Object.groupBy([3, 14, 15, 27], (n) => Math.floor(n / 10));
console.log(Object.keys(byTens), byTens[1]);

const byLength = Map.groupBy(["a", "bb", "cc"], (s) => s.length);
console.log([...byLength.keys()], byLength.get(2), byLength.get("2"));
```

```output for=keys
[ '0', '1', '2' ] [ 14, 15 ]
[ 1, 2 ] [ 'bb', 'cc' ] undefined
```

`byTens[1]` works because property access turns 1 into `'1'` as well. The `Map` has the number 2 as a key, so the string `'2'` does not find it.

Grouping a string by index, and a `Set` by value (rule r4).

```javascript run id=iterables
const halves = Object.groupBy("abcdef", (_, i) => (i < 3 ? "first" : "rest"));
console.log(halves.first.join(""), halves.rest.join(""));

const parity = Map.groupBy(new Set([1, 2, 3, 4, 5]), (n) => n % 2 === 0);
console.log(parity.get(true), parity.get(false));
```

```output for=iterables
abc def
[ 2, 4 ] [ 1, 3, 5 ]
```

The groups share the elements (rule r5).

```javascript run id=same
const items = [{ n: 1 }, { n: 2 }];
const groups = Object.groupBy(items, () => "all");
groups.all[0].n = 99;
console.log(items[0].n, groups.all[0] === items[0]);
```

```output for=same
99 true
```

The `reduce` version and the `Object.groupBy` version (rule r6).

```javascript run id=reduce
const people = [
  { name: "Ana", city: "Lima" },
  { name: "Bo", city: "Oslo" },
  { name: "Cy", city: "Lima" },
];
const viaReduce = people.reduce((acc, p) => {
  const group = acc[p.city] ?? [];
  return { ...acc, [p.city]: [...group, p.name] };
}, {});
console.log(viaReduce);

const viaGroupBy = Object.groupBy(people, (p) => p.city);
console.log(viaGroupBy.Lima.map((p) => p.name));
```

```output for=reduce
{ Lima: [ 'Ana', 'Cy' ], Oslo: [ 'Bo' ] }
[ 'Ana', 'Cy' ]
```

## Versions

The TC39 list of finished proposals gives Array Grouping the expected publication year 2024, so `Object.groupBy` and `Map.groupBy` are part of ECMAScript 2024.[^7] MDN notes an earlier form: "In some versions of some browsers, this method was implemented as the method Array.prototype.group() . Due to web compatibility issues, it is now implemented as a static method."[^1]

## Pitfalls

### Calling an `Object.prototype` method on the result

The result has no prototype (rule r2), so it has no `hasOwnProperty`.

```javascript wrong id=no-proto
const groups = Object.groupBy(["x", "yy", "zz"], (s) => s.length);
try {
  groups.hasOwnProperty("1");
} catch (e) {
  console.log(e.name);
}
console.log(Object.hasOwn(groups, "1"), "3" in groups, groups[3]?.length ?? 0);
```

```output for=no-proto
TypeError
true false 0
```

Use `Object.hasOwn` or the `in` operator. A group that no element fell into is missing, not empty, so read it with `?? []` or `?.`.

### An object key built again

Each object literal is a different key in `Map.groupBy` (rule r3).

```javascript wrong id=new-key
const points = [{ x: 0 }, { x: 5 }, { x: 0 }];
const byPlace = Map.groupBy(points, (p) => ({ origin: p.x === 0 }));
console.log(byPlace.size, byPlace.get({ origin: true }));

const byLabel = Map.groupBy(points, (p) => (p.x === 0 ? "origin" : "away"));
console.log(byLabel.size, byLabel.get("origin"));
```

```output for=new-key
3 undefined
2 [ { x: 0 }, { x: 0 } ]
```

Return a primitive key, or return objects from a fixed set and keep references to them, as the [`basics`](#basics) example does with `restock` and `enough`.

## Comparison

Python's `itertools.groupby` groups runs, not the whole input: it makes "an iterator that returns consecutive keys and groups from the iterable", and "Generally, the iterable needs to already be sorted on the same key function."[^8] Java's `Collectors.groupingBy` returns "a Collector implementing a 'group by' operation on input elements of type T , grouping elements according to a classification function, and returning the results in a Map".[^9] Kotlin's `groupBy` "returns a map where each group key is associated with a list of corresponding elements."[^10] `Map.groupBy` is the closest of these to Java and Kotlin, and `Object.groupBy` is the form for string keys. See [[group-items-by-a-key]].

## References
