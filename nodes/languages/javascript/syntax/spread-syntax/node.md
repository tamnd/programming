---
tag: X86D73
kind: feature
owner: javascript
slug: spread-syntax
name: Spread syntax
aliases: ["...", spread operator, spread element, object spread, array spread]
area: languages/javascript/syntax
level: core
summary: >-
  ...x expands an iterable into arguments or array elements, and copies the own enumerable properties of a value into an object literal. The copy is shallow.
grammar: "SpreadElement : ... AssignmentExpression; ArgumentList : ... AssignmentExpression; PropertyDefinition : ... AssignmentExpression (ECMA-262, 13.2 and 13.3)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#prod-SpreadElement", locator: "13.2.4 Array Initializer; 13.2.5 Object Initializer; 13.3.8 Argument Lists"}
see_also: [javascript/trailing-commas, javascript/the-new-operator, javascript/iterables, javascript/shallow-and-deep-copies, javascript/arrays, python/unpacking-argument-lists, python/unpacking-in-lists-and-list-comprehensions]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Spread_syntax
    title: Spread syntax (...), MDN
    locator: introduction; Description; Examples
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-object-initializer-runtime-semantics-propertydefinitionevaluation
    title: ECMAScript 2026 Language Specification
    locator: 13.2.5.5 PropertyDefinitionEvaluation
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.2.5 Container displays; 6.2.5.4 Dictionary displays; 6.3.3 Calls
    accessed: 2026-11-18
---

## Synopsis

```javascript run id=synopsis
function sum(x, y, z) {
  return x + y + z;
}
const numbers = [1, 2, 3];
console.log(sum(...numbers));
console.log([0, ...numbers, 4]);
console.log({ ...{ a: 1 }, b: 2 });
```

```output for=synopsis
6
[ 0, 1, 2, 3, 4 ]
{ a: 1, b: 2 }
```

MDN: "The spread ( ... ) syntax allows an iterable, such as an array or string, to be expanded in places where zero or more arguments (for function calls) or elements (for array literals) are expected." In an object literal, it "enumerates the properties of an object and adds the key-value pairs to the object being created."[^1]

## Semantics

**r1. Three places.** MDN lists "Function arguments list", "Array literals" and "Object literals". "Although the syntax looks the same, they come with slightly different semantics."[^1]

**r2. Arguments and arrays need an iterable.** "Only iterable values, like Array and String , can be spread in array literals and argument lists." A plain object without a `Symbol.iterator` method gives a TypeError.[^1] A spread can be at any position and can occur more than once.[^1] Example: [`iterables`](#iterables).

**r3. Objects take own enumerable properties.** "spreading in object literals enumerates the own properties of the value." So an array spreads into an object with its indices as keys.[^1] The spec does this with CopyDataProperties.[^2] Example: [`objects`](#objects).

**r4. Primitives in objects.** "All primitives can be spread in objects. Only strings have enumerable own properties, and spreading anything else doesn't create properties on the new object."[^1] So `...false` adds nothing.

**r5. Last value wins, first position stays.** With the same name twice, "the property takes the last value assigned while remaining in the position it was originally set."[^1] Example: [`override`](#override).

**r6. Shallow copy.** "Each array element retains its identity without getting copied." The copy goes one level deep.[^1] Example: [`shallow`](#shallow).

**r7. No setters.** "Object.assign() triggers setters on the target object, whereas spread syntax does not."[^1]

**r8. Spread is not rest.** "Spread syntax looks exactly like rest syntax." Spread expands an array into its elements, and rest collects elements into one. MDN points to rest parameters and the rest property for the other form.[^1]

## Examples

The examples run with Node.js 24.

Iterables in calls and arrays (rule r2), with `new` and a string:

```javascript run id=iterables
function show(v, w, x, y, z) {
  return [v, w, x, y, z].join(" ");
}
const args = [0, 1];
console.log(show(-1, ...args, 2, ...[3]));
console.log(new Date(...[1970, 0, 1]).getFullYear());
console.log([..."hi"], [...new Set([1, 1, 2])]);
```

```output for=iterables
-1 0 1 2 3
1970
[ 'h', 'i' ] [ 1, 2 ]
```

A plain object is not iterable:

```javascript fails id=not-iterable expect=exception:TypeError
const obj = { key1: "value1" };
const array = [...obj];
```

Node.js reports `TypeError: obj is not iterable`.

Spread in object literals (rules r3 and r4):

```javascript run id=objects
const array = [1, 2, 3];
console.log({ ...array });
console.log({ ...true, ..."test", ...10 });
const isSummer = false;
console.log({ apple: 10, ...(isSummer && { watermelon: 30 }) });
```

```output for=objects
{ '0': 1, '1': 2, '2': 3 }
{ '0': 't', '1': 'e', '2': 's', '3': 't' }
{ apple: 10 }
```

When `isSummer` is false, `isSummer && {...}` is `false`, and spreading `false` adds no property.[^1]

Order of keys when names repeat (rule r5). This is the MDN example:[^1]

```javascript run id=override
const obj1 = { foo: "bar", x: 42 };
const obj2 = { foo: "baz", y: 13 };
const mergedObj = { x: 41, ...obj1, ...obj2, y: 9 };
console.log(mergedObj);
```

```output for=override
{ x: 42, foo: 'baz', y: 9 }
```

A copy is one level deep (rule r6):

```javascript run id=shallow
const a = [[1], [2], [3]];
const b = [...a];
b.shift().shift();
console.log(a, b.length, a[1] === b[0]);
```

```output for=shallow
[ [], [ 2 ], [ 3 ] ] 2 true
```

## Pitfalls

### Spread of the arguments array

MDN shows a `merge(...objects)` function that returns `({ ...objects })`. It does not merge. It "spreads an array of arguments into the object literal", so the keys are `0`, `1` and so on.[^1]

```javascript run id=merge
const naive = (...objects) => ({ ...objects });
const merge = (...objects) => objects.reduce((acc, cur) => ({ ...acc, ...cur }));
console.log(naive({ a: 1 }, { b: 2 }));
console.log(merge({ a: 1 }, { b: 2 }));
```

```output for=merge
{ '0': { a: 1 }, '1': { b: 2 } }
{ a: 1, b: 2 }
```

### Many arguments

"When using spread syntax for function calls, be aware of the possibility of exceeding the JavaScript engine's argument length limit."[^1] MDN gives the details on the page for `Function.prototype.apply()`.[^1]

### `undefined` from a condition

`[...(cond ? ['x'] : [])]` adds nothing when `cond` is false. `[cond ? 'x' : undefined]` adds an `undefined` element, and "this element will be visited by methods such as Array.prototype.map() ."[^1]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| iterable into arguments | `f(...xs)`[^1] | `f(*xs)`; "expression must evaluate to an iterable"[^3] |
| mapping into arguments | no such form | `f(**m)`; the mapping gives keyword arguments[^3] |
| merge into a literal | `{ ...a, ...b }`[^1] | `{**a, **b}`, "dictionary unpacking"[^3] |
| iterable into a list | `[...xs]`[^1] | `[*xs]`[^3] |

Python uses `*` for iterables and `**` for mappings. In a call, the `**expression` mapping gives "additional keyword arguments", and "each key in this mapping must be a string."[^3] JavaScript has one `...` form, and its meaning comes from the place where it occurs (rule r1). See [[python/unpacking-argument-lists]].

## References
