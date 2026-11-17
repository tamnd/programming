---
tag: MN6DB0
kind: feature
owner: javascript
slug: collation
name: Collation
aliases: [Intl.Collator, localeCompare, locale-aware string sorting, sensitivity option, numeric collation, caseFirst, usage search]
area: languages/javascript/strings-and-text
level: advanced
summary: >-
  Intl.Collator compares strings by the rules of a locale. Its compare function sorts arrays, and options set what counts as a difference: case, accents, digits as numbers, or a variant order.
grammar: null
spec: {standard: "ECMA-402, 13th edition (ECMAScript 2026 Internationalization API)", url: "https://tc39.es/ecma402/2026/#collator-objects", locator: "10 Collator Objects; 10.3.3 get Intl.Collator.prototype.compare; 10.3.3.2 CompareStrings"}
see_also: [javascript/string-normalization-and-comparison, javascript/internationalization, javascript/locale-information-and-negotiation, sort-a-list, sort-with-a-custom-key]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Internationalization
  title: MDN Web Docs, JavaScript Guide, Internationalization
  locator: Collation
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/Collator
  title: MDN Web Docs, JavaScript Reference, Intl.Collator
  locator: the introduction; Examples
  accessed: 2026-11-18
- url: https://tc39.es/ecma402/2026/
  title: ECMAScript 2026 Internationalization API Specification (ECMA-402, 13th edition)
  locator: 10.3.3 get Intl.Collator.prototype.compare; 10.3.3.2 CompareStrings
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/Collator/Collator
  title: MDN Web Docs, JavaScript Reference, Intl.Collator() constructor
  locator: Parameters; usage; numeric; caseFirst; sensitivity
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/localeCompare
  title: MDN Web Docs, JavaScript Reference, String.prototype.localeCompare()
  locator: the introduction
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/locale.html
  title: The Python Standard Library, Python 3.14, locale
  locator: locale.strcoll; locale.strxfrm
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/text/Collator.html
  title: Java SE 25 API, Class Collator
  locator: the class description
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const words = ["Z", "a", "z", "ä"];
console.log([...words].sort());
console.log([...words].sort(new Intl.Collator("de").compare));
console.log([...words].sort(new Intl.Collator("sv").compare));
console.log([...words].sort(new Intl.Collator("de", { caseFirst: "upper" }).compare));
```

```output for=basics
[ 'Z', 'a', 'z', 'ä' ]
[ 'a', 'ä', 'z', 'Z' ]
[ 'a', 'z', 'Z', 'ä' ]
[ 'a', 'ä', 'Z', 'z' ]
```

| Option | Values | Effect |
|---|---|---|
| `usage` | `sort`, `search` | ordering a list, or finding matches |
| `sensitivity` | `base`, `accent`, `case`, `variant` | which differences make strings unequal |
| `numeric` (`-u-kn-`) | `true`, `false` | digits compared as numbers |
| `caseFirst` (`-u-kf-`) | `upper`, `lower`, `false` | which case sorts first |
| `collation` (`-u-co-`) | `phonebk`, `pinyin`, `emoji` and others | a variant order of the locale |
| `ignorePunctuation` | `true`, `false` | punctuation skipped |

## Semantics

**r1.** "The Intl.Collator object is useful for comparing and sorting strings. It takes two strings and returns a number indicating their relative order, in the same way as the compareFn argument of the Array.prototype.sort method."[^1] The order depends on the locale: in the Synopsis German puts `ä` next to `a`, and Swedish puts it after `z`.[^2] Example: [`basics`](#basics).

**r2.** The `compare` property is a getter that "returns a function that compares two strings according to the sort order of this Collator object."[^3] "The returned function is bound to collator so that it can be passed directly to Array.prototype.sort or other functions."[^3] The result "will be negative when x is ordered before y, positive when x is ordered after y, and zero in all other cases".[^3] MDN: "the specification requires only that the before and after values are negative and positive."[^2]

**r3.** `sensitivity` sets "Which differences in the strings should lead to non-zero result values."[^4] With `base`, "Only strings that differ in base letters compare as unequal."[^4] With `accent`, case is ignored but accents count, and with `case`, accents are ignored but case counts.[^4] "The default is "variant" for usage "sort"".[^4] Example: [`sensitivity`](#sensitivity).

**r4.** "There are two distinct use cases for collation: sorting and searching."[^1] "When searching, you should only pay attention to whether the comparison result is zero (equal) or not, not the sign of the result."[^1] The option is `usage: "search"`, and MDN warns against using `search` for sorting.[^4] Example: [`search`](#search).

**r5.** `numeric` sets whether digits are compared as numbers, so that `test-2` sorts before `test-10`.[^4] `caseFirst` sets "Whether upper case or lower case should sort first."[^4] Each can also be set with a Unicode extension key in the tag, `kn` and `kf`; "if both are provided, this options property takes precedence."[^4] Example: [`numeric`](#numeric).

**r6.** "There are many different ways to sort, even within the same locale. For example, there are two different sort orders in German, phonebook and dictionary."[^1] The `collation` option or the `-u-co-` key picks one.[^4] Example: [`variants`](#variants).

**r7.** `String.prototype.localeCompare` takes the same `locales` and `options`. "In implementations with Intl.Collator API support, this method delegates to Intl.Collator . When comparing large numbers of strings, such as in sorting large arrays, it is better to create an Intl.Collator object and use the function provided by its compare() method."[^5] See [[javascript/string-normalization-and-comparison]].

## Examples

The four sensitivity levels on three pairs (rule r3).

```javascript run id=sensitivity
const pairs = [["a", "b"], ["a", "á"], ["a", "A"]];
for (const sensitivity of ["base", "accent", "case", "variant"]) {
  const c = new Intl.Collator("en", { sensitivity });
  console.log(sensitivity, pairs.map(([x, y]) => (c.compare(x, y) === 0 ? "=" : "!=")).join(" "));
}
```

```output for=sensitivity
base != = =
accent != != =
case != = !=
variant != != !=
```

Finding the entries that match a key, without regard to case or accents (rules r4 and r7).

```javascript run id=search
const names = ["Zoë", "zoe", "Zoey", "ZOE"];
const match = new Intl.Collator("en", { usage: "search", sensitivity: "base" });
console.log(names.filter((n) => match.compare(n, "zoe") === 0));
console.log("réservé".localeCompare("RESERVE"), "réservé".localeCompare("RESERVE", "en", { sensitivity: "base" }));
```

```output for=search
[ 'Zoë', 'zoe', 'ZOE' ]
1 0
```

Digits as numbers, set by option or by tag (rule r5).

```javascript run id=numeric
const files = ["test-10", "test-2", "test-1"];
console.log([...files].sort());
console.log([...files].sort(new Intl.Collator("en", { numeric: true }).compare));
const tagged = new Intl.Collator("en-u-kn");
console.log([...files].sort(tagged.compare), tagged.resolvedOptions().numeric);
```

```output for=numeric
[ 'test-1', 'test-10', 'test-2' ]
[ 'test-1', 'test-2', 'test-10' ]
[ 'test-1', 'test-2', 'test-10' ] true
```

German phonebook order beside the default German order (rule r6).

```javascript run id=variants
const names = ["Hochberg", "Hönigswald", "Holzman"];
const phonebook = new Intl.Collator("de-DE-u-co-phonebk");
console.log([...names].sort(phonebook.compare).join(", "));
console.log([...names].sort(new Intl.Collator("de-DE").compare).join(", "));
console.log(phonebook.resolvedOptions().collation);
```

```output for=variants
Hochberg, Hönigswald, Holzman
Hochberg, Holzman, Hönigswald
phonebk
```

The phonebook order sorts `ö` as if it were `oe`.[^1]

## Pitfalls

### Sorting words with the default `sort`

```javascript wrong id=order
const words = ["zebra", "île", "apple", "Zulu"];
console.log([...words].sort());
console.log([...words].sort(new Intl.Collator("fr").compare));
```

```output for=order
[ 'Zulu', 'apple', 'zebra', 'île' ]
[ 'apple', 'île', 'zebra', 'Zulu' ]
```

Without a compare function, `sort` orders by UTF-16 code units, so capitals come before small letters and accented letters come after `z`. MDN: "Unicode code point order doesn't make sense".[^1] Pass a collator's `compare` (rule r1).

### Matching with `toLowerCase` and `===`

```javascript wrong id=lower
const query = "angstrom";
const stored = ["\u00C5ngstr\u00F6m", "A\u030Angstro\u0308m", "angstroms"];
console.log(stored.map((s) => s.toLowerCase() === query));
const base = new Intl.Collator("en", { usage: "search", sensitivity: "base" });
console.log(stored.map((s) => base.compare(s, query) === 0));
```

```output for=lower
[ false, false, false ]
[ true, true, false ]
```

Lower case leaves the accents in place, in both the composed and the decomposed spelling. A collator with `sensitivity: "base"` sees past both (rules r3 and r4).

## Comparison

Python compares by the process locale with `locale.strcoll`, and `locale.strxfrm` "Transforms a string to one that can be used in locale-aware comparisons."[^6] Java's `Collator` "performs locale-sensitive String comparison. You use this class to build searching and sorting routines for natural language text."[^7] Java's strengths `PRIMARY`, `SECONDARY` and `TERTIARY` match the `base`, `accent` and `variant` sensitivities, as the MDN option list says for the Unicode collation algorithm.[^4][^7] In JavaScript each collator carries its own locale and options (rule r1). See [[sort-with-a-custom-key]].

## References
