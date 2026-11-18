---
tag: NEW-javascript-fundamental-objects
kind: feature
owner: javascript
slug: fundamental-objects
name: Fundamental objects
aliases: [Object, Function, Boolean, Symbol, Error, Object static methods, Object.prototype, Function.prototype, native errors, AggregateError, Error.isError]
area: languages/javascript/stdlib
level: core
summary: >-
  Clause 20 of ECMA-262 defines the fundamental objects: Object, Function, Boolean, Symbol and Error with its subclasses. This page maps their static members and prototype members.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/fundamental-objects.html", locator: "20 Fundamental Objects (20.1 Object to 20.5 Error)"}
see_also: [javascript/standard-built-in-objects, javascript/objects, javascript/well-known-symbols, javascript/eval-and-the-function-constructor, javascript/primitive-wrapper-objects]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects
  title: MDN Web Docs, JavaScript Reference, Standard built-in objects
  locator: Fundamental objects; Error objects
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object
  title: MDN Web Docs, JavaScript Reference, Object
  locator: Description; Object prototype properties
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/fundamental-objects.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 20
  locator: 20.1 Object Objects; 20.2 Function Objects; 20.3 Boolean Objects; 20.4 Symbol Objects; 20.5 Error Objects
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Error
  title: MDN Web Docs, JavaScript Reference, Error
  locator: the introduction; Error types
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2025/multipage/fundamental-objects.html
  title: ECMAScript 2025 Language Specification (ECMA-262, 16th edition), clause 20
  locator: 20.5.2 Properties of the Error Constructor
  accessed: 2026-11-18
- url: https://raw.githubusercontent.com/tc39/proposals/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Error.isError; Explicit Resource Management
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=members
const own = (o) => Object.getOwnPropertyNames(o).filter((k) => typeof Object.getOwnPropertyDescriptor(o, k).value === "function");
console.log("Object:", own(Object).length, "static,", own(Object.prototype).length, "on the prototype");
console.log("Function.prototype:", own(Function.prototype).join(" "));
console.log("Boolean.prototype:", own(Boolean.prototype).join(" "));
console.log("Symbol:", own(Symbol).join(" "), "|", own(Symbol.prototype).join(" "));
console.log("Error:", own(Error).join(" "), "|", own(Error.prototype).join(" "));
```

```output for=members
Object: 23 static, 11 on the prototype
Function.prototype: constructor apply bind call toString
Boolean.prototype: constructor toString valueOf
Symbol: for keyFor | constructor toString valueOf
Error: captureStackTrace prepareStackTrace isError | constructor toString
```

The list shows only the members with string keys that are functions. `Error.captureStackTrace` and `Error.prepareStackTrace` come from the V8 engine of Node. They are not in clause 20.

| Object | Clause | Static members | Prototype members |
|---|---|---|---|
| `Object` | 20.1 | `assign`, `create`, `defineProperty`, `defineProperties`, `entries`, `freeze`, `fromEntries`, `getOwnPropertyDescriptor`, `getOwnPropertyDescriptors`, `getOwnPropertyNames`, `getOwnPropertySymbols`, `getPrototypeOf`, `groupBy`, `hasOwn`, `is`, `isExtensible`, `isFrozen`, `isSealed`, `keys`, `preventExtensions`, `seal`, `setPrototypeOf`, `values` | `hasOwnProperty`, `isPrototypeOf`, `propertyIsEnumerable`, `toLocaleString`, `toString`, `valueOf`, `__proto__`, four legacy accessor methods |
| `Function` | 20.2 | `prototype` | `apply`, `bind`, `call`, `toString`, `[Symbol.hasInstance]` |
| `Boolean` | 20.3 | `prototype` | `toString`, `valueOf` |
| `Symbol` | 20.4 | `for`, `keyFor`, 13 well-known symbols | `description`, `toString`, `valueOf`, `[Symbol.toPrimitive]` |
| `Error` | 20.5 | `isError` | `message`, `name`, `toString` |
| Native errors | 20.5.5 to 20.5.7 | `EvalError`, `RangeError`, `ReferenceError`, `SyntaxError`, `TypeError`, `URIError`, `AggregateError` | the same as `Error` |

## Semantics

**r1.** MDN describes the group in one sentence: "These objects represent fundamental language constructs."[^1] The other built-ins depend on them. Every object inherits from `Object.prototype` unless its prototype is `null`, and every function inherits from `Function.prototype`. Example: [`members`](#members).

**r2.** "Nearly all objects in JavaScript are instances of Object; a typical object inherits properties (including methods) from Object.prototype, although these properties may be shadowed (a.k.a. overridden)."[^2] Most of the work on objects is in the 23 static functions, which take the object as an argument. The prototype has only a few methods, so that they do not clash with the properties of the data. See [[javascript/objects]] and [[javascript/property-descriptors]].

**r3.** `Function.prototype` gives every function `call`, `apply` and `bind`, which set the `this` value, and `toString`, which gives the source text. Its `Symbol.hasInstance` method is the default behavior of `instanceof`.[^3] The constructor `Function` makes a function from strings. See [[javascript/eval-and-the-function-constructor]]. Example: [`functions-and-symbols`](#functions-and-symbols).

**r4.** `Boolean` called as a function converts a value to a boolean. The Boolean constructor "creates and initializes a new Boolean object when called as a constructor."[^3] That object is a wrapper, and every object is truthy. See [[javascript/primitive-wrapper-objects]]. Example: [`boolean-object`](#boolean-object).

**r5.** The Symbol constructor "returns a new Symbol value when called as a function" and "is not intended to be used with the new operator."[^3] Its static properties are the well-known symbols and the registry functions `Symbol.for` and `Symbol.keyFor`. See [[javascript/well-known-symbols]]. Example: [`new-symbol`](#new-symbol).

**r6.** "Error objects are thrown when runtime errors occur. The Error object can also be used as a base object for user-defined exceptions."[^4] The native errors are subclasses. `TypeError` "is used to indicate an unsuccessful operation when none of the other NativeError objects are an appropriate indication of the failure cause."[^3] `EvalError` "is not currently used within this specification. This object remains for compatibility with previous editions of this specification."[^3] `AggregateError` holds a list of errors. Example: [`errors`](#errors).

**r7.** `Error.isError` tests the internal slot that only real error objects have.[^3] So it is true for an error from another realm, where `instanceof Error` can be false, and it is false for a plain object with `name` and `message`. Example: [`errors`](#errors).

**r8.** MDN: "You should avoid calling any of the Object.prototype methods directly from the instance".[^2] An object can have its own property with the same name, and an object with a `null` prototype has none of the methods. Use the static form, such as `Object.hasOwn(obj, key)`. Example: [`null-proto`](#null-proto).

## Examples

```javascript run id=errors
const e = new Error("read failed", { cause: "disk" });
console.log(e.name, e.message, e.cause);
console.log(e instanceof Error, Error.isError(e), Error.isError({ name: "Error", message: "x" }));
const agg = new AggregateError([new TypeError("a"), new RangeError("b")], "two");
console.log(agg.errors.map((x) => x.name), Object.getPrototypeOf(TypeError) === Error);
const other = require("node:vm").runInNewContext("new Error('from another realm')");
console.log(other instanceof Error, Error.isError(other));
```

```output for=errors
Error read failed disk
true true false
[ 'TypeError', 'RangeError' ] true
false true
```

The option `cause` keeps the error that caused this one. The plain object looks like an error, but `Error.isError` gives `false` (rule r7). The constructor `TypeError` inherits from `Error` (rule r6). The `vm` context is another realm with its own `Error`, so `instanceof` fails, but `Error.isError` works.

```javascript run id=functions-and-symbols
function add(a, b) {
  return a + b;
}
console.log(add.call(null, 1, 2), add.apply(null, [3, 4]), add.bind(null, 10)(5), add.length, add.name);
console.log(Symbol("a").description, Symbol.for("k") === Symbol.for("k"), Symbol.keyFor(Symbol.for("k")));
console.log(Boolean(0), Boolean(""), Boolean("0"), typeof Boolean(1));
console.log(Object.prototype.toString.call([]), Object.prototype.toString.call(null));
```

```output for=functions-and-symbols
3 7 15 2 add
a true k
false false true boolean
[object Array] [object Null]
```

`call` takes the arguments one by one, `apply` takes an array, and `bind` makes a new function with fixed first arguments (rule r3). `Symbol.for` gives the same symbol for the same key (rule r5). The string `"0"` is not empty, so it is truthy. `Object.prototype.toString` gives the tag of any value.

## Versions

`Error.isError` is new in the 17th edition (ECMAScript 2026) as 20.5.2.1.[^3] The 16th edition (ECMAScript 2025) does not have it in 20.5.2.[^5] The TC39 list of finished proposals gives 2026 as its year.[^6] `SuppressedError` is a part of Explicit Resource Management, which the list gives for 2027,[^6] so clause 20 of the 17th edition does not have it. MDN lists it with the other error objects.[^1] Node v24 has both `Error.isError` and `SuppressedError`.

## Pitfalls

### A Boolean object in a condition

```javascript wrong id=boolean-object
const flag = new Boolean(false);
if (flag) console.log("runs: the object is truthy");
console.log(typeof flag, flag.valueOf());
```

```output for=boolean-object
runs: the object is truthy
object false
```

`new Boolean(false)` is an object, and every object is truthy (rule r4). Use `Boolean(x)` with no `new`, or `!!x`, to get a boolean.

### new Symbol

```javascript fails id=new-symbol expect=exception:TypeError
console.log(typeof Symbol("id"));
new Symbol("id");
```

A symbol is a primitive, so there is no symbol object to construct (rule r5). Call `Symbol` as a function.

### A method of Object.prototype on a dictionary

```javascript fails id=null-proto expect=exception:TypeError
const dict = Object.create(null);
dict.key = 1;
console.log(Object.hasOwn(dict, "key"));
dict.hasOwnProperty("key");
```

The object has a `null` prototype, so it has no `hasOwnProperty` method (rule r8). `Object.hasOwn` works for every object.

## Comparison

In Java, `java.lang.Object` gives every object `equals`, `hashCode` and `toString`, and a class overrides them. In JavaScript, `Object.prototype` has few methods, and the tools for objects are static functions on `Object` (rule r2). A JavaScript program can throw any value, not only an error. `Error.isError` and `instanceof` test whether a caught value is an error (rule r7).

## References
