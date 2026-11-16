---
tag: CNJ3Z0
kind: task
slug: use-mathematical-constants-and-functions
name: Use mathematical constants and functions
aliases: [math library, math functions, pi constant, Math.PI, math.pi, square root, power function, trigonometric functions, degrees to radians, natural logarithm, real constants and functions]
area: data-and-values/math-in-code
level: intro
summary: >-
  Use pi, e, the square root, the power, the trigonometric functions and the logarithms of the math library, here to turn a distance and an angle in degrees into x and y coordinates.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Real-constants-and-functions/00-TASK.txt
  title: Rosetta Code, Real constants and functions (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://docs.python.org/3/library/math.html
  title: The Python Standard Library, math
  locator: Constants; Angular conversion; Trigonometric functions; math.isclose; math.pow
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math
  title: MDN Web Docs, Math
  locator: Description; Static properties
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Math.html
  title: Java SE 25 API, Class Math
  locator: toRadians(double)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.math?view=net-10.0
  title: .NET API reference, Math class
  locator: Fields; Methods
- url: https://pkg.go.dev/math
  title: Go standard library, math
  locator: Constants; func Log; func Sqrt
- url: https://doc.rust-lang.org/std/f64/consts/index.html
  title: Rust standard library, module std::f64::consts
  locator: Constants
- url: https://doc.rust-lang.org/std/primitive.f64.html
  title: Rust standard library, primitive type f64
  locator: to_radians
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/math.h.html
  title: The Open Group Base Specifications Issue 8, math.h
  locator: the table of symbolic constants
- url: https://en.cppreference.com/w/cpp/numeric/constants.html
  title: cppreference.com, Mathematical constants
  locator: Constants
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Shell-Arithmetic
  title: Bash Reference Manual
  locator: 6.5 Shell Arithmetic
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/awk.html
  title: The Open Group Base Specifications Issue 8, awk
  locator: Arithmetic Functions
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Bitwise_XOR
  title: MDN Web Docs, Bitwise XOR (^)
  locator: the introduction
---

## Task

Use the constants and functions of the math library of the language: pi, e, the square root, the power, the trigonometric functions and the logarithms. The Rosetta Code task on real constants and functions asks for e, pi, the square root, the logarithm, the exponential, the absolute value, the floor, the ceiling and the power[^1].

The contract uses pi and the trigonometric functions. The function `to_cartesian(r, degrees)` takes a distance `r` and an angle in degrees, measured from the positive x axis in the counterclockwise direction. It returns the point `(x, y)`, with `x = r cos(t)` and `y = r sin(t)`, where `t` is the angle in radians. For example, `to_cartesian(2, 90)` is `(0, 2)`, and `to_cartesian(1, 45)` is about `(0.7071, 0.7071)`.

The trigonometric functions of each library take radians. A full turn is 360 degrees or 2 pi radians, so the conversion is `t = degrees * pi / 180`.

The results are floating-point numbers, and they are not exact. `cos(90 degrees)` gives about 6.1e-17, not 0. So the checker of the contract compares the numbers with an absolute tolerance of 1e-9 or a relative tolerance of 1e-12.

## Solutions

| Language | pi | e | Square root | Power | Sine of an angle in degrees | Natural logarithm |
|---|---|---|---|---|---|---|
| Python | `math.pi` | `math.e` | `math.sqrt(x)` | `x ** y` | `math.sin(math.radians(d))` | `math.log(x)` |
| JavaScript, TypeScript | `Math.PI` | `Math.E` | `Math.sqrt(x)` | `x ** y` | `Math.sin(d * Math.PI / 180)` | `Math.log(x)` |
| Java | `Math.PI` | `Math.E` | `Math.sqrt(x)` | `Math.pow(x, y)` | `Math.sin(Math.toRadians(d))` | `Math.log(x)` |
| C# | `Math.PI` | `Math.E` | `Math.Sqrt(x)` | `Math.Pow(x, y)` | `Math.Sin(d * Math.PI / 180)` | `Math.Log(x)` |
| Go | `math.Pi` | `math.E` | `math.Sqrt(x)` | `math.Pow(x, y)` | `math.Sin(d * math.Pi / 180)` | `math.Log(x)` |
| Rust | `std::f64::consts::PI` | `std::f64::consts::E` | `x.sqrt()` | `x.powf(y)` | `d.to_radians().sin()` | `x.ln()` |
| C | `acos(-1.0)` | `exp(1.0)` | `sqrt(x)` | `pow(x, y)` | `sin(d * pi / 180)` | `log(x)` |
| C++ | `std::numbers::pi` | `std::numbers::e` | `std::sqrt(x)` | `std::pow(x, y)` | `std::sin(d * pi / 180)` | `std::log(x)` |
| Bash | `atan2(0, -1)` in awk | `exp(1)` in awk | `sqrt(x)` in awk | `x ^ y` in awk | `sin(d * pi / 180)` in awk | `log(x)` in awk |

- **Python.** The `math` module has the constants `math.pi`, `math.e`, `math.tau`, `math.inf` and `math.nan`. `math.pi` is "The mathematical constant π = 3.141592…, to available precision"[^2]. `math.radians(x)` and `math.degrees(x)` convert the angles. `math.log(x, base)` takes a base, and `math.log2` and `math.log10` are more exact for the bases 2 and 10.
- **JavaScript.** `Math` is an object with static properties and methods: "Unlike most global objects, Math is not a constructor"[^3]. "Math works with the Number type. It doesn't work with BigInt"[^3]. There is no function for degrees, so the solution multiplies by `Math.PI / 180`.
- **TypeScript.** The same code as JavaScript, with the type `[number, number]` for the result.
- **Java.** `Math.toRadians` "Converts an angle measured in degrees to an approximately equivalent angle measured in radians. The conversion from degrees to radians is generally inexact"[^4].
- **C#.** `Math.PI` "Represents the ratio of the circumference of a circle to its diameter, specified by the constant, π"[^5]. `Math.Log(Double)` "Returns the natural (base e) logarithm of a specified number"[^5], and `Math.Log(Double, Double)` takes a base. The solution returns the tuple `(double X, double Y)`.
- **Go.** The constants of the `math` package are untyped constants with more digits than a `float64` holds: `Pi = 3.14159265358979323846264338327950288419716939937510582097494459`[^6]. The function returns two values.
- **Rust.** The constants are in `std::f64::consts`, where `PI` is "Archimedes' constant (π)"[^7]. The functions are methods of `f64`, so `d.to_radians().sin()` reads from left to right. `to_radians` "Converts degrees to radians"[^8].
- **C.** The C standard library has no constant for pi. POSIX defines `M_PI` in `<math.h>` as the value of pi[^9], but a compiler in strict standard mode can hide it. The solution computes pi as `acos(-1.0)`. On some systems the math functions need the option `-lm` at link time.
- **C++.** The header `<numbers>` has `std::numbers::pi`, a `double` constant, and the template `std::numbers::pi_v<T>` for other types[^10].
- **Bash.** "Evaluation is done in the largest fixed-width integers available"[^11], so Bash has no floating-point numbers. The solution calls awk, which has the arithmetic functions `atan2(y,x)`, `cos(x)`, `sin(x)`, `exp(x)`, `log(x)` and `sqrt(x)`, and `cos(x)` returns the "cosine of x, where x is in radians"[^12]. It prints the coordinates with 6 decimals.

A table of the functions and constants in Python:

```python run id=python-math-functions
import math

print(math.pi, math.e)
print(math.sqrt(2), 2 ** 0.5)
print(math.exp(1), math.log(math.e), math.log10(1000), math.log(8, 2))
print(math.floor(-2.5), math.ceil(-2.5), math.trunc(-2.5), abs(-2.5))
print(math.sin(math.radians(30)), math.degrees(math.pi))
print(math.hypot(3, 4), math.atan2(1, 1) * 4)
```

```output for=python-math-functions
3.141592653589793 2.718281828459045
1.4142135623730951 1.4142135623730951
2.718281828459045 1.0 3.0 3.0
-3 -2 -2 2.5
0.49999999999999994 180.0
5.0 3.141592653589793
```

The sine of 30 degrees is 0.5, but the result is 0.49999999999999994, because `math.radians(30)` is not exactly pi / 6. The floor of -2.5 is -3, and the truncation is -2.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `east`, `north` and `west`: the angles 0, 90 and 180 degrees. The result of `north` has an x of about 6.1e-17, not 0, and the tolerance of the checker accepts it.
- `diagonal` and `thirty-degrees`: the results are irrational numbers, so they are not exact.
- `zero-radius`: a distance of 0 gives the origin for each angle.
- `south` and `negative-angle`: 270 degrees and -90 degrees give the same point.
- `full-turn` and `two-turns`: 360 and 720 degrees give the same point as 0 degrees.
- `negative-radius`: a negative distance gives the point on the other side of the origin.
- `large-radius`: the relative tolerance accepts the error in the last digits of large results.

## Pitfalls

- **The trigonometric functions take radians.** A call with an angle in degrees gives a wrong result and no error. `math.sin(90)` is the sine of 90 radians:

```python wrong id=python-sin-of-degrees
import math

print(math.sin(90))
print(math.sin(math.radians(90)))
```

```output for=python-sin-of-degrees
0.8939966636005579
1.0
```

- **A result that should be 0 is a small number.** pi is not exact in a `float`, so `cos(pi / 2)` is not 0. Do not compare floating-point results with `==`. Compare them with a tolerance, such as `math.isclose`. For a comparison with 0, give an absolute tolerance, because "isclose(x, 0) is computed as abs(x) <= rel_tol * abs(x), which is False for any nonzero x"[^2]:

```python run id=python-isclose-zero
import math

x = math.cos(math.pi / 2)
print(x)
print(x == 0, math.isclose(x, 0), math.isclose(x, 0, abs_tol=1e-9))
```

```output for=python-isclose-zero
6.123233995736766e-17
False False True
```

- **`^` is not the power operator.** In C, C++, Java, C#, Go, Rust and JavaScript, `^` is the bitwise exclusive or. In JavaScript, it "returns a number or BigInt whose binary representation has a 1 in each bit position for which the corresponding bits of either but not both operands are 1"[^13]. So `2 ^ 3` is 1. Use `**` in Python and JavaScript, and the power function in the other languages:

```javascript wrong id=javascript-caret-is-xor
console.log(2 ^ 3);
console.log(2 ** 3, Math.pow(2, 3));
```

```output for=javascript-caret-is-xor
1
8 8
```

- **A floating-point power of integers loses digits.** `math.pow` and `pow` in C, Java, C# and Go convert the arguments to `double`, which holds 53 bits. Python says: "Unlike the built-in ** operator, math.pow() converts both its arguments to type float. Use ** or the built-in pow() function for computing exact integer powers"[^2]. 3 to the power 39 needs 62 bits, so the `float` result is wrong in the last digits:

```python wrong id=python-float-pow
import math

print(3 ** 39)
print(int(math.pow(3, 39)))
```

```output for=python-float-pow
4052555153018976267
4052555153018976256
```

The same is true in Go, where `math.Pow` returns a `float64`:

```go wrong id=go-float-pow
package main

import (
	"fmt"
	"math"
)

func main() {
	exact := int64(1)
	for range 39 {
		exact *= 3
	}
	fmt.Println(exact)
	fmt.Println(int64(math.Pow(3, 39)))
}
```

```output for=go-float-pow
4052555153018976267
4052555153018976256
```

- **A value outside the domain gives an error or NaN.** The logarithm of 0 or of a negative number, and the square root of a negative number, have no real result. Python raises an error: "The current implementation will raise ValueError for invalid operations like sqrt(-1.0) or log(0.0)"[^2]. Go returns special values: "Log(0) = -Inf", "Log(x < 0) = NaN" and "Sqrt(x < 0) = NaN"[^6]. Check the argument first, or check the result with `math.IsNaN` and `math.IsInf`:

```python run id=python-domain-error
import math

for f, x in [(math.log, 0.0), (math.sqrt, -1.0)]:
    try:
        f(x)
    except ValueError as e:
        print(f.__name__, type(e).__name__)
```

```output for=python-domain-error
log ValueError
sqrt ValueError
```

```go run id=go-domain-special-values
package main

import (
	"fmt"
	"math"
)

func main() {
	fmt.Println(math.Log(0), math.Log(-1), math.Sqrt(-1))
	fmt.Println(math.IsNaN(math.Sqrt(-1)), math.IsInf(math.Log(0), -1))
}
```

```output for=go-domain-special-values
-Inf NaN NaN
true true
```

- **Integer arguments in C.** In C, `sqrt(2)` converts the 2 to `double`, but `1 / 2 * pi` is 0, because `1 / 2` is an integer division. Write the constants as `1.0 / 2`. See [[integer-division-and-modulo]].

## Related tasks

- [[sum-and-multiply-a-list]]: the sum and the product of many numbers.
- [[compute-the-mean-and-median]]: the mean needs a floating-point division.
- [[do-complex-number-arithmetic]]: the absolute value and the phase of a complex number use the square root and `atan2`.
- [[generate-random-numbers]]: random numbers are the other part of the math library.

See [[numeric-type]] for the types of numbers, and [[numerical-pitfalls]] for the errors of floating-point arithmetic.

## References
