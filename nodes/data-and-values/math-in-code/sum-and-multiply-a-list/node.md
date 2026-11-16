---
tag: CF962H
kind: task
slug: sum-and-multiply-a-list
name: Sum and multiply a list
aliases: [sum of a list, product of a list, sum and product of an array, sum(), math.prod, Iterator::sum, Iterator::product, Enumerable.Sum, std::accumulate, empty product]
area: data-and-values/math-in-code
level: intro
summary: >-
  Compute the sum and the product of a list of integers with a loop or a library function. The sum of an empty list is 0 and the product is 1, and a large product can overflow.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Sum-and-product-of-an-array/00-TASK.txt
  title: Rosetta Code, Sum and product of an array (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://docs.python.org/3/library/functions.html#sum
  title: The Python Standard Library, Built-in Functions
  locator: sum()
- url: https://docs.python.org/3/library/math.html
  title: The Python Standard Library, math
  locator: math.prod; math.fsum
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/MAX_SAFE_INTEGER
  title: MDN Web Docs, Number.MAX_SAFE_INTEGER
  locator: the introduction
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Math.html
  title: Java SE 25 API, Class Math
  locator: multiplyExact(long, long)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.sum?view=net-10.0
  title: .NET API reference, Enumerable.Sum method
  locator: Sum(IEnumerable<Int64>), Exceptions
- url: https://go.dev/ref/spec#Integer_overflow
  title: The Go Programming Language Specification
  locator: Integer overflow
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html
  title: Rust standard library, trait Iterator
  locator: sum; product
- url: https://en.cppreference.com/w/cpp/algorithm/accumulate.html
  title: cppreference.com, std::accumulate
  locator: Example
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Shell-Arithmetic
  title: Bash Reference Manual
  locator: 6.5 Shell Arithmetic
- url: https://en.wikipedia.org/wiki/Kahan_summation_algorithm
  title: Wikipedia, Kahan summation algorithm
  locator: the introduction
---

## Task

Compute the sum and the product of a list of integers. The Rosetta Code task on the sum and product of an array says: "Compute the sum and product of an array of integers"[^1].

The function `sum_and_product(items)` returns the pair `(sum, product)`. For example, `sum_and_product([1, 2, 3, 4])` is `(10, 24)`.

The sum of an empty list is 0, and the product of an empty list is 1. These are the identity values of addition and multiplication: adding 0 or multiplying by 1 does not change a number. With these start values, the sum and the product of a list are the same as the sum and the product of its parts.

The items are 64-bit signed integers, and each partial sum and each partial product fits in a 64-bit signed integer. A product grows fast: the product of 20 numbers of 9 is more than 10 to the power 19, which is out of range. The pitfalls show what happens then.

## Solutions

| Language | Sum | Product |
|---|---|---|
| Python | `sum(items)` | `math.prod(items)` |
| JavaScript, TypeScript | `items.reduce((a, x) => a + x, 0n)` | `items.reduce((a, x) => a * x, 1n)` |
| Java | `LongStream.of(items).sum()` | `LongStream.of(items).reduce(1L, Math::multiplyExact)` |
| C# | `items.Sum()` | `items.Aggregate(1L, (a, x) => checked(a * x))` |
| Go | a `for` loop | a `for` loop |
| Rust | `items.iter().sum::<i64>()` | `items.iter().product::<i64>()` |
| C | a `for` loop | a `for` loop |
| C++ | `std::accumulate(first, last, 0LL)` | `std::accumulate(first, last, 1LL, std::multiplies<>())` |
| Bash | a `for` loop with `$((sum + x))` | a `for` loop with `$((product * x))` |

- **Python.** `sum` "Sums start and the items of an iterable from left to right and returns the total"[^2], and the start value is 0. `math.prod` is the product: "The default start value for the product is 1. When the iterable is empty, return the start value"[^3]. Python integers have no size limit, so they do not overflow.
- **JavaScript.** A `number` is a 64-bit float. `Number.MAX_SAFE_INTEGER` "represents the maximum safe integer in JavaScript (253 – 1)"[^4], that is, 2 to the power 53 minus 1, so the solution uses `BigInt` values. The start values `0n` and `1n` are `BigInt` values too, because a `BigInt` and a `number` cannot be mixed in `+` and `*`.
- **TypeScript.** The same code as JavaScript, with the type `bigint[]`.
- **Java.** `LongStream.sum` wraps around on overflow with no error. `Math.multiplyExact` "Returns the product of the arguments, throwing an exception if the result overflows a long"[^5], so the product fails loudly. `Math.addExact` does the same for a sum.
- **C#.** `Enumerable.Sum` of `long` values throws `OverflowException` when "The sum is larger than Int64.MaxValue"[^6]. LINQ has no product function, so the solution uses `Aggregate` with a `checked` multiplication.
- **Go.** There is no sum function for a slice, so the solution uses a loop. "For signed integers, the operations +, -, *, /, and << may legally overflow"[^7], with no error.
- **Rust.** `sum` and `product` are methods of an iterator. `product` "Iterates over the entire iterator, multiplying all the elements. An empty iterator returns the one value of the type"[^8].
- **C.** The solution uses a loop with `long long` values. A signed overflow in C is undefined behavior, so the precondition of the contract is important.
- **C++.** `std::accumulate` with the start value `1` and `std::multiplies` gives the product[^9]. The start values `0LL` and `1LL` make the accumulator a `long long`.
- **Bash.** "Evaluation is done in the largest fixed-width integers available, with no check for overflow"[^10]. The function prints the sum and the product on one line.

The Python functions:

```python run id=python-sum-prod
import math

items = [1, 2, 3, 4]
print(sum(items), math.prod(items))
print(sum([]), math.prod([]))
print(sum(items, 100), math.prod(items, start=10))
print(math.prod(range(1, 26)))
```

```output for=python-sum-prod
10 24
0 1
110 240
15511210043330985984000000
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: the sum is 0 and the product is 1.
- `single`: the sum and the product of one item are the item.
- `with-zero`: one 0 makes the product 0.
- `two-negatives` and `three-negatives`: the sign of the product depends on the count of negative items.
- `ones`: a product of ones is 1.
- `large-product`: 3037000499 squared is 9223372030926249001, near the largest 64-bit value 9223372036854775807.
- `to-the-limit`: the sum is the largest 64-bit value.
- `beyond-double`: 9007199254740993 is 2 to the power 53 plus 1. A 64-bit float cannot hold it, so a JavaScript `number` gives the wrong value.
- `min`: the smallest 64-bit value, -9223372036854775808.

## Pitfalls

- **A product overflows with no error.** In Java, the `long` product of 2 to the power 32 and 2 to the power 32 is 0, because the result needs 65 bits. `Math.multiplyExact` throws an `ArithmeticException` instead:

```java wrong id=java-product-overflow
import java.util.stream.LongStream;

public class Main {
  public static void main(String[] args) {
    long[] items = {4294967296L, 4294967296L};
    System.out.println(LongStream.of(items).reduce(1L, (a, b) -> a * b));
    try {
      System.out.println(LongStream.of(items).reduce(1L, Math::multiplyExact));
    } catch (ArithmeticException e) {
      System.out.println(e.getClass().getSimpleName());
    }
  }
}
```

```output for=java-product-overflow
0
ArithmeticException
```

  In Rust, `product` "will panic if the computation overflows and overflow checks are enabled"[^8]. A release build has no overflow checks by default, so the result wraps. `checked_mul` gives `None` in each build:

```rust run id=rust-checked-product
fn main() {
    let items: [i64; 2] = [1 << 32, 1 << 32];
    let product = items.iter().try_fold(1i64, |acc, &x| acc.checked_mul(x));
    println!("{product:?}");
    let wide: i128 = items.iter().map(|&x| i128::from(x)).product();
    println!("{wide}");
}
```

```output for=rust-checked-product
None
18446744073709551616
```

- **A JavaScript number loses large integers.** Above 2 to the power 53, a `number` cannot hold each integer, so a sum or a product gives a wrong result with no error. Use `BigInt`:

```javascript wrong id=javascript-number-precision
console.log([9007199254740992, 1].reduce((a, x) => a + x, 0));
console.log([9007199254740992n, 1n].reduce((a, x) => a + x, 0n));
console.log([3037000499, 3037000499].reduce((a, x) => a * x, 1));
console.log([3037000499n, 3037000499n].reduce((a, x) => a * x, 1n));
```

```output for=javascript-number-precision
9007199254740992
9007199254740993n
9223372030926249000
9223372030926249001n
```

- **The sum of floats has a rounding error.** 0.1 is not exact in binary, and each addition rounds the partial sum, so ten additions of 0.1 do not give 1.0. Compensated summation, such as the Kahan summation algorithm, "significantly reduces the numerical error in the total obtained by adding a sequence of finite-precision floating-point numbers, compared to the naive approach"[^11]. In Python, `math.fsum` gives "an accurate floating-point sum of values in the iterable"[^3], and `sum` of floats uses an algorithm that "gives higher accuracy and better commutativity on most builds"[^2]. A plain loop does not:

```python run id=python-float-sum
import math

items = [0.1] * 10
total = 0.0
for x in items:
    total += x
print(total)
print(sum(items), math.fsum(items))
```

```output for=python-float-sum
0.9999999999999999
1.0 1.0
```

  JavaScript `reduce` adds from left to right, as the plain loop does:

```javascript run id=javascript-float-sum
const items = Array(10).fill(0.1);
console.log(items.reduce((a, x) => a + x, 0));
```

```output for=javascript-float-sum
0.9999999999999999
```

  See [[numerical-pitfalls]] for the errors of floating-point sums.

- **The C++ start value sets the type.** `std::accumulate(v.begin(), v.end(), 0)` adds `double` values into an `int`. See [[reduce-a-list-to-a-value]] for an example.

- **The product of an empty list.** A product loop that starts at 0 always gives 0. Start the product at 1, and the sum at 0.

## Related tasks

- [[reduce-a-list-to-a-value]]: the general form of a sum, with an accumulator.
- [[compute-the-mean-and-median]]: the mean is the sum divided by the count.
- [[compute-a-factorial]]: the product of the integers from 1 to n.
- [[find-the-largest-element]]: another reduction of a list to one value.

See [[integer-overflow]] for what happens when a result does not fit, and [[use-big-integers]] for integers with no size limit.

## References
