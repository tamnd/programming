---
tag: Q7FXFV
kind: concept
slug: grapheme-cluster
name: Grapheme cluster
aliases: [grapheme, extended grapheme cluster, user-perceived character, text element, Character (Swift), UAX 29]
area: data-and-values/strings
level: core
summary: >-
  A grapheme cluster is what a user sees as one character, such as é, a flag or a family emoji. It can be many code points, and UAX 29 gives the rules that find its boundaries.
applies_to: core-languages
viz: optional
formal: none
history:
  people: [Mark Davis]
  note: UAX 29, Unicode Text Segmentation, defined legacy and extended grapheme clusters by Unicode 5.1.0 in 2008, and its revision for Unicode 9.0 added the rules for emoji sequences.
  year: 2008
see_also: [string-concept, string-indexing, string-memory-layout, characters-and-code-points, reverse-a-string]
sources:
- url: https://www.unicode.org/reports/tr29/tr29-47.html
  title: Unicode Standard Annex #29, Unicode Text Segmentation, revision 47 (Unicode 17.0.0)
  locator: 3 Grapheme Cluster Boundaries; Table 1a; Table 2; 3.1.1 Grapheme Cluster Boundary Rules; 6.4 Random Access
- url: https://github.com/swiftlang/swift-book/blob/main/TSPL.docc/LanguageGuide/StringsAndCharacters.md
  title: The Swift Programming Language, Strings and Characters
  locator: Extended Grapheme Clusters
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/Segmenter
  title: Intl.Segmenter, MDN Web Docs
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/text/BreakIterator.html
  title: Class BreakIterator, Java SE 25 API
  locator: Class description; Implementation Requirements
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/regex/Pattern.html
  title: Class Pattern, Java SE 25 API
  locator: Unicode Extended Grapheme matcher; Boundary matchers
- url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/character-encoding-introduction
  title: Character encoding in .NET
  locator: Grapheme clusters
- url: https://docs.python.org/3/library/unicodedata.html
  title: unicodedata, Unicode Database, Python 3.15 documentation
  locator: iter_graphemes; grapheme_cluster_break
- url: https://github.com/mrabarnett/mrab-regex
  title: mrab-regex, the regex module for Python, README
  locator: Matching a single grapheme \X
- url: https://doc.rust-lang.org/book/ch08-02-strings.html
  title: The Rust Programming Language, 8.2 Storing UTF-8 Encoded Text with Strings
  locator: Bytes and Scalar Values and Grapheme Clusters; Methods for Iterating Over Strings
- url: https://docs.rs/unicode-segmentation/latest/unicode_segmentation/
  title: unicode-segmentation crate documentation
  locator: Crate description
- url: https://pkg.go.dev/github.com/rivo/uniseg
  title: uniseg, Go package documentation
  locator: Grapheme Clusters
- url: https://unicode-org.github.io/icu/userguide/boundaryanalysis/
  title: Boundary Analysis, ICU User Guide
  locator: Character Boundary
- url: https://www.unicode.org/reports/tr29/tr29-13.html
  title: Unicode Standard Annex #29, Unicode Text Segmentation, revision 13 (Unicode 5.1.0)
  locator: 3 Grapheme Cluster Boundaries
- url: https://www.unicode.org/reports/tr29/tr29-29.html
  title: Unicode Standard Annex #29, Unicode Text Segmentation, revision 29 (Unicode 9.0.0)
  locator: Modifications, Revision 29
---

## Idea

A user does not see code points. A user sees letters, accented letters, flags and emoji, and each one looks like one character. The Unicode Standard calls such a unit a *user-perceived character*. In code, the unit is a *grapheme cluster*: a sequence of one or more code points that forms one character on the screen. UAX 29 says that "A single Unicode code point is often, but not always the same as a basic unit of a writing system for a language"[^1]. The text below has 8 grapheme clusters but 13 code points.

```python run id=idea
import unicodedata

ZWJ = "\u200d"

def is_extend(c):
    # Extend: nonspacing and enclosing marks, and the emoji skin tone modifiers.
    return unicodedata.category(c) in ("Mn", "Me") or 0x1F3FB <= ord(c) <= 0x1F3FF

def is_ri(c):
    return 0x1F1E6 <= ord(c) <= 0x1F1FF

def graphemes(s):
    # A small subset of the UAX 29 rules, enough for this text:
    # no break before Extend or ZWJ (GB9), no break after ZWJ (a short form of GB11),
    # and regional indicators join in pairs (GB12, GB13).
    out = []
    for c in s:
        prev = out[-1][-1] if out else None
        if prev is not None and (is_extend(c) or c == ZWJ or prev == ZWJ):
            out[-1] += c
        elif prev is not None and is_ri(c) and is_ri(prev) and len(out[-1]) == 1:
            out[-1] += c
        else:
            out.append(c)
    return out

t = "cafe\u0301 \U0001F1EB\U0001F1F7 \U0001F469\U0001F3FD\u200d\U0001F692"
print(t)
print("UTF-8 bytes:", len(t.encode("utf-8")))
print("UTF-16 units:", len(t.encode("utf-16-le")) // 2)
print("code points:", len(t))
g = graphemes(t)
print("graphemes:", len(g))
for x in g:
    print(f"[{x}]", " ".join(f"U+{ord(c):04X}" for c in x))
```

```output for=idea
café 🇫🇷 👩🏽‍🚒
UTF-8 bytes: 31
UTF-16 units: 18
code points: 13
graphemes: 8
[c] U+0063
[a] U+0061
[f] U+0066
[é] U+0065 U+0301
[ ] U+0020
[🇫🇷] U+1F1EB U+1F1F7
[ ] U+0020
[👩🏽‍🚒] U+1F469 U+1F3FD U+200D U+1F692
```

The letter `é` here is `e` and the combining accent U+0301. The flag is two *regional indicator* symbols, F and R. The firefighter emoji is four code points: a woman, a skin tone modifier, a zero width joiner (ZWJ) and a fire engine. So the same text has four sizes: 31 bytes in UTF-8, 18 code units in UTF-16, 13 code points and 8 grapheme clusters. The function `graphemes` uses only three of the rules, and it is a sketch. A real program uses the full rules from a library or the standard library.

## How it works

**Rules, not a table.** No list of all grapheme clusters exists, because a cluster can be any base character with any number of marks after it. UAX 29 gives rules that decide, between each two code points, whether a boundary is there. Each code point has a `Grapheme_Cluster_Break` property, such as `Extend`, `ZWJ`, `Regional_Indicator` or `Control`. The class `Extend` is the code points with "Grapheme_Extend = Yes, or Emoji_Modifier=Yes"[^1], which includes the nonspacing marks and the skin tone modifiers. The rules then look at the classes on each side:

- GB9: "Do not break before extending characters or ZWJ."[^1] So `e` and U+0301 stay together.
- GB11: "Do not break within emoji modifier sequences or emoji zwj sequences."[^1] So a ZWJ joins two pictographs into one emoji.
- GB12 and GB13: "do not break between regional indicator (RI) symbols if there is an odd number of RI characters before the break point"[^1]. So the indicators pair up into flags, two at a time.
- GB999: otherwise, break everywhere.

Other rules keep CR LF together, keep the jamo of a Hangul syllable together, and keep some Indic conjuncts together.

**Legacy and extended clusters.** UAX 29 defines two variants. A *legacy* grapheme cluster is a base and the marks after it. An *extended* grapheme cluster also includes "all spacing combining marks, such as the spacing (but dependent) vowel signs in Indic scripts"[^1]. In the Tamil syllable `நி`, the vowel sign is a spacing mark, so the syllable is one extended cluster but two legacy clusters. UAX 29 says that "The extended rules are recommended, except where the legacy variant is required for a specific environment"[^1]. Most libraries use extended clusters.

**Where the boundaries matter.** "Grapheme clusters can be treated as units, by default, for processes such as the formatting of drop caps, as well as the implementation of text selection, arrow key movement, forward deletion, and so forth"[^1]. A text field that moves the cursor by code points puts it between `e` and its accent. A limit on the number of characters must count something, and a count of code points or code units does not match what the user sees. A program that cuts a string to fit a field must cut at a cluster boundary, or it can cut a flag into two letters.

**Tailoring.** The default rules are not right for every language. "The notion of user-perceived character is not always an unambiguous concept for a given writing system"[^1], and UAX 29 allows "tailored grapheme clusters for specific locales"[^1]. Some libraries, such as ICU, accept a locale for this reason.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `unicodedata.iter_graphemes` in Python 3.15; before that, `\X` in the third-party `regex` module |
| JavaScript | stdlib | `Intl.Segmenter` with `granularity: "grapheme"` |
| TypeScript | stdlib | the JavaScript `Intl.Segmenter` and its types |
| Java | stdlib | `BreakIterator.getCharacterInstance()`; `\X` and `\b{g}` in `java.util.regex` |
| Kotlin | stdlib | the Java `BreakIterator` on the JVM |
| C# | stdlib | `StringInfo` and `TextElementEnumerator`; text elements are grapheme clusters |
| Go | library | no standard package; `github.com/rivo/uniseg` |
| Rust | library | no standard API; the `unicode-segmentation` crate, `graphemes(true)` |
| C | library | no standard API; ICU `ubrk_open` with `UBRK_CHARACTER` |
| C++ | library | no standard API; ICU `BreakIterator::createCharacterInstance` |
| Bash | absent | `${#s}` counts characters of the locale, which are code points in UTF-8 |
| PowerShell | stdlib | the .NET `System.Globalization.StringInfo` |
| SQL | absent | no standard function; string lengths count characters of the character set |
| Regex | native | `\X` in Java and the Python `regex` module; not in JavaScript `RegExp` |
| HTML | n/a | the browser segments the text of a document when it draws it |
| CSS | n/a | the browser segments text for line breaks and selection |

Swift is the language that makes grapheme clusters the default. "Every instance of Swift's `Character` type represents a single *extended grapheme cluster*"[^2], and "An extended grapheme cluster is a sequence of one or more Unicode scalars that (when combined) produce a single human-readable character"[^2]. So `count` on a Swift string counts clusters, and the book warns that it "must iterate over the Unicode scalars in the entire string"[^2].

In JavaScript, "The Intl.Segmenter object enables locale-sensitive text segmentation, enabling you to get meaningful items (graphemes, words or sentences) from a string"[^3]. In Java, the default character `BreakIterator` "conforms to the Unicode Consortium's Extended Grapheme Cluster breaks"[^4], and in a regex, `\X` matches "Any Unicode extended grapheme cluster"[^5]. In .NET, a cluster has another name: "The equivalent term in .NET is text element"[^6]. Python 3.15 added `unicodedata.iter_graphemes`, which "uses extended grapheme cluster rules defined by Unicode Standard Annex #29"[^7]. For earlier versions, the `regex` module supports `\X`, and "It conforms to the Unicode specification"[^8]. The Rust standard library leaves the work to a crate, because "Getting grapheme clusters from strings, as with the Devanagari script, is complex, so this functionality is not provided by the standard library"[^9]. The crate `unicode-segmentation` gives "Iterators which split strings on Grapheme Cluster, Word, or Sentence boundaries, according to the Unicode Standard Annex #29 rules"[^10]. In Go, the package `uniseg` "provides tools to iterate over these grapheme clusters"[^11]. In C and C++, the ICU character iterator "locates the boundaries according to the rules defined in" UAX 29[^12].

The example files show each mechanism on the same text. Python shows the code points of one emoji, NFC that joins `e` and its accent but cannot join a flag, a cut that loses the accent, and the Tamil spacing mark. JavaScript counts 8 segments with `Intl.Segmenter`, finds the segment that holds an index, and cuts by segments. Java uses `BreakIterator` and `\X`. C# uses `StringInfo`, `ParseCombiningCharacters` and the text element enumerator. Go, Rust, C and C++ have no standard API, so their examples use the same small rule subset as the Idea block, and their comments name the library that a real program uses. Go cuts at a byte limit and backs up to a cluster boundary, and Rust shows that a reverse by code points moves an accent onto the wrong letter.

## Variations and related ideas

- **Normalization.** NFC can join `e` and U+0301 into the one code point `é`, so a count of code points after NFC is closer to the count of clusters. But most clusters have no single code point, such as flags, emoji sequences and many Indic syllables. Normalization does not replace segmentation.
- **Words and sentences.** UAX 29 also defines word boundaries and sentence boundaries, and the same APIs give them: `Intl.Segmenter` with `"word"`, the word `BreakIterator` in Java and ICU, and `unicode_words` in Rust. Line breaks are in UAX 14.
- **Width on the screen.** A cluster is one unit, but it can take one or two columns in a terminal. The column width is a separate property.
- **String types built on clusters.** Swift `String` and `Character`, and the .NET `StringInfo`, give clusters as the unit. Most other languages keep code units or code points as the unit and add clusters as an API. See [string indexing](/string-indexing/).

## Pitfalls

- **A count of code points is not a count of characters.** `len("👩🏽‍🚒")` is 4 in Python, `"👩🏽‍🚒".length` is 7 in JavaScript, and the user sees one emoji.
- **A cut by code points or bytes can split a cluster.** `"café"[:4]` can lose the accent when `é` is two code points, and a byte limit can cut an emoji in half. Cut at a cluster boundary.
- **A reverse by code points breaks clusters.** The accent moves onto the next letter, and a flag `FR` becomes `RF`. See [reverse a string](/reverse-a-string/).
- **Old rules in old libraries.** The rules and the property data change with new versions of Unicode, as the revision for emoji sequences shows. The .NET documentation says that "prior to .NET 5, the StringInfo class had a bug causing some grapheme clusters to be handled incorrectly"[^6]. Use a current library, and do not store cluster counts that must stay stable across versions.
- **A hand-written segmenter.** A sketch like the one in the Idea block is wrong for Hangul, for Indic scripts and for many emoji. Use the full rules from a library.

## Cost

Segmentation reads the text from the start, so a count of clusters or a search for the i-th cluster is O(n). Each step looks up a property of each code point and applies a small state machine. To find a boundary at a random point is harder: UAX 29 says that "Random access introduces a further complication"[^1], because the rules can need context from before that point. An iterator that moves forward avoids this cost. For storage and for indexes, programs keep byte or code unit offsets, and they find cluster boundaries only when a user sees or edits the text.

## History

Unicode Standard Annex #29 holds the rules. Its version for Unicode 5.1.0, dated 2008, already gave "two variants: legacy grapheme clusters and extended grapheme clusters"[^13], and it said that "The extended grapheme cluster boundaries are recommended for general processing"[^13]. Emoji sequences came later. The revision for Unicode 9.0 "Added the new property values and rules for not breaking emoji sequences"[^14]. Swift made the extended grapheme cluster its `Character` type, and later APIs such as `Intl.Segmenter` and Python `unicodedata.iter_graphemes` brought the rules into the standard libraries of other languages.

## References
