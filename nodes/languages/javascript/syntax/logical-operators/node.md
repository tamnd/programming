---
tag: X9QGDE
kind: feature
owner: javascript
slug: logical-operators
name: Logical operators
aliases: ["&&", "||", "!", logical AND, logical OR, logical NOT, double NOT, truthy, falsy]
area: languages/javascript/syntax
level: intro
summary: >-
  a && b and a || b return one of their operands, not always a boolean, and skip the right operand when the left one decides the result. !a always returns a boolean.
grammar: "LogicalANDExpression : LogicalANDExpression && BitwiseORExpression; LogicalORExpression : LogicalORExpression || LogicalANDExpression (ECMA-262, 13.13)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-binary-logical-operators", locator: "13.13 Binary Logical Operators"}
see_also: [short-circuit-evaluation, javascript/logical-assignment-operators, javascript/comparison-operators, javascript/expressions-and-operators, python/boolean-operators, python/truth-value-testing]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-binary-logical-operators
    title: ECMAScript 2026 Language Specification
    locator: 13.13 Binary Logical Operators; 13.13.1 Runtime Semantics, Evaluation; 13.5.7 Logical NOT Operator
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Logical_AND
    title: Logical AND (&&), MDN
    locator: Description; Short-circuit evaluation; Operator precedence
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Falsy
    title: Falsy, MDN Glossary
    locator: table of falsy values
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Logical_NOT
    title: Logical NOT (!), MDN
    locator: Description; Double NOT (!!)
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Logical_OR
    title: Logical OR (||), MDN
    locator: Description; Short-circuit evaluation; Examples
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.11 Boolean operations
    accessed: 2026-11-18
---

## Synopsis

| Expression | Value | Right operand evaluated |
|---|---|---|
| `a && b` | `a` if `a` is falsy, else `b` | only if `a` is truthy |
| `a \|\| b` | `a` if `a` is truthy, else `b` | only if `a` is falsy |
| `!a` | `false` if `a` is truthy, else `true` | (one operand) |

ECMA-262 notes: "The value produced by a && or || operator is not necessarily of type Boolean. The value produced will always be the value of one of the two operand expressions."[^1]

```javascript run id=synopsis
console.log("Cat" && "Dog", "" && "Dog", "Cat" || "Dog", 0 || "Dog", !"Cat");
```

```output for=synopsis
Dog  Cat Dog false
```

## Semantics

**r1. Truthy and falsy.** "If a value can be converted to true , the value is so-called truthy . If a value can be converted to false , the value is so-called falsy ."[^2] The falsy values are `false`, `null`, `undefined`, `NaN`, `0`, `-0`, `0n` and the empty string. In browsers, the object `document.all` is falsy too.[^3] Every other value is truthy, and this includes every other object: `!!{}` is `true`.[^4] Example: [`truthiness`](#truthiness).

**r2. `&&`.** The steps evaluate the left operand and get its value. "If ToBoolean ( lVal ) is false , return lVal." Else they evaluate the right operand and return its value.[^1] MDN puts it so: `&&` returns "the value of the first falsy operand it encounters; if all values are truthy , the value of the last operand is returned."[^2] Example: [`values`](#values).

**r3. `||`.** The steps are the same with the test turned: "If ToBoolean ( lVal ) is true , return lVal."[^1] MDN: "If x can be converted to true , returns x ; else, returns y ."[^5] Example: [`values`](#values).

**r4. Short-circuit.** When the left operand decides the result, the right operand is not evaluated. For `&&`, "it does not evaluate any of the remaining operands" after a falsy one.[^2] For `||`, "the expr part above is not evaluated , hence any side effects of doing so do not take effect".[^5] Example: [`short-circuit`](#short-circuit).

**r5. `!`.** `!` converts its operand with ToBoolean and returns the opposite boolean.[^1] It "Returns false if its single operand can be converted to true ; otherwise, returns true ."[^4] Two of them, `!!x`, give the boolean for `x`, the same as `Boolean(x)`.[^4] Example: [`truthiness`](#truthiness).

**r6. Precedence.** `!` is a unary operator, so it binds tighter than all binary operators.[^1] "The AND operator has a higher precedence than the OR operator, meaning the && operator is executed before the || operator".[^2] So `a || b && c` is `a || (b && c)`. In the grammar, an operand of `&&` is a BitwiseORExpression, so comparisons bind tighter than `&&`.[^1] Example: [`precedence`](#precedence).

## Examples

The examples run with Node.js 24.

`&&` and `||` return an operand (rules r2 and r3). `JSON.stringify` shows the empty string:

```javascript run id=values
const show = (v) => JSON.stringify(v);
console.log(show("" && "foo"), show(2 && 0), show("foo" && 4));
console.log(show("Cat" || "Dog"), show(false || "Cat"), show(false || ""));
console.log(show(null || 0 || "last"), show(1 && "a" && null && "b"));
```

```output for=values
"" 0 4
"Cat" "Cat" ""
"last" null
```

The right operand runs only when it is needed (rule r4):

```javascript run id=short-circuit
function A() {
  console.log("called A");
  return false;
}
function B() {
  console.log("called B");
  return true;
}
console.log(A() && B());
console.log(B() || A());

const user = null;
console.log(user && user.name);
```

```output for=short-circuit
called A
false
called B
true
null
```

Falsy and truthy values, with `!!` and `Boolean` (rules r1 and r5). Empty arrays and objects are truthy:

```javascript run id=truthiness
const values = [false, null, undefined, NaN, 0, -0, 0n, "", "0", " ", [], {}, -1];
const labels = ["false", "null", "undefined", "NaN", "0", "-0", "0n", '""', '"0"', '" "', "[]", "{}", "-1"];
values.forEach((v, i) => {
  console.log(labels[i].padEnd(9), !!v, Boolean(v) === !!v);
});
```

```output for=truthiness
false     false true
null      false true
undefined false true
NaN       false true
0         false true
-0        false true
0n        false true
""        false true
"0"       true true
" "       true true
[]        true true
{}        true true
-1        true true
```

`&&` binds tighter than `||` (rule r6). The MDN examples:[^2] [^5]

```javascript run id=precedence
console.log(true || false && false);
console.log((true || false) && false);
console.log((2 === 3) || (4 < 0) && (1 === 1));
console.log(!0 + 1, !(0 + 1));
```

```output for=precedence
true
false
false
2 false
```

## Pitfalls

### `||` drops valid falsy values

`count || 10` gives 10 for a count of 0, and `name || 'anon'` replaces an empty string. MDN warns: "If you use this operator to provide a default value to some variable, be aware that any falsy value will not be used."[^5] Use `??` when only `null` and `undefined` mark a missing value.

### Empty arrays and objects are truthy

`if (list)` is true for an empty array, because every object is truthy (rule r1).[^4] Test `list.length` to check for items.

### The string `'0'` is truthy

Only the empty string is falsy.[^3] A string that holds `'0'` or `'false'` is truthy, so convert form input before the test.

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| spelling | `&&`, `\|\|`, `!` | `and`, `or`, `not`[^6] |
| `and` and `or` return | an operand[^1] | "the last evaluated argument"[^6] |
| not returns | a boolean[^4] | a boolean, "regardless of the type of its argument"[^6] |
| empty array or list | truthy[^4] | false, as are empty "strings and containers"[^6] |

Python's `or` has the same rule as JavaScript's `||`: "The expression x or y first evaluates x ; if x is true, its value is returned; otherwise, y is evaluated and the resulting value is returned."[^6] The large difference is the set of false values. See [[python/boolean-operators]] and [[python/truth-value-testing]].

## References
