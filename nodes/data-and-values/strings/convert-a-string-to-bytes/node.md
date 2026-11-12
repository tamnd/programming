---
tag: R5CD07
kind: task
slug: convert-a-string-to-bytes
name: Convert a string to bytes
aliases: [string to byte array, encode a string, getBytes, TextEncoder, as_bytes, byte array of a string]
area: data-and-values/strings
level: intro
summary: Get the bytes of a string in UTF-8, and give the encoding each time so that the result does not change with the platform.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#str.encode
  title: The Python Standard Library, Built-in Types
  locator: str(), str.encode
- url: https://developer.mozilla.org/en-US/docs/Web/API/TextEncoder
  title: MDN Web Docs, TextEncoder
  locator: Instance methods
- url: https://go.dev/ref/spec#Conversions_to_and_from_a_string_type
  title: The Go Programming Language Specification
  locator: Conversions to and from a string type
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/length
  title: MDN Web Docs, String.length
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, Class String
  locator: getBytes()
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.2
  title: The Java Language Specification, Java SE 25 Edition
  locator: 4.2 Primitive Types and Values
- url: https://learn.microsoft.com/en-us/dotnet/api/system.text.encoding.unicode?view=net-10.0
  title: .NET API reference, Encoding.Unicode property
  locator: Definition
- url: https://doc.rust-lang.org/std/string/struct.String.html#method.from_utf8_lossy
  title: Rust standard library, struct String
  locator: from_utf8_lossy
- url: https://en.cppreference.com/w/c/language/string_literal
  title: cppreference.com, String literals (C)
  locator: Explanation
- url: https://en.cppreference.com/w/cpp/string/basic_string/basic_string
  title: cppreference.com, std::basic_string constructor
  locator: Notes
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#index-echo
  title: Bash Reference Manual
  locator: 4.2 Bash Builtin Commands, echo
---

## Task

Convert a string to bytes. The function `to_bytes(s)` returns the bytes of `s` in UTF-8, in order, each from 0 to 255. For example, `to_bytes("abc")` is `[97, 98, 99]`, and `to_bytes("é")` is `[195, 169]`.

A string is text, and bytes are its form in a file, on a network or in memory. The same text has different bytes in different encodings, so the code gives the encoding each time. To write the encoder by hand, see [[encode-and-decode-utf-8]].

## Solutions

| Language | The UTF-8 bytes of a string |
|---|---|
| Python | `s.encode("utf-8")` |
| JavaScript, TypeScript | `new TextEncoder().encode(s)` |
| Java | `s.getBytes(StandardCharsets.UTF_8)` |
| C# | `Encoding.UTF8.GetBytes(s)` |
| Go | `[]byte(s)` |
| Rust | `s.as_bytes()`, or `s.as_bytes().to_vec()` for a copy |
| C | the `char` array of the string, read as `unsigned char` |
| C++ | `std::vector<std::uint8_t>(s.begin(), s.end())` |
| Bash | `printf '%d' "'x"` for each byte, in the C locale |

- **Python.** `str.encode` returns "the string encoded to bytes"[^1]. The result is a `bytes` object.
- **JavaScript and TypeScript.** `TextEncoder.encode` returns "a Uint8Array containing the string encoded using UTF-8"[^2]. It has no other encoding.
- **Java and C#.** Give the charset: `StandardCharsets.UTF_8` and `Encoding.UTF8`.
- **Go.** The conversion "yields a non-nil slice whose successive elements are the bytes of the string"[^3]. A Go string holds the bytes, so the conversion copies them.
- **Rust.** A `str` is always UTF-8, so `as_bytes` gives its bytes with no copy.
- **C and C++.** A string literal in the source is already bytes. The impls in this node expect UTF-8 strings.
- **Bash.** The C locale makes `${s:i:1}` one byte. The code takes the low 8 bits with `& 255`.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: the empty string gives an empty list.
- `control`: a tab and a line feed are the bytes 9 and 10.
- `two-byte` and `latin-1-max`: `é` (U+00E9) is `[195, 169]`, and U+00FF is `[195, 191]`. In Latin-1, each one would be 1 byte.
- `three-byte` and `bom`: `€` (U+20AC) is `[226, 130, 172]`, and U+FEFF is `[239, 187, 191]`.
- `astral`: U+1F642 is 4 bytes, `[240, 159, 153, 130]`.
- `combining`: `e` and U+0301 are `[101, 204, 129]`. This is not the same as the 2 bytes of `é` as one code point. See [[normalize-unicode-text]].
- `mixed`: the bytes of each code point follow the bytes of the code point before it.

## Pitfalls

- **Python str of bytes is not a decode.** "Passing a bytes object to str() without the encoding or errors arguments falls under the first case of returning the informal string representation"[^1]. So `str(b)` gives `b'abc'`. Use `b.decode("utf-8")`. Also, the encoding changes the length:

```python run id=python-str-of-bytes
b = "abc".encode("utf-8")
print(str(b))
print(b.decode("utf-8"))
print(len("\u00e9".encode("latin-1")), len("\u00e9".encode("utf-8")))
```

```output for=python-str-of-bytes
b'abc'
abc
1 2
```

- **JavaScript length is not the number of bytes.** `length` "returns the number of code units in the string"[^4]. U+1F642 is 2 code units and 4 UTF-8 bytes:

```javascript run id=javascript-length-not-bytes
const s = "\u{1f642}";
console.log(s.length, new TextEncoder().encode(s).length);
```

```output for=javascript-length-not-bytes
2 4
```

- **Java getBytes() uses the default charset, and a byte is signed.** `getBytes()` "Encodes this String into a sequence of bytes using the default charset"[^5], so give the charset. Also, `byte` is one of the integral types "whose values are 8-bit, 16-bit, 32-bit and 64-bit signed two's-complement integers, respectively"[^6]. The first byte of `é` is -61, and `b & 0xff` is 195:

```java run id=java-signed-byte
import java.nio.charset.StandardCharsets;

public class Main {
  public static void main(String[] args) {
    byte[] b = "\u00e9".getBytes(StandardCharsets.UTF_8);
    System.out.println(b[0] + " " + (b[0] & 0xff));
  }
}
```

```output for=java-signed-byte
-61 195
```

- **C# Encoding.Unicode is UTF-16.** The property "Gets an encoding for the UTF-16 format using the little endian byte order"[^7]. So `é` is the 2 bytes E9 and 00, not C3 and A9:

```csharp run id=csharp-unicode-is-utf16
using System;
using System.Text;

string s = "\u00e9";
Console.WriteLine(BitConverter.ToString(Encoding.Unicode.GetBytes(s)));
Console.WriteLine(BitConverter.ToString(Encoding.UTF8.GetBytes(s)));
```

```output for=csharp-unicode-is-utf16
E9-00
C3-A9
```

- **Go []byte(s) is a copy.** A change to the slice does not change the string:

```go run id=go-bytes-copy
package main

import "fmt"

func main() {
	s := "jam"
	b := []byte(s)
	b[0] = 'J'
	fmt.Println(s, string(b))
}
```

```output for=go-bytes-copy
jam Jam
```

- **Rust: not each byte sequence is a String.** `String::from_utf8` returns an error for bytes that are not UTF-8. `from_utf8_lossy()` "will replace any invalid UTF-8 sequences with U+FFFD REPLACEMENT CHARACTER"[^8], and 65533 is U+FFFD:

```rust run id=rust-from-utf8-lossy
fn main() {
    let bad = vec![0x61, 0xff];
    println!("{}", String::from_utf8(bad.clone()).is_err());
    let text = String::from_utf8_lossy(&bad);
    let points: Vec<u32> = text.chars().map(u32::from).collect();
    println!("{:?}", points);
}
```

```output for=rust-from-utf8-lossy
true
[97, 65533]
```

- **C sizeof counts the NUL.** For a string literal, the size N is "the size of the string in code units of the execution narrow encoding, including the null terminator"[^9]. So `sizeof` gives 3 for `é`, and `strlen` gives 2:

```c run id=c-sizeof-nul
#include <stdio.h>
#include <string.h>

int main(void) {
    printf("%zu %zu\n", sizeof "\xc3\xa9", strlen("\xc3\xa9"));
    return 0;
}
```

```output for=c-sizeof-nul
3 2
```

- **C++ std::string from a literal stops at a NUL byte.** The constructor from a literal "stops at the first null character"[^10]. Use the `s` suffix to keep each byte:

```cpp run id=cpp-literal-nul
#include <iostream>
#include <string>

using namespace std::string_literals;

int main() {
    std::string a = "a\0b";
    std::string b = "a\0b"s;
    std::cout << a.size() << " " << b.size() << "\n";
}
```

```output for=cpp-literal-nul
1 3
```

- **Bash echo adds a byte.** `echo` writes "the args, separated by spaces, terminated with a newline"[^11]. So `wc -c` counts 4 bytes for `abc`. Use `printf '%s'`:

```bash run id=bash-echo-newline
s='abc'
echo "$s" | wc -c | tr -d ' '
printf '%s' "$s" | wc -c | tr -d ' '
```

```output for=bash-echo-newline
4
3
```

## Related tasks

- [[encode-and-decode-utf-8]]: the UTF-8 bytes of one code point, by hand.
- [[find-the-length-of-a-string]]: the length in bytes, code units or code points.
- [[get-the-code-point-of-a-character]]: the number of a character, not its bytes.
- [[normalize-unicode-text]]: one form of the text, so that the same text gives the same bytes.

## References
