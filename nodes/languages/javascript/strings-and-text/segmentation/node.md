---
tag: M3EQ0P
kind: feature
owner: javascript
slug: segmentation
name: Segmentation
aliases: [Intl.Segmenter, grapheme segmentation in JavaScript, word segmentation, sentence segmentation, isWordLike, Segments containing]
area: languages/javascript/strings-and-text
level: advanced
summary: >-
  Intl.Segmenter splits a string into graphemes, words or sentences by the rules of a locale. Each segment has its text, its code unit index and, for words, whether it is word-like.
grammar: null
spec: {standard: "ECMA-402, 13th edition (ECMAScript 2026 Internationalization API)", url: "https://tc39.es/ecma402/2026/#segmenter-objects", locator: "19 Segmenter Objects; 19.5 Segments Objects; 19.7 Segment Data Objects"}
see_also: [javascript/unicode-and-utf-16-strings, javascript/internationalization, grapheme-cluster, count-the-graphemes-of-a-string]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/Segmenter
  title: MDN Web Docs, JavaScript Reference, Intl.Segmenter
  locator: the introduction; Examples
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Internationalization
  title: MDN Web Docs, JavaScript Guide, Internationalization
  locator: Segmentation
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/Segmenter/Segmenter
  title: MDN Web Docs, JavaScript Reference, Intl.Segmenter() constructor
  locator: Syntax; Parameters; granularity
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/Segmenter/segment
  title: MDN Web Docs, JavaScript Reference, Intl.Segmenter.prototype.segment()
  locator: Return value
  accessed: 2026-11-18
- url: https://tc39.es/ecma402/2026/
  title: ECMAScript 2026 Internationalization API Specification (ECMA-402, 13th edition)
  locator: 19 Segmenter Objects
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/Segments/containing
  title: MDN Web Docs, JavaScript Reference, Segments.prototype.containing()
  locator: the introduction; Return value
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/stdtypes.html
  title: The Python Standard Library, Python 3.14, Built-in Types
  locator: Text Sequence Type
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/text/BreakIterator.html
  title: Java SE 25 API, Class BreakIterator
  locator: the class description
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const flags = "\u{1F1FA}\u{1F1F8}\u{1F1E8}\u{1F1F3}\u{1F1EC}\u{1F1E7}";
console.log(flags.length, [...flags].length);
const graphemes = new Intl.Segmenter("en", { granularity: "grapheme" });
console.log([...graphemes.segment(flags)].length);
console.log([...graphemes.segment("e\u0301")].length, "e\u0301".length);
```

```output for=basics
12 6
3
1 2
```

| Granularity | One segment is | Extra field |
|---|---|---|
| `grapheme` (the default) | a user-perceived character | |
| `word` | a word, or the text between words | `isWordLike` |
| `sentence` | a sentence with its trailing space | |

## Semantics

**r1.** "The Intl.Segmenter object enables locale-sensitive text segmentation, enabling you to get meaningful items (graphemes, words or sentences) from a string."[^1] A string can already be split into UTF-16 code units or code points, "But as you can see, Unicode code points are not the same as what human users perceive as discrete characters."[^2] "When the user interacts with text, a grapheme is the smallest unit of text they can manipulate, such as delete or select."[^2] Example: [`basics`](#basics). See [[javascript/unicode-and-utf-16-strings]].

**r2.** The `granularity` option is `grapheme`, the default, which splits "at grapheme cluster (user-perceived character) boundaries, as determined by the locale", or `word` or `sentence`, which split at word or sentence boundaries "as determined by the locale."[^3] "Intl.Segmenter() can only be constructed with new . Attempting to call it without new throws a TypeError ."[^3]

**r3.** `segment(input)` returns "A new iterable Segments object containing the segments of the input string, using the segmenter's locale and granularity."[^4] In ECMA-402, "A Segments instance is an object that represents the segments of a specific string, subject to the locale and options of its constructing Intl.Segmenter instance."[^5] Each segment is an object with `segment`, "A string containing the segment extracted from the original input string", `index`, "The code unit index in the original input string at which the segment begins", and `input`.[^6] Example: [`words`](#words).

**r4.** With `word`, "The result is the input string, split into segments of words and non-words. If you are counting words, you should filter out the non-words by checking each segment's isWordLike property."[^2] The field is a boolean only for the `word` granularity, and `undefined` otherwise.[^6] "Word segmentation works for character-based languages too."[^2] Example: [`split`](#split).

**r5.** With `sentence`, "the segmenter doesn't remove any characters. It just splits the string into segments, each of which is a sentence."[^2] Example: [`sentences`](#sentences).

**r6.** `containing(index)` "returns an object describing the segment in the string that includes the code unit at the specified index", "or undefined if the supplied index value is out of bounds."[^6] It finds one segment without walking the whole string. Example: [`containing`](#containing).

## Examples

Words and the text between them (rules r3 and r4).

```javascript run id=words
const words = new Intl.Segmenter("en-US", { granularity: "word" });
const str = "It can even split non-space-separated words";
const parts = [...words.segment(str)];
console.log(parts.filter((s) => s.isWordLike).map((s) => s.segment).join(" "));
console.log(parts.slice(0, 3).map((s) => `${s.index}:${JSON.stringify(s.segment)}:${s.isWordLike}`));
```

```output for=words
It can even split non space separated words
[ '0:"It":true', '2:" ":false', '3:"can":true' ]
```

Sentences keep their punctuation and spaces (rule r5).

```javascript run id=sentences
const sentences = new Intl.Segmenter("en-US", { granularity: "sentence" });
console.log([...sentences.segment("I ate a sandwich. Then I went to bed.")].map((s) => s.segment));
```

```output for=sentences
[ 'I ate a sandwich. ', 'Then I went to bed.' ]
```

The word at a code unit index (rule r6).

```javascript run id=containing
const segs = new Intl.Segmenter("fr", { granularity: "word" }).segment("Allons-y!");
const hit = segs.containing(7);
console.log(hit.segment, hit.index, hit.isWordLike);
console.log(segs.containing(99));
```

```output for=containing
y 7 true
undefined
```

## Pitfalls

### Cutting a string by code points

```javascript wrong id=cut
const text = "ok \u{1F44D}\u{1F3FD} go";
const g = [...new Intl.Segmenter("en").segment(text)].map((s) => s.segment);
console.log(text.length, [...text].length, g.length);
console.log([...text].slice(0, 4).join("") === g.slice(0, 4).join(""));
```

```output for=cut
10 8 7
false
```

The thumbs-up with a skin tone is two code points and one grapheme, so taking four code points drops the skin tone. Cut at grapheme boundaries (rule r1).

### Counting words with `split`

```javascript wrong id=split
const text = "我是这篇文档的作者";
console.log(text.split(/\s+/).length);
const zh = new Intl.Segmenter("zh-Hans", { granularity: "word" });
console.log([...zh.segment(text)].filter((s) => s.isWordLike).length);
```

```output for=split
1
7
```

MDN lists the problems with splitting on spaces: "not all languages use spaces to separate words, not all spaces are word-separating, and not all words are separated by spaces."[^2] Use the `word` granularity (rule r4).

### Expecting abbreviations to stay inside a sentence

```javascript wrong id=abbrev
const sentences = new Intl.Segmenter("en-US", { granularity: "sentence" });
console.log([...sentences.segment("Mr. Smith left. He was late.")].map((s) => s.segment));
```

```output for=abbrev
[ 'Mr. ', 'Smith left. ', 'He was late.' ]
```

MDN notes that the segmenter does not support "sentence segmentation suppressions", which would keep a break from following a period such as the one in `Mr.`.[^2] Join such segments by hand if the text has them.

## Comparison

Python's strings "are immutable sequences of Unicode code points",[^7] so `len` counts code points and the standard library has no grapheme splitter. Java's `BreakIterator` "implements methods for finding the location of boundaries in text", with `getWordInstance`, `getLineInstance`, `getSentenceInstance` and `getCharacterInstance` for each kind of boundary.[^8] JavaScript's `Intl.Segmenter` gives segment objects for graphemes, words and sentences (rule r3). See [[grapheme-cluster]] and [[count-the-graphemes-of-a-string]].

## References
