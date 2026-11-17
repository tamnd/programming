---
tag: 2EMKP4
kind: feature
owner: javascript
slug: iterator-helpers
name: Iterator helpers
aliases: [Iterator.prototype.map, Iterator.prototype.filter, Iterator.prototype.take, Iterator.prototype.drop, Iterator.prototype.toArray, Iterator.concat, lazy iterator methods]
area: languages/javascript/collections
level: core
summary: >-
  Iterator.prototype has map, filter, take, drop, flatMap, reduce, toArray, forEach, some, every and find. The first five are lazy: they return an iterator helper that does no work until asked.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/control-abstraction-objects.html#sec-properties-of-the-iterator-prototype-object", locator: "27.1.2 Iterator Helper Objects; 27.1.3.3 Properties of the Iterator Prototype Object"}
see_also: [javascript/iterators, javascript/iterators-and-generators, javascript/array-methods, lazy-sequence, take-the-first-n-items]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Iterator
  title: MDN Web Docs, JavaScript Reference, Iterator
  locator: Iterator helper methods; Iterator helper objects; Static methods; Instance methods
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/control-abstraction-objects.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 27
  locator: 27.1.2 Iterator Helper Objects; 27.1.3.2.1 Iterator.concat; 27.1.3.3 Properties of the Iterator Prototype Object
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Iterator/map
  title: MDN Web Docs, JavaScript Reference, Iterator.prototype.map()
  locator: Return value; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Iterator/take
  title: MDN Web Docs, JavaScript Reference, Iterator.prototype.take()
  locator: Return value; Exceptions; Combining drop() with take()
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Iterator/flatMap
  title: MDN Web Docs, JavaScript Reference, Iterator.prototype.flatMap()
  locator: Parameters; Exceptions
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Iterator/from
  title: MDN Web Docs, JavaScript Reference, Iterator.from()
  locator: Return value; Description
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Sync Iterator helpers; Iterator Sequencing, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/itertools.html
  title: The Python Standard Library, Python 3.14, itertools
  locator: the introduction; islice
  accessed: 2026-11-18
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html
  title: The Rust Standard Library, Trait Iterator
  locator: map
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/package-summary.html
  title: Java SE 25 API, Package java.util.stream
  locator: Stream operations and pipelines
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=chain
function* naturals() {
  let n = 1;
  while (true) yield n++;
}

const firstOddSquares = naturals()
  .filter((n) => n % 2 === 1)
  .map((n) => n * n)
  .take(4)
  .toArray();
console.log(firstOddSquares);

const prices = new Map([["tea", 3], ["cake", 5], ["jam", 4]]);
console.log(prices.values().reduce((a, b) => a + b), prices.keys().some((k) => k.length > 3));
console.log([10, 20, 30].values().drop(1).toArray());
```

```output for=chain
[ 1, 9, 25, 49 ]
12 true
[ 20, 30 ]
```

| Returns an iterator helper (lazy) | Consumes the iterator (eager) |
|---|---|
| `map(fn)`, `filter(fn)`, `flatMap(fn)` | `reduce(fn, init?)`, `toArray()`, `forEach(fn)` |
| `take(n)`, `drop(n)` | `some(fn)`, `every(fn)`, `find(fn)` |

## Semantics

**r1.** "The Iterator class itself provides some helper methods for working with iterators."[^1] In ECMAScript 2026 the prototype has `drop`, `every`, `filter`, `find`, `flatMap`, `forEach`, `map`, `reduce`, `some`, `take` and `toArray`, and the constructor has `Iterator.concat` and `Iterator.from`.[^2] They are "iterator helpers, not iterable helpers, because the only requirement for an object to be iterable is just the presence of a [Symbol.iterator]() method. There is no shared prototype to install these methods on."[^1] So an array needs `.values()` first, and a `Map` needs `.keys()`, `.values()` or `.entries()`.

**r2.** "Among the iterator helper methods, filter() , flatMap() , map() , drop() , and take() return a new Iterator Helper object."[^1] The specification: "An Iterator Helper object is an ordinary object that represents a lazy transformation of some specific source iterator object."[^2] MDN: "The main advantage of iterator helpers over array methods is that they are lazy, meaning that they only produce the next value when requested."[^3] Example: [`lazy`](#lazy).

**r3.** Laziness lets the helpers work on endless sequences: "Iterator helper methods are necessary to work with infinite iterators".[^1] `take` "yields the given number of elements in this iterator and then terminates", and it throws a RangeError "if limit becomes NaN or negative when converted to an integer."[^4] Example: [`chain`](#chain).

**r4.** The other methods walk the iterator. `reduce` on `map.values()` "may be more efficient, especially memory-wise, because it only iterates the iterator once, without memorizing any intermediate values", unlike spreading into an array first.[^1]

**r5.** The callbacks of `map`, `filter` and the others get the element and "The index of the current element being processed."[^3] `flatMap` needs an iterator or iterable from its callback: "unlike Array.prototype.flatMap() , you cannot return single non-iterator/iterable values."[^5] Example: [`flat`](#flat).

**r6.** `Iterator.from` turns a custom iterator into one with the helpers. "This method exists to convert custom iterators, probably exported by libraries, to proper iterators."[^6] A helper "shares the same data source as the underlying iterator, so iterating the iterator helper causes the underlying iterator to be iterated as well."[^1]

## Examples

Nothing runs until a value is asked for (rule r2).

```javascript run id=lazy
const log = [];
const helper = [1, 2, 3]
  .values()
  .map((x) => {
    log.push(x);
    return x * 10;
  });
console.log(log);
console.log(helper.next().value, log);
console.log(helper.toArray(), log);
```

```output for=lazy
[]
10 [ 1 ]
[ 20, 30 ] [ 1, 2, 3 ]
```

`flatMap` and the index argument (rule r5).

```javascript run id=flat
const words = ["ab", "cd"].values().flatMap((w) => w);
try {
  console.log(words.toArray());
} catch (e) {
  console.log(e.name);
}
console.log(["ab", "cd"].values().flatMap((w) => [...w]).toArray());
console.log(["x", "y"].values().map((v, i) => `${i}:${v}`).toArray());
```

```output for=flat
TypeError
[ 'a', 'b', 'c', 'd' ]
[ '0:x', '1:y' ]
```

A string primitive is not accepted from the `flatMap` callback, so spread it into an array.

## Versions

The TC39 list of finished proposals gives Sync Iterator helpers the expected publication year 2025, so the helper methods are part of ECMAScript 2025.[^7] It gives Iterator Sequencing, which adds `Iterator.concat`, the year 2026.[^7] Node v24.10.0 has the ECMAScript 2025 methods but not `Iterator.concat`.

```javascript run id=detect
console.log(typeof Iterator.prototype.map, typeof Iterator.from, typeof Iterator.concat);
```

```output for=detect
function function undefined
```

## Pitfalls

### Calling the array method in place of the helper

`[...].map` is `Array.prototype.map`, which builds a full array at once. On a large array, `.values()` gives the lazy version (rule r1).

```javascript wrong id=eager
let calls = 0;
const big = Array.from({ length: 1000 }, (_, i) => i);
big.map((x) => (calls++, x * 2)).slice(0, 3);
console.log(calls);

calls = 0;
big.values().map((x) => (calls++, x * 2)).take(3).toArray();
console.log(calls);
```

```output for=eager
1000
3
```

### Reusing a helper or its source

A helper and its source are one iterator underneath, and both are used up after one pass (rule r6).

```javascript wrong id=reuse
const source = [1, 2, 3, 4].values();
const evens = source.filter((n) => n % 2 === 0);
console.log(source.next().value, evens.toArray());
console.log(evens.toArray(), source.toArray());
```

```output for=reuse
1 [ 2, 4 ]
[] []
```

## Comparison

Python keeps the same tools in a module: itertools "implements a number of iterator building blocks inspired by constructs from APL, Haskell, and SML", and `islice` "Works like sequence slicing but does not support negative values for start, stop, or step."[^8] JavaScript puts them on `Iterator.prototype` as methods (rule r1). Rust's adapters are lazy too: "as map() is lazy, it is best used when you’re already working with other iterators."[^9] Java streams are built for the same idea: "Many stream operations, such as filtering, mapping, or duplicate removal, can be implemented lazily, exposing opportunities for optimization."[^10] See [[lazy-sequence]] and [[take-the-first-n-items]].

## References
