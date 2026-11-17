---
tag: 697SYE
kind: feature
owner: javascript
slug: the-javascript-grammar
name: The JavaScript grammar
aliases: [ECMAScript grammar, syntactic grammar, lexical grammar, goal symbol, cover grammar, early errors, grammar parameters]
area: languages/javascript/syntax
level: research
summary: >-
  ECMA-262 defines JavaScript with a lexical grammar, a syntactic grammar with the goals Script and Module, grammar parameters, lookahead limits, cover grammars and early error rules.
grammar: "Script : ScriptBody opt; Module : ModuleBody opt (ECMA-262, A.5)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/notational-conventions.html#sec-syntactic-and-lexical-grammars", locator: "5.1 Syntactic and Lexical Grammars; Annex A Grammar Summary"}
see_also: [javascript/lexical-grammar, javascript/automatic-semicolon-insertion, javascript/expression-and-empty-statements, javascript/strict-mode, javascript/operator-precedence, javascript/statements-and-declarations, python/the-python-grammar]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html
    title: ECMAScript 2026 Language Specification
    locator: "12 ECMAScript Language: Lexical Grammar, introduction"
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/notational-conventions.html#sec-syntactic-and-lexical-grammars
    title: ECMAScript 2026 Language Specification
    locator: 5.1.1 to 5.1.5 Grammars and grammar notation; 5.2.5 Static Semantics
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/grammar-summary.html
    title: ECMAScript 2026 Language Specification, Annex A
    locator: A.5 Scripts and Modules
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/error-handling-and-language-extensions.html
    title: ECMAScript 2026 Language Specification
    locator: 17 Error Handling and Language Extensions
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/grammar.html
    title: Full Grammar specification, The Python Language Reference
    locator: 10 Full Grammar specification
    accessed: 2026-11-18
---

## Synopsis

```javascript run id=synopsis
const hi = 4, g = 2, b = 16;
const a = b
/hi/g;
console.log(a);
const re = /hi/g;
console.log(re.test("hi"));
```

```output for=synopsis
2
true
```

The same characters `/hi/g` are a division in the first statement and a regular expression literal in the second. The grammar context decides which one the lexer produces. ECMA-262 says that the identification of input elements "is sensitive to the syntactic grammar context that is consuming the input elements. This requires multiple goal symbols for the lexical grammar."[^1]

## Semantics

**r1. Grammars and notation.** A context-free grammar "consists of a number of productions". Each production has a nonterminal on its left-hand side. From a goal symbol, the grammar defines a set of terminal sequences.[^2] ECMA-262 marks the grammar of a production by its colons. The lexical and RegExp grammars use `::`, the numeric string grammar uses `:::`, and the syntactic grammar uses one `:`.[^2]

**r2. Two levels.** The lexical grammar has Unicode code points as terminals and turns the source into input elements. "Input elements other than white space and comments form the terminal symbols for the syntactic grammar for ECMAScript and are called ECMAScript tokens ."[^2] Line terminators are not tokens, but they stay in the stream and "guide the process of automatic semicolon insertion".[^2]

**r3. Goal symbols.** The syntactic grammar starts from "two alternative goal symbols Script and Module".[^2] Annex A gives `Script : ScriptBody opt` and `Module : ModuleBody opt`.[^3] The lexical grammar has five goals: InputElementDiv, InputElementTemplateTail, InputElementRegExp, InputElementRegExpOrTemplateTail and InputElementHashbangOrRegExp.[^2] "In all other contexts, InputElementDiv is used as the lexical goal symbol ."[^1] Example: [`synopsis`](#synopsis).

**r4. Parameters.** A production can have parameters such as `[Yield, Await, Return]`. A parameterized production "is shorthand for a set of productions defining all combinations of the parameter names".[^2] `+In` sets a parameter, `~In` clears it and `?In` passes it on from the left-hand side.[^2] For example, `ScriptBody : StatementList [~Yield, ~Await, ~Return]` clears the Yield, Await and Return parameters for the statements of a script.[^3] Example: [`params`](#params).

**r5. Lookahead and line terminators.** "[lookahead ∉ set]" and the other lookahead forms limit a production by the next tokens.[^2] "[no LineTerminator here]" makes a restricted production: "it may not be used if a LineTerminator occurs in the input stream at the indicated position."[^2] Example: [`restricted`](#restricted).

**r6. Cover grammars.** "In certain cases, in order to avoid ambiguities, the syntactic grammar uses generalized productions that permit token sequences that do not form a valid ECMAScript Script or Module ." This is used "for object literals and object destructuring patterns".[^2] The tokens matched by the general production are "parsed again using N as the goal symbol".[^2] Example: [`cover`](#cover).

**r7. Static semantics and early errors.** "Context-free grammars are not sufficiently powerful to express all the rules" for a valid Script or Module. More rules are the static semantics, and "A special kind of static semantic rule is an Early Error Rule ."[^2] "An early error is an error that can be detected and reported prior to the evaluation of any construct in the Script containing the error."[^4] Example: [`early`](#early).

**r8. Not a complete account alone.** The syntactic grammar in clauses 13 to 16 "is not a complete account of which token sequences are accepted". Automatic semicolon insertion accepts more sequences, and line terminators in "awkward" places reject some.[^2]

## Examples

The examples run with Node.js 24.

Parameters change what a name means (rule r4). In a sloppy script, `yield` and `await` are identifiers. In a generator body, `yield` is an operator:

```javascript run id=params
var yield = 1;
var await = 2;
function* gen() {
  const got = yield yield + 1;
  return got;
}
const it = gen();
console.log(yield, await, it.next().value, it.next(10).value);
```

```output for=params
1 2 1 10
```

Inside `gen`, `yield + 1` is a yield of `+1`, so the first value is 1. The outer `yield` gets the 10 that the second `next` call sends.

A restricted production (rule r5). `ReturnStatement` has `[no LineTerminator here]` after `return`, so a line break ends the statement:

```javascript run id=restricted
function f() {
  return
  42;
}
function g() {
  return (
    42
  );
}
console.log(f(), g());
```

```output for=restricted
undefined 42
```

A cover grammar (rule r6). The same tokens `{ a, b }` are first an object literal, and then, before `=`, they are parsed again as an assignment pattern:

```javascript run id=cover
let a, b;
({ a, b } = { a: 1, b: 2 });
console.log(a, b);
const sum = (x, y) => x + y;
const pair = (a, b);
console.log(sum(3, 4), pair);
```

```output for=cover
1 2
7 2
```

`(x, y)` before `=>` is a parameter list, and `(a, b)` without `=>` is a comma expression. Both start as the same cover production, CoverParenthesizedExpressionAndArrowParameterList.[^3]

An early error stops the whole script (rule r7):

```javascript fails id=early expect=exception:SyntaxError
console.log("this line never runs");
let x = 1;
let x = 2;
```

The script prints nothing. "The presence of an early error prevents the evaluation of the construct."[^4] An early error in `eval` code is "reported at the time eval is called", so a program can catch it:[^4]

```javascript run id=early-eval
try {
  eval("console.log('never'); let y = 1; let y = 2;");
} catch (err) {
  console.log(err.name);
}
```

```output for=early-eval
SyntaxError
```

## Pitfalls

### A line that starts with `/`, `(`, `[` or a template

The goal symbol of the lexer comes from the syntactic context, and a line break does not end a statement unless the grammar needs it ([`synopsis`](#synopsis)). The spec example `a = b` on one line and `/hi/g.exec(c).map(d);` on the next "is interpreted in the same way as" a division over the two lines.[^1]

### Errors that are not early

Only the conditions in the "Static Semantics: Early Errors" subclauses are early errors. "An implementation shall not treat other kinds of errors as early errors even if the compiler can prove that a construct cannot execute without error under any circumstances."[^4] Also, "All errors that are not early errors are runtime errors."[^4]

### The grammar summary is not the full rule set

Annex A lists productions, but the early error rules and the cover grammar checks are in the clause text (rules r6 and r7). A parser built from Annex A alone accepts some invalid programs.

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| grammar kind | context-free productions, with lookahead and parameters[^2] | a "PEG grammar"[^5] |
| source of the listing | ECMA-262 clauses 12 to 16, Annex A[^2][^3] | "derived directly from the grammar used to generate the CPython parser"[^5] |
| extra rules | static semantics and early errors[^2] | rules named `invalid_` give "specialized syntax errors"[^5] |
| goals | Script and Module[^2] | the starting rules `file`, `interactive`, `eval` and `func_type`[^5] |

The Python grammar says that `invalid_` rules "are NOT used in the first pass of the parser" and run "Only if the first pass fails to parse".[^5] In ECMA-262, early errors are part of what makes a program valid (rule r7). See [[python/the-python-grammar]].

## References
