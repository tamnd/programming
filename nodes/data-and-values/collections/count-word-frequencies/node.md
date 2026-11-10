---
tag: NEW-count-word-frequencies
kind: task
slug: count-word-frequencies
name: Count word frequencies
aliases: [word count, word frequency, most common words, top n words, count occurrences in a map]
area: data-and-values/collections
level: core
summary: Split a text into words, change each word to lower case, count the words in a map, and return the n most frequent words, with ties in code point order.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/collections.html#collections.Counter
    title: The Python Standard Library, collections, Counter objects
    locator: Counter; most_common
  - url: https://docs.python.org/3/library/stdtypes.html#str.casefold
    title: The Python Standard Library, Built-in Types
    locator: str.casefold; str.lower
  - url: https://docs.python.org/3/howto/sorting.html
    title: The Python HOWTOs, Sorting Techniques
    locator: Key Functions; Sort Stability and Complex Sorts
  - url: https://docs.python.org/3/reference/expressions.html#value-comparisons
    title: The Python Language Reference, Expressions
    locator: 6.10.1 Value comparisons (strings, sequences)
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Regular_expressions/Character_class_escape
    title: MDN Web Docs, Character class escape
    locator: Description (\w)
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/localeCompare
    title: MDN Web Docs, String.prototype.localeCompare
    locator: Description
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/Segmenter/segment
    title: MDN Web Docs, Intl.Segmenter.prototype.segment
    locator: Return value (isWordLike)
  - url: https://tc39.es/ecma262/multipage/abstract-operations.html#sec-islessthan
    title: Ecma International, ECMA-262, ECMAScript Language Specification
    locator: 7.2.12 IsLessThan (strings compare by code unit)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html#toLowerCase()
    title: Java SE 25 API, java.lang.String
    locator: toLowerCase(), toLowerCase(Locale), compareTo(String)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/regex/Pattern.html
    title: Java SE 25 API, java.util.regex.Pattern
    locator: Predefined character classes (\w); UNICODE_CHARACTER_CLASS; Characters (\x{h...h})
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Map.html#merge(K,V,java.util.function.BiFunction)
    title: Java SE 25 API, java.util.Map
    locator: merge
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.thenby?view=net-10.0
    title: .NET API reference, Enumerable.ThenBy
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/best-practices-strings
    title: .NET documentation, Best practices for comparing strings in .NET
    locator: String comparisons that use the current culture
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.string.tolowerinvariant?view=net-10.0
    title: .NET API reference, String.ToLowerInvariant
    locator: Remarks
  - url: https://go.dev/ref/spec#For_range
    title: The Go Programming Language Specification
    locator: For statements with range clause (iteration order over maps)
  - url: https://pkg.go.dev/strings#FieldsFunc
    title: Go standard library, package strings
    locator: FieldsFunc, ToLower, Compare
  - url: https://pkg.go.dev/slices#SortFunc
    title: Go standard library, package slices
    locator: SortFunc
  - url: https://doc.rust-lang.org/std/collections/struct.HashMap.html#method.entry
    title: The Rust Standard Library, std::collections::HashMap
    locator: entry; iter (arbitrary order)
  - url: https://en.cppreference.com/w/cpp/container/unordered_map
    title: cppreference.com, std::unordered_map
    locator: Description; operator[]
  - url: https://doc.rust-lang.org/std/primitive.slice.html#method.sort_unstable_by
    title: The Rust Standard Library, primitive type slice
    locator: sort_unstable_by
  - url: https://en.cppreference.com/w/c/string/byte/tolower
    title: cppreference.com, tolower
    locator: Parameters; Return value
  - url: https://en.cppreference.com/w/cpp/string/char_traits/cmp
    title: cppreference.com, std::char_traits::eq, std::char_traits::lt
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/qsort.html
    title: The Open Group, POSIX.1-2024, qsort
    locator: Description
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/uniq.html
    title: The Open Group, POSIX.1-2024, uniq
    locator: Description; Options (-c)
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/tr.html
    title: The Open Group, POSIX.1-2024, tr
    locator: Options (-c); Operands (ranges, octal escapes)
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/sort.html
    title: The Open Group, POSIX.1-2024, sort
    locator: Description; Options (-k, -n, -r); Environment Variables (LC_COLLATE)
  - url: https://www.unicode.org/reports/tr29/
    title: Unicode Standard Annex #29, Unicode Text Segmentation
    locator: 4 Word Boundaries
---

## Task

Count how many times each word occurs in a text. Return the `n` most frequent words with their counts.

A word is a run of characters that are ASCII letters, ASCII digits or not ASCII. Every other ASCII character ends a word. So a space, a newline, a full stop, an apostrophe, a hyphen and an underscore are all separators. Change each word to lower case before you count it, so `Stop` and `stop` are the same word.

Sort the words by count from high to low. If two words have the same count, sort them in code point order. Then return the first `n` pairs. If there are fewer than `n` words, return all of them. A text with no words gives an empty list.

The text contains no character at or above U+D800, so the byte order of UTF-8, the code unit order of UTF-16 and the code point order are the same. The only upper case letters are `A` to `Z`. These limits keep the result the same in every language.

## Solutions

Each solution has three steps. It finds the words, it counts them in a map, and it sorts the map entries with two keys. The second key is the word. It makes the order fixed, because the order of a hash map is not fixed.

- **Python.** `re.findall` with `[A-Za-z0-9\u0080-\U0010FFFF]+` finds the words, and `collections.Counter` counts them. `sorted(counts.items(), key=lambda item: (-item[1], item[0]))[:n]` gives the result. The key `-item[1]` puts high counts first. Strings compare by code point.
- **JavaScript and TypeScript.** The regular expression `/[A-Za-z0-9\u{80}-\u{10FFFF}]+/gu` finds the words, and a `Map` counts them. The sort compares the counts with `y - x`. If they are equal, `||` goes on to `<` and `>` on the words. These operators compare UTF-16 code units, and they do not use the locale.
- **Java.** `Pattern` finds the words. `Map.merge(word, 1L, Long::sum)` counts them. The sort uses `Comparator.comparingLong(WordCount::count).reversed().thenComparing(WordCount::word)`. The code calls `toLowerCase(Locale.ROOT)`, not `toLowerCase()`.
- **C#.** A `[GeneratedRegex]` finds the words, and a `Dictionary` counts them. The query is `OrderByDescending(e => e.Value).ThenBy(e => e.Key, StringComparer.Ordinal).Take(n)`.
- **Go.** `strings.FieldsFunc` splits the text at each run of separators. A `map[string]int` counts the words. `slices.SortFunc` sorts with `cmp.Or(cmp.Compare(b.Count, a.Count), strings.Compare(a.Word, b.Word))`. Go compares strings byte by byte.
- **Rust.** `text.split(is_separator)` gives the words and some empty strings, which the code skips. `*counts.entry(word.to_lowercase()).or_insert(0) += 1` counts them. `sort_unstable_by` is safe here, because no two entries have the same word, so no two entries are equal.
- **C++.** A loop finds the words, and `++counts[word]` on a `std::unordered_map` counts them. `operator[]` inserts a zero count for a new word. `std::ranges::sort` sorts the entries. `std::string` compares each `char` as `unsigned char`, so UTF-8 words sort by code point.
- **C.** The C library has no map. The code copies the text, puts a null character in place of each separator, and changes `A` to `Z` to lower case. It sorts the words with `qsort` and `strcmp`, so equal words are next to each other, and counts each run. A second `qsort` orders the counts. `strcmp` compares bytes as `unsigned char`.
- **Bash.** A pipeline does all the work. `tr -c 'A-Za-z0-9\200-\377' '\n'` puts each word on its own line. `tr 'A-Z' 'a-z'` changes the case. `sort | uniq -c` counts the lines, and `sort -k1,1nr -k2,2` orders them. `head -n` keeps the first `n`. Each command runs with `LC_ALL=C`, so it works on bytes.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty` and `no-words`: an empty text and a text of only separators give `[]`.
- `case`: `Word`, `word`, `WORD` and `wOrD` are one word with the count 4.
- `ties`: `pear`, `fig` and `apple` each occur twice. The result is `apple`, `fig`, `pear`, not the order of the input.
- `fold-before-sort`: `Zebra apple` gives `apple` before `zebra`. If you sort before you change the case, `Zebra` comes first.
- `n-zero` and `n-above-count`: `n = 0` gives `[]`. An `n` above the number of words gives all of them.
- `non-ascii`: `café`, `naïve` and `日本語` are words. A rule that keeps only ASCII letters splits them.
- `code-point-order`: `f é e` gives `e`, `f`, `é`. The code point of `é` (U+00E9) is above the code point of `f`.
- `apostrophe`: `don't` is the two words `don` and `t`. This is the rule of the task. Natural language needs a different rule (see the pitfalls).
- `digits`: `66` and `66a` are words, and they are different words.
- `separators`: a tab, a carriage return, an underscore and a hyphen each end a word.

## Pitfalls

- **The tie order of `most_common`.** `Counter.most_common` puts elements with equal counts in the order it first saw them. So the result depends on the input order:

```python wrong id=py-most-common-ties
from collections import Counter

counts = Counter("pear fig apple fig pear apple".split())
print(counts.most_common(2))
```

```output
[('pear', 2), ('fig', 2)]
```

```python run id=py-sorted-ties
from collections import Counter

counts = Counter("pear fig apple fig pear apple".split())
print(sorted(counts.items(), key=lambda item: (-item[1], item[0]))[:2])
```

```output
[('apple', 2), ('fig', 2)]
```

- **The order of a hash map.** The Go specification does not specify the iteration order over a map, and the order can change from one loop to the next. Rust `HashMap` and C++ `std::unordered_map` also have no fixed order. A result that comes from the order of a map can change on each run. This program loops over the same map 100 times and reports if it saw more than one order:

```go run id=go-map-order
package main

import "fmt"

func main() {
	counts := map[string]int{"apple": 2, "fig": 2, "pear": 2, "plum": 2, "kiwi": 2, "lime": 2}
	orders := map[string]bool{}
	for range 100 {
		order := ""
		for word := range counts {
			order += word + " "
		}
		orders[order] = true
	}
	fmt.Println("more than one order:", len(orders) > 1)
}
```

```output
more than one order: true
```

- **`\w` is ASCII.** In JavaScript, `\w` is `[A-Za-z0-9_]`, also with the `u` flag. In Java, `\w` is `[a-zA-Z_0-9]` unless you set `UNICODE_CHARACTER_CLASS`. So a split on `\W+` breaks `café` into `caf`. `\w` also counts `_` as part of a word:

```javascript wrong id=js-ascii-word
console.log("café naïve under_score".match(/\w+/gu));
```

```output
[ 'caf', 'na', 've', 'under_score' ]
```

- **A split on one space.** `split(" ")` gives an empty string for two spaces in a row, and it keeps the punctuation in the words. So `stop.` and `stop` are different words:

```javascript wrong id=js-split-space
console.log("Stop.  Stop, stop!".toLowerCase().split(" "));
```

```output
[ 'stop.', '', 'stop,', 'stop!' ]
```

- **Lower case that depends on the locale.** In Java, `toLowerCase()` uses the default locale. In a Turkish locale, `I` becomes `ı` (U+0131), so `TITLE` does not become `title`. Use `toLowerCase(Locale.ROOT)`. In C#, `ToLowerInvariant` does the same job:

```java wrong id=java-turkish-lower
import java.util.Locale;

public class Main {
    public static void main(String[] args) {
        Locale.setDefault(Locale.forLanguageTag("tr-TR"));
        System.out.println("TITLE".toLowerCase());
        System.out.println("TITLE".toLowerCase(Locale.ROOT));
    }
}
```

```output
tıtle
title
```

- **A comparison that depends on the locale.** `localeCompare` sorts by the rules of a language, so it puts `é` between `e` and `f`. That order is good for people, but it is not code point order, and it can change from one locale to the next. In C#, `ThenBy` with no comparer uses the default comparer, and for strings that comparer uses the current culture. Give `StringComparer.Ordinal`:

```javascript wrong id=js-locale-compare
console.log(["f", "é", "e"].sort((a, b) => a.localeCompare(b)));
console.log(["f", "é", "e"].sort());
```

```output
[ 'e', 'é', 'f' ]
[ 'e', 'f', 'é' ]
```

- **Lower case is not case folding.** The task changes only `A` to `Z`. For text in other languages, full case folding is a different operation. Python `casefold` changes `ß` to `ss`, so `Straße` and `STRASSE` become one word. `lower` keeps them apart:

```python run id=py-casefold
print("Straße".lower(), "STRASSE".lower())
print("Straße".casefold(), "STRASSE".casefold())
```

```output
straße strasse
strasse strasse
```

- **`tolower` and `isalnum` in C.** These functions take an `int`. If the value is not an `unsigned char` value and is not `EOF`, the behavior is undefined. A UTF-8 byte in a `char` can be negative, so cast it to `unsigned char` first. The result of these functions also depends on the current locale. The C solution compares the bytes with `A` to `Z` itself.

- **`uniq` without `sort`.** `uniq` joins only lines that are next to each other. So you must sort the words before `uniq -c`. Set `LC_ALL=C` on each `sort`. In other locales, `sort` uses the collation rules of the locale, and the order of the words can change.

- **A word for people is a different rule.** The rule of this task splits `don't` and `e-mail`. For natural language, use the word boundaries of Unicode Standard Annex #29. In JavaScript, `Intl.Segmenter` with `granularity: "word"` follows these rules, and `isWordLike` marks the segments that are words:

```javascript run id=js-segmenter
const segmenter = new Intl.Segmenter("en", { granularity: "word" });
const segments = segmenter.segment("Don't stop, e-mail 3.14");
console.log([...segments].filter((s) => s.isWordLike).map((s) => s.segment));
```

```output
[ "Don't", 'stop', 'e', 'mail', '3.14' ]
```

## Related tasks

- [[split-a-string]]: split a text at separators.
- [[sort-a-list-of-records]]: sort by two keys, one of them in reverse.
- [[remove-duplicates-from-a-list]]: another job that uses a map or a set to find equal values.
- [[read-a-file-line-by-line]] and [[read-from-standard-input]]: read the text to count.
- [[utf-8]]: why byte order is code point order.

## References
