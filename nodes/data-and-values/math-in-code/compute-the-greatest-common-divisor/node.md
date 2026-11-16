---
tag: 7JSP23
kind: task
slug: compute-the-greatest-common-divisor
name: Compute the greatest common divisor
aliases: [gcd, greatest common factor, highest common factor, lcm, least common multiple, Euclid's algorithm, math.gcd, math.lcm, std::gcd, std::lcm, BigInteger.gcd]
area: data-and-values/math-in-code
level: intro
summary: >-
  Compute the greatest common divisor and the least common multiple of two integers with Euclid's algorithm or a library function, with negative numbers and zero.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://en.wikipedia.org/wiki/Euclidean_algorithm
  title: Wikipedia, Euclidean algorithm
  locator: the introduction; Number of steps
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Greatest-common-divisor/00-TASK.txt
  title: Rosetta Code, Greatest common divisor (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Least-common-multiple/00-TASK.txt
  title: Rosetta Code, Least common multiple (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://docs.python.org/3/library/math.integer.html
  title: The Python Standard Library, math.integer
  locator: gcd; lcm
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/math/BigInteger.html
  title: Java SE 25 API, Class BigInteger
  locator: gcd(BigInteger)
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Math.html
  title: Java SE 25 API, Class Math
  locator: abs(long); absExact(long)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.numerics.biginteger.greatestcommondivisor?view=net-10.0
  title: .NET API reference, BigInteger.GreatestCommonDivisor method
  locator: Remarks
- url: https://pkg.go.dev/math/big
  title: Go standard library, math/big
  locator: func (*Int) GCD
- url: https://en.cppreference.com/w/cpp/numeric/gcd.html
  title: cppreference.com, std::gcd
  locator: Return value
- url: https://en.cppreference.com/w/cpp/numeric/lcm.html
  title: cppreference.com, std::lcm
  locator: Return value
- url: https://go.dev/ref/spec#Arithmetic_operators
  title: The Go Programming Language Specification
  locator: Integer operators
---

## Task

Compute the greatest common divisor and the least common multiple of two integers.

- The greatest common divisor (GCD) "is the largest natural number that divides both a and b without leaving a remainder"[^1]. Rosetta Code says that it "is also known as greatest common factor (gcf) and greatest common measure"[^2].
- The least common multiple (LCM) "is the smallest positive integer that has both m and n as factors"[^3].

The function `gcd_and_lcm(a, b)` returns the pair `(gcd, lcm)`. For example, `gcd_and_lcm(12, 18)` is `(6, 36)`. Both results are never negative, so `gcd_and_lcm(-12, 18)` is `(6, 36)` too.

Zero needs a rule. Each integer divides 0, so the GCD of 0 and 7 is 7, and the GCD of 0 and 0 is 0 by convention. For the LCM, "if either m or n is zero, then the least common multiple is zero"[^3].

The items are 64-bit signed integers. Neither is the smallest 64-bit value, because its absolute value does not fit, and the LCM fits in 64 bits.

Euclid's algorithm finds the GCD with a loop. It "is based on the principle that the greatest common divisor of two numbers does not change if the larger number is replaced by its difference with the smaller number"[^1]. The remainder of a division does the same as many subtractions, so the loop is:

1. If `b` is 0, the answer is `a`.
2. Otherwise, replace the pair `(a, b)` with `(b, a mod b)`, and go to step 1.

For 48 and 180, the pairs are (48, 180), (180, 48), (48, 36), (36, 12) and (12, 0), so the GCD is 12. The LCM then follows from the GCD: it is the absolute value of `a / gcd * b`.

## Solutions

| Language | GCD | LCM |
|---|---|---|
| Python | `math.gcd(a, b)` | `math.lcm(a, b)` |
| JavaScript, TypeScript | Euclid's algorithm with `BigInt` values | `a / g * b` |
| Java | Euclid's algorithm; `BigInteger.gcd` for big numbers | `a / g * b` |
| C# | Euclid's algorithm; `BigInteger.GreatestCommonDivisor` for big numbers | `a / g * b` |
| Go | Euclid's algorithm; `big.Int.GCD` for big numbers | `a / g * b` |
| Rust | Euclid's algorithm on `u64` values | `a / g * b` |
| C | Euclid's algorithm with `llabs` | `a / g * b` |
| C++ | `std::gcd(a, b)` | `std::lcm(a, b)` |
| Bash | Euclid's algorithm with `$((a % b))` | `$((a / g * b))` |

- **Python.** `math.gcd` and `math.lcm` take any count of integers. If any argument is nonzero, "the returned value is the largest positive integer that is a divisor of all arguments. If all arguments are zero, then the returned value is 0"[^4]. For the LCM, "If any of the arguments is zero, then the returned value is 0"[^4]. Python integers have no size limit.
- **JavaScript.** There is no GCD function. The solution uses `BigInt` values, because a `number` cannot hold each 64-bit integer, and computes the absolute values with `x < 0n ? -x : x`.
- **TypeScript.** The same code as JavaScript, with the type `bigint`.
- **Java.** There is no GCD function for `long` values. `BigInteger.gcd` "Returns a BigInteger whose value is the greatest common divisor of abs(this) and abs(val). Returns 0 if this == 0 && val == 0"[^5]. The solution uses `Math.absExact`, which throws an exception for the smallest `long` value[^6].
- **C#.** There is no GCD function for `long` values. For `BigInteger`, "If either parameter is zero, the method returns the absolute value of the non-zero parameter. If both values are zero, the method returns zero"[^7]. `Math.Abs` throws `OverflowException` for `long.MinValue`.
- **Go.** There is no GCD function for `int64` values. `GCD` of `big.Int` "sets z to the greatest common divisor of a and b and returns z", and "a and b may be positive, zero or negative"[^8].
- **Rust.** The standard library has no GCD function. The solution takes `unsigned_abs` of each value, so the loop works on `u64` values with no sign.
- **C.** The solution uses a loop with `long long` values and `llabs` from `<stdlib.h>`.
- **C++.** `std::gcd` is in `<numeric>`. "If both m and n are zero, returns zero. Otherwise, returns the greatest common divisor of |m| and |n|"[^9]. `std::lcm` is in the same header: "If either m or n is zero, returns zero. Otherwise, returns the least common multiple of |m| and |n|"[^10].
- **Bash.** The solution uses a `while` loop with `$((a % b))`, and prints the GCD and the LCM on one line.

The Python functions:

```python run id=python-gcd-lcm
import math

print(math.gcd(12, 18), math.lcm(12, 18))
print(math.gcd(-12, 18), math.lcm(-12, 18))
print(math.gcd(0, 7), math.gcd(0, 0), math.lcm(0, 7))
print(math.gcd(12, 18, 30), math.lcm(4, 6, 10))
```

```output for=python-gcd-lcm
6 36
6 36
7 0 0
6 60
```

The C++ functions:

```cpp run id=cpp-gcd-lcm
#include <iostream>
#include <numeric>

int main() {
    std::cout << std::gcd(12, 18) << ' ' << std::lcm(12, 18) << '\n';
    std::cout << std::gcd(-12, 18) << ' ' << std::lcm(-12, 18) << '\n';
    std::cout << std::gcd(0, 0) << '\n';
}
```

```output for=cpp-gcd-lcm
6 36
6 36
0
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `common-factor`, `coprime` and `euclid-example`: two numbers with a common factor, and two numbers with no common factor.
- `zero-and-number`: the GCD of 0 and 7 is 7, and the LCM is 0.
- `both-zero`: the GCD and the LCM of 0 and 0 are 0.
- `negative` and `both-negative`: the results are not negative.
- `equal`: the GCD and the LCM of a number and itself are the number.
- `one`: the GCD of 1 and a number is 1.
- `multiple`: when `a` divides `b`, the GCD is `a` and the LCM is `b`.
- `fibonacci`: 832040 and 514229 are consecutive Fibonacci numbers. They are the worst case for Euclid's algorithm, and the loop takes 28 steps. "This result suffices to show that the number of steps in Euclid's algorithm can never be more than five times the number of its digits (base 10)"[^1].
- `big-lcm`: the LCM of 3000000000000000000 and 6000000000000000000 fits in 64 bits, but their product does not.
- `largest`: the largest 64-bit value and its negation.

## Pitfalls

- **The remainder can be negative.** In Go, Java, C, C++, C#, JavaScript and Bash, the remainder has the sign of the dividend. In Go, `x / y` is "truncated towards zero"[^11]. So Euclid's algorithm without the absolute values gives a negative GCD for some inputs. Take the absolute values first. See [[integer-division-and-modulo]]:

```go wrong id=go-gcd-sign
package main

import "fmt"

func gcd(a, b int64) int64 {
	for b != 0 {
		a, b = b, a%b
	}
	return a
}

func main() {
	fmt.Println(gcd(12, -18), gcd(-12, 18))
}
```

```output for=go-gcd-sign
-6 6
```

  In Python, the result of `%` has the sign of the divisor, so the same loop gives other signs. `math.gcd` is never negative.

- **The LCM overflows in the product.** `a * b / gcd` computes the product first, and the product of two large numbers does not fit, with no error. Divide first: `a / gcd * b`. The division is exact, because the GCD divides `a`:

```go wrong id=go-lcm-overflow
package main

import "fmt"

func main() {
	a, b, g := int64(3000000000000000000), int64(6000000000000000000), int64(3000000000000000000)
	fmt.Println(a * b / g)
	fmt.Println(a / g * b)
}
```

```output for=go-lcm-overflow
-2
6000000000000000000
```

- **The absolute value of the smallest integer.** The smallest 64-bit value is -9223372036854775808, and 9223372036854775808 does not fit. In Java, for `Long.MIN_VALUE` "the result is that same value, which is negative. In contrast, the absExact(long) method throws an ArithmeticException for this value"[^6]:

```java wrong id=java-abs-min
public class Main {
  public static void main(String[] args) {
    System.out.println(Math.abs(Long.MIN_VALUE));
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
ArithmeticException
```

  The GCD of the smallest value and 0 is 9223372036854775808, which does not fit either. For `std::gcd` in C++, "If either |m| or |n| is not representable as a value of type std::common_type_t<M, N>, the behavior is undefined"[^9]. Use big integers when the inputs can be this value. See [[use-big-integers]].

- **The subtraction form is slow.** Euclid's original form subtracts the smaller number from the larger number. For 1000000007 and 1, it takes a billion subtractions. The remainder form takes one step.

## Related tasks

- [[check-whether-a-number-is-prime]]: two numbers are coprime when their GCD is 1.
- [[compute-fibonacci-numbers]]: consecutive Fibonacci numbers are the worst case of Euclid's algorithm.
- [[sum-and-multiply-a-list]]: the GCD and the LCM of a list are reductions with these functions.
- [[use-big-integers]]: the GCD of integers with no size limit.

The algorithms are on the algorithm site: [the Euclidean algorithm](https://algorithm.tamnd.com/euclidean-algorithm/) and [the binary GCD algorithm](https://algorithm.tamnd.com/binary-gcd/).

## References
