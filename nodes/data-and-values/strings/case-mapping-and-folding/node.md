---
tag: 57XKP7
kind: concept
slug: case-mapping-and-folding
name: Case mapping and case folding
aliases: [case mapping, case folding, case conversion, upper case, lower case, title case, casefold, caseless matching, Turkish i]
area: data-and-values/strings
level: core
summary: >-
  Case mapping turns text into upper, lower or title case, and case folding erases case for comparison. Both follow Unicode data, can change the length and can depend on the locale.
applies_to: core-languages
viz: optional
formal: none
see_also: [string-concept, grapheme-cluster, string-indexing, characters-and-code-points, value-equality-and-identity]
sources:
- url: https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-5/
  title: The Unicode Standard, Version 17.0, Chapter 5, Implementation Guidelines
  locator: 5.18 Case Mappings
- url: https://www.unicode.org/Public/17.0.0/ucd/SpecialCasing.txt
  title: SpecialCasing-17.0.0.txt, Unicode Character Database
  locator: Unconditional mappings; Conditional Mappings; Language-Sensitive Mappings
- url: https://www.unicode.org/Public/17.0.0/ucd/CaseFolding.txt
  title: CaseFolding-17.0.0.txt, Unicode Character Database
  locator: File header; entries 00DF, 0130, 03C2
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, Python documentation
  locator: str.casefold; str.upper; str.title
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Class String, Java SE 25 API
  locator: toLowerCase(); toUpperCase(Locale); equalsIgnoreCase
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/toLocaleUpperCase
  title: String.prototype.toLocaleUpperCase(), MDN Web Docs
  locator: Description
- url: https://doc.rust-lang.org/std/primitive.str.html
  title: Primitive Type str, The Rust Standard Library
  locator: to_uppercase; to_lowercase
- url: https://doc.rust-lang.org/std/primitive.char.html
  title: Primitive Type char, The Rust Standard Library
  locator: to_uppercase
- url: https://pkg.go.dev/strings
  title: Package strings, Go standard library
  locator: EqualFold; ToUpper; ToUpperSpecial; Title
- url: https://pkg.go.dev/unicode
  title: Package unicode, Go standard library
  locator: SimpleFold; SpecialCase; TurkishCase
- url: https://en.cppreference.com/w/c/string/byte/toupper
  title: toupper, cppreference.com (C)
  locator: Description; Parameters
- url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/best-practices-strings
  title: Best practices for comparing strings in .NET
  locator: Recommendations for string usage; String comparisons that use the current culture
- url: https://en.wikipedia.org/wiki/ASCII
  title: ASCII, Wikipedia
  locator: History
- url: https://en.wikipedia.org/wiki/%C3%9F
  title: ß, Wikipedia
  locator: Introduction
---

## Idea

A program changes the case of text to show it, such as a heading in capitals, and it ignores case to compare text, such as a user name or a file name. For ASCII, both jobs are simple: each of the 26 letters has one upper case and one lower case form. For the rest of Unicode, they are not. The Unicode Standard says that "Case mappings may produce strings of different lengths than the original"[^1], that some mappings depend on the letters around them, and that some depend on the language.

```python run id=idea
# Upper case can change the length: sharp s becomes "SS".
s = "straße"
print(s, s.upper(), len(s), len(s.upper()))

# Lower case depends on context: a final capital sigma becomes "ς".
w = "ΌΣΟΣ"
print(w, w.lower())

# Title case is not always upper case: the digraph "ǳ" has three forms.
dz = "ǳ"
print(dz, dz.upper(), dz.title())

# Lower case is not enough to compare. Case folding maps "ß" to "ss".
print(s.lower() == "STRASSE".lower(), s.casefold() == "STRASSE".casefold())

# The default mappings are not Turkish: "İ" lowercases to "i" and a combining dot.
dotted = "İ"
print(dotted.lower(), len(dotted.lower()), "i".upper())
```

```output for=idea
straße STRASSE 6 7
ΌΣΟΣ όσος
ǳ Ǳ ǲ
False True
i̇ 2 I
```

The German `ß` becomes the two letters `SS` in upper case, so the string grows from 6 to 7 characters. The Greek capital sigma becomes `ς` at the end of a word and `σ` in other places. The digraph `ǳ` has an upper case form `Ǳ` and a different title case form `ǲ`. To compare `straße` with `STRASSE`, lower case is not enough, but case folding works. And the dotted capital `İ` becomes two code points in lower case, because the default mapping is not the Turkish one.

## How it works

**Three mappings.** The Unicode Character Database gives each character an upper case, a lower case and a title case mapping. Title case is for the first letter of a word. For most characters it is the same as upper case, but "the two values are distinguished for these few digraph compatibility characters"[^1], such as `ǳ`. A *simple* mapping maps one code point to one code point. A *full* mapping can map one code point to several, and the extra mappings are in `SpecialCasing.txt`[^2]. "The maximum string expansion as a result of case mapping in the Unicode Standard is three"[^1].

**Complications.** The Unicode Standard lists the cases that break the ASCII picture[^1]:

- *Change in length.* `ß` "expands when uppercased to the sequence of two characters"[^1] `SS`, and the ligature `ﬁ` becomes `FI`[^2]. A mapping can also change the length in bytes but not in characters: in UTF-8, the Turkish lower case form `topkapı` takes eight bytes, and the upper case form `TOPKAPI` takes seven[^1].
- *Context.* The capital sigma `Σ` "lowercases to U+03C3"[^1] `σ` "if it is followed by another letter"[^1], and to the final sigma `ς` if it is not. `SpecialCasing.txt` marks this mapping with the condition `Final_Sigma`[^2].
- *Locale.* "The principal example of a case mapping that depends on the locale is Turkish"[^1]. In Turkish, `i` maps to `İ` and `ı` maps to `I`. `SpecialCasing.txt` lists these mappings for the languages `tr` and `az`[^2].
- *Caseless characters.* Most characters have no case, so "the process of uppercasing a string does not mean that it will no longer contain any lowercase letters"[^1].
- *No round trip.* "No casing operations are reversible"[^1]. After `McGowan` is upper cased, no case operation gets the original back.

**Case folding.** To compare text without case, a program does not compare upper case or lower case forms. It uses *case folding*, "the process of mapping characters of different case to a single form, so that case differences in strings are erased"[^1]. Folding "is more than just conversion to lowercase"[^1]: it maps `ß` to `ss` and both sigmas to `σ`. The data is in `CaseFolding.txt`, which "supports both implementations that require simple case foldings"[^3], where the length does not change, and implementations of full case folding, where the length can grow[^3]. The file says that the "full case foldings are superior"[^3], because with them `FUSS` and `Fuß` match. Folding is "context-insensitive and language-independent (except for the optional, alternate Turkic case foldings)"[^1], and the file marks the Turkic entries with the status `T`[^3]. A program usually keeps the original text and stores the folded form next to it, because "the original source string is not replaced by the folded string because that substitution may erase important information"[^1].

**Folding is not collation.** Case folding answers one question: are two strings the same if case is ignored? It does not sort, and it does not treat `é` and `e` as the same. For that, a program uses a collator with a strength or sensitivity setting, such as `Intl.Collator` or the Java `Collator`.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `upper`, `lower`, `title`, `capitalize`, `swapcase` and `casefold` on `str`; full mappings; no locale rules |
| JavaScript | native | `toUpperCase`, `toLowerCase`; `toLocaleUpperCase("tr")`; no fold, `Intl.Collator` for caseless compare |
| TypeScript | native | the JavaScript methods |
| Java | native | `toUpperCase(Locale)`, `toLowerCase(Locale)`; `equalsIgnoreCase`; `Character.toTitleCase` |
| Kotlin | native | `uppercase()`, `lowercase()` with an optional `Locale`; `equals(other, ignoreCase = true)` |
| C# | native | `ToUpper(CultureInfo)`, `ToUpperInvariant`; `StringComparison.OrdinalIgnoreCase`; `TextInfo.ToTitleCase` |
| Go | stdlib | `strings.ToUpper`, `ToLower`, `ToTitle`; `ToUpperSpecial(unicode.TurkishCase, s)`; `strings.EqualFold` |
| Rust | native | `str::to_uppercase`, `to_lowercase`; `char::to_uppercase` gives an iterator; `eq_ignore_ascii_case` |
| C | stdlib | `toupper` and `tolower` on one byte, by the C locale; `towupper` on one wide character |
| C++ | stdlib | `std::toupper` on one `char`; the `ctype` facet of a `std::locale` |
| Bash | native | `${s^^}` and `${s,,}` for upper and lower case, by the locale |
| PowerShell | native | .NET `ToUpper` and `ToLower`; `-eq` ignores case, and `-ceq` does not |
| SQL | native | `UPPER` and `LOWER`; case-insensitive collations for comparison |
| Regex | native | the flag `i`, or `(?i)`; Unicode case folding in some engines |
| HTML | n/a | no case operations; CSS styles the case of the text |
| CSS | native | `text-transform: uppercase`, `lowercase` and `capitalize`, with the `lang` of the element |

Python, JavaScript, Java and Rust apply the full mappings, so `ß` becomes `SS` and the length changes. Python `casefold` maps `ß` to `ss`[^4], and its documentation calls case folding "similar to lowercasing but more aggressive because it is intended to remove all case distinctions in a string"[^4]. In Java, "Since case mappings are not always 1:1 char mappings, the resulting String and this String may differ in length"[^5]. MDN says the same for JavaScript: "the length of the result string can differ from the input length"[^6]. Rust returns a new `String` "Since some characters can expand into multiple characters when changing the case"[^7], and `char::to_uppercase` "Returns an iterator that yields the uppercase mapping of this char as one or more chars"[^8].

Go and C# map one code point or one `char` at a time, so `ß` stays `ß` in upper case and the length does not change. Go `EqualFold` uses "simple Unicode case-folding"[^9], and `unicode.SimpleFold` walks the set of code points that fold together[^10]. C and C++ map one byte at a time, and in the default C locale `toupper` changes only the letters `a` to `z`[^11].

The locale is the second split. Java, JavaScript, C# and Go have Turkish rules: Java `toUpperCase(Locale)`, JavaScript `toLocaleUpperCase`, a .NET `CultureInfo`, and Go `unicode.TurkishCase`. Python and Rust do not. The Rust `to_uppercase` "does not handle language-specific casing, like Turkish and Azeri I/ı/İ/i"[^7]. The default methods of Java and .NET use the current locale, and the documentation warns about it. The Java documentation gives an example: `toLowerCase()` on `TITLE` returns `tıtle` in a Turkish locale, and it names "programming language identifiers, protocol keys, and HTML tags"[^5] as text that needs `toLowerCase(Locale.ROOT)`[^5]. The .NET guide recommends `StringComparison.Ordinal` or `StringComparison.OrdinalIgnoreCase` "as your safe default for culture-agnostic string matching"[^12].

The example files show each language on the same words. Python shows `capitalize` and `title`, the loss of `McGowan`, `casefold` on `ß`, `ﬁ` and Greek, and a Turkish workaround with `translate`. JavaScript shows the full mappings, the Turkish locale and a collator that ignores case but not accents. Java shows `Locale.ROOT` and the Turkish locale, the length rule of `equalsIgnoreCase`, title case of `ǳ`, and a `char` that cannot expand. C# shows that `ß` stays `ß`, the Turkish culture, `OrdinalIgnoreCase` and `ToTitleCase`. Go shows simple mappings, `TurkishCase`, `EqualFold` and the fold orbit of `K`. Rust shows the iterator of `char::to_uppercase`, the final sigma and the missing Turkish rules. C and C++ show byte-by-byte mappings and an ASCII-only compare.

## Variations and related ideas

- **ASCII-only case.** Protocol keywords, HTTP header names and many identifiers are ASCII, and their rules ignore only ASCII case. Rust `eq_ignore_ascii_case`, C `tolower` in the C locale and C# `OrdinalIgnoreCase` on ASCII text fit this job, and they do not depend on the locale.
- **Title case of words.** The Unicode title case mapping is for one letter. To title case a heading, a program must also find the words, and the rules depend on the language: "The choice of which words to titlecase in headings and titles is dependent on language and local conventions"[^1]. Python `title` uses "a simple language-independent definition of a word as groups of consecutive letters"[^4], so `they're` becomes `They'Re`. Go deprecated `strings.Title` because "The rule Title uses for word boundaries does not handle Unicode punctuation properly"[^9].
- **Case-insensitive identifiers.** Some languages, file systems and protocols ignore case in names. Each one must pick ASCII folding, simple folding or full folding, and a locale.
- **Normalization.** Case folding "does not preserve normalization formats"[^3], so a program that compares text normalizes it after it folds it, or it uses a canonical caseless match.

## Pitfalls

- **The Turkish i.** `toLowerCase()` with no locale in Java, or `ToLower()` in .NET, gives `tıtle` on a machine with a Turkish locale, and a keyword match fails. Pass `Locale.ROOT` or use the invariant culture for text that is not for people.
- **Lower case to compare.** `a.lower() == b.lower()` fails for `straße` and `STRASSE`. Use a fold, such as `casefold` in Python.
- **The length after a case change.** A buffer of the same size is too small for the upper case form of `ß` or `ﬁ`, and an index into the old string does not fit the new one.
- **One character at a time.** A loop that calls `toupper` on each byte, or `char::to_lowercase` on each `char`, misses the final sigma and the expansions. Call the function on the whole string.
- **Bytes over 0x7F in C.** `toupper` on a plain `char` that is negative is undefined behaviour: "If the value of ch is not representable as unsigned char and does not equal EOF, the behavior is undefined"[^11]. Cast to `unsigned char` first.

## Cost

A case mapping reads each code point once and looks it up in a table, so it is O(n). A full mapping can make the result up to three times as long in code points, so it allocates a new string. An ASCII mapping can work in place, because it does not change the length in bytes. A simple mapping keeps the number of code points, but in UTF-8 it can still change the number of bytes[^1]. To compare without case, a fold of both strings costs O(n) time and memory each time. A program that compares the same text often stores the folded form once. A compare function such as Go `EqualFold` takes the two strings and returns a bool, so the caller does not keep folded copies[^9].

## History

In ASCII, upper and lower case letters differ in one bit. In May 1963 a CCITT working party proposed to put the lower case letters in columns 6 and 7 of the code table, which "caused the characters to differ in bit pattern from the upper case by a single bit, which simplified case-insensitive character matching and the construction of keyboards and printers"[^13]. That one bit is the reason why C code can map case with `c ^ 0x20`, and why the ASCII rules stayed the default in many languages. Unicode later moved the rules into data files: the simple mappings in the character database, the special and locale mappings in `SpecialCasing.txt`, and the folds in `CaseFolding.txt`. The data still changes with the languages: German has the capital `ẞ`, and "In 2017, the Council for German Orthography officially adopted a capital form ⟨ẞ⟩ as an acceptable variant"[^14]. The default mapping still makes `ß` into `SS`, but `CaseFolding.txt` folds `ẞ` and `ß` to the same `ss`[^3].

## References
