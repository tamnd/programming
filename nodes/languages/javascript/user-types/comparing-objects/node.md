---
tag: P69WDP
kind: feature
owner: javascript
slug: comparing-objects
name: Comparing objects
aliases: [object equality, object identity, deep equality, shallow equality, isDeepStrictEqual, Object.is with objects, comparing objects by value]
area: languages/javascript/user-types
level: intro
summary: >-
  In JavaScript, ===, == and Object.is compare objects by identity, so two objects with the same properties are not equal. A deep comparison needs a function, such as Node's util.isDeepStrictEqual.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-isstrictlyequal", locator: "7.2.15 IsStrictlyEqual; 7.2.10 SameValue"}
see_also: [javascript/objects, value-equality-and-identity, javascript/shallow-and-deep-copies, javascript/traversing-object-properties]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Strict_equality
  title: MDN Web Docs, JavaScript Reference, Strict equality (===)
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/is
  title: MDN Web Docs, JavaScript Reference, Object.is()
  locator: Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Equality_comparisons_and_sameness
  title: MDN Web Docs, JavaScript Guide, Equality comparisons and sameness
  locator: the introduction
  accessed: 2026-11-18
- url: https://nodejs.org/docs/latest-v24.x/api/util.html
  title: Node.js v24 documentation, Util
  locator: util.isDeepStrictEqual(val1, val2[, options])
  accessed: 2026-11-18
- url: https://nodejs.org/docs/latest-v24.x/api/assert.html
  title: Node.js v24 documentation, Assert
  locator: assert.deepStrictEqual(actual, expected[, message]), Comparison details
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/JSON/stringify
  title: MDN Web Docs, JavaScript Reference, JSON.stringify()
  locator: Description
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/expressions.html
  title: The Python Language Reference, Expressions, Python 3.14
  locator: 6.10.1 Value comparisons
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/datamodel.html
  title: The Python Language Reference, Data model, Python 3.14
  locator: 3.3.1 Basic customization
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const a = { x: 1 };
const b = { x: 1 };
const c = a;
console.log(a === b, a == b, Object.is(a, b));
console.log(a === c, Object.is(a, c));
c.x = 2;
console.log(a.x);
```

```output for=basics
false false false
true true
2
```

| Comparison | Two distinct objects with the same properties | The same object twice |
|---|---|---|
| `a === b`, `a == b`, `Object.is(a, b)` | false | true |
| `util.isDeepStrictEqual(a, b)` in Node.js | true | true |
| a hand-written shallow check | true if each value is the same | true |

## Semantics

**r1.** For `===`: "If both operands are objects, return true only if they refer to the same object."[^1] `Object.is` treats two objects as the same when they are "both the same object (meaning both values reference the same object in memory)".[^2] MDN: "For any non-primitive objects x and y which have the same structure but are distinct objects themselves, all of the above forms will evaluate to false ."[^3] Example: [`basics`](#basics).

**r2.** The three operators differ only on primitives. "Note that the distinctions between these all have to do with their handling of primitives; none of them compares whether the parameters are conceptually similar in structure."[^3]

**r3.** "Comparing the contents of distinct objects or arrays recursively is called deep equality. JavaScript does not provide a general deep comparison operator; libraries and host APIs can provide comparison utilities with different rules."[^3]

**r4.** Node.js has `util.isDeepStrictEqual`, which "Returns true if there is deep strict equality between val1 and val2 . Otherwise, returns false ." "By default, deep strict equality includes comparison of object prototypes and constructors."[^4] The rules are those of `assert.deepStrictEqual`: "Primitive values are compared using Object.is() ." "Object properties are compared unordered." "Recursion stops when both sides differ or either side encounters a circular reference."[^5] Example: [`deep`](#deep).

**r5.** A shallow check compares the own keys and then each value with `===` or `Object.is`. Nested objects are then compared by identity (rule r1). Example: [`shallow`](#shallow).

## Examples

Deep comparison in Node.js (rule r4).

```javascript run id=deep
import("node:util").then(({ isDeepStrictEqual }) => {
  console.log(isDeepStrictEqual({ a: [1, 2] }, { a: [1, 2] }));
  console.log(isDeepStrictEqual({ a: 1 }, { a: "1" }));
  console.log(isDeepStrictEqual({ a: 1, b: 2 }, { b: 2, a: 1 }));
  class Point {
    constructor(x) {
      this.x = x;
    }
  }
  console.log(isDeepStrictEqual(new Point(1), { x: 1 }));
});
```

```output for=deep
true
false
true
false
```

The last pair has the same properties but different prototypes.

A shallow equality function (rule r5).

```javascript run id=shallow
function shallowEqual(a, b) {
  const keys = Object.keys(a);
  return (
    keys.length === Object.keys(b).length &&
    keys.every((k) => Object.hasOwn(b, k) && Object.is(a[k], b[k]))
  );
}
const tags = ["x"];
console.log(shallowEqual({ x: 1, tags }, { tags, x: 1 }));
console.log(shallowEqual({ x: 1, tags: ["x"] }, { x: 1, tags: ["x"] }));
console.log(shallowEqual({ n: NaN }, { n: NaN }));
```

```output for=shallow
true
false
true
```

## Versions

| Feature | Version |
|---|---|
| `util.isDeepStrictEqual` | Node.js v9.0.0[^4] |
| the third parameter of `util.isDeepStrictEqual` | Node.js v24.9.0[^4] |

The Node.js history table for `util.isDeepStrictEqual` gives "Added in: v9.0.0", and for v24.9.0 "Added options parameter to allow skipping prototype comparison."[^4]

## Pitfalls

### Comparing JSON strings

```javascript wrong id=json
const a = { x: 1, y: 2 };
const b = { y: 2, x: 1 };
console.log(JSON.stringify(a) === JSON.stringify(b));
console.log(JSON.stringify({ u: undefined }) === JSON.stringify({}));
```

```output for=json
false
true
```

"Properties are visited using the same algorithm as Object.keys() ", so the string follows the order in which the keys were added. And undefined values "are either omitted (when found in an object) or changed to null (when found in an array)."[^6] Two different objects can give the same string, and two equal ones can give different strings. Use a deep comparison (rule r4).

### Searching an array for an object literal

```javascript wrong id=includes
const users = [{ id: 1 }, { id: 2 }];
console.log(users.includes({ id: 1 }), users.indexOf({ id: 1 }));
console.log(users.some((u) => u.id === 1), users.findIndex((u) => u.id === 2));
```

```output for=includes
false -1
true 1
```

`{ id: 1 }` makes a new object, which is not in the array (rule r1). Search with a function that compares the fields you care about.

## Comparison

Python: "The default behavior for equality comparison (== and != ) is based on the identity of the objects." "Hence, equality comparison of instances with the same identity results in equality, and equality comparison of instances with different identities results in inequality."[^7] A class can change this by defining `__eq__`: in the data model, `x==y` calls `x.__eq__(y)`, and the default `object.__eq__` uses `is`.[^8] JavaScript has no such hook. `===` and `==` always compare objects by identity (rule r1), so a value comparison is a separate function.

## References
