---
tag: ESBBPH
kind: feature
owner: javascript
slug: history-of-javascript
name: History of JavaScript
aliases: [Mocha, LiveScript, ES1, ES3, ES4, ES5, ES6, ECMAScript Harmony]
area: languages/javascript/overview
level: intro
summary: >-
  JavaScript began as Mocha at Netscape in 1995. ECMA-262 standardized it in 1997. ES4 was abandoned, ES5 came in 2009, ES2015 in 2015, and one edition has come each year since.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/", locator: "Introduction"}
see_also: [javascript, javascript/what-is-javascript, javascript/javascript-and-the-ecmascript-specification, javascript/tc39-and-the-proposal-process, typescript]
sources:
  - url: https://zenodo.org/records/3707008
    title: "Allen Wirfs-Brock and Brendan Eich, JavaScript: The First 20 Years (HOPL IV, authors' preprint 2020-03-11)"
    locator: 2.2 The Story of Mocha; 10 Naming the Standard; 18 Harmony; 19.2 CommonJS and Node.js; Fig. 1 ECMA-262 Editions
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/
    title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition)
    locator: Introduction
    accessed: 2026-11-18
  - url: https://ecma-international.org/publications-and-standards/standards/ecma-262/
    title: ECMA-262, Ecma International
    locator: ECMAScript 2026 language specification, 17th edition; Edition 4
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Introduction
    title: Introduction, MDN JavaScript Guide
    locator: JavaScript and Java
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/strict-mode-of-ecmascript.html#sec-strict-mode-of-ecmascript
    title: ECMAScript 2026 Language Specification
    locator: Annex C (informative) The Strict Mode of ECMAScript
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/JavaScript_technologies_overview
    title: JavaScript technologies overview, MDN
    locator: Standardization process; JavaScript implementations
    accessed: 2026-11-18
  - url: https://tc39.es/process-document/
    title: The TC39 Process
    locator: Spec revisions and scheduling
    accessed: 2026-11-18
---

## Synopsis

The main events, from the history by Allen Wirfs-Brock and Brendan Eich for HOPL IV[^1] and the introduction of ECMA-262[^2]:

| Date | Event |
|---|---|
| May 1995 | Brendan Eich writes the first Mocha prototype at Netscape in ten days[^1] |
| September 1995 | the language ships as LiveScript in the first beta of Netscape Navigator 2.0[^1] |
| December 1995 | Netscape and Sun announce it under the name JavaScript[^1] |
| March 1996 | Navigator 2.0 is released[^1] |
| 1996 | Microsoft ships JScript in Internet Explorer 3[^1] |
| November 1996 | work on the ECMAScript specification starts at Ecma[^2] |
| June 1997 | ECMA-262, 1st edition (ES1)[^2] |
| June 1998 | 2nd edition (ES2), aligned with ISO/IEC 16262[^2] |
| December 1999 | 3rd edition (ES3)[^2] |
| July 2008 | the ES4 work ends at the TC39 meeting in Oslo[^1] |
| December 2009 | 5th edition (ES5); there is no 4th edition[^2] [^3] |
| June 2011 | edition 5.1[^2] |
| June 2015 | 6th edition, ECMAScript 2015 (ES6)[^2] |
| 2016 on | one edition each year: ES2016 is the 7th, ES2026 the 17th[^2] [^3] |

## Semantics

**r1.** Netscape wanted a scripting language for the browser that would "look like Java". The code name was Mocha, and Eich wrote the first implementation "in ten contiguous days in May, 1995".[^1] The language was shown to the public under the name LiveScript in the first beta of Navigator 2.0, in September 1995, and was then named JavaScript as part of a partnership with Sun.[^1] MDN says the rename came because JavaScript "follows most Java expression syntax, naming conventions and basic control-flow constructs".[^4]

**r2.** Microsoft built its own implementation, JScript, from October 1995, and shipped it in 1996 in Internet Explorer 3.[^1] "The development of the ECMAScript Language Specification started in November 1996."[^2]

**r3.** The standard could not use the name JavaScript, which was a trademark of Sun. At the first TC39 meeting, the members agreed "to use “ECMAScript” as a placeholder name until a more suitable name could be found", and the placeholder stayed.[^1]

**r4.** The first edition was adopted in June 1997, and became ISO/IEC 16262 in April 1998. The second edition (June 1998) made only editorial changes. The third edition (December 1999) "introduced powerful regular expressions, better string handling, new control statements, try/catch exception handling" and more.[^2] Example: [`by-edition`](#by-edition).

**r5.** A large fourth edition, ES4, was planned for years and never finished. ECMA-262 says the work "was not completed and not published as the fourth edition of ECMAScript but some of it was incorporated into the development of the sixth edition."[^2] At the Oslo meeting of July 2008, the committee agreed on a plan: finish the smaller ES3.1 during 2009, and plan a larger follow-on edition, code named Harmony, that was not bound by the ES4 design.[^1] Ecma says that "“ECMA-262 Edition 4” as an Ecma International publication does not exist".[^3]

**r6.** ES3.1 was published as the 5th edition in December 2009.[^1] [^2] It added accessor properties, reflection on objects, JSON support and strict mode.[^2] Strict mode turns some silent mistakes into errors: in strict code, an assignment to an undeclared name throws a `ReferenceError` instead of making a global variable.[^5] Example: [`strict`](#strict).

**r7.** Some choices of 1995 are still in the language. The result of `typeof null` is the string `object`. In the Mocha engine, "The runtime value of null was encoded with the same internal tag value used for object values". ES5 did not change it, because changing `typeof null` "would not be subtractive", so it did not fit strict mode.[^1] Example: [`typeof-null`](#typeof-null).

**r8.** In September 2008 Google released Chrome with the V8 engine, which "became the new baseline for good JavaScript performance".[^1] In early 2009 Ryan Dahl began Node.js, a JavaScript platform for server applications.[^1]

**r9.** The 6th edition, ECMAScript 2015, was adopted in June 2015. Its major additions "included modules, class declarations, lexical block scoping, iterators and generators, promises for asynchronous programming, destructuring patterns, and proper tail calls."[^2] "ES6 is synonymous with ES2015."[^6]

**r10.** From ECMAScript 2016 on, TC39 works on GitHub and releases one edition a year.[^2] TC39 plans to send each edition to the Ecma General Assembly "in July of each year".[^7] The 17th edition, ECMAScript 2026, was published in June 2026.[^3]

## Examples

One feature or more from each step of the history (rules r4, r6, r9 and r10). The comments give the edition that added each one, from the introduction of ECMA-262.[^2]

```javascript run id=by-edition
// ES3 (1999): regular expressions, try and catch
try {
  null.length;
} catch (e) {
  console.log("ES3", e instanceof TypeError, /^\d{4}$/.test("1999"));
}

// ES5 (2009): JSON and strict mode
const strictThis = (function () {
  "use strict";
  return this;
})();
console.log("ES5", JSON.stringify({ edition: 5 }), strictThis);

// ES2015: block scoping, classes, destructuring
const [first, ...rest] = [2015, 2016, 2017];
class Edition {
  constructor(year) {
    this.year = year;
  }
  toString() {
    return `ES${this.year}`;
  }
}
console.log(`${new Edition(first)}`, rest);

// ES2016: ** and includes
console.log("ES2016", 2 ** 8, rest.includes(2017));

// ES2020: ?? and ?. and BigInt
const options = { retries: 0 };
console.log("ES2020", options.retries ?? 3, options.timeout?.ms, 2n ** 64n);

// ES2022: at and Object.hasOwn
console.log("ES2022", rest.at(-1), Object.hasOwn(options, "retries"));

// ES2023: toSorted leaves the array alone
const years = [2015, 1997, 2009];
console.log("ES2023", years.toSorted(), years);
```

```output for=by-edition
ES3 true true
ES5 {"edition":5} undefined
ES2015 [ 2016, 2017 ]
ES2016 256 true
ES2020 0 undefined 18446744073709551616n
ES2022 2017 true
ES2023 [ 1997, 2009, 2015 ] [ 2015, 1997, 2009 ]
```

Without strict mode, a typo in an assignment makes a new global variable, and the program goes on (rule r6):

```javascript run id=sloppy
function setTotal() {
  totl = 10; // a typo for a variable called total
}
setTotal();
console.log(globalThis.totl);
```

```output for=sloppy
10
```

With `"use strict"`, the same typo throws a `ReferenceError`:

```javascript fails id=strict expect=exception:ReferenceError
"use strict";
function setTotal() {
  totl = 10; // a typo for a variable called total
}
setTotal();
```

`typeof null` has been `"object"` from the first engine (rule r7). To test for an object, also test for `null`:

```javascript run id=typeof-null
console.log(typeof null, typeof undefined, typeof {});
console.log(null instanceof Object);
const value = null;
console.log(value !== null && typeof value === "object");
```

```output for=typeof-null
object undefined object
false
false
```

## Pitfalls

### Old advice may date from ES3

Much JavaScript advice was written when ES3 was the standard, and ES3 "was not replaced by a newer edition until December 2009".[^1] Advice from that time knows nothing of `let`, `const`, classes or modules, which came with ES2015 (rule r9). Check which edition added the feature that the advice works around.

### "ES7" and "ES8" are not version numbers of a product

The editions after ES2015 are named by year. MDN says that before ES6 they were "commonly referred to by their major version numbers", and after ES6 "the specification is named by the publishing year".[^6] So ES7 is ES2016, ES8 is ES2017, and there is no ES4.

### The name JavaScript is not the name of the standard

Sun held the trademark "JavaScript", so the standard is called ECMAScript (rule r3).[^1] The two names mean the same language.

## Comparison

**Java** and Mocha were made at the same time: Eich wrote Mocha "With the Java announcements imminent", and the name JavaScript came from Netscape's partnership with Sun (rule r1).[^1] The languages share syntax, not semantics ([[javascript/what-is-javascript]]).

**JScript** was Microsoft's implementation of the same language. ECMA-262 names it, with JavaScript of Netscape, as one of the "originating technologies" of ECMAScript.[^2]

## References
