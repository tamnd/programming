---
tag: TD6Y3G
kind: feature
owner: javascript
slug: trailing-commas
name: Trailing commas
aliases: [final commas, trailing comma, elision, dangling comma]
area: languages/javascript/syntax
level: intro
summary: >-
  JavaScript accepts a comma after the last item of array and object literals, parameter lists, calls, named imports and exports and destructuring. JSON does not.
grammar: "ArrayLiteral : [ ElementList , Elision opt ]; ObjectLiteral : { PropertyDefinitionList , }; Arguments : ( ArgumentList , ) (ECMA-262, 13.2 and 13.3)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#prod-ArrayLiteral", locator: "13.2.4 Array Initializer; 13.2.5 Object Initializer; 13.3.8 Argument Lists"}
see_also: [javascript/comma-operator, javascript/spread-syntax, javascript/sparse-arrays, javascript/arrays, python/one-element-tuple, python/tuples-and-sequences]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Trailing_commas
    title: Trailing commas, MDN
    locator: introduction; Description; Examples
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#prod-ArrayLiteral
    title: ECMAScript 2026 Language Specification
    locator: 13.2.4 Array Initializer; 13.2.5 Object Initializer; 13.3.8 Argument Lists
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.2.5 Container displays; 6.2.5.3 Tuple displays; 6.3.3 Calls; 6.15 Expression lists
    accessed: 2026-11-18
---

## Synopsis

```javascript run id=synopsis
const arr = [
  1,
  2,
  3,
];
const object = {
  foo: "bar",
  age: 42,
};
function f(p,) {
  return p;
}
console.log(arr.length, Object.keys(object), f(7,));
```

```output for=synopsis
3 [ 'foo', 'age' ] 7
```

MDN: "JavaScript allows trailing commas wherever a comma-separated list of values is accepted and more values may be expected after the last item."[^1] A trailing comma lets a programmer "add a new line without modifying the previously last line".[^1]

## Semantics

**r1. Where they are allowed.** MDN lists "Array literals", "Object literals", "Parameter definitions", "Function calls", "Named imports", "Named exports", "Dynamic import" and "Array and object destructuring".[^1]

**r2. No change in meaning.** "In all these cases, the trailing comma is entirely optional and doesn't change the program's semantics in any way."[^1] "Trailing commas don't affect the length property of function declarations or their arguments object."[^1] Example: [`same`](#same).

**r3. Arrays: one comma is ignored, more make holes.** "JavaScript ignores trailing commas in array literals". "If more than one trailing comma is used, an elision (or hole) is produced."[^1] The grammar has an optional Elision after the last element.[^2] Example: [`holes`](#holes).

**r4. Not after a rest element.** "when using rest parameters , trailing commas are not allowed". The same is true for a rest element in destructuring.[^1] Example: [`rest`](#rest).

**r5. Not alone.** "Function parameter definitions or function invocations only containing a comma will throw a SyntaxError ."[^1] So `f(,)` is an error.

**r6. JSON is stricter.** "JSON , however, disallows all trailing commas."[^1] `JSON.parse` throws a SyntaxError for them. Example: [`json`](#json).

**r7. Arrays were first.** "JavaScript has allowed trailing commas in array literals since the beginning. Trailing commas are now also allowed in object literals, function parameters, named imports, named exports, and more."[^1]

## Examples

The examples run with Node.js 24.

The trailing comma does not change `length` (rule r2):

```javascript run id=same
function one(a) {}
function oneComma(a,) {}
function count() {
  return arguments.length;
}
const methods = { two(a, b,) {} };
console.log(one.length, oneComma.length, count(1, 2,), methods.two.length);
console.log(Math.max(10, 20,));
const [a, b,] = [1, 2];
const { p, q, } = { p: 42, q: true };
console.log(a, b, p, q);
```

```output for=same
1 1 2 2
20
1 2 42 true
```

Holes from extra commas (rule r3):

```javascript run id=holes
const arr = [1, 2, 3, , ,];
console.log(arr.length, 3 in arr, 4 in arr);
const seen = [];
arr.forEach((x) => seen.push(x));
console.log(seen);
```

```output for=holes
5 false false
[ 1, 2, 3 ]
```

The array has length 5, but only three elements. `forEach` skips the holes.[^1]

A comma after a rest parameter (rule r4):

```javascript fails id=rest expect=exception:SyntaxError
function f(...p,) {}
```

Node.js reports `SyntaxError: Rest parameter must be last formal parameter`. A rest element in destructuring has the same rule: `const [a, ...b,] = [1, 2, 3];` is a SyntaxError.[^1]

`JSON.parse` and trailing commas (rule r6):

```javascript run id=json
for (const text of ["[1, 2, 3, 4 ]", "[1, 2, 3, 4, ]", '{"foo" : 1, }']) {
  try {
    console.log(JSON.parse(text));
  } catch (err) {
    console.log(err.name);
  }
}
```

```output for=json
[ 1, 2, 3, 4 ]
SyntaxError
SyntaxError
```

## Pitfalls

### Two commas at the end of an array

`[1, 2, 3, , ,]` has length 5, not 4 ([`holes`](#holes)). MDN: "Sparse arrays are generally unfavorable, so you should avoid having multiple trailing commas."[^1]

### Commas in a regular expression quantifier

A comma in a regular expression quantifier is not a trailing comma, and it changes the meaning. MDN: `/x{2}/` matches "Exactly 2 occurrences" of `x`, and `/x{2,}/` matches "At least 2 occurrences".[^1]

### JSON files

A configuration file with a trailing comma is not valid JSON. Remove the comma before `JSON.parse` ([`json`](#json)).

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| list or array literal | `[1, 2,]`; one comma is ignored[^1] | `[1, 2,]`; the comma "has no effect"[^3] |
| call arguments | `f(a, b,)`[^1] | allowed, "does not affect the semantics"[^3] |
| one item | `[1,]` is an array of one item | `1,` is a tuple; "it requires a trailing comma"[^3] |

In Python, the comma can change the type. "A trailing comma is required only to create a one-item tuple, such as 1, ; it is optional in all other cases."[^3] In JavaScript, a trailing comma never changes the type of a value. See [[python/one-element-tuple]].

## References
