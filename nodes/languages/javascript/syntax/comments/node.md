---
tag: 00SVXR
kind: feature
owner: javascript
slug: comments
name: Comments
aliases: [line comment, block comment, single-line comment, multi-line comment, "//", "/* */", JSDoc comment, HTML-like comments]
area: languages/javascript/syntax
level: intro
summary: >-
  A // comment runs to the end of the line, and a /* */ comment can span lines but cannot nest. Comments act as white space, but a block comment that holds a line break acts as a line terminator.
grammar: "Comment :: MultiLineComment | SingleLineComment; MultiLineComment :: /* MultiLineCommentChars opt */; SingleLineComment :: // SingleLineCommentChars opt (ECMA-262, 12.4 Comments)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html#sec-comments", locator: "12.4 Comments"}
see_also: [comment, javascript/lexical-grammar, javascript/hashbang-comments, javascript/white-space-and-line-terminators, javascript/automatic-semicolon-insertion, python/comments-in-python]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html#sec-comments
    title: ECMAScript 2026 Language Specification
    locator: 12.4 Comments
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Lexical_grammar#comments
    title: Lexical grammar, MDN
    locator: Comments
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Grammar_and_types#comments
    title: Grammar and types, MDN JavaScript Guide
    locator: Comments
    accessed: 2026-11-18
  - url: https://jsdoc.app/about-getting-started
    title: Getting Started with JSDoc, Use JSDoc
    locator: Getting started; Adding documentation comments to your code
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/additional-ecmascript-features-for-web-browsers.html#sec-html-like-comments
    title: ECMAScript 2026 Language Specification
    locator: Annex B introduction; B.1.1 HTML-like Comments
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/lexical_analysis.html#comments
    title: Lexical analysis, The Python Language Reference
    locator: 2.1.3 Comments
    accessed: 2026-11-18
---

## Synopsis

```javascript run id=synopsis
// a line comment: from // to the end of the line

/* a block comment
   that spans lines */

const price = 3;
const count = 4;
const total = price /* inside a line */ * count;

/**
 * A documentation comment for JSDoc.
 * @param {number} a
 */
const twice = (a) => a * 2;
console.log(total, twice(total));
```

```output for=synopsis
12 24
```

"Comments can be either single or multi-line. Multi-line comments cannot nest."[^1] MDN calls the two kinds "line comments and block comments", and gives their use: "to add hints, notes, suggestions, or warnings to JavaScript code", or to disable code.[^2] A third kind, the hashbang comment, is only valid at the start of a file ([[javascript/hashbang-comments]]).[^2]

## Semantics

**r1. Line comments.** A line comment starts with `//`. It "always consists of all code points from the // marker to the end of the line".[^1] The line terminator at the end is not part of the comment. The lexer reads it as its own input element, so the line break still counts for semicolon insertion.[^1] Example: [`comment-asi`](#comment-asi).

**r2. Block comments.** A block comment starts with `/*` and ends at the first `*/`. It can stand on one line, inside a line, or across several lines.[^2] It cannot nest: the first `*/` ends it, and the rest of the outer comment is read as code. To write `*/` inside a comment, break up the pair, for example as `*\/`.[^3] Examples: [`comment-kinds`](#comment-kinds), [`nested`](#nested).

**r3. Comments act as white space.** "Comments behave like white space and are discarded except that, if a MultiLineComment contains a line terminator code point, then the entire comment is considered to be a LineTerminator for purposes of parsing by the syntactic grammar."[^1] So `return /* ... */ 1` returns 1 when the comment is on one line, and returns `undefined` when the comment holds a line break ([[javascript/automatic-semicolon-insertion]]). Example: [`comment-asi`](#comment-asi).

**r4. Escapes in comments.** A Unicode escape such as `\u000A` in a comment is only text, so it does not end the comment ([[javascript/lexical-grammar]]).

**r5. Documentation comments.** JSDoc is "an API documentation generator for JavaScript". Its comments go just before the code that they document, and "Each comment must start with a /** sequence in order to be recognized by the JSDoc parser." Comments that start with `/*`, `/***` or more stars are skipped. Tags such as `@param` give more detail.[^4] For the engine, a JSDoc comment is a plain block comment.

**r6. HTML-like comments.** Annex B, the features that web browsers must keep, adds two more comment forms: `<!--` starts a comment that runs to the end of the line, and `-->` at the start of a line does the same. The extension "is not allowed when parsing source text using the goal symbol Module", and Annex B says programmers should not use its features.[^5] Node.js accepts these comments in a CommonJS script and rejects them in a module. Example: [`html-like`](#html-like).

## Examples

The examples run with Node.js 24.

A line comment, a block comment inside an expression, and code disabled by a block comment (rule r2):

```javascript run id=comment-kinds
// A line comment runs to the end of the line.
const price = 10;
const total = price /* a block comment inside a line */ * 3;
/*
console.log("this call is disabled");
*/
console.log(total);
```

```output for=comment-kinds
30
```

Comments next to `return` (rules r1 and r3). Only the first function returns 1. In `b` the block comment holds a line break, and in `c` the line break after the line comment stays in the input, so in both a semicolon is inserted after `return`:

```javascript run id=comment-asi
function a() {
  return /* a comment without a line break */ 1;
}
function b() {
  return /* a comment that holds
  a line break */ 1;
}
function c() {
  return // a line comment
  1;
}
console.log(a(), b(), c());
```

```output for=comment-asi
1 undefined undefined
```

The HTML-like comments of rule r6, run once as a script and once as a module:

```javascript run id=html-like
const { execFileSync } = require("node:child_process");

const source = [
  "const x = 1;",
  "<!-- in a script, this line is a comment",
  "--> and so is this one",
  "console.log('x is', x);",
].join("\n");

console.log(execFileSync(process.execPath, ["-e", source]).toString().trim());
try {
  execFileSync(process.execPath, ["--input-type=module", "-e", source], { stdio: "pipe" });
} catch (error) {
  console.log("module:", error.stderr.toString().match(/SyntaxError.*/)[0]);
}
```

```output for=html-like
x is 1
module: SyntaxError: HTML comments are not allowed in modules
```

A block comment inside a block comment (rule r2). The first `*/` ends the comment, and the words after it are code:

```javascript fails id=nested expect=exception:SyntaxError
/* outer /* inner */ still the outer comment */
console.log("never runs");
```

Node.js reports `SyntaxError: Unexpected identifier 'the'`, because `still` is read as a name and `the` cannot follow it.

## Pitfalls

### Commenting out code that holds a block comment

Wrapping code in `/* */` fails when the code already has a block comment: the inner `*/` ends the outer comment ([`nested`](#nested)). Use `//` on each line to disable code.[^3]

### A comment between `return` and its value

A block comment that spans lines after `return`, `break`, `continue` or `throw` ends the statement there (rule r3). Keep the value on the line of the keyword ([`comment-asi`](#comment-asi)).[^1]

### `*/` in a regular expression or a string

A block comment ends at the first `*/`, even when the comment holds code with a regular expression such as `/a*/`. Disable such code with line comments.[^1]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| line comment | `//` to the end of the line[^1] | `#` to the end of the physical line[^6] |
| block comment | `/* */`, cannot nest[^1] | none: the only comment is `#`[^6] |
| effect on line structure | a block comment with a line break is a line terminator[^1] | a comment ends the logical line[^6] |

For Python, "A comment starts with a hash character (#) that is not part of a string literal, and ends at the end of the physical line."[^6] See [[python/comments-in-python]].

## References
