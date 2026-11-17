---
tag: 1NT8WW
kind: feature
owner: javascript
slug: property-descriptors
name: Property descriptors
aliases: [property attributes, writable, enumerable, configurable, data descriptor, accessor descriptor, Object.defineProperty, Object.getOwnPropertyDescriptor, Object.getOwnPropertyDescriptors, Object.defineProperties]
area: languages/javascript/user-types
level: core
summary: >-
  Each JavaScript property has attributes: a value or a getter and setter, plus writable, enumerable and configurable flags. A property descriptor is an object that reads or sets them.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-property-descriptor-specification-type", locator: "6.2.6 The Property Descriptor Specification Type; 6.1.7.1 Property Attributes"}
see_also: [javascript/defining-getters-and-setters, javascript/objects, immutable-object, field-and-property]
sources:
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 6
  locator: 6.1.7.1 Property Attributes, Table 3; 6.2.6 The Property Descriptor Specification Type
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/defineProperty
  title: MDN Web Docs, JavaScript Reference, Object.defineProperty()
  locator: Description; Creating a property; Writable attribute; Enumerable attribute
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/getOwnPropertyDescriptor
  title: MDN Web Docs, JavaScript Reference, Object.getOwnPropertyDescriptor()
  locator: the introduction; Return value
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/getOwnPropertyDescriptors
  title: MDN Web Docs, JavaScript Reference, Object.getOwnPropertyDescriptors()
  locator: the introduction
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/fundamental-objects.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 20
  locator: 20.1.2.3 Object.defineProperties; 20.1.2.4 Object.defineProperty
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Object.getOwnPropertyDescriptors, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/howto/descriptor.html
  title: Python HOWTOs, Descriptor Guide, Python 3.14
  locator: the introduction; Definition and introduction
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html
  title: The Java Language Specification, Java SE 25 Edition, chapter 8
  locator: 8.3.1.2 final Fields
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const o = { a: 1 };
Object.defineProperty(o, "b", { value: 2 });
console.log(Object.getOwnPropertyDescriptor(o, "a"));
console.log(Object.getOwnPropertyDescriptor(o, "b"));
console.log(Object.keys(o), o.b);
```

```output for=basics
{ value: 1, writable: true, enumerable: true, configurable: true }
{ value: 2, writable: false, enumerable: false, configurable: false }
[ 'a' ] 2
```

| Attribute | Kind of property | Meaning |
|---|---|---|
| `value` | data | the value |
| `writable` | data | assignment can change the value |
| `get`, `set` | accessor | the getter and setter functions |
| `enumerable` | both | `for...in`, `Object.keys` and spread see the property |
| `configurable` | both | the property can be deleted or redefined |

| Call | Does |
|---|---|
| `Object.getOwnPropertyDescriptor(obj, key)` | returns the descriptor of one own property |
| `Object.getOwnPropertyDescriptors(obj)` | returns the descriptors of all own properties |
| `Object.defineProperty(obj, key, descriptor)` | adds or changes one property |
| `Object.defineProperties(obj, descriptors)` | adds or changes several properties |

## Semantics

**r1.** ECMA-262: "A Property Descriptor is a Record with zero or more fields, where each field's name is an attribute name and its value is a corresponding attribute value". "A data Property Descriptor is one that includes any fields named either [[Value]] or [[Writable]]. An accessor Property Descriptor is one that includes any fields named either [[Get]] or [[Set]]. Any Property Descriptor may have fields named [[Enumerable]] and [[Configurable]]."[^1] In JavaScript code a descriptor is a plain object with the keys `value`, `writable`, `get`, `set`, `enumerable` and `configurable`.[^2] Example: [`basics`](#basics).

**r2.** The attributes, from Table 3 of ECMA-262.[^1] [[Writable]]: "If false, attempts by ECMAScript code to change the property's [[Value]] attribute using [[Set]] will not succeed." [[Enumerable]]: "If true, the property will be enumerated by a for-in enumeration". [[Configurable]]: "If false, attempts to delete the property, change it from a data property to an accessor property or from an accessor property to a data property, or make any changes to its attributes (other than replacing an existing [[Value]] or setting [[Writable]] to false) will fail." MDN adds that the enumerable attribute "defines whether the property is considered by Object.assign() or the spread operator".[^2]

**r3.** `Object.getOwnPropertyDescriptor` "returns an object describing the configuration of a specific property on a given object (that is, one directly present on an object and not in the object's prototype chain)." It returns "A property descriptor of the given property if it exists on the object, undefined otherwise." "The object returned is mutable but mutating it has no effect on the original property's configuration."[^3] `Object.getOwnPropertyDescriptors` "returns all own property descriptors of a given object."[^4] Example: [`read`](#read).

**r4.** The defaults depend on how the property is made. "Normal property addition through assignment creates properties which show up during property enumeration (for...in , Object.keys() , etc.), whose values may be changed and which may be deleted." But "By default, properties added using Object.defineProperty() are not writable, not enumerable, and not configurable."[^2] When `defineProperty` changes a property that exists, "If an attribute is absent from the new descriptor, the old descriptor attribute's value is kept".[^2] Example: [`basics`](#basics).

**r5.** A non-writable property "cannot be reassigned. Trying to write to a non-writable property doesn't change it and results in an error in strict mode."[^2] Example: [`readonly`](#readonly).

**r6.** For a non-configurable property, MDN: "It is not possible to change any attribute of a non-configurable accessor property, and it is not possible to switch between data and accessor property types. For data properties with writable: true , it is possible to modify the value and change the writable attribute from true to false ."[^2] So `writable: false` with `configurable: false` makes the value fixed for good. Example: [`locked`](#locked).

**r7.** `Object.defineProperty` "adds an own property and/or updates the attributes of an existing own property of an object", and `Object.defineProperties` does the same for several properties at once.[^5] It is not an assignment: "Object.defineProperty() uses the [[DefineOwnProperty]] internal method, instead of [[Set]] , so it does not invoke setters, even when the property is already present."[^2] Example: [`nosetter`](#nosetter).

**r8.** "A descriptor must be one of these two flavors; it cannot be both." "If a descriptor is both a data descriptor (because it has value or writable ) and an accessor descriptor (because it has get or set ), an exception is thrown."[^2] Example: [`mixed`](#mixed).

## Examples

Reading descriptors (rule r3).

```javascript run id=read
const o = { x: 1, get y() { return 2; } };
const d = Object.getOwnPropertyDescriptor(o, "x");
d.value = 99;
d.writable = false;
console.log(o.x, Object.getOwnPropertyDescriptor(o, "x").writable);
console.log(Object.getOwnPropertyDescriptor(o, "missing"));
const all = Object.getOwnPropertyDescriptors(o);
console.log(Object.keys(all), typeof all.y.get, "value" in all.y);
```

```output for=read
1 true
undefined
[ 'x', 'y' ] function false
```

A read-only property (rule r5).

```javascript run id=readonly
"use strict";
const settings = {};
Object.defineProperty(settings, "version", { value: 3, enumerable: true });
try {
  settings.version = 4;
} catch (e) {
  console.log(e.name);
}
console.log(settings.version, Object.keys(settings));
```

```output for=readonly
TypeError
3 [ 'version' ]
```

A non-configurable property (rule r6).

```javascript run id=locked
"use strict";
const o = {};
Object.defineProperty(o, "id", { value: 1, writable: true });
o.id = 2;
Object.defineProperty(o, "id", { writable: false });
const tries = [{ enumerable: true }, { writable: true }, { get() {} }].map((change) => {
  try {
    Object.defineProperty(o, "id", change);
    return "ok";
  } catch (e) {
    return e.name;
  }
});
console.log(tries, o.id);
```

```output for=locked
[ 'TypeError', 'TypeError', 'TypeError' ] 2
```

`defineProperty` does not call a setter (rule r7).

```javascript run id=nosetter
const log = [];
const obj = {
  set x(v) {
    log.push(v);
  },
};
obj.x = 1;
Object.defineProperty(obj, "x", { value: 2 });
console.log(log, obj.x);
```

```output for=nosetter
[ 1 ] 2
```

## Versions

| Feature | Edition |
|---|---|
| `Object.getOwnPropertyDescriptors` | ECMAScript 2017[^6] |

The TC39 list of finished proposals gives 2017 as the publication year of `Object.getOwnPropertyDescriptors`.[^6]

## Pitfalls

### Forgetting the `defineProperty` defaults

```javascript wrong id=hidden
const user = { name: "Ada" };
Object.defineProperty(user, "id", { value: 7 });
console.log(Object.keys(user), { ...user }, user.id);
Object.defineProperty(user, "role", {
  value: "admin",
  writable: true,
  enumerable: true,
  configurable: true,
});
console.log(Object.keys(user));
```

```output for=hidden
[ 'name' ] { name: 'Ada' } 7
[ 'name', 'role' ]
```

A property made with `defineProperty` is not writable, enumerable or configurable unless the descriptor says so (rule r4), so `Object.keys` and spread skip `id`. Give every flag you want set to true.

### Mixing `value` and `get`

```javascript wrong id=mixed
try {
  Object.defineProperty({}, "x", {
    value: 1,
    get() {
      return 2;
    },
  });
} catch (e) {
  console.log(e.name);
}
const ok = Object.defineProperty({}, "x", { get: () => 2 });
console.log(ok.x);
```

```output for=mixed
TypeError
2
```

A descriptor is either a data descriptor or an accessor descriptor (rule r8). Use `value` and `writable`, or `get` and `set`.

### A descriptor object that inherits fields

```javascript wrong id=inherited
const base = { enumerable: true };
const desc = Object.create(base);
desc.value = 1;
const o = Object.defineProperty({}, "x", desc);
console.log(Object.keys(o));
const plain = Object.create(null);
plain.value = 1;
console.log(Object.keys(Object.defineProperty({}, "x", plain)));
```

```output for=inherited
[ 'x' ]
[]
```

MDN: "These attributes are not necessarily the descriptor's own properties. Inherited properties will be considered as well."[^2] The `enumerable` field came from `base`. To keep the defaults, MDN suggests to "specify all options explicitly, or create a null -prototype object."[^2]

## Comparison

Python uses the word descriptor for a different idea: "a descriptor is an attribute value that has one of the methods in the descriptor protocol."[^7] The methods are `__get__`, `__set__` and `__delete__`. "Descriptors let objects customize attribute lookup, storage, and deletion."[^7] In Java, whether a field can change is part of its declaration: "A field can be declared final".[^8] In JavaScript the flags belong to each property, and code can read them and change them at run time (rules r3 and r6).

## References
