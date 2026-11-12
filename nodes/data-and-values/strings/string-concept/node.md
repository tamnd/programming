---
tag: K7SNNX
kind: concept
slug: string-concept
name: String
aliases: [string, string type, character string, text string, str, String class]
area: data-and-values/strings
level: intro
summary: >-
  A string is a sequence of characters that holds text. Each language defines what a character is, whether a string can change, and how text differs from bytes.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1957
  people: [Victor Yngve]
  note: According to Jean Sammet, COMIT, made in the 1950s, was the first realistic string handling language, and SNOBOL followed in the early 1960s. C made the null-terminated string common, and Python 3 made the text type a sequence of Unicode code points.
see_also: [characters-and-code-points, utf-8, immutability, split-a-string, reverse-a-string]
same_as:
  wikidata: Q184754
  wikipedia: String_(computer_science)
sources:
- url: https://en.wikipedia.org/wiki/String_(computer_science)
  title: String (computer science), Wikipedia
  locator: Introduction; History; String length; Null-terminated; Length-prefixed
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, Python documentation
  locator: Text Sequence Type - str; Bytes Objects; Bytearray Objects
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Class String, Java SE 25 API
  locator: Class description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String
  title: String, MDN Web Docs
  locator: UTF-16 characters, Unicode code points, and grapheme clusters
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: String types
- url: https://doc.rust-lang.org/book/ch08-02-strings.html
  title: The Rust Programming Language, 8.2 Storing UTF-8 Encoded Text with Strings
  locator: Defining Strings; Internal Representation
- url: https://en.cppreference.com/w/c/string/byte.html
  title: Null-terminated byte strings, C reference, cppreference.com
  locator: Introduction
- url: https://en.cppreference.com/w/cpp/string/basic_string.html
  title: std::basic_string, C++ reference, cppreference.com
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/strings/
  title: Strings and string literals, C# programming guide
  locator: Introduction; Immutability of strings
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/StringBuilder.html
  title: Class StringBuilder, Java SE 25 API
  locator: Class description
- url: https://doc.rust-lang.org/std/string/struct.String.html
  title: Struct String, The Rust Standard Library
  locator: Description
- url: https://go.dev/blog/strings
  title: Rob Pike, Strings, bytes, runes and characters in Go, The Go Blog (2013)
  locator: What is a string; UTF-8 and string literals
- url: https://hackage.haskell.org/package/base/docs/Prelude.html
  title: Prelude, Haskell base library
  locator: type String
- url: https://en.cppreference.com/w/c/language/string_literal.html
  title: String literals, C reference, cppreference.com
  locator: Notes
- url: https://en.wikipedia.org/wiki/COMIT
  title: COMIT, Wikipedia
  locator: Introduction
- url: https://docs.python.org/3/whatsnew/3.0.html
  title: What's New In Python 3.0, Python documentation
  locator: Text Vs. Data Instead Of Unicode Vs. 8-bit
---

## Idea

A program works with text all the time: names, messages, file paths, lines of input and the code of web pages. A string is the value that holds such text. It is a sequence of characters. It has a length, each character has a position, and a part of a string is also a string. Each language has a string type and literals for it, such as `"café"`.

```python run id=idea
# A string is a sequence of characters. It has a length, and each character has an index.
s = "café"
print(len(s), s[0], s[-1], s[1:3])

# A Python string is immutable, so an assignment to one character fails.
try:
    s[0] = "C"
except TypeError as e:
    print(type(e).__name__)

# An operation that "changes" a string makes a new string.
t = "C" + s[1:]
print(s, t)

# Text and bytes are different types. Encoding turns text into bytes.
b = s.encode("utf-8")
print(b, len(b))
print(b.decode("utf-8") == s)
```

```output for=idea
4 c é af
TypeError
café Café
b'caf\xc3\xa9' 5
True
```

The string `"café"` has 4 characters. Index 0 is `c`, index -1 is the last character, and the slice `s[1:3]` is the new string `af`. The assignment `s[0] = "C"` fails, because a Python string cannot change. To get `Café`, the program makes a new string from `"C"` and a slice, and `s` keeps its old value. The last lines show the difference between text and bytes. In UTF-8 the letter `é` takes 2 bytes, so the text has 4 characters but its bytes have a length of 5.

## How it works

**A sequence of characters.** Wikipedia says that "a string is traditionally a sequence of characters, either as a literal constant or as some kind of variable"[^1]. The languages do not agree on what a character is, so they do not agree on what the length and the index of a string count:

- In Python, "Strings are immutable sequences of Unicode code points"[^2].
- In Java, "A String represents a string in the UTF-16 format in which supplementary characters are represented by surrogate pairs", and "Index values refer to char code units"[^3]. JavaScript and C# use UTF-16 code units in the same way. MDN says that JavaScript "Strings are represented fundamentally as sequences of UTF-16 code units"[^4].
- In Go, "A string value is a (possibly empty) sequence of bytes"[^5], and the bytes are UTF-8 by convention.
- In Rust, the type `String` "is a growable, mutable, owned, UTF-8 encoded string type"[^6].
- In C, "A null-terminated byte string (NTBS) is a sequence of nonzero bytes followed by a byte with value zero (the terminating null character)"[^7].

So `"😀".length` is 2 in JavaScript and Java, because the emoji is one code point but two UTF-16 code units. `len("😀")` is 1 in Python and 4 in Go. The pages [string indexing](/string-indexing/) and [find the length of a string](/find-the-length-of-a-string/) give the details.

**Where the length is.** A string must know where it ends. A C string has no stored length. The null character marks the end, so `strlen` must scan the bytes. Most other languages store the length next to the characters, so the string can hold any character, including a null character. A C++ `std::string` stores the length and also keeps a null character after the last element, because "*(s.begin() + s.size()) has value CharT() (a null terminator)"[^8]. The page [how strings are stored](/string-memory-layout/) compares the layouts.

**Immutable strings.** In many languages a string value never changes. Java says: "Strings are constant; their values cannot be changed after they are created. String buffers support mutable strings. Because String objects are immutable they can be shared"[^3]. Go says: "Strings are immutable: once created, it is impossible to change the contents of a string"[^5]. C# says: "String objects are immutable: they can't be changed after they're created"[^9]. Python, JavaScript and Kotlin strings are immutable too. An operation such as concatenation, replacement or a change of case returns a new string. Two variables can share one string with no risk, a string can be a key in a hash map, and two threads can read it with no lock.

**Mutable strings.** Code that builds text piece by piece needs a buffer that can change. Java has `StringBuilder`, "A mutable sequence of characters"[^10], and C# has `System.Text.StringBuilder`. Go has `strings.Builder` and `[]byte`. Python has no mutable string type, but "str.join() or io.StringIO can be used to efficiently construct strings from multiple fragments"[^2]. In Rust, `String` owns a growable buffer on the heap[^11], and `&str` borrows a slice of one. In C, a string is an array of `char`, and the array can change. A C++ `std::string` is also mutable. See [build a string efficiently](/build-a-string-efficiently/).

**Strings and bytes.** Text is a sequence of characters, and the bytes of a file or of a network message are a sequence of numbers from 0 to 255. An encoding such as [UTF-8](/utf-8/) maps one to the other. Python keeps the two apart: "Bytes objects are immutable sequences of single bytes", and "bytearray objects are a mutable counterpart to bytes objects"[^2]. Java and C# use `byte[]` and convert with `getBytes` and `Encoding.UTF8`. Go is different. Its strings hold bytes, and "a string holds arbitrary bytes. It is not required to hold Unicode text, UTF-8 text, or any other predefined format"[^12]. Rust is strict in the other direction: a `String` is always valid UTF-8, and `String::from_utf8` returns an error for bytes that are not. The C language does not separate text from bytes at all, so a `char` array can hold either.

**Operations.** The daily work with strings is a small set of operations, and the other pages of this sub-area show them in each tier 1 language: [length](/find-the-length-of-a-string/), [concatenation](/concatenate-strings/), [join](/join-a-list-of-strings/), [split](/split-a-string/), [substring](/get-a-substring/), [search](/search-for-a-substring/), [replace](/replace-a-substring/), [comparison](/compare-strings/), case change and trim.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `str` of code points, immutable; `bytes` and `bytearray` for binary data |
| JavaScript | native | primitive `string` of UTF-16 code units, immutable; `TextEncoder` for UTF-8 bytes |
| TypeScript | native | `string`, the JavaScript type, and string literal types such as `"on" \| "off"` |
| Java | native | `java.lang.String` of UTF-16 `char` values, immutable; `StringBuilder` is mutable |
| Kotlin | native | `kotlin.String`, immutable, on the JVM `java.lang.String`; `StringBuilder` and `buildString` |
| C# | native | `string` (`System.String`) of UTF-16 `char` values, immutable; `StringBuilder` |
| Go | native | `string`, an immutable sequence of bytes, UTF-8 by convention; `[]byte`, `[]rune` and `strings.Builder` |
| Rust | native | `str` (used as `&str`), a borrowed UTF-8 slice; `String`, owned and growable |
| C | idiom | an array of `char` that ends with a null character; the functions of `<string.h>` |
| C++ | stdlib | `std::string`, a mutable sequence of `char`; `std::string_view`; C strings as well |
| Bash | native | every variable value is a string; `${#s}`, `${s:i:n}` and other parameter expansions |
| PowerShell | native | `[string]`, the .NET `System.String`; single-quoted and expandable double-quoted strings |
| SQL | native | `CHAR(n)`, `VARCHAR(n)` and `CLOB`, with `\|\|` for concatenation |
| Regex | n/a | a pattern matches strings, but the pattern language has no string values |
| HTML | n/a | text content and attribute values are text, but HTML has no string type |
| CSS | native | the `<string>` data type, a quoted sequence of characters, as in `content: "→"` |

All general-purpose languages of the table have strings, but C has only a convention: an array of `char` and a null character at the end. The languages differ in three ways. The first is the unit of a character: a byte in C and Go, a UTF-16 code unit in Java, JavaScript and C#, and a code point in Python. The second is whether a string can change. The third is how strictly the language keeps text apart from bytes.

The example files show these differences. Python shows that `+=` makes a new object, that a `bytearray` changes in place, and that `str` and `bytes` do not mix. JavaScript shows that `length` counts UTF-16 code units, that an assignment to an index throws in strict mode, and that a `String` object is not a primitive string. Java and C# count UTF-16 code units and code points, and change a `StringBuilder` in place. Go shows that `len` counts bytes, that `range` decodes runes, and that a string can hold bytes that are not valid UTF-8. Rust shows `&str` and `String`, a slice on a character boundary, and `String::from_utf8`. C shows the null character, `strlen` and `sizeof`, and the UTF-8 bytes of `"café"`. C++ shows a mutable `std::string` that holds a null character, a `std::string_view` and `c_str`.

## Variations and related ideas

- **Fixed-length and variable-length strings.** Wikipedia describes "fixed-length strings, which have a fixed maximum length to be determined at compile time and which use the same amount of memory whether this maximum is needed or not, and variable-length strings"[^1]. SQL `CHAR(n)` is fixed-length and pads with spaces, and `VARCHAR(n)` has a maximum length.
- **Strings as lists.** In Haskell, "String is an alias for a list of characters"[^13], so list functions work on strings, but each character takes a list cell.
- **String views and slices.** A view refers to characters that another value owns, so a substring needs no copy. Examples are Rust `&str`, C++ `std::string_view` and Go substrings, which share the bytes of the original string.
- **Ropes.** A [rope](/rope/) stores a long string as a tree of short pieces, so an insertion in the middle does not copy the whole text.
- **Byte strings.** Python `bytes`, Go `[]byte` and C `char` arrays hold bytes that may or may not be text. Some programs, such as tools that read file names on Unix, must handle bytes that are not valid in any encoding.
- **Interned strings.** Some languages keep one copy of equal string constants, so that a comparison of two references is enough. This is an implementation technique, and it is not part of the string type.

## Pitfalls

- **Length is not the number of characters.** `"😀".length` is 2 in JavaScript, Java and C#, `len("é")` is 2 in Go, and a flag emoji is two code points in every language. Decide which unit the program needs: bytes for storage, code points for most text processing, or [grapheme clusters](/grapheme-cluster/) for what a user sees.
- **Repeated concatenation in a loop.** An immutable string cannot grow, so `s = s + part` copies all of `s` each time. Over n parts the copying grows with n². Use a builder or a join.
- **`==` on Java strings.** `==` compares references, not characters, so two equal strings can give `false`. Use `equals`. See [compare strings](/compare-strings/).
- **Mixing text and bytes.** A program that decodes bytes with the wrong encoding gets wrong characters, for example `cafÃ©` instead of `café`. Name the encoding at each boundary: file, network and database.
- **C strings without room.** `strcpy` and `strcat` write past the end of the target array if it is too small, and the array needs one more byte for the null character. Also, "String literals are not modifiable", and "If a program attempts to modify the static array formed by a string literal, the behavior is undefined"[^14].
- **A null character inside data.** The C string functions stop at the first null character. Bytes from a file or a network can contain one, so a C program must keep a length for such data.

## Cost

A string of n characters takes memory in proportion to n, plus a small header. The header holds the length and, in most languages, a capacity or a hash. Reading the length is O(1) when the language stores it, and O(n) for `strlen` in C. Concatenation of two strings copies both, so it is O(n + m). A loop that appends n short parts to an immutable string is O(n²) in total, and a builder with a growing buffer makes it O(n). Access by index is O(1) for fixed-size units, such as bytes in Go and UTF-16 code units in Java. Access by code point in a UTF-8 string is O(n), because the program must decode the bytes before the index. Immutable strings save copies when code shares them, and they can cache their hash code.

## History

Strings came from mathematics and logic, where a string is "a sequence of symbols or linguistic elements in a definite order"[^1]. In programming, according to Jean E. Sammet, "the first realistic string handling and pattern matching language" for computers was COMIT in the 1950s, followed by the SNOBOL language of the early 1960s[^1]. COMIT was "developed on the IBM 700/7000 series computers by Victor Yngve, University of Chicago, and collaborators at MIT from 1957 to 1965", for research in machine translation[^15].

The two common layouts of early strings come from two languages. In C, the null character ends a string, "a convention used and perpetuated by the popular C programming language"[^1]. Many Pascal dialects store the length in the first byte, and "Storing the string length as byte limits the maximum string length to 255"[^1].

For a long time a character was one byte, but Unicode changed this. Java, JavaScript and C# chose 16-bit characters. Python 3.0, released on December 3, 2008, split text from bytes: "Python 3.0 uses the concepts of text and (binary) data instead of Unicode strings and 8-bit strings"[^16], and "The type used to hold text is str, the type used to hold data is bytes"[^16]. Go and Rust chose UTF-8. Rob Pike's article on Go strings from 2013 states that "Go source code is always UTF-8"[^12], and the Rust book says that "A String is a wrapper over a Vec<u8>"[^6].

## References
