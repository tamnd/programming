---
tag: G23VJ9
kind: task
slug: count-the-graphemes-of-a-string
name: Count the graphemes of a string
aliases: [grapheme count, count user-perceived characters, extended grapheme cluster, count emoji, text elements, Intl.Segmenter, BreakIterator]
area: data-and-values/strings
level: core
summary: Count the extended grapheme clusters of a string, so that a letter with its combining marks, a flag and an emoji sequence each count as one.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://www.unicode.org/reports/tr29/
  title: Unicode Standard Annex #29, Unicode Text Segmentation
  locator: 3.1.1 Grapheme Cluster Boundary Rules
- url: https://www.unicode.org/Public/UCD/latest/ucd/auxiliary/GraphemeBreakProperty.txt
  title: Unicode Character Database, GraphemeBreakProperty.txt
  locator: Unicode 18.0.0
- url: https://www.unicode.org/Public/UCD/latest/ucd/emoji/emoji-data.txt
  title: Unicode Character Database, emoji-data.txt
  locator: Extended_Pictographic
- url: https://docs.python.org/3/library/unicodedata.html#unicodedata.iter_graphemes
  title: The Python Standard Library, unicodedata
  locator: iter_graphemes
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/text/BreakIterator.html
  title: Java SE 25 API, Class BreakIterator
  locator: Implementation Requirements
- url: https://learn.microsoft.com/en-us/dotnet/api/system.globalization.stringinfo?view=net-10.0
  title: .NET API reference, StringInfo class
  locator: Remarks
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/Segmenter
  title: MDN Web Docs, Intl.Segmenter
  locator: Description
- url: https://pkg.go.dev/unicode/utf8#RuneCount
  title: Go standard library, package unicode/utf8
  locator: RuneCount
- url: https://doc.rust-lang.org/std/primitive.str.html#method.chars
  title: Rust standard library, primitive type str
  locator: chars
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Shell-Parameter-Expansion
  title: Bash Reference Manual
  locator: 3.5.3 Shell Parameter Expansion; 5.2 Bash Variables
---

## Task

Count the graphemes of a string. A grapheme is an extended grapheme cluster of UAX #29, which is the unit for "user-perceived characters"[^1]. The function `count_graphemes(s)` returns that number. For example, `count_graphemes("e\u0301")` is 1, because U+0301 is a combining mark, and the flag U+1F1EB U+1F1F7 is 1. A count of code points gives 2 for each of them.

The rules give a break between each two code points, except in these places:

| Rule | No break | Example |
|---|---|---|
| GB3 | between CR and LF | `\r\n` is 1 |
| GB9 | before Extend or ZWJ | `e` U+0301 is 1 |
| GB11 | between ZWJ and an emoji, after an emoji | U+1F469 U+200D U+1F4BB is 1 |
| GB12, GB13 | inside a pair of regional indicators | U+1F1EB U+1F1F7 is 1 |

GB4 and GB5 come before GB9: "Otherwise, break before and after controls"[^1]. The Grapheme_Cluster_Break value of each code point is in the Unicode data file[^2], and Extended_Pictographic is in the emoji data file[^3].

The contract has a small set of code points: U+0000 to U+00FF, the combining marks U+0300 to U+036F, ZWJ, the variation selectors, the regional indicators, the skin tone modifiers and some emoji. The impls in Python, Go, Rust, C, C++ and Bash have a table for this set only. A full table has many more ranges, and also the rules for Hangul, Prepend, SpacingMark and Indic conjuncts[^1].

## Solutions

| Language | Count |
|---|---|
| Python, with `iter_graphemes` | `sum(1 for _ in unicodedata.iter_graphemes(s))` |
| JavaScript, TypeScript | `[...new Intl.Segmenter(undefined, { granularity: "grapheme" }).segment(s)].length` |
| Java | `BreakIterator.getCharacterInstance()` and a loop on `next()` |
| C# | `new StringInfo(s).LengthInTextElements` |
| Go, Rust, C, C++, Bash | the table and the rules of the contract |

- **Python.** `iter_graphemes` "uses extended grapheme cluster rules defined by Unicode Standard Annex #29"[^4]. Older versions of the module do not have the function, so the impl has its own table.
- **Java.** "The default implementation of the character boundary analysis conforms to the Unicode Consortium's Extended Grapheme Cluster breaks"[^5].
- **C#.** ".NET defines a text element as a unit of text that is displayed as a single character, that is, a grapheme"[^6].

The impls with a table keep 3 values while they read the code points: the number of regional indicators that end at the previous code point, whether the previous code points are an emoji and Extend marks, and whether the previous code point is a ZWJ after that emoji.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: 0 graphemes.
- `precomposed` and `combining`: U+00E9, and `e` U+0301, are each 1 grapheme. `two-marks` adds U+0302, and the count stays 1.
- `leading-mark`: U+0301 at the start has no base. It is a grapheme alone, so U+0301 `a` is 2.
- `crlf` and `lfcr`: CR LF is 1, and LF CR is 2.
- `cr-mark` and `soft-hyphen`: a control does not take a combining mark, so CR U+0301 is 2. U+00AD is a control in the data file[^2], so `a` U+00AD `b` is 3.
- `flag`, `three-indicators` and `two-flags`: "do not break between regional indicator (RI) symbols if there is an odd number of RI characters before the break point"[^1]. So 3 indicators are 2 graphemes, and 4 are 2.
- `zwj-sequence` and `zwj-after-letter`: a ZWJ joins 2 emoji. After the letter `a`, the ZWJ joins the `a`, and the emoji is a new grapheme.
- `skin-tone` and `variation-selector`: U+1F3FD and U+FE0F are Extend, so each pair is 1.

## Pitfalls

- **Python len counts code points.** Each of these strings is 1 grapheme:

```python run id=python-len-code-points
s = "e\u0301"
flag = "\U0001F1EB\U0001F1F7"
print(len(s), len(flag))
```

```output for=python-len-code-points
2 2
```

- **JavaScript length counts UTF-16 code units.** A spread counts code points. Only the segmenter gives 1 for a thumb with a skin tone. `Intl.Segmenter` gives "graphemes, words or sentences"[^7]:

```javascript run id=javascript-segmenter
const s = "\u{1F44D}\u{1F3FD}";
const segmenter = new Intl.Segmenter(undefined, { granularity: "grapheme" });
console.log(s.length, [...s].length, [...segmenter.segment(s)].length);
```

```output for=javascript-segmenter
4 2 1
```

- **Java codePointCount is not a grapheme count.** A flag is 4 UTF-16 code units and 2 code points:

```java run id=java-flag-break-iterator
import java.text.BreakIterator;

public class Main {
  public static void main(String[] args) {
    String flag = "\ud83c\uddeb\ud83c\uddf7";
    BreakIterator it = BreakIterator.getCharacterInstance();
    it.setText(flag);
    int n = 0;
    it.first();
    while (it.next() != BreakIterator.DONE) {
      n++;
    }
    System.out.println(flag.length() + " " + flag.codePointCount(0, flag.length()) + " " + n);
  }
}
```

```output for=java-flag-break-iterator
4 2 1
```

- **C# Length counts UTF-16 code units.** The ZWJ sequence is 5 units and 1 text element:

```csharp run id=csharp-stringinfo
using System;
using System.Globalization;

string s = "\U0001F469\u200d\U0001F4BB";
Console.WriteLine($"{s.Length} {new StringInfo(s).LengthInTextElements}");
```

```output for=csharp-stringinfo
5 1
```

- **Go: a cut by runes drops the mark.** "RuneCount returns the number of runes in p"[^8], so `e` U+0301 is 2. The first rune alone is `e`, without its accent:

```go run id=go-rune-cut
package main

import (
	"fmt"
	"unicode/utf8"
)

func main() {
	s := "e\u0301"
	r := []rune(s)
	fmt.Println(utf8.RuneCountInString(s), string(r[:1]) == "e")
}
```

```output for=go-rune-cut
2 true
```

- **Rust has no grapheme iterator in std.** "Iteration over grapheme clusters may be what you actually want. This functionality is not provided by Rust's standard library, check crates.io instead"[^9]. `chars().take(1)` drops the accent too:

```rust run id=rust-chars-not-graphemes
fn main() {
    let s = "e\u{301}";
    let first: String = s.chars().take(1).collect();
    println!("{} {}", s.chars().count(), first == "e");
}
```

```output for=rust-chars-not-graphemes
2 true
```

- **C: the same text can have 2 lengths.** U+00E9 is 2 bytes, and `e` U+0301 is 3 bytes. Both are 1 grapheme. To compare them, normalize the text first:

```c run id=c-two-forms
#include <stdio.h>
#include <string.h>

int main(void) {
    const char *composed = "\xc3\xa9";
    const char *decomposed = "e\xcc\x81";
    printf("%zu %zu\n", strlen(composed), strlen(decomposed));
    return 0;
}
```

```output for=c-two-forms
2 3
```

- **C++: a cut at a byte count breaks a grapheme.** The ZWJ sequence is 11 bytes. A cut at 4 bytes keeps only the first emoji:

```cpp run id=cpp-byte-cut
#include <iostream>
#include <string>

int main() {
    std::string s = "\xf0\x9f\x91\xa9\xe2\x80\x8d\xf0\x9f\x92\xbb";
    std::cout << s.size() << " " << s.substr(0, 4).size() << "\n";
}
```

```output for=cpp-byte-cut
11 4
```

- **Bash: the locale decides what a character is.** `${#parameter}` "Substitutes the length in characters of the value of parameter"[^10], and LC_CTYPE "determines the interpretation of characters"[^10]. In the C locale, `e` U+0301 gives 3:

```bash run id=bash-length-locale
s=$'e\xcc\x81'
LC_ALL=C
echo "${#s}"
```

```output for=bash-length-locale
3
```

## Related tasks

- [[find-the-length-of-a-string]]: the length in bytes, in UTF-16 code units and in code points.
- [[iterate-over-the-characters-of-a-string]]: the code points of a string in order.
- [[reverse-a-string]]: a reverse by code points moves a combining mark to the wrong letter.
- [[encode-and-decode-utf-8]]: the bytes of each code point.
- [[normalize-unicode-text]]: one form for the 2 ways to write U+00E9.

## References
