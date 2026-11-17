---
tag: M7XEF9
kind: feature
owner: javascript
slug: object-initializers
name: Object initializers
aliases: [object literals, object literal syntax, shorthand property names, computed property names, object spread, spread properties, __proto__ in object literals]
area: languages/javascript/user-types
level: intro
summary: >-
  An object initializer, or object literal, is an expression in curly braces that makes a new object each time it runs. It can hold plain, shorthand, computed and spread properties, and methods.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-object-initializer", locator: "13.2.5 Object Initializer"}
see_also: [javascript/objects, javascript/shallow-and-deep-copies, copy-an-object, record-type]
sources:
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 13
  locator: 13.2.5 Object Initializer; 13.2.5.5 Runtime Semantics, Evaluation
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Working_with_objects
  title: MDN Web Docs, JavaScript Guide, Working with objects
  locator: Using object initializers
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Object_initializer
  title: MDN Web Docs, JavaScript Reference, Object initializer
  locator: the introduction; Property definitions; Duplicate property names; Computed property names; Spread properties; Prototype setter
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Spread_syntax
  title: MDN Web Docs, JavaScript Reference, Spread syntax (...)
  locator: Spread in object literals; Overriding properties
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Rest/Spread Properties, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/expressions.html
  title: The Python Language Reference, Expressions, Python 3.14
  locator: 6.2.8 Dictionary displays
  accessed: 2026-11-18
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification, language version go1.27
  locator: Composite literals
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const name = "Ada";
const key = "born";
const person = {
  name,
  [key]: 1815,
  "full name": "Ada Lovelace",
  1: "one",
  greet() {
    return `Hi, ${this.name}`;
  },
};
console.log(Object.keys(person));
console.log(person.born, person.greet());
```

```output for=basics
[ '1', 'name', 'born', 'full name', 'greet' ]
1815 Hi, Ada
```

| Property definition | Meaning |
|---|---|
| `key: value` | a property with a fixed name |
| `"any text": value`, `1: value` | a name from a string or number literal |
| `key` | shorthand for `key: key` |
| `[expression]: value` | a computed name |
| `name() {}` | a method |
| `get name() {}`, `set name(v) {}` | an accessor property |
| `...source` | copies of the own enumerable properties of `source` |
| `__proto__: proto` | sets the prototype of the new object |

## Semantics

**r1.** ECMA-262: "An object initializer is an expression describing the initialization of an Object, written in a form resembling a literal. It is a list of zero or more pairs of property keys and associated values, enclosed in curly brackets. The values need not be literals; they are evaluated each time the object initializer is evaluated."[^1] So "each object initializer results in a new object being created whenever the statement in which it appears is executed. Identical object initializers create distinct objects that do not compare to each other as equal."[^2] The evaluation steps begin "Let obj be OrdinaryObjectCreate(%Object.prototype%)."[^1] Example: [`fresh`](#fresh).

**r2.** A name can be an identifier, a string literal or a number literal, or a computed name in square brackets.[^1] A computed name lets you "put an expression in square brackets", and the result is "computed and used as the property name."[^3] A shorthand property `{ a }` takes its name and value from the variable `a`.[^3] Example: [`basics`](#basics).

**r3.** "When using the same name for your properties, the second property will overwrite the first."[^3] Example: [`dup`](#dup).

**r4.** "Object literals support the spread syntax. It copies own enumerable properties from a provided object onto a new object."[^3] When two sources have a property with the same name, "the property takes the last value assigned while remaining in the position it was originally set."[^4] "A single spread creates a shallow copy of the original object (but without non-enumerable properties and without copying the prototype)".[^4] Example: [`spread`](#spread).

**r5.** The key `__proto__` written with a colon does not make a property. If the value is an object or `null`, "it points the [[Prototype]] of the created object to that value."[^3] "Only a single prototype setter is permitted in an object literal."[^3] Example: [`proto`](#proto).

**r6.** At the start of a statement, a `{` begins a block, not an object. MDN: "you may need to wrap the object literal in parentheses if the object appears where a statement is expected, so as not to have the literal be confused with a block statement."[^2] Example: [`arrow`](#arrow).

## Examples

Each run makes a new object, and the property values are computed in order (rule r1).

```javascript run id=fresh
const make = (n) => ({ n, tags: [] });
const a = make(1);
const b = make(1);
a.tags.push("x");
console.log(a === b, b.tags.length);
let i = 0;
console.log({ [`k${++i}`]: i, [`k${++i}`]: i });
```

```output for=fresh
false 0
{ k1: 1, k2: 2 }
```

A duplicate name keeps the last value (rule r3).

```javascript run id=dup
const o = { x: 1, y: 2, x: 3 };
console.log(o, Object.keys(o));
```

```output for=dup
{ x: 3, y: 2 } [ 'x', 'y' ]
```

Merging and copying with spread (rule r4).

```javascript run id=spread
const defaults = { color: "red", size: 1 };
const custom = { size: 3, label: "big" };
console.log({ ...defaults, ...custom });
const copy = { ...defaults };
console.log(copy === defaults, copy.color);
```

```output for=spread
{ color: 'red', size: 3, label: 'big' }
false red
```

Setting the prototype in the literal (rule r5).

```javascript run id=proto
const base = {
  hello() {
    return "hi";
  },
};
const child = { __proto__: base, name: "c" };
console.log(child.hello(), Object.hasOwn(child, "__proto__"));
console.log(Object.getPrototypeOf(child) === base, Object.keys(child));
```

```output for=proto
hi false
true [ 'name' ]
```

## Versions

| Feature | Edition |
|---|---|
| duplicate property names allowed in strict mode | after ES2015[^3] |
| spread properties in object literals | ECMAScript 2018[^5] |

The TC39 list of finished proposals gives 2018 as the publication year of Rest/Spread Properties.[^5]

## Pitfalls

### Returning an object literal from an arrow function

```javascript wrong id=arrow
const bad = () => { x: 1 };
const good = () => ({ x: 1 });
console.log(bad(), good());
```

```output for=arrow
undefined { x: 1 }
```

In `bad`, the braces are the function body, and `x:` is a label. Put the literal in parentheses (rule r6).

### Expecting spread to copy nested objects

```javascript wrong id=shallow
const original = { name: "a", tags: ["x"] };
const copy = { ...original };
copy.name = "b";
copy.tags.push("y");
console.log(original.name, original.tags);
```

```output for=shallow
a [ 'x', 'y' ]
```

Spread copies the property values, and the value of `tags` is a reference to one array (rule r4). See [[javascript/shallow-and-deep-copies]].

## Comparison

Python: "A dictionary display is a possibly empty series of dict items (key/value pairs) enclosed in curly braces", for a repeated key "the final dictionary’s value for that key will be the last one given", and "A double asterisk ** denotes dictionary unpacking."[^6] Go: "Composite literals construct new values for structs, arrays, slices, and maps each time they are evaluated", and the literal names its type.[^7] A JavaScript object initializer names no type. It makes an object with `Object.prototype` as its prototype, unless `__proto__` gives another one (rule r5).

## References
