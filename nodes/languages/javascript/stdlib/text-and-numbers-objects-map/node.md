---
tag: NEW-javascript-text-and-numbers-objects-map
kind: feature
owner: javascript
slug: text-and-numbers-objects-map
name: Text and numbers objects map
aliases: [Number, BigInt, Date, String, RegExp, numbers and dates, text processing, Number static methods, String static methods, Temporal]
area: languages/javascript/stdlib
level: intro
summary: >-
  Clauses 21 and 22 of ECMA-262 define Number, BigInt, Math and Date for numbers and dates, and String and RegExp for text. This page maps their members and the pages that explain them.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/numbers-and-dates.html", locator: "21 Numbers and Dates (21.1 to 21.4); 22 Text Processing (22.1, 22.2)"}
see_also: [javascript/standard-built-in-objects, javascript/numbers, javascript/bigints, javascript/strings, javascript/regular-expressions, date-and-time-values]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects
  title: MDN Web Docs, JavaScript Reference, Standard built-in objects
  locator: Numbers and dates; Text processing
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/numbers-and-dates.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 21
  locator: 21.1.1 The Number Constructor; 21.2.1 The BigInt Constructor; 21.4.1.1 Time Values and Time Range
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/text-processing.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 22
  locator: 22.1.1 The String Constructor; 22.2.4 The RegExp Constructor; RegExp.escape
  accessed: 2026-11-18
- url: https://raw.githubusercontent.com/tc39/proposals/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Temporal; RegExp.escape
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=map
console.log(Number.MAX_SAFE_INTEGER, Number.EPSILON, Number.isInteger(5.0), (255).toString(16), (1.005).toFixed(2));
console.log(typeof 10n, 2n ** 64n, BigInt.asUintN(8, 257n), BigInt("0x10"));
console.log(new Date(0).toISOString(), Date.UTC(2026, 0, 1));
console.log("a-b-c".split("-"), "abc".replaceAll("b", "B"), /(\d+)/.exec("x42")[1]);
```

```output for=map
9007199254740991 2.220446049250313e-16 true ff 1.00
bigint 18446744073709551616n 1n 16n
1970-01-01T00:00:00.000Z 1767225600000
[ 'a', 'b', 'c' ] aBc 42
```

| Object | Clause | Holds | Main page |
|---|---|---|---|
| `Number` | 21.1 | 64-bit float values; constants such as `MAX_SAFE_INTEGER` and `EPSILON`; `isInteger`, `isSafeInteger`, `parseInt`; `toFixed`, `toPrecision`, `toString(radix)` | [[javascript/numbers]] |
| `BigInt` | 21.2 | integers of any size; `asIntN`, `asUintN`; `toString(radix)` | [[javascript/bigints]] |
| `Math` | 21.3 | constants and functions on Number values | [[javascript/the-math-object]] |
| `Date` | 21.4 | a time value in milliseconds from 1970 UTC; `now`, `parse`, `UTC`; getters and setters for local time and UTC | [[date-and-time-values]] |
| `String` | 22.1 | UTF-16 strings; `fromCharCode`, `fromCodePoint`, `raw`; the string methods | [[javascript/strings]], [[javascript/string-methods]] |
| `RegExp` | 22.2 | regular expressions; `escape`; `exec`, `test` and the flags | [[javascript/regular-expressions]] |

## Semantics

**r1.** MDN puts these objects in two groups. Numbers and dates: "These are the base objects representing numbers, dates, and mathematical calculations."[^1] Text processing: "These objects represent strings and support manipulating them."[^1] The specification has the same split: clause 21 is Numbers and Dates, and clause 22 is Text Processing.[^2][^3]

**r2.** A primitive number, bigint or string has the methods of its prototype. The value is not an object, but a property read on it uses `Number.prototype`, `BigInt.prototype` or `String.prototype`. So `(255).toString(16)` and `"abc".replaceAll(...)` work. See [[javascript/primitive-wrapper-objects]]. Example: [`map`](#map).

**r3.** Called as a function, `Number` and `String` convert a value. Called with `new`, each "creates and initializes a new" object.[^2] For String, the text is the same.[^3] Use the function form. The BigInt constructor "performs a type conversion when called as a function rather than as a constructor".[^2] It throws a `TypeError` with `new`.

**r4.** Number and BigInt are separate types. An operator with a Number on one side and a BigInt on the other throws a `TypeError`. Convert one side with `Number(x)` or `BigInt(x)`. Example: [`mix-bigint`](#mix-bigint).

**r5.** A `Date` holds one number, the time value. "Time measurement in ECMAScript is analogous to time measurement in POSIX, in particular sharing definition in terms of the proleptic Gregorian calendar, an epoch of midnight at the beginning of 1 January 1970 UTC, and an accounting of every day as comprising exactly 86,400 seconds (each of which is 1000 milliseconds long)."[^2] The months count from 0, and an argument out of range moves the date into the next month. Example: [`date-month`](#date-month).

**r6.** A string is a list of UTF-16 code units, so `length` counts code units, not characters. A character outside the Basic Multilingual Plane takes two units. See [[javascript/unicode-and-utf-16-strings]]. Example: [`statics`](#statics).

**r7.** The static members are few, and most work is in the prototype methods. Example: [`statics`](#statics).

## Examples

```javascript run id=statics
const fns = (o) => Object.getOwnPropertyNames(o).filter((k) => typeof Object.getOwnPropertyDescriptor(o, k).value === "function");
for (const [name, ctor] of [["Number", Number], ["BigInt", BigInt], ["String", String], ["Date", Date], ["RegExp", RegExp]]) {
  console.log(`${name}:`, fns(ctor).join(" "));
}
console.log("é".length, "😀".length, [..."😀"].length, "😀".codePointAt(0).toString(16));
console.log(typeof Temporal);
```

```output for=statics
Number: isFinite isInteger isNaN isSafeInteger parseFloat parseInt
BigInt: asUintN asIntN
String: fromCharCode fromCodePoint raw
Date: now parse UTC
RegExp: escape
1 2 1 1f600
undefined
```

Each constructor has only a few static functions (rule r7). The emoji has one code point but two code units, so its `length` is 2, and the spread gives one element (rule r6). Node v24.10.0 has no `Temporal`.

## Versions

`RegExp.escape` is in the 17th edition (ECMAScript 2026).[^3] The TC39 list of finished proposals gives 2025 as its year.[^4] Temporal is a new API for dates and times. MDN lists it with Number, BigInt, Math and Date.[^1] The TC39 list gives 2027 as its year,[^4] so the 17th edition does not have it, and Node v24.10.0 does not have it (see [`statics`](#statics)). Until it is there, `Date` is the only standard date object.

## Pitfalls

### A day out of range in Date

```javascript wrong id=date-month
const d = new Date(Date.UTC(2026, 1, 30));
console.log(d.toISOString());
console.log(new Date(Date.UTC(2026, 0, 1)).getUTCMonth());
```

```output for=date-month
2026-03-02T00:00:00.000Z
0
```

Month `1` is February, because the months count from 0. February 2026 has 28 days, so day 30 moves to March 2, with no error (rule r5). Check the fields after you make a date from input.

### Number and BigInt in one expression

```javascript fails id=mix-bigint expect=exception:TypeError
console.log(1n + 2n);
console.log(1n + 1);
```

The engine does not convert between the two types, because the conversion can lose data (rule r4). Write `1n + BigInt(1)` or `Number(1n) + 1`.

## Comparison

Python has one integer type with no size limit and a separate float type. JavaScript has Number for both, and a separate BigInt for exact large integers. Python keeps dates in the module `datetime`. In JavaScript, `Date` is a global, but it has only a time value and the local time zone of the host, so programs often use a library for dates until Temporal is in the standard. For text, both languages use immutable strings, but Python counts code points and JavaScript counts UTF-16 code units (rule r6).

## References
