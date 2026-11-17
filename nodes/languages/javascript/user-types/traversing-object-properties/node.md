---
tag: P244S7
kind: feature
owner: javascript
slug: traversing-object-properties
name: Traversing object properties
aliases: [for...in, Object.keys, Object.values, Object.entries, Object.fromEntries, Reflect.ownKeys, iterating over an object, property order]
area: languages/javascript/user-types
level: core
summary: >-
  JavaScript walks an object's properties with for...in, which includes inherited ones, or with Object.keys, Object.values and Object.entries, which give own enumerable ones. Integer keys come first.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html#sec-ordinaryownpropertykeys", locator: "10.1.11.1 OrdinaryOwnPropertyKeys; 14.7.5.9 EnumerateObjectProperties"}
see_also: [javascript/enumerability-and-ownership-of-properties, javascript/querying-object-properties, javascript/maps, javascript/objects]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/for...in
  title: MDN Web Docs, JavaScript Reference, for...in
  locator: the introduction; Description; Deleted, added, or modified properties; Array iteration and for...in
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/entries
  title: MDN Web Docs, JavaScript Reference, Object.entries()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/values
  title: MDN Web Docs, JavaScript Reference, Object.values()
  locator: the introduction
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Reflect/ownKeys
  title: MDN Web Docs, JavaScript Reference, Reflect.ownKeys()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 10
  locator: 10.1.11.1 OrdinaryOwnPropertyKeys
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-statements-and-declarations.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 14
  locator: 14.7.5.9 EnumerateObjectProperties
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/fromEntries
  title: MDN Web Docs, JavaScript Reference, Object.fromEntries()
  locator: the introduction
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Map
  title: MDN Web Docs, JavaScript Reference, Map
  locator: the introduction; Key Order
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Object.values/Object.entries, Object.fromEntries, for-in mechanics, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/datamodel.html
  title: The Python Language Reference, Data model, Python 3.14
  locator: 3.2.11.1 Special attributes
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const scores = { ada: 9, alan: 7 };
for (const key in scores) console.log(key, scores[key]);
for (const [name, score] of Object.entries(scores)) console.log(`${name}=${score}`);
console.log(Object.keys(scores), Object.values(scores));
```

```output for=basics
ada 9
alan 7
ada=9
alan=7
[ 'ada', 'alan' ] [ 9, 7 ]
```

| Form | Gives | Inherited | Non-enumerable | Symbol keys |
|---|---|---|---|---|
| `for (const k in obj)` | keys, one at a time | yes | no | no |
| `Object.keys(obj)` | an array of keys | no | no | no |
| `Object.values(obj)` | an array of values | no | no | no |
| `Object.entries(obj)` | an array of `[key, value]` pairs | no | no | no |
| `Object.getOwnPropertyNames(obj)` | an array of keys | no | yes | no |
| `Reflect.ownKeys(obj)` | an array of keys | no | yes | yes |

## Semantics

**r1.** "The for...in statement iterates over all enumerable string properties of an object (ignoring properties keyed by symbols), including inherited enumerable properties."[^1] Example: [`basics`](#basics).

**r2.** `Object.entries` "returns an array of a given object's own enumerable string-keyed property key-value pairs." "This is the same as iterating with a for...in loop, except that a for...in loop enumerates properties in the prototype chain as well."[^2] `Object.values` "returns an array of a given object's own enumerable string-keyed property values."[^3] `Reflect.ownKeys` "returns an array of the target object's own property keys", and "It is the only way to get all own properties" with string and symbol keys "in one call".[^4] Example: [`basics`](#basics).

**r3.** The order of own keys is fixed by ECMA-262. OrdinaryOwnPropertyKeys lists the keys that are array indices "in ascending numeric index order", then the other String keys "in ascending chronological order of property creation", then the Symbol keys "in ascending chronological order of property creation".[^5] MDN: for `for...in`, "Within each component of the prototype chain, all non-negative integer keys (those that can be array indices) will be traversed first in ascending order by value, then other string keys in ascending chronological order of property creation."[^1] Example: [`order`](#order).

**r4.** `for...in` walks up the prototype chain. MDN: "If multiple objects in the prototype chain have a property with the same name, only the first one will be considered, and it is only visited if it's enumerable."[^1] ECMA-262 has the same rule: "a property of a prototype is not processed if it has the same name as a property that has already been processed".[^6] Example: [`shadow`](#shadow).

**r5.** Changing the object during the loop is risky. ECMA-262: "A property that is deleted before it is processed by the iterator's next method is ignored. If new properties are added to the target object during enumeration, the newly added properties are not guaranteed to be processed in the active enumeration."[^6] MDN: "In general, it is best not to add, modify, or remove properties from the object during iteration, other than the property currently being visited."[^1] Example: [`added`](#added).

**r6.** `Object.fromEntries` "transforms a list of key-value pairs into an object."[^7] With `Object.entries` it turns an object into pairs, so array methods can change the pairs before they go back into an object. Example: [`transform`](#transform).

## Examples

Integer keys first, then strings, then symbols (rule r3).

```javascript run id=order
const o = { b: 1, 10: "x", a: 2, 2: "y", [Symbol("s")]: 3, "-1": "z" };
console.log(Object.keys(o));
console.log(Reflect.ownKeys(o));
```

```output for=order
[ '2', '10', 'b', 'a', '-1' ]
[ '2', '10', 'b', 'a', '-1', Symbol(s) ]
```

`-1` is not an array index, so it keeps its place among the string keys.

A non-enumerable own property hides an inherited one of the same name (rule r4).

```javascript run id=shadow
const proto = { a: 1, b: 2 };
const o = Object.create(proto);
Object.defineProperty(o, "a", { value: 3 });
o.c = 4;
const keys = [];
for (const k in o) keys.push(k);
console.log(keys, o.a);
```

```output for=shadow
[ 'c', 'b' ] 3
```

Properties added during the loop (rule r5).

```javascript run id=added
const o = { a: 1, b: 2 };
const visited = [];
for (const k in o) {
  visited.push(k);
  o[k + k] = 0;
}
console.log(visited, Object.keys(o));
```

```output for=added
[ 'a', 'b' ] [ 'a', 'b', 'aa', 'bb' ]
```

Changing every value with entries and fromEntries (rule r6).

```javascript run id=transform
const prices = { tea: 2, cake: 5 };
const doubled = Object.fromEntries(Object.entries(prices).map(([k, v]) => [k, v * 2]));
console.log(doubled);
```

```output for=transform
{ tea: 4, cake: 10 }
```

## Versions

| Feature | Edition |
|---|---|
| order of own property keys | ECMAScript 2015[^8] |
| `Object.values`, `Object.entries` | ECMAScript 2017[^9] |
| `Object.fromEntries` | ECMAScript 2019[^9] |
| order of inherited keys in `for...in` | ECMAScript 2020[^8] |

MDN: "The order was first defined for own properties only in ECMAScript 2015; ECMAScript 2020 defines order for inherited properties as well."[^8] The TC39 list of finished proposals gives 2017 for `Object.values`/`Object.entries`, 2019 for `Object.fromEntries` and 2020 for `for-in` mechanics.[^9]

## Pitfalls

### Using `for...in` on an array

```javascript wrong id=array
const arr = [10, 20];
arr.extra = "x";
const seen = [];
for (const i in arr) seen.push(i);
console.log(seen, typeof seen[0]);
for (const v of arr) console.log(v);
```

```output for=array
[ '0', '1', 'extra' ] string
10
20
```

"Array indexes are just enumerable properties with integer names", and "the for...in loop will return all enumerable properties, including those with non–integer names and those that are inherited."[^1] MDN suggests a numeric `for` loop, `forEach` or `for...of`, "because they will return the index as a number instead of a string, and also avoid non-index properties."[^1]

### Expecting insertion order for integer keys

```javascript wrong id=intkeys
const byId = {};
byId[30] = "c";
byId[10] = "a";
byId[20] = "b";
console.log(Object.values(byId));
const m = new Map([
  [30, "c"],
  [10, "a"],
  [20, "b"],
]);
console.log([...m.values()]);
```

```output for=intkeys
[ 'a', 'b', 'c' ]
[ 'c', 'a', 'b' ]
```

Keys that are array indices come first in ascending order, whatever order they were added in (rule r3). "A Map object iterates entries, keys, and values in the order of entry insertion."[^8] See [[javascript/maps]].

## Comparison

In Python, an object's own attributes are in `__dict__`, "A dictionary or other mapping object used to store an object’s (writable) attributes."[^10] Iterating over it gives only what the instance holds, much like `Object.keys`, while `for...in` also visits the enumerable properties of the prototypes (rule r1).

## References
