---
tag: TR51K7
kind: feature
owner: javascript
slug: unary-operators
name: Unary operators
aliases: [typeof, void, delete, unary plus, unary minus, unary negation, "+x", "-x", "~", "!"]
area: languages/javascript/syntax
level: core
summary: >-
  The unary operators take one operand: delete removes a property, void gives undefined, typeof names the type, + and - convert to a number, ~ inverts bits and ! negates truth.
grammar: "UnaryExpression : UpdateExpression | delete UnaryExpression | void UnaryExpression | typeof UnaryExpression | + UnaryExpression | - UnaryExpression | ~ UnaryExpression | ! UnaryExpression | AwaitExpression (ECMA-262, 13.5)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-unary-operators", locator: "13.5 Unary Operators"}
see_also: [javascript/expressions-and-operators, javascript/logical-operators, javascript/arithmetic-operators, javascript/bitwise-operators, javascript/exponentiation-operator, javascript/update-expressions, python/arithmetic-operators]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-unary-operators
    title: ECMAScript 2026 Language Specification
    locator: 13.5 Unary Operators; 13.5.1 to 13.5.7
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/typeof
    title: typeof, MDN
    locator: introduction; Description; typeof null; Interaction with undeclared and uninitialized variables
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/delete
    title: delete, MDN
    locator: introduction; Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/void
    title: void operator, MDN
    locator: introduction; Description
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Unary_plus
    title: Unary plus (+), MDN
    locator: introduction; Description; Examples
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Bitwise_NOT
    title: Bitwise NOT (~), MDN
    locator: Description
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.6 Unary arithmetic and bitwise operations; 6.11 Boolean operations
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/simple_stmts.html
    title: Simple statements, The Python Language Reference
    locator: 7.5 The del statement
    accessed: 2026-11-18
---

## Synopsis

| Operator | Result |
|---|---|
| `delete obj.p` | removes the property, gives `true` or `false` |
| `void x` | `undefined` |
| `typeof x` | a string that names the type |
| `+x` | `x` converted to a number |
| `-x` | `x` converted to a numeric value and negated |
| `~x` | the bitwise NOT of `x` |
| `!x` | the opposite boolean of `x` |

```javascript run id=synopsis
const obj = { a: 1, b: 2 };
console.log(delete obj.a, obj, void 0, typeof 42, +"7", -"7", ~5, !0);
```

```output for=synopsis
true { b: 2 } undefined number 7 -7 -6 true
```

## Semantics

**r1. Grammar.** Each unary operator takes a UnaryExpression as its operand, so they nest from the right: `!typeof x` is `!(typeof x)`.[^1] They bind tighter than the binary operators. But a unary expression cannot be the left operand of `**`: in ExponentiationExpression, the left side of `**` is an UpdateExpression.[^1] Example: [`precedence`](#precedence).

**r2. `typeof`.** "The typeof operator returns a string indicating the type of the operand's value."[^2] The steps return one of `'undefined'`, `'object'`, `'string'`, `'symbol'`, `'boolean'`, `'number'`, `'bigint'` and `'function'`. For `null`, the result is `'object'`.[^1] MDN says this "stands since the beginning of JavaScript".[^2] An object that "has a [[Call]] internal method" is a function, and for it the result is `'function'`.[^1] Example: [`typeof`](#typeof).

**r3. `typeof` on an unknown name.** `typeof` "works with undeclared identifiers" and gives `'undefined'` "instead of throwing an error." But a `let`, `const` or `class` name used before its declaration "will throw a ReferenceError".[^2] Example: [`typeof`](#typeof), [`tdz`](#tdz).

**r4. `delete`.** "The delete operator removes a property from an object."[^3] It gives `true` on success and also when the property does not exist. It has an effect on own properties only, and "Non-configurable properties cannot be removed."[^3] In strict mode, `delete` on a plain identifier "is a Syntax Error".[^1] `delete` does not free memory directly: "Memory management is done indirectly via breaking references."[^3] Example: [`delete`](#delete).

**r5. `void`.** "The void operator evaluates the given expression and then returns undefined ."[^4] The operand still runs: "GetValue must be called even though its value is not used because it may have observable side-effects."[^1] Example: [`precedence`](#precedence).

**r6. `+` and `-`.** Unary `+` returns "ToNumber (? GetValue ( expr ))".[^1] It "can convert string representations of integers and floats, as well as the non-string values true , false , and null ." "If it cannot parse a particular value, it will evaluate to NaN ."[^5] On a BigInt, unary `+` throws a TypeError.[^5] Unary `-` uses ToNumeric, so it works on BigInt values, and "Negating +0 𝔽 produces -0 𝔽".[^1] Example: [`numbers`](#numbers).

**r7. `~` and `!`.** `~` uses ToNumeric and then a bitwise NOT for a Number or a BigInt.[^1] For a Number, "it converts the operand to a 32-bit integer and performs number bitwise NOT." So "Bitwise NOTing any 32-bit integer x yields -(x + 1) ."[^6] `!` converts with ToBoolean and returns the opposite.[^1] See [[javascript/bitwise-operators]] and [[javascript/logical-operators]].

## Examples

The examples run with Node.js 24.

The results of `typeof` (rules r2 and r3):

```javascript run id=typeof
console.log(typeof undefined, typeof null, typeof true, typeof 1, typeof 1n);
console.log(typeof "", typeof Symbol(), typeof {}, typeof [], typeof function () {}, typeof class {});
console.log(typeof undeclaredVariable, typeof typeof 1);
```

```output for=typeof
undefined object boolean number bigint
string symbol object object function function
undefined string
```

`typeof` on a `let` name before its declaration throws (rule r3):

```javascript fails id=tdz expect=exception:ReferenceError
console.log(typeof later);
let later = 1;
```

`delete` on own, missing, inherited and non-configurable properties (rule r4). The code is not in strict mode, so a failed `delete` gives `false`:

```javascript run id=delete
const parent = { shared: "parent" };
const child = Object.create(parent);
child.shared = "child";
console.log(delete child.shared, child.shared);
console.log(delete child.missing);

const fixed = {};
Object.defineProperty(fixed, "id", { value: 1, configurable: false });
console.log(delete fixed.id, fixed.id);

const list = [1, 2, 3];
console.log(delete list[1], list, list.length);
```

```output for=delete
true parent
true
false 1
true [ 1, <1 empty item>, 3 ] 3
```

Deleting an array item leaves a hole and keeps the length. Use `splice` to remove an item.

Conversions with `+` and `-` (rule r6):

```javascript run id=numbers
console.log(+"3.5", +"0x1F", +"", +" 12 ", +"12px", +true, +null, +undefined, +[]);
console.log(-"5", -true, Object.is(-0, -(+0)), -(2n));
try {
  +1n;
} catch (err) {
  console.log(err.name);
}
```

```output for=numbers
3.5 31 0 12 NaN 1 0 NaN 0
-5 -1 true -2n
TypeError
```

Grouping of unary operators (rules r1 and r5). MDN shows the `void` case:[^4]

```javascript run id=precedence
const someData = 99;
console.log(typeof someData + " foo", typeof (someData + " foo"));
console.log(void 2 === "2", void (2 === "2"));
console.log(!typeof someData, -~5, - -5);
let count = 0;
void count++;
console.log(count);
```

```output for=precedence
number foo string
false undefined
false 6 5
1
```

## Pitfalls

### `typeof null` is `'object'`

To test for `null`, use `value === null`. To test for an object, use `value !== null && typeof value === 'object'` (rule r2).[^2]

### `typeof` does not tell arrays from objects

`typeof []` is `'object'`. MDN says to "use Array.isArray or Object.prototype.toString.call" for this.[^2]

### Unary `+` and BigInt

`+x` throws for a BigInt `x`, but `-x` works (rule r6).[^5] Use `Number(x)` to convert a BigInt to a number.

### `delete` on an array

`delete list[i]` leaves a hole ([`delete`](#delete)). The length stays the same.

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| negation and plus | `-x`, `+x`; `+` converts strings[^1] | `-x`, `+x`; TypeError "if the argument does not have the proper type"[^7] |
| bitwise NOT | `~x`[^1] | `~x`, defined as "-(x+1)"[^7] |
| logical NOT | `!x`[^1] | `not x`, which "yields True if its argument is false, False otherwise"[^7] |
| remove a name or item | `delete obj.p` for properties only[^3] | `del` statement for names, items and attributes[^8] |

In Python, "All unary arithmetic and bitwise operations have the same priority".[^7] Python's `del` removes names too: "Deletion of a name removes the binding of that name from the local or global namespace".[^8] JavaScript's `delete` cannot remove a variable.[^3] See [[python/arithmetic-operators]].

## References
