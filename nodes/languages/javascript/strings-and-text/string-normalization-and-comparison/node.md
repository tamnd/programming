---
tag: 5PK74R
kind: feature
owner: javascript
slug: string-normalization-and-comparison
name: String normalization and comparison
aliases: [String.prototype.normalize, NFC and NFD in JavaScript, localeCompare, comparing strings in JavaScript, canonical equivalence, string sort order]
area: languages/javascript/strings-and-text
level: advanced
summary: >-
  The < and === operators compare strings by UTF-16 code units, so two spellings of one character differ. normalize() maps a string to NFC, NFD, NFKC or NFKD, and localeCompare sorts by language rules.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/text-processing.html#sec-string.prototype.normalize", locator: "22.1.3.15 String.prototype.normalize; 7.2.12 IsLessThan"}
see_also: [javascript/strings, javascript/unicode-and-utf-16-strings, javascript/string-methods, normalize-unicode-text, compare-strings]
sources:
- url: https://tc39.es/ecma262/2026/multipage/abstract-operations.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 7
  locator: 7.2.12 IsLessThan ( x, y, LeftFirst ), Note 2
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/normalize
  title: MDN Web Docs, JavaScript Reference, String.prototype.normalize()
  locator: the introduction; Parameters; Exceptions; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/localeCompare
  title: MDN Web Docs, JavaScript Reference, String.prototype.localeCompare()
  locator: the introduction; Return value
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/text-processing.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 22
  locator: 22.1.3.12 String.prototype.localeCompare
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
  title: MDN Web Docs, JavaScript Reference, Array.prototype.sort()
  locator: Description
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/unicodedata.html
  title: The Python Standard Library, Python 3.14, unicodedata
  locator: unicodedata.normalize
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, Class String
  locator: compareTo; compareToIgnoreCase
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const composed = "\u00f1";
const decomposed = "n\u0303";
console.log(composed === decomposed, composed.length, decomposed.length);
console.log(composed.normalize("NFD") === decomposed, decomposed.normalize() === composed);
console.log(composed.localeCompare(decomposed));
const ligature = "\ufb00";
console.log(ligature.normalize("NFKC"), ligature.normalize("NFC") === ligature);
```

```output for=basics
false 1 2
true true
0
ff true
```

| Form | Meaning |
|---|---|
| `NFC` (the default) | canonical decomposition, then canonical composition |
| `NFD` | canonical decomposition |
| `NFKC` | compatibility decomposition, then canonical composition |
| `NFKD` | compatibility decomposition |

## Semantics

**r1.** The relational operators and `===` work on code units: "The comparison of Strings uses a simple lexicographic ordering on sequences of UTF-16 code unit values. There is no attempt to use the more complex, semantically oriented definitions of character or string equality and collating order defined in the Unicode specification."[^1] "Therefore String values that are canonically equal according to the Unicode Standard but not in the same normalization form could test as unequal."[^1] Example: [`basics`](#basics).

**r2.** The same text can have more than one encoding: "sometimes more than one code point, or sequence of code points, can represent the same abstract character".[^2] "In Unicode, two sequences of code points have canonical equivalence if they represent the same abstract characters, and should always have the same visual appearance and behavior".[^2] `normalize` "returns the Unicode Normalization Form of this string."[^2] It takes the form as a string, one of `NFC`, `NFD`, `NFKC` and `NFKD`, and uses NFC when the argument is left out.[^2] Any other form throws: "RangeError Thrown if form isn't one of the values specified above."[^2]

**r3.** The K forms go further: compatible sequences represent the same abstract characters but need not be treated alike everywhere, and "All canonically equivalent sequences are also compatible, but not vice versa."[^2] So the ligature U+FB00 becomes the two letters `ff` under NFKC but stays as it is under NFC. See [[normalize-unicode-text]].

**r4.** Code unit order is not code point order: "Also note that lexicographic ordering by code unit differs from ordering by code point for Strings containing surrogate pairs."[^1] Example: [`order`](#order).

**r5.** `localeCompare` "returns a number indicating whether this string comes before, or after, or is the same as the given string in sort order."[^3] It returns "A negative number if referenceStr occurs before compareString ; positive if the referenceStr occurs after compareString ; 0 if they are equivalent."[^3] The specification requires it "to recognize and honour canonical equivalence according to the Unicode Standard, including returning +0𝔽 when comparing distinguishable Strings that are canonically equivalent".[^4] For many comparisons, "it is better to create an Intl.Collator object and use the function provided by its compare() method."[^3] See [[compare-strings]].

**r6.** `Array.prototype.sort` without a compare function uses code units too: "The default sort order is ascending, built upon converting the elements into strings, then comparing their sequences of UTF-16 code unit values."[^5] Example: [`sort`](#sort).

## Examples

A character above U+FFFF can sort before one below it (rule r4).

```javascript run id=order
const half = "\uff61";
const smile = "\u{1f600}";
console.log(half < smile, half.codePointAt(0) < smile.codePointAt(0));
console.log(half.charCodeAt(0).toString(16), smile.charCodeAt(0).toString(16));
```

```output for=order
false true
ff61 d83d
```

The leading surrogate 0xD83D is smaller than 0xFF61, so `<` puts the emoji first, while its code point 0x1F600 is larger.

## Pitfalls

### Looking up text that came in another form

```javascript wrong id=lookup
const names = new Set(["Zo\u00eb"]);
const typed = "Zoe\u0308";
console.log(names.has(typed), names.has(typed.normalize()));

const key = (s) => s.normalize("NFC");
const saved = new Set([...names].map(key));
console.log(saved.has(key(typed)));
```

```output for=lookup
false true
true
```

`Set` and `Map` compare keys by code units (rule r1). Normalize both sides to one form before storing or looking up.

### Sorting words with `sort()` and no compare function

```javascript wrong id=sort
const words = ["éclair", "zebra", "Eagle", "apple"];
console.log([...words].sort().join(" "));
console.log([...words].sort(new Intl.Collator("en").compare).join(" "));
```

```output for=sort
Eagle apple zebra éclair
apple Eagle éclair zebra
```

The default sort puts every capital before every small letter and every accented letter after `z` (rule r6). A collator sorts by the rules of a language (rule r5).

## Comparison

Python has `unicodedata.normalize(form, unistr)`, which does the same job: "Return the normal form form for the Unicode string unistr."[^6] Java's `compareTo` "Compares two strings lexicographically", like the JavaScript `<` (rule r1), and `compareToIgnoreCase` does the same "ignoring case differences."[^7] In JavaScript that job falls to `localeCompare` and `Intl.Collator` with a sensitivity option (rule r5). See [[compare-strings]] and [[normalize-unicode-text]].

## References
