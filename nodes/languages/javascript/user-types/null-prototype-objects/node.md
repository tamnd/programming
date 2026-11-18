---
tag: 99G8EC
kind: feature
owner: javascript
slug: null-prototype-objects
name: Null-prototype objects
aliases: [null prototype, Object.create(null), objects without a prototype, dictionary objects, prototype pollution]
area: languages/javascript/user-types
level: advanced
summary: >-
  A null-prototype object has no prototype, so it inherits nothing from Object.prototype. It is a safe dictionary for arbitrary string keys, but it has no toString, hasOwnProperty or constructor.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/fundamental-objects.html#sec-object.create", locator: "20.1.2.2 Object.create; 20.1.2.13 Object.groupBy"}
see_also: [javascript/creating-and-changing-prototype-chains, javascript/inheritance-and-the-prototype-chain, javascript/maps, javascript/querying-object-properties]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object
  title: MDN Web Docs, JavaScript Reference, Object
  locator: null-prototype objects
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/fundamental-objects.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 20
  locator: 20.1.2.2 Object.create; 20.1.2.13 Object.groupBy
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 13
  locator: 13.2.5.6 PropertyDefinitionEvaluation
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/proto
  title: MDN Web Docs, JavaScript Reference, Object.prototype.__proto__
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/hasOwn
  title: MDN Web Docs, JavaScript Reference, Object.hasOwn()
  locator: Description
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Array Grouping, Expected Publication Year
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Map
  title: MDN Web Docs, JavaScript Reference, Map
  locator: Objects vs. Maps
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/stdtypes.html
  title: The Python Standard Library, Python 3.14, Built-in Types
  locator: Mapping Types, dict
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const dict = Object.create(null);
dict.a = 1;
const dict2 = { __proto__: null, b: 2 };
console.log(Object.getPrototypeOf(dict), Object.getPrototypeOf(dict2));
console.log("toString" in dict, "toString" in {});
console.log(dict.a, Object.keys(dict2), Object.hasOwn(dict, "a"));
```

```output for=basics
null null
false true
1 [ 'b' ] true
```

| Test | Plain object `{}` | Null-prototype object |
|---|---|---|
| `'toString' in o` | true | false |
| `o.hasOwnProperty('a')` | works | TypeError |
| `` `${o}` `` | `'[object Object]'` | TypeError |
| `o.constructor` | `Object` | undefined |
| `Object.hasOwn(o, 'a')`, `Object.keys(o)` | works | works |

## Semantics

**r1.** MDN: "Almost all objects in JavaScript ultimately inherit from Object.prototype". But "you may create null -prototype objects using Object.create(null)", or with a literal whose prototype key is null, and "You can also change the prototype of an existing object to null by calling Object.setPrototypeOf(obj, null) ."[^1] ECMA-262 allows null in `Object.create`: "If O is not an Object and O is not null, throw a TypeError exception."[^2] The literal key sets the prototype "If propValue is an Object or propValue is null".[^3] Example: [`basics`](#basics).

**r2.** "An object with a null prototype can behave in unexpected ways, because it doesn't inherit any object methods from Object.prototype ."[^1] Methods such as `toString`, `valueOf` and `hasOwnProperty` are not there, and `constructor` is undefined. The `__proto__` accessor is missing too: on such an object "the value is always undefined regardless of the object's actual [[Prototype]]".[^4] Example: [`missing`](#missing).

**r3.** "In practice, objects with null prototype are usually used as a cheap substitute for maps."[^1] Keys such as `toString` or `constructor` are then not found by accident. MDN: "In such case, the addition of any method should be done cautiously, as they can be confused with the other key-value pairs stored as data."[^1] Example: [`counts`](#counts).

**r4.** "Making your object not inherit from Object.prototype also prevents prototype pollution attacks. If a malicious script adds a property to Object.prototype , it will be accessible on every object in your program, except objects that have null prototype."[^1] Example: [`pollution`](#pollution).

**r5.** Use the static functions, which work on any object. MDN recommends `Object.hasOwn` "because it works for null -prototype objects and with objects that have overridden the inherited hasOwnProperty() method."[^5] Example: [`basics`](#basics).

**r6.** Some built-in APIs make null-prototype objects, "especially those that use objects as ad hoc key-value collections", such as "The return value of Object.groupBy()" and "The groups and indices.groups properties of the result of RegExp.prototype.exec()".[^1] ECMA-262: "The return value of groupBy is an object that does not inherit from %Object.prototype%."[^2] Example: [`builtins`](#builtins).

## Examples

What a null-prototype object lacks (rule r2).

```javascript run id=missing
const o = Object.create(null);
o.a = 1;
for (const test of [() => `${o}`, () => o.hasOwnProperty("a"), () => o.valueOf()]) {
  try {
    test();
  } catch (e) {
    console.log(e.name);
  }
}
console.log(o.constructor, o.__proto__);
```

```output for=missing
TypeError
TypeError
TypeError
undefined undefined
```

Built-in functions that return null-prototype objects (rule r6).

```javascript run id=builtins
const byParity = Object.groupBy([1, 2, 3], (n) => (n % 2 ? "odd" : "even"));
const { groups } = /(?<year>\d{4})/.exec("in 2026");
console.log(Object.getPrototypeOf(byParity), Object.getPrototypeOf(groups));
console.log(Object.keys(byParity), groups.year);
```

```output for=builtins
null null
[ 'odd', 'even' ] 2026
```

## Versions

| Feature | Edition |
|---|---|
| `Object.groupBy` | ECMAScript 2024[^6] |

The TC39 list of finished proposals gives 2024 for Array Grouping, the proposal that added `Object.groupBy`.[^6]

## Pitfalls

### Using a plain object as a dictionary

```javascript wrong id=counts
const words = ["apple", "constructor", "apple"];
const plain = {};
for (const w of words) plain[w] = (plain[w] || 0) + 1;
console.log(plain.apple, typeof plain.constructor);
const safe = Object.create(null);
for (const w of words) safe[w] = (safe[w] || 0) + 1;
console.log(safe.apple, safe.constructor);
```

```output for=counts
2 string
2 1
```

`plain.constructor` starts as the inherited `Object` function, so `plain[w] || 0` takes that function and `+ 1` makes a string (rule r3). MDN on Map: "An Object has a prototype, so it contains default keys that could collide with your own keys if you're not careful."[^7] Use a null-prototype object or a `Map`.

### Prototype pollution

```javascript wrong id=pollution
const user = {};
const safeUser = Object.create(null);
Object.prototype.authenticated = true;
console.log(user.authenticated, safeUser.authenticated);
delete Object.prototype.authenticated;
```

```output for=pollution
true undefined
```

A property added to `Object.prototype` shows up on every ordinary object (rule r4). The null-prototype object does not see it.

## Comparison

A Python `dict` keeps its keys apart from its methods. `key in d` will "Return True if d has a key key, else False", and `d[key]` "Raises a KeyError if key is not in the map."[^8] So `'keys' in {}` is False in Python. A plain JavaScript object mixes the two, because `in` and property reads also look at `Object.prototype` (rule r2). A null-prototype object or a `Map` gives the Python behavior.

## References
