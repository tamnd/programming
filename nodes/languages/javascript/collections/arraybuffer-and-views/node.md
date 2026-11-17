---
tag: Q9ZSDJ
kind: feature
owner: javascript
slug: arraybuffer-and-views
name: ArrayBuffer and views
aliases: [ArrayBuffer, DataView, SharedArrayBuffer, resizable ArrayBuffer, ArrayBuffer.prototype.transfer, detached buffer, endianness in JavaScript]
area: languages/javascript/collections
level: advanced
summary: >-
  An ArrayBuffer holds raw bytes and cannot be read directly. Typed arrays and DataView are views on it. A buffer can be sliced, resized up to maxByteLength, or transferred, which detaches the old one.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/structured-data.html#sec-arraybuffer-objects", locator: "25.1 ArrayBuffer Objects; 25.2 SharedArrayBuffer Objects; 25.3 DataView Objects"}
see_also: [javascript/javascript-typed-arrays, javascript/uint8array-base64-and-hex, bits-and-bytes, read-a-binary-file, python/binary-data-record-layouts]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/ArrayBuffer
  title: MDN Web Docs, JavaScript Reference, ArrayBuffer
  locator: the introduction; Resizing ArrayBuffers; Transferring ArrayBuffers; Static methods
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Typed_arrays
  title: MDN Web Docs, JavaScript Guide, JavaScript typed arrays
  locator: Buffers; DataView
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/DataView
  title: MDN Web Docs, JavaScript Reference, DataView
  locator: the introduction; Endianness; Instance methods
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/TypedArray
  title: MDN Web Docs, JavaScript Reference, TypedArray
  locator: Behavior when viewing a resizable buffer; ByteOffset must be aligned
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/structured-data.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 25
  locator: 25.1.2 Fixed-length and Resizable ArrayBuffer Objects
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/ArrayBuffer/transfer
  title: MDN Web Docs, JavaScript Reference, ArrayBuffer.prototype.transfer()
  locator: the introduction; Parameters; Description
  accessed: 2026-11-18
- url: https://nodejs.org/docs/latest-v24.x/api/buffer.html
  title: Node.js v24 documentation, Buffer
  locator: the introduction
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Resizable and growable ArrayBuffers; ArrayBuffer transfer, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/struct.html
  title: The Python Standard Library, Python 3.14, struct
  locator: the introduction
  accessed: 2026-11-18
- url: https://pkg.go.dev/encoding/binary
  title: Go Packages, encoding/binary
  locator: Overview
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const buf = new ArrayBuffer(8);
const view = new DataView(buf);
view.setUint16(0, 0x1234);
view.setUint16(2, 0x1234, true);
console.log(new Uint8Array(buf, 0, 4).join(" "));
console.log(view.getUint16(0), view.getUint16(2, true), buf.byteLength);
console.log(ArrayBuffer.isView(view), ArrayBuffer.isView(buf));
console.log(Buffer.from("hi") instanceof Uint8Array);
```

```output for=basics
18 52 52 18
4660 4660 8
true false
true
```

| Action | Code | Result |
|---|---|---|
| allocate | `new ArrayBuffer(n)` | `n` bytes, all 0 |
| allocate resizable | `new ArrayBuffer(n, { maxByteLength: m })` | can `resize` up to `m` |
| copy bytes | `buf.slice(start, end)` | a new buffer |
| move bytes | `buf.transfer(n?)` | a new buffer; `buf` is detached |
| read and write | `new Uint8Array(buf)`, `new DataView(buf)` | a view on the same bytes |

## Semantics

**r1.** "The ArrayBuffer object is used to represent a generic raw binary data buffer. It is an array of bytes, often referred to in other languages as a "byte array"."[^1] "You cannot directly manipulate the contents of an ArrayBuffer ; instead, you create one of the typed array objects or a DataView object which represents the buffer in a specific format, and use that to read and write the contents of the buffer."[^1] `ArrayBuffer.isView()` "Returns true if arg is one of the ArrayBuffer views, such as typed array objects or a DataView ."[^1]

**r2.** Buffers support a few actions. "Allocate: As soon as a new buffer is created, a new memory span is allocated and initialized to 0 ." "Copy: Using the slice() method, you can efficiently copy a portion of the memory without creating views to manually copy each byte."[^2] Transfer and resize are rules r5 and r6.

**r3.** "The DataView view provides a low-level interface for reading and writing multiple number types in a binary ArrayBuffer , without having to care about the platform's endianness."[^3] Each method names a byte offset; for example `getUint16` "Reads 2 bytes starting at the specified byte offset of this DataView and interprets them as a 16-bit unsigned integer."[^3] "DataView defaults to big-endian read and write, but most platforms use little-endian."[^3] A typed array uses the platform's order instead: "Typed array views are in the native byte-order (see Endianness) of your platform."[^2] Example: [`basics`](#basics) and [`record`](#record).

**r4.** A typed array can start at a byte offset, but "the byteOffset argument must be aligned to its element size; in other words, the offset must be a multiple of BYTES_PER_ELEMENT ."[^4] A record with fields at odd offsets needs a `DataView`. Example: [`align`](#align).

**r5.** The specification has two kinds of buffer: "A fixed-length ArrayBuffer is an ArrayBuffer whose byte length cannot change after creation. A resizable ArrayBuffer is an ArrayBuffer whose byte length may change after creation via calls to ArrayBuffer.prototype.resize ( newLength )."[^5] "ArrayBuffer objects can be made resizable by including the maxByteLength option when calling the ArrayBuffer() constructor", and "New bytes are initialized to 0."[^1] A typed array made without a length "will become length-tracking, and will automatically resize to fit the underlying buffer as the latter is resized".[^4] One with a length keeps it, and "When a buffer is shrunk, the viewing typed array may become out of bounds, in which case the typed array's observed size will decrease to 0."[^4] Example: [`resize`](#resize).

**r6.** "The transfer() method of ArrayBuffer instances creates a new ArrayBuffer with the same byte content as this buffer, then detaches this buffer."[^6] The new length may differ: "If newByteLength is larger than the byteLength of this ArrayBuffer , the extra bytes are filled with zeros."[^6] It need not copy, because "implementations may implement this method as a zero-copy move or a realloc".[^6] After it, "byteLength becomes 0 (in both the buffer and the associated typed array views). Methods, such as resize() and slice() , throw a TypeError when invoked."[^1] "You can check whether an ArrayBuffer is detached by its detached property."[^1] Example: [`transfer`](#transfer).

**r7.** A SharedArrayBuffer is the other buffer type. An ArrayBuffer "is always owned by a single execution context at a time"; "A SharedArrayBuffer cannot be transferred (as the buffer is already shared by all execution contexts)", and "SharedArrayBuffer can only be grown but not shrunk."[^2] In Node, "The Buffer class is a subclass of JavaScript's <Uint8Array> class and extends it with methods that cover additional use cases."[^7]

## Examples

A small binary record with fields at byte offsets 0, 1 and 3, little-endian (rule r3).

```javascript run id=record
const buf = new ArrayBuffer(7);
const dv = new DataView(buf);
dv.setUint8(0, 2);
dv.setUint16(1, 500, true);
dv.setFloat32(3, 0.5, true);
console.log(new Uint8Array(buf).join(" "));
console.log(dv.getUint8(0), dv.getUint16(1, true), dv.getFloat32(3, true));
```

```output for=record
2 244 1 0 0 0 63
2 500 0.5
```

A length-tracking view and a fixed view on a resizable buffer (rule r5).

```javascript run id=resize
const buf = new ArrayBuffer(4, { maxByteLength: 8 });
const tracking = new Uint8Array(buf);
const fixed = new Uint8Array(buf, 0, 4);
tracking.fill(1);
buf.resize(6);
console.log(buf.resizable, buf.maxByteLength, tracking.length, fixed.length);
console.log(tracking.join(" "));
buf.resize(2);
console.log(tracking.length, fixed.length);
try {
  buf.resize(9);
} catch (e) {
  console.log(e.name);
}
```

```output for=resize
true 8 6 4
1 1 1 1 0 0
2 0
RangeError
```

Moving the bytes to a new, longer buffer (rule r6).

```javascript run id=transfer
const original = new Uint8Array([1, 2, 3]).buffer;
const view = new Uint8Array(original);
const moved = original.transfer(5);
console.log(original.detached, original.byteLength, view.length);
console.log(moved.byteLength, new Uint8Array(moved).join(" "));
try {
  original.slice(0);
} catch (e) {
  console.log(e.name);
}
```

```output for=transfer
true 0 0
5 1 2 3 0 0
TypeError
```

## Versions

The TC39 list of finished proposals gives Resizable and growable ArrayBuffers and ArrayBuffer transfer the expected publication year 2024, so `resize`, `maxByteLength`, `transfer`, `transferToFixedLength` and `detached` are part of ECMAScript 2024.[^8] Node v24.10.0 has them, as the [`resize`](#resize) and [`transfer`](#transfer) examples show.

## Pitfalls

### Using a view after its buffer was transferred

A view keeps pointing at the old, detached buffer (rule r6). It reads as empty and gives no error, so keep a view of the new buffer instead.

```javascript wrong id=detached
function send(buffer) {
  return buffer.transfer();
}
const bytes = new Uint8Array([7, 8, 9]);
const sent = send(bytes.buffer);
console.log(bytes.length, bytes[0], bytes.buffer.detached);
const kept = new Uint8Array(sent);
console.log(kept.length, kept[0]);
```

```output for=detached
0 undefined true
3 7
```

### A typed array at an odd offset

A multi-byte typed array cannot start at an offset that is not a multiple of its element size (rule r4). A `DataView` can read the same bytes.

```javascript wrong id=align
const buf = new ArrayBuffer(8);
try {
  new Int32Array(buf, 1);
} catch (e) {
  console.log(e.name);
}
const dv = new DataView(buf);
dv.setInt32(1, -2);
console.log(dv.getInt32(1), new Int32Array(buf, 4).length);
```

```output for=align
RangeError
-2 1
```

## Comparison

Python's struct module "converts between Python values and C structs represented as Python bytes objects", and "Compact format strings describe the intended conversions to/from Python values."[^9] JavaScript has no format strings; a `DataView` reads one field per method call, at a byte offset (rule r3). Go's encoding/binary "implements simple translation between numbers and byte sequences", and "Numbers are translated by reading and writing fixed-size values."[^10] In JavaScript the bytes live in an `ArrayBuffer`, and the views give them a type (rule r1). See [[bits-and-bytes]], [[read-a-binary-file]] and [[python/binary-data-record-layouts]].

## References
