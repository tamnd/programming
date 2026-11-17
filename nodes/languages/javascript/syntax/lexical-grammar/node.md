---
tag: F55VDT
kind: feature
owner: javascript
slug: lexical-grammar
name: Lexical grammar
aliases: [lexical analysis, input elements, tokens, tokenizer, lexer, lexical goal symbols]
area: languages/javascript/syntax
level: core
summary: >-
  The lexical grammar cuts source text, a sequence of Unicode code points, into input elements: white space, line terminators, comments and tokens. It takes the longest match, with five goal symbols.
grammar: "InputElementDiv :: WhiteSpace | LineTerminator | Comment | CommonToken | DivPunctuator | RightBracePunctuator (ECMA-262, 12 ECMAScript Language: Lexical Grammar)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html#sec-ecmascript-language-lexical-grammar", locator: "12 ECMAScript Language: Lexical Grammar"}
see_also: [tokens-and-lexing, context-free-grammars, javascript/notational-conventions-of-ecma-262, javascript/white-space-and-line-terminators, javascript/comments, javascript/identifiers-and-reserved-words, javascript/punctuators, javascript/automatic-semicolon-insertion]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-source-code.html#sec-source-text
    title: ECMAScript 2026 Language Specification
    locator: 11.1 Source Text
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Lexical_grammar
    title: Lexical grammar, MDN
    locator: introduction
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Grammar_and_types
    title: Grammar and types, MDN JavaScript Guide
    locator: Basics
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html#sec-ecmascript-language-lexical-grammar
    title: ECMAScript 2026 Language Specification
    locator: "12 ECMAScript Language: Lexical Grammar; 12.6 Tokens"
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/lexical_analysis.html
    title: Lexical analysis, The Python Language Reference
    locator: 2.1.9 Whitespace between tokens; 2.2 Other tokens; 2.3.4 Non-ASCII characters in names
    accessed: 2026-11-18
---

## Synopsis

```text
source text (Unicode code points)
  -> input elements: WhiteSpace | LineTerminator | Comment | token
       token: IdentifierName | PrivateIdentifier | Punctuator
            | NumericLiteral | StringLiteral | Template
            | RegularExpressionLiteral | DivPunctuator | RightBracePunctuator
  -> white space and comments are dropped
  -> line terminators guide semicolon insertion
  -> the tokens go to the syntactic grammar
```

"ECMAScript source text is a sequence of Unicode code points."[^1] The lexical grammar of ECMA-262 clause 12 cuts this text into input elements. MDN calls this step lexical analysis, "in which the text gets scanned from left to right and is converted into a sequence of individual, atomic input elements".[^2] The syntactic grammar then reads the tokens.

## Semantics

**r1. Code points.** Any code point from U+0000 to U+10FFFF may occur where the grammar permits it. The file encoding is not part of the language: an engine works "as if it was an equivalent sequence of SourceCharacter values".[^1] An engine does not normalize the text, so two spellings of one letter, such as a precomposed letter and a letter with a combining mark, stay different.[^1] JavaScript "is case-sensitive and uses the Unicode character set".[^3] Example: [`no-normalize`](#no-normalize).

**r2. Escapes do not end comments.** A Unicode escape such as `\u000A` can stand for a code point in string literals, template literals, regular expression literals and identifiers. In a comment it is only text. ECMA-262 contrasts this with Java, where the escape `\u000A` in a comment acts as a line terminator; in JavaScript, "a Unicode escape sequence occurring within a comment is never interpreted".[^1] Example: [`escape-in-comment`](#escape-in-comment).

**r3. Four kinds of input element.** "The source text of an ECMAScript Script or Module is first converted into a sequence of input elements, which are tokens, line terminators, comments, or white space."[^4] White space and comments are dropped after this step. Line terminators and multi-line comments do not reach the parser as tokens, but "they guide the process for automatic semicolon insertion" ([[javascript/automatic-semicolon-insertion]]).[^2]

**r4. Tokens.** The common tokens are IdentifierName, PrivateIdentifier, Punctuator, NumericLiteral, StringLiteral and Template.[^4] Three more tokens depend on the goal symbol (rule r6): DivPunctuator (`/` and `/=`), RightBracePunctuator (`}`) and RegularExpressionLiteral. The names and keywords are in [[javascript/identifiers-and-reserved-words]], and the operators are in [[javascript/punctuators]].

**r5. The longest match.** "The source text is scanned from left to right, repeatedly taking the longest possible sequence of code points as the next input element."[^4] So `a+++b` is `a ++ + b`, and `1.toString()` fails: the scanner takes `1.` as one numeric literal, and the name `toString` cannot follow it. Examples: [`longest`](#longest), [`number-dot`](#number-dot).

**r6. Goal symbols.** A `/` can start a division or a regular expression literal, and a `}` can close a block or continue a template literal. The scanner cannot decide this alone, so the lexical grammar has five goal symbols, and the syntactic context picks one:[^4]

| Goal symbol | Where it is used |
|---|---|
| InputElementHashbangOrRegExp | at the start of a Script or Module |
| InputElementRegExpOrTemplateTail | where a regular expression literal, a TemplateMiddle or a TemplateTail can come |
| InputElementRegExp | where a regular expression literal can come, but no template part |
| InputElementTemplateTail | where a template part can come, but no regular expression literal |
| InputElementDiv | in all other contexts |

ECMA-262 notes that no context permits both a leading division and a leading regular expression literal, so the choice is never ambiguous.[^4] After an operand, as in `a = b` followed by a line break and `/hi/g`, the `/` is division. Example: [`div-or-regex`](#div-or-regex).

## Examples

The examples run with Node.js 24.

The longest match of rule r5. The scanner reads `+++` as `++` and `+`, so `a` is incremented and `b` is not. White space splits the tokens in another place:

```javascript run id=longest
let a = 1;
let b = 10;
console.log(a+++b);
console.log(a, b);
console.log(a+ +b, a - -b);
```

```output for=longest
11
2 10
12 12
```

The example from the note in ECMA-262 clause 12, with values.[^4] The second line starts with `/`, but after the operand `b` the goal symbol is InputElementDiv, so the text is `b / hi / g`, that is 84 / 2 / 3. After `=`, the same characters are a regular expression literal (rule r6):

```javascript run id=div-or-regex
const b = 84, hi = 2, g = 3;
const a = b
/hi/g
console.log(a);

const pattern = /hi/g;
console.log(typeof pattern, pattern.flags, "high hi".match(pattern));
```

```output for=div-or-regex
14
object g [ 'hi', 'hi' ]
```

An escape in a comment is only text (rule r2), so the call after `\u000A` stays inside the comment. In a string literal, the same escape puts a line feed in the value:

```javascript run id=escape-in-comment
// The escape \u000A does not end this comment: console.log("not run");
console.log("after the comment");
const s = "a\u000Ab";
console.log(s.length, JSON.stringify(s));
```

```output for=escape-in-comment
after the comment
3 "a\nb"
```

No normalization (rule r1). The example writes the names with escapes, which identifiers allow. The first name ends with the precomposed letter U+00E9, and the second ends with `e` and the combining accent U+0301. On screen both are the word café, but they are two identifiers and two different strings:

```javascript run id=no-normalize
const caf\u00e9 = "precomposed";
const cafe\u0301 = "combining";
console.log(caf\u00e9, cafe\u0301);
console.log("caf\u00e9" === "cafe\u0301", "caf\u00e9".length, "cafe\u0301".length);
console.log("cafe\u0301".normalize("NFC") === "caf\u00e9");
```

```output for=no-normalize
precomposed combining
false 4 5
true
```

The longest match takes `1.` as a numeric literal (rule r5), so a name cannot follow it:

```javascript fails id=number-dot expect=exception:SyntaxError
console.log(1.toString());
```

Node.js reports `SyntaxError: Invalid or unexpected token` before any line runs. Write `(1).toString()`, `1..toString()` or `1 .toString()`.

## Pitfalls

### A line that starts with `/`

A line break does not end a statement when the next line can continue it. A line that starts with `/` after an operand is division, not a regular expression literal ([`div-or-regex`](#div-or-regex)). End the statement before it with a semicolon.[^4]

### Letters that look the same

Two names can look the same and differ in their code points, because the engine does not normalize the source (rule r1).[^1] Text pasted from documents with combining marks gives such names. Normalize the files with an editor, or keep names in ASCII.

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| ambiguity rule | the longest possible sequence[^4] | the longest possible string that forms a legal token[^5] |
| white space | an input element that is dropped[^4] | not tokens, but serve to delimit tokens[^5] |
| line structure | line terminators guide semicolon insertion[^2] | NEWLINE, INDENT and DEDENT tokens[^5] |
| non-ASCII names | not normalized[^1] | converted into the normalization form NFKC[^5] |

Python's lexer turns indentation into tokens ([[python/line-structure]]), and the JavaScript lexer does not. The general theory is in [[tokens-and-lexing]].

## References
