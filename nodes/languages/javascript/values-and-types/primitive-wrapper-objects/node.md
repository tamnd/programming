---
tag: X760WQ
kind: feature
owner: javascript
slug: primitive-wrapper-objects
name: Primitive wrapper objects
aliases: [wrapper objects, auto-boxing, boxing, "new String", "new Number", "new Boolean", "Object(primitive)", ToObject]
area: languages/javascript/values-and-types
level: core
summary: >-
  String, Number, Boolean, Symbol and BigInt objects wrap a primitive. A property read on a primitive makes a wrapper. new String and new Boolean give objects that act unlike primitives.
see_also: [javascript/primitive-values, javascript/javascript-data-types-and-data-structures, javascript/booleans-and-truthiness, javascript/the-new-operator, javascript/strings, javascript/bigints, python/objects-values-and-types]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Data_structures
    title: JavaScript data types and data structures, MDN
    locator: Primitive values
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Symbol
    title: Symbol, MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/BigInt
    title: BigInt, MDN
    locator: Description; Constructor
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Primitive
    title: Primitive, MDN Glossary
    locator: introduction
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-toobject
    title: ECMAScript 2026 Language Specification
    locator: 7.1.18 ToObject
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String
    title: String, MDN
    locator: String primitives and String objects
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Boolean
    title: Boolean, MDN
    locator: Boolean primitives and Boolean objects; Boolean coercion
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/datamodel.html
    title: Data model, The Python Language Reference
    locator: 3.1 Objects, values and types
    accessed: 2026-11-18
---

## Synopsis

| Primitive | Wrapper | `new` allowed | Make a wrapper without `new` |
|---|---|---|---|
| string | `String` | yes | `Object('a')` |
| number | `Number` | yes | `Object(1)` |
| boolean | `Boolean` | yes | `Object(true)` |
| symbol | `Symbol` | no, TypeError | `Object(Symbol())` |
| bigint | `BigInt` | no, error | `Object(1n)` |
| null, undefined | none | | `Object(null)` gives `{}` |

"All primitive types, except null and undefined , have their corresponding object wrapper types, which provide useful methods for working with the primitive values."[^1] The `new` column is from the Symbol and BigInt pages.[^2] [^3]

```javascript run id=synopsis
const prim = "foo";
const obj = new String("foo");
console.log(typeof prim, typeof obj, prim.length, obj.length);
console.log(prim === obj, prim == obj, obj.valueOf() === prim);
console.log(new String("a") == new String("a"), new String("a") === new String("a"));
```

```output for=synopsis
string object 3 3
false true true
false false
```

## Semantics

**r1. Auto-boxing.** "When properties are accessed on primitives, JavaScript auto-boxes the value into a wrapper object and accesses the property on that object instead." `'foo'.includes('f')` "implicitly creates a String wrapper object and calls String.prototype.includes() on that object."[^4] "This auto-boxing behavior is not observable in JavaScript code".[^4] Example: [`synopsis`](#synopsis).

**r2. ToObject.** The spec operation behind a wrapper is ToObject. "If argument is either undefined or null , throw a TypeError exception." For a Boolean, Number or String it returns "a new" wrapper object with the value in an internal slot.[^5] Example: [`object-call`](#object-call).

**r3. Calls without `new` give primitives.** Strings "returned from String calls in a non-constructor context (that is, called without using the new keyword) are primitive strings."[^6] `String(1)`, `Number('2')` and `Boolean(0)` are conversions, not wrappers. Example: [`object-call`](#object-call).

**r4. `new String` gives an object.** "String with new returns a string wrapper object." Its `typeof` is `'object'`.[^6] "You should rarely find yourself using String as a constructor."[^6] So `new String('foo') === 'foo'` is false. Example: [`synopsis`](#synopsis).

**r5. `new Boolean(false)` is truthy.** "Do not use the Boolean() constructor with new". MDN says that "all objects, including a Boolean object whose wrapped value is false , are truthy".[^7] Example: [`boolean-trap`](#boolean-trap).

**r6. No `new` for Symbol and BigInt.** `new Symbol()` throws a TypeError. "If you really want to create a Symbol wrapper object, you can use the Object() function".[^2] `BigInt()` "Throws an error when called with new ."[^3] Example: [`new-symbol`](#new-symbol).

**r7. Back to a primitive.** "A String object can always be converted to its primitive counterpart with the valueOf() method."[^6] Example: [`synopsis`](#synopsis).

## Examples

The examples run with Node.js 24.

`Object()` wraps, the plain calls convert (rules r2 and r3):

```javascript run id=object-call
console.log(typeof Object(1), typeof Object(1n), typeof Object(Symbol("s")));
console.log(typeof String(1), typeof Number("2"), typeof Boolean(0));
console.log(Object(null), Object(undefined));
```

```output for=object-call
object object object
string number boolean
{} {}
```

`Object()` called with `null` or `undefined` returns an empty object. The ToObject operation itself throws for these values (rule r2).

A Boolean object holding `false` is truthy (rule r5). MDN gives this case:[^7]

```javascript run id=boolean-trap
const myFalse = new Boolean(false);
if (myFalse) {
  console.log("printed");
}
console.log(Boolean(myFalse), myFalse.valueOf(), !!false);
```

```output for=boolean-trap
printed
true false false
```

`new Symbol()` throws (rule r6):

```javascript fails id=new-symbol expect=exception:TypeError
const sym = new Symbol("foo");
```

String objects and `eval` (rule r4). "Primitives passed to eval are treated as source code; String objects are treated as all other objects are, by returning the object."[^6]

```javascript run id=eval
const s1 = "2 + 2";
const s2 = new String("2 + 2");
console.log(eval(s1), typeof eval(s2), eval(s2.valueOf()));
```

```output for=eval
4 object 4
```

## Pitfalls

### `new String` and `new Number` in comparisons

Two `new String('a')` objects are two objects, so `===` and `==` both give false for them ([`synopsis`](#synopsis)). Use the primitive, or call `valueOf()` (rules r4 and r7).

### `new Boolean(false)` in a condition

A condition on a Boolean object is always true ([`boolean-trap`](#boolean-trap)). Use `Boolean(x)` or `!!x` to convert.[^7]

### Properties on a primitive are lost

A property set on a primitive goes to an "ephemeral wrapper object", so it is gone at the next read.[^4] See [[javascript/primitive-values]].

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| primitive and object forms | both, for string, number, boolean, symbol and bigint[^1] | one form, "All data in a Python program is represented by objects"[^8] |
| methods on `'abc'` | on a String wrapper made for the call[^4] | on the `str` object itself[^8] |

Python has no wrapper types, because each value is already an object with "an identity, a type and a value".[^8] See [[python/objects-values-and-types]].

## References
