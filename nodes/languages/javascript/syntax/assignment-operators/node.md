---
tag: DM010Q
kind: feature
owner: javascript
slug: assignment-operators
name: Assignment operators
aliases: [assignment, "=", compound assignment, "+=", chained assignment, assignment expression, assignment target]
area: languages/javascript/syntax
level: intro
summary: >-
  x = y stores y in the target x, and the expression has the value of y. x op= y is x = x op y, but it evaluates x once. A bad target is a SyntaxError, and a const or read-only one a TypeError.
grammar: "AssignmentExpression : LeftHandSideExpression = AssignmentExpression | LeftHandSideExpression AssignmentOperator AssignmentExpression; AssignmentOperator : one of *= /= %= += -= <<= >>= >>>= &= ^= |= **= (ECMA-262, 13.15)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-assignment-operators", locator: "13.15 Assignment Operators"}
see_also: [assignment, javascript/expressions-and-operators, javascript/logical-assignment-operators, javascript/arithmetic-operators, javascript/exponentiation-operator, python/assignment-statements, python/augmented-assignment, python/assignment-expressions]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Expressions_and_operators
    title: Expressions and operators, MDN JavaScript Guide
    locator: Assignment operators
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-assignment-operators
    title: ECMAScript 2026 Language Specification
    locator: 13.15 Assignment Operators; 13.15.1 Static Semantics, Early Errors; 13.15.2 Runtime Semantics, Evaluation
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Assignment
    title: Assignment (=), MDN
    locator: Syntax; Description; Unqualified identifier assignment
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Addition_assignment
    title: Addition assignment (+=), MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/const
    title: const, MDN
    locator: introduction
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/simple_stmts.html
    title: Simple statements, The Python Language Reference
    locator: 7.2 Assignment statements; 7.2.1 Augmented assignment statements
    accessed: 2026-11-18
---

## Synopsis

| Form | Meaning |
|---|---|
| `x = y` | store `y` in `x` |
| `x += y`, `x -= y`, `x *= y`, `x /= y`, `x %= y`, `x **= y` | `x = x op y`, arithmetic |
| `x <<= y`, `x >>= y`, `x >>>= y`, `x &= y`, `x ^= y`, `x \|= y` | `x = x op y`, bitwise |
| `x &&= y`, `x \|\|= y`, `x ??= y` | assign only on a condition ([[javascript/logical-assignment-operators]]) |
| `[a, b] = [b, a]` | a destructuring pattern as the target |

"An assignment operator assigns a value to its left operand based on the value of its right operand."[^1] ECMA-262 lists twelve compound operators in AssignmentOperator, and three logical forms in separate productions.[^2]

## Semantics

**r1. The value of an assignment.** "The assignment expression itself has a value, which is the assigned value."[^3] The steps end with "Return rVal".[^2] So `console.log(x = 2)` prints 2. Example: [`basics`](#basics).

**r2. Chains.** `=` groups from the right, so `x = y = z` is `x = (y = z)`. "This allows multiple assignments to be chained in order to assign a single value to multiple variables."[^3] Example: [`basics`](#basics).

**r3. Compound assignment.** "x += y is equivalent to x = x + y, except that the expression x is only evaluated once."[^4] The steps read the old value with GetValue, apply the operator, and store the result with PutValue on the same reference.[^2] Example: [`once`](#once).

**r4. Order.** For `=`, the steps evaluate the target first ("Let lRef be ? Evaluation of LeftHandSideExpression"), then the right side.[^2] So in `obj[key()] = value()`, `key()` runs before `value()`. Example: [`once`](#once).

**r5. Targets.** The target is "A valid assignment target, including an identifier or a property accessor".[^3] It can also be a destructuring pattern.[^3] For compound operators, "It is a Syntax Error if the AssignmentTargetType of LeftHandSideExpression is not simple."[^2] A pattern is not simple, so `[a, b] += 1` is an error.

**r6. Errors when the code runs.** MDN lists a ReferenceError, "Thrown in strict mode if assigning to an identifier that is not declared in the scope", and a TypeError, "Thrown in strict mode if assigning to a property that is not modifiable".[^3] In sloppy mode, the first case creates a property on the global object, and the second does nothing.[^3] The value of a `const` "can't be changed through reassignment using the assignment operator", in all modes.[^5] Examples: [`modes`](#modes), [`const-target`](#const-target).

**r7. `=` in declarations.** The `=` in `let x = 1`, in a default parameter, or in a class field is not the assignment operator. MDN: "The assignment operator is completely different from the equals ( = ) sign used as syntactic separators in other locations".[^3]

## Examples

The examples run with Node.js 24.

Plain, chained, compound and destructuring assignment (rules r1, r2 and r5):

```javascript run id=basics
let x = 5, y = 10, z = 25;
x = y;
console.log(x);
x = y = z;
console.log(x, y, z);
console.log(x = 2, x);

let s = "hello";
s += " world";
let n = 2;
n **= 10;
n >>= 3;
console.log(s, n);

let a = 1, b = 2;
[a, b] = [b, a];
console.log(a, b);
```

```output for=basics
10
25 25 25
2 2
hello world 128
2 1
```

A compound assignment evaluates its target once, and `=` evaluates the target before the value (rules r3 and r4). The first assignment calls `target` once. The long form calls it twice:

```javascript run id=once
const obj = { count: 0 };
let reads = 0;
const target = () => {
  reads++;
  return obj;
};
target().count += 1;
console.log(obj.count, reads);
target().count = target().count + 1;
console.log(obj.count, reads);

const log = [];
const key = (k) => (log.push("key " + k), k);
const value = (v) => (log.push("value " + v), v);
obj[key("a")] = value(1);
console.log(log.join(", "));
```

```output for=once
1 1
2 3
key a, value 1
```

An undeclared name and a read-only property, in sloppy code and in strict code (rule r6):

```javascript run id=modes
function sloppy() {
  implicitGlobal = "created";
}
sloppy();
console.log(globalThis.implicitGlobal);

function strict() {
  "use strict";
  undeclaredName = 1;
}
try {
  strict();
} catch (error) {
  console.log(`${error.name}: ${error.message}`);
}

const frozen = Object.freeze({ v: 1 });
frozen.v = 2;
console.log(frozen.v);
(function () {
  "use strict";
  try {
    frozen.v = 2;
  } catch (error) {
    console.log(`${error.name}: ${error.message}`);
  }
})();
```

```output for=modes
created
ReferenceError: undeclaredName is not defined
1
TypeError: Cannot assign to read only property 'v' of object '#<Object>'
```

A `const` binding cannot take another value (rule r6):

```javascript fails id=const-target expect=exception:TypeError
const limit = 10;
limit = 20;
```

Node.js reports `TypeError: Assignment to constant variable.`

## Pitfalls

### `const x = y = 5`

MDN: "Which means y must be a pre-existing variable, and x is a newly declared const variable."[^3] If `y` does not exist, sloppy code makes a global `y`, and strict code throws a ReferenceError.[^3] To declare both, write `const x = 5, y = 5;`.[^3]

### `=` in place of `===`

`if (x = 0)` stores 0 and then tests 0, so the branch never runs. Use `===` to compare ([[javascript/comparison-operators]]).

### A frozen object in sloppy code

A write to a frozen or read-only property fails without an error in sloppy code ([`modes`](#modes)). Strict code shows the failure as a TypeError.[^3]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| kind | an expression with a value[^3] | a statement; `:=` is the expression form[^6] |
| chain `x = y = z` | `x = (y = z)`, from the right[^3] | "assigns the single resulting object to each of the target lists, from left to right"[^6] |
| `x += y` | evaluates `x` once[^4] | "The target is only evaluated once."[^6] |
| in-place change | no; `x += y` stores another value[^2] | "when possible, the actual operation is performed in-place"[^6] |
| undeclared name | a global in sloppy code, a ReferenceError in strict code[^3] | "the name is bound to the object in the current local namespace"[^6] |

Python says that assignment statements "are used to (re)bind names to values and to modify attributes or items of mutable objects".[^6] See [[python/assignment-statements]] and [[python/augmented-assignment]].

## References
