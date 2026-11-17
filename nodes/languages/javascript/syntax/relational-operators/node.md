---
tag: KNMJE7
kind: feature
owner: javascript
slug: relational-operators
name: Relational operators
aliases: ["<", ">", "<=", ">=", in, instanceof, "#x in obj", less than, greater than]
area: languages/javascript/syntax
level: core
summary: >-
  <, >, <= and >= order two values, in tests for a property in an object or its prototype chain, and instanceof tests a prototype chain. Each returns a boolean.
grammar: "RelationalExpression : ShiftExpression | RelationalExpression < ShiftExpression | ... | RelationalExpression instanceof ShiftExpression | RelationalExpression in ShiftExpression | PrivateIdentifier in ShiftExpression (ECMA-262, 13.10)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-relational-operators", locator: "13.10 Relational Operators"}
see_also: [javascript/comparison-operators, javascript/expressions-and-operators, javascript/unary-operators, python/comparison-operators]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-relational-operators
    title: ECMAScript 2026 Language Specification
    locator: 13.10 Relational Operators; 13.10.1 Runtime Semantics, Evaluation; 13.10.2 InstanceofOperator
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Less_than
    title: Less than (<), MDN
    locator: Description; Examples
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Greater_than_or_equal
    title: Greater than or equal (>=), MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/in
    title: in, MDN
    locator: introduction; Exceptions; Description; Examples
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/instanceof
    title: instanceof, MDN
    locator: introduction; Exceptions; Description; instanceof and Symbol.hasInstance
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.10 Comparisons; 6.10.2 Membership test operations
    accessed: 2026-11-18
---

## Synopsis

| Expression | True when |
|---|---|
| `a < b`, `a > b`, `a <= b`, `a >= b` | the order holds after conversion |
| `key in obj` | `obj` or its prototype chain has the property `key` |
| `#field in obj` | `obj` has the private element `#field` |
| `obj instanceof C` | `C.prototype` is in the prototype chain of `obj` |

"The result of evaluating a relational operator is always of type Boolean, reflecting whether the relationship named by the operator holds between its two operands."[^1]

```javascript run id=synopsis
console.log("a" < "b", 5 >= "3", "length" in [], [] instanceof Array);
```

```output for=synopsis
true true true true
```

## Semantics

**r1. One algorithm for order.** All four order operators call IsLessThan. `a > b` swaps the operands, and `a >= b` and `a <= b` negate the result.[^1] "Other operators, including > , >= , and <= , use the same algorithm as < ."[^2] An undefined result, from `NaN`, gives `false` for all four.[^1]

**r2. Conversion.** "First, objects are converted to primitives". "If both values are strings, they are compared as strings, based on the values of the UTF-16 code units (not Unicode code points) they contain."[^2] Else both sides become numbers: `true` is 1, `null` is 0 and `undefined` is NaN. "BigInt and number values can be compared together."[^2] Example: [`order`](#order).

**r3. `>=` is not `>` or `==`.** "x >= y is generally equivalent to !(x < y)", except when a side becomes NaN.[^3] It is also not always `x > y || x == y`: with `null` and 0, "x >= y is true , while x > y || x == y is false ."[^3] Example: [`order`](#order).

**r4. Left first.** "All comparison operators coerce the left operand before the right operand." This is visible when the conversion has side effects.[^2] Example: [`coercion-order`](#coercion-order).

**r5. `in`.** "The in operator returns true if the specified property is in the specified object or its prototype chain."[^4] The left operand is a property name, so `0 in trees` tests an index, not a value. For values, MDN points to `Array.prototype.includes()`.[^4] A property set to `undefined` is still present, and one removed with `delete` is not.[^4] If the right operand is a primitive, `in` throws a TypeError.[^4] Example: [`in`](#in).

**r6. `#x in obj`.** The grammar has a separate form `PrivateIdentifier in ShiftExpression`.[^1] It "returns true if and only if the object was created with that class constructor", and MDN calls it a "branded check".[^4] Example: [`in`](#in).

**r7. `instanceof`.** It "tests to see if the prototype property of a constructor appears anywhere in the prototype chain of an object."[^5] The steps first look for a `Symbol.hasInstance` method on the right operand and call it if it exists. "If target is not an Object , throw a TypeError exception."[^1] Example: [`instanceof`](#instanceof).

**r8. `in` in a `for` head.** "The [In] grammar parameter is needed to avoid confusing the in operator in a relational expression with the in operator in a for statement."[^1] To use the `in` operator inside a `for (...;...;...)` initializer, put it in parentheses.

## Examples

The examples run with Node.js 24.

String, number, BigInt and mixed order (rules r2 and r3). MDN gives these cases:[^2] [^3]

```javascript run id=order
console.log("a" < "b", "a" < "3", "B" < "a", "10" < "9", "10" < 9);
console.log("\uD855\uDE51" < "\uFF3A", 5n >= 3, 3 >= 5n, 1n < 1.5);
console.log(null >= 0, null > 0 || null == 0, undefined >= 3, 3 >= NaN);
console.log("hello" >= 5, 5 >= "hello", true >= 1);
```

```output for=order
true false true true false
true true false true
true false false false
false false true
```

The string `'\uD855\uDE51'` is one character above U+FFFF, written as two code units. Its first code unit is less than U+FF3A, so it sorts first.

The left operand converts before the right (rule r4):

```javascript run id=coercion-order
const log = [];
const left = { valueOf() { log.push("left"); return 1; } };
const right = { valueOf() { log.push("right"); return 2; } };
console.log(left < right, left > right, log.join(" "));
```

```output for=coercion-order
true false left right left right
```

`in` with arrays, inherited properties, `undefined` values and private elements (rules r5 and r6):

```javascript run id=in
const trees = ["redwood", "bay", "cedar"];
console.log(0 in trees, 3 in trees, "bay" in trees, trees.includes("bay"), "length" in trees);

const car = { make: "Honda", model: undefined };
console.log("model" in car, "toString" in car, Object.hasOwn(car, "toString"));
delete car.make;
console.log("make" in car);

class Point {
  #x = 0;
  static isPoint(obj) {
    return #x in obj;
  }
}
console.log(Point.isPoint(new Point()), Point.isPoint({}));
```

```output for=in
true false false true true
true true false
false
true false
```

`instanceof` follows the prototype chain, and `Symbol.hasInstance` overrides it (rule r7):

```javascript run id=instanceof
class A {}
class B extends A {}
const b = new B();
console.log(b instanceof B, b instanceof A, b instanceof Object, new A() instanceof B);
console.log(Object.create(null) instanceof Object, "text" instanceof String);

class Even {
  static [Symbol.hasInstance](n) {
    return n % 2 === 0;
  }
}
console.log(4 instanceof Even, 3 instanceof Even);
```

```output for=instanceof
true true true false
false false
true false
```

A primitive on the right of `in` throws (rule r5):

```javascript fails id=in-primitive expect=exception:TypeError
console.log("length" in "text");
```

## Pitfalls

### Strings compare by code unit

`'10' < '9'` is true, and `'B' < 'a'` is true, because the comparison uses UTF-16 code units (rule r2).[^2] Convert to numbers for numeric order.

### `in` tests keys, not values

`'bay' in trees` is false for an array that holds `'bay'` ([`in`](#in)). Use `includes` for values.[^4]

### `in` sees inherited properties

`'toString' in {}` is true. Use `Object.hasOwn()` "to check for only non-inherited properties".[^4]

### `instanceof` and primitives

A string primitive is not an instance of `String` ([`instanceof`](#instanceof)). Use `typeof` for primitives.

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| order operators | `<`, `>`, `<=`, `>=` with conversion[^2] | `<`, `>`, `<=`, `>=`[^6] |
| chains | `a < b < c` is `(a < b) < c`[^1] | "x < y <= z is equivalent to x < y and y <= z"[^6] |
| membership | `key in obj` tests property names[^4] | `x in s` "evaluates to True if x is a member of s"[^6] |
| `in` on a list or array | tests indexes[^4] | tests items[^6] |

Python's `in` tests values: for a list, "x in y is equivalent to any(x is e or x == e for e in y)".[^6] JavaScript's `in` on an array tests indexes, so the same code means different things. See [[python/comparison-operators]].

## References
