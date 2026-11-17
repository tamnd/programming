---
tag: VJVK98
kind: feature
owner: javascript
slug: notational-conventions-of-ecma-262
name: Notational conventions of ECMA-262
aliases: [ECMA-262 notation, spec notation, grammar notation, algorithm steps, abstract operations, "? and ! shorthands", ReturnIfAbrupt]
area: languages/javascript/overview
level: research
summary: >-
  Clause 5 of ECMA-262 defines how the spec is written: grammars with :, :: and :::, parameters and lookahead, numbered algorithm steps, abstract operations, and the ? and ! shorthands.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/notational-conventions.html#sec-notational-conventions", locator: "5 Notational Conventions (5.1 Syntactic and Lexical Grammars; 5.2 Algorithm Conventions)"}
see_also: [javascript, javascript/javascript-and-the-ecmascript-specification, javascript/language-and-host, context-free-grammars, syntax, tokens-and-lexing, order-of-evaluation, program-semantics]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/notational-conventions.html#sec-notational-conventions
    title: ECMAScript 2026 Language Specification
    locator: 5.1.1 Context-Free Grammars; 5.1.2 to 5.1.4; 5.1.5 Grammar Notation
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/notational-conventions.html#sec-algorithm-conventions
    title: ECMAScript 2026 Language Specification
    locator: 5.2 Algorithm Conventions; 5.2.1 Evaluation Order; 5.2.2 Abstract Operations; 5.2.3 Syntax-Directed Operations
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-applystringornumericbinaryoperator
    title: ECMAScript 2026 Language Specification
    locator: 13.15.3 ApplyStringOrNumericBinaryOperator
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-samevaluezero
    title: ECMAScript 2026 Language Specification
    locator: 7.1.1 ToPrimitive; 7.2.10 SameValueZero
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/notational-conventions.html#sec-shorthands-for-unwrapping-completion-records
    title: ECMAScript 2026 Language Specification
    locator: 5.2.4 Runtime Semantics; 5.2.4.3 Shorthands for Unwrapping Completion Records; 5.2.5 Static Semantics; 5.2.6 Mathematical Operations; 5.2.7 Value Notation
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/indexed-collections.html#sec-array.prototype.includes
    title: ECMAScript 2026 Language Specification
    locator: 23.1.3.16 Array.prototype.includes
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html#sec-return-statement
    title: ECMAScript 2026 Language Specification
    locator: 14.3.1.1 Static Semantics, Early Errors; 14.10 The return Statement
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html#sec-rules-of-automatic-semicolon-insertion
    title: ECMAScript 2026 Language Specification
    locator: 12.10.1 Rules of Automatic Semicolon Insertion
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/introduction.html#notation
    title: The Python Language Reference, 1. Introduction
    locator: introduction; 1.2 Notation
    accessed: 2026-11-18
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-2.html#jls-2.4
    title: The Java Language Specification, chapter 2
    locator: 2.4 Grammar Notation
    accessed: 2026-11-18
---

## Synopsis

Clause 5 of ECMA-262 says how the rest of the spec is written. It has two parts:

- **5.1 grammars.** The productions of the lexical, syntactic and numeric string grammars, and the shorthands in them: optional symbols, parameters, lookahead and `[no LineTerminator here]`.
- **5.2 algorithms.** Numbered steps, abstract operations, syntax-directed operations, completion records with the `?` and `!` shorthands, static semantics, and the kinds of numbers.

| Notation | Meaning | Where |
|---|---|---|
| `A : …` | a production of the syntactic grammar | 5.1.4 |
| `A :: …` | a production of the lexical or RegExp grammar | 5.1.2 |
| `A ::: …` | a production of the numeric string grammar | 5.1.3 |
| X<sub>opt</sub> | X may be left out | 5.1.5.3 |
| A<sub>[Yield, Await]</sub>, `[+In]`, `[~In]`, `[?In]` | a grammatical parameter | 5.1.5.4 |
| `[lookahead ∉ set]` | a restriction on the next tokens | 5.1.5.7 |
| `[no LineTerminator here]` | a restricted production | 5.1.5.8 |
| `Let x be ? Op().` | unwrap a completion, and return it if it is abrupt | 5.2.4.3 |
| `Let x be ! Op().` | unwrap a completion that must be normal | 5.2.4.3 |
| 1<sub>𝔽</sub>, 1<sub>ℤ</sub> | a Number, a BigInt; no subscript is a mathematical value | 5.2.6 |

## Semantics

**r1.** The grammars are [[context-free-grammars]]. "A context-free grammar consists of a number of productions. Each production has an abstract symbol called a nonterminal as its left-hand side, and a sequence of zero or more nonterminal and terminal symbols as its right-hand side."[^1] Starting from a goal symbol, the grammar gives the set of sequences of terminal symbols that it can produce.[^1]

**r2.** The number of colons tells the grammars apart. "Productions of the syntactic grammar are distinguished by having just one colon “:” as punctuation." The lexical and RegExp grammars use two colons, and the numeric string grammar three, which are "never used for parsing source text".[^1]

**r3.** Some symbols are shorthands for several productions. The subscript "opt" "indicates an optional symbol", and an alternative with it "actually specifies two right-hand sides".[^1] A grammatical parameter such as `[Return]` "is shorthand for a set of productions defining all combinations of the parameter names". On the right-hand side, `[+In]` sets the parameter, `[~In]` leaves it out, and `[?In]` passes on the value of the left-hand side.[^1]

**r4.** Some phrases restrict a production. `[lookahead ∉ set]` "indicates that the production may only be used if no element of set is a prefix of the immediately following token sequence".[^1] `[no LineTerminator here]` makes the production a restricted production, which "may not be used if a LineTerminator occurs in the input stream at the indicated position".[^1] Example: [`no-line-terminator`](#no-line-terminator).

**r5.** The semantics are numbered steps. "The algorithms are not intended to imply the use of any specific implementation technique."[^2] Substeps are labelled with letters, then with roman numerals. A step that begins with `Assert:` states an invariant, and assertions "need not be checked by an implementation".[^2]

**r6.** An expression in a step is evaluated "in a left-to-right, inside-to-outside order" ([[order-of-evaluation]]).[^2] This order is visible to programs. ApplyStringOrNumericBinaryOperator, which `+` uses, has the steps "Let lPrim be ? ToPrimitive(lVal)." and then "Let rPrim be ? ToPrimitive(rVal).", so the left operand is converted first.[^3] Example: [`eval-order`](#eval-order).

**r7.** Named algorithms are abstract operations, "named and written in parameterized functional form so that they may be referenced by name from within other algorithms", like `OperationName(arg1, arg2)`.[^2] SameValueZero (7.2.10) is one: it has three steps, and the first is "If SameType(x, y) is false, return false."[^4] A syntax-directed operation instead has one algorithm per production, and its steps can name the symbols of the production "as if they were parameters of the algorithm".[^2] Example: [`same-value-zero`](#same-value-zero).

**r8.** An operation that can fail returns a Completion Record: normal, or abrupt, such as a throw. "Prefix ? and ! are used as shorthands which unwrap Completion Records. ? is used to propagate an abrupt completion to the caller, or otherwise to unwrap a normal completion. ! is used to assert that a Completion Record is normal and unwrap it."[^5] "Throw a TypeError exception." means "Return ThrowCompletion(a newly created TypeError object)."[^5] In JavaScript terms, `?` is the place where an exception from a callee goes on up. Both marks appear in one step of `Array.prototype.includes`: "Let elementK be ? Get(O, ! ToString(𝔽(k)))." The Get can call a getter that throws, so it has `?`; the conversion of an index to a String cannot fail, so it has `!`.[^6] Example: [`abrupt`](#abrupt).

**r9.** Static semantics are rules that the grammar cannot express, such as early errors. "A conforming implementation must, prior to the first evaluation of a Script or Module, validate all of the early error rules of the productions used to parse that Script or Module."[^5] For `let`, 14.3.1.1 says: "It is a Syntax Error if the BoundNames of BindingList contains any duplicate entries."[^7] Example: [`early-error`](#early-error).

**r10.** Numbers in the spec have kinds. "The subscript 𝔽 refers to Numbers, and the subscript ℤ refers to BigInts. Numeric values without a subscript suffix refer to mathematical values."[^5]

## Examples

A restricted production (rule r4). 14.10 has the production `return [no LineTerminator here] Expression ;`.[^7] So a line break after `return` ends the statement, and the rules of automatic semicolon insertion put a semicolon "before the restricted token".[^8] The function returns `undefined`, and `42;` is a statement that never runs.

```javascript run id=no-line-terminator
function answer() {
  return
    42;
}
console.log(answer());
```

```output for=no-line-terminator
undefined
```

The order of the steps (rule r6). The left operand of `+` goes through ToPrimitive before the right one, so `valueOf` of `a` runs first.

```javascript run id=eval-order
const log = [];
const a = { valueOf() { log.push("a"); return 1; } };
const b = { valueOf() { log.push("b"); return 2; } };
console.log(a + b, log);
```

```output for=eval-order
3 [ 'a', 'b' ]
```

An abstract operation written as JavaScript (rule r7). Each comment is one step of 7.2.10. The helper `typeOf` stands for the language type of the spec, which `typeof` gives except for `null` and for functions. The last column checks the result against `Array.prototype.includes`, which compares "using the SameValueZero algorithm".[^6]

```javascript run id=same-value-zero
// The ECMAScript language type of a value, as a lower-case name.
const typeOf = (v) =>
  v === null ? "null" : typeof v === "function" ? "object" : typeof v;

function sameValueZero(x, y) {
  // 1. If SameType(x, y) is false, return false.
  if (typeOf(x) !== typeOf(y)) return false;
  // 2. If x is a Number, then return Number::sameValueZero(x, y).
  if (typeof x === "number") {
    if (Number.isNaN(x) && Number.isNaN(y)) return true;
    return x === y; // +0 and -0 are equal here
  }
  // 3. Return SameValueNonNumber(x, y).
  return x === y;
}

for (const [x, y] of [[NaN, NaN], [0, -0], ["1", 1], [null, undefined]]) {
  console.log(sameValueZero(x, y), [x].includes(y));
}
```

```output for=same-value-zero
true true
true true
false false
false false
```

The `?` shorthand (rule r8). In `Let lPrim be ? ToPrimitive(lVal).`, the call to `valueOf` returns a throw completion, and `?` returns it to the caller. The `+` stops, and the `RangeError` reaches the script.

```javascript fails id=abrupt expect=exception:RangeError
const bad = { valueOf() { throw new RangeError("no number"); } };
const result = bad + 1;
console.log(result);
```

An early error (rule r9). The duplicate name breaks a static rule, so the script is not run at all, and the first line prints nothing.

```javascript fails id=early-error expect=exception:SyntaxError
console.log("never printed");
let a = 1, a = 2;
```

## Pitfalls

### The steps are not an implementation

The spec says so in 5.2 (rule r5). An engine must give the same observable results, but it can reach them another way.

### `?` is not the optional chaining operator

In the algorithms, `?` before a call unwraps a Completion Record (rule r8). In a grammar parameter, `[?In]` passes a parameter on (rule r3). In JavaScript source, `?.` and `? :` are operators. The three uses do not share a meaning.

### Language values and mathematical values differ

A step that says "the length of y" means a mathematical value, an arbitrary real number, while 1<sub>𝔽</sub> is a binary64 Number (rule r10).[^5] In the `includes` step above, 𝔽(k) turns the mathematical index k into a Number.[^6] A JavaScript copy of such a step computes with Numbers, which round.

## Comparison

**Python** gives a grammar but writes the semantics in English. Its reference says: "I chose to use English rather than formal specifications for everything except syntax and lexical analysis." Its grammar notation "is a mixture of EBNF and PEG".[^9] ECMA-262 writes the semantics too as numbered steps (rule r5).

**Java** uses a grammar notation close to that of ECMA-262. In the JLS, "Terminal symbols are shown in fixed width font", "Nonterminal symbols are shown in italic type", and a nonterminal is followed "by a colon".[^10] The JLS writes `{x}` for zero or more occurrences of x.[^10] Clause 5 of ECMA-262 has no such repetition notation, and its productions repeat by recursion, as in `DecimalDigits :: DecimalDigit DecimalDigits DecimalDigit`.[^1]

## References
