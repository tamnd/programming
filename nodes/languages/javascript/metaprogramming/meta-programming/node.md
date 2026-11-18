---
tag: 8T9DJD
kind: feature
owner: javascript
slug: meta-programming
name: Meta programming
aliases: [metaprogramming, meta-programming in JavaScript, Proxy and Reflect, intercepting operations, internal methods, meta level]
area: languages/javascript/metaprogramming
level: advanced
summary: >-
  Proxy and Reflect let a program intercept the basic operations on an object, such as a property read, the in operator and a call. A proxy has a target and a handler, and Reflect does the default.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/reflection.html#sec-reflection", locator: "28 Reflection (28.1 The Reflect Object; 28.2 Proxy Objects)"}
see_also: [javascript/proxies, javascript/proxy-handlers-and-traps, javascript/revocable-proxies, javascript/reflection, javascript/well-known-symbols, proxy-pattern, python/dynamic-attribute-access]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Meta_programming
  title: MDN Web Docs, JavaScript Guide, Meta programming
  locator: the introduction; Proxies; Handlers and traps; Revocable Proxy; Reflection
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Proxy
  title: MDN Web Docs, JavaScript Reference, Proxy
  locator: Description; Object internal methods
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 10
  locator: 10.5 Proxy Object Internal Methods and Internal Slots
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/reflection.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 28
  locator: 28.1 The Reflect Object; 28.2 Proxy Objects
  accessed: 2026-11-18
- url: https://262.ecma-international.org/6.0/index.html
  title: ECMAScript 2015 Language Specification (ECMA-262, 6th edition)
  locator: 26.1 The Reflect Object; 26.2 Proxy Objects
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/datamodel.html
  title: The Python Language Reference, Python 3.14, Data model
  locator: 3.3.2. Customizing attribute access
  accessed: 2026-11-18
- url: https://docs.ruby-lang.org/en/3.4/BasicObject.html
  title: Ruby 3.4 documentation, class BasicObject
  locator: method_missing
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=intercept
const target = { name: "Ana" };
const log = [];
const proxy = new Proxy(target, {
  get(t, key, receiver) {
    log.push(`get ${String(key)}`);
    return Reflect.get(t, key, receiver);
  },
  has(t, key) {
    log.push(`has ${String(key)}`);
    return Reflect.has(t, key);
  },
});

console.log(proxy.name, "age" in proxy, proxy.age);
console.log(log);
console.log(Reflect.ownKeys(proxy), proxy === target);
```

```output for=intercept
Ana false undefined
[ 'get name', 'has age', 'get age' ]
[ 'name' ] false
```

The proxy records each read and each `in` test, and then does the normal operation on the target with `Reflect`. The operations that the handler does not define, such as `Reflect.ownKeys`, go to the target with no change.

| Part | What it is |
|---|---|
| target | the object that the proxy wraps |
| handler | an object whose methods (the traps) replace operations |
| trap | a handler method, such as `get`, `set`, `has` or `apply` |
| `Reflect` | an object with one function for each trap, which does the default operation |
| invariant | a rule that a trap must keep, or the operation throws a `TypeError` |

## Semantics

**r1.** "The Proxy and Reflect objects allow you to intercept and define custom behavior for fundamental language operations (e.g., property lookup, assignment, enumeration, function invocation, etc.). With the help of these two objects you are able to program at the meta level of JavaScript."[^1] Example: [`intercept`](#intercept).

**r2.** Each operation on an object calls an internal method of the object. "The obj.x property access syntax simply invokes the [[Get]] method on the object, and the object uses its own internal method implementation to determine what to return."[^2] "It's important to realize that all interactions with an object eventually boils down to the invocation of one of these internal methods, and that they are all customizable through proxies."[^2]

**r3.** A proxy is an exotic object. "A Proxy object is an exotic object whose essential internal methods are partially implemented using ECMAScript code."[^3] Its handler can replace each of the 13 internal methods. When the handler has no method for an operation, the operation goes to the target: "Invoking an internal method on the proxy results in the invocation of the corresponding internal method on the proxy's target object if the handler object does not have a method corresponding to the internal trap."[^3] Examples: [`intercept`](#intercept), [`default-value`](#default-value).

**r4.** Some rules stay in force. "Semantics that remain unchanged when implementing custom operations are called invariants. If you violate the invariants of a handler, a TypeError will be thrown."[^1] The rules come from the target: "Invariants (semantics that remain unchanged) regarding object non-extensibility or non-configurable properties are verified against the target."[^1] See [[javascript/proxy-handlers-and-traps]].

**r5.** "Reflect is a built-in object that provides methods for interceptable JavaScript operations. The methods are the same as those of the proxy handler's."[^1] "Reflect helps with forwarding default operations from the handler to the target."[^1] Reflect is not a class. It "is not a function object" and "it cannot be used as a constructor with the new operator."[^4] Example: [`reflect-results`](#reflect-results).

**r6.** "The Proxy.revocable() method is used to create a revocable Proxy object. This means that the proxy can be revoked via the function revoke and switches the proxy off."[^1] See [[javascript/revocable-proxies]].

**r7.** A proxy is a new object with its own identity. It is not equal to its target with `===`, and code that keeps the target in a `Set` or a `WeakMap` does not find the proxy. Example: [`identity`](#identity).

## Examples

```javascript run id=default-value
const withDefault = (obj, fallback) =>
  new Proxy(obj, {
    get: (t, key) => (key in t ? t[key] : fallback),
  });

const p = withDefault({ a: 1 }, 42);
p.b = 2;
console.log(p.a, p.b, p.c, Object.keys(p));
```

```output for=default-value
1 2 42 [ 'a', 'b' ]
```

The `get` trap gives 42 for a property that does not exist, as in the example of MDN.[^1] The assignment `p.b = 2` has no trap, so it goes to the target, and `Object.keys` shows the new property (rule r3).

```javascript run id=reflect-results
const obj = { x: 1 };
console.log(Object.keys(obj), Reflect.ownKeys(obj));
console.log(Reflect.has(obj, "x"), "x" in obj);
console.log(Reflect.defineProperty(Object.freeze({}), "y", { value: 2 }));
try {
  Object.defineProperty(Object.freeze({}), "y", { value: 2 });
} catch (e) {
  console.log(e.constructor.name);
}
```

```output for=reflect-results
[ 'x' ] [ 'x' ]
true true
false
TypeError
```

`Reflect.has` is the `in` operator as a function. `Reflect.defineProperty` returns `false` when it cannot define the property, and `Object.defineProperty` throws. MDN: "Because Reflect.defineProperty() returns a Boolean success status, you can just use an if...else block here".[^1] See [[javascript/reflection]].

## Versions

ECMAScript 2015, the 6th edition, added both objects. Its clause 26 has "26.1 The Reflect Object" and "26.2 Proxy Objects".[^5] In the 17th edition (ECMAScript 2026), they are in clause 28, as "28.1 The Reflect Object" and "28.2 Proxy Objects".[^4] Node v24, which runs the examples of this guide, has both.

## Pitfalls

### A proxy is not its target

```javascript wrong id=identity
const user = { name: "Ana" };
const known = new Set([user]);
const p = new Proxy(user, {});
console.log(known.has(p), p === user, p.name === user.name);
```

```output for=identity
false false true
```

The proxy gives the same values as the target, but it is a different object (rule r7). Keep the proxy, or the target, in one place, and compare the same kind of reference.

### Built-in objects and private fields

A proxy with an empty handler does not work for every object. MDN says that the "no-op" forwarding "does not work for native objects, such as DOM elements, Map objects, or anything that has internal slots."[^2] The same is true for classes with private fields. See [[javascript/proxies]] for examples and a fix.

## Comparison

Other languages give hooks for one operation at a time. Python has `__getattr__`, which is "Called when the default attribute access fails with an AttributeError".[^6] Ruby has `method_missing`, which is "Invoked by Ruby when obj is sent a message it cannot handle."[^7] A JavaScript proxy is a separate object that can replace all 13 operations, and the target stays unchanged (rule r3). For Python, see [[python/dynamic-attribute-access]]. For the design pattern, see [[proxy-pattern]].

## References
