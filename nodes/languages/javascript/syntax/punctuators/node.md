---
tag: S3X8FM
kind: feature
owner: javascript
slug: punctuators
name: Punctuators
aliases: [operators and delimiters, Punctuator, OptionalChainingPunctuator, DivPunctuator, RightBracePunctuator, operator tokens]
area: languages/javascript/syntax
level: advanced
summary: >-
  The punctuators are the operator and delimiter tokens. The scanner takes the longest one, and the goal symbol decides if / divides or starts a regex, and if } ends a block or a template part.
grammar: "Punctuator :: OptionalChainingPunctuator | OtherPunctuator; OptionalChainingPunctuator :: ?. [lookahead ∉ DecimalDigit]; DivPunctuator :: / | /=; RightBracePunctuator :: } (ECMA-262, 12.8 Punctuators)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html#sec-punctuators", locator: "12.8 Punctuators"}
see_also: [operator, javascript/lexical-grammar, javascript/identifiers-and-reserved-words, javascript/automatic-semicolon-insertion, javascript/comments, operator-precedence, python/operators-and-delimiters]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html#sec-punctuators
    title: ECMAScript 2026 Language Specification
    locator: "12 ECMAScript Language: Lexical Grammar; 12.6 Tokens; 12.8 Punctuators; 12.9.5 Regular Expression Literals; 12.9.6 Template Literal Lexical Components"
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Lexical_grammar
    title: Lexical grammar, MDN
    locator: introduction
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/lexical_analysis.html#operators-and-delimiters
    title: Lexical analysis, The Python Language Reference
    locator: 2.1.9 Whitespace between tokens; 2.7 Operators and delimiters
    accessed: 2026-11-18
---

## Synopsis

```text
OtherPunctuator  { ( ) [ ] . ... ; , < > <= >= == != === !== + - * % ** ++ --
                 << >> >>> & | ^ ! ~ && || ?? ? : = += -= *= %= **= <<= >>=
                 >>>= &= |= ^= &&= ||= ??= =>
OptionalChainingPunctuator  ?.   (not before a decimal digit)
DivPunctuator               / /=
RightBracePunctuator        }
```

Punctuators are the tokens that are neither names nor literals: the operators, the brackets and the separators.[^1] MDN calls them "punctuators (mostly operators)".[^2] The meaning of each operator is in the pages of the operators; this page is about the tokens.

## Semantics

**r1. The token list.** The grammar of 12.8 lists the punctuators in four productions: OptionalChainingPunctuator, OtherPunctuator, DivPunctuator and RightBracePunctuator.[^1] Punctuator, the first two, is part of CommonToken. "The DivPunctuator, RegularExpressionLiteral, RightBracePunctuator, and TemplateSubstitutionTail productions derive additional tokens that are not included in the CommonToken production."[^1]

**r2. The longest match.** The scanner takes "the longest possible sequence of code points as the next input element".[^1] So `>>>=` is one token, not `>>` and `>=`, and `n --> 0` is `n -- > 0`. The rule looks only at the characters: `x+++++x` is `x ++ ++ + x`, which does not parse, while `x++ + ++x` with spaces does. Examples: [`tokens`](#tokens), [`five-plus`](#five-plus).

**r3. `?.` before a digit.** The token `?.` is OptionalChainingPunctuator only when "[lookahead ∉ DecimalDigit]".[^1] The rule keeps the old meaning of a conditional with a decimal fraction: `yes?.5:1` is `yes ? .5 : 1`. Example: [`tokens`](#tokens).

**r4. `/` and `/=`.** These are not in Punctuator, because the same character can start a RegularExpressionLiteral. The goal symbol InputElementDiv has DivPunctuator, and InputElementRegExp has RegularExpressionLiteral.[^1] Where an operand can come, as at the start of a statement or after `(` or `=`, the `/` starts a regular expression. After an operand it is division ([[javascript/lexical-grammar]]). Example: [`slash`](#slash).

**r5. `//` and the empty regex.** "Regular expression literals may not be empty; instead of representing an empty regular expression literal, the code unit sequence // starts a single-line comment. To specify an empty regular expression, use: /(?:)/."[^1] Example: [`slash`](#slash).

**r6. `}` in templates.** A `}` ends a block, an object literal or a template substitution. In a template, the TemplateMiddle and TemplateTail tokens start with `}`, so the scanner must know that a substitution is open.[^1] The goal symbol InputElementTemplateTail, or InputElementRegExpOrTemplateTail, gives that context, and InputElementDiv and InputElementRegExp have RightBracePunctuator instead.[^1] Example: [`slash`](#slash).

## Examples

The examples run with Node.js 24.

The rules of the longest match and of `?.` (rules r2 and r3):

```javascript run id=tokens
const yes = true;
const missing = null;
console.log(yes?.5:1);
console.log(missing?.length, missing ?? "default");

let n = 3;
const seen = [];
while (n --> 0) seen.push(n);
console.log(seen);

let x = 8;
x >>>= 1;
const add = (a, b) => a + b;
console.log(x, add(x, 1) >= 5);
```

```output for=tokens
0.5
undefined default
[ 2, 1, 0 ]
4 true
```

The loop condition `n --> 0` is `n-- > 0`; there is no `-->` operator.

The `/` and `}` cases of rules r4 to r6. In the first line, `{}` is a block at the start of a statement, so the `/` after it starts a regular expression. In the second line, `({})` is an operand, so the `/` divides. In the third line both slashes follow operands. The last line has two substitutions, each with a `}` of its own inside:

```javascript run id=slash
console.log(eval("{} /foo/.source"));
console.log(eval("({}) / 2"));
console.log(eval("let a = 6, g = 2; a /2/ g"));
console.log(String(new RegExp("")), /(?:)/.test("any text"));
console.log(`a${ { x: 1 }.x }b${ [2, 3].length }c`);
```

```output for=slash
foo
NaN
1.5
/(?:)/ true
a1b2c
```

The longest match does not look at the grammar (rule r2). Without spaces, the scanner reads `++` twice after `x`:

```javascript fails id=five-plus expect=exception:SyntaxError
let x = 1;
const y = x+++++x;
console.log(y);
```

Node.js reports `SyntaxError: Unexpected token '++'`. With spaces, `x++ + ++x` is valid.

## Pitfalls

### Joined operators

`a+++b`, `a-->b` and `x+++++x` depend on the longest match (rule r2). Put spaces around binary operators so that the reader sees the same tokens as the scanner.

### `?.` with a number after it

`obj?.5` is not optional property access, because of the lookahead of rule r3. To read a property with a numeric name, use `obj?.[5]`.[^1]

### A line that starts with `/` or a template

After an operand, a `/` at the start of a line is division (rule r4), and a template literal at the start of a line continues the expression before it as a tagged template. Both are cases for an explicit semicolon ([[javascript/automatic-semicolon-insertion]]).[^1]

## Comparison

| Token | JavaScript | Python |
|---|---|---|
| `//` | starts a comment[^1] | an operator[^3] |
| `===`, `!==` | strict equality | not in the list[^3] |
| `?.`, `??` | optional chaining, nullish coalescing[^1] | not in the list[^3] |
| `++`, `--` | increment, decrement[^1] | not in the list[^3] |
| `:=`, `@` | not in the list[^1] | operators[^3] |
| tie-break | the longest token[^1] | the longest token[^3] |

Python lists its tokens as operators and delimiters ([[python/operators-and-delimiters]]). The same two characters `//` are a comment in JavaScript and an operator in Python. Both lexers use the longest match.

## References
