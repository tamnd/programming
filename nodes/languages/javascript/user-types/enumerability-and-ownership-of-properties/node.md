---
tag: H4WD2H
kind: feature
owner: javascript
slug: enumerability-and-ownership-of-properties
name: Enumerability and ownership of properties
aliases: [enumerable properties, non-enumerable properties, own properties, inherited properties, propertyIsEnumerable, Object.hasOwn, own enumerable string keys]
area: languages/javascript/user-types
level: core
summary: >-
  A JavaScript property is enumerable or not, has a string or symbol key, and is own or inherited. These three facts decide which properties for...in, Object.keys, spread and Object.assign see.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-enumerableownproperties", locator: "7.3.23 EnumerableOwnProperties"}
see_also: [javascript/property-descriptors, javascript/objects, prototype-based-programming, field-and-property]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Enumerability_and_ownership_of_properties
  title: MDN Web Docs, JavaScript Guide, Enumerability and ownership of properties
  locator: the introduction; Querying object properties; Traversing object properties
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/propertyIsEnumerable
  title: MDN Web Docs, JavaScript Reference, Object.prototype.propertyIsEnumerable()
  locator: the introduction; User-defined vs. built-in objects
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/keys
  title: MDN Web Docs, JavaScript Reference, Object.keys()
  locator: Description
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/abstract-operations.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 7
  locator: 7.3.23 EnumerableOwnProperties
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/fundamental-objects.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 20
  locator: 20.1.3.4 Object.prototype.propertyIsEnumerable, Note 1
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/hasOwn
  title: MDN Web Docs, JavaScript Reference, Object.hasOwn()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/assign
  title: MDN Web Docs, JavaScript Reference, Object.assign()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Accessible Object.prototype.hasOwnProperty, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/datamodel.html
  title: The Python Language Reference, Data model, Python 3.14
  locator: 3.2.11 Class instances; 3.2.11.1 Special attributes
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const parent = { inherited: 1 };
const o = Object.create(parent);
o.own = 2;
Object.defineProperty(o, "hidden", { value: 3 });
o[Symbol("tag")] = 4;
const seen = [];
for (const key in o) seen.push(key);
console.log(seen);
console.log(Object.keys(o), Object.getOwnPropertyNames(o));
console.log(Reflect.ownKeys(o));
```

```output for=basics
[ 'own', 'inherited' ]
[ 'own' ] [ 'own', 'hidden' ]
[ 'own', 'hidden', Symbol(tag) ]
```

| Visits | Enumerable own | Non-enumerable own | Enumerable inherited |
|---|---|---|---|
| `Object.keys`, `Object.values`, `Object.entries` | strings | no | no |
| `Object.getOwnPropertyNames` | strings | strings | no |
| `Object.getOwnPropertySymbols` | symbols | symbols | no |
| `Reflect.ownKeys` | yes | yes | no |
| `for...in` | strings | no | strings |
| `Object.assign`, object spread | yes | no | no |

No method in the table visits a non-enumerable inherited property.[^1]

## Semantics

**r1.** "Every property in JavaScript objects can be classified by three factors: Enumerable or non-enumerable; String or symbol; Own property or inherited property from the prototype chain."[^1] Example: [`basics`](#basics).

**r2.** "Enumerable properties are those properties whose internal enumerable flag is set to true, which is the default for properties created via simple assignment or via a property initializer. Properties defined via Object.defineProperty and such are not enumerable by default."[^1] "Most built-in properties are non-enumerable by default, while user-created object properties are often enumerable, unless explicitly designated otherwise."[^2] Example: [`builtins`](#builtins).

**r3.** "Ownership of properties is determined by whether the property belongs to the object directly and not to its prototype chain. All properties, enumerable or not, string or symbol, own or inherited, can be accessed with dot notation or bracket notation."[^1] The flags only change which properties the listing and copying methods visit.

**r4.** "Most iteration means (such as for...in loops and Object.keys ) only visit enumerable keys."[^1] `Object.keys` "returns an array whose elements are strings corresponding to the enumerable string-keyed property names found directly upon object . This is the same as iterating with a for...in loop, except that a for...in loop enumerates properties in the prototype chain as well."[^3] In ECMA-262, EnumerableOwnProperties, which `Object.keys`, `Object.values` and `Object.entries` use, keeps a key only when it is a String and "desc.[[Enumerable]] is true".[^4] Example: [`basics`](#basics).

**r5.** Three tests answer different questions. `propertyIsEnumerable` "returns a boolean indicating whether the specified property is this object's enumerable own property."[^2] ECMA-262: "This method does not consider objects in the prototype chain."[^5] `Object.hasOwn` "returns true if the specified object has the indicated property as its own property. If the property is inherited, or does not exist, the method returns false ."[^6] "Unlike the in operator, this method does not check for the specified property in the object's prototype chain."[^6] Example: [`tests`](#tests).

**r6.** `Object.assign` "copies all enumerable own properties from one or more source objects to a target object." "Both String and Symbol properties are copied."[^7] Object spread visits the same properties.[^1] Example: [`copy`](#copy).

## Examples

The three tests (rule r5).

```javascript run id=tests
const parent = { inherited: 1 };
const o = Object.create(parent);
o.own = 2;
Object.defineProperty(o, "hidden", { value: 3 });
for (const key of ["own", "hidden", "inherited", "missing"]) {
  console.log(key, Object.hasOwn(o, key), o.propertyIsEnumerable(key), key in o);
}
```

```output for=tests
own true true true
hidden true false true
inherited false false true
missing false false false
```

Built-in properties are not enumerable (rule r2).

```javascript run id=builtins
const arr = ["a"];
console.log(arr.propertyIsEnumerable(0), arr.propertyIsEnumerable("length"));
console.log(Object.keys(arr), Object.getOwnPropertyNames(arr));
console.log(Object.keys(Object.prototype), "toString" in {});
```

```output for=builtins
true false
[ '0' ] [ '0', 'length' ]
[] true
```

Copying sees own enumerable properties, with string and symbol keys (rule r6).

```javascript run id=copy
const tag = Symbol("tag");
const src = Object.create({ inherited: 1 });
src.own = 2;
src[tag] = 3;
Object.defineProperty(src, "hidden", { value: 4 });
const a = Object.assign({}, src);
const b = { ...src };
console.log(Object.keys(a), a[tag], Object.keys(b), b[tag]);
```

```output for=copy
[ 'own' ] 3 [ 'own' ] 3
```

## Versions

| Feature | Edition |
|---|---|
| `Object.hasOwn` | ECMAScript 2022[^8] |

The TC39 list of finished proposals gives 2022 as the publication year of Accessible `Object.prototype.hasOwnProperty`, the proposal that added `Object.hasOwn`.[^8]

## Pitfalls

### `for...in` visits inherited properties

```javascript wrong id=forin
const defaults = { color: "red" };
const options = Object.create(defaults);
options.size = 2;
const keys = [];
for (const key in options) keys.push(key);
console.log(keys, Object.keys(options));
```

```output for=forin
[ 'size', 'color' ] [ 'size' ]
```

`for...in` visits enumerable properties on the prototype chain too (rule r4). Use `Object.keys`, or test each key with `Object.hasOwn`.

### Calling `hasOwnProperty` on a null-prototype object

```javascript wrong id=nullproto
const dict = Object.create(null);
dict.a = 1;
try {
  dict.hasOwnProperty("a");
} catch (e) {
  console.log(e.name);
}
console.log(Object.hasOwn(dict, "a"));
```

```output for=nullproto
TypeError
true
```

An object made with `Object.create(null)` inherits nothing, so it has no `hasOwnProperty` method. MDN recommends `Object.hasOwn` "because it works for null -prototype objects and with objects that have overridden the inherited hasOwnProperty() method."[^6]

## Comparison

Python: "A class instance has a namespace implemented as a dictionary which is the first place in which attribute references are searched. When an attribute is not found there, and the instance’s class has an attribute by that name, the search continues with the class attributes."[^9] That dictionary is `__dict__`, "A dictionary or other mapping object used to store an object’s (writable) attributes."[^9] It plays the part of a JavaScript object's own properties, and the class plays the part of the prototype. JavaScript adds a per-property enumerable flag on top of that split (rule r2).

## References
