---
tag: 947VKB
kind: feature
owner: javascript
slug: weakmap-and-weakset
name: WeakMap and WeakSet
aliases: [WeakMap, WeakSet, weak keys, CanBeHeldWeakly, private data with WeakMap, weak cache]
area: languages/javascript/collections
level: advanced
summary: >-
  A WeakMap or WeakSet holds objects and non-registered symbols without keeping them alive. Entries go away with their keys, so the collections have no size and cannot be iterated.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/keyed-collections.html#sec-weakmap-objects", locator: "24.3 WeakMap Objects; 24.4 WeakSet Objects"}
see_also: [javascript/keyed-collections, javascript/maps, javascript/sets, garbage-collection]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WeakMap
  title: MDN Web Docs, JavaScript Reference, WeakMap
  locator: the introduction; Description; Why WeakMap?; Examples
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WeakSet
  title: MDN Web Docs, JavaScript Reference, WeakSet
  locator: Description; Detecting circular references
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/executable-code-and-execution-contexts.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 9
  locator: 9.13 CanBeHeldWeakly
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/keyed-collections.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 24
  locator: 24.3 WeakMap Objects; 24.4 WeakSet Objects
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Symbols as WeakMap keys, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/weakref.html
  title: The Python Standard Library, Python 3.14, weakref
  locator: WeakKeyDictionary
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/WeakHashMap.html
  title: Java SE 25 API, Class WeakHashMap
  locator: the class description
  accessed: 2026-11-18
- url: https://pkg.go.dev/weak
  title: Go standard library, package weak
  locator: the package overview
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const metadata = new WeakMap();
const visited = new WeakSet();

const node = { name: "root" };
metadata.set(node, { created: 1 });
visited.add(node);

console.log(metadata.get(node), metadata.has(node), visited.has(node));
console.log(visited.has({ name: "root" }), metadata.delete(node), metadata.has(node));
console.log(metadata.size, typeof metadata.keys, typeof visited.forEach);
```

```output for=basics
{ created: 1 } true true
false true false
undefined undefined undefined
```

| | `WeakMap` | `WeakSet` |
|---|---|---|
| holds | key and value pairs | values |
| keys or values | objects and non-registered symbols | objects and non-registered symbols |
| methods | `get`, `set`, `has`, `delete` | `add`, `has`, `delete` |
| `size`, iteration, `clear` | none | none |

## Semantics

**r1.** "A WeakMap is a collection of key/value pairs whose keys must be objects or non-registered symbols, with values of any arbitrary JavaScript type, and which does not create strong references to its keys."[^1] "Once an object used as a key has been collected, its corresponding values in any WeakMap become candidates for garbage collection as well".[^1] A `WeakSet` does the same for its values: "If no other references to a value stored in the WeakSet exist, those values can be garbage collected."[^2]

**r2.** The specification lists what can be held: "Only values that are suitable for use as a weak reference may be a key of a WeakMap, an element of a WeakSet, the target of a WeakRef, or one of the targets of a FinalizationRegistry." `CanBeHeldWeakly` returns true for an object, and for a symbol when `KeyForSymbol(v) is undefined`.[^3] A symbol from `Symbol.for` "does not have language identity and is unsuitable for use as a weak reference."[^3] Example: [`keys`](#keys).

**r3.** The collections cannot be listed. For a WeakMap "no mechanism is provided for enumerating the values it holds as keys".[^4] MDN gives the reason: "if a WeakMap exposed any method to obtain a list of its keys, the list would depend on the state of garbage collection, introducing non-determinism."[^1] The specification also lets the engine wait: "An implementation may impose an arbitrarily determined latency between the time a key/value pair of a WeakMap becomes inaccessible and the time when the key/value pair is removed from the WeakMap."[^4]

**r4.** Keys compare by identity. "Like regular Map , value equality is based on the SameValueZero algorithm, which is the same as the === operator because WeakMap can only hold object and symbol keys."[^1] Example: [`basics`](#basics).

**r5.** A WeakMap ties extra data to an object "without affecting the lifetime of the object itself."[^1] MDN lists three uses: private members, metadata on objects that other code owns, and caching the result of a function per input object.[^1] Example: [`cache`](#cache).

**r6.** A WeakSet suits a mark of objects already seen. MDN uses one to stop recursion on "circular data structures by tracking which objects have already been processed."[^2] Example: [`cycles`](#cycles).

## Examples

What a weak collection accepts (rule r2).

```javascript run id=keys
const wm = new WeakMap();
for (const key of [{}, () => {}, Symbol("local"), Symbol.for("shared"), "text", 1]) {
  try {
    wm.set(key, true);
    console.log("ok", typeof key);
  } catch (e) {
    console.log(e.name, typeof key);
  }
}
```

```output for=keys
ok object
ok function
ok symbol
TypeError symbol
TypeError string
TypeError number
```

A cache per input object (rule r5).

```javascript run id=cache
const cache = new WeakMap();
let calls = 0;
function total(order) {
  if (cache.has(order)) return cache.get(order);
  calls++;
  const result = order.items.reduce((sum, item) => sum + item.price, 0);
  cache.set(order, result);
  return result;
}

const order = { items: [{ price: 3 }, { price: 4 }] };
console.log(total(order), total(order), calls);
console.log(total({ items: [{ price: 3 }, { price: 4 }] }), calls);
```

```output for=cache
7 7 1
7 2
```

When `order` is no longer used, the cached total can be collected with it. A `Map` would keep both alive.

A WeakSet that stops a walk at a cycle (rule r6).

```javascript run id=cycles
function names(subject, seen = new WeakSet()) {
  if (seen.has(subject)) return ["(cycle)"];
  seen.add(subject);
  const out = [subject.name];
  for (const child of subject.children) out.push(...names(child, seen));
  return out;
}

const root = { name: "root", children: [] };
const leaf = { name: "leaf", children: [root] };
root.children.push(leaf);
console.log(names(root));
```

```output for=cycles
[ 'root', 'leaf', '(cycle)' ]
```

## Versions

The TC39 list of finished proposals gives Symbols as WeakMap keys the expected publication year 2023, so ECMAScript 2023 is the edition that accepts non-registered symbols as `WeakMap` keys and `WeakSet` values.[^5]

## Pitfalls

### Expecting to list or count the entries

A program that needs the keys or a count needs a `Map` or a `Set`: "If you want to have a list of keys, you should use a Map rather than a WeakMap ."[^1]

```javascript wrong id=list
const wm = new WeakMap([[{}, 1]]);
try {
  for (const entry of wm) console.log(entry);
} catch (e) {
  console.log(e.name);
}
console.log(wm.size, [...new Map([[{}, 1]]).values()]);
```

```output for=list
TypeError
undefined [ 1 ]
```

### A value that refers to its own key

A WeakMap keeps a value alive only while its key is alive: "a key object refers strongly to its contents as long as the key is not garbage collected, but weakly from then on."[^1] So a value may refer to its key without a leak. In a `Map`, the same reference keeps both alive as long as the map is. No run can show garbage collection reliably (rule r3), so this block only shows the shape of the code.

```javascript wrong id=self
const strong = new Map();
const weak = new WeakMap();
let el = { id: "button" };
strong.set(el, { owner: el });
weak.set(el, { owner: el });
el = null;
console.log(strong.size, typeof weak.size);
```

```output for=self
1 undefined
```

`strong` still holds the object after `el = null`, and `strong.size` shows it. `weak` lets the object be collected.

## Comparison

Python has the same idea in a module: `WeakKeyDictionary` is a "Mapping class that references keys weakly. Entries in the dictionary will be discarded when there is no longer a strong reference to the key."[^6] Java's `WeakHashMap` is a "Hash table based implementation of the Map interface, with weak keys", and an entry "will automatically be removed when its key is no longer in ordinary use."[^7] Go's package `weak` "provides ways to safely reference memory weakly, that is, without preventing its reclamation."[^8] Unlike the Python and Java maps, a JavaScript `WeakMap` has no way to list or count its entries (rule r3). See [[garbage-collection]].

## References
