---
tag: 04BH9Z
kind: feature
owner: javascript
slug: nullish-coalescing-operator
name: Nullish coalescing operator
aliases: ["??", nullish coalescing, nullish, default value operator]
area: languages/javascript/syntax
level: core
summary: >-
  a ?? b returns b only when a is null or undefined, and a in all other cases, so 0, false and the empty string stay. It cannot mix with && or || without parentheses.
grammar: "CoalesceExpression : CoalesceExpressionHead ?? BitwiseORExpression; CoalesceExpressionHead : CoalesceExpression | BitwiseORExpression (ECMA-262, 13.13)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-binary-logical-operators", locator: "13.13 Binary Logical Operators"}
see_also: [short-circuit-evaluation, javascript/logical-operators, javascript/logical-assignment-operators, javascript/expressions-and-operators, python/conditional-expressions]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Nullish_coalescing
    title: Nullish coalescing operator (??), MDN
    locator: introduction; Description; Examples
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-binary-logical-operators
    title: ECMAScript 2026 Language Specification
    locator: 13.13 Binary Logical Operators; 13.13.1 Runtime Semantics, Evaluation
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Operator_precedence
    title: Operator precedence, MDN
    locator: Table
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.13 Conditional expressions
    accessed: 2026-11-18
---

## Synopsis

```javascript run id=synopsis
const count = 0;
console.log(count ?? 42, count || 42, null ?? 42, undefined ?? 42);
```

```output for=synopsis
0 42 42 42
```

`??` "returns its right-hand side operand when its left-hand side operand is null or undefined , and otherwise returns its left-hand side operand."[^1]

| Left operand | `a ?? b` | `a \|\| b` |
|---|---|---|
| `null` or `undefined` | `b` | `b` |
| `0`, `NaN`, `false`, the empty string | `a` | `b` |
| any other value | `a` | `a` |

## Semantics

**r1. The test.** The steps evaluate the left operand and get its value. "If lVal is neither undefined nor null , return lVal." Else they evaluate the right operand and return its value.[^2] There is no conversion to a boolean. Example: [`defaults`](#defaults).

**r2. Compared with `||`.** MDN says `??` "can be seen as a special case of the logical OR ( || ) operator". `||` "returns the right-hand side operand if the left operand is any falsy value, not only null or undefined ."[^1] Example: [`defaults`](#defaults).

**r3. Short-circuit.** "Like the 'OR' and 'AND' logical operators, the right-hand side expression is not evaluated if the left-hand side proves to be neither null nor undefined ."[^1] Example: [`short-circuit`](#short-circuit).

**r4. Chains.** CoalesceExpressionHead is a CoalesceExpression or a BitwiseORExpression, so `a ?? b ?? c` is `(a ?? b) ?? c`.[^2] The result is the first operand that is not nullish, or the last operand. Example: [`chain`](#chain).

**r5. No mix with `&&` and `||`.** "It is not possible to combine either the AND ( && ) or OR operators ( || ) directly with ?? . A syntax error will be thrown in such cases."[^1] The grammar does this: each operand of `??` is a BitwiseORExpression, which cannot hold a bare `&&` or `||`.[^2] Parentheses make the code valid: `(null || undefined) ?? "foo"` gives `"foo"`.[^1] Example: [`mixed`](#mixed).

**r6. Precedence.** `??` is "directly higher than the conditional (ternary) operator ".[^1] The MDN precedence table puts `??` and `||` on one level, 3, but the grammar does not let them mix (rule r5).[^3] Comparisons and arithmetic bind tighter, so `a ?? b + 1` is `a ?? (b + 1)`. Example: [`chain`](#chain).

## Examples

The examples run with Node.js 24.

Defaults that keep falsy values (rules r1 and r2). The MDN example, with `JSON.stringify` to show the empty string:[^1]

```javascript run id=defaults
const nullValue = null;
const emptyText = "";
const someNumber = 42;
console.log(JSON.stringify(nullValue ?? "default for A"));
console.log(JSON.stringify(emptyText ?? "default for B"));
console.log(someNumber ?? 0);

const text = "";
console.log(JSON.stringify(text || "hi!"), JSON.stringify(text ?? "hi!"));
console.log(NaN ?? 1, false ?? true);
```

```output for=defaults
"default for A"
""
42
"hi!" ""
NaN false
```

The right operand runs only for a nullish left operand (rule r3):

```javascript run id=short-circuit
function a() {
  console.log("a was called");
  return undefined;
}
function b() {
  console.log("b was called");
  return false;
}
function c() {
  console.log("c was called");
  return "foo";
}
console.log(a() ?? c());
console.log(b() ?? c());
```

```output for=short-circuit
a was called
c was called
foo
b was called
false
```

Chains, precedence and `??` with optional chaining (rules r4 and r6):

```javascript run id=chain
const options = { port: 0, saved: undefined };
const port = options.port ?? options.saved ?? 8080;
const host = options.host ?? options.saved ?? "localhost";
console.log(port, host);

const extra = null;
console.log(extra ?? 1 + 1, (extra ?? 1) + 1);

const foo = { someFooProp: "hi" };
console.log(foo.someFooProp?.toUpperCase() ?? "not available");
console.log(foo.someBarProp?.toUpperCase() ?? "not available");
```

```output for=chain
0 localhost
2 2
HI
not available
```

A bare `||` next to `??` is a syntax error (rule r5):

```javascript fails id=mixed expect=exception:SyntaxError
const value = null || undefined ?? "foo";
console.log(value);
```

## Pitfalls

### Mixing operators needs parentheses

`a || b ?? c` and `a && b ?? c` do not parse (rule r5). Write `(a || b) ?? c` or `a || (b ?? c)`, whichever you mean.[^1]

### `??` is not a test for falsy values

`NaN ?? 1` is `NaN`, and `false ?? true` is `false` ([`defaults`](#defaults)). To replace every falsy value, use `||`.

### Low precedence

`??` binds less tightly than arithmetic and comparisons, so `a ?? b + 1` is `a ?? (b + 1)` ([`chain`](#chain)). It binds more tightly than the conditional operator, so in `x ?? y ? 1 : 2`, the conditional takes `x ?? y` as its test.[^1]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| nullish default | `a ?? b`[^2] | `a if a is not None else b` |
| falsy default | `a \|\| b` | `a or b` |
| values that count as missing | `null` and `undefined`[^1] | `None` in the form above |
| evaluation of `a` | once[^2] | twice in the form above[^4] |

Python has no `??`. A conditional expression does the same job: "The expression x if C else y first evaluates the condition, C rather than x . If C is true, x is evaluated and its value is returned; otherwise, y is evaluated and its value is returned."[^4] With `C` as `a is not None` and `x` as `a`, the name `a` is evaluated twice. See [[python/conditional-expressions]].

## References
