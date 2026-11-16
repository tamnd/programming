---
tag: A3QDBQ
kind: task
slug: detect-division-by-zero
name: Detect division by zero
aliases: [division by zero, ZeroDivisionError, ArithmeticException, DivideByZeroException, checked_div, Math.divideExact, safe division, integer division error]
area: foundations/numbers-in-machines
level: intro
summary: >-
  Divide two integers and report a zero divisor instead of a crash. Integer division by zero is an error, an exception or undefined behavior; float division by zero gives infinity or NaN.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/exceptions.html
    title: Built-in Exceptions, Python 3 documentation
    locator: ZeroDivisionError
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/stdtypes.html
    title: Built-in Types, Python 3 documentation
    locator: Numeric Types, int, float, complex, note 1
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Division
    title: Division (/), MDN Web Docs
    locator: Description; Examples
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/BigInt/asIntN
    title: BigInt.asIntN(), MDN Web Docs
    locator: Description
    accessed: 2026-11-16
  - url: https://docs.oracle.com/javase/specs/jls/se27/html/jls-15.html
    title: "The Java Language Specification, Java SE 27 Edition, Chapter 15: Expressions"
    locator: 15.17.2 Division Operator /
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/lang/Math.html
    title: Math, Java SE 27 API
    locator: divideExact(long, long)
    accessed: 2026-11-16
  - url: https://en.cppreference.com/c/language/operator_arithmetic
    title: Arithmetic operators, C reference, cppreference.com
    locator: Multiplicative operators; Division
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/arithmetic-operators
    title: Arithmetic operators, C# reference, Microsoft Learn
    locator: Division operator /; Arithmetic overflow and division by zero
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Integer operators; Floating-point operators
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/primitive.i64.html
    title: i64, Rust standard library
    locator: checked_div
    accessed: 2026-11-16
  - url: https://tiswww.case.edu/php/chet/bash/bashref.html
    title: Bash Reference Manual, version 5.3
    locator: 6.5 Shell Arithmetic
    accessed: 2026-11-16
---

## Task

Divide two 64-bit signed integers, and round the quotient toward zero. When the divisor is zero, report it, and do not crash or return a wrong value.

The function `safe_div(a, b)` returns the quotient, or no value. For example, `safe_div(7, 2)` is 3, `safe_div(-7, 2)` is -3, and `safe_div(5, 0)` has no value.

There is a second case with no valid result. The smallest 64-bit value, -9223372036854775808, divided by -1 is 9223372036854775808, which is one more than the largest 64-bit value. So the function also returns no value for this case. See [[detect-integer-overflow]] for the other overflows.

Each language treats an integer division by zero in its own way:

- **An error or an exception.** Python raises `ZeroDivisionError`, Java throws `ArithmeticException`, C# throws `DivideByZeroException`, Go panics, Rust panics, and Bash stops the expression with an error.
- **Undefined behavior.** In C and C++ the program has no defined result.
- **No error.** A JavaScript `number` is a float, so `5 / 0` is `Infinity`.

The safe way is the same in each language: test the divisor before the division.

## Solutions

| Language | Check | `5 / 0` on integers | `MIN / -1` |
|---|---|---|---|
| Python | test `b == 0` | `ZeroDivisionError` | exact, no error |
| JavaScript, TypeScript | test `b === 0n` on `BigInt` values | `RangeError` | exact, no error |
| Java | `Math.divideExact` | `ArithmeticException` | `MIN`, no error |
| C, C++ | test both cases before the division | undefined behavior | undefined behavior |
| C# | catch `ArithmeticException` | `DivideByZeroException` | `ArithmeticException` |
| Go | test both cases before the division | run-time panic | `MIN`, no error |
| Rust | `i64::checked_div` | panic | panic |
| Bash | test both cases before the division | an error | no check for overflow |

- **Python.** `ZeroDivisionError` is "Raised when the second argument of a division or modulo operation is zero"[^1]. The operator `//` does not round toward zero: "The result is always rounded towards minus infinity"[^2]. So the solution divides the magnitudes and sets the sign after. A Python `int` has no fixed width, so the solution also compares the quotient with the 64-bit range.
- **JavaScript.** The solution uses `BigInt` values. For `BigInt` operands, the result is "the quotient of the two operands truncated towards zero", and "A RangeError is thrown if the divisor y is 0n"[^3]. `BigInt.asIntN(64, q)` changes `q` only when it does not fit in 64 bits[^4].
- **TypeScript.** The same code as JavaScript, with the type `bigint`.
- **Java.** When "the value of the divisor in an integer division is 0", the plain `/` throws: "then an ArithmeticException is thrown"[^5]. For the smallest value divided by -1, "integer overflow occurs and the result is equal to the dividend", and "Despite the overflow, no exception is thrown in this case."[^5] `Math.divideExact` throws in both cases; the overflow "occurs in this method if x is Long.MIN_VALUE and y is -1"[^6].
- **C.** "If the second operand is zero, the behavior is undefined."[^7] Also, "If the quotient a/b is not representable in the result type, the behavior of both a/b and a%b is undefined"[^7]. The solution tests both cases before the division.
- **C++.** The same test as C, in a `constexpr` function that returns `std::optional<std::int64_t>`.
- **C#.** "Integer division by zero always throws a DivideByZeroException."[^8] For the smallest value divided by -1, "The .NET runtime throws an ArithmeticException in this case"[^8], even in an unchecked context. `DivideByZeroException` is a kind of `ArithmeticException`, so the solution catches `ArithmeticException` and returns `null`.
- **Go.** "If the divisor is zero at run time, a run-time panic occurs."[^9] The smallest value divided by -1 does not panic: the spec says that `x / -1` "is equal to x"[^9] for the most negative value `x`. The solution tests both cases before the division.
- **Rust.** `checked_div` "Computes self / rhs" and returns `None` "if rhs == 0 or the division results in overflow"[^10].
- **Bash.** Bash arithmetic has "no check for overflow, though division by 0 is trapped and flagged as an error"[^11]. The error stops the expansion, so the solution tests the divisor first and returns status 1.

The Python function, and the floor division that it does not use:

```python run id=python-safe-div
INT64_MIN, INT64_MAX = -(2**63), 2**63 - 1


def safe_div(a, b):
    if b == 0:
        return None
    q = abs(a) // abs(b)
    if (a < 0) != (b < 0):
        q = -q
    return q if INT64_MIN <= q <= INT64_MAX else None


print(safe_div(7, 2), safe_div(-7, 2), safe_div(5, 0))
print(safe_div(INT64_MIN, -1))
print(-7 // 2)
try:
    5 // 0
except ZeroDivisionError as e:
    print(type(e).__name__ + ":", e)
```

```output for=python-safe-div
3 -3 None
None
-4
ZeroDivisionError: integer division or modulo by zero
```

In Java, the plain operator and `Math.divideExact` differ only in the overflow case:

```java run id=java-divide
public class Main {
  public static void main(String[] args) {
    long min = Long.MIN_VALUE;
    long minusOne = -1;
    long zero = 0;
    System.out.println(min / minusOne);
    try {
      System.out.println(Math.divideExact(min, minusOne));
    } catch (ArithmeticException e) {
      System.out.println("divideExact: " + e.getMessage());
    }
    try {
      System.out.println(5 / zero);
    } catch (ArithmeticException e) {
      System.out.println("/: " + e.getMessage());
    }
  }
}
```

```output for=java-divide
-9223372036854775808
divideExact: long overflow
/: / by zero
```

In Go, a zero divisor at run time panics, and the program can recover from the panic:

```go run id=go-divide
package main

import (
	"fmt"
	"math"
)

func divide(a, b int64) (q int64, err error) {
	defer func() {
		if r := recover(); r != nil {
			err = fmt.Errorf("%v", r)
		}
	}()
	return a / b, nil
}

func main() {
	fmt.Println(divide(7, 2))
	fmt.Println(divide(5, 0))
	fmt.Println(divide(math.MinInt64, -1))
}
```

```output for=go-divide
3 <nil>
0 runtime error: integer divide by zero
-9223372036854775808 <nil>
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `exact`, `remainder-dropped`, `zero-dividend`: ordinary quotients. Zero divided by a number is 0.
- `zero-divisor`, `negative-by-zero`: a zero divisor gives no value, for each sign of the dividend.
- `zero-by-zero`: 0 divided by 0 gives no value too.
- `negative-dividend`, `negative-divisor`, `both-negative`: the quotient rounds toward zero, so -7 divided by 2 is -3, not -4.
- `min-by-minus-one`: the quotient does not fit in 64 bits.
- `min-by-one`, `max-by-minus-one`, `min-by-max`: other quotients at the limits, which fit.

## Pitfalls

- **Float division by zero is not an error in most languages.** It follows IEEE 754 (see [[nan-and-infinity]]): a nonzero number divided by zero gives an infinity, and 0 divided by 0 gives NaN. In JavaScript, "number division by zero returns Infinity or -Infinity"[^3]. In C#, "Arithmetic operations by using the float and double types never throw an exception"[^8]. In Go, "The result of a floating-point or complex division by zero is not specified beyond the IEEE 754 standard"[^9]. So a test for an exception does not find these results; test the divisor, or test the result with `isFinite` or `isNaN`:

```javascript wrong id=javascript-float-zero
console.log(5 / 0, -5 / 0, 0 / 0);
console.log(Number.isFinite(5 / 0));
console.log(1 / 1e-320);
try {
  console.log(5n / 0n);
} catch (e) {
  console.log(e.name);
}
```

```output for=javascript-float-zero
Infinity -Infinity NaN
false
Infinity
RangeError
```

- **Python float division by zero raises.** Python does not follow the IEEE 754 default here: `1.0 / 0.0` raises `ZeroDivisionError`, like the integer division[^1]. Code that moves between Python and another language must handle both results.

```python wrong id=python-float-zero
for a, b in [(1, 0), (1.0, 0.0), (0.0, 0.0)]:
    try:
        print(a / b)
    except ZeroDivisionError as e:
        print(type(e).__name__ + ":", e)
```

```output for=python-float-zero
ZeroDivisionError: division by zero
ZeroDivisionError: float division by zero
ZeroDivisionError: float division by zero
```

- **A divisor near zero can overflow.** A test for zero does not cover a float divisor that is very small. In JavaScript, `1 / 1e-320` is `Infinity`, because the quotient is larger than the largest float. Test the result too, when the divisor comes from a computation.

- **The remainder has the same problem.** `a % 0` is a division by zero in each language above, and in C `INT_MIN % -1` is undefined behavior[^7]. Test the divisor before a remainder too.

- **A test after a crash is too late in C.** In C and C++, a division by zero has no defined result[^7]. The program can stop, print a wrong value or go on, and a compiler can remove a test that comes after the division. Only a test before the division is safe.

## Related tasks

- [[detect-integer-overflow]]: the other error of integer arithmetic.
- [[do-integer-arithmetic]]: the division and remainder operators of each language.
- [[compare-floating-point-numbers]]: NaN and infinity in comparisons.

See also the concepts [[integer-division-and-modulo]], [[nan-and-infinity]] and [[overflow-semantics]].

## References
