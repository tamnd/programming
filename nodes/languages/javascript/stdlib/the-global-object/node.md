---
tag: NEW-javascript-the-global-object
kind: feature
owner: javascript
slug: the-global-object
name: The global object
aliases: [global object, globalThis, global, window, self, parseInt, parseFloat, isNaN, isFinite, encodeURIComponent, decodeURIComponent, encodeURI, decodeURI, global functions]
area: languages/javascript/stdlib
level: core
summary: >-
  The global object holds the global names: value properties (globalThis, Infinity, NaN, undefined), functions such as parseInt and encodeURIComponent, and the constructors and namespaces.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/global-object.html", locator: "19 The Global Object (19.1 to 19.4)"}
see_also: [javascript/standard-built-in-objects, javascript/strict-mode, javascript/nan-and-infinity, javascript/undefined-and-null, python/the-builtins-namespace]
sources:
- url: https://tc39.es/ecma262/2026/multipage/global-object.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 19
  locator: 19 The Global Object; 19.1 Value Properties; 19.2.4 parseFloat; 19.2.5 parseInt; 19.2.6 URI Handling Functions; 19.3; 19.4
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/globalThis
  title: MDN Web Docs, JavaScript Reference, globalThis
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects
  title: MDN Web Docs, JavaScript Reference, Standard built-in objects
  locator: Value properties; Function properties
  accessed: 2026-11-18
- url: https://nodejs.org/docs/latest-v24.x/api/modules.html
  title: Node.js v24 documentation, Modules, CommonJS modules
  locator: the introduction; The module wrapper
  accessed: 2026-11-18
- url: https://nodejs.org/docs/latest-v24.x/api/globals.html
  title: Node.js v24 documentation, Global objects
  locator: the introduction
  accessed: 2026-11-18
- url: https://262.ecma-international.org/6.0/index.html
  title: ECMAScript 2015 Language Specification (ECMA-262, 6th edition)
  locator: 18.1 Value Properties of the Global Object
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=global
console.log(globalThis === global, typeof window, typeof self);
console.log(globalThis.Array === Array, globalThis.parseInt === parseInt);
globalThis.appName = "demo";
console.log(appName);
```

```output for=global
true undefined undefined
true true
demo
```

In Node, `globalThis` and `global` are the same object, and there is no `window`. A built-in such as `Array` is a property of the global object. A new property of the global object is a global variable.

| Part of clause 19 | Properties |
|---|---|
| 19.1 Value properties | `globalThis`, `Infinity`, `NaN`, `undefined` |
| 19.2 Function properties | `eval`, `isFinite`, `isNaN`, `parseFloat`, `parseInt`, `decodeURI`, `decodeURIComponent`, `encodeURI`, `encodeURIComponent` |
| 19.3 Constructor properties | `Array`, `Map`, `Promise`, `Error` and the other constructors, from `AggregateError` to `WeakSet` |
| 19.4 Other properties | `Atomics`, `JSON`, `Math`, `Reflect` |
| Annex B.2.1 | `escape`, `unescape` |
| The host | for example `window` and `document` in a browser, `global` and `process` in Node |

## Semantics

**r1.** The global object is an ordinary object that the engine makes before the program starts. It cannot be called and cannot be used with `new`. It "may have host-defined properties in addition to the properties defined in this specification. This may include a property whose value is the global object itself."[^1] The browser calls that property `window`, and Node calls it `global`. Example: [`global`](#global).

**r2.** `globalThis` is the standard name. "The globalThis property provides a standard way of accessing the global this value (and hence the global object itself) across environments."[^2] Before it, "On the web you can use window, self, or frames - but in Web Workers only self will work. In Node.js none of these work, and you must instead use global."[^2] Example: [`global`](#global).

**r3.** The value properties are fixed. MDN: "These global properties return a simple value. They have no properties or methods."[^3] The specification makes `Infinity`, `NaN` and `undefined` not writable and not configurable, and `globalThis` writable and configurable.[^1] Example: [`value-props`](#value-props).

**r4.** `parseInt` and `parseFloat` read a number from the start of a string. `parseInt` "produces an integral Number dictated by interpretation of the contents of string according to the specified radix. Leading white space in string is ignored."[^1] `parseFloat` "produces a Number value dictated by interpretation of the contents of the string argument as a decimal literal."[^1] Both stop at the first character that is not part of the number. If there is no number at the start, the result is `NaN`. Example: [`parse`](#parse).

**r5.** "The ECMAScript language itself does not provide any support for using URIs except for functions that encode and decode URIs as described in this section."[^1] `encodeURIComponent` replaces each reserved character with "one, two, three, or four escape sequences representing the UTF-8 encoding of the code point."[^1] `encodeURI` keeps the characters that separate the parts of a URI, such as `/`, `?` and `&`. Example: [`uri`](#uri).

**r6.** The global functions `isNaN` and `isFinite` convert the argument to a number first. `Number.isNaN` and `Number.isFinite` do not convert it. Example: [`isnan`](#isnan).

**r7.** Not every name at the top of a file is global. In Node, "Variables local to the module will be private, because the module is wrapped in a function by Node.js (see module wrapper)."[^4] The Node documentation also says: "The following variables may appear to be global but are not. They exist only in the scope of CommonJS modules".[^5] Those are `require`, `module`, `exports`, `__filename` and `__dirname`. In a classic script, a top-level `var` makes a property of the global object. Example: [`module-scope`](#module-scope).

## Examples

```javascript run id=value-props
for (const k of ["Infinity", "NaN", "undefined", "globalThis"]) {
  const d = Object.getOwnPropertyDescriptor(globalThis, k);
  console.log(k.padEnd(10), d.writable, d.enumerable, d.configurable);
}
```

```output for=value-props
Infinity   false false false
NaN        false false false
undefined  false false false
globalThis true false true
```

The three values cannot change. `globalThis` can be replaced or deleted. MDN: "The globalThis property is configurable and writable so that code authors can hide it when executing untrusted code and prevent exposing the global object."[^2]

```javascript run id=parse
console.log(parseInt("08"), parseInt("0x1F"), parseInt("12px"), parseInt("px12"), parseInt("101", 2));
console.log(parseFloat("3.14abc"), parseFloat(".5e1"), Number("12px"));
```

```output for=parse
8 31 12 NaN 5
3.14 5 NaN
```

`parseInt("12px")` stops at `p`, but `Number("12px")` reads the whole string and gives `NaN`. A string that starts with `0x` is read in base 16. Give the radix, as in `parseInt("101", 2)`, when the input can have other forms (rule r4).

```javascript run id=uri
const s = "a b&c=d/é?";
console.log(encodeURI(s));
console.log(encodeURIComponent(s));
console.log(decodeURIComponent("caf%C3%A9"));
```

```output for=uri
a%20b&c=d/%C3%A9?
a%20b%26c%3Dd%2F%C3%A9%3F
café
```

`encodeURI` keeps `&`, `=`, `/` and `?`. `encodeURIComponent` encodes them too, so use it for one value in a query string. The letter `é` is two bytes in UTF-8, so it becomes two escape sequences (rule r5).

## Versions

ECMAScript 2015, the 6th edition, had three value properties of the global object: "18.1.1 Infinity", "18.1.2 NaN" and "18.1.3 undefined".[^6] The 17th edition (ECMAScript 2026) has four, with "19.1.1 globalThis" first.[^1] Node v24 also has the older name `global`.[^5]

## Pitfalls

### The global isNaN converts its argument

```javascript wrong id=isnan
console.log(isNaN("hello"), Number.isNaN("hello"));
console.log(isFinite("12"), Number.isFinite("12"));
```

```output for=isnan
true false
true false
```

`"hello"` is a string, not `NaN`, but `isNaN` converts it to a number first, and the result is `NaN` (rule r6). Use `Number.isNaN` and `Number.isFinite` to test a value with no conversion.

### A top-level var in a module

```javascript wrong id=module-scope
var counter = 1;
console.log(globalThis.counter);
require("node:vm").runInThisContext("var counter2 = 2");
console.log(globalThis.counter2);
```

```output for=module-scope
undefined
2
```

Node runs the file as a CommonJS module, so `counter` is local to the module (rule r7). The string that `runInThisContext` runs is a script, so its `var` makes a property of the global object. An ES module also keeps its top-level names local. To make a global, set a property of `globalThis`.

### An assignment to undefined in strict mode

```javascript fails id=strict-undefined expect=exception:TypeError
"use strict";
undefined = 5;
```

`undefined` is not writable (rule r3). Code that is not strict ignores the assignment with no error. Strict code throws a `TypeError`. See [[javascript/strict-mode]].

### A malformed escape sequence

```javascript fails id=uri-error expect=exception:URIError
console.log(decodeURIComponent("100%25"));
decodeURIComponent("100%");
```

A `%` must have two hexadecimal digits after it, so the decode functions throw a `URIError`. Wrap the call in `try` when the input comes from a user.

## Comparison

Python keeps its built-in names in the module `builtins`, and a program cannot add a global name to all modules by accident. See [[python/the-builtins-namespace]]. In JavaScript the global object is shared by all the code in a realm, so a property that one script adds is visible to all the others. That is one reason why modules keep their names local.

## References
