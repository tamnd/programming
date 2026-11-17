---
tag: 82J8PW
kind: feature
owner: javascript
slug: array-like-objects
name: Array-like objects
aliases: [array-like, array-like object, arguments object, NodeList, generic array methods, Array.prototype.call]
area: languages/javascript/collections
level: core
summary: >-
  An array-like object has a length and indexed elements but is not an Array, like arguments, a NodeList or a string. Array methods run on it through call, and Array.from turns it into an array.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/indexed-collections.html#sec-properties-of-the-array-prototype-object", locator: "23.1.3 Properties of the Array Prototype Object"}
see_also: [javascript/arrays, javascript/creating-arrays, javascript/array-methods, javascript/sparse-arrays]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array
  title: MDN Web Docs, JavaScript Reference, Array
  locator: Generic array methods; Normalization of the length property; Array-like objects
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Indexed_collections
  title: MDN Web Docs, JavaScript Guide, Indexed collections
  locator: Working with array-like objects
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/indexed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition)
  locator: 23.1.3 Properties of the Array Prototype Object
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/from
  title: MDN Web Docs, JavaScript Reference, Array.from()
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/arguments
  title: MDN Web Docs, JavaScript Reference, The arguments object
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Spread_syntax
  title: MDN Web Docs, JavaScript Reference, Spread syntax (...)
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/fill
  title: MDN Web Docs, JavaScript Reference, Array.prototype.fill()
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/rest_parameters
  title: MDN Web Docs, JavaScript Reference, Rest parameters
  locator: The difference between rest parameters and the arguments object
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/glossary.html#term-sequence
  title: The Python Glossary, Python 3.14
  locator: sequence
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/List.html
  title: Java SE 25 API, Interface List
  locator: the interface description
  accessed: 2026-11-18
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: String types
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=like
const like = { 0: "a", 1: "b", 2: "c", length: 3 };

console.log(Array.isArray(like), typeof like.map);
console.log(Array.prototype.join.call(like, "+"));
console.log(Array.prototype.map.call(like, (s) => s.toUpperCase()));
console.log(Array.from(like), Array.from("hey"));

function count() {
  return [arguments.length, Array.isArray(arguments), [...arguments]];
}
console.log(count(10, 20));
```

```output for=like
false undefined
a+b+c
[ 'A', 'B', 'C' ]
[ 'a', 'b', 'c' ] [ 'h', 'e', 'y' ]
[ 2, false, [ 10, 20 ] ]
```

| Task | Code |
|---|---|
| run an array method on it | `Array.prototype.join.call(obj, ",")` |
| make a real array | `Array.from(obj)` |
| make a real array from an iterable one | `[...obj]` |
| make a real array and map it | `Array.from(obj, fn)` |

## Semantics

**r1.** An array-like object "is expected to actually have a length property and to have indexed elements in the range 0 to length - 1".[^1] MDN defines the term more loosely as "any object that doesn't throw during the length conversion process".[^1] The arguments object, `NodeList` and `HTMLCollection` are array-like.[^1]

**r2.** An array-like object is not an Array. It may "look and behave like arrays on the surface but do not share all of their methods."[^2] So `like.map` is `undefined`, and "Array methods cannot be called directly on array-like objects."[^2]

**r3.** "Array methods are always generic — they don't access any internal data of the array object. They only access the array elements through the length property and the indexed elements."[^1] So `Array.prototype.method.call(obj, ...)` runs the method on `obj`.[^2] The specification says of these methods that they "are not dependent upon their this value being an Array exotic object."[^3] Example: [`like`](#like).

**r4.** The method converts `length` first. "The length property is converted to an integer and then clamped to the range between 0 and 253 - 1", that is, 2 to the power 53, minus 1. A missing `length` "behaves as if it has value 0".[^1] An index below 0 or above `length - 1` is ignored.[^1] A missing index in the range behaves as an empty slot of a [[javascript/sparse-arrays]] array.[^1] Example: [`length`](#length).

**r5.** `Array.from` turns an array-like object into an Array: it works on an iterable, "or, if the object is not iterable, array-like objects (objects with a length property and indexed elements)."[^4] A missing index becomes `undefined`.[^4] For arguments, MDN lists three ways: "slice() , Array.from() , or spread syntax."[^5] Spread is narrower: "Only iterable values, like Array and String , can be spread in array literals and argument lists."[^6]

**r6.** Strings are array-like, and "Array prototype methods can be used on strings as well".[^2] A method that writes does not work on a string, "as strings are immutable."[^7] Example: [`string`](#string).

## Examples

How the methods read `length` (rule r4).

```javascript run id=length
const sparse = { 0: "x", 2: "z", 5: "ignored", length: 3 };
console.log(Array.prototype.join.call(sparse, "-"));
console.log(Array.from(sparse));

console.log(Array.prototype.map.call({}, (x) => x));
console.log(Array.prototype.join.call({ 0: "a", 1: "b", length: "2" }));
```

```output for=length
x--z
[ 'x', undefined, 'z' ]
[]
a,b
```

Index 5 is past the length, so the methods do not see it. An object with no `length` is empty, and the string `'2'` converts to the number 2.

Read-only methods work on strings; writing ones do not (rule r6).

```javascript run id=string
const word = "stressed";
console.log(Array.prototype.map.call(word, (c) => c.charCodeAt(0)).slice(0, 3));
console.log(Array.from(word).reverse().join(""));
try {
  Array.prototype.push.call(word, "!");
} catch (e) {
  console.log(e.name);
}
```

```output for=string
[ 115, 116, 114 ]
desserts
TypeError
```

## Pitfalls

### Calling an array method directly

The guide shows the error: `arguments.forEach` "is not a function".[^2]

```javascript wrong id=direct
function printAll() {
  try {
    arguments.forEach((x) => console.log(x));
  } catch (e) {
    console.log(e.name);
  }
  Array.from(arguments).forEach((x) => console.log(x));
}
printAll("a", "b");
```

```output for=direct
TypeError
a
b
```

Convert the object with `Array.from` first, or use a rest parameter, `(...items)`: "The arguments object is not a real array, while rest parameters are Array instances".[^8]

### Spread on a plain array-like object

`[...obj]` needs an iterable, and "Many objects are not iterable, including all plain objects that lack a Symbol.iterator method".[^6] `{ 0: "a", length: 1 }` is array-like but not iterable, so spread throws. `Array.from` works on both kinds (rule r5).

```javascript wrong id=spread
const obj = { 0: "a", length: 1 };
try {
  console.log([...obj]);
} catch (e) {
  console.log(e.name);
}
console.log(Array.from(obj));
```

```output for=spread
TypeError
[ 'a' ]
```

## Comparison

Python's sequence is the closest idea. It is "An iterable which supports efficient element access using integer indices via the __getitem__() special method and defines a __len__() method".[^9] Java uses an interface: a `List` is "An ordered collection", and "The user can access elements by their integer index".[^10] In Go, a string has indexed bytes: "A string's bytes can be accessed by integer indices 0 through len(s)-1".[^11] In JavaScript, any object with a `length` works with the generic array methods (rule r3).

## References
