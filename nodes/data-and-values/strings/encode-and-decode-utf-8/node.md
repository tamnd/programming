---
tag: 8PZDZM
kind: task
slug: encode-and-decode-utf-8
name: Encode and decode UTF-8
aliases: [UTF-8 encoder, UTF-8 decoder, utf8 encode, utf8 decode, validate UTF-8, overlong encoding, replacement character]
area: data-and-values/strings
level: core
summary: Turn a code point into its 1 to 4 UTF-8 bytes, and turn bytes back into a code point, with a rejection of each sequence that is not well formed.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://www.rfc-editor.org/rfc/rfc3629
  title: RFC 3629, UTF-8, a transformation format of ISO 10646
  locator: 3. UTF-8 definition; 10. Security Considerations
- url: https://docs.python.org/3/library/stdtypes.html#bytes.decode
  title: The Python Standard Library, Built-in Types
  locator: bytes.decode
- url: https://pkg.go.dev/unicode/utf8#DecodeRune
  title: Go standard library, package unicode/utf8
  locator: DecodeRune
- url: https://developer.mozilla.org/en-US/docs/Web/API/TextDecoder/TextDecoder
  title: MDN Web Docs, TextDecoder() constructor
  locator: Parameters, fatal
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, Class String
  locator: String(byte[], Charset)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.text.utf8encoding.-ctor?view=net-10.0
  title: .NET API reference, UTF8Encoding constructors
  locator: Overloads
- url: https://doc.rust-lang.org/std/str/struct.Utf8Error.html
  title: Rust standard library, struct std::str::Utf8Error
  locator: error_len
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Shell-Arithmetic
  title: Bash Reference Manual
  locator: 6.5 Shell Arithmetic
---

## Task

Encode a code point to UTF-8, and decode UTF-8 to a code point. The function `encode_utf8(cp)` returns the bytes of `cp`. For example, `encode_utf8(0xE9)` is `[195, 169]`, and `encode_utf8(0x1F642)` is `[240, 159, 153, 130]`. The function `decode_utf8(b)` returns the code point of the one sequence in `b`, or -1 if `b` is not exactly one well-formed sequence.

The number of bytes comes from the code point:

| Code points | Bytes | Form |
|---|---|---|
| 0 to 0x7F | 1 | `0xxxxxxx` |
| 0x80 to 0x7FF | 2 | `110xxxxx 10xxxxxx` |
| 0x800 to 0xFFFF | 3 | `1110xxxx 10xxxxxx 10xxxxxx` |
| 0x10000 to 0x10FFFF | 4 | `11110xxx 10xxxxxx 10xxxxxx 10xxxxxx` |

"The definition of UTF-8 prohibits encoding character numbers between U+D800 and U+DFFF"[^1], so a surrogate is not a valid input or output. A decoder must also reject an overlong form, which uses more bytes than the table gives. To get the bytes of a whole string with a library, see [[convert-a-string-to-bytes]].

## Solutions

| Language | Encode | Decode, with errors |
|---|---|---|
| Python | `chr(cp).encode("utf-8")` | `bytes(b).decode("utf-8")` |
| JavaScript, TypeScript | `new TextEncoder().encode(String.fromCodePoint(cp))` | `new TextDecoder("utf-8", { fatal: true })` |
| Java | `Character.toString(cp).getBytes(UTF_8)` | a `CharsetDecoder` with `CodingErrorAction.REPORT` |
| C# | `new UTF8Encoding(false, true).GetBytes` | `new UTF8Encoding(false, true).GetString` |
| Go | `utf8.AppendRune(nil, cp)` | `utf8.DecodeRune(b)` |
| Rust | `char::encode_utf8` | `std::str::from_utf8` |
| C, C++, Bash | the bit operations of the table | the bit operations, and the checks below |

The decoder in C, C++ and Bash does these checks:

1. The first byte gives the length: below 0x80, from 0xC0, from 0xE0 or from 0xF0. A byte from 0x80 to 0xBF cannot start a sequence.
2. The input has exactly that number of bytes, and each byte after the first has the form `10xxxxxx`.
3. The code point is not below the smallest code point of its length, so an overlong form fails.
4. The code point is not a surrogate and not above 0x10FFFF.

- **Python.** For `bytes.decode`, "If 'strict' (the default), a UnicodeError exception is raised"[^2].
- **Go.** For `DecodeRune`, "An encoding is invalid if it is incorrect UTF-8, encodes a rune that is out of range, or is not the shortest possible UTF-8 encoding for the value"[^3]. For an invalid input, "it returns (RuneError, 1)"[^3].

## Edge cases

The encoder cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, and the decoder cases are in `cases/decode.jsonl`.

- `one-byte-max` and `two-byte-min`: 0x7F is 1 byte, and 0x80 is 2 bytes. Each boundary of the table has a case on each side.
- `max`: 0x10FFFF is `[244, 143, 191, 191]`, and `above-max`, `[244, 144, 128, 128]`, gives -1.
- `replacement`: `[239, 191, 189]` is a valid U+FFFD. It is not an error.
- `overlong-two` and `overlong-three`: `[192, 129]` and `[224, 128, 128]` use too many bytes, so they give -1.
- `surrogate`: `[237, 160, 128]` is U+D800, so it gives -1.
- `stray-continuation`, `truncated` and `bad-continuation`: a sequence that starts with 0x80, ends too soon or has a wrong second byte gives -1.
- `empty` and `two-sequences`: the input must be exactly one sequence.

## Pitfalls

- **Python errors="replace" hides bad input.** The handler gives U+FFFD for each bad byte, so the error is lost. Use the default, "strict":

```python run id=python-errors-replace
bad = b"\xff"
print(hex(ord(bad.decode("utf-8", errors="replace"))))
try:
    bad.decode("utf-8")
except UnicodeDecodeError as e:
    print(type(e).__name__)
```

```output for=python-errors-replace
0xfffd
UnicodeDecodeError
```

- **JavaScript TextDecoder does not throw by default.** The `fatal` option "defaults to false, which means that the decoder will substitute malformed data with a replacement character"[^4]:

```javascript run id=javascript-textdecoder-fatal
const bad = new Uint8Array([0xc1, 0x81]);
const text = new TextDecoder().decode(bad);
console.log(Array.from(text, (c) => c.codePointAt(0).toString(16)).join(" "));
try {
  new TextDecoder("utf-8", { fatal: true }).decode(bad);
} catch (e) {
  console.log(e.name);
}
```

```output for=javascript-textdecoder-fatal
fffd fffd
TypeError
```

- **Java new String(bytes, UTF_8) does not throw.** "This method always replaces malformed-input and unmappable-character sequences with this charset's default replacement string"[^5]. In the example, the 3 surrogate bytes become one U+FFFD. Use a `CharsetDecoder` to get an error:

```java run id=java-string-replaces
import java.nio.charset.StandardCharsets;

public class Main {
  public static void main(String[] args) {
    byte[] bad = {(byte) 0xed, (byte) 0xa0, (byte) 0x80};
    String s = new String(bad, StandardCharsets.UTF_8);
    StringBuilder out = new StringBuilder();
    s.codePoints().forEach(c -> out.append(Integer.toHexString(c)).append(' '));
    System.out.println(out.toString().trim());
  }
}
```

```output for=java-string-replaces
fffd
```

- **C# Encoding.UTF8 does not throw.** In the example, it gives U+FFFD for a truncated sequence. The `UTF8Encoding` constructor has a parameter that controls "whether to throw an exception when an invalid encoding is detected"[^6]:

```csharp run id=csharp-utf8-replaces
using System;
using System.Text;

byte[] bad = { 0xC3 };
Console.WriteLine(((int)Encoding.UTF8.GetString(bad)[0]).ToString("X"));
try
{
    new UTF8Encoding(false, true).GetString(bad);
}
catch (DecoderFallbackException e)
{
    Console.WriteLine(e.GetType().Name);
}
```

```output for=csharp-utf8-replaces
FFFD
DecoderFallbackException
```

- **Go string(b) does not check the bytes.** The example makes a string from bytes that are not UTF-8, and no error occurs. A range loop then gives U+FFFD for the bad byte, and the code cannot tell it from a real U+FFFD. Use `utf8.Valid` or `utf8.ValidString`:

```go run id=go-string-not-checked
package main

import (
	"fmt"
	"strings"
	"unicode/utf8"
)

func main() {
	s := string([]byte{0x61, 0xff})
	fmt.Println(len(s), utf8.ValidString(s))
	var points []string
	for _, r := range s {
		points = append(points, fmt.Sprintf("%U", r))
	}
	fmt.Println(strings.Join(points, " "))
}
```

```output for=go-string-not-checked
2 false
U+0061 U+FFFD
```

- **Rust: the end of a buffer is not always an error.** `Utf8Error::error_len` gives `None` when "the end of the input was reached unexpectedly"[^7]. In a stream, keep those bytes and add the next block. `Some(1)` is a real error:

```rust run id=rust-error-len
fn main() {
    let cut: &[u8] = &[0x61, 0xe2, 0x82];
    let bad: &[u8] = &[0x61, 0xff, 0x62];
    for b in [cut, bad] {
        if let Err(e) = std::str::from_utf8(b) {
            println!("{} {:?}", e.valid_up_to(), e.error_len());
        }
    }
}
```

```output for=rust-error-len
1 None
1 Some(1)
```

- **C: a decoder without the overlong check.** "Implementations of the decoding algorithm above MUST protect against decoding invalid sequences"[^1]. The naive decoder reads `C1 81` as 65, the letter `A`. A filter that looks for `A` in the bytes does not find it:

```c run id=c-overlong
#include <stdio.h>

// naive_decode reads a 2-byte sequence, but it does not check for an overlong form.
static unsigned naive_decode(const unsigned char *b) {
    return ((b[0] & 0x1fu) << 6) | (b[1] & 0x3fu);
}

int main(void) {
    const unsigned char overlong[] = {0xc1, 0x81};
    unsigned cp = naive_decode(overlong);
    printf("%u %s\n", cp, cp < 0x80 ? "overlong" : "ok");
    return 0;
}
```

```output for=c-overlong
65 overlong
```

- **C++ a cast to char is not an encoder.** `static_cast<char>(0xE9)` gives the 1 byte 0xE9, and a byte from 0xE0 starts a 3-byte sequence. Encode the code point to its 2 bytes:

```cpp run id=cpp-cast-not-encode
#include <iostream>
#include <string>

int main() {
    char32_t cp = U'\u00e9';
    std::string cast(1, static_cast<char>(cp));
    std::string utf8;
    utf8 += static_cast<char>(0xc0 | (cp >> 6));
    utf8 += static_cast<char>(0x80 | (cp & 0x3f));
    std::cout << cast.size() << " " << static_cast<int>(static_cast<unsigned char>(cast[0])) << "\n";
    std::cout << utf8.size() << " " << static_cast<int>(static_cast<unsigned char>(utf8[0])) << " "
              << static_cast<int>(static_cast<unsigned char>(utf8[1])) << "\n";
}
```

```output for=cpp-cast-not-encode
1 233
2 195 169
```

- **Bash: a leading 0 is octal.** "Constants with a leading 0 are interpreted as octal numbers"[^8]. Byte values with a leading 0, for example from a 3-digit format, give wrong numbers. Use the `10#` prefix:

```bash run id=bash-leading-zero-octal
b=010
echo "$((b)) $((10#$b))"
```

```output for=bash-leading-zero-octal
8 10
```

## Related tasks

- [[convert-a-string-to-bytes]]: the UTF-8 bytes of a whole string, with the library.
- [[get-the-code-point-of-a-character]]: the code point of the first character of a string.
- [[iterate-over-the-characters-of-a-string]]: decode each code point of a string in order.
- [[find-the-length-of-a-string]]: the number of bytes and the number of code points.
- [[utf-8]]: the concept page of the UTF-8 encoding form.

## References
