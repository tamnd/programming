---
tag: XH7857
kind: feature
owner: javascript
slug: regular-expressions
name: Regular expressions
aliases: [RegExp, regex in JavaScript, regular expression literals, exec and test, matchAll, named capturing groups, RegExp.escape]
area: languages/javascript/strings-and-text
level: core
summary: >-
  A regular expression is a RegExp object, written as a /pattern/flags literal or built with new RegExp. exec and test search with it, and so do the string methods match, matchAll, replace and split.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/text-processing.html#sec-regexp-regular-expression-objects", locator: "22.2 RegExp (Regular Expression) Objects; 12.9.5 Regular Expression Literals"}
see_also: [javascript/string-methods, javascript/regular-expression-flags, javascript/strings, find-and-replace-with-a-regular-expression, python/string-pattern-matching]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Regular_expressions
  title: MDN Web Docs, JavaScript Guide, Regular expressions
  locator: the introduction; Creating a regular expression; Using regular expressions in JavaScript
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp
  title: MDN Web Docs, JavaScript Reference, RegExp
  locator: the introduction; Literal notation and constructor
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 12
  locator: 12.9.5 Regular Expression Literals, Note 1
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp/exec
  title: MDN Web Docs, JavaScript Reference, RegExp.prototype.exec()
  locator: the introduction; Return value; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/match
  title: MDN Web Docs, JavaScript Reference, String.prototype.match()
  locator: the introduction; Return value
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/matchAll
  title: MDN Web Docs, JavaScript Reference, String.prototype.matchAll()
  locator: the introduction; Parameters; Return value
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Regular_expressions/Named_capturing_group
  title: MDN Web Docs, JavaScript Reference, Named capturing group
  locator: the introduction
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/replace
  title: MDN Web Docs, JavaScript Reference, String.prototype.replace()
  locator: the introduction; Specifying a string as the replacement
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp/escape
  title: MDN Web Docs, JavaScript Reference, RegExp.escape()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/text-processing.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 22
  locator: 22.2.5.1 RegExp.escape ( S )
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: RegExp.escape, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/re.html
  title: The Python Standard Library, Python 3.14, re
  locator: the introduction
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/regex/Pattern.html
  title: Java SE 25 API, Class Pattern
  locator: the class description
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const literal = /(\d{4})-(\d{2})/;
const built = new RegExp("(\\d{4})-(\\d{2})");
const text = "from 2025-03 to 2026-11";
console.log(literal.source === built.source, literal.test(text));
const m = literal.exec(text);
console.log(m[0], m[1], m[2], m.index);
console.log(text.search(literal), "no date".match(literal));
console.log(text.match(/\d{4}/g));
```

```output for=basics
true true
2025-03 2025 03 5
5 null
[ '2025', '2026' ]
```

| Call | Result |
|---|---|
| `re.test(s)` | `true` or `false` |
| `re.exec(s)` | a match array with groups, or `null` |
| `s.match(re)` | like `exec` without `g`; all matched texts with `g` |
| `s.matchAll(re)` | an iterator of match arrays; `re` needs `g` |
| `s.search(re)` | the index of the first match, or `-1` |
| `s.replace(re, r)` | a new string; `r` is a string or a function |

## Semantics

**r1.** "Regular expressions are patterns used to match character combinations in strings. In JavaScript, regular expressions are also objects."[^1] "The RegExp object is used for matching text with a pattern."[^2] The patterns "are used with the exec() and test() methods of RegExp , and with the match() , matchAll() , replace() , replaceAll() , search() , and split() methods of String ."[^1] Example: [`basics`](#basics).

**r2.** There are two ways to make one. "Regular expression literals provide compilation of the regular expression when the script is loaded. If the regular expression remains constant, using this can improve performance."[^1] "Using the constructor function provides runtime compilation of the regular expression."[^1] "Use a string as the first argument to the RegExp() constructor when you want to build the regular expression from dynamic input."[^2] In that string a backslash must itself be escaped, so `/\d/` is `new RegExp("\\d")`.

**r3.** A literal makes a new object each time it runs: "A regular expression literal is an input element that is converted to a RegExp object (see 22.2) each time the literal is evaluated. Two regular expression literals in a program evaluate to regular expression objects that never compare as === to each other even if the two literals' contents are identical."[^3]

**r4.** `exec` "executes a search with this regular expression for a match in a specified string and returns a result array, or null ."[^4] The array holds the whole match at index 0 and the groups after it: "Parentheses around any part of the regular expression pattern causes that part of the matched substring to be remembered."[^1] Its `groups` property is "A null -prototype object of named capturing groups, whose keys are the names, and values are the capturing groups, or undefined if no named capturing groups were defined."[^4]

**r5.** `match` depends on the `g` flag. "If the g flag is used, all results matching the complete regular expression will be returned, but capturing groups are not included."[^5] "If the g flag is not used, only the first complete match and its related capturing groups are returned. In this case, match() will return the same result as RegExp.prototype.exec()".[^5] To get the groups of every match, "you need to use RegExp.prototype.exec() or String.prototype.matchAll() instead."[^5]

**r6.** `matchAll` "returns an iterator of all results matching this string against a regular expression, including capturing groups."[^6] "If regexp is a regex, then it must have the global (g ) flag set, or a TypeError is thrown."[^6] Example: [`named`](#named).

**r7.** "A named capturing group is a particular kind of capturing group that allows you to give a name to the group. The group's matching result can later be identified by this name instead of by its index in the pattern."[^7] The syntax is `(?<name>pattern)`. In a replacement string, `$<Name>` "Inserts the named capturing group where Name is the group name."[^8] `replace` "returns a new string with one, some, or all matches of a pattern replaced by a replacement . The pattern can be a string or a RegExp , and the replacement can be a string or a function called for each match."[^8]

**r8.** "JavaScript RegExp objects are stateful when they have the global or sticky flags set (e.g., /foo/g or /foo/y ). They store a lastIndex from the previous match."[^4] "If the match fails, the exec() method returns null , and sets the regex's lastIndex to 0 ."[^4] Example: [`state`](#state). See [[javascript/regular-expression-flags]].

**r9.** `RegExp.escape` "escapes any potential regex syntax characters in a string, and returns a new string that can be safely used as a literal pattern for the RegExp() constructor."[^9] The specification: "This function returns a copy of S in which characters that are potentially special in a regular expression Pattern have been replaced by equivalent escape sequences."[^10] Example: [`input`](#input).

## Examples

Named groups with `matchAll` and `replace` (rules r6 and r7).

```javascript run id=named
const date = /(?<year>\d{4})-(?<month>\d{2})/g;
const text = "from 2025-03 to 2026-11";
for (const m of text.matchAll(date)) {
  console.log(m.index, m.groups.year, m.groups.month);
}
console.log(text.replace(date, "$<month>/$<year>"));
console.log(text.replace(date, (all, y, mo) => `${mo}.${y}`));
try {
  text.matchAll(/\d{4}/);
} catch (e) {
  console.log(e.name);
}
```

```output for=named
5 2025 03
16 2026 11
from 03/2025 to 11/2026
from 03.2025 to 11.2026
TypeError
```

A function replacement gets the whole match first and then each group.

## Versions

The TC39 list of finished proposals gives `RegExp.escape` the expected publication year 2025, so it is part of ECMAScript 2025.[^11] Node v24.10.0 does not have it, so the [`input`](#input) example tests for it and falls back to a hand-written escape.

## Pitfalls

### Reusing a `g` regex with `test`

```javascript wrong id=state
const word = /cat/g;
console.log(word.test("cat"), word.test("cat"), word.lastIndex);

const once = /cat/;
console.log(once.test("cat"), once.test("cat"));
```

```output for=state
true false 0
true true
```

The first call leaves `lastIndex` at 3, so the second search starts past the end of `"cat"` and fails, which puts `lastIndex` back to 0 (rule r8). Drop the `g` flag when you only ask whether a string matches.

### Building a pattern from user input

```javascript wrong id=input
const term = "1+1=2?";
const text = "is 1+1=2?";
console.log(new RegExp(term).test(text));

const escape = typeof RegExp.escape === "function"
  ? RegExp.escape
  : (s) => s.replace(/[\\^$.*+?()[\]{}|\/]/g, "\\$&");
console.log(new RegExp(escape(term)).test(text));
```

```output for=input
false
true
```

As a pattern, `1+1=2?` means one or more `1`, then `1=`, then an optional `2`, so it does not find the text `1+1=2?`. Escape the input first (rule r9). MDN: "When dynamically creating a RegExp with user-provided content, consider using this function".[^9]

## Comparison

Python has the `re` module: "This module provides regular expression matching operations similar to those found in Perl."[^12] Java has `java.util.regex.Pattern`, "A compiled representation of a regular expression. A regular expression, specified as a string, must first be compiled into an instance of this class."[^13] JavaScript has a literal syntax for the pattern as well as the constructor (rule r2), and the string methods take the RegExp object directly (rule r1). See [[python/string-pattern-matching]] and [[find-and-replace-with-a-regular-expression]].

## References
