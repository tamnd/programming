---
tag: 3W23QF
kind: format
slug: utf-8
name: UTF-8
aliases: [FSS-UTF, UTF-2]
area: data-and-persistence/text-encodings
level: core
summary: >-
  UTF-8 is the encoding that writes each Unicode scalar value as a sequence of one to four bytes. The bytes 00 to 7F are the same as ASCII, and no byte of a longer sequence is in that range.
history:
  year: 1992
  people: [Ken Thompson, Rob Pike]
  note: Thompson designed UTF-8 in September 1992, with design criteria from Pike, for the Plan 9 operating system. X/Open took it into standardization under the names FSS-UTF and UTF-2.
same_as:
  wikidata: Q193537
  wikipedia: UTF-8
sources:
  - url: https://www.rfc-editor.org/rfc/rfc3629
    title: "F. Yergeau, RFC 3629: UTF-8, a transformation format of ISO 10646, November 2003 (STD 63)"
    locator: 1 Introduction; 3 UTF-8 definition; 4 Syntax of UTF-8 Byte Sequences; 6 Byte order mark (BOM); 7 Examples; 10 Security Considerations; 12 Changes from RFC 2279
  - url: https://www.unicode.org/versions/Unicode18.0.0/core-spec/chapter-3/
    title: The Unicode Standard, Version 18.0.0, Chapter 3, Conformance
    locator: 3.8 Surrogates; 3.9 Unicode Encoding Forms (D76, D92, Table 3-6, Table 3-7); 3.9.6 U+FFFD Substitution of Maximal Subparts; 3.10 Unicode Encoding Schemes (D95)
  - url: https://www.unicode.org/versions/Unicode18.0.0/core-spec/chapter-2/
    title: The Unicode Standard, Version 18.0.0, Chapter 2, General Structure
    locator: 2.5 Encoding Forms, UTF-8 (variable width, data size, binary sorting)
  - url: https://www.unicode.org/versions/Unicode18.0.0/core-spec/chapter-23/
    title: The Unicode Standard, Version 18.0.0, Chapter 23, Special Areas and Format Characters
    locator: 23.8 Specials, Byte Order Mark (BOM)
  - url: https://9p.io/sys/doc/utf.html
    title: "R. Pike and K. Thompson, Hello World or Καλημέρα κόσμε or こんにちは 世界, Proc. of the Winter 1993 USENIX Conf., pp. 43-50"
    locator: the sections Standards, UTF and the notes
  - url: https://www.cl.cam.ac.uk/~mgk25/ucs/utf-8-history.txt
    title: Rob Pike, UTF-8 history, e-mail of 30 April 2003, with the mail of September 1992 from the Plan 9 archive
    locator: the first message; the mail of Tue Sep 8 03:22:07 EDT 1992
  - url: https://www.rfc-editor.org/rfc/rfc2279
    title: "F. Yergeau, RFC 2279: UTF-8, a transformation format of ISO 10646, January 1998"
    locator: the header; 1 Introduction; 2 UTF-8 definition
  - url: https://www.rfc-editor.org/rfc/rfc2044
    title: "F. Yergeau, RFC 2044: UTF-8, a transformation format of Unicode and ISO 10646, October 1996"
    locator: the header
  - url: https://www.rfc-editor.org/rfc/rfc2277
    title: "H. Alvestrand, RFC 2277: IETF Policy on Character Sets and Languages, January 1998 (BCP 18)"
    locator: 3.1 What charset to use
  - url: https://encoding.spec.whatwg.org/
    title: WHATWG, Encoding Standard, Living Standard
    locator: Preface; 4.2 Names and labels; 6 Hooks for standards; 8.1.1 UTF-8 decoder
  - url: https://www.unicode.org/reports/tr15/tr15-58.html
    title: Unicode Standard Annex #15, Unicode Normalization Forms, revision 58
    locator: 1.1 Canonical and Compatibility Equivalence; 1.2 Normalization Forms, Table 1
  - url: https://www.unicode.org/reports/tr29/tr29-49.html
    title: Unicode Standard Annex #29, Unicode Text Segmentation, revision 49
    locator: 3 Grapheme Cluster Boundaries, Table 1a
  - url: https://docs.python.org/3.14/library/codecs.html
    title: The Python Standard Library, codecs, Python 3.14
    locator: Error Handlers; Encodings and Unicode; encodings.utf_8_sig
  - url: https://docs.python.org/3.14/library/unicodedata.html
    title: The Python Standard Library, unicodedata, Python 3.14
    locator: unicodedata.normalize
  - url: https://pkg.go.dev/unicode/utf8
    title: Go standard library, package unicode/utf8
    locator: RuneError; EncodeRune; DecodeRuneInString; RuneCount; ValidString
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Source code representation; For statements with range clause
  - url: https://doc.rust-lang.org/std/str/struct.Utf8Error.html
    title: Rust standard library, std::str::Utf8Error
    locator: valid_up_to; error_len
  - url: https://doc.rust-lang.org/std/primitive.char.html
    title: Rust standard library, primitive type char
    locator: Validity and Layout
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/DataInput.html
    title: Java SE 25 API, java.io.DataInput
    locator: Modified UTF-8
  - url: https://openjdk.org/jeps/400
    title: "JEP 400: UTF-8 by Default"
    locator: Summary; Goals
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.text.utf8encoding
    title: .NET API, UTF8Encoding Class
    locator: Remarks (the table of constructors, the BOM and error detection)
  - url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/character-encoding-introduction
    title: .NET documentation, Character encoding in .NET
    locator: The string and char types; Grapheme clusters
  - url: https://tc39.es/ecma262/2026/
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 6.1.4 The String Type; 22.1.3.10 String.prototype.isWellFormed; 22.1.3.15 String.prototype.normalize; 22.1.3.31 String.prototype.toWellFormed
  - url: https://developer.mozilla.org/en-US/docs/Web/API/TextDecoder/TextDecoder
    title: MDN Web Docs, TextDecoder() constructor
    locator: the options fatal and ignoreBOM
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/Segmenter
    title: MDN Web Docs, Intl.Segmenter
    locator: the description
  - url: https://www.sqlite.org/lang_corefunc.html
    title: SQLite, Built-In Scalar SQL Functions
    locator: length(X); octet_length(X); hex(X); unicode(X); char(X1,X2,...,XN)
  - url: https://dev.mysql.com/doc/refman/8.4/en/charset-unicode-utf8mb3.html
    title: MySQL 8.4 Reference Manual, 12.9.2 The utf8mb3 Character Set (3-Byte UTF-8 Unicode Encoding)
    locator: the list of characteristics
  - url: https://html.spec.whatwg.org/multipage/semantics.html
    title: WHATWG, HTML Living Standard
    locator: 4.2.5.4 Specifying the document's character encoding
  - url: https://drafts.csswg.org/css-syntax-3/
    title: CSS Syntax Module Level 3, Editor's Draft
    locator: 3.2 The input byte stream (determine the fallback encoding); 8.3 The @charset Rule
  - url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.text/decode-to-string.html
    title: Kotlin standard library, decodeToString
    locator: the two signatures and the parameter throwOnInvalidSequence
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_character_encoding
    title: PowerShell documentation, about_Character_Encoding
    locator: the values of the Encoding parameter; the default encoding
  - url: https://www.gnu.org/software/bash/manual/bash.html
    title: GNU Bash Reference Manual
    locator: 3.5.3 Shell Parameter Expansion, ${#parameter}
  - url: https://eel.is/c++draft/depr.locale.stdcvt
    title: Working Draft, Programming Languages, C++, [depr.locale.stdcvt]
    locator: the stable name (the subclause was removed)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
    title: Java SE 25 API, java.lang.String
    locator: the class description; String(byte[], Charset); getBytes(Charset)
---

## What it is

UTF-8 is a character encoding. It writes each Unicode scalar value as a sequence of one to four bytes.[^2] A scalar value is a code point in the range U+0000 to U+10FFFF, other than a surrogate code point (U+D800 to U+DFFF). The Unicode Standard defines UTF-8, and RFC 3629 of the IETF defines the same encoding for the protocols of the Internet.[^1]

The encoding has a small set of properties, and most of its uses follow from them:[^1]

- The code points U+0000 to U+007F are one byte each, with the same values as ASCII. So each ASCII text is also a UTF-8 text.
- The bytes 00 to 7F never occur inside a longer sequence. A program that looks only for ASCII bytes, such as `/`, `,` or the newline, can process UTF-8 text without decoding it.
- The first byte of a sequence gives the length of the sequence. The other bytes have a fixed bit pattern, so a reader can find the start of a character from any position in the stream.
- The bytes C0, C1 and F5 to FF never occur.
- The byte order of two UTF-8 strings is the same as the order of their code points.

A program that handles text uses three different units, and UTF-8 makes the difference between them visible. A byte is a code unit of UTF-8. A code point is the number of one character in Unicode. A grapheme cluster is what a reader sees as one character.[^12] The letter "é" can be one code point (U+00E9) or two (U+0065 U+0301). The flag of Vietnam is two code points and eight bytes. A string has a different length in each unit, and each language counts a different unit by default (see Examples).

### History

Rob Pike and Ken Thompson needed an encoding for the Plan 9 operating system in 1992. They first used the original UTF of ISO 10646. That encoding was not self-synchronizing: a reader could not find the character boundaries without a read from the start of the string. It also needed multiplication, division and modulo to encode and decode.[^5] In August 1992, X/Open circulated a proposal for a byte encoding that was safe for file names.[^5]

Pike tells the rest in an e-mail of 2003. Some members of an X/Open committee asked Pike and Thompson to check their design, FSS/UTF. That design could not resynchronize a byte stream that a reader picked up in the middle. Thompson designed a new bit packing "on a placemat in a New Jersey diner one night in September or so 1992". He wrote the code that night, and by the Friday Plan 9 ran only on the new encoding.[^6] The Plan 9 mail archive has the proposal that Thompson sent to X/Open on Tuesday 8 September 1992.[^6] The paper of Pike and Thompson at the Winter 1993 USENIX Conference says that their proposal, "originally known informally as UTF-2 and FSS-UTF, is now referred to as UTF-8", and that ISO approved it to become Annex P to ISO 10646.[^5]

RFC 2044 (October 1996, Informational) and RFC 2279 (January 1998, Standards Track) defined UTF-8 for the Internet.[^8][^7] RFC 2279 says that UTF-8 was a project of the X/Open Joint Internationalization Group, with Gary Miller, Greger Leijonhufvud and John Entenmann as the original authors, and that Thompson and Pike "did significant work for the formal UTF-8".[^7] Pike wrote in 2003 that this story "originates in RFC2279" and is not correct.[^6] RFC 3629 of November 2003 says that Thompson devised UTF-8, "guided by design criteria specified by Rob Pike".[^1]

## Syntax or structure

Table 3-6 of the Unicode Standard and Section 3 of RFC 3629 give the bit layout. The letter x marks a bit of the code point.[^2][^1]

```text
Code points           Byte 1    Byte 2    Byte 3    Byte 4    Bits
U+0000  ..U+007F      0xxxxxxx                                  7
U+0080  ..U+07FF      110xxxxx  10xxxxxx                       11
U+0800  ..U+FFFF      1110xxxx  10xxxxxx  10xxxxxx             16
U+10000 ..U+10FFFF    11110xxx  10xxxxxx  10xxxxxx  10xxxxxx   21
```

The number of leading 1 bits in the first byte is the length of the sequence. A first byte that starts with 0 is a sequence of one byte. Each continuation byte starts with the bits 10, so it is in the range 80 to BF. A reader that starts in the middle of a stream skips the continuation bytes and finds the next first byte. A program needs to search at most four bytes back to find the start of a character.[^3]

The layout alone allows more than one sequence for some code points. The bytes `C0 AF` fit the pattern of two bytes and hold the value 2F, the code of `/`. The bytes `ED A0 80` fit the pattern of three bytes and hold the surrogate D800. These sequences are ill-formed. Table 3-7 of the Unicode Standard lists the only well-formed sequences, and RFC 3629 gives the same rules in ABNF.[^2][^1]

```abnf
UTF8-octets = *( UTF8-char )
UTF8-char   = UTF8-1 / UTF8-2 / UTF8-3 / UTF8-4
UTF8-1      = %x00-7F
UTF8-2      = %xC2-DF UTF8-tail
UTF8-3      = %xE0 %xA0-BF UTF8-tail / %xE1-EC 2( UTF8-tail ) /
              %xED %x80-9F UTF8-tail / %xEE-EF 2( UTF8-tail )
UTF8-4      = %xF0 %x90-BF 2( UTF8-tail ) / %xF1-F3 3( UTF8-tail ) /
              %xF4 %x80-8F 2( UTF8-tail )
UTF8-tail   = %x80-BF
```

Each rule closes one gap of the bit layout:

- **Overlong forms.** A sequence that is longer than necessary is ill-formed. The first byte C0 or C1 can only start an overlong form of two bytes, so these two bytes never occur. After E0, the second byte must be A0 or more. After F0, it must be 90 or more. So the shortest form is the only form.
- **Surrogates.** The code points U+D800 to U+DFFF are not scalar values. UTF-16 uses them in pairs for the code points above U+FFFF.[^2] After ED, the second byte must be 9F or less, so UTF-8 cannot encode a surrogate.
- **The upper limit.** After F4, the second byte must be 8F or less. The first bytes F5 to FF are not used. So the largest value is U+10FFFF, the largest code point that UTF-16 can reach.[^1]

These checks matter for security. A decoder that accepts an overlong form lets a forbidden byte pass a filter in a different form. RFC 3629 gives the example of a parser that rejects the bytes `2F 2E 2E 2F` ("/../") but accepts `2F C0 AE 2E 2F` and reads it as the same path. It says that a virus used this attack against web servers in 2001.[^1] So RFC 3629 says that a decoder MUST protect against the decode of invalid sequences.[^1]

### The byte order mark

The character U+FEFF has the formal name ZERO WIDTH NO-BREAK SPACE, and the informal name byte order mark (BOM). In UTF-8 it is the bytes `EF BB BF`.[^1] UTF-8 has one byte as its code unit, so it has no byte order.[^1] At the start of a text, the bytes `EF BB BF` are a strong sign that the text is UTF-8. The Unicode Standard does not require a BOM in UTF-8, and a BOM does not make a text non-conformant.[^2] RFC 3629 says that U+FEFF anywhere other than at the start of a stream MUST be read as a zero width no-break space. It says that a protocol SHOULD forbid the mark when the protocol always uses UTF-8.[^1] A program that reads the bytes `EF BB BF` with the code page Windows-1252 shows them as "ï»¿".[^4]

### Ill-formed input and U+FFFD

A decoder that finds an ill-formed sequence can stop with an error, or it can put U+FFFD REPLACEMENT CHARACTER in place of the bad bytes. The Unicode Standard requires only that the decoder does not consume a well-formed sequence as part of the error. It describes a practice for the number of U+FFFD characters, the "substitution of maximal subparts". The practice is not required for conformance.[^2] A maximal subpart is the longest start of a sequence that can still be well-formed. Each maximal subpart becomes one U+FFFD, and each byte that cannot start a sequence becomes one U+FFFD. The Unicode Standard says that this practice is the one of the W3C encoding standard, the WHATWG Encoding Standard.[^2][^10]

## Examples

The function `encode` below follows Table 3-6. It checks the result against the codec of Python for one character of each length.[^13]

```python run id=encode
def encode(code_point):
    if code_point < 0x80:
        return bytes([code_point])
    if code_point < 0x800:
        return bytes([0xC0 | code_point >> 6, 0x80 | code_point & 0x3F])
    if code_point < 0x10000:
        if 0xD800 <= code_point <= 0xDFFF:
            raise ValueError(f"U+{code_point:04X} is a surrogate")
        return bytes([0xE0 | code_point >> 12, 0x80 | code_point >> 6 & 0x3F, 0x80 | code_point & 0x3F])
    if code_point <= 0x10FFFF:
        return bytes([0xF0 | code_point >> 18, 0x80 | code_point >> 12 & 0x3F, 0x80 | code_point >> 6 & 0x3F, 0x80 | code_point & 0x3F])
    raise ValueError(f"U+{code_point:X} is above U+10FFFF")


for code_point in 0x41, 0xE9, 0x20AC, 0x1D11E:
    data = encode(code_point)
    assert data == chr(code_point).encode("utf-8")
    bits = " ".join(f"{byte:08b}" for byte in data)
    print(f"U+{code_point:04X} {len(data)} {data.hex(' ').upper():11} {bits}")

for code_point in 0xDC00, 0x110000:
    try:
        encode(code_point)
    except ValueError as error:
        print(error)
```

```output for=encode
U+0041 1 41          01000001
U+00E9 2 C3 A9       11000011 10101001
U+20AC 3 E2 82 AC    11100010 10000010 10101100
U+1D11E 4 F0 9D 84 9E 11110000 10011101 10000100 10011110
U+DC00 is a surrogate
U+110000 is above U+10FFFF
```

The rules of Table 3-7 form a regular language, so a regular expression over bytes can check UTF-8. The pattern below has one branch for each row of the table. It agrees with the strict decoder of Python on each sample. The samples `E0 9F 80` and `F4 80 83 92` are the examples of the Unicode Standard.[^2]

```python run id=byte-pattern
import re

WELL_FORMED = re.compile(
    rb"""(?:
      [\x00-\x7F]
    | [\xC2-\xDF][\x80-\xBF]
    | \xE0[\xA0-\xBF][\x80-\xBF]
    | [\xE1-\xEC\xEE\xEF][\x80-\xBF]{2}
    | \xED[\x80-\x9F][\x80-\xBF]
    | \xF0[\x90-\xBF][\x80-\xBF]{2}
    | [\xF1-\xF3][\x80-\xBF]{3}
    | \xF4[\x80-\x8F][\x80-\xBF]{2}
    )*""",
    re.VERBOSE,
)

samples = [b"h\xc3\xa9\xe2\x82\xac", b"\xc0\xaf", b"\xe0\x9f\x80", b"\xed\xa0\x80", b"\xf4\x80\x83\x92", b"\xf4\x90\x80\x80"]
for data in samples:
    matched = WELL_FORMED.fullmatch(data) is not None
    try:
        data.decode("utf-8")
        decoded = True
    except UnicodeDecodeError:
        decoded = False
    print(f"{data.hex(' '):17} {matched} {decoded}")
```

```output for=byte-pattern
68 c3 a9 e2 82 ac True True
c0 af             False False
e0 9f 80          False False
ed a0 80          False False
f4 80 83 92       True True
f4 90 80 80       False False
```

The strict mode of the Python codec raises `UnicodeDecodeError` for ill-formed input. The error handler `replace` puts U+FFFD in place of the bad bytes.[^13] The count of U+FFFD shows the maximal subparts. The surrogate `ED A0 80` gives three, because no well-formed sequence starts with `ED A0`. The truncated `E2 82` gives one, because it is the start of a well-formed sequence.

```python run id=ill-formed
samples = {
    "overlong /": b"\xc0\xaf",
    "overlong NUL": b"\xc0\x80",
    "surrogate": b"\xed\xa0\x80",
    "above 10FFFF": b"\xf4\x90\x80\x80",
    "truncated": b"\xe2\x82",
    "lone tail": b"\x80",
}
for name, data in samples.items():
    try:
        data.decode("utf-8")
    except UnicodeDecodeError as error:
        replaced = data.decode("utf-8", errors="replace")
        print(f"{name:13} {data.hex(' '):12} {replaced.count(chr(0xFFFD))} x U+FFFD  {error.reason}")
```

```output for=ill-formed
overlong /    c0 af        2 x U+FFFD  invalid start byte
overlong NUL  c0 80        2 x U+FFFD  invalid start byte
surrogate     ed a0 80     3 x U+FFFD  invalid continuation byte
above 10FFFF  f4 90 80 80  4 x U+FFFD  invalid continuation byte
truncated     e2 82        1 x U+FFFD  unexpected end of data
lone tail     80           1 x U+FFFD  invalid start byte
```

A Python `str` is a sequence of code points, so `len` counts code points. The example counts the same strings in three units.

```python run id=lengths
samples = {
    "e and accent": "e\u0301",
    "flag": "\U0001F1FB\U0001F1F3",
    "woman coder": "\U0001F469\u200D\U0001F4BB",
    "hangul": "\uD55C",
}
for name, text in samples.items():
    code_points = " ".join(f"U+{ord(c):04X}" for c in text)
    utf8 = len(text.encode("utf-8"))
    utf16 = len(text.encode("utf-16-le")) // 2
    print(f"{name:13} {code_points:22} bytes={utf8:2} utf16={utf16} code_points={len(text)}")
```

```output for=lengths
e and accent  U+0065 U+0301          bytes= 3 utf16=2 code_points=2
flag          U+1F1FB U+1F1F3        bytes= 8 utf16=4 code_points=2
woman coder   U+1F469 U+200D U+1F4BB bytes=11 utf16=5 code_points=3
hangul        U+D55C                 bytes= 3 utf16=1 code_points=1
```

Each of these strings is one grapheme cluster. The flag is two regional indicator symbols. The woman coder is two emoji with U+200D ZERO WIDTH JOINER between them. The example files for JavaScript, Java and C# count the grapheme clusters too.

Two strings can look the same and differ in their code points, so their bytes differ too. UTF-8 does not change this: it gives one byte sequence for each sequence of code points.[^1] Normalization maps equivalent strings to one form. NFC composes and NFD decomposes. NFKC and NFKD also replace compatibility characters, such as the ligature "ﬁ", with their plain forms.[^11][^14]

```python run id=normalize
import unicodedata

composed = "caf\u00e9"
decomposed = "cafe\u0301"
print(composed == decomposed, len(composed), len(decomposed))
print(composed.encode("utf-8").hex(" "), "|", decomposed.encode("utf-8").hex(" "))
print(unicodedata.normalize("NFC", decomposed) == composed)
print(unicodedata.normalize("NFD", composed) == decomposed)
print(unicodedata.normalize("NFKC", "\uFB01le \u2460"))
```

```output for=normalize
False 4 5
63 61 66 c3 a9 | 63 61 66 65 cc 81
True
True
file 1
```

The codec `utf-8` of Python keeps a leading BOM as the character U+FEFF. The codec `utf-8-sig` removes it on decode and writes it on encode.[^13]

```python run id=bom
data = b"\xef\xbb\xbfid,name\n"
print(repr(data.decode("utf-8")))
print(repr(data.decode("utf-8-sig")))
print("name".encode("utf-8-sig").hex(" "))
print(data.decode("cp1252")[:3], "caf\u00e9".encode("utf-8").decode("cp1252"))
```

```output for=bom
'\ufeffid,name\n'
'id,name\n'
ef bb bf 6e 61 6d 65
ï»¿ cafÃ©
```

## Versions

The bit patterns of the sequences of one to four bytes are the same in each RFC. The changes are in the range of code points and in the rules for invalid input.

| Document | Date | Status | Change |
|---|---|---|---|
| Thompson's proposal to X/Open | September 1992 | proposal | the encoding, sent as the "modified FSS-UTF proposal"[^6] |
| RFC 2044 | October 1996 | Informational | UTF-8 as an RFC; sequences of up to six bytes[^8] |
| RFC 2279 | January 1998 | Standards Track | obsoletes RFC 2044; sequences of up to six bytes, for values up to 7FFFFFFF[^7] |
| RFC 2277 (BCP 18) | January 1998 | Best Current Practice | "Protocols MUST be able to use the UTF-8 charset"[^9] |
| RFC 3629 (STD 63) | November 2003 | Internet Standard | obsoletes RFC 2279; see below[^1] |

RFC 3629 changed these points of RFC 2279:[^1]

- It restricts the range to U+0000 to U+10FFFF, the range that UTF-16 can reach. So a sequence has at most four bytes.
- It makes the Unicode Standard the source of the normative definition.
- It turns the note against the decode of invalid sequences into a MUST NOT.
- It adds the section on the BOM and the ABNF grammar.

ISO/IEC 10646 once allowed values up to U+7FFFFFFF, with sequences of up to six bytes. RFC 3629 warns that a buffer can overflow if a program does not limit the range to U+10FFFF.[^1]

The WHATWG Encoding Standard is the reference for the web platform. It calls UTF-8 "the most appropriate encoding for interchange of Unicode". It says that new protocols and formats must use UTF-8 only.[^10] It defines the decode hooks "UTF-8 decode", which removes a leading BOM, "UTF-8 decode without BOM", and "UTF-8 decode without BOM or fail", which stops at the first error.[^10]

### Variants that are not UTF-8

- **CESU-8.** It encodes each UTF-16 code unit of a surrogate pair as its own three-byte sequence. A code point above U+FFFF takes six bytes. RFC 3629 says that this form is not valid UTF-8.[^1]
- **Modified UTF-8.** Java uses it in `DataOutput.writeUTF`, in class files and in JNI. It writes U+0000 as the two bytes `C0 80`, so the encoded string has no zero byte. It uses only the forms of one, two and three bytes, and it writes a supplementary character as a surrogate pair.[^19]

## Pitfalls

- **Length in the wrong unit.** `len` in Go and Rust counts bytes. `length` in JavaScript, Java and C# counts UTF-16 code units. `len` in Python counts code points. None of them counts what a reader sees as characters. Use a grapheme segmenter to limit the length of a name or to move a cursor.[^12]
- **A cut in the middle of a character.** A byte index can fall inside a sequence. A truncation to a fixed number of bytes can leave half a character at the end. The example files for Go and Rust show this.
- **Silent replacement.** The default decoders of Python (with `errors="replace"`), Go, Java, C#, JavaScript and Rust (`from_utf8_lossy`) put U+FFFD in place of bad input and report nothing. Data is lost, and the count of U+FFFD differs between libraries. Use the strict mode at a trust boundary.
- **Overlong forms and surrogates.** A hand decoder that only follows the bit layout accepts `C0 80` as U+0000 and `ED A0 80` as a surrogate. Check the ranges of Table 3-7, or use the decoder of the standard library.[^2]
- **Lone surrogates from UTF-16 strings.** A string in JavaScript, Java or C# is a sequence of UTF-16 code units, so it can hold a lone surrogate.[^23][^34][^22] UTF-8 cannot encode it. The encoders of JavaScript and C# write U+FFFD in its place, so the text changes on the way out.
- **The BOM.** Some programs write `EF BB BF` at the start of a UTF-8 file. Windows PowerShell does so for each Unicode encoding other than UTF-7.[^31] A parser that does not expect it sees U+FEFF as part of the first field or key. The first column of a CSV file then has a different name. Read with `utf-8-sig` in Python, or remove U+FEFF at the start.[^13]
- **Normalization.** The composed and the decomposed forms of "é" are different strings. A lookup, a unique key or a password check can fail on equal text. RFC 3629 says this can affect security, for example when a program matches an identifier against an access control list.[^1] Normalize the text at the boundary.
- **The wrong decoder.** Text in UTF-8 that a program reads as Windows-1252 or ISO 8859-1 turns "é" into "Ã©" and the BOM into "ï»¿". The reverse error turns each such byte into U+FFFD. Name the encoding in each call that reads or writes text, and do not depend on the default of the platform.[^4][^20]
- **MySQL `utf8`.** In MySQL the character set `utf8mb3` has at most three bytes for each character, so it cannot store characters outside the BMP. The name `utf8` is a deprecated alias for `utf8mb3`. Use `utf8mb4`.[^27]

## Tools and libraries

| Language | Verdict | Encode and decode | Count |
|---|---|---|---|
| Python | stdlib | `str.encode("utf-8")`, `bytes.decode("utf-8", errors=...)`[^13] | `len` counts code points; `unicodedata.normalize`[^14] |
| JavaScript, TypeScript | native | `TextEncoder`, `TextDecoder` with `fatal` and `ignoreBOM`[^24] | `length` counts UTF-16 units; `Intl.Segmenter` for graphemes[^25]; `isWellFormed`, `normalize`[^23] |
| Go | stdlib | Go source is UTF-8, and `string` holds bytes[^16] | `utf8.RuneCountInString`, `utf8.ValidString`[^15] |
| Rust | stdlib | `str::from_utf8`, `String::from_utf8_lossy`[^17] | `len` counts bytes; `chars().count()`; `char` is a scalar value[^18] |
| Java | stdlib | `getBytes(StandardCharsets.UTF_8)`, `CharsetDecoder`[^34] | `length`, `codePointCount`, `BreakIterator`, `Normalizer` |
| C# | stdlib | `Encoding.UTF8`, `new UTF8Encoding(false, true)`[^21] | `Length`, `EnumerateRunes`, `StringInfo`[^22] |
| C, C++ | idiom | `u8` literals; the UTF-8 facets of `<codecvt>` are deprecated and removed from the working draft[^33] | libraries such as ICU and simdutf |
| SQL | native | SQLite stores text as UTF-8 by default[^26] | `length`, `octet_length`, `unicode`, `char`[^26] |
| Kotlin | stdlib | `decodeToString`, with `throwOnInvalidSequence = true` for a strict decode[^30] | as on the JVM |
| PowerShell | stdlib | `-Encoding utf8NoBOM` or `utf8BOM` on the file cmdlets[^31] | the .NET string methods |
| Bash | idiom | the shell passes the bytes through | `${#name}` counts characters in the locale[^32] |
| HTML, CSS | native | the document must be UTF-8[^28]; a style sheet falls back to UTF-8[^29] | n/a |

The HTML standard says that the encoding of a document "must be UTF-8", whether or not it has a declaration such as `<meta charset="utf-8">`.[^28] CSS reads a BOM first, then the HTTP header, then a `@charset "…";` at the very start of the file, then the encoding of the document that links the style sheet, and else uses UTF-8.[^29]

## References
