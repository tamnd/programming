---
tag: VHBGWY
kind: feature
owner: javascript
slug: multi-dimensional-arrays
name: Multi-dimensional arrays
aliases: [2D array, two-dimensional array, nested array, array of arrays, matrix, grid]
area: languages/javascript/collections
level: intro
summary: >-
  A JavaScript multi-dimensional array is an array whose elements are arrays, read as grid[row][col]. Build each row separately: fill with an array puts one shared row in every slot.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/indexed-collections.html#sec-array-objects", locator: "23.1 Array Objects"}
see_also: [javascript/arrays, javascript/creating-arrays, multidimensional-array, flatten-a-nested-list]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Indexed_collections
  title: MDN Web Docs, JavaScript Guide, Indexed collections
  locator: Multi-dimensional arrays
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/from
  title: MDN Web Docs, JavaScript Reference, Array.from()
  locator: Parameters; Examples
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/fill
  title: MDN Web Docs, JavaScript Reference, Array.prototype.fill()
  locator: Parameters; Using fill() to create a matrix of all 1
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array
  title: MDN Web Docs, JavaScript Reference, Array
  locator: Copy an array
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/stdtypes.html#common-sequence-operations
  title: The Python Standard Library, Python 3.14, Built-in Types
  locator: Common Sequence Operations, note 2
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-10.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 10 Arrays
  locator: 10.7 Array Members
  accessed: 2026-11-18
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Array types
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=grid
const board = [
  ["x", "o", "x"],
  ["o", "x", "o"],
  ["o", "x", "x"],
];
console.log(board[1][2], board.length, board[0].length);

const zeros = Array.from({ length: 2 }, () => Array(3).fill(0));
zeros[0][1] = 7;
console.log(zeros);

for (const row of board) {
  console.log(row.join(" "));
}
```

```output for=grid
o 3 3
[ [ 0, 7, 0 ], [ 0, 0, 0 ] ]
x o x
o x o
o x x
```

| Task | Code |
|---|---|
| read a cell | `grid[r][c]` |
| number of rows, of columns | `grid.length`, `grid[r].length` |
| make an `R` by `C` grid of `v` | `Array.from({ length: R }, () => Array(C).fill(v))` |
| visit every cell | nested `for` loops, or `grid.forEach((row) => row.forEach(...))` |
| copy the grid one level deeper | `grid.map((row) => row.slice())` |

## Semantics

**r1.** "Arrays can be nested, meaning that an array can contain another array as an element. Using this characteristic of JavaScript arrays, multi-dimensional arrays can be created."[^1] `grid[r][c]` reads the row `grid[r]`, which is an array, and then reads element `c` of that row.

**r2.** Each row is an array of its own, so rows can have different lengths. `grid.length` counts the rows, and `grid[r].length` gives the length of row `r`. Example: [`jagged`](#jagged).

**r3.** The Guide builds a grid with a loop: it makes the outer array, then puts a new inner array at each index.[^1] `Array.from` does the same in one call: its map function runs once for each index, and its result becomes the element.[^2] Example: [`guide`](#guide).

**r4.** `fill` puts one value in every slot: "if value is an object, each slot in the array will reference that object."[^3] An array is an object, so `Array(3).fill([])` makes three references to one row. MDN's matrix example calls `new Array(4).fill(1)` inside the loop, once per row.[^3] Example: [`shared-rows`](#shared-rows).

**r5.** The copying operations copy one level: "All built-in array-copy operations (spread syntax, Array.from() , Array.prototype.slice() , and Array.prototype.concat() ) create shallow copies."[^4] A copy of a grid made with `slice` or spread shares its rows with the original. Example: [`copy`](#copy).

## Examples

The two-dimensional array from the Guide, built with nested loops (rule r3).

```javascript run id=guide
const a = new Array(4);
for (let i = 0; i < 4; i++) {
  a[i] = new Array(4);
  for (let j = 0; j < 4; j++) {
    a[i][j] = `[${i}, ${j}]`;
  }
}
console.log(a[0].join(" "));
console.log(a[3].join(" "));
```

```output for=guide
[0, 0] [0, 1] [0, 2] [0, 3]
[3, 0] [3, 1] [3, 2] [3, 3]
```

A jagged array, and a transpose with `map` (rule r2).

```javascript run id=jagged
const triangle = [[1], [1, 1], [1, 2, 1]];
console.log(triangle.map((row) => row.length));

const m = [
  [1, 2, 3],
  [4, 5, 6],
];
const t = m[0].map((_, c) => m.map((row) => row[c]));
console.log(t);
```

```output for=jagged
[ 1, 2, 3 ]
[ [ 1, 4 ], [ 2, 5 ], [ 3, 6 ] ]
```

## Pitfalls

### `fill` with a row shares the row

The grid looks right until the program writes to it (rule r4).

```javascript wrong id=shared-rows
const grid = Array(3).fill(Array(3).fill(0));
grid[0][0] = 1;
console.log(grid);
```

```output for=shared-rows
[ [ 1, 0, 0 ], [ 1, 0, 0 ], [ 1, 0, 0 ] ]
```

Make each row in a callback, as in the `grid` example: `Array.from({ length: 3 }, () => Array(3).fill(0))`.

### A copy that shares its rows

Spread copies the outer array only (rule r5).

```javascript wrong id=copy
const original = [[1, 2], [3, 4]];
const shallow = [...original];
shallow[0][0] = 99;
console.log(original[0]);

const deeper = original.map((row) => row.slice());
deeper[1][1] = 0;
console.log(original[1], deeper[1]);
```

```output for=copy
[ 99, 2 ]
[ 3, 4 ] [ 3, 0 ]
```

Copy each row, as `deeper` does.

## Comparison

Python lists have the same trap with repetition. In `[[]] * 3`, "all three elements of [[]] * 3 are references to this single empty list."[^5] A Java multidimensional array is copied shallowly too: "A clone of a multidimensional array is shallow, which is to say that it creates only a single new array. Subarrays are shared."[^6] In Go, "Array types are always one-dimensional but may be composed to form multi-dimensional types."[^7] See [[multidimensional-array]].

## References
