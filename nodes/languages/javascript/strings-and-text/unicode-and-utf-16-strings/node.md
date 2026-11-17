---
tag: PPSHM6
kind: feature
owner: javascript
slug: unicode-and-utf-16-strings
name: Unicode and UTF-16 strings
aliases: [UTF-16 in JavaScript, surrogate pairs, lone surrogates, code units and code points, codePointAt, String.fromCodePoint, isWellFormed, toWellFormed]
area: languages/javascript/strings-and-text
level: core
summary: >-
  JavaScript strings hold UTF-16 code units. A character above U+FFFF takes a surrogate pair, so length counts units. for-of and codePointAt work by code point; isWellFormed finds lone surrogates.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-language-types-string-type", locator: "6.1.4 The String Type"}
see_also: [javascript/strings, javascript/string-escape-sequences, javascript/string-methods, grapheme-cluster, get-the-code-point-of-a-character, python/unicode-in-python]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String
  title: MDN Web Docs, JavaScript Reference, String
  locator: UTF-16 characters, Unicode code points, and grapheme clusters
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 6
  locator: 6.1.4 The String Type
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/codePointAt
  title: MDN Web Docs, JavaScript Reference, String.prototype.codePointAt()
  locator: the introduction; Return value; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/fromCodePoint
  title: MDN Web Docs, JavaScript Reference, String.fromCodePoint()
  locator: the introduction; Exceptions
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/isWellFormed
  title: MDN Web Docs, JavaScript Reference, String.prototype.isWellFormed()
  locator: the introduction; Return value
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/toWellFormed
  title: MDN Web Docs, JavaScript Reference, String.prototype.toWellFormed()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Well-Formed Unicode Strings, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/stdtypes.html
  title: The Python Standard Library, Python 3.14, Built-in Types
  locator: Text Sequence Type, str
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, Class String
  locator: the class description
  accessed: 2026-11-18
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements with range clause
  accessed: 2026-11-18
- url: https://doc.rust-lang.org/std/string/struct.String.html
  title: The Rust Standard Library, Struct String
  locator: UTF-8
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const s = "a😀";
console.log(s.length, [...s].length);
console.log(s.charCodeAt(1).toString(16), s.charCodeAt(2).toString(16));
console.log(s.codePointAt(1).toString(16), s.codePointAt(2).toString(16));
console.log(String.fromCodePoint(0x1f600) === "😀", String.fromCharCode(0xd83d, 0xde00) === "😀");
```

```output for=basics
3 2
d83d de00
1f600 de00
true true
```

| Works on | Operations |
|---|---|
| UTF-16 code units | `length`, `s[i]`, `charAt`, `charCodeAt`, `slice`, `split("")`, `String.fromCharCode` |
| code points | `for...of`, `[...s]`, `Array.from(s)`, `codePointAt`, `String.fromCodePoint`, `\u{...}` |
| well-formedness | `isWellFormed`, `toWellFormed` |

## Semantics

**r1.** "Strings are represented fundamentally as sequences of UTF-16 code units. In UTF-16 encoding, every code unit is exact 16 bits long."[^1] The specification says that "Operations that do interpret String values treat each element as a single UTF-16 code unit."[^2] So `length` and the index of `s[i]` count code units. Example: [`basics`](#basics).

**r2.** "The extra characters are stored in UTF-16 as surrogate pairs, which are pairs of 16-bit code units that represent a single character."[^1] Leading surrogates "have values between 0xD800 and 0xDBFF , inclusive, while trailing surrogates, also called low-surrogate code units, have values between 0xDC00 and 0xDFFF , inclusive."[^1] "Each Unicode character, comprised of one or two UTF-16 code units, is also called a Unicode code point."[^1]

**r3.** Some operations read code points. "split("") will split by UTF-16 code units and will separate surrogate pairs. String indexes also refer to the index of each UTF-16 code unit. On the other hand, [Symbol.iterator]() iterates by Unicode code points."[^1] So `for...of`, spread and `Array.from` give one element per code point. Example: [`iterate`](#iterate).

**r4.** `codePointAt` "returns a non-negative integer that is the Unicode code point value of the character starting at the given index. Note that the index is still based on UTF-16 code units, not Unicode code points."[^3] "If the element at index is a UTF-16 leading surrogate, returns the code point of the surrogate pair. If the element at index is a UTF-16 trailing surrogate, returns only the trailing surrogate code unit."[^3] In the other direction, `String.fromCodePoint` "returns a string created from the specified sequence of code points", and throws a RangeError if a number "is not an integer, is less than 0 , or is greater than 0x10FFFF".[^4]

**r5.** The specification lets a string hold any code units: "ECMAScript does not restrict the value of or relationships between these code units".[^2] "Lone surrogates do not represent any Unicode character."[^1] `isWellFormed` "returns a boolean indicating whether this string contains any lone surrogates", and `toWellFormed` "returns a string where all lone surrogates of this string are replaced with the Unicode replacement character U+FFFD."[^5][^6] This matters at the edges of the program: "encodeURI() will throw a URIError for lone surrogates, because URI encoding uses UTF-8 encoding, which does not have any encoding for lone surrogates."[^1] Example: [`wellformed`](#wellformed).

**r6.** A code point is not always what a reader sees as one character. "On top of Unicode characters, there are certain sequences of Unicode characters that should be treated as one visual unit, known as a grapheme cluster."[^1] MDN adds that "Iterating through grapheme clusters will require some custom code."[^1] See [[grapheme-cluster]].

## Examples

Code units and code points of the same string (rule r3).

```javascript run id=iterate
const word = "né😀";
console.log(word.length, word.split("").length, Array.from(word).length);
for (const ch of word) {
  console.log(ch, ch.codePointAt(0).toString(16).padStart(4, "0"));
}
```

```output for=iterate
4 4 3
n 006e
é 00e9
😀 1f600
```

Finding and fixing a lone surrogate (rule r5).

```javascript run id=wellformed
const broken = "ab\ud800";
console.log(broken.isWellFormed(), "ab😀".isWellFormed());
const fixed = broken.toWellFormed();
console.log(fixed.length, fixed.codePointAt(2).toString(16));
try {
  encodeURI(broken);
} catch (e) {
  console.log(e.name);
}
console.log(encodeURI(fixed));
```

```output for=wellformed
false true
3 fffd
URIError
ab%EF%BF%BD
```

## Versions

`isWellFormed` and `toWellFormed` come from the Well-Formed Unicode Strings proposal, which the TC39 list of finished proposals gives the expected publication year 2024, so they are part of ECMAScript 2024.[^7]

## Pitfalls

### Cutting a string in the middle of a pair

`slice` counts code units (rule r1), so it can keep the leading surrogate and drop the trailing one.

```javascript wrong id=cut
const text = "hi 😀!";
const cut = text.slice(0, 4);
console.log(cut.length, cut.isWellFormed());

const safe = [...text].slice(0, 4).join("");
console.log(safe, safe.isWellFormed());
```

```output for=cut
4 false
hi 😀 true
```

### Reversing a string with `split("")`

```javascript wrong id=reverse
const s = "a😀b";
const bad = s.split("").reverse().join("");
console.log(bad.length, bad.isWellFormed());

const good = [...s].reverse().join("");
console.log(good, good.isWellFormed());
```

```output for=reverse
4 false
b😀a true
```

The pair comes out in the wrong order, as two lone surrogates. Spread keeps each code point whole, but it still splits a grapheme cluster made of several code points (rule r6).

## Comparison

Python's strings "are immutable sequences of Unicode code points", so `len` counts code points.[^8] Java uses the same model as JavaScript: "A String represents a string in the UTF-16 format in which supplementary characters are represented by surrogate pairs", and "Index values refer to char code units".[^9] Go strings hold bytes, and a `for` loop with `range` over a string "iterates over the Unicode code points in the string starting at byte index 0."[^10] In Rust, "strings are required to be valid UTF-8".[^11] A JavaScript string can hold a lone surrogate, which is why `isWellFormed` exists (rule r5). See [[python/unicode-in-python]] and [[get-the-code-point-of-a-character]].

## References
