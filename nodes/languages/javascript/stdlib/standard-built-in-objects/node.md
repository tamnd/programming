---
tag: NEW-javascript-standard-built-in-objects
kind: feature
owner: javascript
slug: standard-built-in-objects
name: Standard built-in objects
aliases: [built-in objects, built-ins, global objects, the JavaScript standard library, standard library of JavaScript, static and instance methods, Array.prototype]
area: languages/javascript/stdlib
level: core
summary: >-
  The standard built-in objects are the objects that every ECMAScript program gets: the global object, constructors such as Array and Map, and namespaces such as Math and JSON.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-standard-built-in-objects.html", locator: "18 ECMAScript Standard Built-in Objects; clauses 19 to 28"}
see_also: [javascript/the-global-object, javascript/fundamental-objects, javascript/language-and-host, javascript/node-js-core-modules, python/standard-library-map]
sources:
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-standard-built-in-objects.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 18
  locator: 18 ECMAScript Standard Built-in Objects
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects
  title: MDN Web Docs, JavaScript Reference, Standard built-in objects
  locator: the introduction; Standard objects by category
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object
  title: MDN Web Docs, JavaScript Reference, Object
  locator: Description (Object prototype properties)
  accessed: 2026-11-18
- url: https://raw.githubusercontent.com/tc39/proposals/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Expected Publication Year 2026 and 2027
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=groups
const names = ["Object", "Array", "Map", "Promise", "Math", "JSON", "Reflect", "Intl", "parseInt", "NaN", "globalThis"];
for (const n of names) {
  console.log(n.padEnd(10), typeof globalThis[n]);
}
```

```output for=groups
Object     function
Array      function
Map        function
Promise    function
Math       object
JSON       object
Reflect    object
Intl       object
parseInt   function
NaN        number
globalThis object
```

Each built-in is a property of the global object. Most are constructors, which are functions. `Math`, `JSON`, `Reflect` and `Intl` are plain objects that hold functions. A few are values, such as `NaN`.

| Group (MDN) | Clause of ECMA-262 | Members | Map page |
|---|---|---|---|
| Value and function properties | 19 | `globalThis`, `Infinity`, `NaN`, `undefined`, `eval`, `parseInt`, `parseFloat`, `isNaN`, `isFinite`, the URI functions | [[javascript/the-global-object]] |
| Fundamental objects, error objects | 20 | `Object`, `Function`, `Boolean`, `Symbol`, `Error` and its subclasses | [[javascript/fundamental-objects]] |
| Numbers and dates | 21 | `Number`, `BigInt`, `Math`, `Date` | [[javascript/text-and-numbers-objects-map]], [[javascript/the-math-object]] |
| Text processing | 22 | `String`, `RegExp` | [[javascript/text-and-numbers-objects-map]] |
| Indexed collections | 23 | `Array`, the typed arrays | [[javascript/collection-objects-map]] |
| Keyed collections | 24 | `Map`, `Set`, `WeakMap`, `WeakSet` | [[javascript/collection-objects-map]] |
| Structured data | 25 | `ArrayBuffer`, `SharedArrayBuffer`, `DataView`, `Atomics`, `JSON` | [[javascript/structured-data-objects]] |
| Managing memory | 26 | `WeakRef`, `FinalizationRegistry` | |
| Control abstraction | 27 | `Iterator`, `Promise`, the generator and async function objects | [[javascript/control-abstraction-objects]] |
| Reflection | 28 | `Reflect`, `Proxy` | [[javascript/reflection]], [[javascript/proxies]] |
| Internationalization | ECMA-402 | `Intl` and its constructors | [[javascript/internationalization]] |

## Semantics

**r1.** "There are certain built-in objects available whenever an ECMAScript Script or Module begins execution. One, the global object, is part of the global environment of the executing program. Others are accessible as initial properties of the global object or indirectly as properties of accessible built-in objects."[^1] So a program can use `Array` or `Math` with no import. Example: [`groups`](#groups).

**r2.** The standard objects are only a part of what a program sees. MDN: "Other objects in the global scope are either created by the user script or provided by the host application. The host objects available in browser contexts are documented in the API reference."[^2] Files, timers and the network come from the host, not from ECMA-262. See [[javascript/language-and-host]].

**r3.** "Many built-in objects are functions: they can be invoked with arguments. Some of them furthermore are constructors: they are functions intended for use with the new operator."[^1] A built-in function that is not a constructor throws a `TypeError` with `new`.[^1] Example: [`not-constructor`](#not-constructor).

**r4.** A constructor has two kinds of members. A static method is a property of the constructor, such as `Array.from`. An instance method is a property of the prototype, such as `Array.prototype.map`, and every array gets it through the prototype chain. MDN names the methods in these two forms, so the name tells where to look. Example: [`find-method`](#find-method).

**r5.** The properties of the built-ins are not enumerable. Clause 18 says that a data property in clauses 19 to 28 is writable and configurable, but not enumerable, unless the text says otherwise.[^1] Some values are fixed: `Math.PI` cannot be changed. So `for...in` and `Object.keys` do not show the built-in methods. Example: [`attributes`](#attributes).

**r6.** A built-in function does not check the count of its arguments. A missing argument is `undefined`, and for extra arguments, "the extra arguments are evaluated by the call and then ignored by the function."[^1] The `length` of a built-in function is the count of its required parameters: "Optional parameters and rest parameters are not included in the parameter count."[^1] Example: [`arguments`](#arguments).

**r7.** A program can add to a built-in prototype, and all objects of that type see the change. MDN calls this "a very powerful although potentially dangerous mechanism to override or extend object behavior."[^3] Example: [`extend-prototype`](#extend-prototype).

## Examples

```javascript run id=find-method
console.log(typeof Array.from, typeof Array.prototype.from);
console.log(typeof Array.prototype.toSorted, typeof [3, 1].toSorted);
console.log(Object.hasOwn(Array.prototype, "map"), Object.hasOwn([], "map"));
console.log(Object.getOwnPropertyNames(Math).filter((k) => typeof Math[k] === "number").join(" "));
```

```output for=find-method
function undefined
function function
true false
E LN10 LN2 LOG10E LOG2E PI SQRT1_2 SQRT2
```

`Array.from` is static, so it is on `Array` and not on the prototype. `toSorted` is on `Array.prototype`, so every array has it, but it is not an own property of the array (rule r4). `Object.getOwnPropertyNames` lists the members of a built-in, including the ones that are not enumerable.

```javascript run id=attributes
const show = (o, k) => {
  const d = Object.getOwnPropertyDescriptor(o, k);
  return `${k}: writable ${d.writable}, enumerable ${d.enumerable}, configurable ${d.configurable}`;
};
console.log(show(globalThis, "Array"));
console.log(show(Array.prototype, "map"));
console.log(show(Math, "PI"));
console.log(show(Array.prototype.map, "length"));
```

```output for=attributes
Array: writable true, enumerable false, configurable true
map: writable true, enumerable false, configurable true
PI: writable false, enumerable false, configurable false
length: writable false, enumerable false, configurable true
```

The constructors and the methods follow the default of clause 18. `Math.PI` and the `length` of a function have their own attributes (rule r5).

```javascript run id=arguments
console.log(Array.prototype.map.length, parseInt.length, Math.max.length);
console.log(parseInt("10"), parseInt("10", undefined), Math.max(1, 5, 3, 9, 2));
console.log(Math.abs(-4, "extra", "ignored"));
```

```output for=arguments
1 2 2
10 10 9
4
```

`map` has one required parameter, because `thisArg` is optional. A missing radix is the same as `undefined`, and `Math.abs` ignores the extra arguments (rule r6).

```javascript run id=feature-check
const checks = {
  "Error.isError": typeof Error.isError,
  "Array.fromAsync": typeof Array.fromAsync,
  "JSON.rawJSON": typeof JSON.rawJSON,
  "Math.sumPrecise": typeof Math.sumPrecise,
  "Uint8Array.fromBase64": typeof Uint8Array.fromBase64,
  "Map.prototype.getOrInsert": typeof Map.prototype.getOrInsert,
  "Iterator.concat": typeof Iterator.concat,
};
for (const [name, type] of Object.entries(checks)) console.log(name.padEnd(26), type);
```

```output for=feature-check
Error.isError              function
Array.fromAsync            function
JSON.rawJSON               function
Math.sumPrecise            undefined
Uint8Array.fromBase64      undefined
Map.prototype.getOrInsert  undefined
Iterator.concat            undefined
```

All seven functions are in the 17th edition (ECMAScript 2026). Node v24.10.0 has only the first three. A test with `typeof` finds out what the engine has before the code uses it.

## Versions

Each edition adds built-ins. The TC39 list of finished proposals gives 2026 as the year for `Error.isError`, `Math.sumPrecise`, `Array.fromAsync`, Uint8Array to and from Base64, JSON.parse source text access, Iterator Sequencing and Upsert.[^4] The 17th edition (ECMAScript 2026) has them as `Error.isError`, `Math.sumPrecise`, `Array.fromAsync`, `Uint8Array.fromBase64`, `JSON.rawJSON`, `Iterator.concat` and `Map.prototype.getOrInsert`. The same list gives 2027 for Temporal and for Explicit Resource Management, so they are not in the 17th edition.[^4] MDN lists `Temporal`, `DisposableStack` and `AsyncDisposableStack` with the other built-ins already.[^2] An engine can lag behind the edition, as [`feature-check`](#feature-check) shows for Node v24.10.0.

## Pitfalls

### A method added to a built-in prototype

```javascript wrong id=extend-prototype
Array.prototype.last = function () {
  return this[this.length - 1];
};
const xs = [10, 20];
const seen = [];
for (const i in xs) seen.push(i);
console.log(xs.last(), seen);
```

```output for=extend-prototype
20 [ '0', '1', 'last' ]
```

The new property is enumerable, so `for...in` shows it on every array. A later edition can also add a method with the same name and other behavior. Write a plain function, such as `last(xs)`, and do not change the built-in prototypes (rule r7).

### new with a function that is not a constructor

```javascript fails id=not-constructor expect=exception:TypeError
console.log(typeof Math.max);
new Math.max(1, 2);
```

`Math.max` is a function but not a constructor (rule r3). Call it with no `new`.

## Comparison

The JavaScript standard library is small compared with the library of Python or Java. ECMA-262 defines values, collections, text, numbers, dates and control objects, but it has no functions for files, the network or processes. MDN: "This chapter documents all of JavaScript's standard, built-in objects, including their methods and properties."[^2] The host adds the rest: the browser adds the Web APIs, and Node adds its core modules. See [[javascript/web-platform-apis]] and [[javascript/node-js-core-modules]]. Python puts a large library in modules that a program imports. See [[python/standard-library-map]].

## References
