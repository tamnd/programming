---
tag: PQAZMT
kind: feature
owner: javascript
slug: text-encoding
name: Text encoding
aliases: [TextEncoder, TextDecoder, encodeInto, UTF-8 encoding in JavaScript, decoding bytes to a string, fatal decoding, stream decoding, WHATWG Encoding Standard]
area: languages/javascript/strings-and-text
level: advanced
summary: >-
  TextEncoder turns a string into UTF-8 bytes in a Uint8Array, and TextDecoder turns bytes in UTF-8 or a legacy encoding back into a string. Both come from the WHATWG Encoding Standard.
grammar: null
spec: {standard: "WHATWG Encoding Standard (Living Standard)", url: "https://encoding.spec.whatwg.org/#api", locator: "7 API; 7.2 Interface TextDecoder; 7.4 Interface TextEncoder"}
see_also: [javascript/unicode-and-utf-16-strings, javascript/strings, javascript/javascript-typed-arrays, encode-and-decode-utf-8, utf-8]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/API/TextEncoder
  title: MDN Web Docs, Web APIs, TextEncoder
  locator: the introduction; Instance methods
  accessed: 2026-11-18
- url: https://encoding.spec.whatwg.org/
  title: WHATWG, Encoding Standard
  locator: 1 Preface; 7.2 Interface TextDecoder; 7.4 Interface TextEncoder
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/API/TextDecoder
  title: MDN Web Docs, Web APIs, TextDecoder
  locator: the introduction; Examples
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/API/TextDecoder/TextDecoder
  title: MDN Web Docs, Web APIs, TextDecoder() constructor
  locator: Parameters; Exceptions
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/API/TextDecoder/decode
  title: MDN Web Docs, Web APIs, TextDecoder.decode()
  locator: Parameters; Exceptions
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/API/TextEncoder/encodeInto
  title: MDN Web Docs, Web APIs, TextEncoder.encodeInto()
  locator: Return value; Buffer sizing
  accessed: 2026-11-18
- url: https://nodejs.org/docs/latest-v24.x/api/util.html
  title: Node.js v24 Documentation, Util
  locator: Class util.TextDecoder; WHATWG supported encodings; Class util.TextEncoder
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/stdtypes.html
  title: The Python Standard Library, Python 3.14, Built-in Types
  locator: str.encode; bytes.decode
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, Class String
  locator: getBytes(Charset)
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const bytes = new TextEncoder().encode("€ and 𠮷");
console.log(bytes.length, [...bytes.subarray(0, 3)]);
console.log(new TextDecoder().decode(bytes));
console.log(new TextEncoder().encoding, new TextDecoder("latin1").encoding);
```

```output for=basics
12 [ 226, 130, 172 ]
€ and 𠮷
utf-8 windows-1252
```

| Call | Result |
|---|---|
| `new TextEncoder().encode(str)` | a `Uint8Array` of UTF-8 bytes |
| `encoder.encodeInto(str, u8)` | `{ read, written }`, bytes put into `u8` |
| `new TextDecoder(label)` | a decoder for `utf-8` (the default) or another encoding |
| `decoder.decode(bytes)` | a string |
| `decoder.decode(chunk, { stream: true })` | a string, with an unfinished sequence kept for the next call |
| `new TextDecoder(label, { fatal: true })` | a decoder that throws on bad bytes |

## Semantics

**r1.** "The TextEncoder interface enables you to encode a JavaScript string using UTF-8."[^1] `encode()` "Takes a string as input, and returns a Uint8Array containing the string encoded using UTF-8."[^1] The standard gives no other choice: "A TextEncoder object offers no label argument as it only supports UTF-8."[^2] Its reason: "The UTF-8 encoding is the most appropriate encoding for interchange of Unicode, the universal coded character set."[^2] Example: [`basics`](#basics).

**r2.** "The TextDecoder interface represents a decoder for a specific text encoding, such as UTF-8 , ISO-8859-2 , or GBK . A decoder takes an array of bytes as input and returns a JavaScript string."[^3] The label "may be any valid label" and "Defaults to "utf-8"".[^4] An unknown label throws a RangeError.[^4] The `encoding` property gives the name the label maps to, so `latin1` gives `windows-1252` in the Synopsis. Example: [`legacy`](#legacy).

**r3.** Bad bytes are replaced by default. The `fatal` option is "A boolean value indicating if the TextDecoder.decode() method must throw a TypeError when decoding invalid data. It defaults to false , which means that the decoder will substitute malformed data with a replacement character."[^4] With `ignoreBOM` false, the default, "the byte order mark will be skipped over when decoding and will not be included in the decoded text."[^4] Example: [`invalid`](#invalid).

**r4.** `stream` is "A boolean flag indicating whether additional data will follow in subsequent calls to decode() . Set to true if processing the data in chunks, and false for the final chunk or if the data is not chunked."[^5] The standard: "The method can be invoked zero or more times with options’s stream set to true, and then once without options’s stream (or set to false), to process a fragmented input."[^2] Example: [`chunks`](#chunks).

**r5.** `encodeInto()` writes into an existing array and returns `read`, "The number of UTF-16 code units from the source that have been converted to UTF-8", and `written`, "The number of bytes modified in the destination Uint8Array . The bytes written are guaranteed to form complete UTF-8 byte sequences."[^6] For a string `s`, "the output space needed for full conversion is never less than s.length bytes and never greater than s.length * 3 bytes."[^6] Example: [`into`](#into).

**r6.** The input of `encode` is a `USVString`, a string of Unicode scalar values, in the interface the standard gives: `Uint8Array encode(optional USVString input = "")`.[^2] A lone surrogate in a JavaScript string is not a scalar value, and the run in [`lone`](#lone) shows it written as the three bytes of U+FFFD. See [[javascript/unicode-and-utf-16-strings]].

**r7.** The Node.js docs give v11.0.0 as the version in which each class was put on the global object.[^7] Its `TextDecoder` is "An implementation of the WHATWG Encoding Standard TextDecoder API", and "All instances of TextEncoder only support UTF-8 encoding."[^7] The decoder's list of encodings depends on the build: "Different Node.js build configurations support different sets of encodings."[^7]

## Examples

A legacy encoding (rule r2).

```javascript run id=legacy
const cp1251 = new Uint8Array([207, 240, 232, 226, 229, 242, 44, 32, 236, 232, 240, 33]);
console.log(new TextDecoder("windows-1251").decode(cp1251));
console.log(new TextDecoder().decode(cp1251).includes("\uFFFD"));
try {
  new TextDecoder("no-such-encoding");
} catch (e) {
  console.log(e.name);
}
```

```output for=legacy
Привет, мир!
true
RangeError
```

Read as UTF-8, the same bytes are not valid and turn into replacement characters.

Bad bytes, `fatal` and the byte order mark (rule r3).

```javascript run id=invalid
const bad = new Uint8Array([0x61, 0xff, 0x62]);
const text = new TextDecoder().decode(bad);
console.log([...text].map((c) => c.codePointAt(0).toString(16)));
try {
  new TextDecoder("utf-8", { fatal: true }).decode(bad);
} catch (e) {
  console.log(e.name);
}
const withBom = new Uint8Array([0xef, 0xbb, 0xbf, 0x68, 0x69]);
console.log(new TextDecoder().decode(withBom).length, new TextDecoder("utf-8", { ignoreBOM: true }).decode(withBom).length);
```

```output for=invalid
[ '61', 'fffd', '62' ]
TypeError
2 3
```

Encoding into a fixed buffer (rule r5).

```javascript run id=into
const buf = new Uint8Array(4);
const result = new TextEncoder().encodeInto("a\u00F1b€", buf);
console.log(result, [...buf]);
```

```output for=into
{ read: 3, written: 4 } [ 97, 195, 177, 98 ]
```

The euro sign needs three more bytes, so it is left out, and `read` says where to go on.

## Pitfalls

### Decoding each chunk with a new decoder

```javascript wrong id=chunks
const euro = new TextEncoder().encode("€");
const chunks = [euro.subarray(0, 2), euro.subarray(2)];
const separate = chunks.map((c) => new TextDecoder().decode(c)).join("");
const decoder = new TextDecoder();
const streamed = chunks.map((c) => decoder.decode(c, { stream: true })).join("") + decoder.decode();
console.log(separate === "€", separate.length, streamed === "€");
```

```output for=chunks
false 2 true
```

A chunk boundary can fall inside a character. Each separate call sees half a sequence and puts out a replacement character. One decoder with `stream: true` keeps the unfinished bytes for the next call (rule r4).

### Expecting a lone surrogate to survive a round trip

```javascript wrong id=lone
const lone = "a\uD800b";
const bytes = new TextEncoder().encode(lone);
console.log([...bytes]);
console.log(new TextDecoder().decode(bytes) === lone);
```

```output for=lone
[ 97, 239, 191, 189, 98 ]
false
```

UTF-8 has no bytes for a lone surrogate, so it comes back as U+FFFD (rule r6). Strings that may hold binary data are not text; keep such data in a `Uint8Array`.

## Comparison

Python's `str.encode` "Return the string encoded to bytes", with `utf-8` as the default, and `bytes.decode` "Return the bytes decoded to a str"; by default "a UnicodeError exception is raised" on bad input.[^8] Java's `String.getBytes(Charset)` "always replaces malformed-input and unmappable-character sequences with this charset's default replacement byte array."[^9] JavaScript decodes with replacement by default and throws only with `fatal` (rule r3), and it encodes only to UTF-8 (rule r1). See [[encode-and-decode-utf-8]].

## References
