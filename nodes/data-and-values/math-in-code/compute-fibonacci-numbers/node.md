---
tag: CTSC17
kind: task
slug: compute-fibonacci-numbers
name: Compute Fibonacci numbers
aliases: [fibonacci, fib, Fibonacci sequence, nth Fibonacci number, memoized fibonacci, functools.cache, Binet's formula]
area: data-and-values/math-in-code
level: intro
summary: >-
  Compute the nth Fibonacci number with a loop that keeps the last two numbers. F(92) is the largest in 64 bits. Naive recursion is slow, and a float formula is not exact.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Fibonacci-sequence/00-TASK.txt
  title: Rosetta Code, Fibonacci sequence (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://en.wikipedia.org/wiki/Fibonacci_sequence
  title: Wikipedia, Fibonacci sequence
  locator: Definition; Closed-form expression; Computation by rounding
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/BigInt
  title: MDN Web Docs, BigInt
  locator: the introduction
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Math.html
  title: Java SE 25 API, Class Math
  locator: addExact(long, long)
- url: https://doc.rust-lang.org/std/primitive.i64.html
  title: Rust standard library, primitive type i64
  locator: checked_add
- url: https://docs.python.org/3/library/functools.html
  title: The Python Standard Library, functools
  locator: cache
- url: https://docs.python.org/3/library/sys.html
  title: The Python Standard Library, sys
  locator: getrecursionlimit
---

## Task

Compute the nth Fibonacci number. The sequence starts with 0 and 1, and each next number is the sum of the two numbers before it. So `F(0) = 0`, `F(1) = 1`, and `F(n) = F(n - 1) + F(n - 2)` for `n` greater than 1. The Rosetta Code task says: "Write a function to generate the nth Fibonacci number"[^1].

Some texts start the sequence at `F(1) = 1` and `F(2) = 1`, "though many authors also include a zeroth element F0 = 0"[^2]. This page starts at `F(0) = 0`. The sequence begins 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55.

The function `fibonacci(n)` returns `F(n)`. For example, `fibonacci(10)` is 55.

The loop keeps only the last two numbers:

1. If `n` is 0, the answer is 0.
2. Set `a` to 0 and `b` to 1.
3. Repeat `n - 1` times: replace the pair `(a, b)` with `(b, a + b)`.
4. The answer is `b`.

The loop takes `n - 1` additions and a fixed amount of memory. The numbers grow fast: F(47) does not fit in 32 bits, and F(93) does not fit in 64 bits. So the contract has the range 0 to 92. For larger `n`, use big integers.

## Solutions

| Language | 64-bit loop | Big result |
|---|---|---|
| Python | a loop; Python integers have no size limit | the same |
| JavaScript, TypeScript | a loop with `BigInt` values | the same |
| Java | a loop with `Math.addExact` | `BigInteger.add` in a loop |
| C# | a loop with `checked` | `BigInteger` in a loop |
| Go | a loop | `big.Int.Add` in a loop |
| Rust | a loop with `checked_add` | none in the standard library |
| C | a loop with `long long` | none |
| C++ | a `constexpr` loop with `long long` | none |
| Bash | a loop with `$((a + b))` | `bc` |

- **Python.** There is no Fibonacci function. The loop uses tuple assignment, `a, b = b, a + b`, and Python integers have no size limit.
- **JavaScript.** There is no Fibonacci function. A `number` holds integers exactly only up to 2 to the power 53, so the solution uses `BigInt`. "BigInt values represent integer values which are too high or too low to be represented by the number primitive"[^3].
- **TypeScript.** The same code as JavaScript, with the type `bigint`.
- **Java.** `Math.addExact` "Returns the sum of its arguments, throwing an exception if the result overflows a long"[^4]. For larger numbers, use `BigInteger`.
- **C#.** A `checked` block throws `OverflowException` on overflow. For larger numbers, use `BigInteger`.
- **Go.** A loop with `int64` values. Go integers wrap around on overflow with no error, so the contract range is important. For larger numbers, use `big.Int`.
- **Rust.** `checked_add` is "Checked integer addition. Computes self + rhs, returning None if overflow occurred"[^5]. The solution returns `None` when the result does not fit.
- **C.** The solution uses a loop with `long long` values.
- **C++.** The same loop as C, as a `constexpr` function.
- **Bash.** The solution uses a loop with `$((a + b))`.

The Python loop, with the first 13 numbers and a big result:

```python run id=python-fibonacci
def fibonacci(n):
    a, b = 0, 1
    for _ in range(n):
        a, b = b, a + b
    return a


print([fibonacci(n) for n in range(13)])
print(fibonacci(100))
```

```output for=python-fibonacci
[0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144]
354224848179261915075
```

The recursive form follows the definition. The Rosetta Code task says that "recursive solutions are generally considered too slow and are mostly used as an exercise in recursion"[^1], because each call makes two more calls. A cache stores each result, so each number is computed once. In Python, `functools.cache` is a "Simple lightweight unbounded function cache"[^6]:

```python run id=python-fibonacci-cache
import functools


@functools.cache
def fibonacci(n):
    return n if n < 2 else fibonacci(n - 1) + fibonacci(n - 2)


print(fibonacci(90))
print(fibonacci.cache_info())
```

```output for=python-fibonacci-cache
2880067194370816120
CacheInfo(hits=88, misses=91, maxsize=None, currsize=91)
```

The cached form still uses one stack frame for each step, so the loop is better.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `ten`, `twenty` and `thirty`: small numbers.
- `zero`: F(0) is 0. A loop that always returns `b` gives 1.
- `one` and `two`: F(1) and F(2) are both 1.
- `largest-32-bit`: F(46) is 1836311903, the largest Fibonacci number in a 32-bit signed integer.
- `beyond-32-bit`: F(47) is 2971215073, which needs more than 32 bits.
- `binet-fails`: F(71) is 308061521170129. The float formula gives 308061521170130.
- `largest-64-bit`: F(92) is 7540113804746346429, the largest Fibonacci number in a 64-bit signed integer.

## Pitfalls

- **Naive recursion is slow.** The recursive form without a cache computes the same numbers again and again. The count of calls grows as fast as the numbers. For F(30), it makes more than 2.6 million calls:

```python run id=python-naive-calls
calls = 0


def fibonacci(n):
    global calls
    calls += 1
    return n if n < 2 else fibonacci(n - 1) + fibonacci(n - 2)


print(fibonacci(30), calls)
```

```output for=python-naive-calls
832040 2692537
```

  Each recursive form also has a depth limit. In Python, `sys.getrecursionlimit()` returns "the maximum depth of the Python interpreter stack"[^7], so a cached `fibonacci(5000)` fails with `RecursionError` if no smaller value is in the cache.

- **The sum overflows with no error.** F(93) does not fit in 64 bits. In Go, the sum wraps around and gives a negative number:

```go wrong id=go-fibonacci-overflow
package main

import "fmt"

func fibonacci(n int) int64 {
	a, b := int64(0), int64(1)
	for i := 0; i < n; i++ {
		a, b = b, a+b
	}
	return a
}

func main() {
	fmt.Println(fibonacci(92))
	fmt.Println(fibonacci(93))
}
```

```output for=go-fibonacci-overflow
7540113804746346429
-6246583658587674878
```

  See [[integer-overflow]] and [[use-big-integers]].

- **The loop computes one number too many.** The short loop runs `n` times and returns `a`, so its last step computes F(n + 1) in `b`. For F(92), that step overflows, although F(92) fits. With a checked addition, the short loop fails for a valid input. Run the loop `n - 1` times and return `b`:

```rust wrong id=rust-one-step-too-many
fn short_loop(n: u32) -> Option<i64> {
    let (mut a, mut b) = (0i64, 1i64);
    for _ in 0..n {
        let next = a.checked_add(b)?;
        a = b;
        b = next;
    }
    Some(a)
}

fn fixed_loop(n: u32) -> Option<i64> {
    if n == 0 {
        return Some(0);
    }
    let (mut a, mut b) = (0i64, 1i64);
    for _ in 1..n {
        let next = a.checked_add(b)?;
        a = b;
        b = next;
    }
    Some(b)
}

fn main() {
    println!("{:?} {:?}", short_loop(92), fixed_loop(92));
}
```

```output for=rust-one-step-too-many
None Some(7540113804746346429)
```

- **The float formula is not exact.** Binet's formula gives F(n) from the golden ratio. "It has become known as Binet's formula"[^2], and F(n) is the nearest integer to the golden ratio to the power `n`, divided by the square root of 5. The formula is exact in real numbers, but a 64-bit float has 53 bits of precision. From F(71), the rounded result is wrong:

```javascript wrong id=javascript-binet
const phi = (1 + Math.sqrt(5)) / 2;
const binet = (n) => Math.round(phi ** n / Math.sqrt(5));
let a = 0n;
let b = 1n;
for (let i = 0; i < 71; i++) {
  [a, b] = [b, a + b];
}
console.log(binet(70), binet(71));
console.log(a);
```

```output for=javascript-binet
190392490709135 308061521170130
308061521170129n
```

## Related tasks

- [[compute-a-factorial]]: another sequence with a loop form and a recursive form.
- [[compute-the-greatest-common-divisor]]: consecutive Fibonacci numbers are the worst case of Euclid's algorithm.
- [[use-big-integers]]: Fibonacci numbers above F(92) need big integers.

For a large `n`, the fast doubling method computes F(n) in about log2(n) steps. The algorithm is on the algorithm site: [Fibonacci by fast doubling](https://algorithm.tamnd.com/fibonacci-fast-doubling/).

## References
