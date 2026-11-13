---
tag: MRBAQA
kind: task
slug: find-the-length-of-a-string
name: Find the length of a string
aliases: [string length, count the characters of a string, len of a string, size of a string, byte length of a string]
area: data-and-values/strings
level: intro
summary: Find the length of a string in UTF-8 bytes, in UTF-16 code units and in code points, and know which of them len, length, size and count give in each language.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://www.rfc-editor.org/rfc/rfc3629.html
  title: F. Yergeau, RFC 3629, UTF-8, a transformation format of ISO 10646 (2003)
  locator: Section 3, UTF-8 definition
- url: https://www.rfc-editor.org/rfc/rfc2781.html
  title: P. Hoffman and F. Yergeau, RFC 2781, UTF-16, an encoding of ISO 10646 (2000)
  locator: Section 2.1, Encoding UTF-16
- url: https://docs.python.org/3/library/stdtypes.html#text-sequence-type-str
  title: The Python Standard Library, Built-in Types
  locator: Text Sequence Type, str
- url: https://docs.python.org/3/library/functions.html#len
  title: The Python Standard Library, Built-in Functions
  locator: len
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/length
  title: MDN Web Docs, String length
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/API/TextEncoder/encode
  title: MDN Web Docs, TextEncoder encode() method
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, java.lang.String
  locator: length(), codePointCount(int, int)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.length?view=net-10.0
  title: .NET API reference, String.Length property
  locator: Remarks
- url: https://learn.microsoft.com/en-us/dotnet/api/system.text.encoding.getbytecount?view=net-10.0
  title: .NET API reference, Encoding.GetByteCount method
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.enumeraterunes?view=net-10.0
  title: .NET API reference, String.EnumerateRunes method
- url: https://go.dev/ref/spec#String_types
  title: The Go Programming Language Specification
  locator: String types
- url: https://pkg.go.dev/unicode/utf8#RuneCountInString
  title: Go standard library, package unicode/utf8
  locator: RuneCountInString
- url: https://pkg.go.dev/unicode/utf16#RuneLen
  title: Go standard library, package unicode/utf16
  locator: RuneLen
- url: https://doc.rust-lang.org/std/primitive.str.html#method.len
  title: The Rust Standard Library, primitive type str
  locator: len, chars, encode_utf16
- url: https://en.cppreference.com/w/c/string/byte/strlen
  title: cppreference.com, strlen, strnlen_s
- url: https://en.cppreference.com/w/cpp/string/basic_string/size
  title: cppreference.com, std::basic_string size, length
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Shell-Parameter-Expansion
  title: Bash Reference Manual, on the site of the Bash maintainer Chet Ramey
  locator: Shell Parameter Expansion; Bash Variables, LC_CTYPE
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/Segmenter
  title: MDN Web Docs, Intl.Segmenter
---

## Task

Find the length of a string. A string has more than one length, so the function `lengths(s)` returns three numbers: the number of bytes in its UTF-8 form, the number of UTF-16 code units, and the number of code points. The built-in length of each language is one of these three. It is not always the number of characters that a reader sees.

UTF-8 uses 1 to 4 bytes for each code point, because characters "are encoded using sequences of 1 to 4 octets"[^1]. UTF-16 uses one 16-bit code unit for a code point up to U+FFFF, and a pair of code units, a high surrogate and a low surrogate, for a code point above U+FFFF[^2]. So `"🙂"` (U+1F642) has a length of 4 bytes, 2 code units and 1 code point.

The number of graphemes, the characters that a reader sees, is a fourth length. It needs the segmentation rules of Unicode, so it is a separate task, [count the graphemes of a string](/count-the-graphemes-of-a-string/). See [grapheme cluster](/grapheme-cluster/).

## Solutions

The built-in length of a string counts the units that the language stores:

| Language | Built-in length | It counts |
|---|---|---|
| Python | `len(s)` | code points |
| JavaScript, TypeScript | `s.length` | UTF-16 code units |
| Java | `s.length()` | UTF-16 code units |
| C# | `s.Length` | UTF-16 code units |
| Go | `len(s)` | bytes |
| Rust | `s.len()` | bytes |
| C | `strlen(s)` | bytes before the first null byte |
| C++ | `s.size()`, `s.length()` | `char` elements, that is, bytes |
| Bash | `${#s}` | characters of the current locale |

- **Python.** A `str` is one of the "immutable sequences of Unicode code points"[^3], and `len` returns "the number of items"[^4], so `len(s)` counts code points. The code encodes the string to count bytes and UTF-16 units. The codec `utf-16-le` writes no byte order mark, so the number of bytes divided by 2 is the number of code units.
- **JavaScript and TypeScript.** `length` gives "the length of the string in UTF-16 code units"[^5]. The string iterator yields code points, so `[...s].length` counts code points. `TextEncoder.encode` returns "A Uint8Array object containing the UTF-8 encoding of the input string"[^6], and its length is the number of bytes.
- **Java.** For `length()`, "The length is equal to the number of Unicode code units in the string"[^7]. `codePointCount(0, s.length())` counts code points, and `getBytes(StandardCharsets.UTF_8).length` counts bytes.
- **C#.** "The Length property returns the number of Char objects in this instance, not the number of Unicode characters"[^8]. A `Char` is one UTF-16 code unit. `Encoding.UTF8.GetByteCount` calculates "the number of bytes produced by encoding a set of characters"[^9] and makes no array. `EnumerateRunes` returns "an enumeration of Rune from this string"[^10], and a `Rune` is one code point.
- **Go.** "A string value is a (possibly empty) sequence of bytes. The number of bytes is called the length of the string"[^11]. `utf8.RuneCountInString` counts code points[^12]. `utf16.RuneLen` "returns the number of 16-bit words in the UTF-16 encoding of the rune"[^13], so the code adds it up over the runes of the string.
- **Rust.** For `str::len`, "This length is in bytes, not chars or graphemes"[^14]. `s.chars().count()` counts code points, and `s.encode_utf16().count()` counts UTF-16 units. Both walk the whole string.
- **C.** A C string has no stored length. `strlen` counts the bytes "up to and not including the first null character"[^15]. The code takes the length `n` as an argument, so the text can hold a null byte. It counts the first bytes of the code points, which are all the bytes that do not have the form `10xxxxxx`. Each first byte from `F0` starts a code point that needs two UTF-16 units.
- **C++.** `size()` returns "the number of CharT elements in the string"[^16], and for `std::string` and `std::string_view` a `CharT` is a byte. The code counts code points as the C code does, with `std::ranges::count_if`.
- **Bash.** `${#s}` "Substitutes the length in characters of the value of parameter"[^17]. The locale category `LC_CTYPE` "determines the interpretation of characters"[^17], so the function counts in the locale `C`, where each byte is a character, and in the locale `C.UTF-8`, where each code point is a character. It counts the bytes `F0` to `F4` to find the code points that need two UTF-16 units.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: all three lengths are 0.
- `two-byte`, `three-byte` and `euro-sign`: `ï` is 2 bytes and `€` is 3 bytes in UTF-8, but each is one UTF-16 unit and one code point. A byte length is larger than the code point length for all text that is not ASCII.
- `astral` and `mixed`: `🙂` is 4 bytes, 2 UTF-16 units and 1 code point. This is the case where JavaScript, Java and C# give a different number from Python.
- `combining-mark` and `precomposed`: `é` can be one code point, U+00E9, or two, `e` and U+0301 COMBINING ACUTE ACCENT. A reader sees one character in each case, but every length in this task gives a different number for the two forms. Normalize the text first if the two forms must have the same length.
- `flag`: the flag `🇫🇷` is two regional indicator code points, so its lengths are 8, 4 and 2.
- `zwj-family`: `👨‍👩‍👧` is three emoji joined by two U+200D ZERO WIDTH JOINER code points. It is 18 bytes, 8 UTF-16 units and 5 code points, and one grapheme.
- `nul`: a null character in the middle. Every language except C and Bash counts it. `strlen` stops at it, and a Bash variable cannot hold it, so the Bash solution has no `nul` case.

## Pitfalls

- **Python `len` counts code points, not what a reader sees.** A letter with a combining mark is two code points. After normalization to NFC, the example has one code point for `é`:

```python run id=python-len-code-points
import unicodedata

word = "cafe\u0301"
print(len(word), len(word.encode("utf-8")))
print(len(unicodedata.normalize("NFC", word)))
print(len("🙂"), len("🙂".encode("utf-8")))
```

```output for=python-len-code-points
5 6
4
1 4
```

- **JavaScript `length` counts UTF-16 code units.** A code point above U+FFFF is two units, and `charCodeAt(0)` gives only its high surrogate. `[...s].length` counts code points, and `Intl.Segmenter`, which gets "meaningful items (graphemes, words or sentences) from a string"[^18], counts graphemes:

```javascript run id=javascript-length-units
const smile = "🙂";
console.log(smile.length, [...smile].length);
console.log(smile.charCodeAt(0).toString(16), smile.codePointAt(0).toString(16));

const family = "👨‍👩‍👧";
const graphemes = [...new Intl.Segmenter("en", { granularity: "grapheme" }).segment(family)];
console.log(family.length, [...family].length, graphemes.length);
```

```output for=javascript-length-units
2 1
d83d 1f642
8 5 1
```

- **Java `length()` and `charAt` see code units.** The `char` at index 1 of `"a🙂b"` is half of the emoji:

```java run id=java-length-surrogates
public class Main {
  public static void main(String[] args) {
    String s = "a🙂b";
    System.out.println(s.length() + " " + s.codePointCount(0, s.length()));
    System.out.println(Integer.toHexString(s.charAt(1)) + " " + Character.isHighSurrogate(s.charAt(1)));
  }
}
```

```output for=java-length-surrogates
4 3
d83d true
```

- **C# `Length` counts `Char` values.** `EnumerateRunes` counts code points. The documentation says to "Use the System.Globalization.StringInfo class to work with each Unicode character instead of each Char"[^8], and `StringInfo.LengthInTextElements` counts the text elements:

```csharp run id=csharp-length-runes
using System.Globalization;
using System.Text;

var s = "e\u0301🙂";
Console.WriteLine($"{s.Length} {s.EnumerateRunes().Count()} {new StringInfo(s).LengthInTextElements}");
Console.WriteLine(Encoding.UTF8.GetByteCount(s));
```

```output for=csharp-length-runes
4 3 2
7
```

- **Go `len` and a loop over the index see bytes.** A `for range` loop over the string itself yields runes and the byte index of each:

```go run id=go-len-bytes
package main

import (
	"fmt"
	"unicode/utf8"
)

func main() {
	s := "日本語"
	fmt.Println(len(s), utf8.RuneCountInString(s))
	for i := range len(s) {
		fmt.Printf("%x ", s[i])
	}
	fmt.Println()
	for i, r := range s {
		fmt.Print(i, ":", string(r), " ")
	}
	fmt.Println()
}
```

```output for=go-len-bytes
9 3
e6 97 a5 e6 9c ac e8 aa 9e
0:日 3:本 6:語
```

- **A byte length in Rust is not a safe place to cut.** `s.len()` is in bytes, and a slice must start and end on a character boundary. `get` is "the non-panicking alternative to indexing the str"[^14], so it returns `None` for a range that ends inside a character, where `&s[0..3]` panics:

```rust run id=rust-len-boundary
fn main() {
    let s = "naïve";
    println!("{} {}", s.len(), s.chars().count());
    println!("{} {}", s.is_char_boundary(2), s.is_char_boundary(3));
    println!("{:?}", s.get(0..3));
    println!("{:?}", s.get(0..4));
}
```

```output for=rust-len-boundary
6 5
true false
None
Some("naï")
```

- **C `strlen` stops at the first null byte, and `sizeof` counts the terminator.** `sizeof` of a character array gives the size of the array, which includes the null character at the end of the literal. Neither is the number of characters of UTF-8 text:

```c run id=c-strlen-nul
#include <stdio.h>
#include <string.h>

int main(void) {
    char text[] = "a\0b";
    printf("%zu %zu\n", strlen(text), sizeof text);
    const char *word = "naïve";
    printf("%zu\n", strlen(word));
}
```

```output for=c-strlen-nul
1 4
6
```

- **Bash `${#s}` depends on the locale.** In the `C` locale, `${#s}` of `naïve` is 6. In a UTF-8 locale, it is 5. A script that counts characters must set `LC_ALL` or `LC_CTYPE`.
- **A length is not a display width.** The width of text on a terminal or a page is a different number, and none of the lengths in this task gives it.

## Related tasks

- [[reverse-a-string]]: a reverse by code units breaks the same characters that a count by code units counts twice.
- [[split-a-string]]: the byte search of the split works because UTF-8 has a fixed form for each first byte.

## References
