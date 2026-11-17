---
tag: XBWERF
kind: feature
owner: javascript
slug: white-space-and-line-terminators
name: White space and line terminators
aliases: [whitespace, line breaks, newlines, LF, CR, LS, PS, ZWNBSP, BOM, format-control characters]
area: languages/javascript/syntax
level: intro
summary: >-
  White space (TAB, VT, FF, ZWNBSP and the Space_Separator characters) only separates tokens. The four line terminators (LF, CR, LS, PS) also end single-line comments and guide semicolon insertion.
grammar: "WhiteSpace :: <TAB> | <VT> | <FF> | <ZWNBSP> | <USP>; LineTerminator :: <LF> | <CR> | <LS> | <PS> (ECMA-262, 12.2 White Space; 12.3 Line Terminators)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html#sec-white-space", locator: "12.1 Unicode Format-Control Characters; 12.2 White Space; 12.3 Line Terminators"}
see_also: [javascript/lexical-grammar, javascript/comments, javascript/automatic-semicolon-insertion, javascript/identifiers-and-reserved-words, python/line-structure, tokens-and-lexing]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html#sec-white-space
    title: ECMAScript 2026 Language Specification
    locator: 12.1 Unicode Format-Control Characters; 12.2 White Space, Table 30; 12.3 Line Terminators, Table 31; 12.9.4.1 Static Semantics, SV; 12.9.6.1 Static Semantics, TV and TRV
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Lexical_grammar
    title: Lexical grammar, MDN
    locator: Format-control characters; White space; Line terminators
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/lexical_analysis.html
    title: Lexical analysis, The Python Language Reference
    locator: 2.1.1 Logical lines; 2.1.2 Physical lines; 2.1.8 Indentation
    accessed: 2026-11-18
---

## Synopsis

| Kind | Code points | Role |
|---|---|---|
| white space | U+0009 TAB, U+000B VT, U+000C FF, U+FEFF ZWNBSP, and every Space_Separator (USP), which includes U+0020 SPACE and U+00A0 NBSP | separates tokens |
| line terminator | U+000A LF, U+000D CR, U+2028 LS, U+2029 PS | separates tokens, ends a `//` comment, guides semicolon insertion |
| format control | category Cf, such as U+200C ZWNJ and U+200D ZWJ | allowed in comments and literals; ZWNJ and ZWJ can be parts of a name |

The table follows ECMA-262 12.1 to 12.3 and MDN.[^1] [^2]

## Semantics

**r1. White space.** "White space code points are used to improve source text readability and to separate tokens (indivisible lexical units) from each other, but are otherwise insignificant."[^1] It may occur between any two tokens and at the start or end of the input. Inside a string literal, a regular expression literal or a template, it is part of the value. It cannot occur inside any other token.[^1] Example: [`white-space`](#white-space).

**r2. Only these code points.** The white space set is the five entries of Table 30. ECMA-262 leaves out on purpose the other code points that have the Unicode property White_Space but are not in the category Space_Separator.[^1] So U+0085 NEXT LINE is not white space in the source, and a program that holds it between two tokens does not parse. MDN says U+0085 "has no special role".[^2] U+180E MONGOLIAN VOWEL SEPARATOR is a format-control character, not white space.[^2] Example: [`white-space`](#white-space).

**r3. Format-control characters.** These are the characters in category Cf. All of them may be used in comments, string literals, template literals and regular expression literals.[^1] U+FEFF, the byte order mark, is a special case: outside comments and literals it counts as white space.[^1] MDN adds that ZWNJ and ZWJ "are treated as identifier parts" ([[javascript/identifiers-and-reserved-words]]).[^2]

**r4. Line terminators.** There are four: LF, CR, LS and PS. "Only the Unicode code points in Table 31 are treated as line terminators."[^1] They separate tokens like white space, "unlike white space code points, line terminators have some influence over the behaviour of the syntactic grammar".[^1] A few productions forbid a line terminator in one place, and line terminators drive automatic semicolon insertion ([[javascript/automatic-semicolon-insertion]]).[^1] "The sequence <CR><LF> is commonly used as a line terminator." It counts as one line for the line numbers of error reports.[^1] Example: [`line-terminators`](#line-terminators).

**r5. Line terminators inside tokens.** "A line terminator cannot occur within any token except a StringLiteral, Template, or TemplateSubstitutionTail."[^1] A string literal is stricter: it may hold a raw LS or PS, but LF and CR only as part of a line continuation, a backslash before the line break, which adds nothing to the value.[^1] A template may hold any line break, and its CR and CR LF become LF in the value.[^1] A single-line comment ends at the first line terminator, and a multi-line comment may hold them ([[javascript/comments]]).[^1] Examples: [`line-terminators`](#line-terminators), [`raw-line-feed`](#raw-line-feed).

**r6. `\s` in regular expressions.** "Line terminators are included in the set of white space code points that are matched by the \s class in regular expressions."[^1] So `\s` matches LS and ZWNBSP, and it does not match U+0085.

## Examples

The examples run with Node.js 24. They build source text with `eval`, so each code point can be written as an escape.

Each white space code point of rule r1 can stand between the tokens of `1 + 2`. NEXT LINE and MONGOLIAN VOWEL SEPARATOR cannot (rule r2):

```javascript run id=white-space
const spaces = [
  ["U+0009 TAB", "\t"],
  ["U+000B VT", "\v"],
  ["U+000C FF", "\f"],
  ["U+00A0 NBSP", "\u00A0"],
  ["U+FEFF ZWNBSP", "\uFEFF"],
  ["U+3000 USP", "\u3000"],
  ["U+0085 NEL", "\u0085"],
  ["U+180E MVS", "\u180E"],
];
for (const [name, cp] of spaces) {
  try {
    console.log(name.padEnd(14), eval(`1${cp}+${cp}2`));
  } catch (error) {
    console.log(name.padEnd(14), error.name);
  }
}
```

```output for=white-space
U+0009 TAB     3
U+000B VT      3
U+000C FF      3
U+00A0 NBSP    3
U+FEFF ZWNBSP  3
U+3000 USP     3
U+0085 NEL     SyntaxError
U+180E MVS     SyntaxError
```

U+3000 IDEOGRAPHIC SPACE is in the category Space_Separator, so it is white space through USP.

Each of the four line terminators, and CR LF, ends a single-line comment (rule r4), so the string after it is the result of `eval`. The second part shows rule r5 and rule r6: a raw LS in a string literal, a template whose CR LF becomes LF, a line continuation, and `\s`:

```javascript run id=line-terminators
const terminators = [["LF", "\n"], ["CR", "\r"], ["CR LF", "\r\n"], ["LS", "\u2028"], ["PS", "\u2029"]];
for (const [name, lt] of terminators) {
  console.log(name.padEnd(5), eval(`// a comment${lt}"after the comment"`));
}

console.log(eval('"a\u2028b"').length);
console.log(eval("`a\r\nb`") === "a\nb");
console.log(eval('"a\\\nb"'));
console.log(/\s/.test("\u2028"), /\s/.test("\uFEFF"), /\s/.test("\u0085"));
```

```output for=line-terminators
LF    after the comment
CR    after the comment
CR LF after the comment
LS    after the comment
PS    after the comment
3
true
ab
true true false
```

A raw line feed cannot occur in a string literal (rule r5):

```javascript fails id=raw-line-feed expect=exception:SyntaxError
const text = "first line
second line";
console.log(text);
```

Node.js reports `SyntaxError: Invalid or unexpected token`. Write `\n` in the string, or use a template literal, which may span lines.

## Pitfalls

### Invisible characters in pasted code

Text copied from a document or a web page can hold U+00A0 NBSP or U+FEFF. Both are white space between tokens (rule r1), so the program still runs. Inside a string or a name they are part of the value, and two strings that look the same compare as different.[^1]

### LS and PS end a comment

An editor may show U+2028 or U+2029 as no line break at all. The engine treats them as line terminators (rule r4), so they end a `//` comment, and the text after them is code.[^1]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| line terminators | LF, CR, CR LF, LS, PS[^1] | LF, CR LF, CR[^3] |
| role of a line break | separates tokens; semicolon insertion | ends a logical line, a NEWLINE token[^3] |
| leading white space | insignificant[^1] | indentation, INDENT and DEDENT tokens[^3] |

In Python a line break ends a statement unless the line is joined, and the indentation forms blocks ([[python/line-structure]]). In JavaScript neither has a role, except through semicolon insertion.

## References
