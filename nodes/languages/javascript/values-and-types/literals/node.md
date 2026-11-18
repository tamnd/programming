---
tag: XQBBKB
kind: feature
owner: javascript
slug: literals
name: Literals
aliases: [literal, array literal, object literal, numeric literal, string literal, regex literal, BigInt literal, boolean literal, null literal]
area: languages/javascript/values-and-types
level: intro
summary: >-
  Literals are values that you write directly in the source: null, true, 42, 0xff, 10n, 'text', [1, 2], {a: 1} and /ab+c/. Array and object literals make a fresh object each time they run.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-primary-expression-literals", locator: "13.2.3 Literals"}
see_also: [javascript/numbers, javascript/strings, javascript/template-literals, javascript/arrays, javascript/sparse-arrays, javascript/object-initializers, javascript/trailing-commas, python/numbers]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Grammar_and_types
    title: Grammar and types, MDN
    locator: Literals
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-primary-expression-literals
    title: ECMAScript 2026 Language Specification
    locator: 13.2.3 Literals; 13.2.4 Array Initializer; 13.2.5 Object Initializer; 13.2.7 Regular Expression Literals; 13.2.8 Template Literals
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/lexical_analysis.html
    title: Lexical analysis, The Python Language Reference
    locator: 2.4 Literals; 2.6 Numeric literals
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.2.5.1 List displays; 6.2.5.4 Dictionary displays
    accessed: 2026-11-18
---

## Synopsis

| Kind | Examples |
|---|---|
| null | `null` |
| Boolean | `true`, `false` |
| number | `117`, `0o17`, `0xff`, `0b101`, `3.14`, `.5`, `1e-3` |
| BigInt | `123n`, `0x1fn` |
| string | `'single'`, `"double"`, `` `template ${x}` `` |
| array | `[]`, `[1, 2, 3]`, `[1, , 3]` |
| object | `{}`, `{ a: 1, 'b-c': 2 }` |
| regular expression | `/ab+c/g` |

MDN describes literals as fixed values that you write in the script, not variables.[^1] In the grammar, the `Literal` production covers only `NullLiteral`, `BooleanLiteral`, `NumericLiteral` and `StringLiteral`.[^2] Array, object, regular expression and template literals have their own sections in 13.2.[^2]

```javascript run id=synopsis
console.log(typeof null, typeof true, typeof 0xff, typeof 10n, typeof "a");
console.log(Array.isArray([1, 2]), typeof { a: 1 }, /ab+c/ instanceof RegExp);
console.log(0o17, 0xff, 0b101, 1e-3, 0x1fn);
```

```output for=synopsis
object boolean number bigint string
true object true
15 255 5 0.001 31n
```

## Semantics

**r1. Number literals.** A literal can be decimal, octal (`0o`), hexadecimal (`0x`) or binary (`0b`). A floating-point literal is decimal and "must have at least one digit, and either a decimal point or e (or E)".[^1] Example: [`synopsis`](#synopsis).

**r2. No sign in a number literal.** MDN notes that "the language specification requires numeric literals to be unsigned". In `-123.4`, the `-` is a unary operator.[^1]

**r3. BigInt literals.** "A trailing n suffix on an integer literal indicates a BigInt literal." Any base is OK, but "leading-zero octal syntax like 0123n is not allowed".[^1] Example: [`bigint-octal`](#bigint-octal).

**r4. String literals.** A string literal uses double or single quotes. "A string must be delimited by quotation marks of the same type".[^1] Template literals use backticks. See [[javascript/template-literals]].

**r5. Array literals.** "An array literal creates a new array object every time the literal is evaluated."[^1] "If you put two commas in a row in an array literal, the array leaves an empty slot for the unspecified element." A trailing comma is ignored.[^1] Example: [`fresh`](#fresh).

**r6. Object literals.** An object literal holds pairs of property names and values in braces. A name that is not a valid identifier or number "must be enclosed in quotes".[^1] Example: [`fresh`](#fresh).

**r7. Regular expression literals.** A regular expression literal is a pattern between slashes, such as `/ab+c/`.[^1]

## Examples

The examples run with Node.js 24.

Each evaluation makes a fresh array and object (rules r5 and r6):

```javascript run id=fresh
function make() {
  return { list: [1, , 3,], "odd-key": true };
}
const a = make();
const b = make();
console.log(a === b, a.list === b.list);
console.log(a.list, a.list.length, 1 in a.list, a["odd-key"]);
```

```output for=fresh
false false
[ 1, <1 empty item>, 3 ] 3 false true
```

The unary minus is not part of the literal (rule r2):

```javascript run id=sign
const x = -0xff;
console.log(x, -(0xff) === x, - 1 === -1);
```

```output for=sign
-255 true true
```

A BigInt literal cannot use a leading zero for octal (rule r3):

```javascript fails id=bigint-octal expect=compile:SyntaxError
console.log(0o123n);
console.log(0123n);
```

## Pitfalls

### An object literal at the start of a statement

MDN says "Do not use an object literal at the beginning of a statement!" There, "the { will be interpreted as the beginning of a block."[^1] Wrap the literal in parentheses, as in `({ a: 1, b: 2 })`.

```javascript fails id=block expect=compile:SyntaxError
{ a: 1, b: 2 }
```

### Holes are not undefined

`[1, , 3]` has an empty slot at index 1. "When using array-traversing methods like Array.prototype.map, empty slots are skipped."[^1] See [[javascript/sparse-arrays]].

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| kinds | null, Boolean, number, string; arrays, objects and regular expressions have literal syntax too[^2] | "string, bytes and numeric literals"; `None`, `True` and `False` are keywords[^3] |
| sign | not part of the literal[^1] | "Numeric literals do not include a sign"[^3] |
| list syntax | array literal `[1, 2]`[^1] | "a possibly empty series of expressions enclosed in square brackets"[^4] |
| map syntax | object literal `{ a: 1 }`[^1] | dictionary display `{1: 'one'}`[^4] |
| complex numbers | none | "no complex literals", but imaginary literals such as `4.2j`[^3] |

Python says "Literals are notations for constant values of some built-in types."[^3] Python calls `[1, 2]` a list display, not a literal.[^4] See [[python/numbers]].

## References
