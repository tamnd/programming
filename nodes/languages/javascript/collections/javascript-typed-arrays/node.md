---
tag: 7YCY1V
kind: feature
owner: javascript
slug: javascript-typed-arrays
name: JavaScript typed arrays
aliases: [typed array, TypedArray, Uint8Array, Int32Array, Float64Array, Uint8ClampedArray, BigInt64Array, binary data in JavaScript]
area: languages/javascript/collections
level: core
summary: >-
  A typed array such as Uint8Array or Float64Array is a fixed-length view of numbers of one type, stored as raw bytes in an ArrayBuffer. It has most array methods, but no push, and values wrap or clamp.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/indexed-collections.html#sec-typedarray-objects", locator: "23.2 TypedArray Objects; Table 70: The TypedArray Constructors"}
see_also: [javascript/arraybuffer-and-views, javascript/arrays, javascript/array-like-objects, bits-and-bytes, python/bytes-and-bytearray]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Typed_arrays
  title: MDN Web Docs, JavaScript Guide, JavaScript typed arrays
  locator: the introduction; Buffers; Views; Typed array views; Similarity to normal arrays; Endianness; Multiple views on the same data; Conversion to normal arrays
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/indexed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 23
  locator: 23.2 TypedArray Objects; 23.2.1 The %TypedArray% Intrinsic Object; 23.2.3.22 %TypedArray%.prototype.map
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/TypedArray
  title: MDN Web Docs, JavaScript Reference, TypedArray
  locator: Description; Value encoding and normalization; Constructor; Instance methods
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Float16 on TypedArrays, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/array.html
  title: The Python Standard Library, Python 3.14, array
  locator: the introduction
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/ByteBuffer.html
  title: Java SE 25 API, Class ByteBuffer
  locator: Byte order
  accessed: 2026-11-18
- url: https://pkg.go.dev/encoding/binary
  title: Go Packages, encoding/binary
  locator: Overview
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const bytes = new Uint8Array([1, 2, 255]);
const floats = new Float64Array(2);
console.log(bytes, floats);
console.log(bytes.length, bytes.BYTES_PER_ELEMENT, floats.byteLength);
console.log(Array.isArray(bytes), typeof bytes.push);
console.log(bytes.map((b) => b * 2));
```

```output for=basics
Uint8Array(3) [ 1, 2, 255 ] Float64Array(2) [ 0, 0 ]
3 1 16
false undefined
Uint8Array(3) [ 2, 4, 254 ]
```

| Constructor | Element size in bytes | Values |
|---|---|---|
| `Int8Array`, `Uint8Array`, `Uint8ClampedArray` | 1 | -128 to 127; 0 to 255; 0 to 255, clamped |
| `Int16Array`, `Uint16Array`, `Float16Array` | 2 | 16-bit integers; 16-bit floats |
| `Int32Array`, `Uint32Array`, `Float32Array` | 4 | 32-bit integers; 32-bit floats |
| `Float64Array` | 8 | ordinary JavaScript numbers |
| `BigInt64Array`, `BigUint64Array` | 8 | 64-bit integers, as BigInt values |

## Semantics

**r1.** "JavaScript typed arrays are array-like objects that provide a mechanism for reading and writing raw binary data in memory buffers."[^1] The specification: "A TypedArray presents an array-like view of an underlying binary data buffer".[^2] They do not replace arrays: "Typed arrays are not intended to replace arrays for any kind of functionality. Instead, they provide developers with a familiar interface for manipulating binary data."[^1]

**r2.** There is one constructor per element type. "There is no global property named TypedArray , nor is there a directly visible TypedArray constructor."[^3] The specification has a %TypedArray% intrinsic, which "is a constructor function object that all of the TypedArray constructor objects inherit from."[^2] Table 70 of the specification gives each constructor its element type and its element size.[^2] `BYTES_PER_ELEMENT` gives that size. Example: [`basics`](#basics).

**r3.** Storage is split in two: "JavaScript typed arrays split the implementation into buffers and views. A buffer is an object representing a chunk of data; it has no format to speak of, and offers no mechanism for accessing its contents. In order to access the memory contained in a buffer, you need to use a view."[^1] A typed array is such a view, and "you can create multiple views onto the same data."[^1] Example: [`views`](#views). See [[javascript/arraybuffer-and-views]].

**r4.** A stored value is converted to the element type. "All integer arrays (except Uint8ClampedArray ) use fixed-width number conversion, which first truncates the decimal part of the number and then takes the lowest bits."[^3] "Uint8ClampedArray first clamps the number to the range 0 to 255 (values greater than 255 become 255 and values less than 0 become 0). It then rounds (instead of flooring) the result to the nearest integer, with half-to-even".[^3] Example: [`encode`](#encode).

**r5.** "Typed arrays are, in principle, fixed-length, so array methods that may change the length of an array are not available. This includes pop , push , shift , splice , and unshift ."[^1] And "calling Array.isArray() on a typed array returns false".[^1] Typed arrays have the "extra set and subarray methods"; "The subarray() method creates a new typed array view that references the same buffer as the original typed array, but with a narrower span."[^1] `slice` "Extracts a section of an array and returns a new array."[^3] Example: [`share`](#share).

**r6.** Other array methods work, and they keep the element type. In the specification, `map` makes its result with TypedArraySpeciesCreate from the source array, so a `Uint8Array` maps into a `Uint8Array`, and r4 applies to the results.[^2] To get a normal array, MDN shows `Array.from()` "as well as the spread syntax".[^1]

**r7.** Reads and writes past the end do nothing: "Out-of-bounds index access always returns undefined , without actually accessing the property on the object. Any attempt to write to such an out-of-bounds property has no effect: it does not throw an error but doesn't change the buffer or typed array."[^1] Byte order is the platform's: "Typed array views are in the native byte-order (see Endianness) of your platform. With a DataView , the byte-order can be controlled."[^1]

## Examples

One buffer, two views (rule r3). Writing -1 into an `Int32Array` sets all four of its bytes, so the result does not depend on byte order.

```javascript run id=views
const buffer = new ArrayBuffer(8);
const ints = new Int32Array(buffer);
const bytes = new Uint8Array(buffer);
ints[0] = -1;
console.log(ints.length, bytes.length, buffer.byteLength);
console.log(bytes.join(" "));
bytes[7] = 128;
console.log(ints[1] !== 0, ints.buffer === bytes.buffer);
```

```output for=views
2 8 8
255 255 255 255 0 0 0 0
true true
```

Wraparound and clamping (rule r4).

```javascript run id=encode
console.log(new Uint8Array([256, 257, -1, 3.9]));
console.log(new Int8Array([127, 128, 200]));
console.log(new Uint8ClampedArray([300, -5, 2.5, 3.5]));
console.log(new Float32Array([0.1])[0]);
```

```output for=encode
Uint8Array(4) [ 0, 1, 255, 3 ]
Int8Array(3) [ 127, -128, -56 ]
Uint8ClampedArray(4) [ 255, 0, 2, 4 ]
0.10000000149011612
```

`Float32Array` keeps fewer bits than a JavaScript number, so 0.1 comes back as the nearest 32-bit float.

`subarray` shares memory, `slice` copies it (rule r5).

```javascript run id=share
const data = new Uint8Array([10, 20, 30, 40]);
const view = data.subarray(1, 3);
const copy = data.slice(1, 3);
data[1] = 99;
console.log(view, copy);
data.set([7, 8], 2);
console.log(data, [...data].concat(0));
```

```output for=share
Uint8Array(2) [ 99, 30 ] Uint8Array(2) [ 20, 30 ]
Uint8Array(4) [ 10, 99, 7, 8 ] [ 10, 99, 7, 8, 0 ]
```

## Versions

The TC39 list of finished proposals gives Float16 on TypedArrays the expected publication year 2025, so `Float16Array` is part of ECMAScript 2025.[^4] Node v24.10.0 has it.

```javascript run id=detect
console.log(typeof Float16Array, new Float16Array([1.5, 65504]));
```

```output for=detect
function Float16Array(2) [ 1.5, 65504 ]
```

## Pitfalls

### A write past the end is ignored

A typed array does not grow, and an out-of-range write gives no error (rule r7).

```javascript wrong id=out-of-range
const scores = new Uint8Array(2);
scores[2] = 5;
scores.push?.(6);
console.log(scores, scores.length, scores[2]);
```

```output for=out-of-range
Uint8Array(2) [ 0, 0 ] 2 undefined
```

Allocate the length you need first, or collect the values in a normal array and convert it with `Uint8Array.from`.

### Results that do not fit the element type

`map` gives back the same type, so results wrap (rules r4 and r6). Convert to a normal array first when the results can leave the range.

```javascript wrong id=wrap
const pixels = new Uint8Array([100, 200]);
console.log(pixels.map((p) => p * 2));
console.log(Array.from(pixels, (p) => p * 2));
console.log(pixels.reduce((a, b) => a + b));
```

```output for=wrap
Uint8Array(2) [ 200, 144 ]
[ 200, 400 ]
300
```

`reduce` returns a plain number, so the sum is not wrapped.

## Comparison

Python's `array` module "defines an object type which can compactly represent an array of basic values", and "The type is specified at object creation time by using a type code, which is a single character."[^5] JavaScript has a constructor per type instead (rule r2), and every typed array is a view on a separate buffer (rule r3). Java's `ByteBuffer` fixes its byte order explicitly: "The initial order of a byte buffer is always BIG_ENDIAN ."[^6] A JavaScript typed array uses the platform order (rule r7). Go's encoding/binary "implements simple translation between numbers and byte sequences", reading and writing "fixed-size values".[^7] See [[bits-and-bytes]] and [[python/bytes-and-bytearray]].

## References
