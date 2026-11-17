---
tag: 8N689X
kind: feature
owner: javascript
slug: property-accessors
name: Property accessors
aliases: [dot notation, bracket notation, member access, computed member access, "obj.prop", "obj[key]"]
area: languages/javascript/syntax
level: intro
summary: >-
  obj.name and obj[expr] read or write a property. The dot form needs an identifier name. The bracket form takes any expression, and converts its value to a string or a Symbol key.
grammar: "MemberExpression : MemberExpression [ Expression ] | MemberExpression . IdentifierName | MemberExpression . PrivateIdentifier (ECMA-262, 13.3)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-property-accessors", locator: "13.3.2 Property Accessors"}
see_also: [javascript/optional-chaining, javascript/identifiers-and-reserved-words, javascript/unary-operators, javascript/expressions-and-operators, python/dynamic-attribute-access]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Property_accessors
    title: Property accessors, MDN
    locator: introduction; Dot notation; Bracket notation; Property names; Method binding
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-property-accessors
    title: ECMAScript 2026 Language Specification
    locator: 13.3.2 Property Accessors; 13.3.3 EvaluatePropertyAccessWithExpressionKey
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.3.1 Attribute references; 6.3.2 Subscriptions and slicings
    accessed: 2026-11-18
---

## Synopsis

```javascript run id=synopsis
const person = {};
person["firstName"] = "Mario";
person.lastName = "Rossi";
const key = "firstName";
console.log(person.firstName, person["lastName"], person[key], person.age);
```

```output for=synopsis
Mario Rossi Mario undefined
```

"Property accessors provide access to an object's properties by using the dot notation or the bracket notation."[^1] Both forms can read a value and can be the target of an assignment.

## Semantics

**r1. Dot notation.** In `object.propertyName`, "the propertyName must be a valid JavaScript identifier which can also be a reserved word ." So `object.$1` is valid and `object.1` is not.[^1] Example: [`dot`](#dot).

**r2. Dot is a short form of brackets.** ECMA-262 says `MemberExpression . IdentifierName` "is identical in its behaviour to" a bracket access with the name as a string.[^2] So `a.b` and `a['b']` are the same access.

**r3. Bracket notation.** In `object[expression]`, "the expression should evaluate to a string or Symbol that represents the property's name." Any string works, "including '1foo' , '!bar!' , or even ' ' (a space)."[^1] Example: [`bracket`](#bracket).

**r4. Keys become strings.** "Each property name is a string or a Symbol . Any other value, including a number, is coerced to a string."[^1] So `object[1]` and `object['1']` are one property. Two different plain objects both become `'[object Object]'`, so they name the same key.[^1] Example: [`keys`](#keys).

**r5. Order in an assignment.** In `a[b] = c`, the key conversion "will not be performed until after evaluation of c ".[^2] The base `a` and the key expression `b` are evaluated first, then `c`, then the key conversion. Example: [`order`](#order).

**r6. Private names.** The grammar has `MemberExpression . PrivateIdentifier`, so `obj.#x` uses the dot form. It has no bracket form for a private name.[^2]

**r7. Methods are not bound.** "A method is not bound to the object that it is a property of." `this` is set by the call, so `obj.method()` passes `obj`, but a method stored in a variable and called alone does not.[^1] Example: [`binding`](#binding).

## Examples

The examples run with Node.js 24.

Dot notation with identifier names, reserved words and numbers (rule r1). A dot after an integer literal is read as a decimal point, so the number needs a space, parentheses or a second dot:[^1]

```javascript run id=dot
const object = { class: "reserved", $1: "dollar" };
console.log(object.class, object.$1);
console.log(77 .toExponential(), (77).toExponential(), 77..toExponential());
```

```output for=dot
reserved dollar
7.7e+1 7.7e+1 7.7e+1
```

Bracket notation with any string, a variable and a function call (rule r3):

```javascript run id=bracket
const obj = { name: "Michel", "1foo": 1, " ": "space", "my-key": 2 };
const getKey = () => "name";
console.log(obj["name"], obj[getKey()], obj["1foo"], obj[" "], obj["my-key"]);
const sym = Symbol("id");
obj[sym] = 42;
console.log(obj[sym], Object.keys(obj).length);
```

```output for=bracket
Michel Michel 1 space 2
42 4
```

Symbol keys do not show in `Object.keys`.

Key conversion (rule r4). The two objects make the same key:

```javascript run id=keys
const object = {};
object["1"] = "value";
console.log(object[1]);

const foo = { uniqueProp: 1 };
const bar = { uniqueProp: 2 };
const table = {};
table[foo] = "first";
table[bar] = "second";
console.log(table[foo], Object.keys(table));

const list = ["a", "b"];
console.log(list["1"], list[1.0], list["01"]);
```

```output for=keys
value
second [ '[object Object]' ]
b b undefined
```

`list['01']` is undefined, because the key `'01'` is a different string from `'1'`.

The order of evaluation in `a[b] = c` (rule r5):

```javascript run id=order
const log = [];
const target = {};
const key = { toString() { log.push("key converted"); return "k"; } };
function base() { log.push("base"); return target; }
function value() { log.push("value"); return 1; }
base()[(log.push("key expression"), key)] = value();
console.log(log.join(", "), target.k);
```

```output for=order
base, key expression, value, key converted 1
```

Methods get `this` from the call (rule r7):

```javascript run id=binding
const counter = {
  count: 5,
  read() {
    return this === counter ? this.count : "not counter";
  },
};
const read = counter.read;
console.log(counter.read(), counter["read"](), read());
```

```output for=binding
5 5 not counter
```

Access on `null` or `undefined` throws:

```javascript fails id=null-base expect=exception:TypeError
const missing = undefined;
console.log(missing.name);
```

Node.js reports `TypeError: Cannot read properties of undefined (reading 'name')`. Use `missing?.name` to get `undefined` in place of the error. See [[javascript/optional-chaining]].

## Pitfalls

### Keys from outside input

MDN says to "beware of using square brackets to access properties whose names are given by external input."[^1] A key such as `'constructor'` finds an inherited property, not data that the program stored:

```javascript run id=outside
const prices = { apple: 3 };
const input = "constructor";
console.log(typeof prices[input], prices[input] === Object);
console.log(Object.hasOwn(prices, input), Object.hasOwn(prices, "apple"));
```

```output for=outside
function true
false true
```

### Objects as keys

`table[foo]` with an object key uses the string `'[object Object]'`, so all plain objects share one key. The second assignment replaces the first ([`keys`](#keys)).

### `eval` in place of brackets

MDN shows code that builds a property path as a string for `eval()`, and says "It is better to use bracket notation instead".[^1]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| attribute or property | `obj.name`[^2] | `obj.name`, "a primary followed by a period and a name"[^3] |
| key in brackets | `obj[expr]`, the same property as the dot form[^2] | `obj[key]`, a subscription, a different operation[^3] |
| key types | strings and Symbols; others become strings[^1] | a key for mappings, an index for sequences[^3] |
| missing name | `undefined` ([`synopsis`](#synopsis)) | `__getattribute__()` raises AttributeError[^3] |

In Python, `obj.name` and `obj['name']` are separate. A subscription is "usually used for selecting an element from a container".[^3] In JavaScript, both forms reach the same property (rule r2). See [[python/dynamic-attribute-access]].

## References
