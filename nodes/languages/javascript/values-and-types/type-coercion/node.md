---
tag: DZA66Y
kind: feature
owner: javascript
slug: type-coercion
name: Type coercion
aliases: [coercion, implicit conversion, ToPrimitive, OrdinaryToPrimitive, Symbol.toPrimitive, valueOf, toString, primitive coercion, numeric coercion]
area: languages/javascript/values-and-types
level: core
summary: >-
  JavaScript converts values to the type that an operation needs. An object becomes a primitive through Symbol.toPrimitive, then valueOf() and toString(), in an order that the hint sets.
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-toprimitive", locator: "7.1.1 ToPrimitive"}
see_also: [javascript/dynamic-and-weak-typing, javascript/explicit-type-conversion, javascript/loose-equality, javascript/primitive-values, javascript/symbols, javascript/booleans-and-truthiness, type-conversion, python/arithmetic-conversions]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Type_coercion
    title: Type coercion, MDN Glossary
    locator: definition; Examples
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Data_structures
    title: JavaScript data types and data structures, MDN
    locator: Type coercion; Primitive coercion; Numeric coercion; Other coercions
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-toprimitive
    title: ECMAScript 2026 Language Specification
    locator: 7.1.1 ToPrimitive; 7.1.1.1 OrdinaryToPrimitive
    accessed: 2026-11-18
  - url: https://docs.python.org/3/library/stdtypes.html
    title: Built-in Types, The Python Standard Library
    locator: Numeric Types (int, float, complex)
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/datamodel.html
    title: Data model, The Python Language Reference
    locator: 3.3.1 Basic customization; 3.3.8 Emulating numeric types
    accessed: 2026-11-18
---

## Synopsis

| Kind | Hint | Order of methods on an object | Used by |
|---|---|---|---|
| primitive | `'default'` | `[Symbol.toPrimitive]`, `valueOf()`, `toString()` | `+`, `==`, the `Date()` constructor |
| numeric, number, BigInt | `'number'` | `[Symbol.toPrimitive]`, `valueOf()`, `toString()` | arithmetic operators, unary `+` |
| string | `'string'` | `[Symbol.toPrimitive]`, `toString()`, `valueOf()` | template literals, as in [`hint`](#hint) |

"Type coercion is the automatic or implicit conversion of values from one data type to another (such as strings to numbers)."[^1] MDN lists the three paths for objects in this order of methods.[^2]

```javascript run id=synopsis
const obj = {
  valueOf() { return 42; },
  toString() { return "obj"; },
};
console.log(obj + 1, obj * 2, `${obj}`, String(obj));
console.log("5" + 9, "5" - 2, [] + [], [1, 2] + 3);
```

```output for=synopsis
43 84 obj obj
59 3  1,23
```

## Semantics

**r1. Primitives first.** ToPrimitive "converts its input argument to a non- Object type". If the input is not an object, it returns the input.[^3]

**r2. Symbol.toPrimitive wins.** When an object has a `Symbol.toPrimitive` method, ToPrimitive calls it with the hint `'default'`, `'string'` or `'number'`. If the result is an object, it throws a TypeError.[^3] "The [Symbol.toPrimitive]() method always takes precedence when doing conversion to any primitive type."[^2] Example: [`hint`](#hint).

**r3. valueOf and toString.** Without that method, OrdinaryToPrimitive tries `toString` then `valueOf` for the string hint, and `valueOf` then `toString` for the others. It skips a method that is not callable or that returns an object. When no method gives a primitive, it throws a TypeError.[^3] Example: [`no-primitive`](#no-primitive).

**r4. No hint acts like number.** "When ToPrimitive is called without a hint, then it generally behaves as if the hint were number."[^3] "Dates treat the absence of a hint as if the hint were string."[^3] Example: [`date`](#date).

**r5. The + operator.** For `+`, "if one operand is a string, string concatenation is performed; otherwise, numeric addition is performed."[^2] So `'5' + 9` gives `'59'`.[^1] Example: [`synopsis`](#synopsis).

**r6. Numeric coercion.** "Numeric coercion is used by all arithmetic operators, since they are overloaded for both numbers and BigInts. The only exception is unary plus, which always does number coercion."[^2]

**r7. Plain objects and arrays.** `{}` and `[]` inherit a `valueOf()` that returns the object itself, so `toString()` runs. `[].toString()` gives the empty string.[^2] Example: [`synopsis`](#synopsis).

## Examples

The examples run with Node.js 24.

An object that answers each hint (rule r2):

```javascript run id=hint
const temp = {
  [Symbol.toPrimitive](hint) {
    console.log("hint:", hint);
    return hint === "number" ? 21 : "21 C";
  },
};
console.log(+temp);
console.log(`${temp}`);
console.log(temp + "!");
```

```output for=hint
hint: number
21
hint: string
21 C
hint: default
21 C!
```

A Date uses the string hint when there is no hint (rule r4):

```javascript run id=date
const d = new Date(0);
console.log(typeof (d + 1), typeof (d - 1), d - 1);
```

```output for=date
string number -1
```

When no method gives a primitive, the conversion throws (rule r3):

```javascript fails id=no-primitive expect=exception:TypeError
const bad = {
  valueOf() { return {}; },
  toString() { return {}; },
};
console.log(bad + 1);
```

## Pitfalls

### + is not always addition

`'5' + 9` is `'59'`, and `'5' - 2` is `3` (rule r5). Convert input strings with `Number()` before you add them.[^1] See [[javascript/explicit-type-conversion]].

### valueOf wins in +

With both methods on one object, `obj + 1` uses `valueOf()` and gives `43`, while a template literal uses `toString()` ([`synopsis`](#synopsis)).[^2]

### Coercion with ==

`==` coerces an object to a primitive with no preferred type.[^2] See [[javascript/loose-equality]].

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| mixed numbers | numeric coercion, as for `'5' - 2`[^2] | "the operand with the “narrower” type is widened to that of the other"[^4] |
| object hooks | `[Symbol.toPrimitive]`, `valueOf()`, `toString()`[^2] | `__add__` and other numeric methods; `type(x).__add__(x, y)` for `x + y`[^5] |
| string form | the string hint[^3] | `__str__`, "Called by str(object)"[^5] |

Python widens numbers in mixed arithmetic, but a class must define methods such as `__add__` for its objects to take part.[^4][^5] JavaScript converts the object to a primitive first, then applies the operator to the primitives.[^3] See [[python/arithmetic-conversions]] and [[type-conversion]].

## References
