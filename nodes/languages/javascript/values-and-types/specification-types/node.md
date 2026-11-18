---
tag: RAGSWB
kind: feature
owner: javascript
slug: specification-types
name: Specification types
aliases: [ECMAScript specification types, Reference Record, Completion Record, Property Descriptor, List, Record, abrupt completion, normal completion, GetValue, PutValue]
area: languages/javascript/values-and-types
level: research
summary: >-
  Clause 6.2 of ECMA-262 defines meta-values that only the algorithms use: Lists, Records, Completion Records, Reference Records and Property Descriptors. Code sees their effects, not the values.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-specification-types", locator: "6.2 ECMAScript Specification Types"}
see_also: [javascript/the-ecmascript-language-types, javascript/property-descriptors, javascript/the-typeof-operator, javascript/strict-mode, javascript/javascript-data-types-and-data-structures, python/objects-values-and-types]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-specification-types
    title: ECMAScript 2026 Language Specification
    locator: 6.2 ECMAScript Specification Types; 6.2.2 The List and Record Specification Types; 6.2.4 The Completion Record Specification Type; 6.2.5 The Reference Record Specification Type; 6.2.5.5 GetValue; 6.2.5.6 PutValue; 6.2.6 The Property Descriptor Specification Type; 6.2.6.5 ToPropertyDescriptor
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/try...catch
    title: try...catch, MDN
    locator: Returning from a finally block
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/datamodel.html
    title: Data model, The Python Language Reference
    locator: 3.1 Objects, values and types
    accessed: 2026-11-18
---

## Synopsis

| Type | Clause | What it models | Effect that code can see |
|---|---|---|---|
| List | 6.2.2 | ordered values, such as argument lists | none directly |
| Record | 6.2.2 | named fields, such as `[[Value]]` | none directly |
| Completion Record | 6.2.4 | normal ends and `break`, `continue`, `return`, `throw` | the result of `try`, `finally` and `eval` |
| Reference Record | 6.2.5 | a resolved or unresolved name or property | `typeof`, `delete`, assignment |
| Property Descriptor | 6.2.6 | the attributes of a property | `Object.defineProperty`, `Object.getOwnPropertyDescriptor` |

"A specification type corresponds to meta-values that are used within algorithms to describe the semantics of ECMAScript language constructs and ECMAScript language types."[^1] The list in 6.2 also has Environment Record, Abstract Closure and Data Block.[^1]

```javascript run id=synopsis
console.log(typeof missingName);
try {
  missingName;
} catch (e) {
  console.log(e.name);
}
console.log(eval("1; if (true) { 2; }"));
```

```output for=synopsis
undefined
ReferenceError
2
```

## Semantics

**r1. Not language values.** "Specification type values are specification artefacts that do not necessarily correspond to any specific entity within an ECMAScript implementation." They "cannot be stored as properties of objects or values of ECMAScript language variables."[^1]

**r2. Lists and Records.** "Values of the List type are simply ordered sequences of list elements containing the individual values."[^1] "The Record type is used to describe data aggregations within the algorithms of this specification." Field names are in double brackets, as in `[[Value]]`.[^1]

**r3. Completion Records.** "The Completion Record specification type is used to explain the runtime propagation of values and control flow". Its `[[Type]]` field is normal, break, continue, return or throw, and an abrupt completion is any type other than normal.[^1] Example: [`finally`](#finally).

**r4. Reference Records.** "The Reference Record type is used to explain the behaviour of such operators as delete, typeof, the assignment operators, the super keyword and other language features."[^1] Its fields are `[[Base]]`, `[[ReferencedName]]`, `[[Strict]]` and `[[ThisValue]]`. A base of unresolvable "indicates that the binding could not be resolved."[^1]

**r5. GetValue and PutValue.** GetValue throws a ReferenceError for an unresolvable reference.[^1] PutValue throws one too when `[[Strict]]` is true. Else it sets a property on the global object.[^1] Example: [`put-value`](#put-value).

**r6. Property Descriptors.** "The Property Descriptor type is used to explain the manipulation and reification of Object property attributes." It is a Record whose fields are attribute names.[^1] ToPropertyDescriptor throws a TypeError if its argument is not an Object.[^1] Example: [`descriptor`](#descriptor).

## Examples

The examples run with Node.js 24.

An assignment to an unresolvable name (rule r5):

```javascript run id=put-value
function sloppy() {
  leaked = 1;
}
sloppy();
console.log(globalThis.leaked);
function strict() {
  "use strict";
  try {
    leaked2 = 1;
  } catch (e) {
    console.log(e.name);
  }
}
strict();
```

```output for=put-value
1
ReferenceError
```

A return completion from `finally` (rule r3). MDN says "the return statement in the finally block overrides the effect of the previous return statement, including its return value."[^2]

```javascript run id=finally
function pick() {
  try {
    return "try";
  } finally {
    return "finally";
  }
}
console.log(pick());
```

```output for=finally
finally
```

A descriptor must be an object (rule r6):

```javascript fails id=descriptor expect=exception:TypeError
Object.defineProperty({}, "x", 1);
```

## Pitfalls

### These names are not APIs

No code can make a Completion Record or a Reference Record (rule r1). Only their effects show, as in [`synopsis`](#synopsis).

### A descriptor object is not a Property Descriptor

`Object.getOwnPropertyDescriptor` returns an ordinary object. In the specification, FromPropertyDescriptor (6.2.6.4) makes such an object from a Property Descriptor, and ToPropertyDescriptor (6.2.6.5) reads one back.[^1]

## Comparison

| Topic | ECMAScript | Python |
|---|---|---|
| how the model is told | language types plus specification types[^1] | objects: "All data in a Python program is represented by objects"[^3] |
| engine details | specification types need not exist in an engine[^1] | notes such as "For CPython, id(x) is the memory address where x is stored."[^3] |

The Python Language Reference describes its model with objects, and marks some engine details as CPython notes.[^3] ECMA-262 describes its algorithms with meta-values that code cannot hold (rule r1). See [[javascript/the-ecmascript-language-types]] and [[python/objects-values-and-types]].

## References
