---
tag: G2D6XD
kind: feature
owner: javascript
slug: dynamic-and-weak-typing
name: Dynamic and weak typing
aliases: [dynamic typing, weak typing, dynamically typed, weakly typed, implicit conversion]
area: languages/javascript/values-and-types
level: intro
summary: >-
  In JavaScript, values have types and variables do not, so a variable can hold any type. Operators convert mismatched operands to a type they accept, in place of a type error.
see_also: [javascript/javascript-data-types-and-data-structures, javascript/primitive-values, javascript/bigints, javascript/string-operators, javascript/arithmetic-operators, static-and-dynamic-typing, type-conversion, python/objects-values-and-types]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Data_structures
    title: JavaScript data types and data structures, MDN
    locator: Dynamic and weak typing; BigInt type; Type coercion
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Dynamic_typing
    title: Dynamic typing, MDN Glossary
    locator: introduction
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Static_typing
    title: Static typing, MDN Glossary
    locator: introduction
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Type_coercion
    title: Type coercion, MDN Glossary
    locator: introduction; Examples
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Symbol
    title: Symbol, MDN
    locator: Symbol type conversions
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/datamodel.html
    title: Data model, The Python Language Reference
    locator: 3.1 Objects, values and types
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.7 Binary arithmetic operations
    accessed: 2026-11-18
---

## Synopsis

```javascript run id=synopsis
let foo = 42;
console.log(typeof foo);
foo = "bar";
console.log(typeof foo);
foo = true;
console.log(typeof foo);

const n = 42;
console.log(n + "1", typeof (n + "1"));
```

```output for=synopsis
number
string
boolean
421 string
```

"JavaScript is a dynamic language with dynamic types." "JavaScript is also a weakly typed language, which means it allows implicit type conversion when an operation involves mismatched types, instead of throwing type errors."[^1]

## Semantics

**r1. Dynamic typing.** "Variables in JavaScript are not directly associated with any particular value type, and any variable can be assigned (and re-assigned) values of all types".[^1] MDN defines dynamically typed languages as those "where the interpreter assigns variables a type at runtime based on the variable's value at the time."[^2] Example: [`synopsis`](#synopsis).

**r2. Against static typing.** In a statically typed language, such as Java, C or C++, "variable types are known at compile time."[^3] JavaScript has no type declarations on variables, so a check such as `typeof` runs when the program runs. Example: [`guard`](#guard).

**r3. Weak typing.** An operator with mismatched operands converts them. MDN calls this type coercion: "the automatic or implicit conversion of values from one data type to another (such as strings to numbers)."[^4] Example: [`coercion`](#coercion).

**r4. The direction of a conversion.** In `'5' + 9`, "JavaScript has coerced the 9 from a number into a string", which gives `'59'`. To get 14, "you'd have to explicitly convert the 5 to a number using the Number() method".[^4] Other arithmetic operators convert to numbers, so `'5' * 2` is 10. Example: [`coercion`](#coercion).

**r5. Bugs from coercion.** "Implicit coercions are very convenient, but can create subtle bugs when conversions happen where they are not expected, or where they are expected to happen in the other direction".[^1]

**r6. Limits.** "For symbols and BigInts , JavaScript has intentionally disallowed certain implicit type conversions."[^1] "A TypeError is thrown if BigInt values are mixed with regular numbers in arithmetic expressions".[^1] For a Symbol, `Symbol('foo') + 'bar'` "throws a TypeError".[^5] Example: [`bigint-mix`](#bigint-mix) and [`symbol-concat`](#symbol-concat).

## Examples

The examples run with Node.js 24.

Coercion in each direction, and an explicit conversion (rules r3 and r4):

```javascript run id=coercion
console.log("5" + 9, "5" * 2, "5" - 2, Number("5") + 9);
console.log("foo" + 1, true + 1, [1, 2] + "", null + 1);
```

```output for=coercion
59 10 3 14
foo1 2 1,2 1
```

A BigInt and a Number in one arithmetic expression throw (rule r6):

```javascript fails id=bigint-mix expect=exception:TypeError
console.log(1n + 1);
```

A Symbol does not convert to a string by `+` (rule r6):

```javascript fails id=symbol-concat expect=exception:TypeError
console.log(Symbol("foo") + "bar");
```

The type check runs at run time. A function that needs a number tests its argument (rule r2):

```javascript run id=guard
function double(x) {
  if (typeof x !== "number") {
    throw new TypeError(`expected a number, got ${typeof x}`);
  }
  return x * 2;
}
console.log(double(21));
try {
  double("21");
} catch (e) {
  console.log(e.message);
}
```

```output for=guard
42
expected a number, got string
```

## Pitfalls

### `+` with a string

If one operand of `+` is a string, `+` joins strings, so `'5' + 9` is `'59'`, not 14. Convert with `Number()` first (rule r4).[^4]

### One variable, many types

A variable can change type at any assignment (rule r1). Code that reads the variable later can get a type that it does not expect. Test the type where it matters, as in [`guard`](#guard).

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| where the type is | on the value[^1] | "Every object has an identity, a type and a value."[^6] |
| can the type change | a variable can take any type[^1] | "an object’s type is also unchangeable"[^6] |
| `+` with a string and a number | converts the number to a string[^4] | the operands "must either both be numbers or both be sequences of the same type"[^7] |

Both languages put the type on the value. Python does not convert a number to a string for `+`, so `'5' + 9` raises a TypeError in Python, but gives `'59'` in JavaScript.[^7] [^4] See [[python/objects-values-and-types]].

## References
