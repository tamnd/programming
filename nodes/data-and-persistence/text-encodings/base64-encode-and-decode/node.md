---
tag: 5BX5KR
kind: task
slug: base64-encode-and-decode
name: Base64 encode and decode
aliases: [base64 encode, base64 decode, base64url, URL-safe base64, decode a base64 string, encode bytes as text]
area: data-and-persistence/text-encodings
level: core
summary: Encode bytes as Base64 text in the standard or the URL-safe alphabet of RFC 4648, and decode the text strictly, so that bad characters, white space, wrong padding and non-zero pad bits are errors.
contract: contract.yaml
category: data-formats
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://www.rfc-editor.org/rfc/rfc4648
    title: RFC 4648, The Base16, Base32, and Base64 Data Encodings
    locator: 3.1 Line Feeds in Encoded Data, 3.2 Padding of Encoded Data, 3.3 Interpretation of Non-Alphabet Characters in Encoded Data, 3.5 Canonical Encoding, 4 Base 64 Encoding, 5 Base 64 Encoding with URL and Filename Safe Alphabet, 10 Test Vectors, 12 Security Considerations
  - url: https://www.rfc-editor.org/rfc/rfc7515
    title: RFC 7515, JSON Web Signature (JWS)
    locator: 2 Terminology, Base64url Encoding
  - url: https://www.rfc-editor.org/rfc/rfc2045
    title: RFC 2045, Multipurpose Internet Mail Extensions (MIME) Part One
    locator: 6.8 Base64 Content-Transfer-Encoding
  - url: https://docs.python.org/3/library/base64.html
    title: The Python Standard Library, base64
    locator: b64encode, b64decode, urlsafe_b64encode
  - url: https://nodejs.org/api/buffer.html#buffers-and-character-encodings
    title: Node.js documentation, Buffer
    locator: Buffers and character encodings, 'base64' and 'base64url'
  - url: https://developer.mozilla.org/en-US/docs/Web/API/Window/btoa
    title: MDN Web Docs, Window.btoa()
    locator: Exceptions, Unicode strings
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Base64.html
    title: Java SE 25 API, java.util.Base64
    locator: class description (Basic, URL and Filename safe, MIME)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Base64.Encoder.html
    title: Java SE 25 API, java.util.Base64.Encoder
    locator: withoutPadding()
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Base64.Decoder.html
    title: Java SE 25 API, java.util.Base64.Decoder
    locator: class description
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.convert.frombase64string?view=net-10.0
    title: .NET API reference, Convert.FromBase64String
    locator: Remarks, Exceptions
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.buffers.text.base64url?view=net-10.0
    title: .NET API reference, Base64Url
    locator: EncodeToString, DecodeFromChars
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.text.encoding.unicode?view=net-10.0
    title: .NET API reference, Encoding.Unicode
    locator: Definition
  - url: https://pkg.go.dev/encoding/base64
    title: Go standard library, package encoding/base64
    locator: StdEncoding, RawURLEncoding, Encoding.Strict, Encoding.Decode
  - url: https://man7.org/linux/man-pages/man1/base64.1.html
    title: Linux man-pages, base64(1)
    locator: Description, -d, -w
---

## Task

Encode bytes as Base64 text, and decode the text back to the bytes. Each solution has two functions, `encode(data, alphabet)` and `decode(text, alphabet)`. The data is an array of bytes, not a string. To encode text, first get its UTF-8 bytes (see [[utf-8]]).

Base64 puts each group of 3 bytes, or 24 bits, into 4 characters of 6 bits. RFC 4648 defines two alphabets of 64 characters. Both give the values 0 to 61 to `A` to `Z`, `a` to `z` and `0` to `9`. They differ in the values 62 and 63:

- `standard` (section 4) uses `+` and `/`. The encoder pads the text with `=` to a multiple of 4 characters.
- `url` (section 5) uses `-` and `_`, which are safe in URLs and file names. The encoder adds no padding. This is the form of RFC 7515, which omits the trailing `=` and allows no line breaks or white space.

A last group of 1 byte gives 2 characters, and a last group of 2 bytes gives 3 characters. The last character then has bits that hold no data. These are the pad bits, and section 3.5 says that the encoder sets them to zero.

The decoder is strict. It accepts a text only when the encoder gives exactly that text for some bytes. So each byte string has one valid text in each alphabet. `decode` reports the error `invalid` for these inputs:

- a character outside the alphabet, for example `-` in standard mode or `+` in url mode
- white space and line breaks
- a missing, extra or misplaced `=`, and any `=` in url mode
- a length that is 1 mod 4, because one character holds only 6 bits
- a last character with pad bits that are not zero

RFC 4648 supports these rules. Section 3.3 says that an implementation must reject characters outside the base alphabet, unless the referring specification says otherwise. Section 12 says that a decoder that ignores such characters makes a covert channel possible. Section 3.5 allows a decoder to reject an encoding whose pad bits are not zero.

## Solutions

- **Python.** `base64.b64encode`, and `base64.urlsafe_b64encode` with `rstrip("=")` for url. The decoder calls `base64.b64decode` with `validate=True`, so a character outside the alphabet raises an error. For url mode it adds the padding and gives `altchars=b"-_"`. `validate=True` still accepts non-zero pad bits (see the pitfalls). So the solution encodes the result again and compares it with the text. Only the canonical text gives itself back, so this one check covers every rule of the contract.
- **JavaScript and TypeScript (Node.js).** `buf.toString("base64")` and `buf.toString("base64url")`. The Node.js documentation says that `base64url` omits the padding when it encodes. `Buffer.from(text, encoding)` decodes, but it accepts both alphabets and skips other characters. So the solution uses the same check: encode again and compare. The TypeScript file adds types only.
- **Java.** `Base64.getEncoder()` and `Base64.getUrlEncoder().withoutPadding()`. The decoders of `getDecoder()` and `getUrlDecoder()` throw `IllegalArgumentException` for a character outside the alphabet. The `Base64.Decoder` documentation says that they accept the padding but do not require it. So the solution adds the check.
- **C#.** `Convert.ToBase64String`, and `Base64Url.EncodeToString` of `System.Buffers.Text`. `Convert.FromBase64String` and `Base64Url.DecodeFromChars` throw `FormatException` for bad text. The `FromBase64String` documentation says that it ignores white space. So the solution adds the check.
- **Go.** `base64.StdEncoding`, and `base64.RawURLEncoding`, which the documentation describes as the url encoding with no padding characters. The decoder calls `Strict()`, which requires zero pad bits. The `Strict` documentation adds that the decoder still ignores the new line characters CR and LF. So the solution rejects a text with `\r` or `\n` first.
- **Rust.** The standard library has no Base64, so the solution implements it. The encoder takes 3 bytes at a time from a table of 64 characters. The decoder removes at most two `=` in standard mode, and a lookup table of 256 entries rejects every other byte that is not in the alphabet. Then it checks the length and the pad bits.
- **C.** The same algorithm as Rust. `b64_encode` returns a new string. `b64_decode` returns `B64_OK`, `B64_INVALID` or `B64_NO_MEMORY`. Before the `malloc`, `b64_encode` checks that the output size fits in `size_t` (see [[integer-overflow]]).
- **C++.** The same algorithm, with `std::span<const std::uint8_t>` and `std::string_view`. `decode` throws `std::invalid_argument`.
- **Bash.** `b64_encode` sends the bytes on the standard input to the `base64` command and removes every newline. GNU `base64` wraps its output after 76 characters by default. For url mode, parameter expansion changes `+` and `/` and removes the padding. `b64_decode` first checks the text with one regular expression in `[[ =~ ]]`. The expression checks the alphabet, the length, the padding and the pad bits. Before `==`, the last data character must be `A`, `Q`, `g` or `w`, because their values are multiples of 16. Before `=`, its value must be a multiple of 4. Then the function changes `-` and `_` back, adds the padding and runs `base64 -d`. The bracket expressions list each character, so the locale does not change them.

## Edge cases

The cases of `encode` are in `cases/basic.jsonl` and `cases/edge.jsonl`. The bytes are lowercase hexadecimal, because the contract types have no type for bytes. Each test also decodes the result and checks that it gives the bytes back.

- `rfc-f` to `rfc-foobar`: the test vectors of RFC 4648 section 10.
- `empty-standard` and `empty-url`: no bytes give the empty text.
- `two-pad-chars`, `one-pad-char`, `url-one-byte` and `url-two-bytes`: a last group of 1 or 2 bytes, with and without padding.
- `zero-bytes`: the bytes `00 00 00` give `AAAA`.
- `plus-and-slash`, `minus-and-underscore`, `all-62-*` and `all-63-*`: the bytes `fb ff` give `+/8=` in standard and `-_8` in url. These cases catch a solution that uses the wrong alphabet.
- `non-ascii-standard` and `non-ascii-url`: the UTF-8 bytes of `é€🙂`.

The cases of `decode` are in `cases/decode.jsonl`. The toolchain does not load this file yet, so the test files hold its cases by name. The cases in `cases/errors.jsonl` must give the error `invalid`. The test files hold them by name too:

- `plus-in-url`, `minus-in-standard`, `bang` and `non-ascii-char`: characters outside the alphabet.
- `space`, `line-feed`, `line-feed-url` and `trailing-crlf`: white space and line breaks.
- `missing-padding`, `missing-one-pad`, `excess-padding`, `padding-in-middle`, `three-pads`, `only-padding`, `padding-then-data` and `padding-in-url`: a wrong padding.
- `length-1-mod-4-standard`, `length-1-mod-4-url` and `one-char-url`: a length that is 1 mod 4.
- `pad-bits-one-byte`, `pad-bits-two-bytes` and `pad-bits-url`: `QR==`, `Zm9=` and `Zm9` have non-zero pad bits.

## Pitfalls

- **Lenient decoders accept bad text.** `Buffer.from(text, "base64")` in Node.js accepts both alphabets and a missing padding, and it skips characters outside the alphabet. The documentation says that `base64` also accepts the URL-safe alphabet and ignores white space. No input gives an error:

```javascript wrong id=javascript-lenient
const texts = ["Zm9vYmFy", "Zm9v YmFy", "Zm9v!YmFy", "Zm9vYmF=", "-_8=", "+/8", "!!!!"];
for (const text of texts) {
  console.log(JSON.stringify(text).padEnd(12), Buffer.from(text, "base64").toString("hex"));
}
```

```output for=javascript-lenient
"Zm9vYmFy"   666f6f626172
"Zm9v YmFy"  666f6f626172
"Zm9v!YmFy"  666f6f626172
"Zm9vYmF="   666f6f6261
"-_8="       fbff
"+/8"        fbff
"!!!!"
```

To be strict, encode the result again and compare it with the text, as the solutions do.

- **Non-zero pad bits give the same bytes.** `b64decode` with `validate=True` rejects characters outside the alphabet, but it ignores the pad bits. So four texts decode to the same two bytes:

```python wrong id=python-pad-bits
import base64

for text in ["Zm8=", "Zm9=", "Zm+=", "Zm/="]:
    print(text, base64.b64decode(text, validate=True))
```

```output for=python-pad-bits
Zm8= b'fo'
Zm9= b'fo'
Zm+= b'fo'
Zm/= b'fo'
```

Code that compares Base64 texts, for example as keys or signatures, then sees four different values for the same data.

- **MIME text has line breaks.** RFC 2045 limits a MIME line to 76 characters, and RFC 4648 section 3.1 forbids line feeds unless the referring specification asks for them. Java's MIME encoder adds `\r\n` after each 76 characters, and the basic decoder rejects the `\r`:

```java run id=java-mime
import java.util.Base64;

public class Main {
    public static void main(String[] args) {
        String mime = Base64.getMimeEncoder().encodeToString(new byte[60]);
        System.out.println("length " + mime.length() + ", line break at " + mime.indexOf("\r\n"));
        try {
            Base64.getDecoder().decode(mime);
        } catch (IllegalArgumentException e) {
            System.out.println("getDecoder: " + e.getMessage());
        }
        System.out.println("getMimeDecoder: " + Base64.getMimeDecoder().decode(mime).length + " bytes");
    }
}
```

```output for=java-mime
length 82, line break at 76
getDecoder: Illegal base64 character d
getMimeDecoder: 60 bytes
```

GNU `base64` wraps after 76 characters too. Give `-w 0` to turn the wrap off, or remove the newlines as the Bash solution does.

- **The standard decoder drops the URL-safe characters.** Without `validate=True`, Python's `b64decode` discards the characters outside the standard alphabet before it checks the padding. So it reads url text with no error and returns other bytes:

```python wrong id=python-wrong-alphabet
import base64

text = base64.urlsafe_b64encode(bytes.fromhex("fbffbf") + b"foo").decode()
print(text)
print(base64.b64decode(text))
print(base64.urlsafe_b64decode(text))
```

```output for=python-wrong-alphabet
-_-_Zm9v
b'foo'
b'\xfb\xff\xbffoo'
```

Decode with the alphabet that the text uses, and decode strictly.

- **JWT parts have no padding.** RFC 7515 removes the trailing `=`. A decoder that requires the padding rejects such a part. In C#, `Convert.FromBase64String` throws, and `Base64Url.DecodeFromChars` decodes it:

```csharp run id=csharp-jwt-padding
using System.Buffers.Text;
using System.Text;

string header = "eyJhbGciOiJub25lIn0";
try
{
    Convert.FromBase64String(header);
}
catch (FormatException e)
{
    Console.WriteLine("FromBase64String: " + e.GetType().Name);
}
Console.WriteLine("Base64Url: " + Encoding.UTF8.GetString(Base64Url.DecodeFromChars(header)));
```

```output for=csharp-jwt-padding
FromBase64String: FormatException
Base64Url: {"alg":"none"}
```

- **The bytes of a string depend on its encoding.** In C#, `Encoding.Unicode` is UTF-16 in little-endian byte order, not UTF-8. The same string gives two different Base64 texts:

```csharp wrong id=csharp-utf16
using System.Text;

Console.WriteLine("Unicode: " + Convert.ToBase64String(Encoding.Unicode.GetBytes("hi")));
Console.WriteLine("UTF8:    " + Convert.ToBase64String(Encoding.UTF8.GetBytes("hi")));
```

```output for=csharp-utf16
Unicode: aABpAA==
UTF8:    aGk=
```

Use UTF-8, unless the other side of the exchange asks for another encoding.

- **`btoa` takes bytes, not text.** `btoa` reads each character of its argument as one byte. MDN says that it throws `InvalidCharacterError` for a character above U+00FF. Encode the text as UTF-8 first:

```javascript run id=javascript-btoa
try {
  btoa("€");
} catch (e) {
  console.log("btoa:", e.name);
}
const bytes = new TextEncoder().encode("€");
console.log("UTF-8:", Buffer.from(bytes).toString("base64"));
```

```output for=javascript-btoa
btoa: InvalidCharacterError
UTF-8: 4oKs
```

## Related tasks

- [[utf-8]]: get the bytes of a string before you encode it.
- [[compute-a-sha-256-hash]]: a digest is bytes, and Base64 is one way to write it as text.
- [[read-and-write-json]]: a JSON string can carry bytes as Base64 text (see [[json]]).
- [[read-from-standard-input]]: the Bash solution reads the bytes on the standard input.
- [[write-a-file]]: write the decoded bytes to a file.
- [[deserialization-of-untrusted-data]]: decoded bytes from an untrusted source are still untrusted.

## References
