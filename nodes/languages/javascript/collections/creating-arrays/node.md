---
tag: 6CP447
kind: feature
owner: javascript
slug: creating-arrays
name: Creating arrays
aliases: [Array.of, Array.from, Array constructor, Array.prototype.fill, array literal, create an array]
area: languages/javascript/collections
level: intro
summary: >-
  JavaScript makes an array with a literal, Array.of, Array.from or the Array constructor. Array.from can map each value, Array(n) makes n empty slots, and fill sets them all to one value.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/indexed-collections.html#sec-properties-of-the-array-constructor", locator: "23.1.2 Properties of the Array Constructor"}
see_also: [javascript/arrays, javascript/indexed-collections, javascript/sparse-arrays, array, dynamic-array]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/of
  title: MDN Web Docs, JavaScript Reference, Array.of()
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/Array
  title: MDN Web Docs, JavaScript Reference, Array() constructor
  locator: Parameters; Exceptions; Examples
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/from
  title: MDN Web Docs, JavaScript Reference, Array.from()
  locator: Parameters; Description; Examples
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/fill
  title: MDN Web Docs, JavaScript Reference, Array.prototype.fill()
  locator: Parameters; Description; Examples
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array
  title: MDN Web Docs, JavaScript Reference, Array
  locator: Array methods and empty slots; Copy an array
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/indexed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition)
  locator: 23.1.2.1 Array.from; 23.1.2.2 Array.fromAsync; 23.1.2.4 Array.of; 23.1.3.7 Array.prototype.fill
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/tutorial/datastructures.html#list-comprehensions
  title: The Python Tutorial, Python 3.14, Data Structures
  locator: 5.1.3 List Comprehensions
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/stdtypes.html#common-sequence-operations
  title: The Python Standard Library, Python 3.14, Built-in Types
  locator: Common Sequence Operations, note 2
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-10.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 10 Arrays
  locator: 10.3 Array Creation; 10.6 Array Initializers
  accessed: 2026-11-18
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Slice types
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=forms
console.log(["a", "b"], Array.of(7), Array(7));
console.log(Array.from("foo"), Array.from(new Set([1, 1, 2])));
console.log(Array.from({ length: 5 }, (_, i) => i * i));
console.log(Array(3).fill(0), [..."hi"]);
```

```output for=forms
[ 'a', 'b' ] [ 7 ] [ <7 empty items> ]
[ 'f', 'o', 'o' ] [ 1, 2 ]
[ 0, 1, 4, 9, 16 ]
[ 0, 0, 0 ] [ 'h', 'i' ]
```

| Form | Result |
|---|---|
| `[a, b]` | an array of the listed values |
| `Array.of(a, b)` | an array of the arguments, even for one number |
| `Array(a, b)` or `new Array(a, b)` | an array of the arguments |
| `Array(n)` with one number | `n` empty slots, no elements |
| `Array.from(items)` | a shallow copy of an iterable or an array-like object |
| `Array.from(items, mapFn)` | the same, with `mapFn` applied to each value |
| `Array(n).fill(v)` | `n` elements that are all `v` |
| `[...items]` | a shallow copy of an iterable |

## Semantics

**r1.** `Array.of` "creates a new Array instance from a variable number of arguments, regardless of number or type of the arguments."[^1] It differs from the `Array()` constructor only for a single argument: "Array.of(7) creates an array with a single element, 7 , whereas Array(7) creates an empty array with a length property of 7".[^1] Example: [`forms`](#forms).

**r2.** `Array(n)` and `new Array(n)` do the same thing.[^2] When `n` is the only argument and "is an integer between 0 and 232 - 1 (inclusive)", the result has length `n` and its slots are empty, "not slots with actual undefined values".[^2] Any other single number throws a `RangeError`.[^2] A single argument that is not a number, such as `'2'`, becomes the one element.[^2] Example: [`holes`](#holes).

**r3.** `Array.from` "creates a new, shallow-copied Array instance from an iterable or array-like object."[^3] It takes an iterable, such as a string, a `Set` or a `Map`, and otherwise an object with a `length` property and indexed elements.[^3] "Array.from() never creates a sparse array. If the items object is missing some index properties, they become undefined in the new array."[^3] Example: [`from`](#from).

**r4.** The second argument of `Array.from` is a map function. It gets the value and the index, and its return value goes into the array.[^3] `Array.from(obj, mapFn)` "has the same result as Array.from(obj).map(mapFn, thisArg) , except that it does not create an intermediate array".[^3] With `{ length: n }` as the first argument it builds an array of `n` computed values. Example: [`from`](#from).

**r5.** `fill(value, start, end)` "changes all elements within a range of indices in an array to a static value. It returns the modified array."[^4] It "fills empty slots in sparse arrays with value as well", so `Array(n).fill(v)` gives `n` elements.[^4] Every slot gets the same value: "if value is an object, each slot in the array will reference that object."[^4] Example: [`shared`](#shared).

**r6.** Spread syntax in a literal, `[...items]`, copies an iterable into a new array, as `Array.from` and `slice` do. "All built-in array-copy operations (spread syntax, Array.from() , Array.prototype.slice() , and Array.prototype.concat() ) create shallow copies."[^5]

**r7.** `Array.from` and `Array.of` work with a subclass. The specification calls each "an intentionally generic factory method; it does not require that its this value be the Array constructor."[^6] ECMA-262, 17th edition (ECMAScript 2026), also has `Array.fromAsync`, which builds an array from an async iterable.[^6][^3]

## Examples

The empty slots of `Array(n)` (rule r2). The `in` operator finds no element at index 0, and `map` skips the empty slots, so it gives back empty slots too.

```javascript run id=holes
const slots = Array(3);
console.log(slots.length, 0 in slots, slots[0]);
console.log(slots.map((_, i) => i));
console.log(Array.from(slots, (_, i) => i));
console.log(Array("2"), Array(1, 2));
try {
  Array(-1);
} catch (e) {
  console.log(e.name);
}
```

```output for=holes
3 false undefined
[ <3 empty items> ]
[ 0, 1, 2 ]
[ '2' ] [ 1, 2 ]
RangeError
```

`map` is one of the methods that MDN lists as having "special treatment for empty slots".[^5] `Array.from` reads each slot as `undefined`, so its map function runs for every index (rule r3).

`Array.from` with iterables, an array-like object, and a map function that makes a range (rules r3 and r4).

```javascript run id=from
const scores = new Map([["ana", 3], ["ben", 5]]);
console.log(Array.from(scores), Array.from(scores.keys()));
console.log(Array.from({ length: 3, 0: "x", 2: "z" }));

const range = (start, stop, step) =>
  Array.from({ length: Math.ceil((stop - start) / step) }, (_, i) => start + i * step);
console.log(range(0, 5, 1), range(1, 10, 2));
```

```output for=from
[ [ 'ana', 3 ], [ 'ben', 5 ] ] [ 'ana', 'ben' ]
[ 'x', undefined, 'z' ]
[ 0, 1, 2, 3, 4 ] [ 1, 3, 5, 7, 9 ]
```

## Pitfalls

### `fill` with an object shares it

`fill` stores the same value in every slot (rule r5). With an object or an array as the value, every row is one object.

```javascript wrong id=shared
const grid = Array(3).fill([]);
grid[0].push("x");
console.log(grid, grid[0] === grid[2]);
```

```output for=shared
[ [ 'x' ], [ 'x' ], [ 'x' ] ] true
```

Use the map function of `Array.from`. It runs once for each index, so each call makes its own array.

```javascript run id=separate
const grid = Array.from({ length: 3 }, () => []);
grid[0].push("x");
console.log(grid, grid[0] === grid[2]);
```

```output for=separate
[ [ 'x' ], [], [] ] false
```

### `Array(n)` is not a list of `n` values

`Array(3)` has no elements, and `map` on it does nothing (example [`holes`](#holes)). Use `Array.from({ length: 3 }, mapFn)`, or `Array(3).fill(v)` for a primitive `v`. To make an array that holds the one number 3, use `[3]` or `Array.of(3)` (rule r1).

## Comparison

Python builds a list from a loop with a list comprehension: "List comprehensions provide a concise way to create lists."[^7] Python has the same trap as `fill`: in `s * n`, "items in the sequence s are not copied; they are referenced multiple times."[^8] A Java array initializer, such as `{1, 2}`, is a way "to create an array and provide some initial values", and the array keeps its length: "Once an array object is created, its length never changes."[^9] Go makes a slice with `make`, which "allocates a new, hidden array to which the returned slice value refers."[^10] See [[array]] and [[dynamic-array]].

## References
