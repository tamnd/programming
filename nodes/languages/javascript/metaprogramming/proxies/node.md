---
tag: 5XZBTD
kind: feature
owner: javascript
slug: proxies
name: Proxies
aliases: [Proxy, new Proxy, Proxy object, proxy target, proxy handler, forwarding proxy, validation proxy, logging proxy]
area: languages/javascript/metaprogramming
level: advanced
summary: >-
  new Proxy(target, handler) makes an object that stands in for the target. The methods of the handler replace operations such as get, set and call, and the other operations go to the target.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/reflection.html#sec-proxy-objects", locator: "28.2 Proxy Objects; 28.2.1.1 Proxy ( target, handler )"}
see_also: [javascript/meta-programming, javascript/proxy-handlers-and-traps, javascript/revocable-proxies, javascript/reflection, proxy-pattern, getters-and-setters]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Proxy/Proxy
  title: MDN Web Docs, JavaScript Reference, Proxy() constructor
  locator: Parameters; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Proxy
  title: MDN Web Docs, JavaScript Reference, Proxy
  locator: the introduction; Description; Examples (No private field forwarding; Validation)
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 10
  locator: 10.5.12 [[Call]]; 10.5.15 ProxyCreate
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/reflection.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 28
  locator: 28.2.1 The Proxy Constructor; 28.2.2 Properties of the Proxy Constructor
  accessed: 2026-11-18
- url: https://262.ecma-international.org/6.0/index.html
  title: ECMAScript 2015 Language Specification (ECMA-262, 6th edition)
  locator: 26.2 Proxy Objects
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/reflect/Proxy.html
  title: Java SE 25 API, Class Proxy
  locator: the class description
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=validate
const person = new Proxy(
  {},
  {
    set(target, key, value) {
      if (key === "age" && !Number.isInteger(value)) {
        throw new TypeError(`age must be an integer, got ${value}`);
      }
      target[key] = value;
      return true;
    },
  },
);

person.age = 30;
person.name = "Bo";
console.log(person.age, person.name);
try {
  person.age = "old";
} catch (e) {
  console.log(e.message);
}
```

```output for=validate
30 Bo
age must be an integer, got old
```

The `set` trap checks each assignment before it stores the value in the target. It returns `true` to report that the assignment worked.

```text
new Proxy(target, handler)
```

| Argument | Rule |
|---|---|
| `target` | any object, "including a native array, a function, or even another proxy"[^1] |
| `handler` | an object whose methods are the traps; `{}` gives a proxy that forwards each operation |

## Semantics

**r1.** "The Proxy object allows you to create an object that can be used in place of the original object, but which may redefine fundamental Object operations like getting, setting, and defining properties. Proxy objects are commonly used to log property accesses, validate, format, or sanitize inputs, and so on."[^2] Example: [`validate`](#validate).

**r2.** Both arguments must be objects. ProxyCreate says: "If target is not an Object, throw a TypeError exception." and "If handler is not an Object, throw a TypeError exception."[^3] The constructor works only with `new`: "If NewTarget is undefined, throw a TypeError exception."[^4]

**r3.** "An empty handler will create a proxy that behaves, in almost all respects, exactly like the target."[^1] Each operation with no trap goes to the target. Example: [`forward`](#forward).

**r4.** A trap gets the target as its first argument. For `get`, it also gets the key and the receiver, which is the proxy or an object that inherits from it. The trap can use `Reflect` to do the default operation: "The Reflect methods provide the reflective semantics for invoking the corresponding object internal methods."[^2] Example: [`negative-index`](#negative-index).

**r5.** A proxy can be called only if its target can be called. "A Proxy exotic object only has a [[Call]] internal method if the initial value of its [[ProxyTarget]] internal slot is an object that has a [[Call]] internal method."[^3] So a proxy of a function can have an `apply` trap, and `typeof` gives `"function"` for it. Examples: [`trace-calls`](#trace-calls), [`not-callable`](#not-callable).

**r6.** A proxy has no class of its own. The Proxy constructor "does not have a "prototype" property because Proxy objects do not have a [[Prototype]] internal slot that requires initialization."[^4] `Object.getPrototypeOf` of a proxy gives the prototype of the target, unless a trap changes it.

**r7.** A proxy does not get the private fields or the internal slots of its target. "As such, the proxy does not have direct access to the original object's private elements."[^2] "Some native JavaScript objects have properties called internal slots, which are not accessible from JavaScript code."[^2] Examples: [`private-field`](#private-field), [`map-slots`](#map-slots).

## Examples

```javascript run id=forward
const target = {};
const p = new Proxy(target, {});
p.a = 37;
console.log(target.a, p.a, p === target, typeof p);
console.log(Object.getPrototypeOf(p) === Object.prototype);
const arr = new Proxy([1, 2, 3], {});
console.log(Array.isArray(arr), arr.length);
```

```output for=forward
37 37 false object
true
true 3
```

With an empty handler, the assignment goes to the target (rule r3). The proxy of an array is an array for `Array.isArray` too.

```javascript run id=negative-index
const negative = (arr) =>
  new Proxy(arr, {
    get(target, key, receiver) {
      const i = typeof key === "string" ? Number(key) : NaN;
      if (Number.isInteger(i) && i < 0) {
        key = String(target.length + i);
      }
      return Reflect.get(target, key, receiver);
    },
  });

const xs = negative(["a", "b", "c"]);
console.log(xs[-1], xs[-3], xs[0], xs.length, xs.at(-1));
```

```output for=negative-index
c a a 3 c
```

Property keys are strings or symbols, so the trap converts the key to a number. It changes a negative index and gives the other keys, such as `length` and `at`, to `Reflect.get` (rule r4).

```javascript run id=trace-calls
function sum(a, b) {
  return a + b;
}
const traced = new Proxy(sum, {
  apply(target, thisArg, args) {
    const result = Reflect.apply(target, thisArg, args);
    console.log(`sum(${args.join(", ")}) = ${result}`);
    return result;
  },
});
traced(2, 3);
console.log(typeof traced, traced.name, traced.length);
```

```output for=trace-calls
sum(2, 3) = 5
function sum 2
```

The `apply` trap runs for each call (rule r5). The reads of `name` and `length` have no trap, so they give the values of `sum`.

## Versions

ECMAScript 2015, the 6th edition, added the `Proxy` constructor in "26.2 Proxy Objects".[^5] In the 17th edition (ECMAScript 2026), the same constructor is in 28.2.[^4] Node v24 runs all the examples on this page.

## Pitfalls

### An apply trap on a proxy of a plain object

```javascript fails id=not-callable expect=exception:TypeError
const p = new Proxy({}, {
  apply() {
    return "called";
  },
});
console.log(typeof p);
p();
```

The target is not a function, so the proxy has no `[[Call]]` method and the trap never runs (rule r5). Use a function as the target.

### Private fields

```javascript fails id=private-field expect=exception:TypeError
class Secret {
  #value;
  constructor(value) {
    this.#value = value;
  }
  get value() {
    return this.#value;
  }
}

const p = new Proxy(new Secret("s3"), {});
console.log(p.value);
```

The getter runs with the proxy as `this`, and the proxy does not have the field `#value` (rule r7). The trap can call the getter on the target in place of the proxy, as MDN shows.[^2]

### Built-in objects with internal slots

```javascript fails id=map-slots expect=exception:TypeError
const m = new Proxy(new Map([["a", 1]]), {});
console.log(m.size);
```

The getter of `size` needs the internal slot of a real `Map` (rule r7). MDN: "As such, you cannot trivially create a forwarding proxy for a map".[^2] Run the method on the target:

```javascript run id=map-fix
const map = new Map([["a", 1]]);
const m = new Proxy(map, {
  get(target, key) {
    const value = Reflect.get(target, key, target);
    return typeof value === "function" ? value.bind(target) : value;
  },
});
console.log(m.size, m.get("a"), m.has("b"));
```

```output for=map-fix
1 1 false
```

## Comparison

Java has dynamic proxies too, but only for interfaces: "Proxy provides static methods for creating objects that act like instances of interfaces but allow for customized method invocation."[^6] A Java proxy replaces method calls. A JavaScript proxy can also replace property reads, assignments, `in`, `delete` and the list of keys, because these are all internal methods (rule r1). For the design pattern, see [[proxy-pattern]].

## References
