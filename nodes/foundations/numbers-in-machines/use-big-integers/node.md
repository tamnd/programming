---
tag: 5TBHF2
kind: task
slug: use-big-integers
name: Use big integers
aliases: [big integers, bignum, BigInteger, BigInt, arbitrary-precision integers, unlimited precision integers, big.Int, exact large integers]
area: foundations/numbers-in-machines
level: core
summary: >-
  Compute an integer that does not fit in 64 bits, exactly. Use the big integer type of the language, or an array of digit groups, and convert back to a fixed-width type with a check.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://en.cppreference.com/c/types/integer
    title: Fixed width integer types, C reference, cppreference.com
    locator: Types; Example
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/reference/types/numeric.html
    title: Numeric types, The Rust Reference
    locator: Integer types
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/stdtypes.html
    title: Built-in Types, Python 3 documentation
    locator: Numeric Types; int.to_bytes; Integer string conversion length limitation
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/BigInt
    title: BigInt, MDN Web Docs
    locator: the introduction; Description
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/math/BigInteger.html
    title: BigInteger, Java SE 27 API
    locator: the class description; pow, longValue, longValueExact
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.numerics.biginteger
    title: BigInteger Struct, .NET API reference, Microsoft Learn
    locator: the definition; Pow; Remarks
    accessed: 2026-11-16
  - url: https://pkg.go.dev/math/big
    title: big, Go standard library
    locator: Overview; func (*Int) Exp, IsInt64, IsUint64
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/types/integer
    title: Fixed width integer types, C++ reference, cppreference.com
    locator: Types; Example
    accessed: 2026-11-16
  - url: https://tiswww.case.edu/php/chet/bash/bashref.html
    title: Bash Reference Manual, version 5.3
    locator: 6.5 Shell Arithmetic
    accessed: 2026-11-16
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/bc.html
    title: bc, The Open Group Base Specifications Issue 8
    locator: NAME; DESCRIPTION; EXTENDED DESCRIPTION
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/BigInt/asIntN
    title: BigInt.asIntN(), MDN Web Docs
    locator: the introduction
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.numerics.biginteger.op_explicit
    title: BigInteger.Explicit Operator, .NET API reference, Microsoft Learn
    locator: Explicit(BigInteger to Int64), Exceptions
    accessed: 2026-11-16
---

## Task

Compute an integer result that is too large for a 64-bit integer, and get all of its digits.

The largest value of a 64-bit signed integer is 9223372036854775807[^1], and the largest value of a 64-bit unsigned integer is 2^64 - 1[^2]. Many results are larger, for example 2 to the power 64, or the product of two large 64-bit values. In a fixed-width type, such a result wraps, saturates, raises an error or becomes a float with fewer digits. A big integer type has no fixed width. It uses as many digits as the value needs, so the arithmetic is exact.

The function `big_power(base, exp)` returns `base` to the power `exp` as decimal text:

```text
big_power(2, 64)    = "18446744073709551616"
big_power(-3, 41)   = "-36472996377170786403"
big_power(1000, 200) = "1" followed by 600 zeros
```

The result is text, so that each language can give it back with no loss, and the checker compares it exactly. `-1000 <= base <= 1000` and `0 <= exp <= 200`, and any base to the power 0 is 1.

## Solutions

| Language | Big integer type | Power | Back to 64 bits, with a check |
|---|---|---|---|
| Python | `int` | `base ** exp` | `x.to_bytes(8, "big", signed=True)` |
| JavaScript, TypeScript | `bigint` | `BigInt(base) ** BigInt(exp)` | compare with `BigInt.asIntN(64, x)` |
| Java | `java.math.BigInteger` | `BigInteger.valueOf(base).pow(exp)` | `x.longValueExact()` |
| C# | `System.Numerics.BigInteger` | `BigInteger.Pow(base, exp)` | `(long)x` |
| Go | `math/big.Int` | `new(big.Int).Exp(x, y, nil)` | `x.IsInt64()`, then `x.Int64()` |
| C, C++ | an array of limbs | repeated multiplication | compare with the text of the limit |
| Rust | an array of limbs, or `u128` | repeated multiplication | `i64::try_from(x)` |
| Bash | an array of limbs | repeated multiplication | none |

- **Python.** "Integers have unlimited precision."[^3] So `str(base ** exp)` is the full solution. To put an `int` into a fixed number of bytes, use `int.to_bytes`: "An OverflowError is raised if the integer is not representable with the given number of bytes"[^3].
- **JavaScript.** A `bigint` is the type for "integer values which are too high or too low to be represented by the number primitive"[^4]. Write a `bigint` literal with a suffix `n`, as in `2n ** 64n`, or convert a number with `BigInt(x)`[^4]. A `bigint` "cannot be mixed with a Number value in operations"[^4], so convert both operands to the same type first.
- **TypeScript.** The same code as JavaScript, with the type `bigint`.
- **Java.** `BigInteger` holds "Immutable arbitrary-precision integers"[^5]. Each operation returns a new object, and `pow(int exponent)` returns the value to the power `exponent`[^5].
- **C#.** `BigInteger` "Represents an arbitrarily large signed integer"[^6], whose value "in theory has no upper or lower bounds"[^6]. `BigInteger.Pow` "Raises a BigInteger value to the power of a specified value"[^6]. Give `CultureInfo.InvariantCulture` to `ToString`, so that the digits do not depend on the culture of the computer.
- **Go.** "Package big implements arbitrary-precision arithmetic (big numbers)"[^7]. For an operation, "the result is the receiver"[^7]: `z.Exp(x, y, nil)` sets `z` to `x` to the power `y`, and `z.String()` gives the decimal text.
- **Rust.** The integer types of the language go up to `u128`, with a maximum of 2^128 - 1, and `i128`[^2]. A larger value needs more than one integer. The solution keeps the value in a `Vec<u32>` of limbs (see below).
- **C, C++.** The fixed-width types such as `int64_t` and `uint64_t` have a width of "typically 8, 16, 32 or 64"[^1] bits. C++ has the same types in `<cstdint>`[^8]. A larger value needs more than one integer, so the solution keeps the value in limbs, in a `malloc` array in C and in a `std::vector` in C++.
- **Bash.** Shell arithmetic "is done in the largest fixed-width integers available, with no check for overflow"[^9]. The solution keeps the limbs in a Bash array. For interactive work, `bc` is an "arbitrary-precision arithmetic language"[^10]: `echo '2^200' | bc` prints all the digits. But "Very large numbers shall be split across lines with 70 characters per line in the POSIX locale"[^10], so join the lines before you use the text.

### Limbs

Where the language has no big integer type, keep the number in an array of limbs, the least significant limb first. Each limb holds 9 decimal digits, so it is a value from 0 to 999999999 and it fits in 32 bits. To multiply the number by a small value `m`, multiply each limb by `m`, add the carry from the limb before, keep the remainder modulo 10^9 and carry the rest. A carry that is left at the end becomes a new limb. With `m <= 1000`, each product is less than 10^12 and fits in 64 bits.

To print the number, print the top limb as it is and each other limb with 9 digits and leading zeros (`%09u` in C). The limbs are decimal, so the printing needs no division.

### The type in use

Python shows the difference between `int` and a fixed width of 64 bits:

```python run id=python-big-integers
x = 2 ** 64
print(x, x + 1)
print(3 ** 41)
try:
    x.to_bytes(8, "big")
except OverflowError:
    print("OverflowError")
print(len(str(1000 ** 200)))
```

```output for=python-big-integers
18446744073709551616 18446744073709551617
36472996377170786403
OverflowError
601
```

In Go, an `int64` wraps without an error, and a `big.Int` tells whether its value fits in an `int64`: "IsInt64 reports whether x can be represented as an int64"[^7].

```go run id=go-big-int
package main

import (
	"fmt"
	"math"
	"math/big"
)

func main() {
	x := big.NewInt(math.MaxInt64)
	x.Add(x, big.NewInt(1))
	fmt.Println(x, x.IsInt64(), x.IsUint64())
	y := new(big.Int).Exp(big.NewInt(2), big.NewInt(100), nil)
	fmt.Println(y)
	var n int64 = math.MaxInt64
	n++
	fmt.Println(n)
}
```

```output for=go-big-int
9223372036854775808 false true
1267650600228229401496703205376
-9223372036854775808
```

In Rust, `u128` gives 64 more bits, and `checked_pow` returns `None` when the result does not fit:

```rust run id=rust-u128
fn main() {
    println!("{:?}", 2u64.checked_pow(64));
    println!("{}", 2u128.pow(64));
    println!("{}", u128::MAX);
    println!("{:?}", 2u128.checked_pow(128));
}
```

```output for=rust-u128
None
18446744073709551616
340282366920938463463374607431768211455
None
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `zero-exponent`, `zero-to-the-zero`: any base to the power 0 is 1, also 0. For example, `Exp` in Go with no modulus gives 1 when `y <= 0`[^7]. The limb solutions start from the limb 1, so they give 1 too.
- `zero-base`: 0 to a positive power is 0, with no sign.
- `minus-one-odd`: -1 to an odd power is -1.
- `negative-odd`, `negative-even`: a negative base gives a negative result for an odd exponent only. The limb solutions compute with `|base|` and add the sign at the end.
- `int64-max-plus-one`: 2^63 is 1 more than the largest `int64`. It fits in a `uint64`, and the native tests check this.
- `uint64-max-plus-one`: 2^64 is 1 more than the largest `uint64`.
- `two-to-the-200`: a result of 61 digits, with inner zeros. A limb that is printed without its leading zeros makes this result wrong.
- `largest`, `smallest`: 1000^200 has 601 digits, and (-1000)^199 has 597 zeros after "-1". Most of the limbs are 0.

## Pitfalls

- **A fixed-width type.** `2 ** 63` in Bash and `n++` on the largest `int64` in Go wrap to a negative value with no error (see the Go block). Use a big integer type when the result can be larger than the type, or detect the overflow (see [[detect-integer-overflow]]).

- **A float in place of an integer.** A JavaScript `number` is a float. A large power looks correct, but its last digits are not:

```javascript wrong id=javascript-number-power
console.log(2 ** 64 + 1 === 2 ** 64);
console.log(String(3 ** 41));
console.log((3n ** 41n).toString());
try {
  console.log(1n + 1);
} catch (e) {
  console.log(e.name);
}
```

```output for=javascript-number-power
true
36472996377170790000
36472996377170786403
TypeError
```

MDN warns that "the precision of a BigInt value may be lost when it is coerced to a Number value"[^4]. Keep the value a `bigint` until the end.

- **A conversion that cuts the value.** In Java, `longValue()` gives no error: when the value is too big for a `long`, "only the low-order 64 bits are returned"[^5]. Use `longValueExact()`, which throws an `ArithmeticException` "If the value of this BigInteger is out of the range of the long type"[^5]:

```java wrong id=java-long-value
import java.math.BigInteger;

public class Main {
  public static void main(String[] args) {
    BigInteger big = BigInteger.TWO.pow(64).add(BigInteger.valueOf(5));
    System.out.println(big.longValue());
    try {
      big.longValueExact();
    } catch (ArithmeticException e) {
      System.out.println("ArithmeticException: " + e.getMessage());
    }
  }
}
```

```output for=java-long-value
5
ArithmeticException: BigInteger out of long range
```

The other languages have the same two kinds of conversion. In JavaScript, `BigInt.asIntN` "truncates a BigInt value to the given number of least significant bits"[^11], with no error. In C#, the explicit conversion to `long` throws an `OverflowException` when the "value is less than Int64.MinValue or is greater than Int64.MaxValue"[^12]. In Go, test `IsInt64()` before `Int64()`, because when x does not fit, "the result is undefined"[^7].

- **Very long decimal text in Python.** "CPython has a global limit for converting between int and str to mitigate denial of service attacks"[^3]. The default limit is 4300 digits ("Illustrative, this is the default"[^3]), and a longer conversion raises a `ValueError`. The arithmetic itself has no limit. To print a longer value, raise the limit with `sys.set_int_max_str_digits`, or print the value in hexadecimal: "Hexadecimal, octal, and binary conversions are unlimited"[^3].

- **Missing zeros in a limb.** A limb of 5 is the 9 digits `000000005` inside the number. Print each limb after the top limb with 9 digits and leading zeros. The case `two-to-the-200` finds this error.

- **The sign of a negative base.** Compute with the absolute value of the base and add `-` only for an odd exponent. The C solution converts the base to `int64_t` before it negates it, so that the negation cannot overflow.

## Related tasks

- [[detect-integer-overflow]]: find out when a fixed-width result is wrong.
- [[do-integer-arithmetic]]: the operators of fixed-width integers.
- [[round-a-number]]: decimal types for values with a fraction.

See also the concepts [[arbitrary-precision-arithmetic]], [[integer-representation]], [[overflow-semantics]] and [[signed-and-unsigned-integers]].

## References
