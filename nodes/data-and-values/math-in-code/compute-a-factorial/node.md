---
tag: GJYVDD
kind: task
slug: compute-a-factorial
name: Compute a factorial
aliases: [factorial, n!, math.factorial, recursive factorial, iterative factorial, big factorial, gamma function, MulRange, tgamma]
area: data-and-values/math-in-code
level: intro
summary: >-
  Compute the factorial of a non-negative integer with a loop, with recursion or with a library function. 20! is the largest factorial in 64 bits, so larger ones need big integers.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Factorial/00-TASK.txt
  title: Rosetta Code, Factorial (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://en.wikipedia.org/wiki/Factorial
  title: Wikipedia, Factorial
  locator: Definition; Growth and approximation; Continuous interpolation and non-integer generalization
- url: https://docs.python.org/3/library/math.integer.html
  title: The Python Standard Library, math.integer
  locator: factorial
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Math.html
  title: Java SE 25 API, Class Math
  locator: multiplyExact(long, long)
- url: https://pkg.go.dev/math/big
  title: Go standard library, math/big
  locator: func (*Int) MulRange
- url: https://docs.python.org/3/library/math.html
  title: The Python Standard Library, math
  locator: gamma
- url: https://en.cppreference.com/w/c/numeric/math/tgamma.html
  title: cppreference.com, tgamma (C)
  locator: Return value
---

## Task

Compute the factorial of a non-negative integer. The factorial of `n`, written `n!`, is the product of the integers from 1 to `n`. The Rosetta Code task says: "Write a function to return the factorial of a number. Solutions can be iterative or recursive"[^1].

The function `factorial(n)` returns `n!`. For example, `factorial(5)` is 5 times 4 times 3 times 2 times 1, which is 120.

The factorial of 0 is 1. "The value of 0! is 1, according to the convention for an empty product"[^2]: a product of no numbers is 1, as the sum of no numbers is 0.

The factorial grows fast: "the factorial has faster than exponential growth"[^2]. 13! does not fit in 32 bits, and 21! does not fit in 64 bits. So the contract has the range 0 to 20, and the pitfalls show what happens above it. For larger `n`, use big integers.

## Solutions

| Language | Loop or library | Big result |
|---|---|---|
| Python | `math.factorial(n)` | the same; Python integers have no size limit |
| JavaScript, TypeScript | a loop with `BigInt` values | the same |
| Java | a loop with `Math.multiplyExact` | `BigInteger.multiply` in a loop |
| C# | a loop with `checked` | `BigInteger` in a loop |
| Go | a loop | `new(big.Int).MulRange(1, n)` |
| Rust | `(1..=n).product::<i64>()` | none in the standard library |
| C | a loop with `long long` | none |
| C++ | a loop with `long long` | none |
| Bash | a loop with `$((f * i))` | `bc` |

- **Python.** `math.factorial` returns the "factorial of the nonnegative integer n"[^3]. It raises `ValueError` for a negative `n`.
- **JavaScript.** There is no factorial function. A `number` holds each factorial up to 22! exactly, but no larger one, so the solution uses `BigInt` values.
- **TypeScript.** The same code as JavaScript, with the type `bigint`.
- **Java.** There is no factorial function. `Math.multiplyExact` throws an `ArithmeticException` "if the result overflows a long"[^4], so 21! fails loudly.
- **C#.** There is no factorial function. A `checked` block throws `OverflowException` on overflow.
- **Go.** There is no factorial function for `int64` values. For big integers, "MulRange sets z to the product of all integers in the range [a, b] inclusively and returns z"[^5], so `MulRange(1, n)` is `n!`.
- **Rust.** `product` of the range `1..=n` is the factorial. The empty range `1..=0` gives 1.
- **C.** The solution uses a loop with `long long` values. A signed overflow is undefined behavior, so the range of the contract is important.
- **C++.** The same loop as C, as a `constexpr` function, so the compiler can compute a factorial of a constant.
- **Bash.** The solution uses a `for` loop with `$((f * i))`. Bash arithmetic does not check for overflow.

The Python function, with a big result:

```python run id=python-factorial
import math

print(math.factorial(0), math.factorial(5), math.factorial(20))
print(math.factorial(25))
try:
    math.factorial(-1)
except ValueError as e:
    print(type(e).__name__)
```

```output for=python-factorial
1 120 2432902008176640000
15511210043330985984000000
ValueError
```

The Go function for big integers:

```go run id=go-mulrange
package main

import (
	"fmt"
	"math/big"
)

func main() {
	fmt.Println(new(big.Int).MulRange(1, 25))
	fmt.Println(new(big.Int).MulRange(1, 0))
}
```

```output for=go-mulrange
15511210043330985984000000
1
```

The recursive form follows the definition `n! = n * (n - 1)!`, with `0! = 1`. Each call waits for the next call, so it uses one stack frame for each step:

```rust run id=rust-recursive-factorial
fn factorial(n: u64) -> u64 {
    if n == 0 { 1 } else { n * factorial(n - 1) }
}

fn main() {
    println!("{} {}", factorial(5), factorial(20));
}
```

```output for=rust-recursive-factorial
120 2432902008176640000
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `five` and `ten`: small factorials.
- `zero`: the factorial of 0 is 1, the empty product.
- `one`: the factorial of 1 is 1.
- `largest-32-bit`: 12! is 479001600, the largest factorial in a 32-bit signed integer.
- `beyond-32-bit`: 13! is 6227020800, which needs more than 32 bits.
- `largest-64-bit`: 20! is 2432902008176640000, the largest factorial in a 64-bit signed integer.

## Pitfalls

- **The product overflows with no error.** 21! does not fit in 64 bits. In Java, a `long` loop wraps around and gives a negative number. `Math.multiplyExact` throws an exception:

```java wrong id=java-factorial-overflow
public class Main {
  public static void main(String[] args) {
    long f = 1;
    for (int i = 1; i <= 21; i++) {
      f *= i;
    }
    System.out.println(f);
    try {
      long g = 1;
      for (int i = 1; i <= 21; i++) {
        g = Math.multiplyExact(g, i);
      }
      System.out.println(g);
    } catch (ArithmeticException e) {
      System.out.println(e.getClass().getSimpleName());
    }
  }
}
```

```output for=java-factorial-overflow
-4249290049419214848
ArithmeticException
```

  See [[integer-overflow]] and [[use-big-integers]].

- **A float factorial is not exact.** A 64-bit float has 53 bits of precision. The factorials up to 22! are exact as floats, but larger ones are rounded:

```javascript wrong id=javascript-factorial-float
let f = 1;
let g = 1n;
for (let i = 1; i <= 25; i++) {
  f *= i;
  g *= BigInt(i);
}
console.log(f);
console.log(g);
```

```output for=javascript-factorial-float
1.5511210043330986e+25
15511210043330985984000000n
```

- **Deep recursion fails.** The recursive form uses a stack frame for each step. In Python, the default recursion limit is about 1000 frames, so a recursive factorial of 5000 fails. The loop has no limit, and the result has 54233 bits:

```python run id=python-recursion-limit
import math


def factorial(n):
    return 1 if n == 0 else n * factorial(n - 1)


try:
    factorial(5000)
except RecursionError as e:
    print(type(e).__name__)
print(math.factorial(5000).bit_length())
```

```output for=python-recursion-limit
RecursionError
54233
```

- **A negative argument.** The factorial of a negative integer has no value. A loop from 1 to `n` returns 1 for each negative `n`, with no error. Check the argument first, as `math.factorial` does.

- **The gamma function is not the factorial.** The gamma function extends the factorial to other numbers: "The gamma function (shifted one unit left to match the factorials) continuously interpolates the factorial to non-integer values"[^2]. `gamma(n + 1)` is `n!`, and the result is a float. In Python, `math.gamma(x)` "Return the Gamma function at x"[^6]. In C and C++, `tgamma` returns the value of the gamma function[^7]:

```python run id=python-gamma
import math

print(math.gamma(6), math.factorial(5))
print(math.isclose(math.gamma(0.5) ** 2, math.pi))
```

```output for=python-gamma
120.0 120
True
```

## Related tasks

- [[sum-and-multiply-a-list]]: the factorial is the product of the list 1 to `n`.
- [[compute-fibonacci-numbers]]: another sequence with a loop form and a recursive form.
- [[use-big-integers]]: factorials above 20! need big integers.
- [[check-whether-a-number-is-prime]]: another small task on integers.

The algorithm is on the algorithm site: [factorial](https://algorithm.tamnd.com/factorial/).

## References
