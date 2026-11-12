---
tag: T148Q0
kind: concept
slug: string-indexing
name: String indexing
aliases: [string index, "s[i]", character at index, charAt, byte offset, code unit index, code point index, String.Index]
area: data-and-values/strings
level: core
summary: >-
  An index into a string counts bytes, UTF-16 code units or code points, and the choice decides the cost of s[i] and whether a slice can cut a character in half.
applies_to: core-languages
viz: optional
formal: none
see_also: [string-concept, string-memory-layout, characters-and-code-points, grapheme-cluster, split-a-string]
sources:
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, Python documentation
  locator: Text Sequence Type - str
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Index expressions; For statements with range clause
- url: https://go.dev/blog/strings
  title: Rob Pike, Strings, bytes, runes and characters in Go, The Go Blog (2013)
  locator: Printing strings; Range loops
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Class String, Java SE 25 API
  locator: Class description; charAt(int)
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String
  title: String, MDN Web Docs
  locator: UTF-16 characters, Unicode code points, and grapheme clusters
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/strings/
  title: Strings and string literals, C# programming guide
  locator: Introduction
- url: https://doc.rust-lang.org/book/ch08-02-strings.html
  title: The Rust Programming Language, 8.2 Storing UTF-8 Encoded Text with Strings
  locator: Indexing into Strings; Slicing Strings
- url: https://doc.rust-lang.org/std/primitive.str.html
  title: Primitive Type str, The Rust Standard Library
  locator: Index, Panics; is_char_boundary
- url: https://github.com/swiftlang/swift-book/blob/main/TSPL.docc/LanguageGuide/StringsAndCharacters.md
  title: The Swift Programming Language, Strings and Characters
  locator: String Indices
- url: https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/
  title: Language Server Protocol Specification 3.17
  locator: Position
- url: https://peps.python.org/pep-0393/
  title: PEP 393, Flexible String Representation
  locator: Rationale
---

## Idea

In an array of numbers, `a[i]` is the element at position i, and it takes O(1) time. A program expects the same from `s[i]` on a string. But a string is stored as a sequence of code units, and one character can take several units. So each language must choose what `i` counts: bytes, UTF-16 code units or code points. The choice decides what `s[i]` returns, how fast it is, and whether an index can point into the middle of a character.

```python run id=idea
s = "naïve 😀"

# Python indexes code points.
print(len(s), s[2], s[6])

# The same text as UTF-8 bytes and as UTF-16 code units.
utf8 = s.encode("utf-8")
utf16 = s.encode("utf-16-le")
print(len(utf8), len(utf16) // 2)

# Where does each code point start in each unit?
b = u = 0
for cp, ch in enumerate(s):
    print(f"{ch!r}: code point {cp}, byte {b}, UTF-16 unit {u}")
    b += len(ch.encode("utf-8"))
    u += len(ch.encode("utf-16-le")) // 2
```

```output for=idea
7 ï 😀
11 8
'n': code point 0, byte 0, UTF-16 unit 0
'a': code point 1, byte 1, UTF-16 unit 1
'ï': code point 2, byte 2, UTF-16 unit 2
'v': code point 3, byte 4, UTF-16 unit 3
'e': code point 4, byte 5, UTF-16 unit 4
' ': code point 5, byte 6, UTF-16 unit 5
'😀': code point 6, byte 7, UTF-16 unit 6
```

The text has 7 code points, 11 UTF-8 bytes and 8 UTF-16 code units. The three indexes agree for `n` and `a`. After `ï`, which takes 2 bytes, the byte index is one ahead. The emoji is code point 6, byte 7 and UTF-16 unit 6, and it takes 4 bytes and 2 code units. So "index 6" means a different place in Python, in Go and in Java.

## How it works

**Three kinds of index.**

- *Code points.* In Python, a string is a sequence of code points, and "indexing a string produces strings of length 1"[^1]. `s[6]` is the emoji.
- *Bytes.* In Go, `s[i]` on a string "is the non-constant byte value at index x and the type of a[x] is byte"[^2]. The Go blog says it plainly: "indexing a string accesses individual bytes, not characters"[^3]. C and C++ also index bytes when the text is UTF-8.
- *UTF-16 code units.* In Java, "Index values refer to char code units, so a supplementary character uses two positions in a String"[^4]. MDN says the same for JavaScript: "String indexes also refer to the index of each UTF-16 code unit"[^5]. C# counts `char` values in the same way, and its `Length` property is "the number of Char objects it contains, not the number of Unicode characters"[^6].

**Rust refuses `s[i]`.** A Rust `str` has no integer index, and `s[0]` does not compile. The Rust book gives two reasons. First, it is "not clear what the return type of the string-indexing operation should be: a byte value, a character, a grapheme cluster, or a string slice"[^7]. Second, "indexing operations are expected to always take constant time (O(1))"[^7], but a character index in UTF-8 needs a scan from the start. Rust allows a range of byte offsets, such as `&s[0..4]`, but it panics "if begin or end does not point to the starting byte offset of a character (as defined by is_char_boundary)"[^8]. The method `get(0..3)` returns `None` instead of a panic.

**Swift uses opaque indexes.** In Swift, each string has an associated index type, `String.Index`, and an index gives the position of one `Character`[^9]. Because characters have different sizes, to find the character at a position, "you must iterate over each Unicode scalar from the start or end of that" string, and "For this reason, Swift strings can't be indexed by integer values"[^9]. A program moves an index with `index(after:)` and `index(_:offsetBy:)`, and the cost of the move is visible in the code.

**The cost of each kind.** A byte index or a code unit index is O(1), because each unit has a fixed size. A code point index is O(1) only if each code point has the same width. CPython makes this true: it stores each string with 1, 2 or 4 bytes per code point (see [how strings are stored](/string-memory-layout/)). In UTF-8 or UTF-16, the code point at index i needs a scan of the units before it, so it is O(n). Go `[]rune(s)` and Java `codePoints()` make an array of code points once, and after that each index is O(1).

**Boundaries.** A byte index or a code unit index can point into the middle of a character. In UTF-8 the bytes of the form `10xxxxxx` continue a character, so a valid start is any other byte. In UTF-16 a low surrogate continues a character. A slice that starts or ends at a continuation unit cuts the character in half. Rust and Swift check this. Go, Java, JavaScript, C# and C++ do not, and they return a string that is not valid text.

**Iteration instead of an index.** Most code that seems to need `s[i]` reads the characters in order. An iterator decodes each character once, so the whole loop is O(n). In Go, the `range` clause on a string "iterates over the Unicode code points in the string starting at byte index 0", and each step gives "the index of the first byte of successive UTF-8-encoded code points in the string"[^2]. Rust `char_indices` gives the same pairs. See iterate over the characters of a string.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `s[i]` by code point, O(1); negative indexes; slices are cut to fit |
| JavaScript | native | `s[i]`, `charAt` and `at` by UTF-16 code unit; `codePointAt`; `[...s]` for code points |
| TypeScript | native | the JavaScript rules; `s[i]` has the type `string` |
| Java | native | `charAt(i)` by UTF-16 code unit; `codePointAt`, `offsetByCodePoints` |
| Kotlin | native | `s[i]` and `get(i)` give a UTF-16 `Char`; `codePointAt` on the JVM |
| C# | native | `s[i]` by UTF-16 code unit; `^i` from the end; `StringInfo` for text elements |
| Go | native | `s[i]` gives a byte; `range` gives byte offsets and runes; `[]rune(s)` |
| Rust | native | no `s[i]`; byte ranges on character boundaries; `chars().nth(i)`, `char_indices` |
| C | idiom | `s[i]` is a `char`, one byte; no bounds check |
| C++ | stdlib | `s[i]` and `s.at(i)` give a `char`; `at` checks the bounds |
| Bash | native | `${s:i:n}` counts characters in the current locale, bytes in the C locale |
| PowerShell | native | `$s[i]` gives a .NET `char`; negative indexes count from the end |
| SQL | native | `SUBSTRING(s FROM i FOR n)` from position 1; characters in the column's character set |
| Regex | n/a | a match reports offsets in the units of the host language |
| HTML | n/a | no indexes in markup |
| CSS | n/a | no string indexes |

The general-purpose languages split into three groups: Python counts code points, Go, C and C++ count bytes, and Java, JavaScript, C# and Kotlin count UTF-16 code units. Rust and Swift stand apart, because they do not let an integer pick a character at all.

The example files show each rule. Python shows negative indexes, `IndexError` and a slice that is cut to fit, a combining mark that is a separate code point, and a byte offset that turns into a code point index. JavaScript reads half of a surrogate pair with `s[6]`, a whole code point with `codePointAt`, and graphemes with `Intl.Segmenter`. Java converts a code point index to a `char` index with `offsetByCodePoints` and cuts a surrogate pair with `substring`. C# reads a surrogate pair with `char.ConvertToUtf32`, indexes from the end and counts text elements with `StringInfo`. Go shows that `s[2]` is a byte, that `range` gives byte offsets, and that `s[:3]` is not valid UTF-8. Rust uses `as_bytes`, `is_char_boundary`, `get` and `char_indices`. C and C++ find the byte where a code point starts, and C++ `at` throws `std::out_of_range`.

## Variations and related ideas

- **Grapheme indexes.** A user sees `é` or a flag as one character, but it can be several code points. Swift `Character`, C# `StringInfo` and JavaScript `Intl.Segmenter` work with [grapheme clusters](/grapheme-cluster/). An index by grapheme is always O(n).
- **Offsets in protocols.** A protocol that sends positions in text must name the unit. The Language Server Protocol used UTF-16: "Prior to 3.17 the offsets were always based on a UTF-16 string representation"[^10]. In version 3.17, "clients and servers can agree on a different string encoding representation (e.g. UTF-8)"[^10], and the client lists the encodings that it supports in `general.positionEncodings`.
- **Byte offsets as indexes.** Rust, Go and many parsers keep byte offsets, which are O(1) and stable, and they turn them into line and column numbers only for messages.
- **A fixed width for O(1) access.** UTF-32 arrays, Go `[]rune` and CPython's flexible layout make the code point index O(1), at a cost in memory.

## Pitfalls

- **Half a surrogate pair.** In Java, JavaScript and C#, `s.substring(0, 7)` or `s.slice(0, 7)` can end between the two halves of an emoji. The result is a lone surrogate, and an encoder replaces it with `?` or U+FFFD.
- **Half a UTF-8 sequence.** In Go and C++, `s[:3]` on `"naïve"` keeps only the first byte of `ï`. Find a boundary first, with `utf8.RuneStart` in Go or `is_char_boundary` in Rust.
- **O(n²) loops.** `for i in range(n): s.chars().nth(i)` in Rust, or a loop over `offsetByCodePoints` in Java, scans from the start each time. Iterate instead.
- **An index from another language.** A JavaScript front end that sends `s.indexOf("😀")` to a Go server sends a UTF-16 offset, and Go reads it as a byte offset. Convert at the boundary, or send the text.
- **Code points are not characters.** Even a code point index cuts `e` from its combining accent `U+0301`. For text that a user edits, use grapheme boundaries.

## Cost

A byte or code unit index is O(1) and reads one unit of memory. A code point index is O(1) only in a fixed-width layout. In UTF-8 and UTF-16 it is O(n) for one lookup, and a loop of n lookups is O(n²). An array of code points costs O(n) time and 4 bytes per code point once, and after that each lookup is O(1). A grapheme index needs the segmentation rules of Unicode, so it is O(n) and slower than a code point scan.

## History

Early string types had one byte per character, so a byte index and a character index were the same. The 16-bit units of Java, JavaScript and C# were also one unit per character until Unicode grew past 65,536 code points, and then an index could fall between the two halves of a surrogate pair. CPython had the same problem with its narrow builds: "on systems only supporting UTF-16, users complain that non-BMP characters are not properly supported"[^11]. PEP 393 fixed it with a flexible width, and "The distinction between narrow and wide Unicode builds is dropped"[^11]. Rust and Swift, which came later, chose not to offer an integer index into text at all.

## References
