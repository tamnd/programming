---
tag: QJYYVN
kind: task
slug: do-integer-arithmetic
name: Do integer arithmetic
aliases: [integer operators, arithmetic operators, integer division, remainder operator, modulo operator, power operator, exponentiation, divmod]
area: foundations/numbers-in-machines
level: intro
summary: >-
  Add, subtract, multiply, divide, take the remainder and raise to a power with integers. The division, the remainder and the power differ most between languages.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/stdtypes.html
    title: Built-in Types, Python 3 documentation
    locator: Numeric Types, int, float, complex; Bitwise Operations on Integer Types
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Remainder
    title: Remainder (%), MDN Web Docs
    locator: the introduction
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Exponentiation
    title: Exponentiation (**), MDN Web Docs
    locator: the introduction
    accessed: 2026-11-16
  - url: https://docs.oracle.com/javase/specs/jls/se27/html/jls-15.html
    title: "The Java Language Specification, Java SE 27 Edition, Chapter 15: Expressions"
    locator: 15.17.2 Division Operator /
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/lang/Math.html
    title: Math, Java SE 27 API
    locator: pow(double, double)
    accessed: 2026-11-16
  - url: https://en.cppreference.com/c/language/operator_arithmetic
    title: Arithmetic operators, C reference, cppreference.com
    locator: Multiplicative operators
    accessed: 2026-11-16
  - url: https://en.cppreference.com/c/numeric/math/pow
    title: pow, powf, powl, C reference, cppreference.com
    locator: the description
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/arithmetic-operators
    title: Arithmetic operators, C# reference, Microsoft Learn
    locator: Division operator /, Integer division
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.math.pow
    title: Math.Pow(Double, Double) Method, .NET API reference, Microsoft Learn
    locator: the definition
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Arithmetic operators; Integer operators
    accessed: 2026-11-16
  - url: https://pkg.go.dev/math
    title: math, Go standard library
    locator: func Pow
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/reference/expressions/operator-expr.html
    title: Operator expressions, The Rust Reference
    locator: Arithmetic and Logical Binary Operators
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/primitive.i64.html
    title: i64, Rust standard library
    locator: pow
    accessed: 2026-11-16
  - url: https://tiswww.case.edu/php/chet/bash/bashref.html
    title: Bash Reference Manual, version 5.3
    locator: 6.5 Shell Arithmetic
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/isSafeInteger
    title: Number.isSafeInteger(), MDN Web Docs
    locator: Description
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Bitwise_XOR
    title: Bitwise XOR (^), MDN Web Docs
    locator: the introduction
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/BigInt
    title: BigInt, MDN Web Docs
    locator: Operators
    accessed: 2026-11-16
---

## Task

Apply the six integer operations to two integers `a` and `b` and a small exponent `n`: the sum, the difference, the product, the quotient, the remainder and the power.

The function `arith(a, b, n)` returns a list of six values: `a + b`, `a - b`, `a * b`, the quotient `q` of `a` divided by `b` rounded toward zero, the remainder `a - b * q`, and `a` to the power `n`. For example, `arith(7, 2, 3)` is `[9, 5, 14, 3, 1, 343]`, and `arith(-7, 2, 3)` is `[-5, -9, -14, -3, -1, -343]`.

The sum, the difference and the product are the same in each language, while the results fit in the type. Three operations differ:

- **Division.** Most languages round the integer quotient toward zero, so -7 divided by 2 is -3. Python rounds toward minus infinity, so `-7 // 2` is -4. JavaScript has no integer division: `7 / 2` is 3.5.
- **Remainder.** The remainder goes with the division. With the quotient rounded toward zero, the remainder has the sign of the dividend `a`. With the floor quotient of Python, it has the sign of the divisor `b`. See [[integer-division-and-modulo]].
- **Power.** Python, JavaScript and Bash have the operator `**`. Java, C, C++, C# and Go have no power operator for integers, and their power function works on floats. In several of them, `^` is the bitwise exclusive or, not a power.

The contract keeps `a` and `b` from -100000 to 100000 and `n` from 0 to 3, so each result is exact in a 64-bit integer and in a JavaScript number. For larger values, see [[detect-integer-overflow]] and [[use-big-integers]]. The divisor is never 0; see [[detect-division-by-zero]].

## Solutions

| Language | Quotient | Remainder | Power |
|---|---|---|---|
| Python | `//` rounds down; the solution divides the magnitudes | `a - b * q` | `a ** n` |
| JavaScript, TypeScript | `Math.trunc(a / b)` | `a % b` | `a ** n` |
| Java | `a / b` | `a % b` | a loop; `Math.pow` takes `double` values |
| C, C++ | `a / b` | `a % b` | a loop; `pow` takes `double` values |
| C# | `a / b` | `a % b` | a loop; `Math.Pow` takes `double` values |
| Go | `a / b` | `a % b` | a loop; `math.Pow` takes `float64` values |
| Rust | `a / b` | `a % b` | `a.pow(n)` |
| Bash | `$((a / b))` | `$((a % b))` | `$((a ** n))` |

- **Python.** `x // y` is the "floored quotient of x and y"[^1]: "The result is always rounded towards minus infinity"[^1]. The contract asks for the quotient rounded toward zero, so the solution divides the absolute values and sets the sign after. `x ** y` gives "x to the power y"[^1], and the result is an exact `int`.
- **JavaScript.** `a / b` divides two numbers and gives a float, so the solution rounds it with `Math.trunc`. The remainder operator `%` "always takes the sign of the dividend"[^2]. The operator `**` "returns the result of raising the first operand to the power of the second operand"[^3].
- **TypeScript.** The same code as JavaScript, with the type `number`.
- **Java.** "Integer division rounds toward 0"[^4]. Java has no power operator. `Math.pow` "Returns the value of the first argument raised to the power of the second argument"[^5], but its arguments and its result are `double` values. The solution multiplies in a loop.
- **C.** For integer operands, the quotient is "truncated towards zero"[^6]. `pow` "Computes the value of base raised to the power exponent"[^7] for `double` values, so the solution uses a loop.
- **C++.** The same operators as C, in a `constexpr` function that returns a `std::array`.
- **C#.** The quotient of two integers "equals the quotient of the two operands rounded toward zero"[^8]. `Math.Pow` "Returns a specified number raised to the specified power"[^9], as a `double`, so the solution uses a loop.
- **Go.** The quotient is "truncated towards zero"[^10]. `math.Pow` "returns x**y, the base-x exponential of y"[^11] for `float64` values, so the solution uses a loop.
- **Rust.** "Integer division rounds towards zero"[^12], and "the remainder will have the same sign as the dividend"[^12]. `i64::pow` "Raises self to the power of exp"[^13] with an exact integer result.
- **Bash.** Arithmetic expansion has `**` for "exponentiation"[^14], and "The operators and their precedence, associativity, and values are the same as in the C language"[^14].

The Python operators, and the truncated division of the contract:

```python run id=python-operators
a, b = -7, 2
print(a + b, a - b, a * b, a // b, a % b, a**3)
print(divmod(a, b))
q = abs(a) // abs(b) * (1 if (a < 0) == (b < 0) else -1)
print(q, a - b * q)
```

```output for=python-operators
-5 -9 -14 -4 1 -343
(-4, 1)
-3 -1
```

The same values in JavaScript, where `/` gives a float:

```javascript run id=javascript-operators
const a = -7;
const b = 2;
console.log(a + b, a - b, a * b, a / b, Math.trunc(a / b), a % b, a ** 3);
```

```output for=javascript-operators
-5 -9 -14 -3.5 -3 -1 -343
```

In C, `/` and `%` round toward zero for each sign:

```c run id=c-operators
#include <stdio.h>

int main(void) {
    int pairs[4][2] = {{7, 2}, {-7, 2}, {7, -2}, {-7, -2}};
    for (int i = 0; i < 4; i++) {
        int a = pairs[i][0], b = pairs[i][1];
        printf("%2d / %2d = %2d, %2d %% %2d = %2d\n", a, b, a / b, a, b, a % b);
    }
}
```

```output for=c-operators
 7 /  2 =  3,  7 %  2 =  1
-7 /  2 = -3, -7 %  2 = -1
 7 / -2 = -3,  7 % -2 =  1
-7 / -2 =  3, -7 % -2 = -1
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `small`, `exact-division`: positive values, with and without a remainder.
- `negative-dividend`, `negative-divisor`, `both-negative`: the quotient rounds toward zero, and the remainder has the sign of `a`.
- `zero-dividend`: 0 divided by a number is 0, and `0 ** 0` is 1.
- `zero-power`: each number to the power 0 is 1.
- `divisor-larger`: when `|b| > |a|`, the quotient is 0 and the remainder is `a`.
- `largest`, `smallest`: the largest product is 10 to the power 10, and the largest power is 10 to the power 15. Both are safe integers in JavaScript[^15].

## Pitfalls

- **`^` is not a power.** In Python, JavaScript, Go and Bash, `^` is the bitwise exclusive or. Python lists `x ^ y` as the "bitwise exclusive or of x and y"[^1], JavaScript has "The bitwise XOR"[^16] operator, Go lists `^` as "bitwise XOR"[^10], and Bash lists `^` as "bitwise exclusive OR"[^14]. So `2 ^ 3` is 1, not 8:

```python wrong id=python-caret
print(2 ^ 3, 2**3)
print(10 ^ 2, 10**2)
```

```output for=python-caret
1 8
8 100
```

- **A float power is not exact for large results.** A `double` holds every integer from -(2 to the power 53 - 1) to 2 to the power 53 - 1 exactly, but not every integer outside this range. These are the safe integers of JavaScript, and each one "can be exactly represented as an IEEE-754 double precision number"[^15]. In Java, `(long) Math.pow(3, 39)` is not 3 to the power 39, because the `double` result is rounded. A loop of integer products is exact while it does not overflow:

```java wrong id=java-pow-double
public class Main {
  public static void main(String[] args) {
    long loop = 1;
    for (int i = 0; i < 39; i++) {
      loop *= 3;
    }
    System.out.println((long) Math.pow(3, 39));
    System.out.println(loop);
  }
}
```

```output for=java-pow-double
4052555153018976256
4052555153018976267
```

- **Python `//` and `%` are not truncated division.** Code that is moved from C or Java to Python gives other results for negative values: `-7 // 2` is -4 and `-7 % 2` is 1 in Python, but -3 and -1 in C. Use `math.trunc(a / b)` only for small values, because `a / b` is a float division.

- **JavaScript integers above the safe integers are not exact.** The operators work on 64-bit floats[^15]. For larger integers, use `BigInt` values. With them, division "truncates fractional components towards zero"[^17], so `7n / 2n` is `3n`.

- **The operators can overflow.** In a fixed-width type, `+`, `-`, `*` and the power can give a result that does not fit. Each language has its own rule (see [[overflow-semantics]]), and [[detect-integer-overflow]] shows how to check.

## Related tasks

- [[detect-integer-overflow]]: check a result that does not fit.
- [[detect-division-by-zero]]: check the divisor.
- [[use-big-integers]]: integers of any size.
- [[round-a-number]]: rounding a float quotient.

See also the concepts [[integer-division-and-modulo]], [[overflow-semantics]] and [[integer-representation]].

## References
