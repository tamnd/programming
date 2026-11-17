---
tag: GNXEE9
kind: feature
owner: javascript
slug: inheritance-and-the-prototype-chain
name: Inheritance and the prototype chain
aliases: [prototype chain, prototypal inheritance, prototype link, property lookup, property shadowing, Object.getPrototypeOf, isPrototypeOf]
area: languages/javascript/user-types
level: core
summary: >-
  Each JavaScript object links to a prototype object or to null. A property read that misses the object goes on to its prototype, and so on to the end of the chain. Assignment makes an own property.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html#sec-ordinary-object-internal-methods-and-internal-slots", locator: "10.1 Ordinary Object Internal Methods and Internal Slots; 10.1.8.1 OrdinaryGet"}
see_also: [prototype-based-programming, javascript/objects, javascript/querying-object-properties, javascript/enumerability-and-ownership-of-properties]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Inheritance_and_the_prototype_chain
  title: MDN Web Docs, JavaScript Guide, Inheritance and the prototype chain
  locator: the introduction; Inheriting properties; Inheriting methods; Using different methods to create objects; Performance; Conclusion
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 10
  locator: 10.1 Ordinary Object Internal Methods and Internal Slots; 10.1.2.1 OrdinarySetPrototypeOf; 10.1.8.1 OrdinaryGet
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/getPrototypeOf
  title: MDN Web Docs, JavaScript Reference, Object.getPrototypeOf()
  locator: the introduction; Return value
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/fundamental-objects.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 20
  locator: 20.1.2.23 Object.setPrototypeOf
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/isPrototypeOf
  title: MDN Web Docs, JavaScript Reference, Object.prototype.isPrototypeOf()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/datamodel.html
  title: The Python Language Reference, Data model, Python 3.14
  locator: 3.2.11 Class instances
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const animal = {
  legs: 4,
  describe() {
    return `${this.name} has ${this.legs} legs`;
  },
};
const bird = { __proto__: animal, legs: 2 };
const robin = { __proto__: bird, name: "Robin" };
console.log(robin.describe());
console.log(Object.getPrototypeOf(robin) === bird, Object.getPrototypeOf(bird) === animal);
console.log(Object.getPrototypeOf(Object.getPrototypeOf(animal)));
console.log(robin.wings, animal.isPrototypeOf(robin));
```

```output for=basics
Robin has 2 legs
true true
null
undefined true
```

The chain here is `robin`, then `bird`, then `animal`, then `Object.prototype`, then `null`.

| Operation | Uses the chain |
|---|---|
| read `o.p` | yes: `o`, then each prototype, until `p` is found or the chain ends |
| assign `o.p = v` | only to find an inherited setter or a read-only property; otherwise it makes or changes an own `p` |
| `p in o` | yes |
| `Object.hasOwn(o, p)` | no |
| `proto.isPrototypeOf(o)` | yes: is `proto` anywhere in the chain of `o` |
| `Object.getPrototypeOf(o)` | gives the next link only |

## Semantics

**r1.** "Each object has an internal link to another object called its prototype. That prototype object has a prototype of its own, and so on until an object is reached with null as its prototype."[^1] In ECMA-262: "All ordinary objects have an internal slot called [[Prototype]]. The value of this internal slot is either null or an object and is used for implementing inheritance."[^2] `Object.getPrototypeOf` "returns the prototype (i.e., the value of the internal [[Prototype]] property) of the specified object."[^3] Example: [`basics`](#basics).

**r2.** A read looks along the chain. "When trying to access a property of an object, the property will not only be sought on the object, but also on the prototype of the object, the prototype of the prototype, and so on, until either a property with a matching name is found or the end of the prototype chain is reached."[^1] OrdinaryGet asks the prototype when there is no own property, and "If parent is null, return undefined."[^2] Example: [`basics`](#basics).

**r3.** An own property hides a property of the same name further up the chain. "An inherited function acts just as any other property, including property shadowing as shown above (in this case, a form of method overriding)."[^1] Example: [`basics`](#basics), where `bird.legs` hides `animal.legs`.

**r4.** Inside an inherited method, `this` is the object the method was read from. "When an inherited function is executed, the value of this points to the inheriting object, not to the prototype object where the function is an own property."[^1] For an inherited getter, OrdinaryGet ends with "Return ? Call(getter, Receiver)."[^2] Example: [`basics`](#basics).

**r5.** Assignment does not change the prototype. ECMA-262: "If P refers to a writable data property on the [[Prototype]] object, set access of P on O creates a new data property named P on O. If P refers to a non-writable data property on the [[Prototype]] object, set access of P on O fails. If P refers to an accessor property on the [[Prototype]] object, the accessor is inherited by O for both get access and set access."[^2] MDN: "Setting a property to an object creates an own property. The only exception to the getting and setting behavior rules is when it's intercepted by a getter or setter."[^1] Example: [`assign`](#assign).

**r6.** The link is live. "This means that all properties defined on prototype are effectively shared by all instances, and you can even later change parts of prototype and have the changes appear in all existing instances."[^1] Example: [`live`](#live).

**r7.** A chain cannot loop. For OrdinarySetPrototypeOf: "The loop in step 7 guarantees that there will be no cycles in any prototype chain that only includes objects that use the ordinary object definitions for [[GetPrototypeOf]] and [[SetPrototypeOf]]."[^2] When the change is refused, `Object.setPrototypeOf` does this: "If status is false, throw a TypeError exception."[^4] Example: [`cycle`](#cycle).

**r8.** `isPrototypeOf` "checks if this object exists in another object's prototype chain."[^5] "Otherwise, the this value is converted to an object, and the prototype chain of object is searched for the this value, until the end of the chain is reached or the this value is found."[^5] Example: [`basics`](#basics).

## Examples

Assignment makes an own property, except for inherited setters and read-only properties (rule r5).

```javascript run id=assign
"use strict";
const base = { color: "red", set size(v) { this._size = v * 10; } };
Object.defineProperty(base, "id", { value: 1, writable: false });
const o = { __proto__: base };
o.color = "blue";
console.log(o.color, base.color, Object.hasOwn(o, "color"));
o.size = 3;
console.log(o._size, Object.hasOwn(o, "size"), Object.hasOwn(o, "_size"));
try {
  o.id = 2;
} catch (e) {
  console.log(e.name, o.id);
}
```

```output for=assign
blue red true
30 false true
TypeError 1
```

The setter on `base` runs with `this` set to `o`, so `_size` is an own property of `o` (rules r4 and r5).

A change to the prototype shows up in objects made before it (rule r6).

```javascript run id=live
const proto = { greet() { return "hello"; } };
const a = { __proto__: proto };
proto.greet = function () {
  return "hi";
};
proto.wave = () => "wave";
console.log(a.greet(), a.wave());
```

```output for=live
hi wave
```

A prototype chain cannot loop (rule r7).

```javascript run id=cycle
const a = {};
const b = { __proto__: a };
try {
  Object.setPrototypeOf(a, b);
} catch (e) {
  console.log(e.name);
}
console.log(Object.getPrototypeOf(a) === Object.prototype);
```

```output for=cycle
TypeError
true
```

## Pitfalls

### Putting a mutable value on the prototype

```javascript wrong id=shared
const cartProto = { items: [], add(x) { this.items.push(x); } };
const cart1 = { __proto__: cartProto };
const cart2 = { __proto__: cartProto };
cart1.add("tea");
console.log(cart2.items, Object.hasOwn(cart1, "items"));
const cart3 = { __proto__: cartProto, items: [] };
cart3.add("cake");
console.log(cart3.items, cartProto.items);
```

```output for=shared
[ 'tea' ] false
[ 'cake' ] [ 'tea' ]
```

`this.items.push(x)` reads `items` and does not assign it, so it finds the one array on the prototype (rules r2 and r5). Every object that inherits it sees the change (rule r6). Give each object its own array, as `cart3` does.

### Adding methods to a built-in prototype

```javascript wrong id=monkey
Array.prototype.last = function () {
  return this[this.length - 1];
};
const keys = [];
for (const k in [10, 20]) keys.push(k);
console.log([10, 20].last(), keys);
delete Array.prototype.last;
```

```output for=monkey
20 [ '0', '1', 'last' ]
```

Every array inherits the new property, and `for...in` lists it because it is enumerable. MDN: "This misfeature is called monkey patching. Doing monkey patching risks forward compatibility, because if the language adds this method in the future but with a different signature, your code will break."[^1]

### Long chains and missing properties

"The lookup time for properties that are high up on the prototype chain can have a negative impact on the performance, and this may be significant in the code where performance is critical. Additionally, trying to access nonexistent properties will always traverse the full prototype chain."[^1] Keep chains short where speed matters.

## Comparison

Python looks for an attribute on the instance and then on its class: "A class instance has a namespace implemented as a dictionary which is the first place in which attribute references are searched. When an attribute is not found there, and the instance’s class has an attribute by that name, the search continues with the class attributes."[^6] In JavaScript the next place to look is an ordinary object, not a class, and any object can be a prototype (rule r1). MDN notes that JavaScript classes "do not bring a new inheritance pattern": they use the same chain.[^1]

## References
