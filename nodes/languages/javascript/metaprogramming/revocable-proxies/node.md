---
tag: QH7PC3
kind: feature
owner: javascript
slug: revocable-proxies
name: Revocable proxies
aliases: [Proxy.revocable, revoke, revoked proxy, membrane, membranes, object capability, revocable reference]
area: languages/javascript/metaprogramming
level: advanced
summary: >-
  Proxy.revocable(target, handler) returns a proxy and a revoke function. After revoke(), each operation on the proxy throws a TypeError. A membrane wraps each object that comes out of it.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/reflection.html#sec-proxy.revocable", locator: "28.2.2.1 Proxy.revocable ( target, handler )"}
see_also: [javascript/proxies, javascript/proxy-handlers-and-traps, javascript/meta-programming, javascript/weakmap-and-weakset, proxy-pattern]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Proxy/revocable
  title: MDN Web Docs, JavaScript Reference, Proxy.revocable()
  locator: the introduction; Return value; Description
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/reflection.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 28
  locator: 28.2.2.1 Proxy.revocable ( target, handler )
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 10
  locator: 10.5 Proxy Object Internal Methods and Internal Slots; 10.5.14 ValidateNonRevokedProxy
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Meta_programming
  title: MDN Web Docs, JavaScript Guide, Meta programming
  locator: Revocable Proxy
  accessed: 2026-11-18
- url: https://262.ecma-international.org/6.0/index.html
  title: ECMAScript 2015 Language Specification (ECMA-262, 6th edition)
  locator: 26.2.2.1 Proxy.revocable ( target, handler )
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=revoke
const { proxy, revoke } = Proxy.revocable({ secret: 42 }, {});
console.log(proxy.secret);
revoke();
try {
  proxy.secret;
} catch (e) {
  console.log(e.constructor.name, e.message);
}
console.log(typeof proxy);
revoke();
```

```output for=revoke
42
TypeError Cannot perform 'get' on a proxy that has been revoked
object
```

The proxy works until `revoke()` runs. After that, the read throws. `typeof` still works, because it does not call a trap, and a second call of `revoke` does nothing.

```text
const { proxy, revoke } = Proxy.revocable(target, handler);
```

## Semantics

**r1.** "The Proxy.revocable() factory function is the same as the Proxy() constructor, except that in addition to creating a proxy object, it also creates a revoke function that can be called to disable the proxy. The proxy object and the revoke function are wrapped in a plain object."[^1] The specification makes this object with two properties, `"proxy"` and `"revoke"`.[^2]

**r2.** Revoking sets the two slots of the proxy to null. "When a proxy is revoked, its [[ProxyHandler]] and [[ProxyTarget]] internal slots are set to null causing subsequent invocations of internal methods on that Proxy object to throw a TypeError exception."[^3] Each internal method starts with ValidateNonRevokedProxy, which "throws a TypeError exception if proxy has been revoked."[^3] Example: [`revoke`](#revoke).

**r3.** "Once a proxy is revoked, it remains revoked, and calling revoke() again has no effect".[^1] The revoke function "does not take any parameters, nor does it rely on the this value."[^1] So the code can pass `revoke` as a callback, as in [`lease`](#lease).

**r4.** Revoking lets the target go. "Letting users interact with an object through a revocable proxy allows you to control the lifetime of the object exposed to the user".[^1] MDN adds that the object can be garbage-collected while the user still holds its proxy.[^1] The revoked proxy no longer refers to the target (rule r2), so the garbage collector can take the target when no other code refers to it.

**r5.** Some operations do not use an internal method, so they work on a revoked proxy. MDN notes that `typeof` "doesn't trigger any trap".[^4] Example: [`revoke`](#revoke).

**r6.** Revoking cuts off only the proxy itself. An object that the code read through the proxy before the revoke is the real object, and it stays usable. A membrane fixes this: it wraps each object that leaves the proxy in a new revocable proxy, and it revokes all of them together. Examples: [`leak`](#leak), [`membrane`](#membrane).

## Examples

```javascript run id=lease
function lend(resource, ms) {
  const { proxy, revoke } = Proxy.revocable(resource, {});
  setTimeout(revoke, ms);
  return proxy;
}

const db = { query: (q) => `rows for ${q}` };
const lease = lend(db, 10);
console.log(lease.query("a"));
setTimeout(() => {
  try {
    lease.query("b");
  } catch (e) {
    console.log("revoked:", e.constructor.name);
  }
}, 30);
```

```output for=lease
rows for a
revoked: TypeError
```

The code gives access to `db` for 10 milliseconds. The timer calls `revoke` with no `this` and no arguments (rule r3).

```javascript run id=membrane
function membrane(root) {
  const revokes = [];
  const wrap = (value) => {
    if (value === null || (typeof value !== "object" && typeof value !== "function")) {
      return value;
    }
    const { proxy, revoke } = Proxy.revocable(value, {
      get: (t, k) => wrap(Reflect.get(t, k)),
      apply: (t, self, args) => wrap(Reflect.apply(t, self, args)),
    });
    revokes.push(revoke);
    return proxy;
  };
  return { root: wrap(root), revokeAll: () => revokes.forEach((r) => r()) };
}

const app = { config: { port: 8080 }, getUser: () => ({ name: "Ana" }) };
const { root, revokeAll } = membrane(app);
const config = root.config;
const user = root.getUser();
console.log(config.port, user.name);
revokeAll();
for (const p of [root, config, user]) {
  try {
    p.x;
  } catch (e) {
    console.log("blocked", e.constructor.name);
  }
}
```

```output for=membrane
8080 Ana
blocked TypeError
blocked TypeError
blocked TypeError
```

Each object that the code gets from `root`, by a read or by a call, is a revocable proxy too. One call of `revokeAll` cuts off `root`, `config` and `user` (rule r6). A full membrane also wraps the arguments that go in and keeps one proxy for each object, but the idea is the same.

## Versions

ECMAScript 2015, the 6th edition, added the function as "26.2.2.1 Proxy.revocable ( target, handler )".[^5] The 17th edition (ECMAScript 2026) has it as 28.2.2.1.[^2] Node v24 runs all the examples on this page.

## Pitfalls

### Objects read before the revoke

```javascript wrong id=leak
const app = { config: { port: 8080 } };
const { proxy, revoke } = Proxy.revocable(app, {});
const config = proxy.config;
revoke();
console.log(config.port, config === app.config);
```

```output for=leak
8080 true
```

`proxy.config` gave the real object, so the revoke does not stop the code that holds it (rule r6). Use a membrane, as in [`membrane`](#membrane), when the code must lose access to all the objects.

## Comparison

A revocable proxy is a reference that its owner can take back. In Java or Python, a program writes a wrapper class with a flag and tests the flag in each method. In JavaScript, the engine does the test for each of the 13 internal methods (rule r2). For proxies in general, see [[javascript/proxies]].

## References
