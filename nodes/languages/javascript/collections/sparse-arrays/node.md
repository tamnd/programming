---
tag: CDG059
kind: feature
owner: javascript
slug: sparse-arrays
name: Sparse arrays
aliases: [sparse array, empty slot, array hole, holes in arrays, empty items]
area: languages/javascript/collections
level: core
summary: >-
  A sparse array has empty slots: indexes below its length that hold no element. Some array methods skip the slots, others read them as undefined, and the length counts them all.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/indexed-collections.html#sec-array-objects", locator: "23.1 Array Objects"}
see_also: [javascript/arrays, javascript/creating-arrays, javascript/array-methods, array]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Indexed_collections
  title: MDN Web Docs, JavaScript Guide, Indexed collections
  locator: Sparse arrays
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/indexed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition)
  locator: 23.1.3.15 Array.prototype.forEach, Note 1
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array
  title: MDN Web Docs, JavaScript Reference, Array
  locator: Array methods and empty slots
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/from
  title: MDN Web Docs, JavaScript Reference, Array.from()
  locator: Description
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/stdtypes.html#common-sequence-operations
  title: The Python Standard Library, Python 3.14, Built-in Types
  locator: Common Sequence Operations, notes
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-10.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 10 Arrays
  locator: 10.6 Array Initializers
  accessed: 2026-11-18
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Composite literals
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=make
const a = Array(3);
const b = [1, , 3];
const c = [1, 2];
c[4] = 5;
const d = [1, 2];
d.length = 4;
const e = [1, 2, 3];
delete e[1];

console.log(a, b, c);
console.log(d, e);
console.log(1 in b, b[1], b.length);
```

```output for=make
[ <3 empty items> ] [ 1, <1 empty item>, 3 ] [ 1, 2, <2 empty items>, 5 ]
[ 1, 2, <2 empty items> ] [ 1, <1 empty item>, 3 ]
false undefined 3
```

| Operation | An empty slot |
|---|---|
| `a[i]`, `for...of`, spread `[...a]` | reads as `undefined` |
| `forEach`, `filter`, `some`, `every`, `reduce` | skipped |
| `map`, `concat`, `copyWithin`, `reverse` | kept as an empty slot |
| `flat` | removed |
| `find`, `findIndex`, `includes`, `join`, `keys`, `values`, `entries` | treated as `undefined` |
| `toSorted`, `toReversed`, `toSpliced`, `with`, `fill`, `Array.from` | becomes `undefined` or the fill value |
| `i in a`, `Object.keys(a)`, `for...in` | not present |

## Semantics

**r1.** Arrays can contain empty slots, "which are not the same as slots filled with the value undefined."[^1] The length counts the empty slots, and the `in` operator finds no element at them, as the [`make`](#make) example shows.

**r2.** Five operations make empty slots: the `Array(n)` constructor, consecutive commas in a literal, a write to an index greater than `length`, a write of a larger `length`, and `delete` on an element.[^1] Example: [`make`](#make).

**r3.** "In some operations, empty slots behave as if they are filled with undefined."[^1] These include indexed access, `for...of` and spread syntax.[^1] Example: [`read`](#read).

**r4.** "But in others (most notably array iteration methods), empty slots are skipped."[^1] The specification says of `forEach`: "callback is called only for elements of the array which actually exist; it is not called for missing elements of the array."[^2] Property enumeration, as in `Object.keys` and `for...in`, lists only the indexes that hold an element.[^1] Example: [`read`](#read).

**r5.** Some copying methods keep the slots: "Other methods, such as concat , copyWithin , etc., preserve empty slots when doing the copying, so in the end the array is still sparse."[^3] `map` is one of them. `flat`, in contrast, removes empty slots.[^3]

**r6.** Other methods "do not treat empty slots specially and treat them as if they contain undefined."[^3] MDN lists `entries`, `fill`, `find`, `findIndex`, `findLast`, `findLastIndex`, `includes`, `join`, `keys`, `toLocaleString`, `toReversed`, `toSorted`, `toSpliced`, `values` and `with`.[^3] So the copying methods return a dense array. `Array.from` does too: "Array.from() never creates a sparse array."[^4] Example: [`densify`](#densify).

## Examples

The same array read in three ways (rules r3 and r4).

```javascript run id=read
const arr = [1, 2, , , 5];
console.log([...arr]);
console.log(arr.map((x) => x * 10));
console.log(arr.filter(() => true), arr.some((x) => !x));
console.log(Object.keys(arr), arr.includes(undefined), arr.indexOf(undefined));
```

```output for=read
[ 1, 2, undefined, undefined, 5 ]
[ 10, 20, <2 empty items>, 50 ]
[ 1, 2, 5 ] false
[ '0', '1', '4' ] true -1
```

`includes` treats the slots as `undefined` and finds one. `indexOf` skips them and finds none.

Turning a sparse array into a dense one (rule r6).

```javascript run id=densify
const holes = [1, , 3];
console.log(Array.from(holes), holes.toSorted(), holes.with(0, 1));
console.log(holes.flat(), holes.filter(() => true));
```

```output for=densify
[ 1, undefined, 3 ] [ 1, 3, undefined ] [ 1, undefined, 3 ]
[ 1, 3 ] [ 1, 3 ]
```

`Array.from` and `with` fill each slot with `undefined`. `flat` and `filter` drop the slots and close the gap.

## Pitfalls

### `map` over `Array(n)` does nothing

`map` skips the empty slots of `Array(3)` and keeps them in its result (rule r5).

```javascript wrong id=map-holes
console.log(Array(3).map((_, i) => i));
console.log(Array.from({ length: 3 }, (_, i) => i));
```

```output for=map-holes
[ <3 empty items> ]
[ 0, 1, 2 ]
```

Use `Array.from` with a map function, as in [[javascript/creating-arrays]].

### `delete` leaves a hole

`delete` removes the property but keeps the length, so the array gets an empty slot (rule r2). Use `splice` to remove an element and move the rest down.

```javascript wrong id=delete
const list = ["a", "b", "c"];
delete list[1];
console.log(list, list.length);
list.splice(1, 1);
console.log(list, list.length);
```

```output for=delete
[ 'a', <1 empty item>, 'c' ] 3
[ 'a', 'c' ] 2
```

## Comparison

A Python list has no empty slots to read: "An IndexError is raised if i is outside the sequence range."[^5] A Java array always has a value in each slot, because "each component of the array is initialized to its default value".[^6] A Go array literal may skip indexes with keys, and the skipped elements get the zero value: the specification writes `[10]float32{-1, 4: -0.1, -0.1, 9: -1}` for the array `[10]float32{-1, 0, 0, 0, -0.1, -0.1, 0, 0, 0, -1}`.[^7] See [[array]].

## References
