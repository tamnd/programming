---
tag: 9F7YSB
kind: feature
owner: javascript
slug: automatic-semicolon-insertion
name: Automatic semicolon insertion
aliases: [ASI, semicolon insertion, optional semicolons, no-semicolon style, restricted productions, "[no LineTerminator here]"]
area: languages/javascript/syntax
level: core
summary: >-
  The parser inserts a semicolon before a token that does not fit after a line break or before }, at the end of input, and after return, break, continue, throw or yield at a line break.
grammar: "ReturnStatement : return ; | return [no LineTerminator here] Expression ; (ECMA-262, 12.10 Automatic Semicolon Insertion, restricted productions)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html#sec-automatic-semicolon-insertion", locator: "12.10 Automatic Semicolon Insertion"}
see_also: [javascript/lexical-grammar, javascript/white-space-and-line-terminators, javascript/comments, javascript/statements-and-declarations, javascript/punctuators, statements-and-expressions, python/line-structure, python/statements-and-suites]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html#sec-automatic-semicolon-insertion
    title: ECMAScript 2026 Language Specification
    locator: 12.10.1 Rules of Automatic Semicolon Insertion; 12.10.2 Examples; 12.10.3.1 Interesting Cases in Statement Lists
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Lexical_grammar#automatic_semicolon_insertion
    title: Lexical grammar, MDN
    locator: Automatic semicolon insertion
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Grammar_and_types#basics
    title: Grammar and types, MDN JavaScript Guide
    locator: Basics
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/lexical_analysis.html#logical-lines
    title: Lexical analysis, The Python Language Reference
    locator: 2.1.1 Logical lines; 2.1.5 Explicit line joining; 2.1.6 Implicit line joining
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/simple_stmts.html
    title: Simple statements, The Python Language Reference
    locator: 7. Simple statements
    accessed: 2026-11-18
---

## Synopsis

```javascript run id=synopsis
function f() {
  return        // a semicolon goes here: f returns undefined
    "value";
}

let a, b = 1, c = 1;
a = b           // here too, and ++ applies to c
++c

const g = (s) => () => s;
const x = "called";
const h = g     // no semicolon here: g(x) is a call
(x)

console.log(f(), a, c, h());
```

```output for=synopsis
undefined 1 2 called
```

"Most ECMAScript statements and declarations must be terminated with a semicolon." Such semicolons "may be omitted from the source text in certain situations", and the parser then inserts them.[^1] The rules do not say that each line break ends a statement. The parser inserts a semicolon only where the next token does not fit, and after a few keywords.

## Semantics

**r1. The offending token.** The parser reads the tokens from left to right. When a token "is not allowed by any production of the grammar", a semicolon is inserted before it if one of these is true:[^1]

- a LineTerminator separates it from the previous token;
- the token is `}`;
- the previous token is `)`, and the semicolon would end a `do`-`while` statement.

A block comment that holds a line break counts as a line terminator ([[javascript/comments]]).[^2] Example: [`inserted`](#inserted).

**r2. The end of the input.** When the input ends and the tokens are not a complete program, a semicolon is inserted at the end.[^1]

**r3. Restricted productions.** Some productions have the mark "[no LineTerminator here]". A line break at that place inserts a semicolon even when the next token would fit.[^1] The places are:[^1]

| Production | Effect of a line break |
|---|---|
| postfix `++`, `--` | the operator applies to the next line, as a prefix |
| `continue`, `break` | the label is dropped |
| `return`, `throw` | `return` returns `undefined`; `throw` is a syntax error |
| `yield` | `yield` gives `undefined` |
| arrow function | a line break before `=>` is a syntax error |
| `async` | the `async` is not part of the function or method after it |

ECMA-262 adds the advice: "A postfix ++ or -- operator should be on the same line as its operand", and the expression after `return`, `throw` or `yield` should start on the same line.[^1] Example: [`inserted`](#inserted).

**r4. Two places where no semicolon goes.** ECMA-262 adds an overriding condition: "a semicolon is never inserted automatically if the semicolon would then be parsed as an empty statement or if that semicolon would become one of the two semicolons in the header of a for statement".[^1] So `if (x)` followed by a line break and `const y = 1` is an error, and not an empty `if` ([`no-empty-statement`](#no-empty-statement)).[^2]

**r5. Lines that continue the line before.** Rule r1 needs a token that does not fit, and many tokens fit at the start of a line. After an expression, a semicolon is required if the next line begins with `(`, `[`, a template literal, unary `+` or `-`, or a regular expression literal.[^1] Without it, the lines join into a call, an index, a tagged template, a binary `+` or `-`, or a division. Examples: [`call-trap`](#call-trap), [`index-trap`](#index-trap), [`joined`](#joined).

**r6. The two styles.** MDN: "It is considered best practice, however, to always write a semicolon after a statement, even when it is not strictly needed."[^3] Code without semicolons must follow the rules of r3 and r5, and MDN suggests a semicolon at the start of each line that begins with `(`, `[`, `` ` ``, `+`, `-` or `/`.[^2] Example: [`leading-semicolon`](#leading-semicolon).

## Examples

The examples run with Node.js 24. Most lines have no semicolons, so the parser inserts them.

Rules r1 to r3. `return` at the end of a line returns `undefined`. The `++` on its own line is a prefix `++c`, not `b++`. The block `{ 1` and `2 }` gets two semicolons. `yield` at the end of a line gives `undefined`. After the `)` of a `do`-`while`, a semicolon is inserted before `console`:

```javascript run id=inserted
function sum(a, b) {
  return
    a + b;
}
let a = 1, b = 2, c = 3
a = b
++c
console.log(sum(1, 2), a, b, c)
console.log(eval("{ 1\n2 } 3"))

function* numbers() {
  yield
    42
}
console.log([...numbers()])

let i = 0
do i++; while (i < 3) console.log("i is", i)
```

```output for=inserted
undefined 2 2 4
3
[ undefined ]
i is 3
```

The trap of rule r5 with `(`. The second line fits after the string, so no semicolon is inserted, and the string is called as a function:

```javascript fails id=call-trap expect=exception:TypeError
const greeting = "hello"
(function () {
  console.log("never runs")
})()
```

Node.js reports `TypeError: "hello" is not a function`. The same trap with `[`: the line `[1, 2, 3]` becomes an index into `1`, and the comma expression `1, 2, 3` is the key `3`:[^2]

```javascript fails id=index-trap expect=exception:TypeError
const b = 1
[1, 2, 3].forEach((n) => console.log(n))
```

Node.js reports `TypeError: Cannot read properties of undefined (reading 'forEach')`.

Unary `-` and a template literal at the start of a line (rule r5). The `-5` subtracts, and the template becomes a tagged template of `String`:

```javascript run id=joined
const total = 10
-5
console.log(total)

const label = String
`text`
console.log(typeof label, label)
```

```output for=joined
5
string text
```

The fix of rule r6 for code without semicolons: a semicolon at the start of the line that begins with `(` or `[`:

```javascript run id=leading-semicolon
const greeting = "hello"
;(function () {
  console.log(greeting)
})()
;[1, 2].forEach((n) => console.log(n))
```

```output for=leading-semicolon
hello
1
2
```

No empty statement is inserted (rule r4), so the declaration becomes the body of the `if`, and a declaration is not allowed there:

```javascript fails id=no-empty-statement expect=exception:SyntaxError
if (Math.random() > 0.5)
  const x = 1
```

Node.js reports `SyntaxError: Unexpected token 'const'`.[^2]

## Pitfalls

### A line that starts with `(` or `[`

This is the main trap of code without semicolons ([`call-trap`](#call-trap), [`index-trap`](#index-trap)). An immediately invoked function or an array destructuring at the start of a line joins the line before. End the line before with a semicolon, or start the line with one.[^1] [^2]

### A value on the line after `return`

`return` followed by a line break returns `undefined` (rule r3), even when the next line holds an object literal. Start the value on the line of `return`, for example with `return (` and the value inside the parentheses.[^2]

### A class field before a generator method

In a class body, `a = 1` followed by a line with `*gen() {}` reads as `a = 1 * gen() {}`, which is a syntax error. End class fields with semicolons.[^2]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| end of a statement | a semicolon, written or inserted[^1] | the end of the logical line, a NEWLINE token[^4] |
| semicolon | ends a statement | separates simple statements on one line[^5] |
| line break inside an expression | allowed; the statement goes on if the next token fits[^1] | only inside brackets, or after a backslash[^4] |

In Python, "Several simple statements may occur on a single line separated by semicolons."[^5] A Python line break ends a statement by default, and a JavaScript line break ends one only when the next token does not fit. See [[python/line-structure]].

## References
