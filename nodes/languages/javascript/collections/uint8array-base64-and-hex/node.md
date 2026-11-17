---
tag: 721SVQ
kind: feature
owner: javascript
slug: uint8array-base64-and-hex
name: Uint8Array base64 and hex
aliases: [Uint8Array.fromBase64, Uint8Array.prototype.toBase64, Uint8Array.fromHex, Uint8Array.prototype.toHex, setFromBase64, setFromHex, base64url in JavaScript]
area: languages/javascript/collections
level: advanced
summary: >-
  Uint8Array.fromBase64 and fromHex decode a string into bytes, and toBase64 and toHex encode bytes as a string. They are in ECMAScript 2026; Node v24 has Buffer encodings instead.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/indexed-collections.html#sec-uint8array-objects", locator: "23.3 Uint8Array Objects; 23.3.1.1 Uint8Array.fromBase64; 23.3.1.2 Uint8Array.fromHex; 23.3.2 Properties of the Uint8Array Prototype Object"}
see_also: [javascript/javascript-typed-arrays, javascript/arraybuffer-and-views, base64-encode-and-decode, convert-a-string-to-bytes, python/bytes-and-bytearray]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Uint8Array/fromBase64
  title: MDN Web Docs, JavaScript Reference, Uint8Array.fromBase64()
  locator: the introduction; Parameters; Exceptions
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Uint8Array/toBase64
  title: MDN Web Docs, JavaScript Reference, Uint8Array.prototype.toBase64()
  locator: the introduction; Parameters; Examples
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/indexed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 23
  locator: 23.3.1.1 Uint8Array.fromBase64; 23.3.1.2 Uint8Array.fromHex; 23.3.2.1 to 23.3.2.4
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Uint8Array/fromHex
  title: MDN Web Docs, JavaScript Reference, Uint8Array.fromHex()
  locator: the introduction; Parameters; Exceptions
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Uint8Array/toHex
  title: MDN Web Docs, JavaScript Reference, Uint8Array.prototype.toHex()
  locator: the introduction; Examples
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Uint8Array/setFromBase64
  title: MDN Web Docs, JavaScript Reference, Uint8Array.prototype.setFromBase64()
  locator: the introduction; Return value
  accessed: 2026-11-18
- url: https://nodejs.org/docs/latest-v24.x/api/buffer.html
  title: Node.js v24 documentation, Buffer
  locator: Buffers and character encodings
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Uint8Array to/from Base64, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/base64.html
  title: The Python Standard Library, Python 3.14, base64
  locator: the introduction
  accessed: 2026-11-18
- url: https://pkg.go.dev/encoding/base64
  title: Go Packages, encoding/base64
  locator: Overview
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Base64.html
  title: Java SE 25 API, Class Base64
  locator: the class description
  accessed: 2026-11-18
---

## Synopsis

The blocks on this page call the ECMAScript 2026 methods when they exist and fall back to Node's `Buffer` when they do not, so they print the same lines in both cases. The bytes are the ones in MDN's examples.

```javascript run id=encode
const bytes = new Uint8Array([29, 233, 101, 161]);
const hasNative = typeof bytes.toBase64 === "function";

const b64 = hasNative ? bytes.toBase64() : Buffer.from(bytes).toString("base64");
const url = hasNative
  ? bytes.toBase64({ alphabet: "base64url", omitPadding: true })
  : Buffer.from(bytes).toString("base64url");
const hex = hasNative
  ? new Uint8Array([202, 254, 208, 13]).toHex()
  : Buffer.from([202, 254, 208, 13]).toString("hex");
console.log(b64, url, hex);

const back = hasNative ? Uint8Array.fromBase64(b64) : new Uint8Array(Buffer.from(b64, "base64"));
console.log(back);
```

```output for=encode
HelloQ== HelloQ cafed00d
Uint8Array(4) [ 29, 233, 101, 161 ]
```

| Method | Direction | Options |
|---|---|---|
| `Uint8Array.fromBase64(str, opts?)` | base64 string to a new `Uint8Array` | `alphabet`, `lastChunkHandling` |
| `u8.setFromBase64(str, opts?)` | base64 string into an existing `Uint8Array` | the same; returns `{ read, written }` |
| `u8.toBase64(opts?)` | bytes to a base64 string | `alphabet`, `omitPadding` |
| `Uint8Array.fromHex(str)` | hex string to a new `Uint8Array` | none |
| `u8.setFromHex(str)` | hex string into an existing `Uint8Array` | none |
| `u8.toHex()` | bytes to a hex string | none |

## Semantics

**r1.** "The Uint8Array.fromBase64() static method creates a new Uint8Array object from a base64-encoded string."[^1] "The toBase64() method of Uint8Array instances returns a base64-encoded string based on the data in this Uint8Array object."[^2] The specification puts these methods, and the hex ones, on `Uint8Array` and on `Uint8Array.prototype` only, in clause 23.3; the other typed arrays do not have them.[^3]

**r2.** Hex works on whole bytes: `fromHex` "creates a new Uint8Array object from a hexadecimal string", and the string must "Have an even number of characters because two characters encode one byte."[^4] It is "case-insensitive" and must "Not contain whitespace".[^4] `toHex` "returns a hex-encoded string based on the data in this Uint8Array object."[^5]

**r3.** Base64 has two alphabets. The string may use "two special characters, which are either + and / (if using alphabet: "base64" in options ) or - and _ (if using alphabet: "base64url" in options )", and "Any ASCII white space characters within the string are ignored."[^1] On the encoding side, `omitPadding` is "A boolean specifying whether to omit padding characters (= ) at the end of the base64 string. The default is false ."[^2] Example: [`encode`](#encode).

**r4.** The decoders are strict about bad input. `fromBase64` throws a SyntaxError "if the input string contains characters outside the specified alphabet, or if the last chunk does not satisfy the lastChunkHandling option."[^1] `fromHex` throws a SyntaxError "if the input string contains characters outside the hex alphabet, or its length is odd."[^4] The `lastChunkHandling` option is `loose` by default, `strict` requires padding, and `stop-before-partial` "is useful if the string is coming from a stream and the last chunk is not yet complete."[^1]

**r5.** `setFromBase64` "populates this Uint8Array object with bytes from a base64-encoded string, returning an object indicating how many bytes were read and written", and "This method is most suitable for populating a pre-allocated array buffer."[^6]

**r6.** MDN prefers these methods to the string functions `atob` and `btoa`. `fromBase64` "should be preferred over Window.atob() because it results in a byte array, which is easier to work with than a string containing raw bytes, unless your decoded binary data is actually intended to be ASCII text."[^1] Node's `Buffer`, a subclass of `Uint8Array`, has its own encodings: `'hex'` will "Encode each byte as two hexadecimal characters", and `'base64url'`, "When encoding a Buffer to a string, this encoding will omit padding."[^7] Example: [`atob`](#atob).

## Examples

Hex without `toHex` or `Buffer`, as MDN suggests for single numbers: "To convert individual numbers into hex, use the Number.prototype.toString() method with radix set to 16 instead."[^5]

```javascript run id=by-hand
const bytes = new Uint8Array([202, 254, 208, 13, 5]);
const hex = Array.from(bytes, (b) => b.toString(16).padStart(2, "0")).join("");
console.log(hex);
const pairs = hex.match(/../g).map((h) => parseInt(h, 16));
console.log(new Uint8Array(pairs));
```

```output for=by-hand
cafed00d05
Uint8Array(5) [ 202, 254, 208, 13, 5 ]
```

`padStart` keeps the leading zero of `05`.

## Versions

The TC39 list of finished proposals gives Uint8Array to/from Base64 the expected publication year 2026, so the six methods are part of ECMAScript 2026.[^8] Node v24.10.0 does not have them:

```javascript run id=detect
console.log(typeof Uint8Array.fromBase64, typeof Uint8Array.prototype.toHex);
```

```output for=detect
undefined undefined
```

## Pitfalls

### A lenient hex decoder

Node's `'hex'` decoding stops at the first bad pair: "Data truncation may occur when decoding strings that do not exclusively consist of an even number of hexadecimal characters."[^7] `fromHex` throws instead (rule r4). The `strictHex` function below uses `fromHex` when it exists and checks the string itself when it does not.

```javascript wrong id=lenient
console.log(Buffer.from("cafe!d00d", "hex"), Buffer.from("abc", "hex"));

function strictHex(s) {
  if (typeof Uint8Array.fromHex === "function") return Uint8Array.fromHex(s);
  if (s.length % 2 !== 0 || /[^0-9a-f]/i.test(s)) throw new SyntaxError("not hex");
  return new Uint8Array(Buffer.from(s, "hex"));
}
for (const s of ["cafe!d00d", "abc", "CAFEd00d"]) {
  try {
    console.log(strictHex(s));
  } catch (e) {
    console.log(e.name);
  }
}
```

```output for=lenient
<Buffer ca fe> <Buffer ab>
SyntaxError
SyntaxError
Uint8Array(4) [ 202, 254, 208, 13 ]
```

### Reading `atob` output as text

`atob` returns one character per byte, so UTF-8 text comes back as separate characters (rule r6). Turn the characters into bytes, then decode the bytes as UTF-8.

```javascript wrong id=atob
const raw = atob("4pyT");
console.log(raw.length, raw === "✓");
const bytes = Uint8Array.from(raw, (c) => c.charCodeAt(0));
console.log(bytes, new TextDecoder().decode(bytes) === "✓");
```

```output for=atob
3 false
Uint8Array(3) [ 226, 156, 147 ] true
```

With `Uint8Array.fromBase64` the first two steps become one call.

## Comparison

Python's base64 module "provides functions for encoding binary data to printable ASCII characters and decoding such encodings back to binary data."[^9] Go's encoding/base64 "implements base64 encoding as specified by RFC 4648."[^10] Java's `Base64` class "consists exclusively of static methods for obtaining encoders and decoders for the Base64 encoding scheme", and its basic decoder "rejects data that contains characters outside the base64 alphabet."[^11] In JavaScript the codecs are methods of `Uint8Array` itself (rule r1), and the alphabet is an option, not a separate encoder object. See [[base64-encode-and-decode]] and [[convert-a-string-to-bytes]].

## References
