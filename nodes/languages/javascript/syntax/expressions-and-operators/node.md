---
tag: 76PM67
kind: feature
owner: javascript
slug: expressions-and-operators
name: Expressions and operators
aliases: [expression, operator, operand, binary operator, unary operator, ternary operator, evaluation order]
area: languages/javascript/syntax
level: intro
summary: >-
  An expression is code that resolves to a value. Operators join operands into larger expressions. Precedence and associativity set the grouping, and the operands are evaluated from left to right.
grammar: "Expression : AssignmentExpression | Expression , AssignmentExpression (ECMA-262, 13.16 Comma Operator); each operator level from 13.3 to 13.15 is a production of its own"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-ecmascript-language-expressions", locator: "13 ECMAScript Language: Expressions"}
see_also: [statements-and-expressions, operator-precedence, order-of-evaluation, javascript/statements-and-declarations, javascript/assignment-operators, javascript/arithmetic-operators, javascript/comparison-operators, python/operator-precedence-in-python]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-ecmascript-language-expressions
    title: ECMAScript 2026 Language Specification
    locator: "13 ECMAScript Language: Expressions; 13.15.1 Static Semantics: Early Errors"
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Expressions_and_operators
    title: Expressions and operators, MDN JavaScript Guide
    locator: introduction
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators
    title: Expressions and operators, MDN
    locator: Expressions and operators by category; What are operators?
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Operator_precedence
    title: Operator precedence, MDN
    locator: Precedence and associativity
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.12 Assignment expressions; 6.13 Conditional expressions; 6.16 Evaluation order
    accessed: 2026-11-18
---

## Synopsis

| ECMA-262 section | Operators | Page |
|---|---|---|
| 13.2 Primary Expression | `this`, names, literals, `( )` | |
| 13.3 Left-Hand-Side Expressions | `a.b`, `a[b]`, `a?.b`, `f()`, `new C()` | |
| 13.4 Update Expressions | `++`, `--` | [[javascript/update-expressions]] |
| 13.5 Unary Operators | `delete`, `void`, `typeof`, `+`, `-`, `~`, `!`, `await` | |
| 13.6 to 13.8 | `**`, `*`, `/`, `%`, `+`, `-` | [[javascript/arithmetic-operators]] |
| 13.9 Bitwise Shift Operators | `<<`, `>>`, `>>>` | [[javascript/bitwise-operators]] |
| 13.10 Relational Operators | `<`, `>`, `<=`, `>=`, `instanceof`, `in` | |
| 13.11 Equality Operators | `==`, `!=`, `===`, `!==` | [[javascript/comparison-operators]] |
| 13.12 Binary Bitwise Operators | `&`, `^`, `\|` | [[javascript/bitwise-operators]] |
| 13.13 Binary Logical Operators | `&&`, `\|\|`, `??` | |
| 13.14 Conditional Operator | `c ? a : b` | |
| 13.15 Assignment Operators | `=`, `+=`, `&&=` and the others | [[javascript/assignment-operators]] |
| 13.16 Comma Operator | `a, b` | |

The sections are those of clause 13.[^1] The order of the rows follows the grammar, from the primary expressions down to the comma.[^1]

## Semantics

**r1. Expressions.** An expression "is a valid unit of code that resolves to a value."[^2] MDN names two types: "those that have side effects (such as assigning values) and those that purely evaluate."[^2] `x = 7` is of the first type, and its value is 7. `3 + 4` is of the second type.[^2] Example: [`kinds`](#kinds).

**r2. Operators.** "Where an expression contains slots for further nested expressions, the part(s) that are not slots are known as operators."[^3] In `a + b`, the operator is `+`, and "each operator corresponds to an expression."[^3]

**r3. Operand count.** A binary operator takes two operands, and "All binary operators in JavaScript are infix."[^2] A unary operator takes one operand. MDN says that `++` and `--` are the only postfix operators, and that the other unary operators are prefix.[^2] The conditional operator `c ? a : b` is the one ternary operator.[^2] Example: [`kinds`](#kinds).

**r4. Grouping.** "The precedence of operators determines the order they are applied when evaluating an expression."[^2] So `1 + 2 * 3` is 7. Parentheses change the grouping.[^2] Within one level of precedence, associativity sets the grouping: "Assignment operators are right-associative".[^4] The operator precedence page of MDN has the full table.[^4]

**r5. Evaluation order.** Precedence and associativity "only affect the order of evaluation of operators (the implicit grouping), but not the order of evaluation of operands."[^4] "The operands are always evaluated from left-to-right."[^4] The logical operators `&&`, `||` and `??` and the conditional operator can skip an operand.[^4] Example: [`order`](#order).

**r6. Assignment targets.** The left side of `=` must be a valid target: "It is a Syntax Error if the AssignmentTargetType of LeftHandSideExpression is not simple."[^1] This is a compile error, so no part of the code runs. Example: [`bad-target`](#bad-target).

**r7. Discarded values.** A value that is not part of a bigger construct is discarded. MDN calls this usually a programmer mistake, if the expression has no side effects.[^2] [[javascript/statements-and-declarations]] covers the expression statement.

## Examples

The examples run with Node.js 24.

An assignment with a side effect and an addition without one, then precedence, unary and postfix operators, the ternary operator and the comma operator (rules r1, r3 and r4):

```javascript run id=kinds
let x;
console.log(x = 7, 3 + 4, x);
console.log(1 + 2 * 3, (1 + 2) * 3);
console.log(typeof x, -x, !x, x++, x, ++x);
console.log(x > 5 ? "big" : "small");
console.log((x = 1, x + 1));
```

```output for=kinds
7 7 7
7 9
number -7 false 7 8 9
big
2
```

The operands run from left to right, also where precedence groups the right side first (rule r5). `*` binds tighter than `+`, and `**` groups from the right, but the calls still run in the order of the source:

```javascript run id=order
const echo = (name, value) => {
  console.log("evaluating", name);
  return value;
};
console.log(echo("a", 1) + echo("b", 2) * echo("c", 3));
console.log(echo("d", 2) ** echo("e", 3) ** echo("f", 2));
```

```output for=order
evaluating a
evaluating b
evaluating c
7
evaluating d
evaluating e
evaluating f
512
```

`n + 1` is not a place to store a value (rule r6):

```javascript fails id=bad-target expect=exception:SyntaxError
let n = 1;
n + 1 = 3;
```

Node.js reports `SyntaxError: Invalid left-hand side in assignment`.

## Pitfalls

### Precedence does not set the order of side effects

In `f() + g() * h()`, `f` runs first, although the multiplication groups first (rule r5). Code that needs a fixed order of side effects must use separate statements.

### A discarded expression

`x + 1;` alone is a valid statement, but it changes nothing. `x === 1;` in place of `x = 1;` gives no error. A linter can find such statements.

### Assignment inside a condition

`=` is an operator, so `if (x = 0)` is valid. It stores 0 and tests the value 0, which is false. Use `===` for a test ([[javascript/comparison-operators]]).

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| assignment | `=` is an operator inside expressions[^1] | `=` is a statement; `:=` is an assignment expression[^5] |
| evaluation order | operands from left to right[^4] | "Python evaluates expressions from left to right."[^5] |
| the right side of `=` | the target reference is evaluated first[^1] | "the right-hand side is evaluated before the left-hand side"[^5] |
| ternary | `c ? a : b`[^2] | `a if c else b`[^5] |

Python calls `:=` an "assignment expression (sometimes also called a “named expression” or “walrus”)".[^5] See [[python/assignment-expressions]] and [[python/operator-precedence-in-python]].

## References
