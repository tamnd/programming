---
tag: 4D1QN8
kind: feature
owner: javascript
slug: identifiers-and-reserved-words
name: Identifiers and reserved words
aliases: [identifier names, keywords, reserved words, contextual keywords, future reserved words, IdentifierName, ReservedWord]
area: languages/javascript/syntax
level: intro
summary: >-
  An identifier is an IdentifierName (ID_Start or $ or _, then ID_Continue or $) that is not a reserved word. Some words are reserved only in strict code, modules or async functions.
grammar: "IdentifierName :: IdentifierStart | IdentifierName IdentifierPart; IdentifierStartChar :: UnicodeIDStart | $ | _; IdentifierPartChar :: UnicodeIDContinue | $ (ECMA-262, 12.7 Names and Keywords)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html#sec-names-and-keywords", locator: "12.7 Names and Keywords; 12.7.2 Keywords and Reserved Words"}
see_also: [identifier, javascript/lexical-grammar, javascript/white-space-and-line-terminators, javascript/punctuators, python/identifiers-in-python, python/keywords, python/soft-keywords]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html#sec-names-and-keywords
    title: ECMAScript 2026 Language Specification
    locator: 12.7 Names and Keywords; 12.7.1 Identifier Names; 12.7.2 Keywords and Reserved Words
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Lexical_grammar#identifiers
    title: Lexical grammar, MDN
    locator: Identifiers; Keywords; Reserved words; Future reserved words; Identifiers with special meanings
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/lexical_analysis.html#identifiers
    title: Lexical analysis, The Python Language Reference
    locator: 2.3 Names; 2.3.1 Keywords; 2.3.2 Soft Keywords; 2.3.4 Non-ASCII characters in names
    accessed: 2026-11-18
---

## Synopsis

```text
always reserved     break case catch class const continue debugger default delete do
                    else enum export extends false finally for function if import in
                    instanceof new null return super switch this throw true try typeof
                    var void while with
reserved in some    await (modules, async functions)   yield (strict code, generators)
contexts            let static implements interface package private protected public
                    (strict code)
not reserved        as async from get meta of set target; arguments and eval
                    (restricted in strict code)
```

"The syntactic grammar defines Identifier as an IdentifierName that is not a ReservedWord."[^1] An IdentifierName follows the Unicode identifier rules of UAX #31, with `$` and `_` added. A keyword is a word with a use in the grammar, and a reserved word is a keyword that cannot name a variable.[^1]

## Semantics

**r1. Identifier names.** The first code point has the Unicode property ID_Start, or is `$` or `_`. Each later one has ID_Continue, or is `$`.[^1] ID_Continue includes the digits, so a name cannot start with a digit, and it includes U+200C ZWNJ and U+200D ZWJ.[^2] MDN gives the set as the pattern `/[$_\p{ID_Start}][$\p{ID_Continue}]*/u`, not counting escapes, and warns against an ASCII-only pattern.[^2] Names are case-sensitive. Example: [`names`](#names).

**r2. Escapes in names.** A `\uXXXX` or `\u{X}` escape may stand for any code point of a name. It contributes that code point, and the result must still be a valid name.[^1] "Identifiers are always compared by string value, so escape sequences are interpreted." So `你好` and `\u4f60\u597d` are one identifier.[^2] Two names that are canonically equivalent in Unicode are still different unless their code points are the same ([[javascript/lexical-grammar]]).[^1] Example: [`names`](#names).

**r3. Reserved words.** "A reserved word is an IdentifierName that cannot be used as an identifier."[^1] The list holds 38 words, from `await` to `yield`, and all of them except `await` and `yield` are always reserved.[^1] `enum` has no use in the grammar, and ECMA-262 calls it "a future reserved word".[^1] An escape does not hide a reserved word: the name `els\u{65}` is `else`, and it cannot be declared.[^1] [^2] Example: [`escaped-keyword`](#escaped-keyword).

**r4. Reserved in some contexts.** ECMA-262 sorts the identifier names in five categories:[^1]

| Category | Names |
|---|---|
| always allowed, not keywords | such as `Math`, `window`, `toString`, `_` |
| never allowed | the reserved words except `await` and `yield` |
| allowed by context | `await`, `yield` |
| not allowed in strict mode code | `let`, `static`, `implements`, `interface`, `package`, `private`, `protected`, `public` |
| always allowed, but keywords in some productions | `as`, `async`, `from`, `get`, `meta`, `of`, `set`, `target` |

"await is reserved only inside async functions and modules." "async is not reserved; it can be used as a variable name or statement label without restriction."[^1] The words of the last three rows are called "conditional keyword, or contextual keyword".[^1] Example: [`contextual`](#contextual).

**r5. `arguments` and `eval`.** "The names arguments and eval are not keywords, but they are subject to some restrictions in strict mode code."[^1] MDN: they cannot be declared as identifiers in strict mode.[^2] Example: [`contextual`](#contextual).

**r6. Property names.** Property keys and private names are IdentifierNames, not Identifiers, so they may be reserved words: `obj.import`, `{ class: 2 }` and `#import` are valid.[^2] Example: [`names`](#names).

**r7. Old reserved words.** ECMAScript 1 to 3 also reserved `abstract`, `boolean`, `byte`, `char`, `double`, `final`, `float`, `goto`, `int`, `long`, `native`, `short`, `synchronized`, `throws`, `transient` and `volatile`. They are not in the ReservedWord list of ECMAScript 2026, so they are valid names.[^2] [^1]

## Examples

The examples run with Node.js 24. The script is CommonJS, so it is not strict unless it says so.

Names with `$`, `_` and letters beyond ASCII, an escape that spells `π`, and reserved words as property keys and a private name (rules r1, r2 and r6):

```javascript run id=names
const $ = "dollar";
const _ = "underscore";
const π = Math.PI;
const 你好 = "hello";
console.log($, _, π.toFixed(2), 你好, \u{3c0} === π);

const obj = { import: 1, class: 2, if: 3 };
console.log(obj.import + obj.class + obj.if);

class C {
  #import = "a private name";
  get value() { return this.#import; }
}
console.log(new C().value);
```

```output for=names
dollar underscore 3.14 hello true
6
a private name
```

Which words can name a `var`, in sloppy code and in strict code (rules r3 to r5). `new Function` compiles each declaration without running it. The last line shows that a module reserves `await`:

```javascript run id=contextual
const names = ["if", "enum", "let", "static", "yield", "implements", "await", "async", "of", "eval", "arguments"];

function check(source) {
  try {
    new Function(source);
    return "ok";
  } catch (error) {
    return error.name;
  }
}

console.log("name".padEnd(11), "sloppy".padEnd(12), "strict");
for (const name of names) {
  const sloppy = check(`var ${name} = 1;`);
  const strict = check(`"use strict"; var ${name} = 1;`);
  console.log(name.padEnd(11), sloppy.padEnd(12), strict);
}

const { spawnSync } = require("node:child_process");
const result = spawnSync(process.execPath, ["--input-type=module", "-e", "var await = 1;"], { encoding: "utf8" });
console.log("await in a module:", result.stderr.match(/SyntaxError.*/)[0]);
```

```output for=contextual
name        sloppy       strict
if          SyntaxError  SyntaxError
enum        SyntaxError  SyntaxError
let         ok           SyntaxError
static      ok           SyntaxError
yield       ok           SyntaxError
implements  ok           SyntaxError
await       ok           ok
async       ok           ok
of          ok           ok
eval        ok           SyntaxError
arguments   ok           SyntaxError
await in a module: SyntaxError: Unexpected reserved word
```

An escape does not turn a reserved word into a name (rule r3):

```javascript fails id=escaped-keyword expect=exception:SyntaxError
const els\u{65} = 1;
console.log("never runs");
```

Node.js reports `SyntaxError: Keyword must not contain escaped characters`.

## Pitfalls

### `let`, `static` and `yield` as names

Sloppy code accepts `var let` or `var static`, and the same code fails in strict code ([`contextual`](#contextual)). Do not use these words as names.[^1] [^2]

### A name with a ZWJ or a ZWNJ

ZWNJ and ZWJ are valid inside a name and do not show on screen (rule r1).[^2] Two names can then look the same and differ, as in [[javascript/white-space-and-line-terminators]].

### An ASCII-only pattern for names

A tool that finds names with `/[A-Za-z_$][\w$]*/` misses names such as `你好`. MDN gives the Unicode pattern of rule r1 instead.[^2]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| first character | ID_Start, `$`, `_`[^1] | id_start, which includes `_`[^3] |
| normalization | none[^1] | NFKC[^3] |
| `async`, `await` | `async` not reserved; `await` by context[^1] | both are keywords[^3] |
| words reserved by context | `await`, `yield`, the strict-mode words[^1] | the soft keywords `match`, `case`, `_` and `type`[^3] |

Python says of its soft keywords: "Some names are only reserved under specific contexts."[^3] JavaScript has the same idea in its contextual keywords. See [[python/keywords]] and [[python/soft-keywords]].

## References
