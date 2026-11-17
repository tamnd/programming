---
tag: SESNWW
kind: feature
owner: javascript
slug: comma-operator
name: Comma operator
aliases: [comma, "a, b", sequence expression, comma expression]
area: languages/javascript/syntax
level: intro
summary: >-
  a, b evaluates a, discards its value, then evaluates b and returns it. It has the lowest precedence of all operators. Commas in arrays, calls and declarations are separators, not this operator.
grammar: "Expression : AssignmentExpression | Expression , AssignmentExpression (ECMA-262, 13.16)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-comma-operator", locator: "13.16 Comma Operator ( , )"}
see_also: [javascript/expressions-and-operators, javascript/assignment-operators, javascript/update-expressions, javascript/punctuators, side-effect]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Comma_operator
    title: Comma operator (,), MDN
    locator: introduction; Description; Examples
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-comma-operator
    title: ECMAScript 2026 Language Specification
    locator: 13.16 Comma Operator; 13.16.1 Runtime Semantics, Evaluation
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.15 Expression lists
    accessed: 2026-11-18
---

## Synopsis

```javascript run id=synopsis
let x = 1;
x = (x++, x);
console.log(x);
x = (2, 3);
console.log(x);
```

```output for=synopsis
2
3
```

The comma operator "evaluates each of its operands (from left to right) and returns the value of the last operand."[^1] It is the outer rule of the Expression grammar, so any expression can be a list of assignment expressions joined by commas.[^2]

## Semantics

**r1. Steps.** The steps evaluate the left operand and call GetValue on it, then evaluate the right operand and return its value.[^2] The left value is not used, but "GetValue must be called even though its value is not used because it may have observable side-effects."[^2] Example: [`synopsis`](#synopsis).

**r2. Only side effects count.** "Because all expressions except the last are evaluated and then discarded, these expressions must have side effects to be useful."[^1] Examples of side effects are assignments, function calls, and `++` and `--`.[^1]

**r3. Lowest precedence.** "The comma operator has the lowest precedence of all operators." So `a = b = 3, c = 4` sets `a` to 3, and the whole expression gives 4. Put the comma expression in parentheses to use its value in a larger expression.[^1] Example: [`assignments`](#assignments).

**r4. Separators are a different thing.** "The comma operator is completely different from commas used as syntactic separators in other locations".[^1] These places include array and object initializers, parameter lists, call arguments, `let`, `const` and `var` binding lists, and import and export lists. There, a comma expression must be in parentheses: `const a = (1, b = 2);` sets `a` to 2.[^1] Example: [`separators`](#separators).

**r5. Use in `for`.** "The most common usage of this operator is to supply multiple updaters in a for loop."[^1] In `for (let i = 0, j = 9; i <= 9; i++, j--)`, the comma in the head is a separator, and the comma in the update is the operator.[^1] Example: [`for-loop`](#for-loop).

**r6. The result is a value, not a reference.** "The comma operator always returns the last expression as a value instead of a reference ." So `(0, obj.method)()` calls the method with `this` as `undefined`, but `(obj.method)()` keeps `this`.[^1] Example: [`reference`](#reference).

## Examples

The examples run with Node.js 24.

A loop with two counters (rule r5). It prints the other diagonal of a 3 by 3 grid:

```javascript run id=for-loop
const grid = [
  [1, 2, 3],
  [4, 5, 6],
  [7, 8, 9],
];
for (let i = 0, j = 2; i <= 2; i++, j--) {
  console.log(i, j, grid[i][j]);
}
```

```output for=for-loop
0 2 3
1 1 5
2 0 7
```

Precedence against assignment (rule r3). MDN gives this example:[^1]

```javascript run id=assignments
let a, b, c;
const result = (a = b = 3, c = 4);
console.log(a, b, c, result);

let x, y, z;
x = (y = 5, z = 6);
console.log(x, y, z);
```

```output for=assignments
3 3 4 4
6 5 6
```

Separators and the operator (rule r4). In the arrow function, the comma operator runs `sum += x` and then returns `x * x`:

```javascript run id=separators
const a = 1, b = 2;
console.log(a, b);
let d;
const e = (1, d = 2);
console.log(e, d);
console.log([1, 2, 3].length, [(1, 2, 3)].length);

let sum = 0;
const squares = [1, 2, 3, 4, 5].map((x) => ((sum += x), x * x));
console.log(squares, sum);
```

```output for=separators
1 2
2 2
3 1
[ 1, 4, 9, 16, 25 ] 15
```

A comma expression loses the `this` binding (rule r6):

```javascript run id=reference
const obj = {
  value: "obj",
  method() {
    return this === obj;
  },
};
console.log(obj.method(), (obj.method)(), (0, obj.method)());
```

```output for=reference
true true false
```

The third call does not get `obj` as `this`. The comma expression gives a plain function value, and the object it came from is lost.[^1]

## Pitfalls

### An extra comma changes the meaning

`console.log((1, 2))` prints 2, but `console.log(1, 2)` prints both numbers. Inside a call, a comma separates arguments (rule r4).

### `return a, b` returns `b`

`return (x += 1, x);` is the same as `return ++x;`.[^1] A `return` with a comma list returns the last value, which is easy to misread as two return values.

### No trailing comma operator

"Comma operators cannot appear as trailing commas ."[^1] A trailing comma in an array or a call is a separator, so `(a, b,)` in an expression is a syntax error:

```javascript fails id=trailing expect=exception:SyntaxError
let a = 1, b = 2;
console.log((a, b,));
```

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| `a, b` as an expression | evaluates both, gives `b`[^1] | gives the tuple `(a, b)`[^3] |
| order | left to right[^2] | "evaluated from left to right"[^3] |
| two updates in a loop | `i++, j--` in the `for` head | two statements in the body |

In Python, "an expression list containing at least one comma yields a tuple."[^3] So `x = 1, 2` makes `x` a tuple, and in JavaScript `x = 1, 2` sets `x` to 1 (rule r3). Python has no comma operator.

## References
