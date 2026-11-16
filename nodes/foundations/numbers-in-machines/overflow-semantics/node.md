---
tag: JV2T7E
kind: concept
slug: overflow-semantics
name: Integer overflow behaviour
aliases: [overflow semantics, overflow behavior, wrapping arithmetic, wraparound, checked arithmetic, saturating arithmetic, saturation arithmetic, trapping arithmetic, signed overflow]
area: foundations/numbers-in-machines
level: core
summary: >-
  When an integer result does not fit in its type, a language wraps it, traps, saturates it at the limit, or leaves the behavior undefined. Most languages have a default rule and ways to choose another.
applies_to: core-languages
viz: optional
formal: none
see_also: [integer-representation, integer-overflow, twos-complement, signed-and-unsigned-integers, binary-number, type-conversion]
sources:
  - url: https://en.wikipedia.org/wiki/Integer_overflow
    title: Integer overflow, Wikipedia
    locator: the introduction; Inconsistent behavior; Integer overflow handling in various programming languages; Detection; Examples
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Integer overflow
    accessed: 2026-11-16
  - url: https://docs.oracle.com/javase/specs/jls/se27/html/jls-4.html
    title: "The Java Language Specification, Java SE 27 Edition, Chapter 4: Types, Values, and Variables"
    locator: 4.2.2 Integer Operations
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/arithmetic-operators
    title: Arithmetic operators, C# reference, Microsoft Learn
    locator: Arithmetic overflow and division by zero, Integer arithmetic overflow
    accessed: 2026-11-16
  - url: https://en.cppreference.com/c/language/operator_arithmetic
    title: Arithmetic operators, C reference, cppreference.com
    locator: Overflows
    accessed: 2026-11-16
  - url: https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html
    title: Options for Code Generation Conventions, Using the GNU Compiler Collection (GCC)
    locator: -ftrapv; -fwrapv
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/reference/behavior-not-considered-unsafe.html
    title: Behavior not considered unsafe, The Rust Reference
    locator: Integer overflow
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/reference/expressions/operator-expr.html
    title: Operator expressions, The Rust Reference
    locator: Overflow
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/reference/profiles.html
    title: Profiles, The Cargo Book
    locator: overflow-checks; Default profiles, dev and release
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/compiler-options/language
    title: C# Compiler Options for language feature rules, Microsoft Learn
    locator: CheckForOverflowUnderflow
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/lang/Math.html
    title: Math, Java SE 27 API
    locator: addExact
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/Saturation_arithmetic
    title: Saturation arithmetic, Wikipedia
    locator: the introduction; Implementations
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/primitive.i32.html
    title: i32, Rust standard library
    locator: checked_add; wrapping_add; overflowing_add; saturating_add
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/numeric/add_sat
    title: std::saturating_add, C++ reference, cppreference.com
    locator: the synopsis and the description
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/stdtypes.html
    title: Built-in Types, Python 3 documentation
    locator: Numeric Types, int, float, complex
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/MAX_SAFE_INTEGER
    title: Number.MAX_SAFE_INTEGER, MDN Web Docs
    locator: the description and the example
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/BigInt/asIntN
    title: BigInt.asIntN(), MDN Web Docs
    locator: the description
    accessed: 2026-11-16
  - url: https://www.postgresql.org/docs/current/datatype-numeric.html
    title: Numeric Types, PostgreSQL documentation
    locator: 8.1.1 Integer Types
    accessed: 2026-11-16
  - url: https://en.cppreference.com/c/numeric/ckd_add
    title: ckd_add, C reference, cppreference.com
    locator: the synopsis and the description
    accessed: 2026-11-16
  - url: https://gcc.gnu.org/onlinedocs/gcc/Integer-Overflow-Builtins.html
    title: Built-in Functions to Perform Arithmetic with Overflow Checking, Using the GNU Compiler Collection (GCC)
    locator: __builtin_add_overflow
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_arithmetic_operators
    title: about_Arithmetic_Operators, PowerShell documentation, Microsoft Learn
    locator: Adding and multiplying non-numeric types; the type of the result
    accessed: 2026-11-16
---

## Idea

An integer type with a fixed width holds a fixed range of values. A 32-bit signed integer holds -2147483648 to 2147483647. The sum 2147483647 + 1 is outside that range, so the hardware cannot give the exact result in 32 bits. This is *integer overflow*. Each language must have a rule for it, and the rules are not the same. Wikipedia lists the choices in a table: for signed integers, Ada raises `Constraint_Error`, C and C++ have "undefined behavior", Java wraps "modulo power of two", and Swift "Causes error unless using special overflow operators."[^1]

There are four main rules and one way to avoid the problem:

- **Wrap.** Keep the low *N* bits of the exact result. The result is correct modulo 2ᴺ, so 2147483647 + 1 gives -2147483648.
- **Trap.** Stop the operation with an error, an exception or a panic.
- **Saturate.** Give the largest or the smallest value of the type, whichever is nearer to the exact result.
- **Undefined.** The language makes no promise, and the compiler may assume that overflow never happens.
- **Grow.** The integer type has no fixed width, so the exact result always fits.

```python run id=policies
LO, HI = -128, 127  # the range of an 8-bit signed integer


def wrap(x):
    return (x - LO) % 256 + LO


def saturate(x):
    return max(LO, min(HI, x))


def trap(x):
    if not LO <= x <= HI:
        raise OverflowError(f"{x} does not fit in 8 bits")
    return x


for a, b in ((100, 27), (100, 28), (-100, -29), (127, 127)):
    exact = a + b
    try:
        trapped = trap(exact)
    except OverflowError:
        trapped = "error"
    print(f"{a} + {b}: exact {exact}, wrap {wrap(exact)}, "
          f"saturate {saturate(exact)}, trap {trapped}")
```

```output for=policies
100 + 27: exact 127, wrap 127, saturate 127, trap 127
100 + 28: exact 128, wrap -128, saturate 127, trap error
-100 + -29: exact -129, wrap 127, saturate -128, trap error
127 + 127: exact 254, wrap -2, saturate 127, trap error
```

## How it works

**Wrap.** A binary adder that gives *N* bits gives the wrapped result for free. For unsigned values, the Go specification says that the operations "discard high bits upon overflow"[^2]. For signed values, Go says that `+`, `-`, `*`, `/` and `<<` "may legally overflow", that "Overflow does not cause a run-time panic", and that "A compiler may not optimize code under the assumption that overflow does not occur."[^2] Java wraps too: "The integer operators do not indicate overflow or underflow in any way."[^3] C# wraps in an unchecked context, where "the result is truncated by discarding any high-order bits that don't fit in the destination type."[^4]

**Undefined.** C and C++ wrap unsigned values. For C, "adding one to UINT_MAX gives 0"[^5]. Signed overflow is different. In C, "When signed integer arithmetic operation overflows (the result does not fit in the result type), the behavior is undefined: it may wrap around according to the rules of the representation (typically 2's complement), it may trap on some platforms or due to compiler options (e.g. -ftrapv in GCC and Clang), or may be completely optimized out by the compiler."[^5] The rule lets the compiler simplify code. For example, it can assume that `x + 1 > x` is always true for a signed `x`. Compiler options change the rule. In GCC, `-ftrapv` "generates traps for signed overflow on addition, subtraction, multiplication operations"[^6], and `-fwrapv` "instructs the compiler to assume that signed arithmetic overflow of addition, subtraction and multiplication wraps around using twos-complement representation."[^6]

**Trap.** Rust treats overflow as a bug: "If a program contains arithmetic overflow, the programmer has made an error."[^7] Its reference says: "Integer operators will panic when they overflow when compiled in debug mode."[^8] Other builds may panic or wrap[^7]. Cargo turns the checks on and off with the `overflow-checks` setting, which is `true` in the `dev` profile and `false` in the `release` profile[^9]. In C#, a `checked` context makes overflow throw: "an OverflowException is thrown."[^4] The compiler option `CheckForOverflowUnderflow` makes checked the default context, and "The default value for this option is false"[^10]. Java has no checked context, but `Math.addExact` "Returns the sum of its arguments, throwing an exception if the result overflows" an `int`[^11].

**Saturate.** "Saturation arithmetic is a version of arithmetic in which all operations, such as addition and multiplication, are limited to a fixed range between a minimum and maximum value."[^12] It is common in signal processing: "adjusting the volume level of a sound signal can result in overflow, and saturation causes significantly less distortion to the sound than wrap-around."[^12] Rust's `saturating_add` computes the sum "saturating at the numeric bounds instead of overflowing"[^13]. C++26 adds `std::saturating_add` in `<numeric>`[^14].

**Grow.** In Python, "Integers have unlimited precision."[^15] JavaScript numbers are floats, so a large sum is rounded, not wrapped. MDN shows that `Number.MAX_SAFE_INTEGER + 1` and `Number.MAX_SAFE_INTEGER + 2` are equal[^16]. The JavaScript `BigInt` type grows, and `BigInt.asIntN` "truncates a BigInt value to the given number of least significant bits and returns that value as a signed integer"[^17], which is a wrap on request.

**Databases.** SQL databases usually reject the value. PostgreSQL says: "Attempts to store values outside of the allowed range will result in an error."[^18]

**Operations that choose the rule.** Many languages let the program choose the rule for one operation:

| Rule | C | C++ | C# | Java | Rust |
|---|---|---|---|---|---|
| wrap | unsigned types | unsigned types | `unchecked` | the operators | `wrapping_add` |
| trap or report | `ckd_add` (C23) | compiler builtins | `checked` | `Math.addExact` | `checked_add`, `overflowing_add` |
| saturate | by hand | `std::saturating_add` (C++26) | by hand | by hand | `saturating_add` |

C23 `ckd_add` stores the wrapped result and returns true when it is not "the mathematical result of the operation"[^19]. GCC and Clang have `__builtin_add_overflow`, and "these built-in functions have fully defined behavior for all argument values"[^20]. Rust's `overflowing_add` returns "a tuple of the addition along with a boolean indicating whether an arithmetic overflow would occur"[^13], and `checked_add` returns `None` on overflow[^13].

## In languages

| Language | Verdict | Default rule and other choices |
|---|---|---|
| Python | idiom | `int` grows; wrap by hand with a mask |
| JavaScript | idiom | `Number` rounds; `BigInt` grows; `BigInt.asIntN` wraps; `\| 0` wraps to 32 bits |
| TypeScript | idiom | as JavaScript |
| Java | stdlib | wraps; `Math.addExact` throws |
| Kotlin | idiom | wraps as in Java; `Math.addExact` on the JVM |
| C# | native | wraps in `unchecked`; throws in `checked` |
| Go | idiom | wraps; checks by hand |
| Rust | native | panics in debug builds; `checked_`, `wrapping_`, `saturating_` and `overflowing_` methods |
| C | stdlib | unsigned wraps; signed is undefined; `ckd_add` in C23 |
| C++ | stdlib | unsigned wraps; signed is undefined; `std::saturating_add` in C++26 |
| Bash | idiom | fixed-width arithmetic with no overflow check |
| PowerShell | idiom | widens the result to a larger type |
| SQL | native | overflow is an error |
| Regex | n/a | no arithmetic |
| HTML | n/a | no arithmetic |
| CSS | n/a | no integer types with a width |

The verdict says how directly a language lets a program choose the rule. C# and Rust have the choices in the language. Java, C and C++ have them in the standard library. Python, JavaScript and Go need code by hand for some rules. PowerShell avoids overflow in a different way: "If the result of an operation is too large for the type, the type of the result is widened to accommodate the result"[^21].

## Variations and related ideas

- **Overflow in conversions.** A conversion to a narrower type can lose the value too. See [[type-conversion]].
- **The security weakness.** An overflow that goes unnoticed can make a buffer too small. See [[integer-overflow]].
- **Two's complement.** The wrapped result of a signed operation is the two's complement result. See [[twos-complement]].
- **Detection with sanitizers.** "UBSan (undefined behavior sanitizer) is available for C compilers."[^1] It reports signed overflow when the program runs.

## Pitfalls

- **A check after the overflow in C or C++.** A check such as `if (a + b < a)` for a signed `a` can be removed by the compiler, because signed overflow is undefined[^5]. Check before the operation, or use `ckd_add` or a builtin.
- **Different builds, different results in Rust.** A debug build panics, and a release build has no overflow checks by default, so it can wrap silently[^7][^9]. A test that runs only debug builds does not show what the release build does.
- **The most negative value.** `-INT_MIN` and `INT_MIN / -1` overflow. In C#, even an unchecked context can throw an `ArithmeticException` for `int.MinValue / -1`[^4].
- **JavaScript rounding.** A sum above `Number.MAX_SAFE_INTEGER` does not wrap and does not throw. It is rounded to a near float[^16].
- **Mean of two values.** `(a + b) / 2` can overflow when the mean fits. Wikipedia gives this as a common example: "Taking the arithmetic mean of two numbers by adding them and dividing by two"[^1]. When `a` and `b` are both 0 or more and `a <= b`, use `a + (b - a) / 2`.

## Cost

Wrapping costs nothing, because it is what the adder does. A trap or a check adds a test of the overflow flag after the operation, which is a branch that is nearly always not taken. The cost of checks is the reason that Cargo turns them off in the release profile by default[^9]. Saturation needs a compare and a select in software, but some processors have it in hardware: saturation "was one of the extensions made by the Intel MMX extension, specifically for such signal-processing applications."[^12] Growth has the highest cost, because each value can need memory on the heap.

## History

Overflow has caused well-known failures. "An unhandled arithmetic overflow in the engine steering software was the primary cause of the crash of the 1996 maiden flight of the Ariane 5 rocket."[^1] In 2015, the U.S. Federal Aviation Administration said that it would order Boeing 787 operators to reset the electrical system periodically, "to avoid an integer overflow which could lead to loss of electrical power and ram air turbine deployment"[^1]. Wikipedia says that "In Java 8, there are overloaded methods, for example Math.addExact(int, int)"[^1]. The standard checked operations of C came in C23 with `<stdckdint.h>`[^19], and saturating operations come to C++ in C++26[^14].

## References
