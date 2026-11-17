---
tag: FG3Z6R
kind: feature
owner: javascript
slug: sorting-arrays
name: Sorting arrays
aliases: [Array.prototype.sort, sort, compare function, comparator, localeCompare, stable sort, sort numbers]
area: languages/javascript/collections
level: core
summary: >-
  sort orders an array in place. Without a compare function it compares the elements as strings, so numbers need (a, b) => a - b. The sort is stable, and localeCompare orders text by language rules.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/indexed-collections.html#sec-array.prototype.sort", locator: "23.1.3.30 Array.prototype.sort"}
see_also: [javascript/copying-array-methods, javascript/array-methods, comparator, stable-sort, sort-a-list]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
  title: MDN Web Docs, JavaScript Reference, Array.prototype.sort()
  locator: the introduction; Parameters; Description; Sort stability; Sorting with non-well-formed comparator
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/indexed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition)
  locator: 23.1.3.30 Array.prototype.sort; 23.1.3.30.1 SortIndexedProperties
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/localeCompare
  title: MDN Web Docs, JavaScript Reference, String.prototype.localeCompare()
  locator: the introduction; Return value; Examples
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/stdtypes.html#list.sort
  title: The Python Standard Library, Python 3.14, Built-in Types
  locator: Lists, list.sort
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Stream.html
  title: Java SE 25 API, Interface Stream
  locator: sorted()
  accessed: 2026-11-18
- url: https://pkg.go.dev/slices
  title: Go standard library, package slices
  locator: func SortFunc; func SortStableFunc
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const nums = [40, 1, 5, 200];
console.log(nums.toSorted(), nums.toSorted((a, b) => a - b));

const words = ["pear", "Fig", "apple"];
console.log(words.toSorted(), words.toSorted((a, b) => a.localeCompare(b)));

const people = [{ name: "Ana", age: 31 }, { name: "Bo", age: 25 }];
console.log(people.sort((a, b) => a.age - b.age).map((p) => p.name));
```

```output for=basics
[ 1, 200, 40, 5 ] [ 1, 5, 40, 200 ]
[ 'Fig', 'apple', 'pear' ] [ 'apple', 'Fig', 'pear' ]
[ 'Bo', 'Ana' ]
```

| Compare function | Order |
|---|---|
| none | the elements as strings, by UTF-16 code units |
| `(a, b) => a - b` | numbers, ascending |
| `(a, b) => b - a` | numbers, descending |
| `(a, b) => a.localeCompare(b)` | strings, by the rules of a language |
| `(a, b) => a.key - b.key` | objects, by a numeric key |

A compare function returns a negative number to put `a` first, a positive number to put `b` first, and 0 to keep their order.

## Semantics

**r1.** `sort` "sorts the elements of an array in place and returns the reference to the same array, now sorted."[^1] `toSorted` does the same on a copy; see [[javascript/copying-array-methods]].

**r2.** Without a compare function, "all non-undefined array elements are sorted by converting them to strings and comparing strings in UTF-16 code units order."[^1] So 80 sorts before 9, "because numbers are converted to strings".[^1] "All undefined elements are sorted to the end of the array."[^1] Example: [`basics`](#basics).

**r3.** The compare function gets two elements, `a` and `b`. A result greater than 0 sorts `a` after `b`, a result less than 0 sorts `a` before `b`, and 0 keeps their original order.[^1] For numbers, "the compare function can subtract b from a", which works as long as the array has no `NaN`.[^1]

**r4.** The sort is stable. The specification states the condition on the result of the algorithm and ends with "i.e., the sort is stable."[^2] Elements that compare equal stay in the order they had before the sort. This lets a program sort by one key and then by another. Example: [`stable`](#stable).

**r5.** A compare function must be consistent. MDN lists five properties it must have, among them purity, anti-symmetry and transitivity. If a comparing function does not have all of them, "the program's behavior is not well-defined".[^1] The specification says that "The sort order is implementation-defined if SortCompare is not a consistent comparator for the elements of items."[^2] Example: [`bad-compare`](#bad-compare).

**r6.** `localeCompare` "returns a number indicating whether this string comes before, or after, or is the same as the given string in sort order."[^3] The result fits the compare function: "A negative number if referenceStr occurs before compareString ; positive if the referenceStr occurs after compareString ; 0 if they are equivalent."[^3] Its options include `numeric: true`, which compares runs of digits as numbers.[^3] Example: [`text`](#text).

## Examples

A stable sort keeps the order of equal keys (rule r4). The list is in name order, and the sort by grade keeps Alex before Devlin.

```javascript run id=stable
const students = [
  { name: "Alex", grade: 15 },
  { name: "Devlin", grade: 15 },
  { name: "Eagle", grade: 13 },
  { name: "Sam", grade: 14 },
];
students.sort((a, b) => a.grade - b.grade);
console.log(students.map((s) => `${s.name} ${s.grade}`).join(", "));
```

```output for=stable
Eagle 13, Sam 14, Alex 15, Devlin 15
```

Text in language order, and numbers inside text (rule r6).

```javascript run id=text
const items = ["réservé", "Premier", "Cliché", "communiqué", "café", "Adieu"];
console.log(items.toSorted().join(" "));
console.log(items.toSorted((a, b) => a.localeCompare(b)).join(" "));

const files = ["item10", "item2", "item1"];
console.log(files.toSorted((a, b) => a.localeCompare(b, undefined, { numeric: true })));
```

```output for=text
Adieu Cliché Premier café communiqué réservé
Adieu café Cliché communiqué Premier réservé
[ 'item1', 'item2', 'item10' ]
```

## Versions

ECMAScript 2019, the 10th edition, made the specification require a stable sort. MDN: "Before version 10 (or ECMAScript 2019), sort stability was not guaranteed".[^1] The 17th edition (ECMAScript 2026) has the same rule (rule r4).[^2]

## Pitfalls

### Numbers sorted as strings

`sort()` with no argument compares strings (rule r2), so 10 comes before 9.

```javascript wrong id=numbers
console.log([9, 10, 1, 100].sort());
```

```output for=numbers
[ 1, 10, 100, 9 ]
```

Pass `(a, b) => a - b`.

### A compare function that never returns a negative number

MDN gives this compare function as one that is not well-formed (rule r5): "if a > b , it returns 1 ; but by swapping a and b , it returns 0 instead of a negative value. Therefore, the resulting array will be different across engines."[^1] MDN also says that V8, the engine of Node.js, would not sort the array at all.[^1]

```javascript wrong id=bad-compare
const arr = [3, 1, 4, 1, 5, 9];
const compareFn = (a, b) => (a > b ? 1 : 0);
console.log(arr.sort(compareFn));
console.log(arr.sort((a, b) => a - b));
```

```output for=bad-compare
[ 3, 1, 4, 1, 5, 9 ]
[ 1, 1, 3, 4, 5, 9 ]
```

Return a negative number, 0 or a positive number: `(a, b) => a - b` for numbers, or `localeCompare` for strings.

## Comparison

Python guarantees a stable sort: "The sort() method is guaranteed to be stable."[^4] Java's stream sort is stable for ordered streams: "For ordered streams, the sort is stable."[^5] Go has two functions. `slices.SortFunc` "is not guaranteed to be stable", and `slices.SortStableFunc` sorts "while keeping the original order of equal elements".[^6] See [[stable-sort]] and [[comparator]].

## References
