---
tag: BA12VR
kind: feature
owner: javascript
slug: javascript-and-the-ecmascript-specification
name: JavaScript and the ECMAScript specification
aliases: [ECMAScript, ECMA-262, ES2026, ECMA-402, the JavaScript standard]
area: languages/javascript/overview
level: intro
summary: >-
  ECMA-262, the ECMAScript Language Specification, defines the JavaScript language. Ecma TC39 publishes one edition each year, and ECMA-402 adds the Intl API.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/", locator: "Introduction; clauses 1 to 4"}
see_also: [javascript, javascript/what-is-javascript, javascript/history-of-javascript, javascript/tc39-and-the-proposal-process, program-semantics, context-free-grammars]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/
    title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition)
    locator: Introduction
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/JavaScript_technologies_overview
    title: JavaScript technologies overview, MDN
    locator: JavaScript, the core language (ECMAScript); Standardization process; Internationalization API
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/scope.html#sec-scope
    title: ECMAScript 2026 Language Specification
    locator: 1 Scope
    accessed: 2026-11-18
  - url: https://ecma-international.org/publications-and-standards/standards/ecma-262/
    title: ECMA-262, Ecma International
    locator: ECMAScript 2026 language specification, 17th edition; Edition 4
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/conformance.html#sec-conformance
    title: ECMAScript 2026 Language Specification
    locator: 2 Conformance
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/additional-ecmascript-features-for-web-browsers.html
    title: ECMAScript 2026 Language Specification
    locator: Annex B (normative) Additional ECMAScript Features for Web Browsers; B.2.1.1 escape; B.2.2.1 String.prototype.substr
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/normative-references.html#sec-normative-references
    title: ECMAScript 2026 Language Specification
    locator: 3 Normative References
    accessed: 2026-11-18
  - url: https://tc39.es/ecma402/2026/
    title: ECMAScript 2026 Internationalization API Specification (ECMA-402, 13th edition)
    locator: 1 Scope; 4.4 Implementation Dependencies; 10.1.1, 11.1.1, 16.1.1
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/overview.html#sec-organization-of-this-specification
    title: ECMAScript 2026 Language Specification
    locator: 4.5 Organization of This Specification
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Introduction
    title: Introduction, MDN JavaScript Guide
    locator: JavaScript and the ECMAScript specification; JavaScript documentation versus the ECMAScript specification
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/indexed-collections.html#sec-array.prototype.includes
    title: ECMAScript 2026 Language Specification
    locator: 23.1.3.16 Array.prototype.includes; 23.1.3.17 Array.prototype.indexOf
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-equality-operators-runtime-semantics-evaluation
    title: ECMAScript 2026 Language Specification
    locator: 13.11.1 Runtime Semantics, Evaluation (of the equality operators)
    accessed: 2026-11-18
  - url: https://tc39.es/process-document/
    title: The TC39 Process
    locator: Spec revisions and scheduling
    accessed: 2026-11-18
  - url: https://github.com/tc39/ecma262/blob/main/README.md
    title: tc39/ecma262 README
    locator: ECMAScript (the repository holds the current draft)
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/introduction.html
    title: The Python Language Reference, Python 3.15
    locator: 1. Introduction
    accessed: 2026-11-18
  - url: https://docs.oracle.com/javase/specs/
    title: Java SE Specifications
    locator: Java SE 27, released September 2026 as JSR 402
    accessed: 2026-11-18
---

## Synopsis

JavaScript is defined by a standard of Ecma International called ECMA-262, the ECMAScript Language Specification. The 2026 edition says: "This Ecma Standard defines the ECMAScript 2026 Language. It is the seventeenth edition of the ECMAScript Language Specification."[^1] The name of the language in the standard is ECMAScript, and MDN says that "ECMAScript" and "JavaScript" "can be used interchangeably".[^2]

| Document | What it defines | Where to read it |
|---|---|---|
| ECMA-262 | the language: syntax, semantics, the standard library | `https://tc39.es/ecma262/2026/` (one page) or `https://tc39.es/ecma262/2026/multipage/` |
| ECMA-402 | the `Intl` API for locales, collation, number and date formats | `https://tc39.es/ecma402/2026/` |
| ECMA-404 | the JSON data interchange format | the Ecma International site |
| the draft | the next edition while it is written | `https://tc39.es/ecma262/` |

The edition is named by the year: ES2015 is the 6th edition, ES2016 the 7th, and so on, so ES2026 is the 17th.[^1] [^2]

## Semantics

**r1.** ECMA-262 is the only normative definition of the language. Its scope clause says: "This Standard defines the ECMAScript 2026 general-purpose programming language."[^3] Ecma International publishes it as "ECMAScript® 2026 language specification 17th edition, June 2026".[^4]

**r2.** A conforming implementation must provide all the types, values, objects, properties, functions, syntax and semantics of the standard. It may add more types, values, objects, properties and functions, and more syntax. It must not implement a Forbidden Extension (17.1), and it must not redefine facilities that the standard does not leave to the implementation or the host.[^5]

**r3.** Some subclauses are marked Normative Optional. An implementation may leave them out, but "Web browsers are generally required to implement all normative optional subclauses."[^5] Annex B holds the extra syntax and built-ins, such as `escape` and `String.prototype.substr`, that are required when the host is a web browser.[^6] Example: [`annex-b`](#annex-b).

**r4.** ECMA-262 builds on other standards. Its normative references are IEEE 754-2019 for floating-point arithmetic, the Unicode Standard and ISO/IEC 10646 for source text, ECMA-402 "specifically the annual edition corresponding to this edition of this specification", and ECMA-404 for JSON.[^7]

**r5.** ECMA-402, the ECMAScript Internationalization API Specification, defines the `Intl` object: `Intl.Collator` (10.1.1), `Intl.DateTimeFormat` (11.1.1), `Intl.NumberFormat` (16.1.1) and others. Its 2026 edition is the 13th.[^8] An implementation that has an API for "the linguistic and cultural conventions used by different human languages and countries" must implement ECMA-402.[^5] The set of locales and the exact formats are implementation dependent.[^8] Example: [`intl`](#intl).

**r6.** The standard has a fixed layout. Clause 5 defines the notation. Clauses 6 to 10 define the execution environment, clauses 11 to 17 the language, and clauses 18 to 28 the standard library. Clause 29 is the memory model for `SharedArrayBuffer` and `Atomics`.[^9] So a question about `let` is answered in clauses 11 to 17, and a question about `Array.prototype.sort` in clause 23.

**r7.** The standard is written for implementers. MDN says that the specification "is not intended to help script programmers", and that "JavaScript supports all functionality outlined in the ECMAScript specification."[^10] MDN is the guide, and ECMA-262 is the authority when the two seem to differ.

**r8.** The specification states each built-in as numbered algorithm steps, so small details are exact. `Array.prototype.includes` compares with SameValueZero, which finds `NaN`. `Array.prototype.indexOf` compares with IsStrictlyEqual, which never finds `NaN`. The note on `includes` says this difference is intentional, and that `includes` treats a hole in the array as `undefined`, while `indexOf` skips it.[^11] IsStrictlyEqual is also the algorithm of the `===` operator (13.11.1).[^12] Example: [`same-value`](#same-value).

**r9.** A new edition comes out each year. ECMAScript 2016 "was the first ECMAScript edition released under Ecma TC39's new yearly release cadence and open development process."[^1] TC39 "intends to submit a specification to the Ecma General Assembly for ratification in July of each year".[^13] Before ES2015 the editions were called by number (ES3, ES5); from ES2015 on they are called by year, and "ES6 is synonymous with ES2015".[^2]

**r10.** Each edition lists what it added in its introduction. For example, the 16th edition (ES2025) added the `Iterator` global, the `Set.prototype` methods, JSON modules, `RegExp.escape`, `Promise.try` and `Float16Array` with `Math.f16round`.[^1] Example: [`editions`](#editions).

## Examples

The features that the introduction lists for ES2025 (rule r10), tested in Node.js. The program ran on Node.js 24.10.0.

```javascript run id=editions
const es2025 = {
  "Promise.try": Promise.try,
  "RegExp.escape": RegExp.escape,
  "Math.f16round": Math.f16round,
  Float16Array: globalThis.Float16Array,
};
for (const [name, value] of Object.entries(es2025)) {
  console.log(name, typeof value);
}
console.log(RegExp.escape("(1+1)"));
console.log(Math.f16round(0.1));
```

```output for=editions
Promise.try function
RegExp.escape function
Math.f16round function
Float16Array function
\(1\+1\)
0.0999755859375
```

`includes` and `indexOf` follow different algorithms (rule r8). `NaN` is found by one and not by the other, `-0` and `0` count as equal in both, and the hole at index 1 is `undefined` for `includes` only.

```javascript run id=same-value
const values = [1, NaN, -0];
console.log(values.indexOf(NaN), values.includes(NaN));
console.log(values.indexOf(0), values.includes(0));

const sparse = [1, , 3]; // index 1 is a hole
console.log(sparse.indexOf(undefined), sparse.includes(undefined));
```

```output for=same-value
-1 true
2 true
-1 true
```

The `Intl` objects of ECMA-402 (rule r5). The same number and date come out in the format of each locale, and the German and Swedish collations put `ä` in different places.

```javascript run id=intl
const n = 1234567.891;
console.log(new Intl.NumberFormat("en-US").format(n));
console.log(new Intl.NumberFormat("de-DE").format(n));
console.log(new Intl.NumberFormat("en-IN").format(n));

const june = new Date(Date.UTC(1997, 5, 1));
const style = { dateStyle: "long", timeZone: "UTC" };
console.log(new Intl.DateTimeFormat("en-GB", style).format(june));
console.log(new Intl.DateTimeFormat("fr-FR", style).format(june));

const words = ["b", "a", "ä", "z"];
console.log(words.toSorted(new Intl.Collator("de").compare).join(" "));
console.log(words.toSorted(new Intl.Collator("sv").compare).join(" "));
```

```output for=intl
1,234,567.891
1.234.567,891
12,34,567.891
1 June 1997
1 juin 1997
a ä b z
a b z ä
```

## Pitfalls

### Citing the draft instead of an edition

`https://tc39.es/ecma262/` is the draft of the next edition. The tc39/ecma262 repository says that it holds "the source for the current draft of ECMA-262".[^14] Section numbers and text in the draft change from month to month. To cite a rule, link the edition with its year, such as `https://tc39.es/ecma262/2026/`, and give the section number. The dated edition is what Ecma approved.[^4]

### An edition is not a promise about an engine

An edition lists what the standard requires, not what a given engine ships. The ES2026 edition defines `Math.sumPrecise`, but Node.js 24.10.0 does not have it. Test for a feature before you use it, and keep a fallback:

```javascript run id=sum-precise
const prices = [0.1, 0.2, 0.3];

function sum(values) {
  if (typeof Math.sumPrecise === "function") {
    return Math.sumPrecise(values); // ES2026
  }
  // Fallback: Neumaier's compensated sum, close to but not the same as sumPrecise
  let total = 0;
  let error = 0;
  for (const v of values) {
    const t = total + v;
    error += Math.abs(total) >= Math.abs(v) ? total - t + v : v - t + total;
    total = t;
  }
  return total + error;
}

console.log(prices.reduce((a, b) => a + b));
console.log(sum(prices));
```

```output for=sum-precise
0.6000000000000001
0.6
```

### The Annex B features work, but are legacy

Browsers and Node.js run the Annex B built-ins, so code that uses them seems fine (rule r3). Annex B says of them: "Programmers should not use or assume the existence of these features and behaviours when writing new ECMAScript code."[^6] Each one has a replacement in the main body of the standard: `slice` for `substr`, and `encodeURIComponent` for `escape`, which works on UTF-8 bytes.

```javascript run id=annex-b
const name = "JavaScript";
console.log(name.substr(4, 3), name.slice(4, 7));
console.log(name.bold());
console.log(escape("café"), encodeURIComponent("café"));
```

```output for=annex-b
Scr Scr
<b>JavaScript</b>
caf%E9 caf%C3%A9
```

### "ES6" and "ES2015" are one edition

Old books and posts say ES6, ES7 and ES8. These are the 6th, 7th and 8th editions, which are ES2015, ES2016 and ES2017.[^1] [^2] The number counts the editions, and the year is the year of publication.

## Comparison

**Python** has a language reference, but its introduction says: "I chose to use English rather than formal specifications for everything except syntax and lexical analysis." It also says that CPython "is the one Python implementation in widespread use".[^15] ECMA-262 instead writes every rule as algorithm steps, and no single engine defines the language.

**Java** has the Java Language Specification, with one edition for each Java SE release. Java SE 27 was released in September 2026 as JSR 402, with "The Java Language Specification, Java SE 27 Edition".[^16] Java gets two editions a year, ECMAScript one.

## Implementation notes

The examples ran on Node.js 24.10.0, which embeds V8. In that release, the four ES2025 features of `editions` are present, and `Math.sumPrecise` of ES2026 is not, so `sum-precise` takes its fallback. The `intl` output depends on the locale data that the engine ships (rule r5).

## References
