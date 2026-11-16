---
tag: 1FVD97
kind: task
slug: do-complex-number-arithmetic
name: Do complex number arithmetic
aliases: [complex arithmetic, add complex numbers, multiply complex numbers, divide complex numbers, complex division, complex conjugate, complex absolute value, complex phase, argument of a complex number, cmath, math/cmplx, std::complex, System.Numerics.Complex]
area: data-and-values/math-in-code
level: core
summary: >-
  Add, multiply and divide two complex numbers, and find the absolute value and the phase. Use the built-in or library type, or a small type of your own.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Arithmetic-Complex/00-TASK.txt
  title: Rosetta Code, Arithmetic/Complex (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://en.wikipedia.org/wiki/Complex_number
  title: Wikipedia, Complex number
  locator: Complex conjugate, absolute value, argument and division
- url: https://docs.python.org/3/library/cmath.html
  title: The Python Standard Library, cmath
  locator: phase
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Math.html
  title: Java SE 25 API, Class Math
  locator: hypot(double, double)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.numerics.complex?view=net-10.0
  title: Complex Struct, .NET API reference
  locator: Properties; Remarks
- url: https://pkg.go.dev/math/cmplx
  title: Go standard library, math/cmplx
  locator: func Phase
- url: https://en.cppreference.com/w/c/language/operator_arithmetic.html
  title: cppreference.com, Arithmetic operators (C)
  locator: Multiplicative operators, complex division
---

## Task

Add, multiply and divide two complex numbers, and find the absolute value and the phase of a complex number. The Rosetta Code task says: "Show addition, multiplication, negation, and inversion of complex numbers in separate functions"[^1]. It also says: "Some languages have complex number libraries available"[^1]. If a language has a complex type, use it. If it does not, define a small type.

The function `complex_arithmetic(a, b)` takes two complex numbers as pairs `[re, im]`. It returns a list of 8 numbers: the sum `a + b`, the product `a * b` and the quotient `a / b`, each as a real part and an imaginary part, then the absolute value of `a` and the phase of `a`. For `a = 3 + 4i` and `b = 1 - 2i`, the result is `[4, 2, 11, -2, -1, 2, 5, 0.927...]`.

The rules are:

- `(a + bi) + (c + di)` is `(a + c) + (b + d)i`.
- `(a + bi) * (c + di)` is `(ac - bd) + (ad + bc)i`, because `i` times `i` is -1.
- `(a + bi) / (c + di)` multiplies the top and the bottom by the conjugate `c - di`. The bottom becomes the real number `c * c + d * d`.
- The absolute value of `a + bi` is the square root of `a * a + b * b`, the distance from 0.
- The phase (or argument) is the angle from the positive real axis, `atan2(b, a)`. "One possible choice to uniquely specify the argument (also known as choosing a principal value) is to require it to be within the interval" from -π to π[^2].

Subtraction and the reciprocal follow from these rules: `a - b` is `a + (-b)`, and `1 / b` is the quotient with `a = 1`.

## Solutions

| Language | Complex type | Division |
|---|---|---|
| Python | the built-in `complex`, and `cmath.phase` | the operator `/` |
| JavaScript, TypeScript | none; functions on pairs `[re, im]` | a scaled formula |
| Java | none; a `record` with two `double` fields | a scaled formula |
| C# | `System.Numerics.Complex` | the operator `/` |
| Go | the built-in `complex128`, and `math/cmplx` | the operator `/` |
| Rust | none in the standard library; a `struct` with two `f64` fields | a scaled formula |
| C | `double complex` in `<complex.h>` | the operator `/` |
| C++ | `std::complex<double>` in `<complex>` | the operator `/` |
| Bash | none; `awk` does the floating-point work | a scaled formula |

- **Python.** `complex(*a)` makes a complex number from a pair. `abs(z)` is the absolute value, and `cmath.phase(z)` is the phase. "The result lies in the range [-π, π]"[^3].
- **JavaScript.** There is no complex type. The solution has functions on arrays of two numbers. `Math.hypot` gives the absolute value and `Math.atan2` gives the phase.
- **TypeScript.** The same functions as JavaScript, with the type `[number, number]`.
- **Java.** There is no complex type in the standard library. The solution defines a `record Complex(double re, double im)` with the methods `plus`, `times`, `div`, `abs` and `phase`. `Math.hypot` "Returns sqrt(x2 +y2) without intermediate overflow or underflow"[^4].
- **C#.** `System.Numerics.Complex` has the operators and the properties `Magnitude` and `Phase`. `Phase` "Gets the phase of a complex number"[^5].
- **Go.** `complex(re, im)` makes a `complex128`, and `real` and `imag` get the parts back. `cmplx.Abs` is the absolute value. "Phase returns the phase (also called the argument) of x. The returned value is in the range [-Pi, Pi]."[^6]
- **Rust.** The standard library has no complex type, so the solution defines a `struct` and implements the traits `Add`, `Mul` and `Div` for it. The crate `num-complex` has a complete type.
- **C.** `CMPLX(re, im)` makes a `double complex`. `creal` and `cimag` get the parts, and `cabs` and `carg` give the absolute value and the phase.
- **C++.** `std::complex<double>` has the operators, and `std::abs` and `std::arg` give the absolute value and the phase.
- **Bash.** Bash arithmetic has only integers, so one `awk` program computes the 8 numbers and prints them.

The Python function:

```python run id=python-complex-arithmetic
import cmath


def complex_arithmetic(a, b):
    z, w = complex(*a), complex(*b)
    s, p, q = z + w, z * w, z / w
    return [s.real, s.imag, p.real, p.imag, q.real, q.imag, abs(z), cmath.phase(z)]


print(complex_arithmetic([3, 4], [1, -2]))
z = 3 + 4j
print(z - (1 - 2j), -z, z.conjugate(), 1 / z)
```

```output for=python-complex-arithmetic
[4.0, 2.0, 11.0, -2.0, -1.0, 2.0, 5.0, 0.9272952180016122]
(2+6j) (-3-4j) (3-4j) (0.12-0.16j)
```

The second line shows the other operations of the Rosetta Code task: subtraction, negation, the conjugate and the reciprocal. The Go type `complex128` has the same operators:

```go run id=go-complex128
package main

import (
	"fmt"
	"math/cmplx"
)

func main() {
	z, w := complex(3, 4), complex(1, -2)
	fmt.Println(z+w, z*w, z/w)
	fmt.Println(real(z), imag(z), cmplx.Abs(z), cmplx.Phase(z))
	fmt.Println(cmplx.Conj(z), -z, 1/z)
}
```

```output for=go-complex128
(4+2i) (11-2i) (-1+2i)
3 4 5 0.9272952180016122
(3-4i) (-3-4i) (0.12-0.16i)
```

In a language with no complex type, the division is the hard part. The formula with `c * c + d * d` can overflow when the parts are large, although the quotient is small. A good library prevents this. For the C operator `/`, "complex division is not allowed to overflow intermediate results"[^7]. Your own division can scale the numbers: divide by the larger part of the divisor first, so no intermediate value is larger than necessary. The JavaScript functions:

```javascript run id=javascript-scaled-division
function div([a, b], [c, d]) {
  if (Math.abs(c) >= Math.abs(d)) {
    const r = d / c;
    const t = c + d * r;
    return [(a + b * r) / t, (b - a * r) / t];
  }
  const r = c / d;
  const t = c * r + d;
  return [(a * r + b) / t, (b * r - a) / t];
}

console.log(div([3, 4], [1, -2]));
console.log(div([1e150, 1e150], [1e155, 1e155]));
console.log(Math.hypot(1e200, 1e200), Math.atan2(4, 3));
```

```output for=javascript-scaled-division
[ -1, 2 ]
[ 0.000009999999999999999, 0 ]
1.414213562373095e+200 0.9272952180016122
```

`r` is the ratio of the smaller part of the divisor to the larger part, so its size is at most 1. The second quotient is 0.00001 with a rounding error in the last digit, as the `large-divisor` case shows. `Math.hypot` computes the absolute value with the same care.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `textbook`: `(3 + 4i)` and `(1 - 2i)`, with small whole-number results.
- `rosetta`: the numbers `1 + i` and `3.14159 + 1.25i` of the Rosetta Code task.
- `real-only`: two real numbers. The results are the real results, with imaginary parts 0.
- `imaginary-unit`: `i` times `i` is -1, and the phase of `i` is π/2.
- `negative-real`: the phase of -2 is π, the end of the range.
- `zero`: the first number is 0. The product, the quotient, the absolute value and the phase are 0.
- `negative-phase`: `1 - i` is below the real axis, so its phase is negative, -π/4.
- `imaginary-divisor`: the divisor `2i` has the real part 0, so the scaled division takes its second branch.
- `large-divisor`: `(1e150 + 1e150i) / (1e155 + 1e155i)` is `1e-5`. The simple formula computes `c * c` as `1e310`, which overflows to infinity.

## Pitfalls

- **The simple formulas overflow.** `c * c + d * d` in the division and `a * a + b * b` in the absolute value overflow for large parts. The results are then 0 or infinity, with no error:

```javascript wrong id=javascript-simple-formulas
function naiveDiv([a, b], [c, d]) {
  const den = c * c + d * d;
  return [(a * c + b * d) / den, (b * c - a * d) / den];
}

console.log(naiveDiv([1e150, 1e150], [1e155, 1e155]));
console.log(Math.sqrt(1e200 * 1e200 + 1e200 * 1e200));
```

```output for=javascript-simple-formulas
[ 0, 0 ]
Infinity
```

The quotient is `1e-5`, not 0, and the absolute value is about `1.414e200`. Use a scaled division and `Math.hypot`, as in the solutions.

- **The sign of zero changes the phase.** On the negative real axis, the phase is π or -π, and the sign of the imaginary part decides. In Python, "The sign of the result is the same as the sign of z.imag, even when z.imag is zero"[^3]. The conjugate of `-1 + 0i` has the imaginary part `-0.0`:

```python wrong id=python-negative-zero-phase
import cmath

z = complex(-1, 0)
print(cmath.phase(z), cmath.phase(z.conjugate()))
print(z.conjugate(), z == z.conjugate())
```

```output for=python-negative-zero-phase
3.141592653589793 -3.141592653589793
(-1-0j) True
```

The two numbers are equal, but their phases are different. If the code needs one value on the negative real axis, change `-0.0` to `0.0` first.

- **Division by zero gives no error.** In Python, `z / 0j` raises `ZeroDivisionError`. In C, C++, Go and C#, the result has infinite or NaN parts, and the program continues. For .NET, "Mathematical operations with complex numbers that are invalid or that overflow the range of the Double data type do not throw an exception"[^5]. Check the divisor first if 0 is possible.
- **Exact comparison of results.** The parts are floats, so a result is often not exact. For the `rosetta` case, the real part of the product is `1.8915899999999999`, not `1.89159`. Compare with a tolerance, as the tests do.
- **The C++ `std::norm` is not the absolute value.** `std::norm(z)` is the square of the absolute value. Use `std::abs` for the distance from 0.
- **The output formats differ.** Python prints `(3+4j)`, Go prints `(3+4i)` and C++ prints `(3,4)`. Print the two parts yourself when a program reads the output.

The C type `double complex` in a short program. The operator `/` scales, so the large case is correct:

```c run id=c-double-complex
#include <complex.h>
#include <stdio.h>

int main(void) {
    double complex z = CMPLX(3, 4), w = CMPLX(1, -2);
    double complex q = z / w;
    printf("%g %g %g %g\n", creal(q), cimag(q), cabs(z), carg(z));
    double complex big = CMPLX(1e150, 1e150) / CMPLX(1e155, 1e155);
    printf("%g %g\n", creal(big), cimag(big));
    return 0;
}
```

```output for=c-double-complex
-1 2 5 0.927295
1e-05 0
```

## Related tasks

- [[complex-number-type]]: the complex types of the languages, and how they store the two parts.
- [[use-mathematical-constants-and-functions]]: `atan2`, `hypot` and π, which the phase and the absolute value use.
- [[do-matrix-arithmetic]]: another task on numbers with a type of its own in some languages.
- [[floating-point-arithmetic]]: why the results are not exact.

## References
