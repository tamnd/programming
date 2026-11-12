---
tag: P3D8VD
kind: concept
slug: string-memory-layout
name: How strings are stored
aliases: [string representation, string layout, null-terminated string, length-prefixed string, Pascal string, small string optimization, SSO, compact strings]
area: data-and-values/strings
level: core
summary: >-
  A string is stored as a block of code units with a terminator or a stored length. The encoding of the units and the small string optimization change its size and speed.
applies_to: core-languages
viz: optional
formal: none
history:
  people: [Dennis Ritchie]
  note: PDP-11 and PDP-10 assemblers produced null-terminated strings before C, and Ritchie chose the same convention for C. Pascal dialects stored the length in the first byte. Later runtimes chose a stored length and a compact encoding, for example Python with PEP 393 and Java with JEP 254.
see_also: [string-concept, string-indexing, utf-8, characters-and-code-points, rope]
same_as:
  wikidata: Q184754
  wikipedia: String_(computer_science)
sources:
- url: https://en.cppreference.com/w/c/string/byte.html
  title: Null-terminated byte strings, C reference, cppreference.com
  locator: Introduction
- url: https://en.wikipedia.org/wiki/String_(computer_science)
  title: String (computer science), Wikipedia
  locator: Representations; Null-terminated; Length-prefixed
- url: https://pkg.go.dev/reflect#StringHeader
  title: Package reflect, Go standard library
  locator: type StringHeader
- url: https://pkg.go.dev/unsafe#StringData
  title: Package unsafe, Go standard library
  locator: func StringData
- url: https://doc.rust-lang.org/std/string/struct.String.html
  title: Struct String, The Rust Standard Library
  locator: Representation
- url: https://en.cppreference.com/w/cpp/string/basic_string.html
  title: std::basic_string, C++ reference, cppreference.com
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string?view=net-10.0
  title: String Class, .NET API reference
  locator: Remarks, Embedded null characters
- url: https://www.swift.org/blog/utf8-string/
  title: Michael Ilseman, UTF-8 String, Swift.org blog (2019)
  locator: Introduction; Unified Storage Representation
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Class String, Java SE 25 API
  locator: Class description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String
  title: String, MDN Web Docs
  locator: UTF-16 characters, Unicode code points, and grapheme clusters
- url: https://peps.python.org/pep-0393/
  title: PEP 393, Flexible String Representation
  locator: Abstract; Specification
- url: https://openjdk.org/jeps/254
  title: JEP 254, Compact Strings
  locator: Summary; Motivation; Description
- url: https://devblogs.microsoft.com/oldnewthing/20230803-00/?p=108532
  title: Raymond Chen, Inside STL, The string, The Old New Thing (2023)
  locator: The small string optimization; the gcc, Microsoft and clang layouts
- url: https://en.cppreference.com/w/cpp/language/types.html
  title: Fundamental types, C++ reference, cppreference.com
  locator: Character types, wchar_t
- url: https://en.wikipedia.org/wiki/Null-terminated_string
  title: Null-terminated string, Wikipedia
  locator: History
---

## Idea

A string is text, but in memory it is a block of numbers. The program must know two things about the block: where it ends, and how each number maps to a character. C marks the end with a byte of value zero. Most other languages store the length next to the characters. For the second question, each runtime picks an encoding: bytes of UTF-8, 16-bit units of UTF-16, or a width that fits the widest character of the string.

```python run id=idea
import sys

# CPython stores each string with 1, 2 or 4 bytes per code point (PEP 393).
# The widest code point in the string decides the width.
for ch in ["a", "é", "€", "😀"]:
    per_char = sys.getsizeof(ch * 101) - sys.getsizeof(ch * 100)
    print(f"U+{ord(ch):04X} {per_char} byte(s) per code point")

# One wide code point makes the whole string wide.
narrow = "a" * 1000
wide = "a" * 999 + "😀"
print(len(narrow), len(wide), sys.getsizeof(wide) - sys.getsizeof(narrow) > 2000)
```

```output for=idea
U+0061 1 byte(s) per code point
U+00E9 1 byte(s) per code point
U+20AC 2 byte(s) per code point
U+1F600 4 byte(s) per code point
1000 1000 True
```

Each extra character adds 1 byte when all code points are below U+0100, 2 bytes when one of them needs 16 bits, and 4 bytes when one is above U+FFFF. Both long strings have 1000 code points. But one emoji makes CPython store all 1000 of them in 4 bytes each, so the second string takes about 3000 bytes more than the first.

## How it works

**The end of the string.** There are two main layouts.

- *A terminator.* "A null-terminated byte string (NTBS) is a sequence of nonzero bytes followed by a byte with value zero (the terminating null character)"[^1]. Wikipedia notes that "This representation of an n-character string takes n + 1 space"[^2]. The cost is that `strlen` must scan every byte to find the length, and that the string cannot contain a byte of value zero.
- *A stored length.* "The length of a string can also be stored explicitly, for example by prefixing the string with the length as a byte value"[^2]. Many Pascal dialects did this, and "Storing the string length as byte limits the maximum string length to 255"[^2]. Modern runtimes store the length in a full machine word.

**A header and a buffer.** Most languages keep a small fixed-size header and put the characters in a buffer on the heap.

- Go describes its string header as a structure with the fields `Data uintptr` and `Len int`, and says that "StringHeader is the runtime representation of a string"[^3]. The header has no capacity, because a Go string cannot grow. `unsafe.StringData` "returns a pointer to the underlying bytes of str"[^4].
- In Rust, "A String is made up of three components: a pointer to some bytes, a length, and a capacity", and "This buffer is always stored on the heap"[^5]. A `&str` has only the pointer and the length.
- A C++ `std::string` stores its characters contiguously, and "*(s.begin() + s.size()) has value CharT() (a null terminator)"[^6]. So it has both a stored length and a terminator, and `c_str()` can give the buffer to a C function with no copy.
- In .NET, "a String object can include embedded null characters, which count as a part of the string's length"[^7].

**Slices.** A header with a pointer and a length can point into the middle of another string's buffer. Go substrings and Rust `&str` slices work this way, so a substring costs no copy. The slice keeps the whole buffer alive, so a small substring of a large string can hold a lot of memory.

**The encoding of the units.** The encoding decides how many bytes each character takes.

- *UTF-8.* Go, Rust and C++ `std::string` usually hold UTF-8, where a code point takes 1 to 4 bytes. Swift also uses it: "Swift 5 switches the preferred encoding of strings from UTF-16 to UTF-8 while preserving efficient Objective-C-interoperability"[^8].
- *UTF-16.* "A String represents a string in the UTF-16 format in which supplementary characters are represented by surrogate pairs"[^9] in Java, and JavaScript "Strings are represented fundamentally as sequences of UTF-16 code units"[^10]. C# works in the same way. A code point takes 2 or 4 bytes.
- *A width per string.* With PEP 393, the CPython string type has "multiple internal representations, depending on the character with the largest Unicode ordinal (1, 2, or 4 bytes)"[^11]. So the index of a code point is still O(1), but one wide character makes the whole string wide.
- *Latin-1 when possible.* Java's compact strings store "characters encoded either as ISO-8859-1/Latin-1 (one byte per character), or as UTF-16 (two bytes per character), based upon the contents of the string"[^12]. The reason was that "most String objects contain only Latin-1 characters"[^12]. The API still shows UTF-16, so `length()` and `charAt` do not change.

**The small string optimization.** Many strings are short: names, keys and words. A C++ library can keep the characters of a short string inside the string object, so it needs no heap allocation. Raymond Chen explains that "the small string optimization means that an empty string can be created without any heap allocations"[^13]. The three main C++ libraries differ. For gcc, the internal buffer "holds as many characters as fit into 16 bytes (minus 1, for the null terminator)"[^13]. The Microsoft library also uses 16 bytes. The clang library uses one bit to tell the two layouts apart, which "lets clang shrink a std::string down to 24 bytes on 64-bit systems, while still allowing 8-bit strings up to 22 characters and 16-bit strings up to 10 characters to be treated as short"[^13]. Swift small strings also "pack their contents directly in the String struct"[^8], so they need no allocation.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | object header, stored length, 1, 2 or 4 bytes per code point (PEP 393) |
| JavaScript | native | UTF-16 code units in the language; the engine chooses the internal layout |
| TypeScript | native | the JavaScript layout; types do not change the representation |
| Java | native | `byte[]` with a coder flag: Latin-1 or UTF-16 (compact strings) |
| Kotlin | native | `java.lang.String` on the JVM, with the same compact layout |
| C# | native | object with a stored length and UTF-16 `char` values; null characters allowed inside |
| Go | native | header of pointer and length, 16 bytes on 64-bit systems; UTF-8 bytes; substrings share bytes |
| Rust | native | `String`: pointer, capacity and length; `&str`: pointer and length; UTF-8 bytes |
| C | idiom | `char` array that ends with a null byte; the length is not stored |
| C++ | stdlib | `std::string`: stored size, capacity and a null terminator; small string optimization |
| Bash | native | C strings inside the shell, so a variable cannot hold a null byte |
| PowerShell | native | the .NET `System.String` layout |
| SQL | native | each database has its own layout: fixed `CHAR(n)` with padding and `VARCHAR(n)` with a length |
| Regex | n/a | a regex engine reads the strings of its host language |
| HTML | n/a | the browser stores text nodes as DOM strings, which are UTF-16 |
| CSS | n/a | the stylesheet is text, and the engine decides the storage |

The languages fall into three groups. C keeps no length. Go, Rust and C++ keep a length in a small header and UTF-8 bytes in a buffer. Java, JavaScript and C# keep UTF-16 code units, and Java and CPython save memory with a narrower width when the text allows it.

The example files make the layouts visible. Python compares the size of one text in Latin-1, UTF-8, UTF-16 and UTF-32, and shows a null character inside a `str`. JavaScript shows a surrogate pair and a lone surrogate that is not well-formed text. Java and C# count the bytes of each encoding and store a null `char` in a string. Go shows the 16-byte header and a substring that shares the bytes of its parent. Rust shows the sizes of `String`, `&str` and `Box<str>`, the capacity, and a slice at an offset in the same buffer. C compares a null-terminated array with a length-prefixed structure. C++ finds the characters of a short string inside the object and those of a long string outside it.

## Variations and related ideas

- **Ropes.** A [rope](/rope/) stores a long string as a tree of pieces, so that an insertion or a concatenation does not copy the whole text.
- **Interning.** A runtime can keep one copy of each equal string constant, so that many references share it.
- **Copy on write.** A mutable string can share its buffer with a copy until one of the two changes. Then the string that changes makes its own copy of the buffer.
- **UTF-32.** A fixed width of 4 bytes makes the index of a code point O(1) but uses the most memory.
- **Wide strings in C and C++.** The size of `wchar_t` depends on the platform. "In practice, it is 32 bits and holds UTF-32 on Linux and many other non-Windows systems, but 16 bits and holds UTF-16 code units on Windows"[^14]. So the same code stores text in different ways.

## Pitfalls

- **`strlen` in a loop condition.** `for (i = 0; i < strlen(s); i++)` scans the whole string on each pass, so the loop is O(n²). Compute the length once.
- **A null byte in binary data.** C string functions stop at the first null byte. Data from a file can contain one, so keep a separate length.
- **A small slice of a large string.** In Go and Rust, a slice keeps the whole buffer alive. Copy the slice with `strings.Clone` or `to_owned` when you keep it for a long time.
- **Size estimates that assume one byte per character.** A buffer for N characters in UTF-8 needs up to 4N bytes. In UTF-16 a character outside the Basic Multilingual Plane needs 2 code units.
- **Lone surrogates.** A UTF-16 string can hold half of a surrogate pair. It is a valid JavaScript or Java string, but it is not valid text, and an encoder replaces it or fails.
- **Assumptions about object size.** The size of a `std::string` and the length of its inline buffer differ between C++ libraries, so code must not depend on them.

## Cost

The length is O(1) to read when the layout stores it, and O(n) for a C string. A header with a pointer adds one indirection to each access and one heap allocation to each new string, unless the string is small enough for an inline buffer. A fixed-width encoding gives O(1) access by index. UTF-8 and UTF-16 give O(1) access by code unit but O(n) access by code point. The memory cost of N characters is about N bytes for Latin-1 or ASCII text in UTF-8, Java compact strings or CPython, 2N bytes for UTF-16, and 4N bytes for UTF-32 or a CPython string with an emoji in it.

## History

Null-terminated strings are older than C. They "were produced by the .ASCIZ directive of the PDP-11 assembly languages and the ASCIZ directive of the MACRO-10 macro assembly language for the PDP-10"[^15]. "C designer Dennis Ritchie chose to follow the convention of null-termination to avoid the limitation on the length of a string and because maintaining the count seemed, in his experience, less convenient than using a terminator"[^15]. The choice spread with C, "a convention used and perpetuated by the popular C programming language"[^2]. Pascal dialects used a length byte, and later versions of length-prefixed strings "use 16-, 32-, or 64-bit words to store the string length"[^2].

Later runtimes tuned their layout for memory. PEP 393 (Python 3.3) gave CPython "a space-efficient representation in common cases"[^11]. JEP 254 (Java 9) took the same path for Java, because "strings are a major component of heap usage"[^12]. In 2019 Swift moved from UTF-16 to UTF-8 storage. Before that change, "string content could be in one of two native storage encodings: UTF-16 for Unicode-rich text, and a dedicated ASCII storage class when contents are all ASCII"[^8].

## References
