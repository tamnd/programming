---
tag: PMSCAQ
kind: task
slug: reverse-a-string
name: Reverse a string
aliases: [reverse the characters of a string, reverse a string by code points, reverse a string by grapheme clusters]
area: data-and-values/strings
level: core
summary: Reverse the code points of a string without breaking UTF-8 or UTF-16, and reverse its grapheme clusters where the standard library can find them.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://www.unicode.org/reports/tr29/tr29-49.html
    title: Unicode Standard Annex 29, Unicode Text Segmentation, revision 49
    locator: Section 3, Grapheme Cluster Boundaries; Section 3.1.1, rules GB3, GB6, GB9, GB11, GB12 and GB13; Table 2, the Extend and Regional_Indicator rows
  - url: https://www.rfc-editor.org/rfc/rfc3629.html
    title: F. Yergeau, RFC 3629, UTF-8, a transformation format of ISO 10646 (2003)
    locator: Section 3, Section 4 and Section 10
  - url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/character-encoding-introduction
    title: Microsoft Learn, Introduction to character encoding in .NET
    locator: The string and char types; Surrogate pairs; The Rune type as a scalar value; Grapheme clusters
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.globalization.stringinfo?view=net-10.0
    title: .NET API reference, StringInfo
    locator: Remarks; the GetNextTextElementLength and GetTextElementEnumerator methods
  - url: https://docs.python.org/3/library/stdtypes.html#text-sequence-type-str
    title: The Python Standard Library, Built-in Types
    locator: Text Sequence Type, str; Common Sequence Operations, note 5
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/split
    title: MDN Web Docs, String.prototype.split()
    locator: Description, the warning about the empty separator
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/Symbol.iterator
    title: MDN Web Docs, String.prototype[Symbol.iterator]()
    locator: Description
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/Segmenter/Segmenter
    title: MDN Web Docs, Intl.Segmenter() constructor
    locator: Parameters, the granularity option
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
    title: Java SE 25 API, java.lang.String
    locator: the class description
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/StringBuilder.html#reverse()
    title: Java SE 25 API, java.lang.StringBuilder
    locator: reverse()
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/regex/Pattern.html#unicodesupport
    title: Java SE 25 API, java.util.regex.Pattern
    locator: Unicode support; the summary row for \X
  - url: https://go.dev/ref/spec#Conversions_to_and_from_a_string_type
    title: The Go Programming Language Specification
    locator: String types; Conversions to and from a string type, rules 2 and 4; For statements with range clause
  - url: https://pkg.go.dev/unicode/utf8#ValidString
    title: Go standard library, package unicode/utf8
    locator: ValidString
  - url: https://doc.rust-lang.org/std/primitive.str.html#method.chars
    title: The Rust Standard Library, primitive type str
    locator: chars; Invariant
  - url: https://doc.rust-lang.org/std/string/struct.String.html#method.from_utf8
    title: The Rust Standard Library, std::string::String
    locator: from_utf8, Errors
  - url: https://www.gnu.org/software/bash/manual/bash.html#Shell-Parameter-Expansion
    title: GNU Bash Reference Manual
    locator: Shell Parameter Expansion, ${#parameter} and ${parameter:offset:length}; Bash Variables, LC_ALL and LC_CTYPE
---

## Task

Reverse the order of the characters in a string. The word "character" can mean four different things, and each one gives a different result.

- A **byte** is one unit of UTF-8. A code point takes 1 to 4 bytes in UTF-8.
- A **UTF-16 code unit** is one unit of a Java, JavaScript or C# string. A code point above U+FFFF takes two units: a high surrogate followed by a low surrogate. This is a surrogate pair.
- A **code point** is one Unicode scalar value, such as U+0065 for `e` or U+1F642 for 🙂.
- A **grapheme cluster** is what a reader sees as one character. UAX #29 defines the extended grapheme cluster. One cluster can hold many code points: a letter and its combining marks, a flag made of two regional indicators, or emoji joined by U+200D ZERO WIDTH JOINER.

The main entry, `reverse_code_points(s)`, reverses the code points. Each code point keeps its bytes or its units in their order. So the result has the same length as `s` in bytes, in units and in code points.

The second entry, `reverse_graphemes(s)`, reverses the extended grapheme clusters. The JavaScript, TypeScript, Java and C# solutions have it, because their standard libraries find grapheme boundaries. The Rust standard library does not provide grapheme iteration. This page uses only the standard library, so the other solutions reverse code points only.

In C, C++, Go and Bash a string can hold any bytes. There the solution checks that the bytes are valid UTF-8. If they are not, it reports an error and returns no result.

## Solutions

- **Python.** `s[::-1]`. A `str` is a sequence of code points, so a slice with step -1 reverses the code points.
- **JavaScript and TypeScript.** `[...s].reverse().join("")`. The spread syntax uses the string iterator, which yields code points and keeps each surrogate pair together. For graphemes, `Intl.Segmenter` with `granularity: "grapheme"` splits the string at grapheme cluster boundaries, and the code reverses the segments. The TypeScript file adds types only.
- **Java.** `new StringBuilder(s).reverse()`. The method treats a surrogate pair as one character, so the pair keeps its order. For graphemes, the regular expression `\X` matches one extended grapheme cluster. The code collects the matches and reverses the list.
- **C#.** `s.EnumerateRunes()` yields one `Rune` for each Unicode scalar value. The code appends the runes in the opposite order. For graphemes, `StringInfo.GetTextElementEnumerator` yields text elements. The API reference calls a text element an extended grapheme cluster.
- **Go.** `utf8.ValidString` checks the bytes first. Then `[]rune(s)` gives the code points, `slices.Reverse` reverses them, and `string(runes)` encodes them again. The check is necessary, because the conversion to `[]rune` does not report bad bytes.
- **Rust.** `s.chars().rev().collect()`. A `char` is a Unicode scalar value. A `&str` is always valid UTF-8, so the function has no error case.
- **C.** The function reads the length of each UTF-8 sequence from its first byte and checks the sequence against the syntax of RFC 3629. Then it copies the bytes to the mirror position in the output buffer. It makes one pass and allocates nothing.
- **C++.** The same loop over a `std::string_view`. The function returns `std::expected<std::string, ReverseError>`.
- **Bash.** In a UTF-8 locale, `${#s}` and `${s:i:1}` count characters, so a loop from the end reverses the code points. The function sets `LC_ALL=C.UTF-8` in a local variable, because `LC_CTYPE` controls how Bash reads characters. First it sends the string through `iconv -f UTF-8 -t UTF-8`, which fails on bad bytes.

## Edge cases

The cases of `reverse_code_points` are in `cases/basic.jsonl` and `cases/edge.jsonl`. The cases of `reverse_graphemes` are in `cases/graphemes.jsonl`. The byte strings that must give an error are in `cases/errors.jsonl`.

- `empty` and `single`: the result is the input.
- `two-byte` and `three-byte`: `ï` is 2 bytes and `日` is 3 bytes in UTF-8. A byte reversal puts the continuation bytes first, and the result is not valid UTF-8.
- `astral` and `two-astral`: 🙂 is 4 bytes in UTF-8 and a surrogate pair in UTF-16. A reversal by UTF-16 unit puts the low surrogate before the high surrogate.
- `combining-mark`: `e`, U+0301 COMBINING ACUTE ACCENT, `x`. The code point reversal gives `x`, U+0301, `e`, so the accent moves to the `x`. The case records this result on purpose. `reverse_graphemes` gives `x`, `e`, U+0301, because rule GB9 does not break before an extending character.
- `flag`: the flag of France is the regional indicators F and R. The code point reversal gives R and F, so the flag is lost.
- Grapheme cases: `flags` keeps each pair of regional indicators (GB12 and GB13). `zwj-family` keeps an emoji ZWJ sequence (GB11). `skin-tone` keeps U+1F3FD with its emoji, because an emoji modifier is in the class Extend (GB9). `crlf` keeps CR LF together (GB3). `hangul-jamo` keeps a leading jamo with its vowel jamo (GB6).
- Errors: the bytes `FF FE`, a sequence cut by the end of the string, an encoded surrogate (`ED A0 80`) and an overlong form (`C0 AF`). RFC 3629 does not allow these bytes.

## Pitfalls

- **Byte reversal.** Reverse the bytes of UTF-8 text and each multibyte sequence comes out backwards. Rust `String::from_utf8` checks the bytes and returns an error. C code that reverses a `char` array gives the bad bytes to the caller with no error.

```rust wrong id=rust-reverse-bytes
fn main() {
    let s = "naïve";
    let bytes: Vec<u8> = s.bytes().rev().collect();
    match String::from_utf8(bytes) {
        Ok(t) => println!("bytes: {t}"),
        Err(e) => println!("bytes: error: {e}"),
    }
    let t: String = s.chars().rev().collect();
    println!("chars: {t}");
}
```

```output
bytes: error: invalid utf-8 sequence of 1 bytes from index 2
chars: evïan
```

- **Reversal by UTF-16 unit.** JavaScript `s.split("")` splits the string into UTF-16 code units, so it breaks each surrogate pair. After the reversal, the low surrogate comes first, and the two units are no longer a pair. C# `Array.Reverse` on `s.ToCharArray()` has the same bug, because a `char` is one UTF-16 code unit. The spread syntax splits by code point:

```javascript wrong id=js-split-units
const units = (t) => Array.from({ length: t.length }, (_, i) => t.charCodeAt(i).toString(16).padStart(4, "0")).join(" ");
const s = "a🙂b";
console.log("input:   ", units(s));
console.log("split:   ", units(s.split("").reverse().join("")));
console.log("spread:  ", units([...s].reverse().join("")));
```

```output
input:    0061 d83d de42 0062
split:    0062 de42 d83d 0061
spread:   0062 d83d de42 0061
```

- **Code point reversal and grapheme clusters.** Python `s[::-1]`, Rust `chars().rev()`, Go `[]rune`, Java `StringBuilder.reverse` and the JavaScript spread syntax all reverse code points. A combining mark moves to the letter that was after it. A flag turns into a different pair of regional indicators. A ZWJ sequence comes out in the opposite order:

```python run id=python-code-points
def codes(s: str) -> str:
    return " ".join(f"U+{ord(c):04X}" for c in s)


for s in ["e\u0301x", "\U0001F1EB\U0001F1F7", "\U0001F468\u200d\U0001F466"]:
    print(codes(s), "->", codes(s[::-1]))
```

```output
U+0065 U+0301 U+0078 -> U+0078 U+0301 U+0065
U+1F1EB U+1F1F7 -> U+1F1F7 U+1F1EB
U+1F468 U+200D U+1F466 -> U+1F466 U+200D U+1F468
```

Reverse the grapheme clusters to keep each one whole:

```javascript run id=js-graphemes
const segmenter = new Intl.Segmenter(undefined, { granularity: "grapheme" });
const reverse = (s) => Array.from(segmenter.segment(s), (part) => part.segment).reverse().join("");
const codes = (s) => [...s].map((c) => "U+" + c.codePointAt(0).toString(16).toUpperCase().padStart(4, "0")).join(" ");

for (const s of ["e\u0301x", "\u{1F1EB}\u{1F1F7}", "\u{1F468}\u200d\u{1F466}"]) {
  console.log(codes(s), "->", codes(reverse(s)));
}
```

```output
U+0065 U+0301 U+0078 -> U+0078 U+0065 U+0301
U+1F1EB U+1F1F7 -> U+1F1EB U+1F1F7
U+1F468 U+200D U+1F466 -> U+1F468 U+200D U+1F466
```

- **Silent replacement in Go.** A Go string holds any bytes. The conversion to `[]rune` puts U+FFFD in place of a bad byte and gives no error. Call `utf8.ValidString` first:

```go run id=go-runes-replace
package main

import (
	"fmt"
	"slices"
	"unicode/utf8"
)

func main() {
	s := "caf\xc3"
	runes := []rune(s)
	slices.Reverse(runes)
	for _, r := range runes {
		fmt.Printf("U+%04X ", r)
	}
	fmt.Println()
	fmt.Println("valid:", utf8.ValidString(s))
}
```

```output
U+FFFD U+0066 U+0061 U+0063
valid: false
```

## Related tasks

- [[split-a-string]]: another job on a string, where the separator search works on bytes or units.
- [[sort-a-list-of-records]]: a string key can compare by code unit or by code point.

## References
