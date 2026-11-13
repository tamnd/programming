---
tag: KPSK22
kind: task
slug: normalize-unicode-text
name: Normalize Unicode text
aliases: [Unicode normalization, NFC, NFD, NFKC, NFKD, canonical equivalence, precomposed character, decomposed character]
area: data-and-values/strings
level: core
summary: Put text in one normalization form, NFC or NFD, so that the 2 ways to write a letter with an accent give the same code points.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://www.unicode.org/reports/tr15/
  title: Unicode Standard Annex #15, Unicode Normalization Forms
  locator: 1.1 Canonical and Compatibility Equivalence; 1.2 Normalization Forms; 1.4 Concatenation of Normalized Strings
- url: https://docs.python.org/3/library/unicodedata.html#unicodedata.normalize
  title: The Python Standard Library, unicodedata
  locator: normalize
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/normalize
  title: MDN Web Docs, String.prototype.normalize()
  locator: Parameters
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/text/Normalizer.html
  title: Java SE 25 API, Class Normalizer
  locator: Class description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.normalize?view=net-10.0
  title: .NET API reference, String.Normalize
  locator: Normalize()
- url: https://pkg.go.dev/golang.org/x/text/unicode/norm
  title: Go module golang.org/x/text, package norm
  locator: Overview
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Conditional-Constructs
  title: Bash Reference Manual
  locator: 3.2.5.2 Conditional Constructs
---

## Task

Put a string in a normalization form. The letter U+00E9 has 2 forms: the one code point U+00E9, and `e` with U+0301 COMBINING ACUTE ACCENT. They are canonical equivalents, which "represent the same abstract character, and which when correctly displayed should always have the same visual appearance and behavior"[^1]. But a compare of the code points finds 2 different strings.

The function `normalize(s, form)` returns `s` in one of 2 forms:

| Form | Rule | `caf` U+00E9 | `cafe` U+0301 |
|---|---|---|---|
| NFD | "Canonical Decomposition"[^1] | `cafe` U+0301 | `cafe` U+0301 |
| NFC | "Canonical Decomposition, followed by Canonical Composition"[^1] | `caf` U+00E9 | `caf` U+00E9 |

After the same form, the 2 strings are equal. NFKD and NFKC also use compatibility equivalence, which is "a weaker type of equivalence"[^1]. Those forms can change the text, so this task does not use them.

The contract has a small set of code points: U+0000 to U+00FF, and the marks U+0300, U+0301, U+0302 and U+0308 after one of the vowels `a`, `e`, `i`, `o`, `u`, `A`, `E`, `I`, `O` and `U`. For this set, the full rule is a table of 40 pairs: each vowel with each mark, and its composite from U+00C0 to U+00FC. A full normalizer also puts the marks in the canonical order, and it has many more pairs[^1].

## Solutions

| Language | Normalize |
|---|---|
| Python | `unicodedata.normalize(form, s)` |
| JavaScript, TypeScript | `s.normalize(form)` |
| Java | `Normalizer.normalize(s, Normalizer.Form.valueOf(form))` |
| C# | `s.Normalize(NormalizationForm.FormC)` or `FormD` |
| Go | the table; the module `golang.org/x/text/unicode/norm` has the full rule |
| Rust, C, C++, Bash | the table |

- **Python.** For `unicodedata.normalize`, "Valid values for form are 'NFC', 'NFKC', 'NFD', and 'NFKD'"[^2].
- **JavaScript.** For the argument of `normalize`, "If omitted or undefined, "NFC" is used"[^3].
- **Java.** `Normalizer` "transforms Unicode text into an equivalent composed or decomposed form, allowing for easier sorting and searching of text"[^4].
- **C#.** `Normalize()` with no argument gives a string "whose binary representation is in Unicode normalization form C"[^5].
- **Go.** "Package norm contains types and functions for normalizing Unicode strings"[^6]. It is in the module `golang.org/x/text`, so the impl has the table and needs no module.

The impls with the table read the code points. For NFD, each composite becomes its vowel and its mark. For NFC, a mark after a vowel replaces the vowel with the composite.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `nfc-composes` and `nfd-decomposes`: `e` U+0301 becomes U+00E9 in NFC, and U+00E9 becomes `e` U+0301 in NFD.
- `already-nfc` and `already-nfd`: a string that is in the form does not change.
- `mixed-nfc` and `mixed-nfd`: one string has both forms of `caf` U+00E9. The result has one form only.
- `upper-grave`, `diaeresis`, `circumflex` and `umlauts`: the other marks, and the upper case vowels.
- `spacing-accent` and `no-break-space`: U+00B4 and U+00A0 have compatibility decompositions only, so NFC and NFD do not change them.
- `sharp-s`: U+00DF has no decomposition.
- `empty`: the result is empty.

## Pitfalls

- **Python == compares code points.** The 2 forms of `caf` U+00E9 are equal only after the same form:

```python run id=python-compare-forms
import unicodedata

a = "caf\u00e9"
b = "cafe\u0301"
print(a == b, unicodedata.normalize("NFC", a) == unicodedata.normalize("NFC", b))
```

```output for=python-compare-forms
False True
```

- **JavaScript: 2 normalized strings make a string that is not normalized.** "none of the Normalization Forms are closed under string concatenation"[^1]. Normalize the result of a concatenation again:

```javascript run id=javascript-concatenation
const x = "e";
const y = "\u0301";
console.log(x.normalize() + y.normalize() === (x + y).normalize());
```

```output for=javascript-concatenation
false
```

- **Java NFKC changes the text.** NFC keeps the ligature U+FB01 as 1 character, and NFKC makes it the 2 letters `fi`. Compatibility equivalents "may have distinct visual appearances or behaviors"[^1], so do not use NFKC on text that a person reads:

```java run id=java-nfkc-ligature
import java.text.Normalizer;

public class Main {
  public static void main(String[] args) {
    String ligature = "\ufb01";
    String nfc = Normalizer.normalize(ligature, Normalizer.Form.NFC);
    String nfkc = Normalizer.normalize(ligature, Normalizer.Form.NFKC);
    System.out.println(nfc.length() + " " + nfkc);
  }
}
```

```output for=java-nfkc-ligature
1 fi
```

- **C# Normalize() with no argument gives NFC.** `IsNormalized()` also checks NFC. A string in NFD is not normalized for it:

```csharp run id=csharp-default-form-c
using System;

string s = "e\u0301";
Console.WriteLine($"{s.IsNormalized()} {s.Normalize().Length}");
```

```output for=csharp-default-form-c
False 1
```

- **Go: a search finds a part of a letter.** In NFD, `cafe` U+0301 contains `cafe`, but the reader sees U+00E9 at the end, not `e`. In NFC, the search finds nothing:

```go run id=go-search-part-of-letter
package main

import (
	"fmt"
	"strings"
)

func main() {
	decomposed := "cafe\u0301"
	composed := "caf\u00e9"
	fmt.Println(strings.Contains(decomposed, "cafe"), strings.Contains(composed, "cafe"))
}
```

```output for=go-search-part-of-letter
true false
```

- **Rust: the 2 forms have different lengths.** In NFC the word is 5 bytes, and in NFD it is 6 bytes. A limit on the length gives a different result for the same text:

```rust run id=rust-form-lengths
fn main() {
    let composed = "caf\u{e9}";
    let decomposed = "cafe\u{301}";
    println!("{} {} {}", composed.len(), decomposed.len(), composed == decomposed);
}
```

```output for=rust-form-lengths
5 6 false
```

- **C: the 2 forms sort at different places.** `strcmp` compares bytes. `e` U+0301 comes before `f`, and U+00E9 comes after it:

```c run id=c-sort-order
#include <stdio.h>
#include <string.h>

int main(void) {
    int decomposed = strcmp("e\xcc\x81", "f") < 0;
    int composed = strcmp("\xc3\xa9", "f") < 0;
    printf("%d %d\n", decomposed, composed);
    return 0;
}
```

```output for=c-sort-order
1 0
```

- **C++: a set holds the same word 2 times.** Normalize each key before it goes into a `std::set` or a `std::map`:

```cpp run id=cpp-set-two-forms
#include <iostream>
#include <set>
#include <string>

int main() {
    std::set<std::string> words = {"caf\xc3\xa9", "cafe\xcc\x81"};
    std::cout << words.size() << "\n";
}
```

```output for=cpp-set-two-forms
2
```

- **Bash: quote the right side of ==.** With `==`, "the string to the right of the operator is considered a pattern"[^7]. If you quote a part of the pattern, "the quoted portion is matched literally"[^7]. With the quotes, the 2 forms are different:

```bash run id=bash-compare-forms
a=$'caf\xc3\xa9'
b=$'cafe\xcc\x81'
if [[ $a == "$b" ]]; then echo same; else echo different; fi
```

```output for=bash-compare-forms
different
```

## Related tasks

- [[compare-strings]]: normalize the 2 strings before the compare.
- [[count-the-graphemes-of-a-string]]: both forms of U+00E9 are 1 grapheme.
- [[convert-a-string-to-bytes]]: the 2 forms give different bytes.
- [[change-the-case-of-a-string]]: normalize the text before a case change.

## References
