---
tag: H982H2
kind: feature
owner: javascript
slug: string-methods
name: String methods
aliases: [JavaScript string methods, String.prototype methods, slice and substring, indexOf and includes, replace and replaceAll, split, padStart, trim]
area: languages/javascript/strings-and-text
level: core
summary: >-
  The methods on String.prototype query, search, compose, split and transform a string. Each one returns a new value and leaves the string as it was. Indexes count UTF-16 code units.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/text-processing.html#sec-properties-of-the-string-prototype-object", locator: "22.1.3 Properties of the String Prototype Object"}
see_also: [javascript/strings, javascript/unicode-and-utf-16-strings, split-a-string, replace-a-substring, python/string-methods]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Numbers_and_strings
  title: MDN Web Docs, JavaScript Guide, Numbers and strings
  locator: String object
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/replace
  title: MDN Web Docs, JavaScript Reference, String.prototype.replace()
  locator: the introduction; Specifying a string as the replacement
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/at
  title: MDN Web Docs, JavaScript Reference, String.prototype.at()
  locator: the introduction
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/slice
  title: MDN Web Docs, JavaScript Reference, String.prototype.slice()
  locator: the introduction; Parameters
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/substring
  title: MDN Web Docs, JavaScript Reference, String.prototype.substring()
  locator: Differences between substring() and slice()
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/indexOf
  title: MDN Web Docs, JavaScript Reference, String.prototype.indexOf()
  locator: Return value; Checking occurrences
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/padStart
  title: MDN Web Docs, JavaScript Reference, String.prototype.padStart()
  locator: the introduction
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/split
  title: MDN Web Docs, JavaScript Reference, String.prototype.split()
  locator: the introduction; Parameters
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/replaceAll
  title: MDN Web Docs, JavaScript Reference, String.prototype.replaceAll()
  locator: the introduction; Exceptions
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/text-processing.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 22
  locator: 22.1.3 Properties of the String Prototype Object
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/stdtypes.html
  title: The Python Standard Library, Python 3.14, Built-in Types
  locator: String Methods, str.split
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, Class String
  locator: the class description
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const s = "  Hello, World  ";
const t = s.trim();
console.log(JSON.stringify(t), t.length, s.length);
console.log(t.at(0), t.at(-1), t.indexOf("o"), t.lastIndexOf("o"));
console.log(t.includes("World"), t.startsWith("Hell"), t.endsWith("!"));
console.log(t.slice(7), t.slice(-5, -2), t.toUpperCase());
console.log(t.split(", "), t.replace("l", "L"), t.replaceAll("l", "L"));
console.log("7".padStart(3, "0"), "ab".repeat(3), "a".concat("b", 1));
```

```output for=basics
"Hello, World" 12 16
H d 4 8
true true false
World Wor HELLO, WORLD
[ 'Hello', 'World' ] HeLlo, World HeLLo, WorLd
007 ababab ab1
```

| Kind | Methods |
|---|---|
| query | `at`, `charAt`, `charCodeAt`, `codePointAt` |
| search | `indexOf`, `lastIndexOf`, `includes`, `startsWith`, `endsWith`, `search`, `match`, `matchAll` |
| composition | `concat`, `padStart`, `padEnd`, `repeat` |
| decomposition | `slice`, `substring`, `split`, `trim`, `trimStart`, `trimEnd` |
| transformation | `toLowerCase`, `toUpperCase`, `replace`, `replaceAll`, `normalize`, `toWellFormed` |

## Semantics

**r1.** MDN's guide groups the methods by what they do. "Query: get the character or character code at a particular string index." "Search: get information about a substring that conforms to a pattern, or test if a particular substring exists." "Composition: combine strings into a longer string." "Decomposition: break a string into smaller strings." "Transformation: return a new string based on the current string's content."[^1] A string is immutable, so none of them changes it: `replace`, for example, "does not mutate the string value it's called on. It returns a new string."[^2] See [[javascript/strings]].

**r2.** The methods take positions in UTF-16 code units. `at` "returns a new String consisting of the single UTF-16 code unit located at the specified offset", and "Negative integers count back from the last string character."[^3] `slice` "extracts a section of this string and returns it as a new string", and "If indexStart < 0 , the index is counted from the end of the string."[^4] See [[javascript/unicode-and-utf-16-strings]].

**r3.** `substring` is like `slice` with two differences. It "swaps its two arguments if indexStart is greater than indexEnd , meaning that a string is still returned. The slice() method returns an empty string if this is the case."[^5] And "If either or both of the arguments are negative or NaN , the substring() method treats them as if they were 0 ."[^5] Example: [`slicing`](#slicing).

**r4.** `indexOf` returns "The index of the first occurrence of searchString found, or -1 if not found."[^6] So "the correct way to check is test whether the return value is -1".[^6] `includes`, `startsWith` and `endsWith` return a boolean.

**r5.** The composition methods build a longer string. `padStart` "pads this string with a given string (repeated and/or truncated, if needed) so that the resulting string has a given length."[^7] Example: [`basics`](#basics).

**r6.** `split` "takes a pattern and divides this string into an ordered list of substrings by searching for the pattern, puts these substrings into an array, and returns the array."[^8] The optional limit stops the split after that many entries: "Any leftover text is not included in the array at all."[^8] Example: [`split`](#split).

**r7.** With a string pattern, `replace` changes one match: "If pattern is a string, only the first occurrence will be replaced."[^2] `replaceAll` "returns a new string with all matches of a pattern replaced by a replacement", and "If pattern is a regex, then it must have the global (g ) flag set, or a TypeError is thrown."[^9] In a replacement string, `$` starts a pattern: `$$` gives one `$`, and "$& Inserts the matched substring."[^2] Example: [`dollar`](#dollar).

**r8.** The methods do not need a real string as `this`. Of several of them, the specification says: "This method is intentionally generic; it does not require that its this value be a String object."[^10]

## Examples

`slice` and `substring` with the same arguments (rule r3).

```javascript run id=slicing
const text = "Mozilla";
console.log(JSON.stringify(text.slice(5, 2)), text.substring(5, 2));
console.log(text.slice(-3), text.substring(-3));
console.log(text.slice(2, -1), JSON.stringify(text.substring(2, -1)));
```

```output for=slicing
"" zil
lla Mozilla
zill "Mo"
```

`split` with a limit, and with an empty separator (rules r2 and r6).

```javascript run id=split
const csv = "a,b,c,d";
console.log(csv.split(","), csv.split(",", 2));
console.log("a,,b".split(","), "".split(","), "abc".split(""));
```

```output for=split
[ 'a', 'b', 'c', 'd' ] [ 'a', 'b' ]
[ 'a', '', 'b' ] [ '' ] [ 'a', 'b', 'c' ]
```

## Pitfalls

### Expecting `replace` to change every match

```javascript wrong id=replace
const path = "a/b/c";
console.log(path.replace("/", "-"));
console.log(path.replaceAll("/", "-"), path.replace(/\//g, "-"));
try {
  path.replaceAll(/\//, "-");
} catch (e) {
  console.log(e.name);
}
```

```output for=replace
a-b/c
a-b-c a-b-c
TypeError
```

### A `$` in the replacement text

The replacement is a pattern too (rule r7), so a price like `$&5` comes out changed. A function as the replacement is used as written.

```javascript wrong id=dollar
const template = "Total: PRICE";
console.log(template.replace("PRICE", "$&5"));
console.log(template.replace("PRICE", "$$5"));
console.log(template.replace("PRICE", () => "$&5"));
```

```output for=dollar
Total: PRICE5
Total: $5
Total: $&5
```

### Testing `indexOf` as a boolean

A match at the start gives 0, which is falsy, and no match gives -1, which is truthy (rule r4).

```javascript wrong id=indexof
const word = "apple";
console.log(word.indexOf("a") ? "found" : "missing");
console.log(word.indexOf("z") ? "found" : "missing");
console.log(word.includes("a"), word.indexOf("z") !== -1);
```

```output for=indexof
missing
found
true false
```

## Comparison

Python's `str.split` has a maxsplit argument: "If maxsplit is given, at most maxsplit splits are done (thus, the list will have at most maxsplit+1 elements)."[^11] So the last element keeps the rest of the string, while the JavaScript limit drops it (rule r6). Java's `String` "includes methods for examining individual characters of the sequence, for comparing strings, for searching strings, for extracting substrings, and for creating a copy of a string with all characters translated to uppercase or to lowercase."[^12] Java's `replace(CharSequence target, CharSequence replacement)` "Replaces each substring of this string that matches the literal target sequence with the specified literal replacement sequence."[^12] The JavaScript `replace` with a string pattern changes only the first match (rule r7). See [[python/string-methods]], [[split-a-string]] and [[replace-a-substring]].

## References
