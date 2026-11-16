---
tag: J95CT6
kind: task
slug: check-whether-a-number-is-prime
name: Check whether a number is prime
aliases: [is prime, primality test, prime check, trial division, isPrime, isProbablePrime, ProbablyPrime, math.isqrt]
area: data-and-values/math-in-code
level: intro
summary: >-
  Check whether an integer is prime with trial division up to its square root, and use a library test for big numbers. Numbers below 2 are not prime.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Primality-by-trial-division/00-TASK.txt
  title: Rosetta Code, Primality by trial division (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://docs.python.org/3/library/math.integer.html
  title: The Python Standard Library, math.integer
  locator: isqrt
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/math/BigInteger.html
  title: Java SE 25 API, Class BigInteger
  locator: isProbablePrime(int)
- url: https://pkg.go.dev/math/big
  title: Go standard library, math/big
  locator: func (*Int) ProbablyPrime
- url: https://en.wikipedia.org/wiki/Primality_test
  title: Wikipedia, Primality test
  locator: Simple methods; Fermat primality test; Baillie–PSW primality test
---

## Task

Check whether an integer is prime. A prime is an integer greater than 1 whose only positive divisors are 1 and itself.

The Rosetta Code task on primality by trial division says: "Write a boolean function that tells whether a given integer is prime", and "Remember that 1 and all non-positive numbers are not prime"[^1].

The function `is_prime(n)` returns `true` or `false`. For example, `is_prime(17)` is `true`, and `is_prime(561)` is `false`, because 561 is 3 times 11 times 17.

Trial division divides `n` by each candidate divisor. Only the candidates up to the square root of `n` are necessary: if `n` is `a` times `b` and both are larger than the square root, the product is larger than `n`. So the loop is:

1. If `n` is less than 2, the answer is `false`. If `n` is 2, the answer is `true`. If `n` is even, the answer is `false`.
2. For each odd `i` from 3 while `i * i <= n`: if `i` divides `n`, the answer is `false`.
3. The answer is `true`.

The Rosetta Code task says the same: "Even numbers greater than 2 may be eliminated right away"[^1]. The loop takes about the square root of `n` divided by 2 steps. For `n` near 2 to the power 31, that is about 23000 steps.

The items are 64-bit signed integers, and the cases are small enough for trial division. For a number with 19 digits, trial division takes about 1.5 billion steps, so use a library test, as the solutions show.

## Solutions

| Language | Trial division | Library test for big numbers |
|---|---|---|
| Python | `math.isqrt(n)` as the bound | none in the standard library |
| JavaScript, TypeScript | `BigInt` values, `i * i <= n` | none |
| Java | `i <= n / i` | `BigInteger.isProbablePrime(certainty)` |
| C# | `i <= n / i` | none |
| Go | `i <= n/i` | `big.Int.ProbablyPrime(0)` |
| Rust | `i <= n / i` | none in the standard library |
| C | `i <= n / i` | none |
| C++ | `i <= n / i` | none |
| Bash | `$((i * i <= n))` | none |

- **Python.** `math.isqrt(n)` returns "the floor of the exact square root of n, or equivalently the greatest integer a such that a² ≤ n"[^2]. It is exact for each size of integer, but `math.sqrt` returns a float, which is not exact for large numbers.
- **JavaScript.** There is no prime test. The solution uses `BigInt` values, because a `number` cannot hold each 64-bit integer.
- **TypeScript.** The same code as JavaScript, with the type `bigint`.
- **Java.** `BigInteger.isProbablePrime` "Returns true if this BigInteger is probably prime, false if it's definitely composite"[^3]. The argument `certainty` sets the error: if the call returns true, "the probability that this BigInteger is prime exceeds (1 - 1/2certainty)"[^3], that is, 1 minus 1 divided by 2 to the power `certainty`.
- **C#.** There is no prime test in the standard library. The solution uses trial division.
- **Go.** `ProbablyPrime` applies "the Miller-Rabin test with n pseudorandomly chosen bases as well as a Baillie-PSW test", and it "is 100% accurate for inputs less than 2⁶⁴"[^4]. So `big.NewInt(n).ProbablyPrime(0)` is an exact test for each `int64` value.
- **Rust.** The standard library has no prime test. The solution uses trial division.
- **C.** The solution uses trial division with `long long` values.
- **C++.** The solution uses trial division with `long long` values.
- **Bash.** The solution uses a `while` loop with `$((n % i))`, and returns the exit status 0 for a prime and 1 for other numbers.

The Python solution, with the primes below 30:

```python run id=python-is-prime
import math


def is_prime(n):
    if n < 2:
        return False
    if n % 2 == 0:
        return n == 2
    for i in range(3, math.isqrt(n) + 1, 2):
        if n % i == 0:
            return False
    return True


print([n for n in range(30) if is_prime(n)])
print(is_prime(2147483647), is_prime(561))
```

```output for=python-is-prime
[2, 3, 5, 7, 11, 13, 17, 19, 23, 29]
True False
```

The library test in Go is exact for each 64-bit value. 9223372036854775783 is the largest prime below 2 to the power 63:

```go run id=go-probably-prime
package main

import (
	"fmt"
	"math/big"
)

func main() {
	fmt.Println(big.NewInt(9223372036854775783).ProbablyPrime(0))
	fmt.Println(big.NewInt(9223372036854775781).ProbablyPrime(0))
	fmt.Println(big.NewInt(561).ProbablyPrime(0))
}
```

```output for=go-probably-prime
true
false
false
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `small-prime`, `small-composite`, `even` and `odd-composite`: the simple cases.
- `negative`, `zero` and `one`: numbers below 2 are not prime.
- `two`: the only even prime.
- `three`: the first odd prime; the loop does not run.
- `square-of-prime` and `large-square`: 25 is 5 times 5, and 4293001441 is 65521 times 65521. A loop that stops before the square root misses the divisor.
- `carmichael`: 561 is not prime, but it passes the Fermat test for each base that is coprime to it.
- `large-prime`, `mersenne-prime` and `large-composite`: 1000000007 and 2147483647 are prime, and 2147483649 is 3 times 715827883.

## Pitfalls

- **The loop stops one step early.** The bound includes the square root. With `i * i < n` in place of `i * i <= n`, the square of a prime, such as 25, is wrongly prime.

- **The square overflows.** `i * i` can be larger than the largest value of the type. In 32-bit arithmetic, 46341 squared does not fit, and the result is negative, so `i * i <= n` is true again. Compare with `i <= n / i`, or use a wider type:

```go wrong id=go-square-overflow
package main

import "fmt"

func main() {
	var i int32 = 46341
	fmt.Println(i*i, int64(i)*int64(i))
}
```

```output for=go-square-overflow
-2147479015 2147488281
```

- **A float square root is not exact.** `math.sqrt(n)` returns a float, and a 64-bit float has 53 bits of precision. For a large `n`, the rounded root can be 1 too small, and the loop misses the last divisor. Use an integer square root, such as `math.isqrt` in Python, or the comparison `i <= n / i`.

- **The Fermat test is not a prime test.** If `n` is prime, then `a` to the power `n - 1` is 1 modulo `n`. The reverse is false: "Some composite numbers (Carmichael numbers) have the property"[^5] for each base `a` that is coprime to `n`. 561 is the smallest of them:

```python run id=python-carmichael
print(pow(2, 560, 561), pow(5, 560, 561), 561 % 3)
```

```output for=python-carmichael
1 1 0
```

  Use a library test, such as `isProbablePrime` in Java or `ProbablyPrime` in Go. They combine tests such as Miller-Rabin and the Baillie-PSW test, which "has no known counterexamples"[^5].

- **A prime test in a loop is slow.** To find all primes up to a limit, a test for each number repeats work. A sieve marks the multiples of each prime once. See the algorithm site below.

## Related tasks

- [[compute-the-greatest-common-divisor]]: two numbers are coprime when their GCD is 1.
- [[compute-a-factorial]]: another small task on integers.
- [[use-big-integers]]: prime tests on numbers with no size limit.

The algorithms are on the algorithm site: [trial division](https://algorithm.tamnd.com/trial-division/), [the Miller-Rabin test](https://algorithm.tamnd.com/miller-rabin/) and [the sieve of Eratosthenes](https://algorithm.tamnd.com/sieve-of-eratosthenes/).

## References
