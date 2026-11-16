---
tag: AFWVZY
kind: concept
slug: complex-number-type
name: Complex numbers in languages
aliases: [complex type, complex number type, complex data type, complex128, complex64, double complex, _Complex, std::complex, System.Numerics.Complex, num-complex, imaginary literal, cmath]
area: data-and-values/math-in-code
level: core
summary: >-
  A complex number type holds a real part and an imaginary part, usually two floats. Some languages have it built in, some in the library and some not at all.
applies_to: core-languages
viz: optional
formal: none
see_also: [numeric-type, do-complex-number-arithmetic, use-mathematical-constants-and-functions, floating-point-arithmetic]
same_as:
  wikidata: Q4230065
  wikipedia: Complex_data_type
sources:
- url: https://en.wikipedia.org/wiki/Complex_data_type
  title: Complex data type, Wikipedia
  locator: Introduction; Complex-number arithmetic; Language support
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, Python documentation
  locator: Numeric Types - int, float, complex
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Numeric types; Imaginary literals
- url: https://en.cppreference.com/w/cpp/numeric/complex.html
  title: std::complex, C++ reference, cppreference.com
  locator: Template parameters; Array-oriented access; Non-member functions
- url: https://docs.python.org/3/library/cmath.html
  title: cmath, Mathematical functions for complex numbers, Python documentation
  locator: Introduction
- url: https://pkg.go.dev/math/cmplx
  title: Go standard library, math/cmplx
  locator: Overview
- url: https://docs.rs/num-complex/latest/num_complex/struct.Complex.html
  title: Complex in num_complex, docs.rs
  locator: Representation and Foreign Function Interface Compatibility
- url: https://en.cppreference.com/w/c/numeric/complex.html
  title: Complex number arithmetic, C reference, cppreference.com
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/api/system.numerics.complex?view=net-10.0
  title: Complex Struct, .NET API reference
  locator: Remarks
---

## Idea

A complex number has two parts: a real part `a` and an imaginary part `b`, written `a + bi`, where `i` times `i` is -1. Programs for signal processing, electrical circuits, physics and fractal images compute with complex numbers. A complex number type holds the two parts in one value and defines `+`, `-`, `*` and `/` on them, so the code looks like the mathematics.

"A complex variable or value is usually represented as a pair of floating-point numbers"[^1]. Python has the type as a built-in, with a literal that ends in `j`:

```python run id=idea
import cmath

z = 3 + 4j
w = complex(1, -2)
print(z.real, z.imag, abs(z))
print(z + w, z * w, z / w)
print(z.conjugate(), 1j * 1j)
print(cmath.sqrt(-1), cmath.phase(-1))
```

```output for=idea
3.0 4.0 5.0
(4+2j) (11-2j) (-1+2j)
(3-4j) (-1+0j)
1j 3.141592653589793
```

`z.real` and `z.imag` are the two parts, and `abs(z)` is the distance from 0, which is 5 for `3 + 4j`. The product and the quotient follow the rules of complex arithmetic. `1j * 1j` is -1, and the square root of -1 is `1j`. The module `cmath` holds the functions for complex numbers.

## How it works

**Two floats.** Most complex types are a pair of floating-point numbers in Cartesian form. In Python, "Complex numbers have a real and imaginary part, which are each a floating-point number"[^2]. Go has two built-in types: `complex64`, "the set of all complex numbers with float32 real and imaginary parts", and `complex128`, "the set of all complex numbers with float64 real and imaginary parts"[^3]. A `complex128` value uses 16 bytes.

**Arithmetic.** Languages with a complex type "extend the basic arithmetic operations ('+', '−', '×', '÷') to act on them"[^1]. The rules are:

- `(a + bi) + (c + di)` is `(a + c) + (b + d)i`.
- `(a + bi) * (c + di)` is `(ac - bd) + (ad + bc)i`, because `i` times `i` is -1.
- `(a + bi) / (c + di)` multiplies the top and the bottom by the conjugate `c - di`, so the bottom becomes the real number `c * c + d * d`.

A multiplication takes 4 float multiplications and 2 additions. A division takes more, and a good library also scales the numbers to prevent overflow, as the pitfalls show.

**Literals and construction.** In Python, "Appending 'j' or 'J' to a numeric literal yields an imaginary number"[^2], so `3 + 4j` is a complex number. In Go, "An imaginary literal represents the imaginary part of a complex constant"[^3], with the letter `i`, as in `3 + 4i`. C has the macro `I` in `<complex.h>`, so `3.0 + 4.0 * I` is a `double complex`. C++ has the literals `1.0i` in `std::complex_literals`, which came in C++14[^4]. C# has no literal: `new Complex(3, 4)` builds the value.

**Polar form.** A complex number is also a length and an angle: the absolute value (or magnitude) and the phase (or argument). Each library has both: `abs` and `cmath.phase` in Python, `std::abs` and `std::arg` in C++, `cabs` and `carg` in C, `cmplx.Abs` and `cmplx.Phase` in Go, and `Magnitude` and `Phase` in C#. The functions `cmath.polar` and `cmath.rect`, and `std::polar` in C++, convert between the two forms.

**Functions in a separate module.** The real math functions do not accept complex numbers, so each language has a second set. Python's `cmath` "provides access to mathematical functions for complex numbers"[^5]. Go's `math/cmplx` "provides basic constants and mathematical functions for complex numbers"[^6]. C has `csqrt`, `cexp` and the other functions with the prefix `c`, and C++ overloads `std::sqrt` and `std::exp` for `std::complex`.

**Memory layout.** The two parts are next to each other in memory, real part first. In C++, "For any object z of type std::complex<T>, reinterpret_cast<T(&)[2]>(z)[0] is the real part of z and reinterpret_cast<T(&)[2]>(z)[1] is the imaginary part of z"[^4]. In Rust, the `Complex<T>` type of the crate `num-complex` "is memory layout compatible with an array [T; 2]"[^7]. So an array of complex numbers can go to a C or Fortran library, such as a fast Fourier transform, with no copy.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `complex`, literals with `j`, the module `cmath` |
| JavaScript | absent | no complex type; an object with two numbers, or a library |
| TypeScript | absent | no complex type, as in JavaScript |
| Java | absent | no complex type in the standard library; a record of two `double` values, or a library |
| Kotlin | absent | no complex type in the standard library |
| C# | stdlib | `System.Numerics.Complex`, with `double` parts |
| Go | native | `complex64` and `complex128`, imaginary literals with `i`, `real`, `imag`, the package `math/cmplx` |
| Rust | library | `Complex<T>` in the crate `num-complex` |
| C | native | `double complex` and the macro `I` in `<complex.h>`; optional, an implementation can define `__STDC_NO_COMPLEX__`[^8] |
| C++ | stdlib | the class template `std::complex<T>` in `<complex>`; literals `1.0i` |
| Bash | absent | no floating-point arithmetic |
| PowerShell | stdlib | `[System.Numerics.Complex]` from .NET |
| SQL | absent | no complex type in standard SQL |
| Regex | n/a | no numbers |
| HTML | n/a | no numbers |
| CSS | n/a | no complex numbers |

Python, Go and C have the type in the language, with a literal or a macro for `i`. C++ and C# have it in the standard library as a class or a struct. Java, JavaScript and Rust have no complex type in the standard library, so a program uses a library or writes a small type.

The example files show the type in each language. Python shows the parts, the polar form, parsing from a string and `cmath`. C shows `double complex`, `CMPLX`, `cabs` and `csqrt`. C++ shows `std::complex`, the literal `2.0i`, and `std::norm`, which is the square of the absolute value. C# shows `System.Numerics.Complex` and its output format. Go shows `complex128`, the functions `real` and `imag`, and `math/cmplx`.

## Variations and related ideas

- **Polar storage.** A type can store the length and the angle in place of the two parts. Multiplication is then cheap, but addition is not, so the common types use the Cartesian form and convert when needed.
- **Integer complex numbers.** Complex numbers with integer parts (Gaussian integers) are rare in libraries. In C++, the behavior of `std::complex<T>` "is unspecified (and may fail to compile)" if `T` is not a floating-point type[^4]. Go has no integer complex type. The Rust type `Complex<T>` accepts integer types such as `Complex<i32>`.
- **Imaginary types.** C99 also defines the optional type `double imaginary` for numbers with no real part, but few compilers have it.
- **Quaternions.** Quaternions extend complex numbers to four parts for 3D rotation. They are in graphics libraries, not in the standard libraries of the tier 1 languages.

## Pitfalls

- **The real square root does not accept a negative number.** `math.sqrt(-1)` in Python raises `ValueError`, and `sqrt(-1.0)` in C returns NaN. Use the complex function, `cmath.sqrt` or `csqrt`:

```python wrong id=real-sqrt
import cmath
import math

try:
    math.sqrt(-1)
except ValueError as e:
    print(type(e).__name__)
print(cmath.sqrt(-1))
```

```output for=real-sqrt
ValueError
1j
```

- **Complex numbers have no order.** `<` and `>` have no meaning for complex numbers, so Python raises `TypeError`, and Go and C++ do not compile the comparison. Compare the absolute values, or the real parts, when an order is necessary:

```python wrong id=no-order
try:
    print(1j < 2j)
except TypeError as e:
    print(type(e).__name__)
print(sorted([3 + 4j, 1 - 1j, 2j], key=abs))
```

```output for=no-order
TypeError
[(1-1j), 2j, (3+4j)]
```

- **The simple division formula overflows.** The formula with `c * c + d * d` overflows for large parts, although the quotient is small. A library scales the numbers first:

```python wrong id=naive-division
def naive_divide(z, w):
    a, b, c, d = z.real, z.imag, w.real, w.imag
    den = c * c + d * d
    return complex((a * c + b * d) / den, (b * c - a * d) / den)


z = complex(1e300, 1e300)
print(naive_divide(z, z))
print(z / z)
```

```output for=naive-division
(nan+nanj)
(1+0j)
```

- **Exact comparison of results.** The parts are floats, so a result is often not exact. `cmath.exp(1j * cmath.pi)` is not exactly -1. Compare with a tolerance, such as `cmath.isclose`.
- **`std::norm` is not the length.** In C++, `std::norm(z)` is the square of the absolute value, so `std::norm` of `3 + 4i` is 25, not 5. Use `std::abs` for the length.
- **`j` alone is a name.** In Python, `j` is a variable name, and `1j` is the imaginary unit. `2 * j` uses a variable `j`. In Go, `i` alone is also a name, and `1i` is the imaginary unit.
- **The output format differs.** Python prints `(3+4j)`, Go prints `(3+4i)`, C++ prints `(3,4)` and C# prints `<3; 4>`. Do not parse one format with the parser of another language.

## Cost

A complex value takes the space of two floats: 16 bytes for two 64-bit floats. Addition is two float additions. Multiplication is four float multiplications and two additions, and division needs a division and, in a careful library, a scaling step. The complex functions such as `sqrt` and `exp` call several real functions each.

## History

Fortran has a `COMPLEX` type, and the Wikipedia list of languages with a complex type starts with it[^1]. C added complex types in C99: "The C programming language, as of C99, supports complex number math with the three built-in types double _Complex, float _Complex, and long double _Complex"[^8]. C11 made the support optional: if the macro `__STDC_NO_COMPLEX__` is defined, "the complex types, the header <complex.h> and all of the names listed here are not provided"[^8]. C++ has the class template `std::complex` in its standard library, and Go made `complex64` and `complex128` built-in types[^3]. .NET added the struct `Complex` in `System.Numerics`, and its "Complex type uses the Cartesian coordinate system (real, imaginary) when instantiating and manipulating complex numbers"[^9].

## References
