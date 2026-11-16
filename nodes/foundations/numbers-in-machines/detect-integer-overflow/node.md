---
tag: XDABZC
kind: task
slug: detect-integer-overflow
name: Detect integer overflow
aliases: [checked addition, overflow check, checked_add, Math.addExact, __builtin_add_overflow, ckd_add, checked context, safe integer arithmetic]
area: foundations/numbers-in-machines
level: core
summary: >-
  Add two 64-bit integers and report an overflow instead of a wrong sum. Use a checked operation, a compiler builtin or a test of the operands before the addition.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://en.cppreference.com/c/language/operator_arithmetic
    title: Arithmetic operators, C reference, cppreference.com
    locator: Overflows; Unary arithmetic, Notes
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Integer overflow
    accessed: 2026-11-16
  - url: https://docs.oracle.com/javase/specs/jls/se27/html/jls-4.html
    title: "The Java Language Specification, Java SE 27 Edition, Chapter 4: Types, Values, and Variables"
    locator: 4.2.2 Integer Operations
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/checked-and-unchecked
    title: The checked and unchecked statements, C# reference, Microsoft Learn
    locator: the introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/stdtypes.html
    title: Built-in Types, Python 3 documentation
    locator: Numeric Types, int, float, complex
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/isSafeInteger
    title: Number.isSafeInteger(), MDN Web Docs
    locator: Description
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/BigInt/asIntN
    title: BigInt.asIntN(), MDN Web Docs
    locator: Description
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/lang/Math.html
    title: Math, Java SE 27 API
    locator: addExact(long, long); multiplyExact(long, long); abs(long); absExact(long)
    accessed: 2026-11-16
  - url: https://gcc.gnu.org/onlinedocs/gcc/Integer-Overflow-Builtins.html
    title: Built-in Functions to Perform Arithmetic with Overflow Checking, Using the GNU Compiler Collection (GCC)
    locator: __builtin_add_overflow; __builtin_mul_overflow
    accessed: 2026-11-16
  - url: https://en.cppreference.com/c/numeric/ckd_add
    title: ckd_add, C reference, cppreference.com
    locator: Return value
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/primitive.i64.html
    title: i64, Rust standard library
    locator: checked_add; wrapping_add; overflowing_add; saturating_add
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/reference/expressions/operator-expr.html
    title: Operator expressions, The Rust Reference
    locator: Overflow
    accessed: 2026-11-16
  - url: https://tiswww.case.edu/php/chet/bash/bashref.html
    title: Bash Reference Manual, version 5.3
    locator: 6.5 Shell Arithmetic
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/ctypes.html
    title: ctypes, A foreign function library for Python, Python 3 documentation
    locator: Fundamental data types, c_byte
    accessed: 2026-11-16
---

## Task

Add two 64-bit signed integers. Return the sum when it fits in 64 bits. When it does not fit, report the overflow, and do not return a wrong sum.

The function `checked_add(a, b)` returns `a + b`, or no value. For example, `checked_add(2, 3)` is 5, and `checked_add(9223372036854775807, 1)` has no value, because 9223372036854775807 is the largest 64-bit signed integer.

The plain `+` operator does not do this check in most languages. In C, "When signed integer arithmetic operation overflows (the result does not fit in the result type), the behavior is undefined"[^1]. In Go, the sum wraps around, and "Overflow does not cause a run-time panic"[^2]. In Java, "The integer operators do not indicate overflow or underflow in any way"[^3], and the sum wraps. In C#, the sum also wraps by default[^4]. See [[overflow-semantics]] for the rule of each language.

There are three ways to detect the overflow:

- **A checked operation.** A library function or a language feature does the addition and reports the overflow: `Math.addExact` in Java, `checked_add` in Rust, a `checked` context in C#.
- **A compiler builtin.** GCC and Clang have `__builtin_add_overflow`, and C23 has `ckd_add`. They compute the exact sum and tell whether it fits.
- **A test before the operation.** Compare the operands with the limits of the type, and add only when the sum fits. With `b > 0`, the sum overflows when `a > MAX - b`. With `b < 0`, it overflows when `a < MIN - b`. These two subtractions cannot overflow. This test works in each language, and in C it is the only way with no undefined behavior and no extension.

## Solutions

| Language | Check | The plain operator |
|---|---|---|
| Python | compare the exact sum with the range | exact; Python integers have no fixed width |
| JavaScript, TypeScript | `BigInt.asIntN(64, s) === s` on `BigInt` values | a `number` is exact only up to 2 ** 53 - 1 |
| Java | `Math.addExact`, which throws `ArithmeticException` | wraps |
| C | `__builtin_add_overflow`, or a test before the operation | undefined behavior |
| C++ | a test before the operation | undefined behavior |
| C# | `checked(a + b)`, which throws `OverflowException` | wraps in the default unchecked context |
| Go | a sign test after the addition | wraps |
| Rust | `i64::checked_add`, which returns `None` | panics in a debug build |
| Bash | a test before the operation | wraps |

- **Python.** "Integers have unlimited precision."[^5] So `a + b` is the exact sum, and the solution compares it with the range of a 64-bit integer after the addition.
- **JavaScript.** A `number` is a 64-bit float, so it holds integers exactly only in the safe range from -(2 ** 53 - 1) to 2 ** 53 - 1[^6]. The solution uses `BigInt` values, which are exact. `BigInt.asIntN` "truncates a BigInt value to the given number of bits, and interprets the result as a signed integer"[^7], so `BigInt.asIntN(64, s)` is equal to `s` only when `s` fits in 64 bits.
- **TypeScript.** The same code as JavaScript, with the type `bigint`.
- **Java.** `Math.addExact(long, long)` "Returns the sum of its arguments, throwing an exception if the result overflows a long"[^8]. The solution catches the `ArithmeticException` and returns an empty `OptionalLong`. The test file also checks a second form with a test before the operation.
- **C.** `__builtin_add_overflow` is a GCC and Clang extension. The GCC manual says that "these built-in functions have fully defined behavior for all argument values"[^9]. The function returns true on overflow. C23 has the same check in `<stdckdint.h>`: `ckd_add` returns "false if the value assigned to *result correctly represents the mathematical result of the addition, true otherwise"[^10]. GCC 13 has no `<stdckdint.h>`, so the solution uses the builtin, with a portable test before the operation as the second function.
- **C++.** The solution tests the operands with `std::numeric_limits<std::int64_t>` before the addition. The function is `constexpr`, so a `static_assert` can check it at compile time.
- **C#.** "In a checked context, a System.OverflowException is thrown."[^4] The solution catches the exception and returns `null` as a `long?`. "The default statement is unchecked"[^4], and there "addition wraps from the maximum value to the minimum value"[^4].
- **Go.** The sum wraps, and the wrap is defined behavior. So the solution adds first and tests after: the sum overflowed when `a` and `b` have the same sign and the sum has the other sign.
- **Rust.** `checked_add` "Computes self + rhs" and returns `None` "if overflow occurred"[^11]. A plain `a + b` is not a check: "Integer operators will panic when they overflow when compiled in debug mode"[^12], and a build without overflow checks does not report the overflow (see [[overflow-semantics]]). `checked_add` gives the same result in each build.
- **Bash.** "Evaluation is done in the largest fixed-width integers available, with no check for overflow"[^13]. The solution tests the operands before the addition and returns status 1 on overflow.

The Python function:

```python run id=python-checked-add
INT64_MIN, INT64_MAX = -(2**63), 2**63 - 1


def checked_add(a, b):
    s = a + b
    return s if INT64_MIN <= s <= INT64_MAX else None


print(checked_add(2, 3))
print(checked_add(INT64_MAX, 1))
print(checked_add(INT64_MIN, INT64_MAX))
```

```output for=python-checked-add
5
None
-1
```

The GCC builtin stores the sum wrapped to 64 bits in all cases, and its return value tells whether that stored value is the exact sum:

```c run id=c-builtin
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    int64_t s;
    bool over = __builtin_add_overflow(INT64_MAX, 1, &s);
    printf("overflow: %s, stored: %" PRId64 "\n", over ? "yes" : "no", s);
    over = __builtin_add_overflow(INT64_MAX - 1, 1, &s);
    printf("overflow: %s, stored: %" PRId64 "\n", over ? "yes" : "no", s);
}
```

```output for=c-builtin
overflow: yes, stored: -9223372036854775808
overflow: no, stored: 9223372036854775807
```

Rust has four forms of the addition. `wrapping_add` computes the sum "wrapping around at the boundary of the type"[^11]. `overflowing_add` returns "a tuple of the addition along with a boolean indicating whether an arithmetic overflow would occur"[^11]. `saturating_add` stops at the limit of the type "instead of overflowing"[^11]:

```rust run id=rust-add-forms
fn main() {
    let a = i64::MAX;
    println!("{:?}", a.checked_add(1));
    println!("{:?}", a.checked_add(-1));
    println!("{}", a.wrapping_add(1));
    println!("{:?}", a.overflowing_add(1));
    println!("{}", a.saturating_add(1));
}
```

```output for=rust-add-forms
None
Some(9223372036854775806)
-9223372036854775808
(-9223372036854775808, true)
9223372036854775807
```

In Go, a test after the addition is correct, because the wrap is defined:

```go run id=go-test-after
package main

import (
	"fmt"
	"math"
)

func main() {
	var a, b int64 = math.MaxInt64, 1
	s := a + b
	fmt.Println(s)
	fmt.Println("overflow:", (a >= 0) == (b >= 0) && (s >= 0) != (a >= 0))
}
```

```output for=go-test-after
-9223372036854775808
overflow: true
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `small`, `negative`: sums far from the limits.
- `beyond-32-bit`: 9000000000 does not fit in 32 bits, but it fits in 64 bits.
- `max-plus-zero`, `min-plus-zero`: the limits themselves are valid sums.
- `max-plus-one`, `max-plus-max`: the sum is above the largest value.
- `min-minus-one`, `min-plus-min`: the sum is below the smallest value.
- `min-plus-max`, `max-minus-one`: operands with different signs never overflow, even at the limits.

## Pitfalls

- **A test after the operation in C and C++.** A test such as `if (a + b < a)` computes the sum first. When the sum overflows, the behavior is already undefined[^1], and a compiler can remove the test. Use the test before the operation, or a builtin. The test after the operation is correct only where the wrap is defined, as in Go and Java.

- **Negation and absolute value overflow too.** The negative of the smallest value does not fit, because the range of a two's complement integer has one more negative value than positive values (see [[twos-complement]]). In C, "The unary minus invokes undefined behavior due to signed integer overflow when applied to INT_MIN, LONG_MIN, or LLONG_MIN"[^1]. In Java, for `Math.abs(Long.MIN_VALUE)` "the result is that same value, which is negative"[^8], and `Math.absExact` throws an exception[^8]:

```java wrong id=java-abs-min
public class Main {
  public static void main(String[] args) {
    System.out.println(Math.abs(Long.MIN_VALUE));
    System.out.println(-Long.MIN_VALUE);
    try {
      System.out.println(Math.absExact(Long.MIN_VALUE));
    } catch (ArithmeticException e) {
      System.out.println(e.getClass().getSimpleName());
    }
  }
}
```

```output for=java-abs-min
-9223372036854775808
-9223372036854775808
ArithmeticException
```

- **A JavaScript number loses precision before it overflows.** Above 2 ** 53 - 1, a `number` cannot hold each integer, so `a + 1` and `a + 2` can give the same value with no error. Use `Number.isSafeInteger` to test a result, or use `BigInt`[^6]:

```javascript wrong id=javascript-unsafe
const a = Number.MAX_SAFE_INTEGER;
console.log(a + 1 === a + 2);
console.log(Number.isSafeInteger(a + 1));
console.log(BigInt(a) + 2n);
```

```output for=javascript-unsafe
true
false
9007199254740993n
```

- **Fixed-width types in Python wrap.** A Python `int` does not overflow, but the fixed-width types of `ctypes`, `array` and NumPy do. For the `ctypes` integer types, "no overflow checking is done"[^14]:

```python wrong id=python-ctypes-wrap
import ctypes

print(ctypes.c_int64(2**63).value)
print(ctypes.c_int8(200).value)
```

```output for=python-ctypes-wrap
-9223372036854775808
-56
```

- **Each operation needs its own check.** A check of the addition does not cover a multiplication. Use the checked form of each operation: `Math.multiplyExact` in Java[^8], `__builtin_mul_overflow` in GCC[^9], `checked_mul` in Rust. A test before a multiplication is harder to write than a test before an addition, so prefer the library function.

- **A wrong check of the result.** Do not test `s < 0` alone after the addition. A negative sum of two negative operands is correct. The sum overflowed only when the operands have the same sign and the sum has the other sign.

## Related tasks

- [[do-integer-arithmetic]]: the operators and their overflow rules.
- [[detect-division-by-zero]]: the other error of integer arithmetic.
- [[use-big-integers]]: when a sum must not overflow at all.

See also the concepts [[overflow-semantics]], [[twos-complement]] and [[integer-representation]].

## References
