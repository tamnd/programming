---
tag: 82F9BM
kind: feature
owner: javascript
slug: bigints
name: BigInts
aliases: [BigInt, bigint primitive, arbitrary-precision integers in JavaScript, the n suffix, BigInt.asIntN, BigInt.asUintN]
area: languages/javascript/strings-and-text
level: core
summary: >-
  A BigInt is a primitive integer of any size, written with an n suffix or made with BigInt(). Its arithmetic is exact, division truncates, and it cannot be mixed with numbers in arithmetic.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-language-types-bigint-type", locator: "6.1.6.2 The BigInt Type"}
see_also: [javascript/strings, javascript/regular-expressions, javascript/javascript-typed-arrays]
sources:
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 6
  locator: 6.1.6.2 The BigInt Type
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Numbers_and_strings
  title: MDN Web Docs, JavaScript Guide, Numbers and strings
  locator: BigInts
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/BigInt
  title: MDN Web Docs, JavaScript Reference, BigInt
  locator: Description; Operators; Comparisons; Conditionals; Cryptography; Use within JSON; BigInt coercion
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/BigInt/BigInt
  title: MDN Web Docs, JavaScript Reference, BigInt() constructor
  locator: Syntax; Return value; Exceptions
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/BigInt/asIntN
  title: MDN Web Docs, JavaScript Reference, BigInt.asIntN()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: BigInt, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/stdtypes.html
  title: The Python Standard Library, Python 3.14, Built-in Types
  locator: Numeric Types
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/math/BigInteger.html
  title: Java SE 25 API, Class BigInteger
  locator: the class description
  accessed: 2026-11-18
- url: https://pkg.go.dev/math/big
  title: Go standard library, package math/big
  locator: Overview
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const big = 2n ** 64n;
console.log(big, typeof big);
console.log(2 ** 64 === 2 ** 64 + 1, big === big + 1n);
console.log(7n / 2n, -7n / 2n, 7n % 2n);
console.log(BigInt("0xff"), BigInt(42), String(big));
```

```output for=basics
18446744073709551616n bigint
true false
3n -3n 1n
255n 42n 18446744073709551616
```

| Form | Value |
|---|---|
| `123n`, `0xffn` | a BigInt literal |
| `BigInt(42)`, `BigInt("42")` | a BigInt from an integer number or a string |
| `typeof 1n` | `"bigint"` |
| `1n + 1` | a TypeError |
| `5n / 2n` | `2n` |

## Semantics

**r1.** "The BigInt type represents an integer value. The value may be any size and is not limited to a particular bit-width."[^1] "Conceptually, a BigInt is just an arbitrarily long sequence of bits which encodes an integer. You can safely do any arithmetic operations without losing precision or over-/underflowing."[^2] Example: [`basics`](#basics).

**r2.** "A BigInt value, also sometimes just called a BigInt, is a bigint primitive, created by appending n to the end of an integer literal, or by calling the BigInt() function (without the new operator) and giving it an integer value or string value."[^3] "Attempting to construct it with new throws a TypeError ."[^4] `BigInt` throws a RangeError "if the parameter is a non-integral number" and a SyntaxError "if the parameter is a string that cannot be parsed as a BigInt ."[^4] Strings "can have leading and trailing whitespaces and can be prefixed with 0b , 0o , or 0x ."[^4] Example: [`convert`](#convert).

**r3.** "A BigInt value cannot be used with methods in the built-in Math object and cannot be mixed with a Number value in operations; they must be coerced to the same type."[^3] The comparison operators may mix them, and "Addition (+ ) involving a string and a BigInt returns a string."[^3] "A BigInt value is not strictly equal to a Number value, but it is loosely so".[^3] Example: [`mixing`](#mixing).

**r4.** "Division (/ ) truncates fractional components towards zero, since BigInt is unable to represent fractional quantities."[^3] Unary plus and `>>>` do not work on BigInts; `>>>` "is the only bitwise operator that's unsupported, as every BigInt value is signed."[^3]

**r5.** "A BigInt value follows the same conversion rules as Numbers when: it is converted to a Boolean".[^3] "Namely, only 0n is falsy; everything else is truthy."[^3]

**r6.** `BigInt.asIntN` "truncates a BigInt value to the given number of least significant bits and returns that value as a signed integer."[^5] `BigInt.asUintN` does the same as an unsigned integer.[^3] They give the wrap-around of fixed-width integer types. Example: [`fixed`](#fixed).

**r7.** "Using JSON.stringify() with any BigInt value will raise a TypeError , as BigInt values aren't serialized in JSON by default."[^3] A replacer function can turn them into strings first.[^3] Example: [`json`](#json).

**r8.** "The operations supported on BigInt values are not constant-time and are thus open to timing attacks. JavaScript BigInts therefore could be dangerous for use in cryptography without mitigating factors."[^3]

## Examples

Conversions that work and conversions that throw (rule r2).

```javascript run id=convert
for (const v of [1.5, "1.5", "12abc"]) {
  try {
    console.log(BigInt(v));
  } catch (e) {
    console.log(e.name);
  }
}
try {
  new BigInt(1);
} catch (e) {
  console.log(e.name);
}
console.log(BigInt(" 0b101 "), BigInt(true));
```

```output for=convert
RangeError
SyntaxError
SyntaxError
TypeError
5n 1n
```

Mixing BigInts and numbers (rules r3 and r5).

```javascript run id=mixing
try {
  1n + 1;
} catch (e) {
  console.log(e.name);
}
console.log(1n + BigInt(1), Number(1n) + 1);
console.log(1n == 1, 1n === 1, 2n > 1, 0n ? "yes" : "no");
console.log("n = " + 5n);
try {
  Math.max(1n, 2n);
} catch (e) {
  console.log(e.name);
}
```

```output for=mixing
TypeError
2n 2
true false true no
n = 5
TypeError
```

Wrapping to a fixed number of bits (rule r6).

```javascript run id=fixed
const max = 2n ** 63n - 1n;
console.log(BigInt.asIntN(64, max), BigInt.asIntN(64, max + 1n));
console.log(BigInt.asUintN(8, 257n), BigInt.asIntN(4, 25n));
```

```output for=fixed
9223372036854775807n -9223372036854775808n
1n -7n
```

## Versions

The TC39 list of finished proposals gives BigInt the expected publication year 2020, so it is part of ECMAScript 2020.[^6] Node v24.10.0 runs every example on this page.

## Pitfalls

### Making a BigInt from a number that has already lost digits

```javascript wrong id=precision
const id = 9007199254740993;
console.log(id, BigInt(id));
console.log(BigInt("9007199254740993"), 9007199254740993n);
```

```output for=precision
9007199254740992 9007199254740992n
9007199254740993n 9007199254740993n
```

The number literal is rounded before `BigInt` sees it. MDN: "Using a string prevents loss of precision, since long number literals don't represent what they seem like."[^2] Use a string or an `n` literal.

### Sending a BigInt through JSON

```javascript wrong id=json
const data = { id: 12345678901234567890n };
try {
  JSON.stringify(data);
} catch (e) {
  console.log(e.name);
}
const replacer = (key, value) => (typeof value === "bigint" ? value.toString() : value);
console.log(JSON.stringify(data, replacer));
```

```output for=json
TypeError
{"id":"12345678901234567890"}
```

The reading side must turn the string back with `BigInt` (rule r7).

## Comparison

In Python the one integer type already has no size limit: "Integers have unlimited precision."[^7] Java has the class `BigInteger`, "Immutable arbitrary-precision integers."[^8] Go has `math/big`: "Package big implements arbitrary-precision arithmetic (big numbers)."[^9] A JavaScript BigInt is a primitive type next to Number, with its own literals and the usual operators (rules r1 and r2), and it does not mix with Number in arithmetic (rule r3).

## References
