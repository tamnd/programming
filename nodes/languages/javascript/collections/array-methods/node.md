---
tag: C1GVBC
kind: feature
owner: javascript
slug: array-methods
name: Array methods
aliases: [map, filter, reduce, find, some, every, flat, flatMap, at, includes, Array.prototype methods, iterative methods]
area: languages/javascript/collections
level: core
summary: >-
  The methods of Array.prototype transform, search and flatten arrays. map, filter, flat and flatMap return a new array; reduce folds the elements into one value; some methods change the array itself.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/indexed-collections.html#sec-properties-of-the-array-prototype-object", locator: "23.1.3 Properties of the Array Prototype Object"}
see_also: [javascript/arrays, javascript/creating-arrays, collection-pipeline, in-place-and-copying-operations, flatten-a-nested-list]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array
  title: MDN Web Docs, JavaScript Reference, Array
  locator: Copying methods and mutating methods; Iterative methods
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/map
  title: MDN Web Docs, JavaScript Reference, Array.prototype.map()
  locator: Parameters; Using parseInt() with map()
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/filter
  title: MDN Web Docs, JavaScript Reference, Array.prototype.filter()
  locator: the introduction; Parameters
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/reduce
  title: MDN Web Docs, JavaScript Reference, Array.prototype.reduce()
  locator: Parameters; Exceptions; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/find
  title: MDN Web Docs, JavaScript Reference, Array.prototype.find()
  locator: the introduction; Return value
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/indexed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition)
  locator: 23.1.3.6 Array.prototype.every, Note 1; 23.1.3.16 Array.prototype.includes, Note 3
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/includes
  title: MDN Web Docs, JavaScript Reference, Array.prototype.includes()
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/indexOf
  title: MDN Web Docs, JavaScript Reference, Array.prototype.indexOf()
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/at
  title: MDN Web Docs, JavaScript Reference, Array.prototype.at()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/flat
  title: MDN Web Docs, JavaScript Reference, Array.prototype.flat()
  locator: Parameters; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/flatMap
  title: MDN Web Docs, JavaScript Reference, Array.prototype.flatMap()
  locator: Description
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/functions.html
  title: The Python Standard Library, Python 3.14, Built-in Functions
  locator: map(); filter()
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Stream.html
  title: Java SE 25 API, Interface Stream
  locator: the class description
  accessed: 2026-11-18
- url: https://pkg.go.dev/slices
  title: Go standard library, package slices
  locator: func Index; func ContainsFunc
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=pipeline
const orders = [
  { item: "tea", qty: 2, price: 3 },
  { item: "cake", qty: 1, price: 5 },
  { item: "jam", qty: 0, price: 4 },
];

const lines = orders.filter((o) => o.qty > 0).map((o) => `${o.item} x${o.qty}`);
const total = orders.reduce((sum, o) => sum + o.qty * o.price, 0);
console.log(lines, total);
console.log(orders.find((o) => o.price > 3).item, orders.some((o) => o.qty === 0));
```

```output for=pipeline
[ 'tea x2', 'cake x1' ] 11
cake true
```

| Method | Returns | Stops early |
|---|---|---|
| `map(fn)` | a new array of the results of `fn` | no |
| `filter(fn)` | a new array of the elements where `fn` is truthy | no |
| `reduce(fn, init)` | one value, folded from left to right | no |
| `find(fn)`, `findIndex(fn)` | the first match, or its index | yes |
| `some(fn)`, `every(fn)` | a boolean | yes |
| `flat(depth)` | a new array with nested arrays spread out | no |
| `flatMap(fn)` | `map(fn)` followed by `flat(1)` | no |
| `at(i)` | the element at `i`; a negative `i` counts from the end | no callback |
| `includes(v)`, `indexOf(v)` | a boolean, or the index or -1 | no callback |

## Semantics

**r1.** `map`, `filter`, `reduce`, `find`, `some`, `every` and `flatMap` are iterative methods. Their callback "is called sequentially and at most once for each element in the array".[^1] It gets three arguments: the element, the index and the array.[^2] The methods do not change the array, "but the function provided as callbackFn can."[^1]

**r2.** `map` returns "a new array populated with the results of calling a provided function on every element".[^2] The callback of `filter` "should return a truthy value to keep the element in the resulting array, and a falsy value otherwise."[^3] Both copy shallowly, as all copying methods do: "The copy always happens shallowly".[^1] Example: [`pipeline`](#pipeline).

**r3.** `reduce` runs a callback over all elements "in ascending-index order, and accumulates them into a single value."[^4] With an initial value, the first call gets that value and the first element. Without one, "accumulator is initialized to the first value in the array", and on an empty array the call throws a `TypeError`.[^4] Example: [`empty-reduce`](#empty-reduce).

**r4.** `find` "returns the first element in the provided array that satisfies the provided testing function", or `undefined`.[^5] `some` and `every` stop at the first element that settles the answer. Of `every`, the specification says: "If such an element is found, every immediately returns false."[^6] Example: [`search`](#search).

**r5.** `includes` compares with SameValueZero, which "works like strict equality === (where -0 and +0 are considered equal), with the exception that NaN is considered equal to itself."[^7] `indexOf` uses strict equality, so "indexOf() always returns -1 when searchElement is NaN".[^8] The specification gives a second difference: `includes` "does not skip missing array elements, instead treating them as undefined."[^6] Example: [`search`](#search).

**r6.** `at` "takes an integer value and returns the item at that index, allowing for positive and negative integers. Negative integers count back from the last item in the array."[^9] An index out of range gives `undefined`.[^9]

**r7.** `flat(depth)` concatenates sub-arrays into a new array, "recursively up to the specified depth", and the depth "Defaults to 1."[^10] It also "removes empty slots if the array being flattened is sparse."[^10] `flatMap(fn)` "is identical to map(callbackFn, thisArg) followed by flat(1)".[^11] Example: [`flatten`](#flatten).

**r8.** Some other methods change the array: `push`, `pop`, `shift`, `unshift`, `splice`, `sort`, `reverse`, `fill` and `copyWithin`.[^1] The page [[javascript/copying-array-methods]] covers the copying versions of `sort`, `reverse` and `splice`. Example: [`mutate`](#mutate).

## Examples

Searching (rules r4, r5 and r6).

```javascript run id=search
const temps = [12, 17, NaN, 21, 9];
console.log(temps.find((t) => t > 15), temps.findIndex((t) => t > 15));
console.log(temps.every((t) => t > 10), temps.some((t) => t > 20));
console.log(temps.includes(NaN), temps.indexOf(NaN));
console.log(temps.at(0), temps.at(-1), temps.at(10));
```

```output for=search
17 1
false true
true -1
12 9 undefined
```

Flattening (rule r7). `flatMap` can return an empty array to drop an element.

```javascript run id=flatten
const nested = [1, [2, [3, [4]]]];
console.log(nested.flat(), nested.flat(Infinity));

const words = ["a cup", "", "of tea"];
console.log(words.flatMap((s) => (s ? s.split(" ") : [])));
```

```output for=flatten
[ 1, 2, [ 3, [ 4 ] ] ] [ 1, 2, 3, 4 ]
[ 'a', 'cup', 'of', 'tea' ]
```

A mutating method and a copying method side by side (rule r8).

```javascript run id=mutate
const queue = ["a", "b", "c"];
const tail = queue.slice(1);
const removed = queue.splice(0, 1);
console.log(tail, removed, queue);
```

```output for=mutate
[ 'b', 'c' ] [ 'a' ] [ 'b', 'c' ]
```

`slice` leaves `queue` as it is. `splice` removes the element from `queue` and returns the removed elements.

## Pitfalls

### `reduce` on an empty array

Without an initial value, `reduce` needs a first element (rule r3).

```javascript wrong id=empty-reduce
const prices = [];
try {
  console.log(prices.reduce((a, b) => a + b));
} catch (e) {
  console.log(e.name);
}
console.log(prices.reduce((a, b) => a + b, 0));
```

```output for=empty-reduce
TypeError
0
```

Pass the initial value, here `0`, whenever the array can be empty.

### A callback that takes more than one argument

`map` passes the index as the second argument (rule r1). MDN shows the effect with `parseInt`, whose second argument is the radix: for `['1', '2', '3'].map(parseInt)`, "the actual result is [1, NaN, NaN]".[^2]

```javascript wrong id=parse
console.log(["1", "2", "3"].map(parseInt));
console.log(["1", "2", "3"].map((s) => parseInt(s, 10)));
```

```output for=parse
[ 1, NaN, NaN ]
[ 1, 2, 3 ]
```

Wrap the function in an arrow function that passes only the arguments it should get.

## Comparison

Python's `map` and `filter` return iterators, not lists: `map` returns "an iterator that applies function to every item of iterable, yielding the results."[^12] Java has these operations on streams: "stream operations are composed into a stream pipeline", and "Streams are lazy".[^13] A JavaScript `map` or `filter` returns a new array.[^2][^3] Go's `slices` package has search functions: `Index` "returns the index of the first occurrence of v in s, or -1 if not present", and `ContainsFunc` "stops as soon as a call to f returns true."[^14] See [[collection-pipeline]].

## References
