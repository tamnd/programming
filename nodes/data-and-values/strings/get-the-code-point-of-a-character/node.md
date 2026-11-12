---
tag: QSJE4Y
kind: task
slug: get-the-code-point-of-a-character
name: Get the code point of a character
aliases: [ord, character code, char code, codePointAt, Unicode value of a character, rune value]
area: data-and-values/strings
level: intro
summary: Get the number of the code point of a character, not the value of its first byte or its first UTF-16 code unit.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/functions.html#ord
  title: The Python Standard Library, Built-in Functions
  locator: ord()
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/codePointAt
  title: MDN Web Docs, String.prototype.codePointAt()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, Class String
  locator: charAt(int), codePointAt(int)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.char.converttoutf32?view=net-10.0
  title: .NET API reference, Char.ConvertToUtf32 method
  locator: Overloads, Exceptions
- url: https://pkg.go.dev/unicode/utf8#DecodeRuneInString
  title: Go standard library, package unicode/utf8
  locator: DecodeRuneInString
- url: https://doc.rust-lang.org/std/primitive.char.html
  title: Rust standard library, primitive type char
  locator: Validity and Layout, from_u32
- url: https://www.rfc-editor.org/rfc/rfc3629
  title: RFC 3629, UTF-8, a transformation format of ISO 10646
  locator: 3. UTF-8 definition
- url: https://en.cppreference.com/w/cpp/language/string_literal
  title: cppreference.com, String literal
  locator: Explanation
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#index-printf
  title: Bash Reference Manual
  locator: 4.2 Bash Builtin Commands, printf
---

## Task

Get the code point of a character. The function `code_point(c)` takes a string of exactly one code point and returns its number. For example, `code_point("A")` is 65, `code_point("é")` is 233 and `code_point("🙂")` is 128578 (U+1F642).

The result is the number of the code point, from 1 to 1114111 (U+10FFFF). It is not the value of the first byte of a UTF-8 string, and it is not the value of the first UTF-16 code unit. To visit each character of a longer string, see [[iterate-over-the-characters-of-a-string]].

## Solutions

| Language | Get the code point |
|---|---|
| Python | `ord(c)` |
| JavaScript, TypeScript | `c.codePointAt(0)` |
| Java | `c.codePointAt(0)` |
| C# | `Rune.GetRuneAt(c, 0).Value` or `char.ConvertToUtf32(c, 0)` |
| Go | `r, _ := utf8.DecodeRuneInString(c)` |
| Rust | `c.chars().next()`, then `as u32` |
| C | decode the UTF-8 sequence from its bytes |
| C++ | the same decoding, or `std::u32string` with `U"..."` literals |
| Bash | decode the bytes in the C locale |

- **Python.** "If the argument is a one-character string, return the Unicode code point of that character"[^1].
- **JavaScript and TypeScript.** `codePointAt` returns the code point value "of the character starting at the given index"[^2]. The index counts UTF-16 code units.
- **Java.** `codePointAt` "Returns the character (Unicode code point) at the specified index"[^3].
- **C#.** `ConvertToUtf32` "Converts the value of a UTF-16 encoded surrogate pair into a Unicode code point"[^4].
- **Go.** `DecodeRuneInString` gives the first rune of a string and its length in bytes. For an empty string, the docs say: "If s is empty it returns (RuneError, 0)"[^5].
- **Rust.** A `char` is "any ‘Unicode code point’ other than a surrogate code point"[^6], and `as u32` gives its number.
- **C, C++ and Bash.** UTF-8 uses "sequences of 1 to 4 octets"[^7]. The first byte gives the length and the high bits. Each continuation byte, `10xxxxxx`, gives 6 more bits.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `space`, `tilde` and `tab`: ASCII characters have 1 byte, and the code point is the value of that byte.
- `two-byte`: `é` (U+00E9) is 233, from 2 UTF-8 bytes.
- `three-byte` and `cjk`: `€` (U+20AC) is 8364, and `日` (U+65E5) is 26085. Each one has 3 UTF-8 bytes.
- `combining`: U+0301 alone is a character, with the code point 769.
- `astral`: U+1F642 is 128578. It has 4 UTF-8 bytes and 2 UTF-16 code units.
- `max`: U+10FFFF, 1114111, is the largest code point.

## Pitfalls

- **Python ord takes only one character.** `é` as `e` and U+0301 is 2 code points, so `ord` raises `TypeError`. Normalize the text first, see [[normalize-unicode-text]]:

```python run id=python-ord-one-character
print(ord("\u00e9"))
try:
    ord("e\u0301")
except TypeError as e:
    print(type(e).__name__)
```

```output for=python-ord-one-character
233
TypeError
```

- **JavaScript charCodeAt gives a UTF-16 code unit.** For U+1F642, `charCodeAt(0)` gives the high surrogate 55357. Also, `codePointAt(1)` gives the low surrogate 56898, because "the index is still based on UTF-16 code units, not Unicode code points"[^2]:

```javascript run id=javascript-charcodeat-unit
const s = "\u{1f642}";
console.log(s.charCodeAt(0), s.codePointAt(0), s.codePointAt(1));
```

```output for=javascript-charcodeat-unit
55357 128578 56898
```

- **Java charAt gives a char.** `charAt` "Returns the char value at the specified index"[^3], and a `char` is a UTF-16 code unit. So U+1F642 has a length of 2:

```java run id=java-charat-unit
public class Main {
  public static void main(String[] args) {
    String s = "\uD83D\uDE42";
    System.out.println((int) s.charAt(0) + " " + s.codePointAt(0) + " " + s.length());
  }
}
```

```output for=java-charat-unit
55357 128578 2
```

- **C# s[1] is the second half of a pair.** `(int)s[0]` is the high surrogate. `ConvertToUtf32(s, 1)` starts at the low surrogate, so it throws `ArgumentException`:

```csharp run id=csharp-index-surrogate
using System;

string s = "\U0001F642";
Console.WriteLine($"{(int)s[0]} {char.ConvertToUtf32(s, 0)}");
try
{
    char.ConvertToUtf32(s, 1);
}
catch (ArgumentException e)
{
    Console.WriteLine(e.GetType().Name);
}
```

```output for=csharp-index-surrogate
55357 128578
ArgumentException
```

- **Go s[0] is a byte.** `s[0]` is 240, the first byte of U+1F642. `DecodeRuneInString` gives the rune and its 4 bytes:

```go run id=go-index-byte
package main

import (
	"fmt"
	"unicode/utf8"
)

func main() {
	s := "\U0001F642"
	r, size := utf8.DecodeRuneInString(s)
	fmt.Println(s[0], r, size)
}
```

```output for=go-index-byte
240 128578 4
```

- **Rust: not each u32 is a char.** Each `char` can be cast to a `u32`, but "not all valid u32s are valid chars"[^6]. `char::from_u32` returns `None` for a surrogate such as 0xD800:

```rust run id=rust-from-u32-surrogate
fn main() {
    let c = '\u{1F642}';
    println!("{}", c as u32);
    println!("{:?} {:?}", char::from_u32(0xD800), char::from_u32(0x41));
}
```

```output for=rust-from-u32-surrogate
128578
None Some('A')
```

- **C c[0] is a byte.** The first byte of U+1F642 is 240. The decoding takes 3 bits from the first byte and 6 bits from each of the 3 continuation bytes:

```c run id=c-first-byte
#include <stdio.h>
#include <string.h>

int main(void) {
    const char *c = "\xf0\x9f\x99\x82";
    const unsigned char *p = (const unsigned char *)c;
    unsigned long cp = ((p[0] & 0x07ul) << 18) | ((p[1] & 0x3ful) << 12) | ((p[2] & 0x3ful) << 6) | (p[3] & 0x3ful);
    printf("%zu %d %lu\n", strlen(c), p[0], cp);
    return 0;
}
```

```output for=c-first-byte
4 240 128578
```

- **C++ std::string holds bytes.** U+1F642 is 4 elements of a `std::string`. A UTF-32 string literal has the type `const char32_t[N]`[^8], so each element of a `std::u32string` is one code point:

```cpp run id=cpp-u32string
#include <iostream>
#include <string>

int main() {
    std::string bytes = "\xf0\x9f\x99\x82";
    std::u32string points = U"\U0001F642";
    std::cout << bytes.size() << " " << points.size() << " " << static_cast<unsigned long>(points[0]) << "\n";
}
```

```output for=cpp-u32string
4 1 128578
```

- **Bash printf uses the locale.** For `printf '%d' "'x"`, "the value is the numeric value of the following character, using the current locale"[^9]. In the C locale, the value is only the first byte of `é`, 195, not 233. The code takes the low 8 bits with `& 255`, so that the value is from 0 to 255:

```bash run id=bash-printf-locale
c=$'\xc3\xa9'
(
  LC_ALL=C
  printf -v b '%d' "'$c"
  echo "${#c} $((b & 255))"
)
```

```output for=bash-printf-locale
2 195
```

## Related tasks

- [[iterate-over-the-characters-of-a-string]]: get each character of a string, then its code point.
- [[encode-and-decode-utf-8]]: the bytes of a code point in UTF-8, and the reverse.
- [[convert-a-string-to-bytes]]: the bytes of a whole string.
- [[change-the-case-of-a-string]]: a change of case that uses the code points of the ASCII letters.

## References
