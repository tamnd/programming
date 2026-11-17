---
tag: T2JQ9F
kind: feature
owner: javascript
slug: optional-chaining
name: Optional chaining
aliases: ["?.", optional chaining operator, "?.()", "?.[]", safe navigation]
area: languages/javascript/syntax
level: core
summary: >-
  a?.b, a?.[k] and f?.() give undefined when a or f is null or undefined, in place of a TypeError. The rest of the chain is then skipped. An optional chain cannot be assigned to.
grammar: "OptionalExpression : MemberExpression OptionalChain | CallExpression OptionalChain | OptionalExpression OptionalChain (ECMA-262, 13.3)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-optional-chains", locator: "13.3.9 Optional Chains"}
see_also: [javascript/nullish-coalescing-operator, javascript/logical-operators, javascript/expressions-and-operators, javascript/punctuators, python/dynamic-attribute-access, python/attribute-access-customization]
sources:
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-optional-chains
    title: ECMAScript 2026 Language Specification
    locator: 13.3.1.1 Static Semantics, Early Errors; 13.3.9 Optional Chains
    accessed: 2026-11-18
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Optional_chaining
    title: Optional chaining (?.), MDN
    locator: introduction; Description; Short-circuiting; Examples
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html#sec-punctuators
    title: ECMAScript 2026 Language Specification
    locator: 12.8 Punctuators
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.3.1 Attribute references
    accessed: 2026-11-18
---

## Synopsis

| Form | Value when the base is `null` or `undefined` | Otherwise |
|---|---|---|
| `obj?.prop` | `undefined` | `obj.prop` |
| `obj?.[expr]` | `undefined` | `obj[expr]` |
| `fn?.(args)` | `undefined` | `fn(args)` |

```javascript run id=synopsis
const adventurer = { name: "Alice", cat: { name: "Dinah" } };
console.log(adventurer.cat?.name, adventurer.dog?.name, adventurer.someMethod?.());
```

```output for=synopsis
Dinah undefined undefined
```

## Semantics

**r1. The test.** "An optional chain is a chain of one or more property accesses and function calls, the first of which begins with the token ?. ."[^1] The steps evaluate the base, and "If baseValue is either undefined or null , then Return undefined ." Else the chain runs as a normal access or call.[^1] Example: [`synopsis`](#synopsis).

**r2. Only nullish values.** The test is not a falsy test. MDN compares the older form `obj.first && obj.first.second`: if `obj.first` is a falsy value "that's not null or undefined , such as 0 , it would still short-circuit and make nestedProp become 0".[^2] With `?.`, a base of 0 or the empty string goes on to the property access. Example: [`only-nullish`](#only-nullish).

**r3. Calls.** `f?.()` returns `undefined` when `f` is nullish. But "if there is a property with such a name which is not a function, using ?. will still raise a TypeError exception".[^2] In `obj.method?.()`, only `obj.method` is tested, so a nullish `obj` still throws. Write `obj?.method?.()` to test both.[^2] Example: [`calls`](#calls).

**r4. Short-circuit.** When the base is nullish, the rest of the chain is not evaluated. MDN: "if the left operand is null or undefined , the expression will not be evaluated." In `potentiallyNullObj?.a.b`, the `.b` access does not run either.[^2] Example: [`short-circuit`](#short-circuit).

**r5. Parentheses end the chain.** The short-circuit "only happens along one continuous" chain of accesses. "If you group one part of the chain, then subsequent property accesses will still be evaluated."[^2] So `(a?.b).c` throws when `a` is nullish. Example: [`grouped`](#grouped).

**r6. Not allowed.** "It is invalid to try to assign to the result of an optional chaining expression".[^2] A template tag cannot be an optional chain, and the constructor of `new` cannot be one.[^2] An undeclared root still throws: "Optional chaining cannot be used on a non-declared root object, but can be used with a root object with value undefined ."[^2] Example: [`assign`](#assign).

**r7. The token.** The punctuator is `?.` with the rule `[lookahead ∉ DecimalDigit ]`.[^3] So `a?.5:1` is a conditional with the number `.5`, not an optional chain.

## Examples

The examples run with Node.js 24.

A base of 0 or the empty string is not nullish (rule r2). `&&` stops at 0, but `?.` reads the property:

```javascript run id=only-nullish
const obj = { first: 0 };
console.log(obj.first && obj.first.toFixed(1));
console.log(obj.first?.toFixed(1));
console.log("".length, ""?.length, null?.length);
const flag = true;
console.log(flag?.5:1);
```

```output for=only-nullish
0
0.0
0 0 undefined
0.5
```

Optional calls (rule r3). A property that is not a function still throws:

```javascript run id=calls
function doSomething(onError) {
  onError?.("disk full");
  return "done";
}
console.log(doSomething());
console.log(doSomething((message) => console.log("error:", message)));

const api = { version: 2 };
try {
  api.version?.();
} catch (err) {
  console.log(err.name);
}
const missing = null;
console.log(missing?.method?.());
```

```output for=calls
done
error: disk full
done
TypeError
undefined
```

The rest of the chain does not run (rule r4). `x` stays 0, and `.a.b` gives no error:

```javascript run id=short-circuit
const potentiallyNullObj = null;
let x = 0;
const prop = potentiallyNullObj?.[x++];
console.log(prop, x);
console.log(potentiallyNullObj?.a.b);

const customer = { name: "Carl", details: { age: 82 } };
console.log(customer.details?.address?.city ?? "unknown city");
console.log(customer.contact?.email.toLowerCase());
```

```output for=short-circuit
undefined 0
undefined
unknown city
undefined
```

Parentheses end the chain (rule r5):

```javascript fails id=grouped expect=exception:TypeError
const potentiallyNullObj = null;
console.log((potentiallyNullObj?.a).b);
```

Node.js reports `TypeError: Cannot read properties of undefined (reading 'b')`.

An optional chain is not a target (rule r6):

```javascript fails id=assign expect=exception:SyntaxError
const object = {};
object?.property = 1;
```

## Pitfalls

### `?.` hides mistakes

`user?.name` gives `undefined` when `user` is `null` by mistake. The error then shows up later, far from its cause. Use `?.` only where a nullish value is valid, and let other places throw.

### It tests one level

`a?.b.c` tests `a` only. If `a.b` can be nullish too, write `a?.b?.c` ([`short-circuit`](#short-circuit)).[^2]

### The result is `undefined`, not `null`

`null?.x` is `undefined` (rule r1). Code that tests `=== null` misses it. Combine with `??` to choose the value: `obj?.x ?? fallback`.

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| optional access | `a?.b`, `a?.[k]`, `f?.()`[^1] | no operator |
| missing attribute | `undefined` for `a.b` | AttributeError[^4] |
| access on null or None | TypeError for `a.b`, `undefined` for `a?.b` | AttributeError for `a.b` |

In Python, `__getattribute__()` "either returns a value or raises AttributeError if the attribute is not available."[^4] There is no `?.` in the reference, so a test such as `a.b if a is not None else None` does the job. See [[python/dynamic-attribute-access]] and [[python/attribute-access-customization]].

## References
