---
tag: 85VAST
kind: concept
slug: arbitrary-precision-arithmetic
name: Arbitrary-precision arithmetic
aliases: [bignum, bignum arithmetic, big integer, BigInteger, BigInt, multiple-precision arithmetic, infinite-precision arithmetic, arbitrary-precision integer]
area: foundations/numbers-in-machines
level: core
summary: >-
  Arbitrary-precision arithmetic stores a number as a list of digits that grows as needed, so integers do not overflow. The cost is speed and memory that grow with the size of the number.
applies_to: core-languages
viz: optional
formal: none
see_also: [integer-overflow, overflow-semantics, integer-representation, signed-and-unsigned-integers, integer-division-and-modulo, floating-point-arithmetic]
same_as:
  wikidata: Q527381
  wikipedia: Arbitrary-precision_arithmetic
sources:
  - url: https://en.wikipedia.org/wiki/Arbitrary-precision_arithmetic
    title: Arbitrary-precision arithmetic, Wikipedia
    locator: the introduction; Applications; Implementation issues; Pre-set precision; Implementation; History; Software libraries
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/stdtypes.html
    title: Built-in Types, The Python Standard Library
    locator: Numeric Types; Integer string conversion length limitation
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/math/BigInteger.html
    title: BigInteger, Java SE 27 API
    locator: the class description; API Note
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/BigInt
    title: BigInt, MDN Web Docs
    locator: Description; Cryptography; Use within JSON
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.numerics.biginteger
    title: BigInteger Struct, .NET API reference, Microsoft Learn
    locator: Remarks; Mutability
    accessed: 2026-11-16
  - url: https://pkg.go.dev/math/big
    title: big package, math/big, Go standard library
    locator: Overview
    accessed: 2026-11-16
  - url: https://docs.rs/num-bigint/latest/num_bigint/
    title: num_bigint crate documentation, docs.rs
    locator: the crate description
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/GNU_Multiple_Precision_Arithmetic_Library
    title: GNU Multiple Precision Arithmetic Library, Wikipedia
    locator: the introduction; C++ library interface
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/Karatsuba_algorithm
    title: Karatsuba algorithm, Wikipedia
    locator: the introduction; History
    accessed: 2026-11-16
---

## Idea

A fixed-width integer has a set number of bits, so it can hold only a set range of values. Arbitrary-precision arithmetic, also called bignum arithmetic, removes that limit. Wikipedia says the calculations are "performed on numbers whose digits of precision are potentially limited only by the available memory of the host system"[^1]. The hardware arithmetic of most processors "typically offers between 8 and 64 bits of precision"[^1].

A bignum does not fit in one machine word. It is stored as a sequence of digits in a large base, for example 2^32. The implementations "typically use variable-length arrays of digits"[^1], and the array grows when a result needs more digits. Each operation works on the digits one at a time, with a carry, as on paper.

```python run id=digits
BASE = 2**32


def to_digits(n):
    digits = []
    while n:
        digits.append(n % BASE)
        n //= BASE
    return digits or [0]


def from_digits(digits):
    n = 0
    for d in reversed(digits):
        n = n * BASE + d
    return n


def add(a, b):
    out, carry = [], 0
    for i in range(max(len(a), len(b))):
        s = (a[i] if i < len(a) else 0) + (b[i] if i < len(b) else 0) + carry
        out.append(s % BASE)
        carry = s // BASE
    if carry:
        out.append(carry)
    return out


def mul(a, b):
    out = [0] * (len(a) + len(b))
    for i, x in enumerate(a):
        carry = 0
        for j, y in enumerate(b):
            t = out[i + j] + x * y + carry
            out[i + j] = t % BASE
            carry = t // BASE
        out[i + len(b)] += carry
    while len(out) > 1 and out[-1] == 0:
        out.pop()
    return out


x = 2**64 - 1
y = 2**100 + 12345
dx, dy = to_digits(x), to_digits(y)
print("x digits:", dx)
print("y digits:", dy)
s = add(dx, dy)
p = mul(dx, dy)
print("sum digits:", len(s), "ok:", "yes" if from_digits(s) == x + y else "no")
print("product digits:", len(p), "ok:", "yes" if from_digits(p) == x * y else "no")
```

```output for=digits
x digits: [4294967295, 4294967295]
y digits: [12345, 0, 0, 16]
sum digits: 4 ok: yes
product digits: 6 ok: yes
```

The block keeps each number as a list of base 2^32 digits, with the lowest digit first. The addition carries from one digit to the next, and the multiplication is the method of school. CPython stores its `int` as "an arbitrary length number stored in binary form"[^2], so the block checks its results against `int`.

## How it works

**Where the support is.** Some languages use bignums for all integers. Wikipedia names "Lisp, Python, Perl, Haskell, Ruby and Raku"[^1], and says that this "eliminates the concern of incorrect results (or exceptions) due to simple overflow"[^1]. In Python, "Integers have unlimited precision."[^2] Other languages keep fixed-width integers as the default and give a separate bignum type:

- Java has `java.math.BigInteger`, "Immutable arbitrary-precision integers"[^3]. Its operations "exactly mimic those of Java's integer arithmetic operators"[^3], so division by zero throws an `ArithmeticException`.
- JavaScript has the `bigint` primitive, written with an `n` after the literal, as in `10n`[^4]. "BigInt values represent integer values which are too high or too low to be represented by the number primitive."[^4]
- C# has `System.Numerics.BigInteger`, "an immutable type that represents an arbitrarily large integer whose value in theory has no upper or lower bounds"[^5].
- Go has the `math/big` package, which "implements arbitrary-precision arithmetic (big numbers)"[^6] with the types `Int`, `Rat` and `Float`.
- Rust has no bignum in the standard library. The `num-bigint` crate has `BigUint`, which is "represented as a vector of digits"[^7], and `BigInt`.
- C and C++ have no bignum in the standard library. GMP is "a free library for arbitrary-precision arithmetic, operating on signed integers, rational numbers, and floating-point numbers"[^8], and it has a C++ interface with the class `mpz_class`[^8].

**Growth.** The size of a result depends on the size of the operands. The sum of two *N*-digit numbers has at most *N* + 1 digits, and the product has at most 2*N* digits. So a loop that multiplies again and again, such as a factorial, makes numbers that get longer at each step, and each step takes more time than the one before it.

**Immutable values and in-place values.** In Java and C#, an operation returns a new object. Microsoft says that `number++` on a `BigInteger` does not change the object: the runtime "creates a new BigInteger object and assigns it a value one greater than its previous value"[^5]. Go uses the opposite design to reuse memory. "the invocation c.Add(a, b) computes the sum a + b and stores the result in c"[^6], and the result can be one of the operands, which "may be safely overwritten (and its memory reused)"[^6].

**The limits that stay.** A bignum is limited by memory and time, not by a word size. In .NET, "an OutOfMemoryException can be thrown for any operation that causes a BigInteger value to grow too large"[^5]. Java requires a large range, not an unlimited one: "An ArithmeticException is thrown when a BigInteger constructor or method would generate a value outside of the supported range."[^3] CPython limits the conversion of large integers to and from decimal strings, which is slow: "CPython has a global limit for converting between int and str to mitigate denial of service attacks."[^2] The default limit is 4300 digits[^2]. "Hexadecimal, octal, and binary conversions are unlimited."[^2]

## In languages

| Language | Verdict | Big integer type |
|---|---|---|
| Python | native | every `int` is a bignum |
| JavaScript | native | `bigint`, with literals such as `10n` |
| TypeScript | native | `bigint`, as JavaScript |
| Java | stdlib | `java.math.BigInteger` |
| Kotlin | stdlib | `java.math.BigInteger` on the JVM |
| C# | stdlib | `System.Numerics.BigInteger` |
| Go | stdlib | `math/big` `Int`, `Rat` and `Float` |
| Rust | library | the `num-bigint` crate |
| C | library | GMP `mpz_t` |
| C++ | library | GMP `mpz_class` |
| Bash | absent | `$(( ))` uses fixed-width integers |
| PowerShell | stdlib | `[bigint]`, which is .NET `BigInteger` |
| SQL | absent | integer types have a fixed width; `NUMERIC` has a declared precision |
| Regex | n/a | no arithmetic |
| HTML | n/a | no arithmetic |
| CSS | n/a | no big integer type |

In JavaScript, a `bigint` "cannot be mixed with a Number value in operations; they must be coerced to the same type"[^4], and `/` on two `bigint` values "truncates fractional components towards zero"[^4]. In Python, the change from small to large values is not visible, because there is one `int` type.

## Variations and related ideas

- **Pre-set precision.** "In some languages such as REXX and ooRexx, the precision of all calculations must be set before doing a calculation."[^1] Python and Ruby "extend the precision automatically to prevent overflow"[^1].
- **Big rationals and big floats.** Some libraries store "a numerator–denominator pair (rationals)"[^1]. Go's `big.Rat` and GMP's rational type are examples[^6][^8]. A big float has a fixed precision that the program picks, so it still rounds. Go's `big.Float` is an example[^6].
- **Overflow detection.** A program can keep fixed-width integers and detect overflow, then move to a bignum only when it is necessary. Wikipedia describes processors that raise an exception on overflow, after which "the operation could be restarted in software using arbitrary-precision arithmetic"[^1]. See [[integer-overflow]] and [[overflow-semantics]].
- **Symbolic computation.** A computer algebra system keeps an expression such as π·sin(2) as an expression, which is a different method from arbitrary-precision arithmetic[^1].

## Pitfalls

- **Precision lost on conversion.** A JavaScript program that converts a large `bigint` to a number can lose digits: "the precision of a BigInt value may be lost when it is coerced to a Number value"[^4].
- **JSON.** In JavaScript, "Using JSON.stringify() with any BigInt value will raise a TypeError"[^4]. A program must convert big values to strings, or give a `toJSON` method, before it writes JSON.
- **Operations that are not constant-time.** MDN says: "The operations supported on BigInt values are not constant-time and are thus open to timing attacks."[^4] Do not use a general bignum type for secret keys without a library made for cryptography.
- **A fixed-width step in the middle.** A value that passes through an `int` or a `long` before it becomes a bignum can overflow first. In Java, `BigInteger.valueOf(a * b)` multiplies two `long` values before the conversion. Convert first, then multiply.
- **Unbounded input.** A number that comes from a user can be very large. The CPython limit on string conversion exists for that reason[^2], and other languages have no such limit.

## Cost

Wikipedia says that arbitrary-precision arithmetic "is considerably slower than arithmetic using numbers that fit entirely within processor registers"[^1], because the hardware does fixed-width arithmetic and the software must do bignum arithmetic. For *N* digits, addition and subtraction take O(*N*) time[^1]. The school method of multiplication takes Θ(*N*^2) operations[^1]. Faster methods exist: the Karatsuba algorithm reduces the work to about *N*^1.58 digit products[^9], and the Schönhage–Strassen algorithm, based on fast Fourier transforms, is faster again for very large *N*[^1]. In GMP, "Different algorithms are used for different operand sizes"[^8]. The Java documentation warns that "the algorithmic complexity of the methods of this class varies and may be superlinear in the size of the input"[^3].

Memory also grows with the value. Each number needs a separate allocation for its digits, so a bignum costs more memory than a value of 8 bytes even when the value is small.

A program needs bignums only when its values can leave the fixed range. Wikipedia says that in many programs "the task or the programmer can guarantee that the integer values in a specific application will not grow large enough to cause an overflow"[^1]. A 64-bit integer is enough for a count of items or a size in bytes. Bignums are necessary for public-key cryptography, "whose algorithms commonly employ arithmetic with integers having hundreds of digits"[^1], in programs where "artificial limits and overflows would be inappropriate"[^1], and to check the results of fixed-precision calculations[^1].

## History

IBM's first business computer, the IBM 702 of the mid-1950s, did integer arithmetic in hardware "on digit strings of any length from 1 to 511 digits"[^1]. "The earliest widespread software implementation of arbitrary-precision arithmetic was probably that in Maclisp."[^1] Anatoly Karatsuba found his fast multiplication algorithm in 1960, and it was published in 1962[^9]. The first GMP release was in 1991[^8].

## References
