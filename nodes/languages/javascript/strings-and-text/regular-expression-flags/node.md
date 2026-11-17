---
tag: 3RDC37
kind: feature
owner: javascript
slug: regular-expression-flags
name: Regular expression flags
aliases: [RegExp flags, regex flags in JavaScript, the g flag, the u and v flags, sticky regex, dotAll, hasIndices, regex modifiers]
area: languages/javascript/strings-and-text
level: advanced
summary: >-
  The flags d, g, i, m, s, u, v and y change how a RegExp matches. They are fixed when the regex is made, each has a read-only property, and the i, m and s flags can be switched for part of a pattern.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/text-processing.html#sec-regexpinitialize", locator: "22.2.3.3 RegExpInitialize; 22.2.6.4 get RegExp.prototype.flags"}
see_also: [javascript/regular-expressions, javascript/unicode-and-utf-16-strings, javascript/string-methods, find-and-replace-with-a-regular-expression]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Regular_expressions
  title: MDN Web Docs, JavaScript Guide, Regular expressions
  locator: Advanced searching with flags
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/text-processing.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 22
  locator: 22.2.3.3 RegExpInitialize ( obj, pattern, flags )
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp/flags
  title: MDN Web Docs, JavaScript Reference, RegExp.prototype.flags
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp/multiline
  title: MDN Web Docs, JavaScript Reference, RegExp.prototype.multiline
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp/dotAll
  title: MDN Web Docs, JavaScript Reference, RegExp.prototype.dotAll
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp/unicode
  title: MDN Web Docs, JavaScript Reference, RegExp.prototype.unicode
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp/unicodeSets
  title: MDN Web Docs, JavaScript Reference, RegExp.prototype.unicodeSets
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp/sticky
  title: MDN Web Docs, JavaScript Reference, RegExp.prototype.sticky
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp/hasIndices
  title: MDN Web Docs, JavaScript Reference, RegExp.prototype.hasIndices
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp/exec
  title: MDN Web Docs, JavaScript Reference, RegExp.prototype.exec()
  locator: Return value
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Regular_expressions/Modifier
  title: MDN Web Docs, JavaScript Reference, Modifier
  locator: the introduction; Syntax; Description
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: s (dotAll) flag; RegExp Match Indices; RegExp v flag; RegExp Modifiers, Expected Publication Year
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp
  title: MDN Web Docs, JavaScript Reference, RegExp
  locator: Literal notation and constructor
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/re.html
  title: The Python Standard Library, Python 3.14, re
  locator: Flags
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const re = /a.c/gis;
console.log(re.flags, re.global, re.ignoreCase, re.dotAll);
console.log("A\nC abc".match(re));
console.log(new RegExp("x", "yg").flags);
try {
  new RegExp("x", "gg");
} catch (e) {
  console.log(e.name);
}
```

```output for=basics
gis true true true
[ 'A\nC', 'abc' ]
gy
SyntaxError
```

| Flag | Property | Effect |
|---|---|---|
| `d` | `hasIndices` | match results carry start and end indices |
| `g` | `global` | find every match |
| `i` | `ignoreCase` | ignore case |
| `m` | `multiline` | `^` and `$` match at each line |
| `s` | `dotAll` | `.` matches line terminators too |
| `u` | `unicode` | work on code points |
| `v` | `unicodeSets` | `u` plus set operations in classes |
| `y` | `sticky` | match only at `lastIndex` |

## Semantics

**r1.** "Regular expressions have optional flags that allow for functionality like global searching and case-insensitive searching. These flags can be used separately or together in any order, and are included as part of the regular expression."[^1] A literal puts them after the closing slash, and the constructor takes them as a second string.[^1] "Note that the flags are an integral part of a regular expression. They cannot be added or removed later."[^1]

**r2.** The specification allows only the letters `d`, `g`, `i`, `m`, `s`, `u`, `v` and `y`, and throws a SyntaxError for any other letter or for a letter given twice.[^2] Example: [`basics`](#basics).

**r3.** "The flags accessor property of RegExp instances returns the flags of this regular expression."[^3] "Flags in the flags property are sorted alphabetically".[^3] "The set accessor of flags is undefined . You cannot change this property directly."[^3] Each flag also has its own accessor, such as `global` and `dotAll`. Example: [`fixed`](#fixed).

**r4.** "The m flag indicates that a multiline input string should be treated as multiple lines. For example, if m is used, ^ and $ change from matching at only the start or end of the entire string to the start or end of any line within the string."[^4] With `s`, the dot "should additionally match the following line terminator"[^5] characters: LF, CR, U+2028 and U+2029.[^5] Example: [`lines`](#lines).

**r5.** "The u flag enables various Unicode-related features."[^6] With it, code point escapes and `\p{...}` escapes are read as such, and "Surrogate pairs will be interpreted as whole characters instead of two separate characters."[^6] "The v flag is an "upgrade" to the u flag that enables more Unicode-related features."[^7] It allows "intersection, union, and subtraction syntaxes" in character classes.[^7] "Because u and v interpret the same regex in incompatible ways, using both flags results in a SyntaxError ."[^7] Example: [`astral`](#astral). See [[javascript/unicode-and-utf-16-strings]].

**r6.** "The y flag indicates that the regex attempts to match the target string only from the index indicated by the lastIndex property (and unlike a global regex, does not attempt to match from any later indexes)."[^8] "When the exec() method is called on a sticky regex, if the regex fails to match at lastIndex , the regex immediately returns null and resets lastIndex to 0."[^8] Example: [`sticky`](#sticky).

**r7.** "The d flag indicates that the result of a regular expression match should contain the start and end indices of the substrings of each capture group. It does not change the regex's interpretation or matching behavior in any way, but only provides additional information in the matching result."[^9] Each entry of `indices` "is a two-element array, where the first number represents the match's start index, and the second number, its end index."[^10] Example: [`indices`](#indices).

**r8.** "A modifier overrides flag settings in a specific part of a regular expression. It can be used to enable or disable flags that change the meanings of certain regex syntax elements. These flags are i , m , and s ."[^11] The forms are `(?flags1:pattern)` and `(?flags1-flags2:pattern)`.[^11] Example: [`modifier`](#modifier).

## Examples

`m` and `s` with a string of two lines (rule r4).

```javascript run id=lines
const text = "one\ntwo";
console.log(text.match(/^\w+$/g), text.match(/^\w+$/gm));
console.log(/one.two/.test(text), /one.two/s.test(text));
```

```output for=lines
null [ 'one', 'two' ]
false true
```

A tokenizer with `y`: each match must start where the last one ended (rule r6).

```javascript run id=sticky
const tok = /\d+|[+*]/y;
const src = "12+3*4";
const out = [];
let m;
while ((m = tok.exec(src))) out.push(m[0]);
console.log(out.join(" "), tok.lastIndex);

const g = /\d+/g;
g.lastIndex = 2;
const y = /\d+/y;
y.lastIndex = 2;
console.log(g.exec(src)[0], y.exec(src));
```

```output for=sticky
12 + 3 * 4 0
3 null
```

At index 2 there is a `+`, so the global regex moves on and finds `3`, while the sticky one fails.

Start and end of each group with `d` (rule r7).

```javascript run id=indices
const m = /(?<year>\d{4})-(\d\d)/d.exec("on 2026-11");
console.log(m.indices[0], m.indices[1], m.indices[2], m.indices.groups.year);
```

```output for=indices
[ 3, 10 ] [ 3, 7 ] [ 8, 10 ] [ 3, 7 ]
```

Case-insensitive for one part of the pattern only (rule r8).

```javascript run id=modifier
const re = /(?i:hello) World/;
console.log(re.test("HELLO World"), re.test("HELLO WORLD"), re.flags === "");
```

```output for=modifier
true false true
```

## Versions

The TC39 list of finished proposals gives these expected publication years: the `s` (dotAll) flag 2018, RegExp Match Indices (the `d` flag) 2022, the RegExp `v` flag 2024 and RegExp Modifiers 2025.[^12] So `s` is in ECMAScript 2018, `d` in ECMAScript 2022, `v` in ECMAScript 2024 and modifiers in ECMAScript 2025. Node v24.10.0 runs all four, as the examples on this page show.

## Pitfalls

### Matching a character above U+FFFF without `u`

```javascript wrong id=astral
const smile = "\u{1f600}";
console.log(/^.$/.test(smile), /^.$/u.test(smile));
console.log(/[\p{L}--[a-z]]/v.test("a"), /[\p{L}--[a-z]]/v.test("A"));
try {
  new RegExp(".", "uv");
} catch (e) {
  console.log(e.name);
}
```

```output for=astral
false true
false true
SyntaxError
```

Without `u` or `v`, the emoji is two code units and `.` matches only one of them (rule r5). The `v` class subtracts the small letters a to z from all letters. Asking for both `u` and `v` is an error.

### Trying to turn on a flag later

```javascript wrong id=fixed
const re = /cat/;
const desc = Object.getOwnPropertyDescriptor(RegExp.prototype, "global");
console.log(desc.set, "cat cat".replace(re, "dog"));

const all = new RegExp(re, re.flags + "g");
console.log(all.flags, "cat cat".replace(all, "dog"));
```

```output for=fixed
undefined dog cat
g dog dog
```

`global` has no setter, so the flag cannot be changed on the object (rules r1 and r3). Make a new RegExp from the old one: the constructor takes a regex as its first argument and the flags as its second.[^13]

## Comparison

Python passes flags such as `re.IGNORECASE` to the functions of the `re` module, and a pattern can set them inline: with `(?aiLmsux)`, "the letters set the corresponding flags for the entire regular expression".[^14] The group form `(?aiLmsux-imsx:...)` is like a JavaScript modifier: "The letters set or remove the corresponding flags for the part of the expression".[^14] A JavaScript flag is part of the RegExp object (rule r1), and its inline form changes only `i`, `m` and `s` inside a group (rule r8). See [[javascript/regular-expressions]].

## References
