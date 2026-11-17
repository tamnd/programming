---
tag: DC2NNY
kind: feature
owner: javascript
slug: arrays
name: Arrays
aliases: [Array, JavaScript array, array object, array methods, Array.isArray]
area: languages/javascript/collections
level: intro
summary: >-
  A JavaScript array is an object that holds an ordered list of values of any types under the keys 0, 1, 2 and so on. It grows and shrinks, and its length property follows the highest index.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/indexed-collections.html#sec-array-objects", locator: "23.1 Array Objects"}
see_also: [javascript/indexed-collections, array, dynamic-array, in-place-and-copying-operations, collection-pipeline]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array
  title: MDN Web Docs, JavaScript Reference, Array
  locator: Description; Array indices; Relationship between length and numerical properties; Array methods and empty slots; Copying methods and mutating methods; Iterative methods; Generic array methods
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/indexed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition)
  locator: 23.1 Array Objects; 23.1.3 Properties of the Array Prototype Object
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/length
  title: MDN Web Docs, JavaScript Reference, Array.prototype.length
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Strict_equality
  title: MDN Web Docs, JavaScript Reference, Strict equality (===)
  locator: Description
- url: https://docs.python.org/3.14/tutorial/introduction.html#lists
  title: The Python Tutorial, Python 3.14, An Informal Introduction to Python
  locator: 3.1.4 Lists
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-10.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 10 Arrays
  locator: 10.3 Array Creation
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Slice types; Appending to and copying slices
---

## Synopsis

```javascript run id=groups
const fruits = ["banana", "apple"];
fruits.push("peach");
fruits.unshift("kiwi");
console.log(fruits, fruits.length);

console.log(fruits.indexOf("apple"), fruits.includes("plum"), fruits.at(-1));
console.log(fruits.map((f) => f.length), fruits.filter((f) => f.includes("a")));
console.log(fruits.slice(1, 3), fruits.join("+"));
console.log(fruits.pop(), fruits.shift(), fruits);
```

```output for=groups
[ 'kiwi', 'banana', 'apple', 'peach' ] 4
2 false peach
[ 4, 6, 5, 5 ] [ 'banana', 'apple', 'peach' ]
[ 'banana', 'apple' ] kiwi+banana+apple+peach
peach kiwi [ 'banana', 'apple' ]
```

The methods of `Array.prototype` fall into groups:

| Group | Methods | Changes the array |
|---|---|---|
| add and remove | `push`, `pop`, `unshift`, `shift`, `splice` | yes |
| search | `indexOf`, `lastIndexOf`, `includes`, `find`, `findIndex`, `findLast`, `findLastIndex` | no |
| iterate | `forEach`, `map`, `filter`, `reduce`, `reduceRight`, `some`, `every`, `flatMap` | no |
| copy and combine | `slice`, `concat`, `flat`, `toSpliced`, `toSorted`, `toReversed`, `with` | no |
| reorder and fill | `sort`, `reverse`, `fill`, `copyWithin` | yes |
| convert | `join`, `toString`, `toLocaleString`, `keys`, `values`, `entries` | no |

The static methods `Array.from`, `Array.of` and `Array.isArray` make arrays and test for them.[^1][^2]

## Semantics

**r1.** "JavaScript arrays are resizable and can contain a mix of different data types."[^1] An array is not a primitive. It is an object, and `typeof` gives `'object'` for it.[^1] `Array.isArray` tells an array from other objects. Example: [`objects`](#objects).

**r2.** An element is a property whose key is an array index. The index is a nonnegative integer, "or their respective string form".[^1] The engine converts the `2` in `years[2]` to the string `'2'`. So `'2'` and `'02'` are different keys, and only `'2'` is an array index.[^1] A key that is not an array index makes a named property, and "the array's traversal and mutation operations cannot be applied to these named properties."[^1] Example: [`objects`](#objects).

**r3.** "The value of the length property is a nonnegative integer with a value less than 232", that is, less than 2 to the power 32.[^3] A write to an index at or past the end makes the length one more than that index.[^1] A larger `length` adds empty slots, not `undefined` values, and a smaller `length` deletes elements.[^3] An invalid length, such as a negative number, a fraction or 2 to the power 32, throws a `RangeError`.[^3]

**r4.** Methods treat the empty slots of a sparse array in two ways. The older methods, such as `forEach`, `map` and `filter`, skip them. The methods that the language gained later, such as `keys`, `find`, `includes` and `toSorted`, treat them as `undefined`.[^1] The page [[javascript/sparse-arrays]] lists them.

**r5.** Some methods change the array, and others return a copy. The copying methods make a shallow copy: "The copy always happens shallowly".[^1] An object in the copy is the same object as in the original. The mutating methods return the same array or a number, such as the length after `push`.[^1]

**r6.** The iterative methods take a callback. "The callback function is called sequentially and at most once for each element in the array".[^1] It gets the element, the index and the array. `every`, `some`, `find`, `findIndex`, `findLast` and `findLastIndex` stop as soon as the result is known.[^1]

**r7.** "Array methods are always generic".[^1] They read only `length` and the indexed elements, so they work on array-like objects too.[^1] The specification makes the same point: most of the methods "are not dependent upon their this value being an Array exotic object."[^2]

## Examples

An array is an object with extra rules for its index keys (rules r1 and r2).

```javascript run id=objects
const years = [1950, 1960, 1970];
years["02"] = "not an element";
years.label = "decades";

console.log(typeof years, Array.isArray(years), Array.isArray({ length: 0 }));
console.log(years[2] === years["2"], years.length);
console.log(Object.keys(years));
console.log(years.map((y) => y + 5));
```

```output for=objects
object true false
true 3
[ '0', '1', '2', '02', 'label' ]
[ 1955, 1965, 1975 ]
```

The named properties `02` and `label` do not change the length, and `map` does not visit them.

The `length` rules (rule r3).

```javascript run id=length
const fruits = ["fig", "kiwi", "pear"];
fruits[5] = "lime";
console.log(fruits.length, Object.keys(fruits));

fruits.length = 7;
console.log(fruits);

fruits.length = 2;
console.log(fruits);

try {
  fruits.length = 2 ** 32;
} catch (e) {
  console.log(e.name);
}
```

```output for=length
6 [ '0', '1', '2', '5' ]
[ 'fig', 'kiwi', 'pear', <2 empty items>, 'lime', <1 empty item> ]
[ 'fig', 'kiwi' ]
RangeError
```

A copying method shares the objects of the original (rule r5), and a mutating method changes the array in place.

```javascript run id=shallow
const people = [{ name: "Ada" }, { name: "Alan" }];
const copy = people.slice();
copy[0].name = "Grace";
copy.push({ name: "Edsger" });

console.log(people.map((p) => p.name), copy.length);
console.log(people.reverse() === people, people[0].name);
```

```output for=shallow
[ 'Grace', 'Alan' ] 3
true Alan
```

## Pitfalls

### `===` compares arrays by identity

Two arrays with the same elements are two objects. "If both operands are objects, return true only if they refer to the same object."[^4]

```javascript wrong id=equal
const a = [1, 2, 3];
const b = [1, 2, 3];
console.log(a === b, a == b);
```

```output for=equal
false false
```

Compare the elements, for example with `a.length === b.length && a.every((x, i) => x === b[i])`.

### A string key that is not an index

`'02'`, `'-1'` and `'1.5'` are not array indexes, so a write with one of them makes a named property (rule r2). The array keeps its length, and `forEach` and `map` never see the value. Use a number as the index, or use a `Map` for keys of other kinds.

## Comparison

A Python list is close to a JavaScript array. It holds items of any type and is "a mutable type", so a program can change its content.[^5] A Java array cannot grow: "Once an array object is created, its length never changes."[^6] A Go slice grows with the built-in `append`, which returns the slice with the added elements.[^7] See [[array]] and [[dynamic-array]] for the concept.

## References
