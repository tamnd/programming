---
tag: MZN4YR
kind: feature
owner: javascript
slug: explicit-type-conversion
name: Explicit type conversion
aliases: [type conversion, typecasting, "Number()", "String()", "Boolean()", "parseInt()", "parseFloat()", unary plus, double NOT, number coercion, string coercion, boolean coercion]
area: languages/javascript/values-and-types
level: intro
summary: >-
  Number(x), String(x) and Boolean(x) convert a value on request. They use the steps of implicit coercion, with small changes for BigInts and symbols. parseInt reads a number prefix.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-type-conversion", locator: "7.1 Type Conversion"}
see_also: [javascript/type-coercion, javascript/numbers, javascript/booleans-and-truthiness, javascript/nan-and-infinity, javascript/primitive-wrapper-objects, javascript/symbols, type-conversion, python/type-conversions]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Type_conversion
    title: Type conversion, MDN Glossary
    locator: definition
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number
    title: Number, MDN
    locator: Number coercion
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String
    title: String, MDN
    locator: String coercion
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Boolean
    title: Boolean, MDN
    locator: Boolean primitives and Boolean objects; Boolean coercion
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-type-conversion
    title: ECMAScript 2026 Language Specification
    locator: 7.1 Type Conversion
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/parseInt
    title: parseInt(), MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://docs.python.org/3/library/functions.html
    title: Built-in Functions, The Python Standard Library
    locator: bool; float; int
    accessed: 2026-11-18
---

## Synopsis

| To | Function | Short form | Notes |
|---|---|---|---|
| number | `Number(x)` | `+x` | `+x` throws for a BigInt; `Number(x)` does not |
| string | `String(x)` | `` `${x}` `` | `` `${x}` `` throws for a symbol; `String(x)` does not |
| Boolean | `Boolean(x)` | `!!x` | never calls methods on objects |
| integer from text | `parseInt(s, radix)` | | stops at the first bad character |
| number from text | `parseFloat(s)` | | stops at the first bad character |

MDN says that "Type conversion (or typecasting) means transfer of data from one data type to another." In `Number("0x11")`, "the string "0x11" is explicitly converted to the number 17".[^1]

```javascript run id=synopsis
console.log(Number("0x11"), Number(" 42 "), Number(""), Number("4px"), Number(null), Number(undefined));
console.log(String(12.5), String(null), String([1, 2]), String(Symbol("s")));
console.log(Boolean(""), Boolean("0"), Boolean(0n), Boolean({}), !!NaN);
console.log(parseInt("4px", 10), parseFloat("3.5e2 kg"), parseInt("ff", 16));
```

```output for=synopsis
17 42 0 NaN 0 NaN
12.5 null 1,2 Symbol(s)
false true false true false
4 350 255
```

## Semantics

**r1. The same steps as coercion.** The explicit forms run the steps of number, string and Boolean coercion, as listed on the MDN Number, String and Boolean pages.[^2][^3][^4] The specification defines these steps in 7.1.[^5]

**r2. Number(x).** "undefined turns into NaN. null turns into 0." Strings "are converted by parsing them as if they contain a number literal. Parsing failure results in NaN."[^2] "Empty or whitespace-only strings are converted to 0." "Numeric separators are not allowed."[^2] Example: [`synopsis`](#synopsis).

**r3. Number(x) and +x.** "+x does exactly the number coercion steps", and Number(x) "uses the same algorithm to convert x, except that BigInts don't throw a TypeError".[^2] Example: [`bigint`](#bigint).

**r4. String(x).** Numbers and BigInts use the steps of `toString(10)`. Objects use the hint `'string'`, so `toString()` runs before `valueOf()`.[^3] String(x) is like `` `${x}` ``, "except that Symbols don't throw a TypeError".[^3]

**r5. Boolean(x).** Only `undefined`, `null`, `0`, `-0`, `NaN`, `0n` and `''` give false. "All objects become true."[^4] "boolean coercion does not attempt to convert objects to primitives by calling user methods."[^4] See [[javascript/booleans-and-truthiness]].

**r6. parseInt and parseFloat read a prefix.** "If parseInt encounters a character in the input string that is not a valid numeral in the specified radix, it ignores it and all succeeding characters and returns the integer value parsed up to that point."[^6] "If the first character cannot be converted to a number with the radix in use, parseInt returns NaN."[^6] Example: [`synopsis`](#synopsis).

## Examples

The examples run with Node.js 24.

`Number()` accepts a BigInt, and unary `+` does not (rule r3):

```javascript fails id=bigint expect=exception:TypeError
console.log(Number(2n ** 64n));
console.log(+10n);
```

Strings that look like numbers, and strings that do not (rule r2):

```javascript run id=form
const fields = ["12", " 7 ", "", "1_000", "abc"];
for (const f of fields) {
  const n = Number(f);
  console.log(JSON.stringify(f), n, Number.isNaN(n));
}
```

```output for=form
"12" 12 false
" 7 " 7 false
"" 0 false
"1_000" NaN true
"abc" NaN true
```

## Pitfalls

### The empty string is 0

`Number('')` is `0`, not `NaN` (rule r2). Test for an empty string first when it means no value.

### Boolean('false') is true

Each non-empty string gives true (rule r5).

### `new Boolean(false)` is truthy

`Boolean(x)` without `new` gives a primitive. MDN says: "Do not use the Boolean() constructor with new." All objects, "including a Boolean object whose wrapped value is false, are truthy".[^4] See [[javascript/primitive-wrapper-objects]].

### parseInt is not Number

`parseInt('4px', 10)` is `4`, but `Number('4px')` is `NaN` (rule r6).

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| to number | `Number(x)`, `parseInt(s, radix)`[^2][^6] | `int(x)`, `float(x)`[^7] |
| underscores in text | `Number('1_000')` is `NaN`[^2] | `int(' -12_345 \n ')` is `-12345`[^7] |
| to Boolean | `Boolean(x)`[^4] | `bool(x)` uses "the standard truth testing procedure"[^7] |

Python `int` returns "an integer object constructed from a number or a string".[^7] See [[python/type-conversions]] and [[type-conversion]].

## References
