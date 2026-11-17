---
tag: 4D3BW2
kind: feature
owner: javascript
slug: indexed-collections
name: Indexed collections
aliases: [indexed collection, array, typed array, array length, array index]
area: languages/javascript/collections
level: intro
summary: >-
  An indexed collection holds values in order and finds each one by a number from 0. JavaScript has two kinds: the Array, which holds any values, and the typed arrays, which hold numbers of one type.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/indexed-collections.html", locator: "23 Indexed Collections"}
see_also: [array, dynamic-array, array-indexing, collection]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Indexed_collections
  title: MDN Web Docs, JavaScript Guide, Indexed collections
  locator: the introduction; Creating an array; Referring to array elements; Populating an array; Understanding length
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/indexed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition)
  locator: 23.1 Array Objects; 23.1.3 Properties of the Array Prototype Object; 23.2 TypedArray Objects
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Typed_arrays
  title: MDN Web Docs, JavaScript Guide, JavaScript typed arrays
  locator: the introduction
- url: https://docs.python.org/3.14/tutorial/introduction.html#lists
  title: The Python Tutorial, Python 3.14, An Informal Introduction to Python
  locator: 3.1.4 Lists
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-10.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 10 Arrays
  locator: the introduction; 10.3 Array Creation
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Array types; Slice types
---

## Synopsis

```javascript run id=forms
const literal = ["Wind", "Rain", "Fire"];
const called = Array("Wind", "Rain", "Fire");
const empty = Array(3);
const bytes = new Uint8Array([1, 2, 3]);

console.log(literal[0], literal.length);
console.log(called.length, empty.length, 0 in empty);
console.log(bytes[2], bytes.length, Array.isArray(bytes));
```

```output for=forms
Wind 3
3 3 false
3 3 false
```

The forms of an indexed collection:

| Form | Example | Result |
|---|---|---|
| array literal | `["a", "b"]` | an Array with the listed elements |
| `Array(...)` or `new Array(...)` | `Array("a", "b")` | the same Array |
| `Array(n)` with one number | `Array(3)` | an Array with length 3 and no elements |
| element access | `a[0]`, `a[i] = v` | reads or writes the element at index `i` |
| `length` | `a.length`, `a.length = 0` | one more than the highest index; a write truncates |
| typed array | `new Uint8Array(4)`, `Float64Array.of(1.5)` | a view of numbers of one type over a buffer |

## Semantics

**r1.** MDN defines the array as "an ordered list of values that you refer to with a name and an index."[^1] The first index is 0.[^1]

**r2.** The literal, the call `Array(...)` and the call `new Array(...)` with the same list of values make equivalent arrays.[^1] The Array constructor behaves the same when it is called as a function and when it is called with `new`.[^2]

**r3.** "When a single Number value is passed to the Array() constructor or function, it is interpreted as an arrayLength, not as a single element."[^1] The result has that length and no elements. A single number that is not a whole number, such as `9.3`, throws a `RangeError`.[^1] Example: [`one-number`](#one-number).

**r4.** "Because elements are also properties, you can access them using property accessors."[^1] So `arr['length']` and `arr.length` read the same property. An assignment with a key that is not an array index, such as `arr[3.4] = 1`, makes an ordinary property and does not change the length.[^1] Example: [`length`](#length).

**r5.** The `length` property is one more than the highest index of an element.[^1] A write of a smaller value to `length` deletes the elements at that index and above, and a write of 0 empties the array.[^1] Example: [`length`](#length).

**r6.** "JavaScript typed arrays are array-like objects that provide a mechanism for reading and writing raw binary data in memory buffers."[^3] The specification describes each one as "an array-like view of an underlying binary data buffer".[^2] A typed array holds numbers of one element type, and `Array.isArray` returns false for it.[^3] Example: [`typed`](#typed).

**r7.** Most methods of `Array.prototype` are generic. The specification says that they "are not dependent upon their this value being an Array exotic object."[^2] The typed arrays have methods of their own with the same names, but not all of the Array methods: they have no `push` and no `pop`.[^3]

## Examples

Populating an array by index, and walking it in index order.

```javascript run id=populate
const emp = [];
emp[0] = "Casey Jones";
emp[1] = "Phil Lesh";
emp[2] = "August West";

for (let i = 0; i < emp.length; i++) {
  console.log(i, emp[i]);
}
for (const name of emp) {
  console.log(name.toUpperCase());
}
```

```output for=populate
0 Casey Jones
1 Phil Lesh
2 August West
CASEY JONES
PHIL LESH
AUGUST WEST
```

The `length` property follows the highest index, and a write to it truncates the array (rules r4 and r5).

```javascript run id=length
const cats = [];
cats[30] = "Dusty";
console.log(cats.length);

const pets = ["Dusty", "Misty", "Twiggy"];
pets.length = 2;
console.log(pets);
pets.length = 0;
console.log(pets, pets.length);

const arr = [];
arr[3.4] = "Oranges";
console.log(arr.length, Object.hasOwn(arr, "3.4"));
```

```output for=length
31
[ 'Dusty', 'Misty' ]
[] 0
0 true
```

A typed array converts each value to its element type. A `Uint8Array` holds whole numbers from 0 to 255, so 256 becomes 0 and 1.7 becomes 1 (rule r6).

```javascript run id=typed
const bytes = new Uint8Array(4);
bytes[0] = 255;
bytes[1] = 256;
bytes[2] = 1.7;
bytes[3] = -1;
console.log(bytes);
console.log(typeof bytes.push, typeof [].push);
console.log(Array.from(bytes).map((b) => b * 2));
```

```output for=typed
Uint8Array(4) [ 255, 0, 1, 255 ]
undefined function
[ 510, 0, 2, 510 ]
```

`Array.from` copies the typed array into an Array, which can hold any values (rule r7).

## Pitfalls

### `Array(n)` with one number

`Array(3)` makes an empty array of length 3, not an array that holds 3 (rule r3). The same call with a fraction throws.

```javascript run id=one-number
console.log(Array(3), Array(3).length);
console.log(Array("3"), Array.of(3), [3]);
try {
  Array(9.3);
} catch (e) {
  console.log(e.name);
}
```

```output for=one-number
[ <3 empty items> ] 3
[ '3' ] [ 3 ] [ 3 ]
RangeError
```

Use a literal, or `Array.of`, to make an array of one number.

### A lost write to a typed array

A typed array does not grow when the program writes past its end. The write does nothing and throws no error. A value outside the range of the element type wraps around, as 256 did in the `typed` example.

```javascript wrong id=past-end
const counts = new Uint8Array(2);
counts[2] = 9;
console.log(counts.length, counts[2]);
```

```output for=past-end
2 undefined
```

## Comparison

A Python list is written as "a list of comma-separated values (items) between square brackets", and its items can have different types, as in a JavaScript array.[^4] A Java array has a fixed size: "Once an array object is created, its length never changes."[^5] A JavaScript Array has no such limit (rule r5). In Go, the length of an array is part of its type, and a slice is "a descriptor for a contiguous segment of an underlying array".[^6] A Go slice can grow with `append`, as a JavaScript array grows with `push`. See [[array]] and [[dynamic-array]].

## References
