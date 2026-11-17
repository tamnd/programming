---
tag: 72VR4Y
kind: feature
owner: javascript
slug: string-escape-sequences
name: String escape sequences
aliases: [escape sequences in JavaScript, backslash escapes, Unicode escape sequence, Unicode code point escape, hexadecimal escape sequence, line continuation, legacy octal escape]
area: languages/javascript/strings-and-text
level: intro
summary: >-
  A backslash in a JavaScript string literal starts an escape such as \n, \', \\, \xHH, \uHHHH or \u{H...}. Another character after a backslash stands for itself. Octal escapes fail in strict mode.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html#sec-literals-string-literals", locator: "12.9.4 String Literals; 12.9.4.1 Static Semantics: Early Errors"}
see_also: [javascript/strings, javascript/template-literals, javascript/unicode-and-utf-16-strings, python/string-literals, python/raw-strings]
sources:
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 12
  locator: 12.9.4 String Literals; 12.9.4.1 (Early Errors)
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Lexical_grammar
  title: MDN Web Docs, JavaScript Reference, Lexical grammar
  locator: String literals; Escape sequences; Hexadecimal escape sequences; Unicode escape sequences; Unicode code point escapes
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Template_literals
  title: MDN Web Docs, JavaScript Reference, Template literals
  locator: Description; Tagged templates and escape sequences
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/raw
  title: MDN Web Docs, JavaScript Reference, String.raw()
  locator: the introduction
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/lexical_analysis.html
  title: The Python Language Reference, Python 3.14, Lexical analysis
  locator: 2.5.4.7 Unrecognized escape sequences
  accessed: 2026-11-18
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: String literals
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
console.log("quote:\" apostrophe:\' backslash:\\");
console.log(["\t", "\n", "\0"].map((c) => c.charCodeAt(0)).join(" "));
console.log("\xA9 \u00A9 \u{A9}", "\xA9" === "\u{A9}");
console.log("\u{1F600}" === "\uD83D\uDE00", "\u{1F600}".length);
console.log("\z" === "z", "a\
b");
```

```output for=basics
quote:" apostrophe:' backslash:\
9 10 0
© © © true
true 2
true ab
```

| Escape | Value |
|---|---|
| `\n`, `\r`, `\t`, `\v`, `\b`, `\f` | line feed, carriage return, tab, vertical tab, backspace, form feed |
| `\'`, `\"`, `\\` | the quote or the backslash itself |
| `\0` | U+0000, when no digit follows |
| `\xHH` | a code unit from 00 to FF |
| `\uHHHH` | one UTF-16 code unit |
| `\u{H...}` | a code point from 0 to 10FFFF |
| `\` and a line break | nothing: the literal continues on the next line |

## Semantics

**r1.** "A string literal is 0 or more Unicode code points enclosed in single or double quotes. Unicode code points may also be represented by an escape sequence. All code points may appear literally in a string literal except for the closing quote code points, U+005C (REVERSE SOLIDUS), U+000D (CARRIAGE RETURN), and U+000A (LINE FEED). Any code points may appear in the form of an escape sequence."[^1] See [[javascript/strings]].

**r2.** The single-character escapes are `\0`, `\'`, `\\`, `\n`, `\r`, `\v`, `\t`, `\b`, `\f`, and a backslash before a double quote.[^2] "Any escape sequence not listed below becomes an "identity escape" that becomes the code point itself. For example, \z is the same as z ."[^2] Example: [`basics`](#basics).

**r3.** Three escapes give a character by number. "Hexadecimal escape sequences consist of \x followed by exactly two hexadecimal digits representing a code unit or code point in the range 0x0000 to 0x00FF."[^2] "A Unicode escape sequence consists of exactly four hexadecimal digits following \u . It represents a code unit in the UTF-16 encoding."[^2] "A Unicode code point escape consists of \u{ , followed by a code point in hexadecimal base, followed by } . The value of the hexadecimal digits must be in the range 0 and 0x10FFFF inclusive. Code points in the range U+10000 to U+10FFFF do not need to be represented as a surrogate pair."[^2] See [[javascript/unicode-and-utf-16-strings]].

**r4.** A backslash at the end of a line is a line continuation, and its value is empty: "The SV of DoubleStringCharacter :: LineContinuation is the empty String."[^1] MDN: "Make sure there is no space or any other character after the backslash (except for a line break), otherwise it will not work. If the next line is indented, the extra spaces will also be present in the string's value."[^2] Example: [`continuation`](#continuation).

**r5.** Octal escapes such as `\101`, and `\8` and `\9`, are legacy syntax. For `LegacyOctalEscapeSequence` and `NonOctalDecimalEscapeSequence`, "It is a Syntax Error if IsStrict(this production) is true."[^1] Template literals reject them in all code: there, "\ followed by any decimal digit other than 0 , or \0 followed by a decimal digit" is a syntax error, along with "\x followed by fewer than two hex digits".[^3] Example: [`octal`](#octal).

**r6.** "String.raw() static method is a tag function of template literals."[^4] It gives the text as written, with each backslash kept, so it is a way to write a string with many backslashes. See [[javascript/template-literals]].

## Examples

Legacy octal escapes work in sloppy code only (rule r5). The `Function` constructor makes sloppy code unless its body starts with `"use strict"`.

```javascript run id=octal
console.log(new Function('return "\\101"')());
const bodies = ['"use strict"; return "\\101"', "return `\\101`", 'return "\\x4"'];
for (const body of bodies) {
  try {
    new Function(body);
    console.log("ok");
  } catch (e) {
    console.log(e.name);
  }
}
console.log("\x41" === "A", "\u{41}" === "A");
```

```output for=octal
A
SyntaxError
SyntaxError
SyntaxError
true true
```

## Pitfalls

### Backslashes in a Windows path

`\n` and `\t` are escapes, so this path holds a line feed and a tab, not four characters (rule r2). Double each backslash, or use `String.raw` (rule r6).

```javascript wrong id=path
const path = "C:\new\table";
console.log(path.length, path.includes("\n"), path.includes("\t"));

const fixed = "C:\\new\\table";
console.log(fixed, fixed.length, String.raw`C:\new\table` === fixed);
```

```output for=path
10 true true
C:\new\table 12 true
```

### Indentation after a line continuation

The spaces at the start of the next line stay in the string (rule r4).

```javascript wrong id=continuation
const message = "first part \
    second part";
console.log(JSON.stringify(message));

const joined = "first part " +
  "second part";
console.log(JSON.stringify(joined));
```

```output for=continuation
"first part     second part"
"first part second part"
```

## Comparison

Python keeps the backslash of an escape it does not know: "Unlike in Standard C, all unrecognized escape sequences are left in the string unchanged, that is, the backslash is left in the result".[^5] JavaScript drops it, so the escape `\q` gives just `q` (rule r2). Python 3.12 made such escapes a `SyntaxWarning`.[^5] In Go, "Raw string literals are character sequences between back quotes" and "Within the quotes, any character may appear except back quote."[^6] JavaScript has no raw string literal; `String.raw` with a template literal does the same job (rule r6). See [[python/string-literals]] and [[python/raw-strings]].

## References
