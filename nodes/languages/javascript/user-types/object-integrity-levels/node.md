---
tag: KBJT1F
kind: feature
owner: javascript
slug: object-integrity-levels
name: Object integrity levels
aliases: [Object.freeze, Object.seal, Object.preventExtensions, frozen objects, sealed objects, non-extensible objects, deep freeze, Object.isFrozen]
area: languages/javascript/user-types
level: core
summary: >-
  Object.preventExtensions stops new properties, Object.seal also stops deleting and reconfiguring them, and Object.freeze also makes data properties read-only. All three are shallow.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-setintegritylevel", locator: "7.3.15 SetIntegrityLevel; 7.3.16 TestIntegrityLevel"}
see_also: [javascript/property-descriptors, javascript/objects, immutable-object, javascript/shallow-and-deep-copies]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/preventExtensions
  title: MDN Web Docs, JavaScript Reference, Object.preventExtensions()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/seal
  title: MDN Web Docs, JavaScript Reference, Object.seal()
  locator: Description
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/abstract-operations.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 7
  locator: 7.3.15 SetIntegrityLevel; 7.3.16 TestIntegrityLevel
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/freeze
  title: MDN Web Docs, JavaScript Reference, Object.freeze()
  locator: Description; Deep freezing
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/isFrozen
  title: MDN Web Docs, JavaScript Reference, Object.isFrozen()
  locator: Description
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/fundamental-objects.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 20
  locator: 20.1.2.16 Object.isExtensible; 20.1.2.18 Object.isSealed
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/const
  title: MDN Web Docs, JavaScript Reference, const
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: The Java Language Specification, Java SE 25 Edition, chapter 4
  locator: 4.12.4 final Variables
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
"use strict";
for (const lock of [Object.preventExtensions, Object.seal, Object.freeze]) {
  const o = lock({ x: 1 });
  const changes = {
    add: () => (o.y = 2),
    change: () => (o.x = 3),
    remove: () => delete o.x,
  };
  const result = Object.entries(changes).map(([name, f]) => {
    try {
      f();
      return `${name} ok`;
    } catch {
      return `${name} fails`;
    }
  });
  console.log(lock.name, result.join(", "));
}
```

```output for=basics
preventExtensions add fails, change ok, remove ok
seal add fails, change ok, remove fails
freeze add fails, change fails, remove fails
```

| Level | Add | Delete or reconfigure | Change a data value | Test |
|---|---|---|---|---|
| `Object.preventExtensions(o)` | no | yes | yes | `!Object.isExtensible(o)` |
| `Object.seal(o)` | no | no | yes | `Object.isSealed(o)` |
| `Object.freeze(o)` | no | no | no | `Object.isFrozen(o)` |

## Semantics

**r1.** `Object.preventExtensions` "prevents new properties from ever being added to an object (i.e., prevents future extensions to the object). It also prevents the object's prototype from being re-assigned."[^1] "Note that the properties of a non-extensible object, in general, may still be deleted." "There is no way to make an object extensible again once it has been made non-extensible."[^1] Example: [`basics`](#basics).

**r2.** "Sealing an object is equivalent to preventing extensions and then changing all existing properties' descriptors to configurable: false . This has the effect of making the set of properties on the object fixed." It "does not prevent the values of data properties from being changed."[^2] In ECMA-262, SetIntegrityLevel "is used to fix the set of own properties of an object."[^3]

**r3.** "Freezing an object is equivalent to preventing extensions and then changing all existing properties' descriptors' configurable to false".[^4] For a data property, SetIntegrityLevel with level frozen uses the descriptor "{ [[Configurable]]: false, [[Writable]]: false }", and for an accessor property it uses "{ [[Configurable]]: false }".[^3] So a getter on a frozen object can still return a new value each time, and "the setter can still be called without throwing errors when setting the property."[^4] Example: [`accessor`](#accessor).

**r4.** A change that the level forbids "will fail, either silently or by throwing a TypeError exception (most commonly, but not exclusively, when in strict mode)."[^4] The blocks on this page use strict mode, so the failures throw.

**r5.** "An object is frozen if and only if it is not extensible, all its properties are non-configurable, and all its data properties (that is, properties which are not accessor properties with getter or setter components) are non-writable."[^5] TestIntegrityLevel checks extensibility first: "If the object is extensible, none of its properties are examined."[^3] `Object.isSealed` returns "? TestIntegrityLevel(O, sealed)" for an object.[^6] So an empty non-extensible object is both sealed and frozen. Example: [`tests`](#tests).

**r6.** The levels are shallow. "The result of calling Object.freeze(object) only applies to the immediate properties of object itself".[^4] "Note that values that are objects can still be modified, unless they are also frozen."[^4] And the prototype is not frozen: "Object.preventExtensions() only prevents addition of own properties. Properties can still be added to the object prototype."[^1] Examples: [`proto`](#proto), [`shallow`](#shallow).

**r7.** "freeze() returns the same object that was passed into the function. It does not create a frozen copy."[^4] Example: [`copy`](#copy).

## Examples

Testing the levels (rule r5).

```javascript run id=tests
const empty = Object.preventExtensions({});
const sealed = Object.seal({ a: 1 });
const frozen = Object.freeze({ a: 1 });
console.log(Object.isFrozen(empty), Object.isSealed(empty));
console.log(Object.isFrozen(sealed), Object.isSealed(sealed), Object.isExtensible(sealed));
console.log(Object.isFrozen(frozen), Object.isSealed(frozen));
```

```output for=tests
true true
false true false
true true
```

Accessors keep working on a frozen object (rule r3).

```javascript run id=accessor
"use strict";
let count = 0;
const counter = Object.freeze({
  get next() {
    count += 1;
    return count;
  },
  set start(v) {
    count = v;
  },
});
counter.start = 10;
console.log(counter.next, counter.next, Object.isFrozen(counter));
```

```output for=accessor
11 12 true
```

The prototype stays open, but it cannot be replaced (rules r1 and r6).

```javascript run id=proto
const proto = {};
const o = Object.preventExtensions(Object.create(proto));
proto.greet = () => "hi";
console.log(o.greet());
try {
  Object.setPrototypeOf(o, {});
} catch (e) {
  console.log(e.name);
}
```

```output for=proto
hi
TypeError
```

MDN: "any [[Prototype]] re-assignment will throw a TypeError ."[^1]

## Pitfalls

### Expecting `freeze` to freeze nested objects

```javascript wrong id=shallow
"use strict";
const employee = Object.freeze({ name: "Ada", address: { city: "London" } });
employee.address.city = "Paris";
console.log(employee.address.city, Object.isFrozen(employee.address));
function deepFreeze(obj) {
  for (const key of Reflect.ownKeys(obj)) {
    const value = obj[key];
    if (value && typeof value === "object") deepFreeze(value);
  }
  return Object.freeze(obj);
}
const fixed = deepFreeze({ name: "Ada", address: { city: "London" } });
try {
  fixed.address.city = "Paris";
} catch (e) {
  console.log(e.name);
}
console.log(fixed.address.city);
```

```output for=shallow
Paris false
TypeError
London
```

Only the outer object is frozen (rule r6). MDN: "To make an object immutable, recursively freeze each non-primitive property (deep freeze)."[^4]

### Expecting `const` to freeze the object

```javascript wrong id=const
const settings = { debug: false };
settings.debug = true;
console.log(settings.debug);
const locked = Object.freeze({ debug: false });
console.log(Reflect.set(locked, "debug", true), locked.debug);
```

```output for=const
true
false false
```

"The value of a constant can't be changed through reassignment using the assignment operator, but if a constant is an object, its properties can be added, updated, or removed."[^7] `const` fixes the variable, and `Object.freeze` fixes the object.

### Expecting `freeze` to return a copy

```javascript wrong id=copy
"use strict";
const original = { n: 1 };
const frozen = Object.freeze(original);
console.log(frozen === original);
try {
  original.n = 2;
} catch (e) {
  console.log(e.name);
}
const copy = { ...frozen };
copy.n = 3;
console.log(original.n, copy.n, Object.isFrozen(copy));
```

```output for=copy
true
TypeError
1 3 false
```

`freeze` changes the object you pass it (rule r7). To keep a changeable version, copy it before freezing.

## Comparison

Java's `final` is about the variable, like JavaScript's `const`: "If a final variable holds a reference to an object, then the state of the object may be changed by operations on the object, but the variable will always refer to the same object."[^8] JavaScript adds levels on the object itself, so one object can be frozen while every variable that refers to it stays reassignable (rules r1 to r3).

## References
