---
tag: V5YGS3
kind: concept
slug: characters-and-code-points
name: Characters, code points and graphemes
aliases: [character, code point, code unit, grapheme cluster, Unicode, UTF-16, surrogate pair, rune, Unicode normalization, NFC, text encoding]
area: foundations/characters-and-text
level: core
summary: >-
  Text has four layers. Readers see grapheme clusters, Unicode numbers code points, an encoding stores them as code units, and the units become bytes. Each language counts one layer.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1991
  people: [Joe Becker, Lee Collins, Mark Davis, Ken Thompson, Rob Pike]
  note: ASCII came in 1963, Unicode 1.0 in 1991, UTF-8 in 1992, and surrogate pairs in Unicode 2.0 in 1996.
see_also: [utf-8, reverse-a-string, split-a-string, base64-encode-and-decode, value-equality-and-identity, immutability, code-injection]
same_as:
  wikidata: Q32483
  wikipedia: Character_(computing)
sources:
- url: https://www.unicode.org/reports/tr29/
  title: 'Unicode Standard Annex #29, Unicode Text Segmentation'
  locator: 3 Grapheme Cluster Boundaries
- url: https://docs.python.org/3/howto/unicode.html
  title: Unicode HOWTO, Python documentation
  locator: Definitions
- url: https://doc.rust-lang.org/std/primitive.char.html
  title: Primitive Type char, The Rust Standard Library
  locator: Validity and Layout
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Class String, Java SE 25 API documentation
  locator: Class description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String
  title: String, MDN Web Docs
  locator: UTF-16 characters, Unicode code points, and grapheme clusters
- url: https://go.dev/blog/strings
  title: Strings, bytes, runes and characters in Go, The Go Blog
  locator: What is a string? and Code points, characters, and runes
- url: https://www.unicode.org/reports/tr15/
  title: 'Unicode Standard Annex #15, Unicode Normalization Forms'
  locator: 1.1 Canonical and Compatibility Equivalence
---

## Idea

Text in a program has four layers. A reader sees characters, which Unicode calls grapheme clusters. Unicode gives each abstract character a number, its code point, and one visible character can take several code points, such as a letter and a combining accent, or an emoji and a skin tone modifier. An encoding form stores each code point as one or more code units of 8, 16 or 32 bits, and a byte order stores the code units as bytes. "The length of a string" can mean any of these four counts, and each language picks one. Most bugs with text come from code that uses one layer where it means another.

```python run id=idea
import unicodedata

SKIN = range(0x1F3FB, 0x1F400)  # skin tone modifiers
FLAG = range(0x1F1E6, 0x1F200)  # regional indicator letters


def graphemes(s):  # a simplified version of the rules of UAX #29
    out = []
    for ch in s:
        cp, prev = ord(ch), out[-1] if out else ""
        joins = (
            unicodedata.combining(ch)  # a combining accent
            or ch in "\u200d\ufe0f"  # zero width joiner, emoji style
            or prev.endswith("\u200d")  # the character after a joiner
            or cp in SKIN
            or (cp in FLAG and len(prev) == 1 and ord(prev) in FLAG)
        )
        if out and joins:
            out[-1] += ch
        else:
            out.append(ch)
    return out


samples = ["A", "\u00e9", "e\u0301", "\u20ac", "\U0001f600",
           "\U0001f44d\U0001f3fd", "\U0001f1fb\U0001f1f3",
           "\U0001f469\u200d\U0001f4bb"]
print("graphemes code-points utf16-units utf8-bytes  first code point")
for s in samples:
    units = len(s.encode("utf-16-le")) // 2
    first = f"U+{ord(s[0]):04X} {unicodedata.name(s[0])}"
    print(f"{len(graphemes(s)):9} {len(s):11} {units:11} {len(s.encode()):10}  {first}")
```

```output for=idea
graphemes code-points utf16-units utf8-bytes  first code point
        1           1           1          1  U+0041 LATIN CAPITAL LETTER A
        1           1           1          2  U+00E9 LATIN SMALL LETTER E WITH ACUTE
        1           2           2          3  U+0065 LATIN SMALL LETTER E
        1           1           1          3  U+20AC EURO SIGN
        1           1           2          4  U+1F600 GRINNING FACE
        1           2           4          8  U+1F44D THUMBS UP SIGN
        1           2           4          8  U+1F1FB REGIONAL INDICATOR SYMBOL LETTER V
        1           3           5         11  U+1F469 WOMAN
```

The simplified `graphemes` function follows a few of the rules of Unicode text segmentation: a combining mark, a skin tone modifier, a zero width joiner and the character after it, and the second letter of a flag all join the character before them. Each sample is one character on the screen. The decomposed `e` with an acute accent is 2 code points, the grinning face is 1 code point but 2 UTF-16 units, and the woman at a laptop is 3 code points joined into one picture, 5 UTF-16 units and 11 bytes of UTF-8. No count other than the first matches what a reader calls the length.

## How it works

**Characters and grapheme clusters.** The unit that a reader sees, selects with the cursor and deletes with one key press is not a fixed number of anything in memory. Unicode Standard Annex #29 says: "A single Unicode code point is often, but not always the same as a basic unit of a writing system for a language, or what a typical user might think of as a “character”."[^1] It defines grapheme clusters as "user-perceived characters" and gives rules to find their boundaries in a string of code points.[^1] The rules cover combining marks, Hangul syllables, emoji sequences with joiners and modifiers, and pairs of regional indicators that make a flag. The rules change as Unicode adds characters, so code that splits, truncates or reverses text for display uses a library that tracks the current version of the rules.

**Code points.** Unicode assigns a number to each abstract character, from 0 to 0x10FFFF, and writes it as U+ followed by at least four hexadecimal digits: U+0041 is `A` and U+1F44D is the thumbs up sign. The Python documentation describes the model: "a Unicode string is a sequence of code points, which are numbers from 0 through 0x10FFFF (1,114,111 decimal)."[^2] The range from U+D800 to U+DFFF is reserved for the surrogates of UTF-16 and holds no characters. A code point outside that range is a Unicode scalar value, and Rust's `char` type holds exactly these: "A char is a ‘Unicode scalar value’, which is any ‘Unicode code point’ other than a surrogate code point."[^3]

**Code units and encoding forms.** An encoding form maps each code point to a sequence of code units. The Python documentation continues: "This sequence of code points needs to be represented in memory as a set of code units, and code units are then mapped to 8-bit bytes."[^2] UTF-8 uses 1 to 4 bytes for each code point, keeps ASCII as it is, and is the usual form for files, network protocols and the web; see [UTF-8](/utf-8/). UTF-16 uses one 16-bit unit for code points up to U+FFFF and a surrogate pair of two units for the rest. UTF-32 uses one 32-bit unit for each code point, which makes indexing by code point simple and costs 4 bytes for each character. When UTF-16 or UTF-32 goes into bytes, the order of the bytes in each unit matters, and a byte order mark or a label such as UTF-16LE states it.

**Strings in languages.** The string type of a language fixes which layer its indices and lengths count. Java, JavaScript and C# chose 16-bit units. The Java documentation says: "A String represents a string in the UTF-16 format in which supplementary characters are represented by surrogate pairs" and "Index values refer to char code units, so a supplementary character uses two positions in a String."[^4] MDN says the same of JavaScript: "Strings are represented fundamentally as sequences of UTF-16 code units."[^5] Go and Rust store UTF-8 bytes. The Go blog says: "In Go, a string is in effect a read-only slice of bytes."[^6] It adds that a string literal holds valid UTF-8, and that "Those sequences represent Unicode code points, called runes."[^6] Python hides the encoding and indexes by code point. No mainstream string type indexes by grapheme cluster, because finding one needs a scan with the segmentation rules.

**Normalization.** Some characters have more than one sequence of code points. `é` is U+00E9, or `e` followed by U+0301 COMBINING ACUTE ACCENT. Unicode Standard Annex #15 calls such sequences canonically equivalent: "Canonical equivalence is a fundamental equivalency between characters or sequences of characters which represent the same abstract character, and which when correctly displayed should always have the same visual appearance and behavior."[^7] A program that compares code points sees two different strings. Normalization converts text to one form: NFC composes characters where it can, and NFD decomposes them. NFKC and NFKD also fold compatibility characters, such as the ligature `ﬁ` to `fi`, and are for search and identifiers, not for storage of the text that a user wrote.

**Case and comparison.** Case mapping is not one character to one character. The German `ß` has the upper case `SS`, so `upper` can make a string longer, and the Turkish dotless `ı` makes the result depend on the language of the text. Comparing text without case uses case folding, not lower case. Sorting text for people uses collation rules for a language, not the order of code points, and is a job for a library such as ICU.

**Invalid input.** Bytes from outside a program may not be valid in the encoding that the program expects. A decoder can reject the input, or replace each bad sequence with U+FFFD REPLACEMENT CHARACTER, as `from_utf8_lossy` in Rust and `errors="replace"` in Python do. Strings of 16-bit units have their own invalid case. MDN defines a lone surrogate as a 16-bit code unit in the range of high surrogates that has no low surrogate after it, or the reverse, and such a string cannot convert to UTF-8 without loss.[^5] Code that cuts a string at a fixed index of units or bytes can make invalid text from valid text, as the wrong blocks of the JavaScript and Go files show.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `str` of code points; `encode`, `unicodedata` |
| JavaScript | native | UTF-16 units; code point iteration; `Intl.Segmenter` |
| TypeScript | native | as JavaScript |
| Java | native | UTF-16 `String`; code point methods; `\X` in regex |
| Kotlin | native | UTF-16 `String` on the JVM |
| C# | native | UTF-16 `string`; `Rune`; `StringInfo` |
| Go | native | UTF-8 bytes; `rune`; `unicode/utf8` |
| Rust | native | UTF-8 `str`; `char` is a scalar value |
| C | idiom | `char` is a byte; UTF-8 by hand or a library |
| C++ | native | `char8_t`, `char16_t`, `char32_t` strings |
| Bash | idiom | lengths in the current locale; `iconv` |
| PowerShell | native | .NET strings of UTF-16 units |
| SQL | native | character sets, collations, `CHAR_LENGTH` |
| Regex | native | `\X` and `\p{...}` in many engines |
| HTML | native | the charset declaration; character references |
| CSS | native | `@charset`; escapes such as `\1F44D` |

Every example file uses the same string, `café` with a decomposed accent, a space and a thumbs up with a skin tone. A reader sees 6 characters. The string has 8 code points, 10 UTF-16 units and 15 bytes of UTF-8, and each language reports the count of its own layer: Python 8, JavaScript, Java and C# 10, Go and Rust 15. Java with `\X`, C# with `StringInfo` and JavaScript with `Intl.Segmenter` also count the 6 grapheme clusters. Python, Go and Rust need a package outside the standard library for that count.

Languages differ in what a string can hold. A Rust `str` is always valid UTF-8, and the standard library checks this when it makes one from bytes. A Go string can hold any bytes, and code checks them with `utf8.ValidString` when it matters. Strings in Java, JavaScript and C# can hold lone surrogates. A Python `str` can hold any code point, including surrogates, which some decoders use to carry undecodable bytes through a program.

## Variations and related ideas

- **Legacy encodings.** ASCII, the ISO 8859 family, Windows code pages, Shift JIS and GB 18030 encode a smaller set of characters, or the same set in a different way. Programs that read old files must know which one they use, because the bytes do not say.
- **Fixed-width strings.** UTF-32 strings, `std::u32string` and Python's internal representation give constant-time indexing by code point, but code points are still not characters.
- **Ropes and indexed text.** Text editors store long text in trees of chunks and keep counts of bytes, code points, UTF-16 units and lines for each chunk, so that they can convert positions between the layers quickly.
- **Byte strings.** Python `bytes`, Rust `Vec<u8>` and `OsString`, and Go `[]byte` hold data that is not text, or text in an unknown encoding, such as file names on Unix.
- **Binary-to-text encodings.** Base64 and percent encoding put bytes into ASCII text. See [Base64](/base64-encode-and-decode/).
- **String operations.** Reversing and splitting a string must respect grapheme clusters and code points. See [reverse a string](/reverse-a-string/) and [split a string](/split-a-string/).

## Pitfalls

- **Length that is not the length.** `len`, `length` and `Length` count code points, UTF-16 units or bytes. A limit of 10 characters in a form can reject 6 characters that a user typed, or a database column of 10 bytes can reject 4 letters. State which unit each limit uses.
- **Cutting inside a character.** Truncating by bytes or units can split a code point, and truncating by code points can split a grapheme cluster, which drops an accent or breaks an emoji. The wrong blocks in the JavaScript and Go files and the fails block in the Rust file show this. Cut at boundaries from a segmenter.
- **Comparing text that is not normalized.** Two strings that look the same can differ in their code points, so equality, hashing and lookups fail. The wrong block in the Python file shows this. Normalize text to NFC when it enters the program.
- **Guessing the encoding.** Reading a file with the default encoding of the machine gives different results on different machines. State the encoding at every boundary, and use UTF-8 where the choice is free.
- **Case mapping by hand.** Adding 32 to a letter, or upper-casing one character at a time, breaks for every script but basic Latin. Use the case functions of the language with a fixed locale, and casefold to compare.
- **Lookalike characters.** The Latin `a` and the Cyrillic `а` look the same and are different code points. Attackers use this in domain names and user names. Check identifiers for mixed scripts, and see [code injection](/code-injection/).

## Cost

Length in code units or bytes is O(1) in most languages, because the string stores it. Counting code points in UTF-8 or UTF-16 takes O(n) time, and counting grapheme clusters takes O(n) time with a table lookup for each code point. Indexing by code point is O(1) in UTF-32 and in Python, which picks 1, 2 or 4 bytes for each code point from the largest one in the string, and O(n) in UTF-8 and UTF-16. Converting between encodings takes O(n) time and a new buffer. UTF-8 takes 1 byte for each ASCII character and up to 3 bytes for other characters of the Basic Multilingual Plane, so it is smaller than UTF-16 for most markup and source code, and larger for text in Chinese or Japanese. Normalization takes O(n) time, and most implementations first check quickly whether the text is already normal, which it usually is.

## History

ASCII, published in 1963 by the American Standards Association, gave 128 codes for English letters, digits and controls in 7 bits. ISO 8859 and many national and vendor code pages in the 1980s used the eighth bit for other European letters, and Asian standards used several bytes for each character. Joe Becker of Xerox, with Lee Collins and Mark Davis of Apple, proposed a single 16-bit character set in 1988, and the Unicode Consortium published Unicode 1.0 in 1991. Ken Thompson and Rob Pike designed UTF-8 in 1992 for the Plan 9 operating system.

Sixteen bits proved too few. Unicode 2.0 added surrogate pairs in 1996, which made UTF-16 a variable-width encoding, after Windows NT, Java and JavaScript had already chosen 16-bit characters. This is why `length` counts code units in those languages, and Java added methods for code points in 2004. Python 3.0 made `str` a Unicode string in 2008, and Python 3.3 made every build index it by code point in 2012. Go, from 2009, and Rust, from 2015, made UTF-8 the string representation. Emoji came into Unicode in 2010, and their sequences of modifiers and joiners made grapheme clusters a daily problem for programs that had treated a code point as a character.

## References
