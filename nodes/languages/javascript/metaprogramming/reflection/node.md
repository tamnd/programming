---
tag: DVDVYV
kind: feature
owner: javascript
slug: reflection
name: Reflection
aliases: [Reflect, the Reflect object, Reflect.get, Reflect.set, Reflect.ownKeys, Reflect.construct, Reflect.apply, Reflect and Object methods, forwarding in traps]
area: languages/javascript/metaprogramming
level: advanced
summary: >-
  Reflect has 13 functions, one for each proxy trap, that do the default operation. They return false where Object methods throw, and they take a receiver or a new.target.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/reflection.html#sec-reflect-object", locator: "28.1 The Reflect Object (28.1.1 to 28.1.14)"}
see_also: [javascript/meta-programming, javascript/proxies, javascript/proxy-handlers-and-traps, javascript/property-descriptors, getters-and-setters]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Reflect
  title: MDN Web Docs, JavaScript Reference, Reflect
  locator: Description; Static methods
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/reflection.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 28
  locator: 28.1 The Reflect Object; 28.1.2 Reflect.construct; 28.1.5 Reflect.get; 28.1.7 Reflect.getPrototypeOf; 28.1.14 Reflect [ %Symbol.toStringTag% ]
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Meta_programming
  title: MDN Web Docs, JavaScript Guide, Meta programming
  locator: Reflection
  accessed: 2026-11-18
- url: https://262.ecma-international.org/6.0/index.html
  title: ECMAScript 2015 Language Specification (ECMA-262, 6th edition)
  locator: 26.1 The Reflect Object; 26.1.5 Reflect.enumerate
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=own-keys
const s = Symbol("id");
const obj = { a: 1, [s]: 2 };
Object.defineProperty(obj, "hidden", { value: 3, enumerable: false });
console.log(Object.keys(obj));
console.log(Reflect.ownKeys(obj));
console.log(Reflect.has(obj, "a"), Reflect.get(obj, s));
console.log(Object.getOwnPropertyNames(Reflect).length, String(Reflect));
```

```output for=own-keys
[ 'a' ]
[ 'a', 'hidden', Symbol(id) ]
true 2
13 [object Reflect]
```

`Object.keys` gives only the enumerable string keys. `Reflect.ownKeys` gives all the own keys, with the symbols. `Reflect` has 13 functions, and its tag is `"Reflect"`.

| Function | Same operation as |
|---|---|
| `Reflect.get(t, k, receiver?)` | `t[k]` |
| `Reflect.set(t, k, v, receiver?)` | `t[k] = v` |
| `Reflect.has(t, k)` | `k in t` |
| `Reflect.deleteProperty(t, k)` | `delete t[k]` |
| `Reflect.ownKeys(t)` | `Object.getOwnPropertyNames` and `Object.getOwnPropertySymbols` together |
| `Reflect.defineProperty(t, k, d)` | `Object.defineProperty`, but returns a boolean |
| `Reflect.getOwnPropertyDescriptor(t, k)` | `Object.getOwnPropertyDescriptor` |
| `Reflect.getPrototypeOf(t)` | `Object.getPrototypeOf` |
| `Reflect.setPrototypeOf(t, p)` | `Object.setPrototypeOf`, but returns a boolean |
| `Reflect.isExtensible(t)` | `Object.isExtensible` |
| `Reflect.preventExtensions(t)` | `Object.preventExtensions`, but returns a boolean |
| `Reflect.apply(f, self, args)` | `f.apply(self, args)` |
| `Reflect.construct(F, args, newTarget?)` | `new F(...args)` |

## Semantics

**r1.** "The Reflect namespace object contains static methods for invoking interceptable JavaScript object internal methods. The methods are the same as those of proxy handlers."[^1] Each function calls one internal method. For example, `Reflect.ownKeys` does "Let keys be ? target.[[OwnPropertyKeys]]()."[^2] Example: [`own-keys`](#own-keys).

**r2.** `Reflect` is a plain object, not a class. It "is not a function object", it "cannot be used as a constructor with the new operator" and it "cannot be invoked as a function."[^2] MDN: "All properties and methods of Reflect are static (just like the Math object)."[^1] Example: [`not-constructor`](#not-constructor).

**r3.** "The major use case of Reflect is to provide default forwarding behavior in Proxy handler traps."[^1] A trap and its `Reflect` function have the same name and the same arguments, so a trap can pass its arguments to `Reflect` with no change. Examples: [`forward-receiver`](#forward-receiver), [`lost-receiver`](#lost-receiver).

**r4.** The functions that change an object report the result. MDN says that `Reflect.defineProperty` "Returns a boolean that is true if the property was successfully defined."[^1] `Reflect.set`, `Reflect.setPrototypeOf` and `Reflect.preventExtensions` also return a boolean. The `Object` functions with the same names throw a `TypeError`, or return the object. MDN: "Because Reflect.defineProperty() returns a Boolean success status, you can just use an if...else block here".[^3] Example: [`booleans`](#booleans).

**r5.** Each function, except `apply` and `construct`, starts with the same step: "If target is not an Object, throw a TypeError exception."[^2] Some `Object` functions convert a primitive to an object first, so `Object.getPrototypeOf` works for a string, and `Reflect.getPrototypeOf` throws for it. Example: [`non-object`](#non-object).

**r6.** `Reflect.get` and `Reflect.set` take a receiver, which is the `this` value for a getter or a setter. "If receiver is not present, then Set receiver to target."[^2] MDN: "Reflect.get() allows you to run a getter with a custom this value, while property accessors always use the current object as the this value."[^1] Example: [`receiver`](#receiver).

**r7.** `Reflect.construct` takes a third argument, the `new.target`. "Reflect.construct() is the only way to construct a target function with a specific new.target value."[^1] "If newTarget is not present, set newTarget to target."[^2] Example: [`construct`](#construct).

## Examples

```javascript run id=booleans
const frozen = Object.freeze({ x: 1 });
console.log(Reflect.set(frozen, "x", 2));
console.log(Reflect.defineProperty(frozen, "y", { value: 1 }));
console.log(Reflect.deleteProperty(frozen, "x"));
console.log(Reflect.setPrototypeOf(frozen, null));
if (!Reflect.defineProperty(frozen, "y", { value: 1 })) {
  console.log("could not add y");
}
```

```output for=booleans
false
false
false
false
could not add y
```

The object is frozen, so each change fails, and each function returns `false` (rule r4). The code tests the result with `if` and needs no `try`.

```javascript run id=receiver
const proto = {
  get who() {
    return this.name;
  },
};
const child = { name: "child" };
console.log(Reflect.get(proto, "who"), Reflect.get(proto, "who", child));
const store = {};
const setter = {
  set v(x) {
    this.value = x;
  },
};
Reflect.set(setter, "v", 5, store);
console.log(store, setter.value);
```

```output for=receiver
undefined child
{ value: 5 } undefined
```

With no receiver, the getter runs with `proto` as `this`, and `proto` has no `name`. With `child` as the receiver, the getter reads `child.name`. The setter runs with `store` as `this`, so the value goes to `store` (rule r6).

```javascript run id=construct
class Base {
  constructor() {
    this.made = new.target.name;
  }
}
class Child extends Base {}
const a = Reflect.construct(Base, []);
const b = Reflect.construct(Base, [], Child);
console.log(a.made, b.made, b instanceof Child);
console.log(Reflect.apply(Math.max, null, [3, 9, 4]));
```

```output for=construct
Base Child true
9
```

The constructor of `Base` runs both times. With `Child` as the third argument, `new.target` is `Child`, and the new object gets the prototype of `Child` (rule r7). `Reflect.apply` calls a function with a `this` value and an array of arguments.

```javascript run id=forward-receiver
const user = {
  first: "Ana",
  last: "Lee",
  get full() {
    return `${this.first} ${this.last}`;
  },
};
const reads = [];
const p = new Proxy(user, {
  get(target, key, receiver) {
    reads.push(key);
    return Reflect.get(target, key, receiver);
  },
});
console.log(p.full);
console.log(reads);
```

```output for=forward-receiver
Ana Lee
[ 'full', 'first', 'last' ]
```

The trap gives its receiver, the proxy, to `Reflect.get`. So the getter runs with the proxy as `this`, and its reads of `first` and `last` go through the trap too (rules r3 and r6).

## Versions

ECMAScript 2015, the 6th edition, added the object as "26.1 The Reflect Object".[^4] That edition also had "26.1.5 Reflect.enumerate ( target )".[^4] The 17th edition (ECMAScript 2026) has the object in 28.1, with 13 functions and no `Reflect.enumerate`.[^2] Node v24 runs all the examples on this page.

## Pitfalls

### A trap that forwards with target[key]

```javascript wrong id=lost-receiver
const user = {
  first: "Ana",
  last: "Lee",
  get full() {
    return `${this.first} ${this.last}`;
  },
};
const reads = [];
const p = new Proxy(user, {
  get(target, key) {
    reads.push(key);
    return target[key];
  },
});
console.log(p.full);
console.log(reads);
```

```output for=lost-receiver
Ana Lee
[ 'full' ]
```

`target[key]` runs the getter with the target as `this`, so the trap does not see the reads of `first` and `last`. The value is correct here, but a trap that changes or checks values does not run for them. Forward with `Reflect.get(target, key, receiver)`, as in [`forward-receiver`](#forward-receiver).

### Reflect is not a class

```javascript fails id=not-constructor expect=exception:TypeError
console.log(typeof Reflect);
new Reflect();
```

`Reflect` is an ordinary object with functions, like `Math` (rule r2). Call its functions directly.

### A primitive target

```javascript fails id=non-object expect=exception:TypeError
console.log(Object.getPrototypeOf("text") === String.prototype);
Reflect.getPrototypeOf("text");
```

`Object.getPrototypeOf` converts the string to an object, but `Reflect.getPrototypeOf` throws (rule r5). Test the type first when the value can be a primitive.

## Comparison

MDN: "Nearly every Reflect method's behavior can be done with some other syntax or method."[^1] So `Reflect` adds little new power. It gives each internal method as a function with a fixed form: the target first, a boolean result for each change, and a receiver where a getter or a setter can run. That form matches the traps, so `Reflect` is the normal way to write the default case of a trap (rule r3). In many other languages, reflection means that a program reads the classes, fields and methods of a type at run time. In JavaScript, properties are the data of an object, so `Reflect.ownKeys` and `Reflect.getOwnPropertyDescriptor` do most of that work. For the traps, see [[javascript/proxy-handlers-and-traps]].

## References
