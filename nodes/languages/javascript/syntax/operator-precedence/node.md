---
tag: GEV9BN
kind: feature
owner: javascript
slug: operator-precedence
name: Operator precedence
aliases: [precedence, associativity, precedence table, operator grouping, order of operations]
area: languages/javascript/syntax
level: core
summary: >-
  Precedence and associativity decide how operators group, from grouping at level 18 to the comma at level 1. They do not change the order of evaluation: operands always run left to right.
grammar: "Each precedence level is a grammar rule: MultiplicativeExpression, AdditiveExpression, ..., AssignmentExpression, Expression (ECMA-262, 13)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html", locator: "13 ECMAScript Language: Expressions"}
see_also: [operator-precedence, order-of-evaluation, javascript/expressions-and-operators, javascript/logical-operators, javascript/nullish-coalescing-operator, javascript/exponentiation-operator, javascript/comma-operator, python/operator-precedence-in-python]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Operator_precedence
    title: Operator precedence, MDN
    locator: Precedence and associativity; Short-circuiting; Table
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html
    title: ECMAScript 2026 Language Specification
    locator: 13.6 Exponentiation Operator; 13.13 Binary Logical Operators; 13.15 Assignment Operators
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.5 The power operator; 6.17 Operator precedence
    accessed: 2026-11-18
---

## Synopsis

| Level | Operators | Associativity |
|---|---|---|
| 18 | grouping `(x)` | n/a |
| 17 | `x.y`, `x?.y`, `x[y]`, `new x(y)`, `x(y)`, `import(x)` | left to right |
| 16 | `new x` | n/a |
| 15 | `x++`, `x--` | n/a |
| 14 | `++x`, `--x`, `!x`, `~x`, `+x`, `-x`, `typeof x`, `void x`, `delete x`, `await x` | n/a |
| 13 | `**` | right to left |
| 12 | `*`, `/`, `%` | left to right |
| 11 | `+`, `-` | left to right |
| 10 | `<<`, `>>`, `>>>` | left to right |
| 9 | `<`, `<=`, `>`, `>=`, `in`, `instanceof` | left to right |
| 8 | `==`, `!=`, `===`, `!==` | left to right |
| 7, 6, 5 | `&`, then `^`, then `\|` | left to right |
| 4 | `&&` | left to right |
| 3 | `\|\|`, `??` | left to right |
| 2 | `=` and the compound assignments, `x ? y : z`, `=>`, `yield`, `...x` | right to left |
| 1 | `,` | left to right |

The MDN table "lists operators in order from highest precedence (18) to lowest precedence (1)."[^1]

```javascript run id=synopsis
let a;
let b;
console.log(3 + 4 * 5, 4 * 3 ** 2, (a = b = 5));
```

```output for=synopsis
23 36 5
```

## Semantics

**r1. Precedence.** "Operators with higher precedence become the operands of operators with lower precedence."[^1] If two operators have different levels, "the operator with the higher precedence goes first and associativity does not matter."[^1] So `3 + 10 * 2` is 23 and `(3 + 10) * 2` is 26. Example: [`grouping`](#grouping).

**r2. Associativity.** "Within operators of the same precedence, the language groups them by associativity ." Left-associative means `(a OP1 b) OP2 c`, and right-associative means `a OP1 (b OP2 c)`.[^1] Assignment and `**` are right-associative: `a = b = 5` is `a = (b = 5)`, and `4 ** 3 ** 2` is `4 ** (3 ** 2)`, or 262144.[^1] Example: [`grouping`](#grouping).

**r3. Not the order of evaluation.** "Note that operator precedence and associativity only affect the order of evaluation of operators (the implicit grouping), but not the order of evaluation of operands . The operands are always evaluated from left-to-right."[^1] In `f() + g() * h()`, `f` runs first, though `*` groups first. Example: [`evaluation`](#evaluation).

**r4. Short-circuit.** `&&`, `||`, `??` and `?.` may skip their right operand. MDN: "in the expression a && (b + c) , if a is falsy , then the sub-expression (b + c) will not even get evaluated, even if it is grouped and therefore has higher precedence than && ."[^1] Example: [`evaluation`](#evaluation).

**r5. Grammar limits.** The table is a summary. In ECMA-262, each level is a grammar rule, and some operands are narrower than the table shows.[^2] The left operand of `**` is an UpdateExpression, so `-2 ** 2` is a syntax error.[^2] The operands of `??` are BitwiseORExpressions, so `a || b ?? c` is a syntax error.[^2] MDN's notes say the same: the operands of `??` "cannot be a logical OR || or logical AND && operator without grouping."[^1] Example: [`limits`](#limits).

**r6. Unary operators and `new`.** A prefix unary operator at level 14 groups before a binary operator: `typeof a + b` is `(typeof a) + b`.[^1] `new` without arguments is level 16, below member access, so `new Foo++` is `(new Foo)++`, which is a syntax error.[^1] Example: [`grouping`](#grouping).

## Examples

The examples run with Node.js 24.

Grouping by precedence and by associativity (rules r1, r2 and r6). MDN gives these cases:[^1]

```javascript run id=grouping
console.log(3 + 10 * 2, 3 + (10 * 2), (3 + 10) * 2);
console.log(4 ** 3 ** 2, 4 / 3 / 2, 2 ** 3 / 3 ** 2);
console.log(1 + 2 ** 3 * 4 / 5 >> 6, (1 + ((2 ** 3) * 4) / 5) >> 6);
const a = 1;
const b = 2;
console.log(typeof a + b, typeof (a + b));
console.log(true || false && false, (true || false) && false);
```

```output for=grouping
23 23 26
262144 0.6666666666666666 0.8888888888888888
0 0
number2 number
true false
```

Operands run left to right, whatever the grouping (rules r3 and r4):

```javascript run id=evaluation
function echo(name, num) {
  console.log(`Evaluating the ${name} side`);
  return num;
}
console.log(echo("left", 4) ** echo("middle", 3) ** echo("right", 2));
console.log(echo("left", 4) / echo("middle", 3) ** echo("right", 2));

const skipped = false && echo("never", 1) + echo("never", 2);
console.log(skipped);
```

```output for=evaluation
Evaluating the left side
Evaluating the middle side
Evaluating the right side
262144
Evaluating the left side
Evaluating the middle side
Evaluating the right side
0.4444444444444444
false
```

A unary operand on the left of `**` needs parentheses (rule r5):

```javascript fails id=limits expect=exception:SyntaxError
console.log(-2 ** 2);
```

With parentheses, both readings are valid:

```javascript run id=limits-fixed
console.log((-2) ** 2, -(2 ** 2), (null || undefined) ?? "foo");
```

```output for=limits-fixed
4 -4 foo
```

## Pitfalls

### Precedence does not decide which call runs first

In `a() + b() * c()`, the calls run as `a`, `b`, `c` (rule r3).[^1] Code that depends on side effects in operands follows the text order, not the table.

### `&&` before `||`

`a || b && c` is `a || (b && c)`, because `&&` is at level 4 and `||` at level 3.[^1] Add parentheses when you mix them, for the reader.

### Bitwise operators below comparisons

`&`, `^` and `|` are at levels 7 to 5, below `===` at level 8.[^1] So `flags & MASK === MASK` is `flags & (MASK === MASK)`. Write `(flags & MASK) === MASK`.

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| table | 18 levels[^1] | a table "from highest precedence (most binding) to lowest precedence (least binding)"[^3] |
| `**` and unary minus | `-2 ** 2` is a syntax error[^2] | `-1**2` "results in -1"[^3] |
| comparisons | relational at 9, equality at 8[^1] | "comparisons, membership tests, and identity tests, all have the same precedence"[^3] |
| right to left | `**`, assignment, `? :`, `=>`[^1] | "exponentiation and conditional expressions"[^3] |

In Python, "The power operator binds more tightly than unary operators on its left; it binds less tightly than unary operators on its right."[^3] JavaScript makes the left case an error, so the reader never has to guess. See [[python/operator-precedence-in-python]] and [[operator-precedence]].

## References
