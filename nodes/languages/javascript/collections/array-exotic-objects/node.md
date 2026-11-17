---
tag: 966Q11
kind: feature
owner: javascript
slug: array-exotic-objects
name: Array exotic objects
aliases: [Array exotic object, array index, length invariant, ArraySetLength, DefineOwnProperty of arrays, Array.isArray]
area: languages/javascript/collections
level: research
summary: >-
  ECMA-262 defines an Array as an exotic object whose [[DefineOwnProperty]] keeps length one more than the highest array index. Writing an index grows length, and writing length deletes elements.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html#sec-array-exotic-objects", locator: "10.4.2 Array Exotic Objects"}
see_also: [javascript/arrays, javascript/sparse-arrays, javascript/array-like-objects, array]
sources:
- url: https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 10
  locator: 10.4.2 Array Exotic Objects; 10.4.2.1 [[DefineOwnProperty]]; 10.4.2.2 ArrayCreate; 10.4.2.4 ArraySetLength
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 6
  locator: 6.1.7 The Object Type
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array
  title: MDN Web Docs, JavaScript Reference, Array
  locator: Relationship between length and numerical properties
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/abstract-operations.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 7
  locator: 7.2.2 IsArray
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/indexed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 23
  locator: 23.1.1 The Array Constructor
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/stdtypes.html#common-sequence-operations
  title: The Python Standard Library, Python 3.14, Built-in Types
  locator: Common Sequence Operations, notes
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

```javascript run id=invariant
const a = ["x", "y"];
a[4] = "z";
console.log(a.length);

a.length = 1;
console.log(a, Object.keys(a));

const plain = { length: 0 };
plain[4] = "z";
console.log(plain.length);

console.log(Object.getOwnPropertyDescriptor(a, "length"));
```

```output for=invariant
5
[ 'x' ] [ '0' ]
0
{ value: 1, writable: true, enumerable: false, configurable: false }
```

| Term in ECMA-262 | Meaning |
|---|---|
| Array exotic object | an object whose `[[DefineOwnProperty]]` is the one of 10.4.2.1 |
| array index | a canonical numeric string for an integer from 0 to 2 to the power 32, minus 2 |
| element | a property whose name is an array index |
| `length` | a non-configurable data property, always less than 2 to the power 32 |
| `ArraySetLength` | the steps that run when a program defines `length` |
| `IsArray` | the test behind `Array.isArray`, which also looks through a Proxy |

## Semantics

**r1.** "An Array is an exotic object that gives special treatment to array index property keys".[^1] What makes it exotic is one internal method: an object is an Array "if its [[DefineOwnProperty]] internal method uses the following implementation, and its other essential internal methods use the definitions found in 10.1."[^1] Reads, deletes and key listing are ordinary.

**r2.** "An array index is an integer index n such that CanonicalNumericIndexString(n) returns an integral Number in the inclusive interval from +0𝔽 to 𝔽(232 - 2)", that is, up to 2 to the power 32, minus 2.[^2] A key such as `'01'` or `'4294967295'` is not an array index, so it makes an ordinary property. "A property whose property name is an array index is also called an element."[^1] Example: [`keys`](#keys).

**r3.** The invariant: "The value of the 'length' property is numerically greater than the name of every own property whose name is an array index".[^1] The specification keeps it in two directions: "whenever an own property is added whose name is an array index, the value of the 'length' property is changed, if necessary, to be one more than the numeric value of that array index", and when `length` changes, "every own property whose name is an array index whose value is not smaller than the new length is deleted."[^1] MDN puts the first direction in engine terms: when the index "is outside the current bounds of the array, the engine will update the array's length property accordingly".[^3] Example: [`invariant`](#invariant).

**r4.** `length` is a data property, not an accessor. `ArrayCreate` defines it with `[[Writable]]: true`, `[[Enumerable]]: false` and `[[Configurable]]: false`.[^1] So `Object.getOwnPropertyDescriptor` shows a plain value, and `delete a.length` fails.

**r5.** A write to `length` runs `ArraySetLength`. It converts the value with `ToUint32` and with `ToNumber`, and "If SameValueZero(newLen, numberLen) is false, throw a RangeError exception."[^1] It deletes elements "in descending numeric index order". If an element cannot be deleted, it stops, sets `length` to one more than that index and returns false.[^1] Example: [`stuck`](#stuck).

**r6.** If `length` is not writable, the array cannot grow: "If index ≥ length and lengthDesc.[[Writable]] is false, return false."[^1] Example: [`frozen-length`](#frozen-length).

**r7.** `Array.isArray` uses `IsArray`, which returns true for an Array exotic object and, for a Proxy, "Return ? IsArray(proxyTarget)."[^4] The test is the internal method, not the prototype (rule r1), so a plain object whose prototype is `Array.prototype` is not an Array. A subclass instance is one if its constructor calls `super`: "Subclass constructors that intend to inherit the exotic Array behaviour must include a super call to the Array constructor to initialize subclass instances that are Array exotic objects."[^5] Example: [`is-array`](#is-array).

## Examples

Which keys are array indexes (rule r2).

```javascript run id=keys
const a = [];
a["01"] = "named";
a["4294967295"] = "named too";
a["4294967294"] = "last index";
console.log(a.length, Object.keys(a));
```

```output for=keys
4294967295 [ '4294967294', '01', '4294967295' ]
```

`'4294967294'` is the largest array index, so `length` becomes 2 to the power 32, minus 1. The other two keys are ordinary properties.

An element that cannot be deleted stops the truncation (rule r5). `Reflect` returns the result instead of throwing.

```javascript run id=stuck
const a = [1, 2, 3, 4];
Object.defineProperty(a, 1, { configurable: false });
console.log(Reflect.set(a, "length", 0), a.length, a);
```

```output for=stuck
false 2 [ 1, 2 ]
```

`ArraySetLength` deleted index 3 and index 2, failed at index 1, and set `length` to 2.

A non-writable `length` blocks growth (rule r6).

```javascript run id=frozen-length
const a = [1, 2];
Object.defineProperty(a, "length", { writable: false });
console.log(Reflect.set(a, 2, 3), Reflect.set(a, 0, 9), a);
try {
  a.push(4);
} catch (e) {
  console.log(e.name);
}
```

```output for=frozen-length
false true [ 9, 2 ]
TypeError
```

What `Array.isArray` accepts (rule r7).

```javascript run id=is-array
const fake = Object.setPrototypeOf({}, Array.prototype);
fake[3] = "x";
console.log(fake instanceof Array, Array.isArray(fake), fake.length);

const proxy = new Proxy([], {});
class Stack extends Array {
  constructor() {
    super();
  }
}
console.log(Array.isArray(proxy), Array.isArray(new Stack()));
```

```output for=is-array
true false 0
true true
```

`fake` inherits `length` from `Array.prototype`, whose `length` is 0, and its own index property does not change it. The `Stack` constructor calls `super`, so its instance is exotic.

## Pitfalls

### `instanceof Array` is not the test

`instanceof` checks the prototype chain, so `fake` in the [`is-array`](#is-array) example passes it but has none of the `length` behaviour. Use `Array.isArray`, which asks whether the object is exotic (rule r7).

### A `length` that is not a whole number

`ArraySetLength` throws for a value that does not survive `ToUint32` unchanged (rule r5).

```javascript wrong id=bad-length
const a = [1, 2, 3];
for (const v of [1.5, -1, 2 ** 32, "2"]) {
  try {
    a.length = v;
    console.log("ok", a.length);
  } catch (e) {
    console.log(e.name);
  }
}
```

```output for=bad-length
RangeError
RangeError
RangeError
ok 2
```

The string `'2'` converts to the same number by both routes, so it is accepted.

## Comparison

Python raises an error instead of growing a list: "An IndexError is raised if i is outside the sequence range."[^6] A Java array has "The public final field length, which contains the number of components of the array", and the field cannot change.[^7] In Go, "The length is part of the array's type".[^8] A JavaScript array is the only one of these whose length follows writes to its elements, and that comes from one exotic internal method (rule r1).

## References
