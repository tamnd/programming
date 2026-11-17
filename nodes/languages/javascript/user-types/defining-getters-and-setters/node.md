---
tag: DBMA21
kind: feature
owner: javascript
slug: defining-getters-and-setters
name: Defining getters and setters
aliases: [get syntax, set syntax, accessor properties, getters in JavaScript, setters in JavaScript, computed properties, lazy getters, getter-only property]
area: languages/javascript/user-types
level: core
summary: >-
  A getter runs a function when a property is read, and a setter runs one when it is assigned. JavaScript defines them with get and set in an object literal or with Object.defineProperty.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-functions-and-classes.html#sec-method-definitions", locator: "15.4 Method Definitions; 6.1.7.1 Property Attributes"}
see_also: [javascript/defining-methods, javascript/objects, getters-and-setters, field-and-property]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/get
  title: MDN Web Docs, JavaScript Reference, get
  locator: the introduction; Syntax; Description; Smart / self-overwriting / lazy getters
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/set
  title: MDN Web Docs, JavaScript Reference, set
  locator: the introduction; Syntax; Description
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 6
  locator: 6.1.7.1 Property Attributes, Table 3
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Errors/Getter_only
  title: MDN Web Docs, JavaScript Reference, TypeError, setting getter-only property
  locator: What went wrong?
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/defineProperty
  title: MDN Web Docs, JavaScript Reference, Object.defineProperty()
  locator: Description
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/abstract-operations.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 7
  locator: 7.3.25 CopyDataProperties
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/getOwnPropertyDescriptors
  title: MDN Web Docs, JavaScript Reference, Object.getOwnPropertyDescriptors()
  locator: Examples, Creating a shallow copy
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/functions.html
  title: The Python Standard Library, Python 3.14, Built-in Functions
  locator: property
  accessed: 2026-11-18
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/properties
  title: C# Programming Guide, Properties
  locator: the introduction
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const temp = {
  celsius: 20,
  get fahrenheit() {
    return (this.celsius * 9) / 5 + 32;
  },
  set fahrenheit(f) {
    this.celsius = ((f - 32) * 5) / 9;
  },
};
console.log(temp.fahrenheit);
temp.fahrenheit = 212;
console.log(temp.celsius, Object.keys(temp));
```

```output for=basics
68
100 [ 'celsius', 'fahrenheit' ]
```

| Form | Runs |
|---|---|
| `get name() { ... }` | on each read of `obj.name` |
| `set name(value) { ... }` | on each assignment to `obj.name`, with the value |
| `get [expression]() { ... }` | a getter with a computed name |
| `Object.defineProperty(obj, key, { get, set })` | adds an accessor to an existing object |
| `delete obj.name` | removes the accessor |

## Semantics

**r1.** "The get syntax binds an object property to a function that will be called when that property is looked up."[^1] "The set syntax binds an object property to a function to be called when there is an attempt to set that property."[^2] Both can also be used in classes.[^1][^2] Example: [`basics`](#basics).

**r2.** "A getter must have exactly zero parameters."[^1] "A setter must have exactly one parameter."[^2] Code that reads or assigns the property looks the same as for a normal property.

**r3.** "An object property is either a data property or an accessor property, but it cannot simultaneously be both."[^1] In ECMA-262, an accessor property has [[Get]] and [[Set]] attributes in place of [[Value]] and [[Writable]]. The [[Get]] function "is called with an empty arguments list to retrieve the property value each time a get access of the property is performed", and the [[Set]] function "is called with an arguments list containing the assigned value as its sole argument each time a set access of the property is performed."[^3] Example: [`descriptor`](#descriptor).

**r4.** In an object literal, "Properties defined using this syntax are own properties of the created object, and they are configurable and enumerable."[^1] In a class, "Unlike getter properties in object literals, getter properties in classes are not enumerable."[^1]

**r5.** A property can have only one of the two. For a property with only a setter, "any attempts to access it will result in undefined".[^2] For a property with only a getter, an assignment "will be silently ignored in non-strict mode, it will throw a TypeError in strict mode."[^4] Example: [`readonly`](#readonly).

**r6.** "To append a getter to an existing object later at any time, use Object.defineProperty() ."[^1] Properties added that way are not enumerable unless the descriptor says so: "By default, properties added using Object.defineProperty() are not writable, not enumerable, and not configurable."[^5] Example: [`later`](#later).

**r7.** "Getters give you a way to define a property of an object, but they do not calculate the property's value until it is accessed."[^1] A getter can delete itself and put a data property with the result in its place, so the work is done once: "The value is calculated the first time the getter is called and is then cached so subsequent accesses return the cached value without recalculating it."[^1] Example: [`lazy`](#lazy).

## Examples

The descriptor of an accessor property (rule r3).

```javascript run id=descriptor
const box = {
  get size() {
    return 1;
  },
};
const d = Object.getOwnPropertyDescriptor(box, "size");
console.log(typeof d.get, d.set, "value" in d, d.enumerable);
```

```output for=descriptor
function undefined false true
```

An accessor added after the object is made (rule r6).

```javascript run id=later
const point = { x: 3, y: 4 };
Object.defineProperty(point, "length", {
  get() {
    return Math.hypot(this.x, this.y);
  },
});
console.log(point.length, Object.keys(point));
point.x = 6;
point.y = 8;
console.log(point.length);
```

```output for=later
5 [ 'x', 'y' ]
10
```

A lazy getter that replaces itself (rule r7).

```javascript run id=lazy
let runs = 0;
const config = {
  get table() {
    runs += 1;
    const value = Array.from({ length: 3 }, (_, i) => i * i);
    delete this.table;
    this.table = value;
    return value;
  },
};
console.log(config.table, config.table, runs);
console.log(typeof Object.getOwnPropertyDescriptor(config, "table").get);
```

```output for=lazy
[ 0, 1, 4 ] [ 0, 1, 4 ] 1
undefined
```

## Pitfalls

### A setter that assigns to its own name

```javascript wrong id=recursive
const user = {
  set name(value) {
    this.name = value.trim();
  },
};
try {
  user.name = " Ada ";
} catch (e) {
  console.log(e.name);
}
const fixed = {
  _name: "",
  get name() {
    return this._name;
  },
  set name(value) {
    this._name = value.trim();
  },
};
fixed.name = " Ada ";
console.log(fixed.name);
```

```output for=recursive
RangeError
Ada
```

Each set access of `name` calls the setter (rule r3), so `this.name = ...` inside it calls the setter again until the stack runs out. Keep the value in another property.

### Assigning to a getter-only property

```javascript wrong id=readonly
"use strict";
const circle = {
  r: 2,
  get area() {
    return 3 * this.r ** 2;
  },
};
try {
  circle.area = 1;
} catch (e) {
  console.log(e.name);
}
console.log(circle.area);
```

```output for=readonly
TypeError
12
```

In strict mode the assignment throws, and in non-strict code it does nothing (rule r5). Add a setter, or do not assign.

### Expecting spread to copy the getter

```javascript wrong id=copy
let n = 0;
const source = {
  get next() {
    n += 1;
    return n;
  },
};
const copy = { ...source };
console.log(copy.next, copy.next, typeof Object.getOwnPropertyDescriptor(copy, "next").get);
const full = Object.create(Object.getPrototypeOf(source), Object.getOwnPropertyDescriptors(source));
console.log(full.next, full.next);
```

```output for=copy
1 1 undefined
2 3
```

Spread reads each property with Get and puts the value on the new object: the steps are "Let propValue be ? Get(from, nextKey)." and then "Perform ! CreateDataPropertyOrThrow(target, nextKey, propValue)."[^6] The copy has a data property with the value at the time of the copy. To keep the accessor, copy the descriptors. MDN gives `Object.create()` with `Object.getOwnPropertyDescriptors()` as the way to make a shallow copy "between two unknown objects".[^7]

## Comparison

Python's `property` "Return a property attribute. fget is a function for getting an attribute value. fset is a function for setting an attribute value."[^8] C#: "A property is a member that provides a flexible mechanism to read, write, or compute the value of a data field. Properties appear as public data members, but they're implemented as special methods called accessors."[^9] In both, the accessor is declared in a class. In JavaScript an accessor can be put on any object, in a literal or later with `Object.defineProperty` (rule r6). See [[getters-and-setters]].

## References
