---
tag: 638EEE
kind: feature
owner: javascript
slug: objects
name: Objects
aliases: [JavaScript objects, plain objects, object properties, property keys, dot notation, bracket notation, delete operator, objects as reference values]
area: languages/javascript/user-types
level: intro
summary: >-
  A JavaScript object is a collection of properties, each with a string or symbol key. Properties can be read, added and deleted at run time, and objects are handled by reference.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-object-type", locator: "6.1.7 The Object Type"}
see_also: [javascript/maps, prototype-based-programming, value-types-and-reference-types, field-and-property]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Working_with_objects
  title: MDN Web Docs, JavaScript Guide, Working with objects
  locator: Objects and properties; Accessing properties; Deleting properties; Inheritance; Comparing objects
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 6
  locator: 6.1.7 The Object Type
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object
  title: MDN Web Docs, JavaScript Reference, Object
  locator: the introduction; Description; null-prototype objects
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/datamodel.html
  title: The Python Language Reference, Data model, Python 3.14
  locator: 3.1 Objects, values and types; Class instances
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: The Java Language Specification, Java SE 25 Edition, chapter 4
  locator: 4.3.1 Objects
  accessed: 2026-11-18
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification, language version go1.27
  locator: Struct types
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const car = { make: "Ford", year: 1969 };
car.color = "red";
car["model name"] = "Mustang";
console.log(car.make, car["year"], car.wheels);
delete car.year;
console.log("year" in car, Object.keys(car));
console.log(typeof car, typeof [], typeof (() => {}));
```

```output for=basics
Ford 1969 undefined
false [ 'make', 'color', 'model name' ]
object object function
```

| Operation | Syntax |
|---|---|
| create | `{ key: value }` |
| read | `obj.key` or `obj[expression]` |
| add or change | `obj.key = value` |
| remove | `delete obj.key` |
| test | `key in obj` or `Object.hasOwn(obj, key)` |
| list the own keys | `Object.keys(obj)` |

## Semantics

**r1.** "An object is a collection of properties, and a property is an association between a name (or key) and a value. A property's value can be a function, in which case the property is known as a method."[^1] In ECMA-262 each object "represents a collection of properties", and "Each property is either a data property, or an accessor property".[^2] Example: [`basics`](#basics).

**r2.** "A property key is either a String or a Symbol. All Strings and Symbols, including the empty String, are valid as property keys."[^2] MDN: "Property names can only be strings or Symbols", and other keys are converted to strings.[^1] A number key becomes its string, and an object key becomes the result of its `toString()` method.[^1] Example: [`keys`](#keys).

**r3.** "Property accessors come in two syntaxes: dot notation and bracket notation."[^1] Dot notation needs a fixed name, and "you cannot use dot notation to access a property whose name is not a valid JavaScript identifier."[^1] Bracket notation takes an expression, so "This notation is also very useful when property names are to be dynamically determined".[^1] A property that does not exist reads as `undefined`.[^1] Example: [`dotvar`](#dotvar).

**r4.** Objects can change after they are made. "You can always add a property to a previously defined object."[^1] "You can remove a non-inherited property using the delete operator."[^1] Example: [`basics`](#basics).

**r5.** "All objects in JavaScript inherit from at least one other object. The object being inherited from is known as the prototype".[^1] For an object made with `{}`, that is `Object.prototype`: "a typical object inherits properties (including methods) from Object.prototype".[^3] The `in` operator finds inherited properties, and `Object.hasOwn` finds only the object's own ones. Example: [`inherit`](#inherit).

**r6.** "In JavaScript, objects are a reference type. Two distinct objects are never equal, even if they have the same properties. Only comparing the same object reference with itself yields true."[^1] Two variables can refer to one object, and a change through one is seen through the other. Example: [`references`](#references). See [[value-types-and-reference-types]].

**r7.** Arrays and functions are objects too. MDN: "Array indices are, in fact, properties with string keys that contain integers."[^1] ECMA-262: "some objects are callable; these are referred to as functions or function objects", and "All functions in ECMAScript are members of the Object type."[^2] The `typeof` operator gives `function` for them and `object` for other objects, as in [`basics`](#basics).

## Examples

Keys are strings or symbols (rule r2).

```javascript run id=keys
const obj = {};
const id = Symbol("id");
obj[1] = "number key";
obj[id] = "symbol key";
obj[{}] = "object key";
console.log(Object.keys(obj), Object.keys(obj).map((k) => typeof k));
console.log(obj["1"], obj[id]);
```

```output for=keys
[ '1', '[object Object]' ] [ 'string', 'string' ]
number key symbol key
```

`Object.keys` lists only the string keys. The symbol key is still there and reads back with `obj[id]`.

Inherited and own properties (rule r5).

```javascript run id=inherit
const point = { x: 1 };
console.log("x" in point, "toString" in point);
console.log(Object.hasOwn(point, "x"), Object.hasOwn(point, "toString"));
console.log(Object.getPrototypeOf(point) === Object.prototype, String(point));
```

```output for=inherit
true true
true false
true [object Object]
```

Two names for one object (rule r6).

```javascript run id=references
const fruit = { name: "apple" };
const same = fruit;
const copy = { name: "apple" };
same.name = "grape";
console.log(fruit.name, fruit === same, fruit === copy);
```

```output for=references
grape true false
```

## Pitfalls

### Using an object as a key

```javascript wrong id=objkey
const seen = {};
const a = { id: 1 };
const b = { id: 2 };
seen[a] = "first";
seen[b] = "second";
console.log(Object.keys(seen), seen[a]);
const byObject = new Map([[a, "first"], [b, "second"]]);
console.log(byObject.get(a));
```

```output for=objkey
[ '[object Object]' ] second
first
```

Both objects turn into the same string key, so the second write replaces the first (rule r2). A `Map` keeps the objects themselves as keys. See [[javascript/maps]].

### Reading a key with `in` from an object used as a dictionary

```javascript wrong id=inkey
const ages = { alice: 18, bob: 27 };
const has = (name) => name in ages;
console.log(has("alice"), has("toString"));
console.log(Object.hasOwn(ages, "toString"), typeof ages.toString);
```

```output for=inkey
true true
false function
```

`toString` is inherited from `Object.prototype`, so `in` finds it. MDN: "The presence of Object.prototype properties will cause some bugs".[^3] Use `Object.hasOwn` (rule r5), or a `Map`.

### Dot notation with a variable

```javascript wrong id=dotvar
const car = { make: "Ford" };
const key = "make";
console.log(car.key, car[key]);
```

```output for=dotvar
undefined Ford
```

`car.key` reads the property named `key`, which does not exist. To use the value of a variable as the name, use brackets (rule r3). MDN: "The variable must be passed in bracket notation."[^1]

## Comparison

Python: "Objects are Python’s abstraction for data." A class instance "has a namespace implemented as a dictionary which is the first place in which attribute references are searched."[^4] Java: "An object is a class instance or an array."[^5] Go has no general object type; "A struct is a sequence of named elements, called fields, each of which has a name and a type."[^6] A JavaScript object needs no class or type declaration, and its properties can be added and deleted at run time (rule r4).

## References
