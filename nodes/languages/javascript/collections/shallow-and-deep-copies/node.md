---
tag: 20VNVZ
kind: feature
owner: javascript
slug: shallow-and-deep-copies
name: Shallow and deep copies
aliases: [shallow copy, deep copy, structuredClone, copy an object in JavaScript, clone an object, JSON.parse(JSON.stringify()), Object.assign copy]
area: languages/javascript/collections
level: core
summary: >-
  Spread, Object.assign, slice, Array.from and concat make shallow copies that share nested objects. structuredClone, from the host, makes a deep copy but drops prototypes and rejects functions.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/fundamental-objects.html#sec-object.assign", locator: "20.1.2.1 Object.assign ( target, ...sources )"}
see_also: [javascript/copying-array-methods, copy-an-object, in-place-and-copying-operations, python/copying-collections]
sources:
- url: https://developer.mozilla.org/en-US/docs/Glossary/Shallow_copy
  title: MDN Web Docs, Glossary, Shallow copy
  locator: the definition; the list of built-in copy operations
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/fundamental-objects.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 20
  locator: 20.1.2.1 Object.assign ( target, ...sources )
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Glossary/Deep_copy
  title: MDN Web Docs, Glossary, Deep copy
  locator: the definition; JSON.stringify(); structuredClone()
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/API/Window/structuredClone
  title: MDN Web Docs, Web APIs, Window structuredClone() method
  locator: the introduction; Exceptions; Description
  accessed: 2026-11-18
- url: https://nodejs.org/docs/latest-v24.x/api/globals.html
  title: Node.js v24 documentation, Global objects
  locator: structuredClone(value[, options])
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/API/Web_Workers_API/Structured_clone_algorithm
  title: MDN Web Docs, Web APIs, The structured clone algorithm
  locator: Things that don't work with structured clone; Supported types
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/copy.html
  title: The Python Standard Library, Python 3.14, copy
  locator: the introduction
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Object.html
  title: Java SE 25 API, Class Object
  locator: clone()
  accessed: 2026-11-18
- url: https://doc.rust-lang.org/std/clone/trait.Clone.html
  title: The Rust Standard Library, Trait Clone
  locator: the description
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const original = { name: "tea", tags: ["hot"], made: new Date(0) };
const shallow = { ...original };
const deep = structuredClone(original);

original.name = "coffee";
original.tags.push("sweet");
console.log(shallow.name, shallow.tags);
console.log(deep.name, deep.tags, deep.made instanceof Date);
console.log(shallow.tags === original.tags, deep.tags === original.tags);
```

```output for=basics
tea [ 'hot', 'sweet' ]
tea [ 'hot' ] true
true false
```

| Copy | Depth | Notes |
|---|---|---|
| `{ ...obj }`, `Object.assign({}, obj)` | shallow | own enumerable properties |
| `[...arr]`, `arr.slice()`, `Array.from(arr)`, `arr.concat()` | shallow | the elements are shared |
| `JSON.parse(JSON.stringify(x))` | deep | only data that JSON can hold |
| `structuredClone(x)` | deep | a host function; handles cycles, `Map`, `Set`, `Date` |

## Semantics

**r1.** "A shallow copy of an object is a copy whose properties share the same references (point to the same underlying values) as those of the source object from which the copy was made."[^1] So "Re-assigning top-level properties of the copy does not affect the source object. Re-assigning nested object properties of the copy does affect the source object."[^1] Example: [`basics`](#basics).

**r2.** "In JavaScript, all standard built-in object-copy operations (spread syntax, Array.prototype.concat() , Array.prototype.slice() , Array.from() , and Object.assign() ) create shallow copies rather than deep copies."[^1] The specification says that `Object.assign` "copies the values of all of the enumerable own properties from one or more source objects to a target object."[^2] Example: [`arrays`](#arrays).

**r3.** "A deep copy of an object is a copy whose properties do not share the same references (point to the same underlying values) as those of the source object from which the copy was made."[^3] The language itself has no deep copy: "JavaScript has no built-in mechanism that performs a true deep copy".[^3] One way, "if it can be serialized, is to use JSON.stringify() to convert the object to a JSON string, and then JSON.parse() to convert the string back into a (completely new) JavaScript object".[^3] But "many JavaScript objects are not serializable at all".[^3] Example: [`json`](#json).

**r4.** `structuredClone` "creates a deep clone of a value using the structured clone algorithm", and "It also supports circular references".[^4] It comes from the host: "structuredClone() isn't a feature of the JavaScript language itself".[^3] Node has it as a global, documented as "The WHATWG structuredClone method."[^5] Example: [`circular`](#circular).

**r5.** The structured clone algorithm supports `Array`, `ArrayBuffer`, `Date`, `Map`, `Set`, `RegExp`, typed arrays, error types and "plain objects (e.g., from object literals)".[^6] Some things are lost or rejected: "Function objects cannot be duplicated by the structured clone algorithm; attempting to throws a DataCloneError exception." "The prototype chain is not walked or duplicated." "Class private elements are not duplicated."[^6] And "Property descriptors, setters, getters, and similar metadata-like features are not duplicated."[^6] Example: [`class`](#class).

**r6.** There is no generic copy either: "JavaScript has no built-in mechanism that performs a generic shallow copy (the existing ones only assume specific object types like plain objects or arrays)".[^1] A copy is only as deep as the code that makes it, so code that needs one level more can copy that level by hand, for example `rows.map((row) => [...row])`.

## Examples

Five shallow copies of a nested array (rule r2).

```javascript run id=arrays
const grid = [[1, 2], [3, 4]];
const copies = [[...grid], grid.slice(), Array.from(grid), grid.concat(), Object.assign([], grid)];
grid[0][0] = 99;
grid[1] = ["new"];
console.log(copies.map((c) => `${c[0][0]}/${c[1][0]}`).join(" "));

const rowsCopied = copies[0].map((row) => [...row]);
grid[0][1] = 0;
console.log(rowsCopied[0], copies[0][0]);
```

```output for=arrays
99/3 99/3 99/3 99/3 99/3
[ 99, 2 ] [ 99, 0 ]
```

Each copy sees the change inside the shared first row, and none sees the new second row.

What a JSON round trip keeps and what `structuredClone` keeps (rules r3 and r5).

```javascript run id=json
const data = { when: new Date(0), skip: undefined, ids: new Map([[1, "a"]]), n: NaN };
console.log(JSON.parse(JSON.stringify(data)));

const clone = structuredClone(data);
console.log(clone.when instanceof Date, "skip" in clone, clone.ids.get(1), clone.n);
```

```output for=json
{ when: '1970-01-01T00:00:00.000Z', ids: {}, n: null }
true true a NaN
```

A structure with a cycle (rule r4).

```javascript run id=circular
const root = { name: "root", children: [] };
root.children.push({ name: "leaf", parent: root });

const copy = structuredClone(root);
console.log(copy !== root, copy.children[0].parent === copy);
try {
  JSON.stringify(root);
} catch (e) {
  console.log(e.name);
}
```

```output for=circular
true true
TypeError
```

## Pitfalls

### Changing a nested object through a shallow copy

The copy and the source share `sizes` (rule r1), so the change shows up in both.

```javascript wrong id=nested
const defaults = { theme: "light", sizes: { font: 12 } };
const mine = { ...defaults };
mine.theme = "dark";
mine.sizes.font = 16;
console.log(defaults.theme, defaults.sizes.font);

const safe = structuredClone(defaults);
safe.sizes.font = 20;
console.log(defaults.sizes.font, safe.sizes.font);
```

```output for=nested
light 16
16 20
```

### Cloning class instances and functions

`structuredClone` does not keep the prototype, so methods are gone, and it throws on functions (rule r5).

```javascript wrong id=class
class Point {
  constructor(x) {
    this.x = x;
  }
  size() {
    return Math.abs(this.x);
  }
}
const c = structuredClone(new Point(-3));
console.log(c instanceof Point, c.x, typeof c.size);
try {
  structuredClone({ onClick() {} });
} catch (e) {
  console.log(e.name);
}
```

```output for=class
false -3 undefined
DataCloneError
```

For class instances, write a copy method or constructor that builds a new instance from the fields.

## Comparison

Python's copy module "provides generic shallow and deep copy operations": "A shallow copy constructs a new compound object and then (to the extent possible) inserts references into it to the objects found in the original. A deep copy constructs a new compound object and then, recursively, inserts copies into it of the objects found in the original."[^7] JavaScript has no generic copy in the language (rules r3 and r6). Java's `Object.clone` fills the new object's fields "with exactly the contents of the corresponding fields of this object, as if by assignment; the contents of the fields are not themselves cloned."[^8] Rust's `Clone` is "A common trait that allows explicit creation of a duplicate value."[^9] See [[copy-an-object]] and [[python/copying-collections]].

## References
