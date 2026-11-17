---
tag: 23PBFE
kind: feature
owner: javascript
slug: querying-object-properties
name: Querying object properties
aliases: [in operator, Object.hasOwn, hasOwnProperty, checking if a property exists, property existence test, optional chaining, undefined properties]
area: languages/javascript/user-types
level: core
summary: >-
  JavaScript tests for a property with in, which also searches the prototype chain, or Object.hasOwn, which checks own properties only. Optional chaining reads a property that may be missing.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-relational-operators", locator: "13.10 Relational Operators; 7.3.11 HasProperty; 7.3.12 HasOwnProperty"}
see_also: [javascript/enumerability-and-ownership-of-properties, javascript/objects, javascript/property-descriptors, prototype-based-programming]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/in
  title: MDN Web Docs, JavaScript Reference, in
  locator: the introduction; Description; Using the in operator with deleted or undefined properties
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/abstract-operations.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 7
  locator: 7.3.11 HasProperty; 7.3.12 HasOwnProperty
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/hasOwn
  title: MDN Web Docs, JavaScript Reference, Object.hasOwn()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 13
  locator: 13.10.1 Runtime Semantics, Evaluation
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/fundamental-objects.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 20
  locator: 20.1.2.14 Object.hasOwn
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Optional_chaining
  title: MDN Web Docs, JavaScript Reference, Optional chaining (?.)
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Optional Chaining, Accessible Object.prototype.hasOwnProperty, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/functions.html
  title: The Python Standard Library, Python 3.14, Built-in Functions
  locator: hasattr
  accessed: 2026-11-18
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification, language version go1.27
  locator: Index expressions
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const user = { name: "Ada", nickname: undefined };
console.log("name" in user, "toString" in user, "age" in user);
console.log(Object.hasOwn(user, "name"), Object.hasOwn(user, "toString"));
console.log(user.nickname === undefined, "nickname" in user);
console.log(user.address?.city);
```

```output for=basics
true true false
true false
true true
undefined
```

| Test | Own properties | Inherited properties | A property set to `undefined` |
|---|---|---|---|
| `key in obj` | true | true | true |
| `Object.hasOwn(obj, key)` | true | false | true |
| `obj.key !== undefined` | true | true | false |
| `obj.propertyIsEnumerable(key)` | true if enumerable | false | true if enumerable |

## Semantics

**r1.** "The in operator returns true if the specified property is in the specified object or its prototype chain."[^1] In ECMA-262 it calls HasProperty, which "is used to determine whether an object has a property with the specified property key. The property may be either own or inherited."[^2] Example: [`basics`](#basics).

**r2.** `Object.hasOwn` "returns true if the specified object has the indicated property as its own property. If the property is inherited, or does not exist, the method returns false ."[^3] It uses HasOwnProperty, which "is used to determine whether an object has an own property with the specified property key."[^2] "Object.hasOwn() is intended as a replacement for Object.prototype.hasOwnProperty() ."[^3] Example: [`basics`](#basics).

**r3.** Reading a missing property gives `undefined`, but so does reading a property whose value is `undefined`. MDN: "A property may be present in an object but have value undefined ." "To make in return false after a property is added, use the delete operator instead of setting that property's value to undefined ."[^1] Example: [`undef`](#undef).

**r4.** The right side of `in` must be an object. The steps say "If rVal is not an Object, throw a TypeError exception."[^4] The steps of `Object.hasOwn` begin "Let obj be ? ToObject(O).", so it works on a string.[^5] Example: [`primitive`](#primitive).

**r5.** For arrays, `in` tests an index, not a value. "The in operator cannot be used to search for values in other collections. To test if a certain value exists in an array, use Array.prototype.includes() ."[^1] "The in operator will return false for empty array slots, even if accessing it directly returns undefined ."[^1] Example: [`sparse`](#sparse).

**r6.** Optional chaining reads through a value that may be missing. "If the object accessed or function called using this operator is undefined or null , the expression short circuits and evaluates to undefined instead of throwing an error."[^6] Example: [`optional`](#optional).

## Examples

A property set to `undefined` is still there (rule r3).

```javascript run id=undef
const o = { a: undefined };
console.log("a" in o, o.a, o.b);
o.a = 1;
delete o.a;
console.log("a" in o);
```

```output for=undef
true undefined undefined
false
```

Empty slots in an array (rule r5).

```javascript run id=sparse
const empties = new Array(3);
const filled = new Array(3).fill(undefined);
console.log(empties[2], 2 in empties, 2 in filled);
```

```output for=sparse
undefined false true
```

Reading a path that may be missing (rule r6).

```javascript run id=optional
const order = { customer: { name: "Ada" } };
console.log(order.customer?.name, order.shipping?.city, order.notify?.());
try {
  order.shipping.city;
} catch (e) {
  console.log(e.name);
}
```

```output for=optional
Ada undefined undefined
TypeError
```

## Versions

| Feature | Edition |
|---|---|
| optional chaining `?.` | ECMAScript 2020[^7] |
| `Object.hasOwn` | ECMAScript 2022[^7] |

The TC39 list of finished proposals gives 2020 for Optional Chaining and 2022 for Accessible `Object.prototype.hasOwnProperty`, the proposal that added `Object.hasOwn`.[^7]

## Pitfalls

### Testing a property by its truth value

```javascript wrong id=truthy
const stock = { apples: 0 };
console.log(stock.apples ? "have" : "missing");
console.log("apples" in stock, Object.hasOwn(stock, "apples"));
```

```output for=truthy
missing
true true
```

`0` is falsy, so the test reports a property that exists as missing. MDN gives the same warning for `&&` chains, where a falsy value "that's not null or undefined , such as 0" stops the chain.[^6] Use `in` or `Object.hasOwn` to ask whether the property is there (rules r1 and r2).

### Using `in` on a primitive

```javascript wrong id=primitive
try {
  console.log("length" in "abc");
} catch (e) {
  console.log(e.name);
}
console.log("length" in new String("abc"), Object.hasOwn("abc", "length"));
```

```output for=primitive
TypeError
true true
```

A string literal is not an object, so `in` throws (rule r4).

### Using `in` to search an array for a value

```javascript wrong id=values
const trees = ["redwood", "bay"];
console.log("bay" in trees, 1 in trees, trees.includes("bay"));
```

```output for=values
false true true
```

`"bay" in trees` asks for a property named `bay`. Use `includes` to search the values (rule r5).

## Comparison

Python's `hasattr`: "The result is True if the string is the name of one of the object’s attributes, False if not. (This is implemented by calling getattr(object, name) and seeing whether it raises an AttributeError or not.)"[^8] Like `in`, it finds attributes from the class too. Go has no general property test, but a map index in the special form `v, ok := a[x]` "yields an additional untyped boolean value. The value of ok is true if the key x is present in the map, and false otherwise."[^9] JavaScript has both kinds of test: `in` for the whole prototype chain and `Object.hasOwn` for the object alone (rules r1 and r2).

## References
