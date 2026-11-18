---
tag: FX276A
kind: feature
owner: javascript
slug: creating-and-changing-prototype-chains
name: Creating and changing prototype chains
aliases: [Object.create, Object.setPrototypeOf, setting the prototype, proto key in object literals, proto accessor, changing the prototype of an object]
area: languages/javascript/user-types
level: core
summary: >-
  JavaScript sets an object's prototype at creation with a proto key in a literal, Object.create, or new. Object.setPrototypeOf changes it later, which is legal but slower.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/fundamental-objects.html#sec-object.setprototypeof", locator: "20.1.2.2 Object.create; 20.1.2.23 Object.setPrototypeOf"}
see_also: [javascript/inheritance-and-the-prototype-chain, javascript/constructors, javascript/object-initializers, prototype-based-programming]
sources:
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 13
  locator: 13.2.5.6 PropertyDefinitionEvaluation
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Inheritance_and_the_prototype_chain
  title: MDN Web Docs, JavaScript Guide, Inheritance and the prototype chain
  locator: Building longer inheritance chains; Different ways of creating and mutating prototype chains
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/create
  title: MDN Web Docs, JavaScript Reference, Object.create()
  locator: the introduction; Parameters; Classical inheritance with Object.create()
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/fundamental-objects.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 20
  locator: 20.1.2.2 Object.create; 20.1.2.23 Object.setPrototypeOf
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/setPrototypeOf
  title: MDN Web Docs, JavaScript Reference, Object.setPrototypeOf()
  locator: Exceptions; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/proto
  title: MDN Web Docs, JavaScript Reference, Object.prototype.__proto__
  locator: Description
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/datamodel.html
  title: The Python Language Reference, Data model, Python 3.14
  locator: 3.3.2.4 __slots__, Notes on using __slots__
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const base = {
  hello() {
    return `hi from ${this.name}`;
  },
};
const a = { __proto__: base, name: "a" };
const b = Object.create(base, { name: { value: "b" } });
const c = { name: "c" };
Object.setPrototypeOf(c, base);
for (const o of [a, b, c]) console.log(o.hello(), Object.getPrototypeOf(o) === base);
```

```output for=basics
hi from a true
hi from b true
hi from c true
```

| Way | When | Notes |
|---|---|---|
| `{ __proto__: p, x: 1 }` | at creation | standard literal syntax; own properties in the same literal |
| `Object.create(p, descriptors)` | at creation | the second argument gives property descriptors |
| `new F()` or `new C()` for a class | at creation | the prototype is `F.prototype`; see [[javascript/constructors]] |
| `Object.setPrototypeOf(o, p)` | later | allowed, but it slows the engine down |
| `o.__proto__ = p` | later | a deprecated accessor on `Object.prototype` |

## Semantics

**r1.** In an object literal, a key written as `__proto__` that "IsComputedPropertyKey of PropertyName is false" for does not make a property. If its value is an object or null, ECMA-262 does this: "Perform ! object.[[SetPrototypeOf]](propValue)."[^1] For any other value, MDN says it "only fails silently without throwing an exception."[^2] The guide adds: "Declaring extra own properties on the object at creation is more ergonomic than Object.create ."[^2] Example: [`literal`](#literal).

**r2.** `Object.create` "creates a new object, using an existing object as the prototype of the newly created object."[^3] Its second argument, "If specified and not undefined , an object whose enumerable own properties specify property descriptors to be added to the newly-created object, with the corresponding property names."[^3] ECMA-262: "If O is not an Object and O is not null, throw a TypeError exception."[^4] Example: [`create`](#create).

**r3.** "While all methods above will set the prototype chain at object creation time, Object.setPrototypeOf() allows mutating the [[Prototype]] internal property of an existing object. It can even force a prototype on a prototype-less object created with Object.create(null) or remove the prototype of an object by setting it to null ."[^2] When the change is refused: "If status is false, throw a TypeError exception."[^4] MDN lists the refused cases: "The obj parameter is non-extensible, or it's an immutable prototype exotic object, such as Object.prototype or window ."[^5] Example: [`change`](#change).

**r4.** Changing a prototype later costs speed. "However, you should set the prototype during creation if possible, because setting the prototype dynamically disrupts all optimizations that engines have made to the prototype chain."[^2]

**r5.** For constructor functions, the chain is built on the `prototype` objects. "To build longer prototype chains, we can set the [[Prototype]] of Constructor.prototype via the Object.setPrototypeOf() function." "In class terms, this is equivalent to using the extends syntax."[^2] Example: [`chain`](#chain).

**r6.** The accessor `Object.prototype.__proto__` also reads and sets the prototype, but MDN calls these accessors "non-standard and deprecated", and says "You should almost always use Object.setPrototypeOf instead."[^2] The accessor "is just an accessor property on Object.prototype consisting of a getter and setter function", so an object that does not inherit from `Object.prototype` does not have it.[^6] Example: [`nullproto`](#nullproto).

## Examples

The literal key (rule r1).

```javascript run id=literal
const p = { kind: "proto" };
const a = { __proto__: p };
const b = { ["__proto__"]: p };
const c = { __proto__: 5 };
console.log(Object.getPrototypeOf(a) === p, Object.hasOwn(a, "__proto__"));
console.log(Object.getPrototypeOf(b) === Object.prototype, Object.hasOwn(b, "__proto__"));
console.log(Object.getPrototypeOf(c) === Object.prototype);
```

```output for=literal
true false
true true
true
```

The computed key in `b` makes an ordinary own property.

`Object.create` with descriptors and with null (rule r2).

```javascript run id=create
const o = Object.create(
  { inherited: 1 },
  { hidden: { value: 2 }, shown: { value: 3, enumerable: true } },
);
console.log(Object.keys(o), o.hidden, o.inherited);
try {
  Object.create(5);
} catch (e) {
  console.log(e.name);
}
const bare = Object.create(null);
console.log(Object.getPrototypeOf(bare), "toString" in bare);
```

```output for=create
[ 'shown' ] 2 1
TypeError
null false
```

A descriptor that does not say `enumerable: true` makes a non-enumerable property.

Changing a prototype later (rule r3).

```javascript run id=change
const walker = { move: () => "walk" };
const swimmer = { move: () => "swim" };
const pet = Object.create(walker);
console.log(pet.move());
Object.setPrototypeOf(pet, swimmer);
console.log(pet.move());
Object.setPrototypeOf(pet, null);
console.log(typeof pet.move, Object.getPrototypeOf(pet));
for (const target of [Object.preventExtensions({}), Object.prototype]) {
  try {
    Object.setPrototypeOf(target, swimmer);
  } catch (e) {
    console.log(e.name);
  }
}
```

```output for=change
walk
swim
undefined null
TypeError
TypeError
```

A chain of constructor functions (rule r5).

```javascript run id=chain
function Base() {}
Base.prototype.hello = function () {
  return "base hello";
};
function Derived() {
  Base.call(this);
}
Object.setPrototypeOf(Derived.prototype, Base.prototype);
Derived.prototype.bye = function () {
  return "derived bye";
};
const d = new Derived();
console.log(d.hello(), d.bye(), d instanceof Base, d.constructor === Derived);
```

```output for=chain
base hello derived bye true true
```

## Pitfalls

### Replacing `Derived.prototype` with `Object.create`

```javascript wrong id=legacy
function Base() {}
function Derived() {}
Derived.prototype = Object.create(Base.prototype);
const d = new Derived();
console.log(d instanceof Base, d.constructor === Derived, d.constructor === Base);
```

```output for=legacy
true false true
```

The guide warns about this older pattern: "because this reassigns the prototype property and removes the constructor property, it can be more error-prone".[^2] MDN's `Object.create` page notes "caveats to watch out for using create() , such as re-adding the constructor property to ensure proper semantics."[^3] Use `Object.setPrototypeOf(Derived.prototype, Base.prototype)` as in [`chain`](#chain), or a class with `extends`.

### Setting the accessor on a null-prototype object

```javascript wrong id=nullproto
const dict = Object.create(null);
dict.__proto__ = { greet: "hi" };
console.log(dict.greet, Object.keys(dict), Object.getPrototypeOf(dict));
Object.setPrototypeOf(dict, { greet: "hi" });
console.log(dict.greet);
```

```output for=nullproto
undefined [ '__proto__' ] null
hi
```

`dict` does not inherit the accessor (rule r6), so the assignment makes an own property with that name and leaves the prototype as null. `Object.setPrototypeOf` works on every object.

## Comparison

A Python instance is linked to its class, and the data model allows assignment to the instance's `__class__` only within limits: it "works only if both classes have the same" `__slots__`.[^7] In JavaScript the link goes to any object or to null, and `Object.setPrototypeOf` can change it to any other object, unless the object is non-extensible or the change would make a loop (rule r3).

## References
