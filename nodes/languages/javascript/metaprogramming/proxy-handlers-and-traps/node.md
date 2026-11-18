---
tag: W01RMT
kind: feature
owner: javascript
slug: proxy-handlers-and-traps
name: Proxy handlers and traps
aliases: [proxy traps, handler methods, get trap, set trap, has trap, ownKeys trap, apply trap, construct trap, proxy invariants, Table 29]
area: languages/javascript/metaprogramming
level: advanced
summary: >-
  A proxy handler can have 13 traps, one for each internal method, such as get for [[Get]] and apply for [[Call]]. The proxy checks the results against the target and throws if one breaks an invariant.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html#sec-proxy-object-internal-methods-and-internal-slots", locator: "10.5 Proxy Object Internal Methods and Internal Slots (Table 29: Proxy Handler Methods)"}
see_also: [javascript/proxies, javascript/meta-programming, javascript/reflection, javascript/revocable-proxies, getters-and-setters]
sources:
- url: https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 10
  locator: 10.5 Proxy Object Internal Methods and Internal Slots; 10.5.7 to 10.5.13
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Proxy/Proxy
  title: MDN Web Docs, JavaScript Reference, Proxy() constructor
  locator: Handler functions
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Proxy/Proxy/get
  title: MDN Web Docs, JavaScript Reference, handler.get()
  locator: the introduction; Parameters; Invariants
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Proxy/Proxy/set
  title: MDN Web Docs, JavaScript Reference, handler.set()
  locator: Return value
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Proxy/Proxy/ownKeys
  title: MDN Web Docs, JavaScript Reference, handler.ownKeys()
  locator: Return value; Invariants
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Proxy/Proxy/construct
  title: MDN Web Docs, JavaScript Reference, handler.construct()
  locator: the introduction; Return value
  accessed: 2026-11-18
- url: https://262.ecma-international.org/6.0/index.html
  title: ECMAScript 2015 Language Specification (ECMA-262, 6th edition)
  locator: 9.5 Proxy Object Internal Methods and Internal Slots
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Proxy
  title: MDN Web Docs, JavaScript Reference, Proxy
  locator: Description
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/datamodel.html
  title: The Python Language Reference, Python 3.14, Data model
  locator: 3.3.2. Customizing attribute access; 3.3.6. Emulating callable objects
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=all-traps
const seen = [];
const handler = {};
for (const trap of Object.getOwnPropertyNames(Reflect)) {
  handler[trap] = (...args) => {
    seen.push(trap);
    return Reflect[trap](...args);
  };
}
const p = new Proxy(function () {}, handler);

p.x = 1;
p.x;
"x" in p;
delete p.x;
Object.keys(p);
Object.getPrototypeOf(p);
Object.setPrototypeOf(p, Function.prototype);
p();
new p();
Object.isExtensible(p);
Object.preventExtensions(p);
console.log([...new Set(seen)].join(" "));
console.log(new Set(seen).size, Object.getOwnPropertyNames(Reflect).length);
```

```output for=all-traps
set getOwnPropertyDescriptor defineProperty get has deleteProperty ownKeys getPrototypeOf setPrototypeOf apply construct isExtensible preventExtensions
13 13
```

`Reflect` has one function for each trap, with the same name and the same arguments. So this handler has all 13 traps, and each trap records its name and does the default operation. The assignment `p.x = 1` runs three traps, because the default `[[Set]]` asks for the property and then defines it.

| Trap | Internal method | Operations that use it |
|---|---|---|
| `getPrototypeOf` | `[[GetPrototypeOf]]` | `Object.getPrototypeOf`, `instanceof` |
| `setPrototypeOf` | `[[SetPrototypeOf]]` | `Object.setPrototypeOf` |
| `isExtensible` | `[[IsExtensible]]` | `Object.isExtensible` |
| `preventExtensions` | `[[PreventExtensions]]` | `Object.preventExtensions`, `Object.freeze` |
| `getOwnPropertyDescriptor` | `[[GetOwnProperty]]` | `Object.getOwnPropertyDescriptor`, `Object.keys` |
| `defineProperty` | `[[DefineOwnProperty]]` | `Object.defineProperty` |
| `has` | `[[HasProperty]]` | `in` |
| `get` | `[[Get]]` | `obj.key`, `obj[key]` |
| `set` | `[[Set]]` | `obj.key = value` |
| `deleteProperty` | `[[Delete]]` | `delete obj.key` |
| `ownKeys` | `[[OwnPropertyKeys]]` | `Object.keys`, `Reflect.ownKeys`, `for...in` |
| `apply` | `[[Call]]` | `f(...)`, `f.call`, `f.apply` |
| `construct` | `[[Construct]]` | `new F(...)` |

## Semantics

**r1.** The specification gives one handler method for each internal method, in "Table 29: Proxy Handler Methods".[^1] "When a handler method is called to provide the implementation of a Proxy object internal method, the handler method is passed the proxy's target object as a parameter."[^1] Example: [`all-traps`](#all-traps).

**r2.** "Handler functions are sometimes called traps, because they trap calls to the underlying target object."[^2] A trap is optional. When the handler has no trap for an operation, the operation goes to the target.[^1]

**r3.** In a trap, `this` is the handler. MDN says it for each trap: "this is bound to the handler."[^3] The `get` and `set` traps also get the receiver, which "is usually either the proxy itself or an object that inherits from the proxy."[^3]

**r4.** A trap must return the right kind of value. "The set() method must return a Boolean indicating whether or not the assignment succeeded. Other values are coerced to booleans."[^4] "Many operations, including using property accessors in strict mode, throw a TypeError if the [[Set]] internal method returns false."[^4] "The ownKeys() method must return an array-like object where each element is either a String or a Symbol containing no duplicate items."[^5] "The construct() method must return an object, representing the newly created object."[^6] Examples: [`set-false`](#set-false), [`count-instances`](#count-instances).

**r5.** The proxy checks the result of most traps against the target. "Some of the internal method invariants defined in 6.1.7.3 are essential integrity invariants. These invariants are explicitly enforced by the Proxy object internal methods specified in this section."[^1] For `[[Get]]`: "The value reported for a property must be the same as the value of the corresponding target object property if the target object property is a non-writable, non-configurable own data property."[^1] For `[[OwnPropertyKeys]]`: "If the target object is not extensible, then the result List must contain all the keys of the own properties of the target object and no other values."[^1] Examples: [`frozen-value`](#frozen-value), [`missing-key`](#missing-key).

**r6.** The invariants protect only what the target cannot change: a property that is not configurable, or a target that is not extensible. For an ordinary property, a trap can report any value, hide the property or add new keys. Example: [`hide-private`](#hide-private).

**r7.** `apply` and `construct` exist only for a proxy of a function. "In order for the new operation to be valid on the resulting Proxy object, the target used to initialize the proxy must itself be a valid constructor."[^6] Example: [`count-instances`](#count-instances).

## Examples

```javascript run id=hide-private
const hide = (obj) =>
  new Proxy(obj, {
    has: (t, k) => !String(k).startsWith("_") && k in t,
    ownKeys: (t) => Reflect.ownKeys(t).filter((k) => !String(k).startsWith("_")),
    get: (t, k, r) => (String(k).startsWith("_") ? undefined : Reflect.get(t, k, r)),
  });

const user = hide({ name: "Ana", _token: "x1" });
console.log(Object.keys(user), "_token" in user, user._token, JSON.stringify(user));
```

```output for=hide-private
[ 'name' ] false undefined {"name":"Ana"}
```

Three traps hide the keys that start with `_`. `JSON.stringify` uses `ownKeys` and `get`, so it does not show the key either. The property `_token` is configurable and the target is extensible, so no invariant stops the traps (rule r6).

```javascript run id=count-instances
class Point {
  constructor(x, y) {
    this.x = x;
    this.y = y;
  }
}
const counted = new Proxy(Point, {
  construct(target, args, newTarget) {
    counted.made = (counted.made ?? 0) + 1;
    return Reflect.construct(target, args, newTarget);
  },
});
const p = new counted(1, 2);
new counted(3, 4);
console.log(p, p instanceof Point, counted.made);
```

```output for=count-instances
Point { x: 1, y: 2 } true 2
```

The `construct` trap counts each `new` and makes the object with `Reflect.construct` (rule r7). The count is a property of the proxy, and it goes to the class `Point`, because the handler has no `set` trap.

## Versions

ECMAScript 2015, the 6th edition, defined the proxy internal methods in "9.5 Proxy Object Internal Methods and Internal Slots".[^7] In the 17th edition (ECMAScript 2026), they are in 10.5, and Table 29 lists the 13 handler methods.[^1]

## Pitfalls

### A trap that breaks an invariant

```javascript fails id=frozen-value expect=exception:TypeError
const target = {};
Object.defineProperty(target, "id", { value: 7, writable: false, configurable: false });
const p = new Proxy(target, { get: () => 99 });
console.log(target.id);
console.log(p.id);
```

The property `id` cannot change, so the `get` trap must report 7 (rule r5). Node prints `7` for the target and then throws a `TypeError` for the proxy.

```javascript fails id=missing-key expect=exception:TypeError
const target = Object.preventExtensions({ a: 1, b: 2 });
const p = new Proxy(target, { ownKeys: () => ["a"] });
console.log(Object.keys(p));
```

The target is not extensible, so `ownKeys` must list all its keys (rule r5). Hide keys only on an extensible target, as in [`hide-private`](#hide-private).

### A set trap that returns false

```javascript fails id=set-false expect=exception:TypeError
"use strict";
const ro = new Proxy({ a: 1 }, { set: () => false });
console.log(Reflect.set(ro, "a", 2), ro.a);
ro.a = 3;
```

`Reflect.set` reports the `false` result, but an assignment in strict mode code throws (rule r4). A trap that forgets `return true` returns `undefined`, which is `false`, so each assignment in a module or a class throws.

### A trap that uses the proxy

```javascript fails id=recursion expect=exception:RangeError
const p = new Proxy({}, {
  get(target, key, receiver) {
    return receiver[key] ?? "none";
  },
});
console.log(p.name);
```

The receiver is the proxy, so `receiver[key]` runs the `get` trap again, with no end. MDN: "If you use Reflect methods within a proxy trap, and the Reflect method call gets intercepted by the trap again, there may be infinite recursion."[^8] Read from `target`, or call `Reflect.get(target, key, receiver)`.

## Comparison

The 13 traps are the full list of operations on a JavaScript object, so a proxy can replace anything that code does with the object (rule r1). Python has a separate special method for each operation, and a class defines them for its own objects. For example, `__setattr__` is "Called when an attribute assignment is attempted."[^9] and `__call__` is called "when the instance is “called” as a function".[^9] A JavaScript handler is a separate object, so one handler can wrap any target. For the default operations, see [[javascript/reflection]].

## References
