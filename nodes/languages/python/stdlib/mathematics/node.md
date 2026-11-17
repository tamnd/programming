---
tag: T4890E
kind: feature
owner: python
slug: mathematics
name: Mathematics
aliases: [math module, random module, statistics module, secrets module, math.isclose, math.fsum, random.seed, statistics.mean, pseudo-random numbers]
area: languages/python/stdlib
level: intro
summary: >-
  Four modules cover everyday mathematics. math has the float functions and constants, random makes pseudo-random choices, statistics gives the mean and median, and secrets makes secure tokens.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/math.html", locator: "math, Mathematical functions"}
proposals: []
see_also: [python/the-standard-library-tour, python/floating-point-arithmetic, python/decimal-floating-point-arithmetic, python/fractions, python/the-numeric-tower, python/the-operator-module]
sources:
  - url: https://docs.python.org/3.15/tutorial/stdlib.html
    title: Brief tour of the standard library, Python 3.15 documentation
    locator: 10.6 Mathematics
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/math.html
    title: math, Mathematical functions, Python 3.15 documentation
    locator: isclose(); fsum(); Number-theoretic functions; CPython implementation detail
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/random.html
    title: random, Generate pseudo-random numbers, Python 3.15 documentation
    locator: Introduction; seed(); Notes on Reproducibility
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/secrets.html
    title: secrets, Generate secure random numbers for managing secrets, Python 3.15 documentation
    locator: Introduction; Random numbers; token_hex()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/statistics.html
    title: statistics, Mathematical statistics functions, Python 3.15 documentation
    locator: mean(); median(); StatisticsError
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html
    title: Built-in functions, Python 3.15 documentation
    locator: sum()
    accessed: 2026-11-16
---

## Synopsis

```python run id=tour
import math
import random
import statistics

print(math.sqrt(2), math.floor(-2.5), math.ceil(-2.5))
random.seed(42)
print(random.random())
data = [2.75, 1.75, 1.25, 0.25, 0.5, 1.25, 3.5]
print(statistics.mean(data), statistics.median(data))
```

```output for=tour
1.4142135623730951 -3 -2
0.6394267984578837
1.6071428571428572 1.25
```

## Semantics

**r1.** "The math module gives access to the underlying C library functions for floating-point math".[^1] It "consists mostly of thin wrappers around the platform C math library functions."[^2] Example: [`tour`](#tour).

**r2.** "The current implementation will raise ValueError for invalid operations like sqrt(-1.0) or log(0.0)". "A NaN will not be returned from any of the functions above unless one or more of the input arguments was a NaN".[^2] Example: [`sqrt-negative`](#sqrt-negative).

**r3.** `math.isclose(a, b)` compares two floats with a tolerance. "The default tolerance is 1e-09, which assures that the two values are the same within about 9 decimal digits."[^2] Example: [`float-equality`](#float-equality).

**r4.** `math.fsum` "Avoids loss of precision by tracking multiple intermediate partial sums."[^2] Example: [`fsum`](#fsum).

**r5.** The integer functions `comb`, `factorial`, `gcd`, `isqrt`, `lcm` and `perm` take integers and return exact integers.[^2] Example: [`integers`](#integers).

**r6.** "Python uses the Mersenne Twister as the core generator."[^3] It is "completely deterministic". "By reusing a seed value, the same sequence should be reproducible from run to run as long as multiple threads are not running."[^3] Example: [`seeded`](#seeded).

**r7.** "The pseudo-random generators of this module should not be used for security purposes. For security or cryptographic uses, see the secrets module."[^3] "The secrets module provides access to the most secure source of randomness that your operating system provides."[^4] Example: [`secure-token`](#secure-token).

**r8.** "The statistics module calculates basic statistical properties (the mean, median, variance, etc.) of numeric data".[^1] For `mean`, "If data is empty, StatisticsError will be raised."[^5] `StatisticsError` is a "Subclass of ValueError for statistics-related exceptions."[^5] Example: [`mean-empty`](#mean-empty).

## Examples

```python run id=integers
import math

print(math.comb(5, 2), math.perm(5, 2), math.factorial(5))
print(math.gcd(12, 18), math.lcm(4, 6), math.isqrt(10))
print(math.factorial(25))
```

```output for=integers
10 20 120
6 12 3
15511210043330985984000000
```

`comb(5, 2)` is the number of ways to choose 2 items from 5 without order, and `perm(5, 2)` counts them with order. The results are exact integers (rule r5).

```python run id=fsum
import math

total = 0.0
for value in [0.1] * 10:
    total += value
print(total, total == 1.0)
print(math.fsum([0.1] * 10), math.fsum([0.1] * 10) == 1.0)
```

```output for=fsum
0.9999999999999999 False
1.0 True
```

Each `+=` rounds the running total to a float, and the errors add up. `fsum` keeps the lost parts and rounds once at the end (rule r4).

```python run id=seeded
import random

random.seed(42)
first = [random.random() for _ in range(2)]
random.seed(42)
second = [random.random() for _ in range(2)]
print(first == second, first[0])
rng = random.Random(7)
deck = list(range(10))
rng.shuffle(deck)
print(sorted(deck) == list(range(10)), 0 <= rng.randrange(6) < 6)
```

```output for=seeded
True 0.6394267984578837
True True
```

The same seed gives the same sequence (rule r6). A `random.Random` object has its own state, so a test or a simulation can use its own generator and not change the shared one.

```python run id=statistics
import statistics as st

data = [2.75, 1.75, 1.25, 0.25, 0.5, 1.25, 3.5]
print(st.mean(data), st.median(data), st.mode(data))
print(round(st.stdev(data), 4), st.quantiles([1, 2, 3, 4, 5, 6, 7, 8], n=4))
```

```output for=statistics
1.6071428571428572 1.25 1.25
1.1713 [2.25, 4.5, 6.75]
```

`stdev` is the sample standard deviation, and `quantiles` with `n=4` gives the 3 cut points of the quartiles (rule r8).

```python run id=secure-token
import secrets
import string

token = secrets.token_hex(16)
alphabet = string.ascii_letters + string.digits
password = "".join(secrets.choice(alphabet) for _ in range(12))
print(len(token), all(c in "0123456789abcdef" for c in token))
print(len(password), all(c in alphabet for c in password))
```

```output for=secure-token
32 True
12 True
```

`token_hex(16)` uses 16 random bytes, and "each byte converted to two hex digits", so the string has 32 characters.[^4] The values change on each run, so the example prints only their length and alphabet (rule r7).

## Versions

> [!since v="3.12"]
> The built-in `sum` uses a more accurate algorithm for floats: "Summation of floats switched to an algorithm that gives higher accuracy and better commutativity on most builds."[^6] The [`fsum`](#fsum) example uses a loop for this reason.

> [!since v="3.15"]
> The integer functions are in the new `math.integer` module. The names in `math` stay as aliases: "Use the math.integer functions instead of these aliases."[^2]

## Pitfalls

```python wrong id=float-equality
import math

print(0.1 + 0.2 == 0.3, 0.1 + 0.2)
print(math.isclose(0.1 + 0.2, 0.3))
```

```output for=float-equality
False 0.30000000000000004
True
```

Binary floats cannot hold 0.1, 0.2 or 0.3 exactly, so `==` fails. Compare floats with `math.isclose` (rule r3). Near zero, give an `abs_tol`, because `isclose(x, 0)` "is False for any nonzero x and rel_tol less than 1.0."[^2]

```python fails id=sqrt-negative expect=exception:ValueError
import math

print(math.sqrt(-1))
```

`math.sqrt` returns a float, so a negative input is an invalid operation and raises `ValueError` (rule r2).

```python fails id=mean-empty expect=exception:StatisticsError
import statistics

print(statistics.mean([]))
```

An empty list has no mean (rule r8). Check the length first, or catch `ValueError`, which also catches `StatisticsError`.

Do not use `random` for passwords, tokens or keys. The Mersenne Twister "is completely unsuitable for cryptographic purposes" (rules r6 and r7).[^3] Use `secrets`.

## Comparison

For exact decimal results, use [[python/decimal-floating-point-arithmetic]]. For exact ratios, use [[python/fractions]]. For the float format and its rounding, see [[python/floating-point-arithmetic]]. For large arrays of numbers, the tutorial points to other projects: "The SciPy project <https://scipy.org> has many other modules for numerical computations."[^1]
