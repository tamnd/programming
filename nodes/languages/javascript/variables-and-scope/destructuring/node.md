---
tag: NEW-javascript-destructuring
kind: feature
owner: javascript
slug: destructuring
name: Destructuring
aliases: [destructuring assignment, destructuring binding, binding pattern, assignment pattern, array destructuring, object destructuring, rest property]
area: languages/javascript/variables-and-scope
level: core
summary: >-
  Destructuring unpacks array items or object properties into separate variables. A binding pattern follows var, let or const; an assignment pattern sets targets that already exist.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-destructuring-assignment", locator: "13.15.5 Destructuring Assignment; 14.3.3 Destructuring Binding Patterns"}
see_also: [javascript/declarations, javascript/let-and-const, javascript/var-declarations, destructuring-assignment, assignment, python/iterable-unpacking, python/assignment-statements]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Destructuring
    title: Destructuring, MDN
    locator: Description; Default value; Rest properties and rest elements; Destructuring primitive values
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-destructuring-assignment
    title: ECMAScript 2026 Language Specification
    locator: 13.15.5.2 Runtime Semantics, DestructuringAssignmentEvaluation
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/simple_stmts.html
    title: Simple statements, The Python Language Reference
    locator: 7.2 Assignment statements
    accessed: 2026-11-18
---

## Synopsis

```text
const [a, b = default, ...rest] = iterable;
const { key, key2: name, key3 = default, ...others } = object;
[a, b] = [b, a];
({ key } = object);
```

"The destructuring syntax is a JavaScript syntax that makes it possible to unpack values from arrays, or properties from objects, into distinct variables."[^1]

```javascript run id=synopsis
const [first, , third = 3, ...more] = [1, 2, undefined, 4, 5];
const { a, b: renamed, ...others } = { a: 1, b: 2, c: 3 };
console.log(first, third, more, a, renamed, others);
```

```output for=synopsis
1 3 [ 4, 5 ] 1 2 { c: 3 }
```

## Semantics

**r1. Two kinds of pattern.** "For both object and array destructuring, there are two kinds of destructuring patterns: binding pattern and assignment pattern". "In binding patterns, the pattern starts with a declaration keyword (var, let, or const)." "In assignment patterns, the pattern does not start with a keyword."[^1]

**r2. Where a binding pattern can go.** MDN lists "The looping variable of for...in for...of, and for await...of loops; Function parameters; The catch binding variable."[^1] Example: [`params`](#params).

**r3. Defaults.** "The default value is used when the property is not present, or has value undefined. It is not used if the property has value null." "It will only be evaluated when necessary."[^1] Example: [`defaults`](#defaults).

**r4. Rest.** "For array destructuring, it collects remaining elements of the iterable into a new array". "For object destructuring, it copies all enumerable own properties of the object that are not already picked off by the destructuring pattern into a new object".[^1]

**r5. Arrays use the iterator.** For an array pattern, the specification starts with "Let iteratorRecord be ? GetIterator ( value, sync )."[^2] So any iterable works, for example a string or a Set. Example: [`iter`](#iter).

**r6. Objects use property access.** "Object destructuring is almost equivalent to property accessing." An object pattern first performs "RequireObjectCoercible ( value )", so "destructuring null or undefined throws a TypeError."[^2][^1] Example: [`null`](#null).

## Examples

The examples run with Node.js 24.

Patterns in parameters and in a `for...of` loop (rule r2):

```javascript run id=params
function area({ width, height = 1 }) {
  return width * height;
}
for (const [key, value] of Object.entries({ x: 1, y: 2 })) {
  console.log(key, value);
}
console.log(area({ width: 4 }), area({ width: 4, height: 2 }));
```

```output for=params
x 1
y 2
4 8
```

Defaults with `undefined` and `null`, and a swap (rule r3):

```javascript run id=defaults
const [p = 1] = [];
const { q = 2 } = { q: undefined };
const { r = 2 } = { r: null };
let m = 1, n = 3;
[m, n] = [n, m];
console.log(p, q, r, m, n);
```

```output for=defaults
1 2 null 3 1
```

An array pattern on a string and on a Set (rule r5). A missing item is `undefined`:

```javascript run id=iter
const [c1, c2, c3] = "hi";
const [s1, s2] = new Set([7, 8]);
console.log(c1, c2, c3, s1 + s2);
```

```output for=iter
h i undefined 15
```

An object pattern on `null` (rule r6):

```javascript fails id=null expect=exception:TypeError
const {} = null;
```

## Pitfalls

### An object pattern at the start of a statement

"The parentheses ( ... ) around the assignment statement are required when using object literal destructuring without a declaration." Without them, "the { a, b } on the left-hand side is considered a block and not an object literal".[^1]

### One keyword for all names

"All variables share the same declaration, so if you want some variables to be re-assignable but others to be read-only, you may have to destructure twice".[^1]

### A default does not replace null

Only `undefined` and a missing property use the default (rule r3). Example: [`defaults`](#defaults) gives `r` the value `null`.

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| sequence | `[a, b] = list`[^1] | `a, b = seq`[^3] |
| count must match | no; a missing item is `undefined` (example [`iter`](#iter)) | yes[^3] |
| rest | `...rest`[^1] | starred target `*rest`[^3] |
| mapping or object | `{ key } = obj`[^1] | no assignment form |

In Python, the object "must be an iterable with the same number of items as there are targets in the target list".[^3] JavaScript does not check the count. MDN says this "capability is similar to features present in languages such as Perl and Python."[^1] See [[python/iterable-unpacking]] and [[destructuring-assignment]].

## References
