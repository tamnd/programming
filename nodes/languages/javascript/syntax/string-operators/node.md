---
tag: ETAF97
kind: feature
owner: javascript
slug: string-operators
name: String operators
aliases: [string concatenation, concatenation operator, "+ on strings", "+= on strings"]
area: languages/javascript/syntax
level: intro
summary: >-
  + joins two strings, and converts the other operand to a string when one side is a string. += appends. + works left to right, so 1 + 2 + '3' is '33'. Template literals also build strings.
grammar: "AdditiveExpression : AdditiveExpression + MultiplicativeExpression (ECMA-262, 13.8)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-applystringornumericbinaryoperator", locator: "13.15.3 ApplyStringOrNumericBinaryOperator"}
see_also: [javascript/arithmetic-operators, javascript/assignment-operators, javascript/comparison-operators, javascript/template-literals, javascript/strings, python/building-strings, python/strings]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Expressions_and_operators
    title: Expressions and operators, MDN JavaScript Guide
    locator: String operators
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-applystringornumericbinaryoperator
    title: ECMAScript 2026 Language Specification
    locator: 13.8 Additive Operators; 13.15.3 ApplyStringOrNumericBinaryOperator
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Addition
    title: Addition (+), MDN
    locator: Description; Addition using strings
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Addition_assignment
    title: Addition assignment (+=), MDN
    locator: Description; Addition assignment using strings
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Multiplication
    title: Multiplication (*), MDN
    locator: introduction
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Template_literals
    title: Template literals (Template strings), MDN
    locator: String interpolation
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.7 Binary arithmetic operations
    accessed: 2026-11-18
---

## Synopsis

```javascript run id=synopsis
let word = "alpha";
word += "bet";
console.log("my " + "string", word, 5 + "foo", 1 + 2 + "3", "1" + 2 + 3);
```

```output for=synopsis
my string alphabet 5foo 33 123
```

The concatenation operator `+` "concatenates two string values together, returning another string that is the union of the two operand strings." The shorthand `+=` "can also be used to concatenate strings."[^1] The same `+` does numeric addition. Which one runs depends on the operand types.

## Semantics

**r1. To primitives first.** For `+`, the steps are "Let lPrim be ? ToPrimitive ( lVal )" and "Let rPrim be ? ToPrimitive ( rVal )".[^2] An object operand calls its `valueOf()` first.[^3] Example: [`coercion`](#coercion).

**r2. One string makes a concatenation.** "If lPrim is a String or rPrim is a String , then" both are converted with ToString and the result is "the string-concatenation of lStr and rStr".[^2] MDN: "If one of the operands is a string, the other is converted to a string and they are concatenated".[^3] Example: [`synopsis`](#synopsis).

**r3. Else numeric addition.** If neither primitive is a string, both go to ToNumeric and the result is a number or a BigInt.[^2] "Strings have priority over other types, so adding a string to a BigInt produces string concatenation rather than a TypeError ."[^3] Example: [`coercion`](#coercion).

**r4. Left to right.** `+` is left-associative. AdditiveExpression is on the left of its own rule, so `a + b + c` is `(a + b) + c`.[^2] Each `+` checks its own two operands. So `1 + 2 + '3'` adds first and then joins, but `'1' + 2 + 3` joins twice. Example: [`synopsis`](#synopsis).

**r5. `+=`.** "x += y is equivalent to x = x + y , except that the expression x is only evaluated once."[^4] So `+=` joins under the same rules: `bar = 5; bar += 'foo'` gives `'5foo'`.[^4]

**r6. Only `+` joins.** The other arithmetic operators convert strings to numbers. MDN shows `'3' * 2` as 6 and `'foo' * 2` as NaN.[^5] There is no string repeat operator; use the `repeat` method. Example: [`coercion`](#coercion).

**r7. Template literals.** A template literal builds a string from parts and placeholders, such as `` `Fifteen is ${a + b}.` ``. MDN: "Template literals coerce their expressions directly to strings , while addition coerces its operands to primitives first."[^6] Example: [`template`](#template).

## Examples

The examples run with Node.js 24.

Conversions with `+` and the other operators (rules r1, r3 and r6). `JSON.stringify` shows the type of each result:

```javascript run id=coercion
const show = (v) => (typeof v === "bigint" ? v + "n" : JSON.stringify(v));
console.log(show("foo" + false), show("2" + 2), show("1" + 2n));
console.log(show(true + 1), show(null + "x"), show(undefined + "x"));
console.log(show([1, 2] + [3]), show({} + "x"));
console.log(show("3" * 2), show("foo" * 2), show("9" - "4"));
console.log("ab".repeat(3));
```

```output for=coercion
"foofalse" "22" "12"
2 "nullx" "undefinedx"
"1,23" "[object Object]x"
6 null 5
ababab
```

`JSON.stringify(NaN)` prints `null`. Arrays join their items with commas, and a plain object becomes `[object Object]`.

`+` calls `valueOf()`, and a template literal calls `toString()` (rules r1 and r7):

```javascript run id=template
const price = {
  valueOf() {
    return 42;
  },
  toString() {
    return "forty-two";
  },
};
console.log("" + price, `${price}`, "".concat(price), String(price));

const a = 5;
const b = 10;
console.log("Fifteen is " + (a + b) + " and not " + (2 * a + b) + ".");
console.log(`Fifteen is ${a + b} and not ${2 * a + b}.`);
```

```output for=template
42 forty-two forty-two forty-two
Fifteen is 15 and not 20.
Fifteen is 15 and not 20.
```

Build a long string in a loop with `+=`, or collect the parts and join them:

```javascript run id=build
let csv = "";
for (const row of [["a", 1], ["b", 2]]) {
  csv += row.join(",") + "\n";
}
console.log(JSON.stringify(csv));

const parts = [];
for (let i = 1; i <= 3; i++) parts.push("item" + i);
console.log(parts.join(" | "));
```

```output for=build
"a,1\nb,2\n"
item1 | item2 | item3
```

## Pitfalls

### The order of `+` changes the result

`1 + 2 + '3'` is `'33'`, but `'1' + 2 + 3` is `'123'` (rule r4). Use parentheses or a template literal when you mix numbers and strings.

### Form values are strings

Values from a form field or from the command line are strings, so `input + 1` joins in place of adding. Convert first with `Number(input)`.

### `'' + x` is not the same as `String(x)`

`'' + x` calls `valueOf()`, so MDN advises not to use it to "perform string coercion".[^3] Use `String(x)` or a template literal ([`template`](#template)).

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| join two strings | `a + b` | `a + b` |
| string plus number | joins, `5 + 'foo'` is `'5foo'`[^3] | TypeError: the operands "must either both be numbers or both be sequences of the same type"[^7] |
| repeat | `'ab'.repeat(3)`; `'ab' * 3` is NaN[^5] | `'ab' * 3`, "sequence repetition"[^7] |
| interpolation | template literals[^6] | f-strings, see [[python/formatted-string-literals]] |

Python's `+` does not convert: "The arguments must either both be numbers or both be sequences of the same type."[^7] So `'5' + 2` is an error in Python and `'52'` in JavaScript. See [[python/building-strings]] and [[python/strings]].

## References
