---
tag: QTRCNV
kind: feature
owner: javascript
slug: logical-assignment-operators
name: Logical assignment operators
aliases: ["||=", "&&=", "??=", logical OR assignment, logical AND assignment, nullish coalescing assignment, logical nullish assignment]
area: languages/javascript/syntax
level: core
summary: >-
  x ||= y, x &&= y and x ??= y assign y only when x is falsy, truthy or nullish. They short-circuit: if no assignment is due, y is not evaluated and no setter runs. Each evaluates x once.
grammar: "AssignmentExpression : LeftHandSideExpression &&= AssignmentExpression | LeftHandSideExpression ||= AssignmentExpression | LeftHandSideExpression ??= AssignmentExpression (ECMA-262, 13.15)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-assignment-operators", locator: "13.15 Assignment Operators"}
see_also: [short-circuit-evaluation, javascript/assignment-operators, javascript/expressions-and-operators, python/boolean-operators, python/augmented-assignment]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Logical_OR_assignment
    title: Logical OR assignment (||=), MDN
    locator: introduction; Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Logical_AND_assignment
    title: Logical AND assignment (&&=), MDN
    locator: introduction; Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Nullish_coalescing_assignment
    title: Nullish coalescing assignment (??=), MDN
    locator: introduction; Description; Using nullish coalescing assignment
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-assignment-operators
    title: ECMAScript 2026 Language Specification
    locator: 13.15.1 Static Semantics, Early Errors; 13.15.2 Runtime Semantics, Evaluation
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/simple_stmts.html
    title: Simple statements, The Python Language Reference
    locator: 7.2.1 Augmented assignment statements
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.11 Boolean operations
    accessed: 2026-11-18
---

## Synopsis

| Form | Same as | Assigns when `x` is |
|---|---|---|
| `x \|\|= y` | `x \|\| (x = y)` | falsy |
| `x &&= y` | `x && (x = y)` | truthy |
| `x ??= y` | `x ?? (x = y)` | `null` or `undefined` |

Each form is "equivalent to" the second column, "except that the expression x is only evaluated once."[^1] [^2] [^3] The value of the whole expression is the old `x` when no assignment happens, and `y` when it does.[^4]

## Semantics

**r1. `||=`.** It "only evaluates the right operand and assigns to the left if the left operand is falsy."[^1] Falsy values include `0`, the empty string, `null` and `undefined`. The steps: "If ToBoolean ( lVal ) is true , return lVal."[^4] Example: [`forms`](#forms).

**r2. `&&=`.** It "only evaluates the right operand and assigns to the left if the left operand is truthy."[^2] The steps return the old value if ToBoolean of it is false.[^4] Example: [`forms`](#forms).

**r3. `??=`.** It "only evaluates the right operand and assigns to the left if the left operand is nullish ( null or undefined )."[^3] So `0`, the empty string and `false` stay. Examples: [`forms`](#forms), [`defaults`](#defaults).

**r4. Short-circuit.** "No assignment is performed if the left-hand side is not falsy, due to short-circuiting of the logical OR operator."[^1] Then no setter runs, and a `const` target throws no error.[^1] And "if x is not falsy, y is not evaluated at all."[^1] The same holds for `&&=` and `??=` with their own tests.[^2] [^3] Example: [`short-circuit`](#short-circuit).

**r5. Once only.** The steps evaluate the target reference once, read it once, and write to the same reference.[^4] This is the difference from `x = x || y`, which always writes.

**r6. Targets.** The target must be simple: an identifier or a property access. For each of the three forms, "It is a Syntax Error if the AssignmentTargetType of LeftHandSideExpression is not simple."[^4] A destructuring pattern is not allowed.

## Examples

The examples run with Node.js 24.

The three forms on falsy, truthy and nullish values (rules r1 to r3). The empty string is falsy but not nullish, and `0` is not nullish:

```javascript run id=forms
let a = 0, b = 1, c = null;
a ||= 10;
b ||= 10;
c ??= 10;
console.log(a, b, c);

let d = 1, e = 0;
d &&= "and";
e &&= "and";
console.log(d, e);

let empty = "", missing, zero = 0;
empty ||= "or";
missing ??= "nullish";
zero ??= 5;
console.log(JSON.stringify(empty), missing, zero);
```

```output for=forms
10 1 10
and 0
"or" nullish 0
```

No setter call, no `const` error and no call to `compute` when the test does not pass (rule r4). Only `&&=` writes, because the value 1 is truthy:

```javascript run id=short-circuit
const settings = {
  stored: 1,
  get value() {
    return this.stored;
  },
  set value(v) {
    console.log("setter called with", v);
    this.stored = v;
  },
};
settings.value ||= 2;
settings.value &&= 3;
console.log(settings.value);

const fixed = 1;
fixed ||= 2;
console.log(fixed);

let calls = 0;
const compute = () => (calls++, "computed");
let cache = "ready";
cache ||= compute();
console.log(cache, calls);
```

```output for=short-circuit
setter called with 3
3
1
ready 0
```

Default values for options with `??=` (rule r3). MDN notes that `??=` "also applies the default value if the property has value null", where a destructuring default does not.[^3] A `speed` of 0 stays:

```javascript run id=defaults
function config(options) {
  options.duration ??= 100;
  options.speed ??= 25;
  return options;
}
console.log(config({ duration: 125 }));
console.log(config({ duration: null, speed: 0 }));
```

```output for=defaults
{ duration: 125, speed: 25 }
{ duration: 100, speed: 0 }
```

When the test passes, the assignment happens, and a `const` target throws (rule r4):

```javascript fails id=const-assigns expect=exception:TypeError
const count = 0;
count ||= 1;
```

Node.js reports `TypeError: Assignment to constant variable.`

## Pitfalls

### `||=` replaces valid values

`count ||= 10` replaces a count of 0, and `name ||= "anon"` replaces an empty string. Use `??=` when only `null` and `undefined` mark a missing value.[^1]

### Not the same as `x = x || y`

`x = x || y` always writes to `x`, so a setter runs, and a `const` target throws. `x ||= y` writes only when it must (rule r4).[^1]

### A `const` passes by luck

`const x = 1; x ||= 2;` gives no error, because no assignment happens.[^1] The same code with `const x = 0` throws ([`const-assigns`](#const-assigns)). The error depends on the value, so tests with one value can miss it.

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| logical assignment | `\|\|=`, `&&=`, `??=`[^4] | none; the augmented operators are arithmetic and bitwise only[^5] |
| the long form | `x = x \|\| y` | `x = x or y` |
| result of `or` | an operand, not a boolean | "if x is true, its value is returned; otherwise, y is evaluated and the resulting value is returned"[^6] |
| nullish test | `??=` | `if x is None: x = y` |

Python's augmented assignment is "the combination, in a single statement, of a binary operation and an assignment statement".[^5] `or` is not in its list. See [[python/augmented-assignment]] and [[python/boolean-operators]].

## References
