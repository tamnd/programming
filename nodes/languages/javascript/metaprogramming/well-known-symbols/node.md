---
tag: JSFGVV
kind: feature
owner: javascript
slug: well-known-symbols
name: Well-known symbols
aliases: [Symbol.iterator, Symbol.asyncIterator, Symbol.toPrimitive, Symbol.toStringTag, Symbol.hasInstance, Symbol.species, Symbol.isConcatSpreadable, Symbol.unscopables, Symbol.match, Symbol.split, "@@iterator", protocol symbols]
area: languages/javascript/metaprogramming
level: advanced
summary: >-
  The well-known symbols, such as Symbol.iterator and Symbol.toPrimitive, are the keys of methods that the language calls. An object defines one of them to change for...of, a conversion or instanceof.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-well-known-symbols", locator: "6.1.5.1 Well-Known Symbols (Table 1: Well-known Symbols)"}
see_also: [javascript/iterables, javascript/iterators, javascript/async-iteration, javascript/meta-programming, javascript/reflection]
sources:
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 6
  locator: 6.1.5.1 Well-Known Symbols; Table 1
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Symbol
  title: MDN Web Docs, JavaScript Reference, Symbol
  locator: Well-known Symbols; Static properties
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Symbol/toPrimitive
  title: MDN Web Docs, JavaScript Reference, Symbol.toPrimitive
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Symbol/hasInstance
  title: MDN Web Docs, JavaScript Reference, Symbol.hasInstance
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Symbol/species
  title: MDN Web Docs, JavaScript Reference, Symbol.species
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://262.ecma-international.org/6.0/index.html
  title: ECMAScript 2015 Language Specification (ECMA-262, 6th edition)
  locator: 19.4.2 Properties of the Symbol Constructor
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/datamodel.html
  title: The Python Language Reference, Python 3.14, Data model
  locator: 3.3.7. Emulating container types
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=range
class Range {
  constructor(from, to) {
    this.from = from;
    this.to = to;
  }
  *[Symbol.iterator]() {
    for (let i = this.from; i <= this.to; i++) yield i;
  }
  get [Symbol.toStringTag]() {
    return "Range";
  }
  [Symbol.toPrimitive](hint) {
    return hint === "number" ? this.to - this.from + 1 : `${this.from}..${this.to}`;
  }
}
const r = new Range(1, 4);
console.log([...r], Object.prototype.toString.call(r));
console.log(+r, `${r}`);
```

```output for=range
[ 1, 2, 3, 4 ] [object Range]
4 1..4
```

The class defines three methods with symbol keys. The spread syntax calls `[Symbol.iterator]`, `Object.prototype.toString` reads `[Symbol.toStringTag]`, and the conversions call `[Symbol.toPrimitive]`.

| Symbol | Kind of value | Used by |
|---|---|---|
| `Symbol.iterator` | method | `for...of`, spread, destructuring |
| `Symbol.asyncIterator` | method | `for await...of` |
| `Symbol.toPrimitive` | method | conversion to a number or a string |
| `Symbol.toStringTag` | string | `Object.prototype.toString` |
| `Symbol.hasInstance` | method | `instanceof` |
| `Symbol.species` | constructor | `map`, `filter` and other methods that make a copy |
| `Symbol.isConcatSpreadable` | boolean | `Array.prototype.concat` |
| `Symbol.match`, `Symbol.matchAll`, `Symbol.replace`, `Symbol.search`, `Symbol.split` | method | the string methods with the same names |
| `Symbol.unscopables` | object | the `with` statement |

## Semantics

**r1.** "Well-known symbols are built-in Symbol values that are explicitly referenced by algorithms of this specification. They are typically used as the keys of properties whose values serve as extension points of a specification algorithm."[^1] Table 1 of the 17th edition lists 13 of them.[^1] Example: [`range`](#range).

**r2.** Each well-known symbol is a property of the `Symbol` constructor. MDN: "All static properties of the Symbol constructor are Symbols themselves, whose values are constant across realms."[^2] The specification: "Unless otherwise specified, well-known symbols values are shared by all realms (9.3)."[^1] So `Symbol.iterator` in another realm, such as an iframe, is the same value. Example: [`list`](#list).

**r3.** A program cannot make a well-known symbol. `Symbol("iterator")` makes a new symbol with the same description, and the language does not look for it. Example: [`own-symbol`](#own-symbol).

**r4.** `Symbol.iterator` is "A method that returns the default iterator for an object. Called by the semantics of the for-of statement."[^1] `Symbol.asyncIterator` does the same for "the for-await-of statement."[^1] See [[javascript/iterables]] and [[javascript/async-iteration]].

**r5.** `Symbol.toPrimitive` is "A method that converts an object to a corresponding primitive value. Called by the ToPrimitive abstract operation."[^1] The method gets a hint, which is the string `number`, `string` or `default`. A numeric conversion, such as the one for `*`, gives `number`. A string conversion, such as the one for a template literal, gives `string`. The `+` operator gives `default`.[^3] The method "must return a primitive, or a TypeError is thrown."[^3] Examples: [`hints`](#hints), [`object-result`](#object-result).

**r6.** `Symbol.toStringTag` is "A String valued property that is used in the creation of the default string description of an object. Accessed by the built-in method Object.prototype.toString."[^1] It changes only the `[object Tag]` text. Example: [`tag-only`](#tag-only).

**r7.** `Symbol.hasInstance` is "A method that determines if a constructor object recognizes an object as one of the constructor's instances. Called by the semantics of the instanceof operator."[^1] "The instanceof operator looks up this symbol on its right-hand operand for the method used to determine if the constructor object recognizes an object as its instance."[^4] Example: [`instance-and-spread`](#instance-and-spread).

**r8.** `Symbol.species` is "A function valued property that is the constructor function that is used to create derived objects."[^1] "Methods that create copies of an object may look up this symbol on the object for the constructor function to use when creating the copy."[^5] Example: [`species`](#species).

**r9.** `Symbol.isConcatSpreadable` is "A Boolean valued property that if true indicates that an object should be flattened to its array elements by Array.prototype.concat."[^1] The regular expression symbols let an object act as the pattern of a string method: `Symbol.split` is "Called by the String.prototype.split method."[^1] Examples: [`instance-and-spread`](#instance-and-spread), [`list`](#list).

## Examples

```javascript run id=list
const names = Object.getOwnPropertyNames(Symbol).filter((k) => typeof Symbol[k] === "symbol");
console.log(names.join(" "));
console.log(names.length, Symbol.iterator.description, typeof Symbol.iterator);
const words = { [Symbol.split](s) { return s.split(" ").reverse(); } };
console.log("a b c".split(words));
```

```output for=list
asyncIterator hasInstance isConcatSpreadable iterator match matchAll replace search species split toPrimitive toStringTag unscopables dispose asyncDispose
15 Symbol.iterator symbol
[ 'c', 'b', 'a' ]
```

Node v24 has the 13 symbols of Table 1 and two more, `Symbol.dispose` and `Symbol.asyncDispose`. MDN lists them for the `using` and `await using` declarations.[^2] Table 1 of the 17th edition does not list them.[^1] The object `words` has a `[Symbol.split]` method, so `split` calls it in place of a normal split (rule r9).

```javascript run id=hints
const money = {
  amount: 5,
  [Symbol.toPrimitive](hint) {
    console.log("hint:", hint);
    return hint === "string" ? `$${this.amount}` : this.amount;
  },
};
console.log(money * 2);
console.log(`${money}`);
console.log(money + "");
```

```output for=hints
hint: number
10
hint: string
$5
hint: default
5
```

Each conversion gives a different hint (rule r5). The `+` operator gives `default`, because it can add numbers or join strings.

```javascript run id=instance-and-spread
class Even {
  static [Symbol.hasInstance](n) {
    return Number.isInteger(n) && n % 2 === 0;
  }
}
console.log(4 instanceof Even, 3 instanceof Even);
const arrayLike = { length: 2, 0: "x", 1: "y", [Symbol.isConcatSpreadable]: true };
console.log(["a"].concat(arrayLike));
```

```output for=instance-and-spread
true false
[ 'a', 'x', 'y' ]
```

The static method decides the result of `instanceof`, so the test works for numbers, which are not objects of the class (rule r7). The flag makes `concat` add the elements of the object, not the object (rule r9).

```javascript run id=species
class MyArray extends Array {
  static get [Symbol.species]() {
    return Array;
  }
}
const a = MyArray.from([1, 2, 3]);
const doubled = a.map((x) => x * 2);
console.log(a instanceof MyArray, doubled instanceof MyArray, doubled);
```

```output for=species
true false [ 2, 4, 6 ]
```

`map` reads `[Symbol.species]` from the constructor of `a`, and makes a plain `Array` (rule r8). MDN warns: "Avoid relying on it if possible."[^5]

## Versions

ECMAScript 2015, the 6th edition, added symbols. Its section "19.4.2 Properties of the Symbol Constructor" has 11 well-known symbols, from `Symbol.hasInstance` to `Symbol.unscopables`, and no `Symbol.asyncIterator` or `Symbol.matchAll`.[^6] The 17th edition (ECMAScript 2026) has 13 in Table 1.[^1] Earlier editions wrote the names in the form `@@iterator`: "Previous editions of this specification used a notation of the form @@name, where the current edition would use %Symbol.name%."[^1] Node v24 also has `Symbol.dispose` and `Symbol.asyncDispose` (see [`list`](#list)).

## Pitfalls

### A new symbol with the same name

```javascript wrong id=own-symbol
const key = Symbol("iterator");
const obj = { *[key]() { yield 1; } };
console.log(key === Symbol.iterator, Symbol.for("Symbol.iterator") === Symbol.iterator);
try {
  for (const x of obj) console.log(x);
} catch (e) {
  console.log(e.constructor.name, e.message);
}
```

```output for=own-symbol
false false
TypeError obj is not iterable
```

`Symbol("iterator")` and `Symbol.for(...)` make other symbols (rule r3). Use the property `Symbol.iterator` itself as the key.

### A tag is not a type

```javascript wrong id=tag-only
const tagged = { [Symbol.toStringTag]: "Config" };
console.log(String(tagged), `${tagged}`);
console.log(JSON.stringify(tagged), Object.keys(tagged));
```

```output for=tag-only
[object Config] [object Config]
{} []
```

The tag changes the text of `toString`, and nothing else (rule r6). A symbol key is not in `JSON.stringify` or `Object.keys`. Do not use the tag to test the type of an object, because any object can set it.

### A conversion that returns an object

```javascript fails id=object-result expect=exception:TypeError
const bad = {
  [Symbol.toPrimitive]() {
    return {};
  },
};
console.log(`${bad}`);
```

The method must return a primitive (rule r5). Return a number, a string, a boolean, a bigint, a symbol, `null` or `undefined`.

## Comparison

Python has special method names for the same work. For example, `__iter__` is a special method, and "This method is called when an iterator is required for a container."[^7] A Python special name is a normal attribute name with two underscores at each end. A JavaScript well-known symbol is a value that no other code can make (rule r3), so a protocol method cannot clash with a normal property name. For the other hooks, see [[javascript/meta-programming]].

## References
