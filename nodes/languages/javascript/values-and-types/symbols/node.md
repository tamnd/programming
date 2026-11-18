---
tag: X1GD42
kind: feature
owner: javascript
slug: symbols
name: Symbols
aliases: [Symbol, "Symbol()", "Symbol.for", "Symbol.keyFor", symbol description, well-known symbols, symbol keys, global symbol registry]
area: languages/javascript/values-and-types
level: core
summary: >-
  Symbol() makes a unique primitive that can be a property key. Symbol.for() shares one symbol per string key. Symbol keys are hidden from for...in, Object.keys and JSON.stringify.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-language-types-symbol-type", locator: "6.1.5 The Symbol Type"}
see_also: [javascript/primitive-values, javascript/primitive-wrapper-objects, javascript/javascript-data-types-and-data-structures, javascript/iterables, javascript/relational-operators, javascript/weakmap-and-weakset, python/dictionaries]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-language-types-symbol-type
    title: ECMAScript 2026 Language Specification
    locator: 6.1.5 The Symbol Type; 6.1.5.1 Well-Known Symbols
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Symbol
    title: Symbol, MDN
    locator: Description; Shared Symbols in the global Symbol registry; Well-known Symbols; Finding Symbol properties on objects; Symbol type conversions; Symbols and for...in iteration; Symbols and JSON.stringify()
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Data_structures
    title: JavaScript data types and data structures, MDN
    locator: Symbol type
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/datamodel.html
    title: Data model, The Python Language Reference
    locator: 3.1 Objects, values and types
    accessed: 2026-11-18
  - url: https://docs.python.org/3/library/stdtypes.html
    title: Built-in Types, The Python Standard Library
    locator: Mapping Types (dict)
    accessed: 2026-11-18
---

## Synopsis

| Call | Result |
|---|---|
| `Symbol()`, `Symbol('desc')` | a symbol that is different from all others |
| `sym.description` | the description string, or `undefined` |
| `Symbol.for('key')` | the registry symbol for `'key'`, made at the first call |
| `Symbol.keyFor(sym)` | the registry key of `sym`, or `undefined` |
| `Object.getOwnPropertySymbols(obj)` | the own symbol keys of `obj` |
| `Symbol.iterator`, `Symbol.hasInstance`, ... | the well-known symbols |

"The Symbol type is the set of all non-String values that may be used as the key of an Object property". "Each Symbol is unique and immutable."[^1]

```javascript run id=synopsis
const a = Symbol("id");
const b = Symbol("id");
console.log(typeof a, a === b, a.description, a.toString());
console.log(Symbol.for("app") === Symbol.for("app"), Symbol.keyFor(Symbol.for("app")), Symbol.keyFor(a));
```

```output for=synopsis
symbol false id Symbol(id)
true app undefined
```

## Semantics

**r1. Each call makes a symbol.** A call to `Symbol('foo')` does not convert the string to a symbol. Each call makes a different symbol, and the string is only a description.[^2] The specification keeps the description in an immutable internal slot, which holds a String or undefined.[^1] `sym.description` reads it.[^2] Example: [`synopsis`](#synopsis).

**r2. No `new`.** `new Symbol()` throws a TypeError. Use `Object(sym)` for a wrapper object.[^2] See [[javascript/primitive-wrapper-objects]].

**r3. Keys that do not clash.** "The purpose of symbols is to create unique property keys that are guaranteed not to clash with keys from other code."[^3] Example: [`keys`](#keys).

**r4. Hidden from string-key APIs.** "Symbols are not enumerable in for...in iterations." `Object.getOwnPropertyNames()` "will not return Symbol object properties", and `Object.getOwnPropertySymbols()` does.[^2] "Symbol-keyed properties will be completely ignored when using JSON.stringify()".[^2] Example: [`keys`](#keys).

**r5. The global registry.** "Symbol.for(tokenString) takes a string key and returns a symbol value from the registry, while Symbol.keyFor(symbolValue) takes a symbol value and returns the string key corresponding to it."[^2] Registry symbols "behave almost exactly like the strings they wrap", so "they are not guaranteed to be unique and are not garbage collectable."[^2] Example: [`synopsis`](#synopsis).

**r6. Well-known symbols.** The static properties of `Symbol` are well-known symbols, and built-in operations use them as protocols.[^2] They "are typically used as the keys of properties whose values serve as extension points of a specification algorithm."[^1] Example: [`well-known`](#well-known).

**r7. Conversions.** "When trying to convert a Symbol to a number, a TypeError will be thrown". `Symbol('foo') + 'bar'` also throws, but `String(sym)` works.[^2] Example: [`concat`](#concat).

## Examples

The examples run with Node.js 24.

A symbol key next to string keys (rules r3 and r4):

```javascript run id=keys
const secret = Symbol("secret");
const obj = { name: "box", [secret]: 42 };
console.log(obj[secret], Object.keys(obj), JSON.stringify(obj));
console.log(Object.getOwnPropertySymbols(obj), Reflect.ownKeys(obj));
for (const k in obj) console.log("for...in:", k);
```

```output for=keys
42 [ 'name' ] {"name":"box"}
[ Symbol(secret) ] [ 'name', Symbol(secret) ]
for...in: name
```

Well-known symbols change built-in operations (rule r6):

```javascript run id=well-known
class Range {
  constructor(n) {
    this.n = n;
  }
  *[Symbol.iterator]() {
    for (let i = 0; i < this.n; i++) yield i;
  }
  get [Symbol.toStringTag]() {
    return "Range";
  }
}
const r = new Range(3);
console.log([...r], String(r));
```

```output for=well-known
[ 0, 1, 2 ] [object Range]
```

`String(sym)` works and `+` throws (rule r7):

```javascript fails id=concat expect=exception:TypeError
const sym = Symbol("foo");
console.log(String(sym));
console.log(sym + "bar");
```

## Pitfalls

### The description is not the identity

`Symbol('id') === Symbol('id')` is false (rule r1). Keep the symbol in a variable, or use `Symbol.for` to share it.

### Symbol keys are not private

`Object.getOwnPropertySymbols()` and `Reflect.ownKeys()` list them ([`keys`](#keys)). Any code that has the object can read these keys.

### Symbol keys vanish in JSON

`JSON.stringify` drops symbol-keyed properties (rule r4).[^2]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| a unique value | `Symbol()`[^2] | a fresh object; the `is` operator "compares the identity of two objects"[^4] |
| key types | strings and symbols[^3] | "almost arbitrary values", but not values that are "not hashable"[^5] |
| hidden keys | symbol keys skip `for...in` and JSON[^2] | no hidden kind of key |

Python dictionary keys can be of almost any type, so code that needs a key that does not clash can use a fresh object.[^5] JavaScript object keys are only strings and symbols, so the symbol fills this role (rule r3). See [[python/dictionaries]].

## References
