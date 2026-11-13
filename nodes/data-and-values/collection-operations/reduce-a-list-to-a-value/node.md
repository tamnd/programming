---
tag: WS8915
kind: task
slug: reduce-a-list-to-a-value
name: Reduce a list to a value
aliases: [fold a list, aggregate a list, sum a list, accumulate, inject, catamorphism, sum of an array]
area: data-and-values/collection-operations
level: intro
summary: Combine the items of a list into one value with an accumulator, here the sum of a list of integers, and give an initial value for the empty list.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/functools.html#functools.reduce
  title: The Python Standard Library, functools
  locator: functools.reduce()
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/IntStream.html
  title: Java SE 25 API, Interface IntStream
  locator: sum()
- url: https://go.dev/ref/spec#Integer_overflow
  title: The Go Programming Language Specification
  locator: Integer overflow, Numeric types
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.fold
  title: Rust standard library, trait Iterator
  locator: fold, sum
- url: https://en.cppreference.com/w/cpp/algorithm/accumulate
  title: cppreference.com, std::accumulate
  locator: Notes
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/reduce
  title: MDN Web Docs, Array.prototype.reduce()
  locator: Exceptions
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.aggregate?view=net-10.0
  title: .NET API reference, Enumerable.Aggregate method
  locator: Exceptions
- url: https://en.cppreference.com/w/c/language/operator_arithmetic
  title: cppreference.com, Arithmetic operators (C)
  locator: Overflows
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Shell-Arithmetic
  title: Bash Reference Manual
  locator: 6.5 Shell Arithmetic
---

## Task

Reduce a list of integers to one value, the sum of the items. The function `total(items)` starts with an accumulator of 0. For each item, it adds the item to the accumulator. For example, `total([1, 2, 3, 4])` is 10, and `total([])` is 0.

A reduce is also known as a fold or an aggregate. It takes a list, an initial value and a function of two arguments. The function takes the accumulator and the next item, and returns the next accumulator. The initial value is also the result for an empty list.

The items are 32-bit signed integers, and the total fits in a 32-bit signed integer. A partial sum can be outside this range. For example, the total of `[2147483647, 1, -1]` is 2147483647, but the partial sum after the second item is 2147483648.

## Solutions

| Language | Reduce the integers to their sum |
|---|---|
| Python | `reduce(add, items, 0)` |
| JavaScript, TypeScript | `items.reduce((acc, x) => acc + x, 0)` |
| Java | `Arrays.stream(items).asLongStream().reduce(0L, Long::sum)` |
| C# | `items.Aggregate(0L, (acc, x) => acc + x)` |
| Go | a `for` loop with an `int64` accumulator |
| Rust | `items.iter().fold(0i64, \|acc, &x\| acc + i64::from(x))` |
| C | a `for` loop with a `long long` accumulator |
| C++ | `std::accumulate(items.begin(), items.end(), 0LL)` |
| Bash | a `for` loop with `acc=$((acc + x))` |

- **Python.** The third argument of `reduce` is the initial value. "If the optional initial is present, it is placed before the items of the iterable in the calculation, and serves as a default when the iterable is empty"[^1].
- **Java.** `IntStream.sum` is a reduce too: "This is a special case of a reduction"[^2]. The solution adds the items as `long` values, then `Math.toIntExact` makes the `int` result.
- **C#.** The seed `0L` sets the type of the accumulator to `long`. `checked` makes the conversion to `int` fail if the total does not fit.
- **Go.** The standard library has no reduce function, so the solution uses a loop. The accumulator is an `int64`, because an `int` is "either 32 or 64 bits"[^3].
- **Rust.** `fold` "Folds every element into an accumulator by applying an operation, returning the final result"[^4]. `i32::try_from` makes the result.
- **C.** C has no reduce function, so the solution uses a loop. A `long long` accumulator holds each partial sum.
- **C++.** "std::accumulate performs a left fold"[^5]. The initial value `0LL` sets the type of the accumulator.
- **Bash.** Bash has no reduce function, so the solution uses a loop over the arguments.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: the sum of an empty list is 0, the initial value.
- `single`: the sum of one item is that item.
- `negative` and `zero-sum`: negative items make the sum smaller, and `[5, -5]` gives 0.
- `max`, `min` and `to-the-limit`: the total can be -2147483648 or 2147483647.
- `partial-overflow` and `partial-underflow`: a partial sum is outside the 32-bit range, but the total is in it. A 32-bit accumulator overflows for these cases.

## Pitfalls

- **Python reduce of an empty list needs an initial value.** "If initial is not given and iterable contains only one item, the first item is returned"[^1]. For an empty list without an initial value, `reduce` raises a `TypeError`:

```python run id=python-reduce-empty
from functools import reduce
from operator import add

print(reduce(add, [1, 2, 3], 0))
print(reduce(add, [], 0))
try:
    reduce(add, [])
except TypeError as e:
    print(type(e).__name__)
```

```output for=python-reduce-empty
6
0
TypeError
```

- **JavaScript reduce of an empty array needs an initial value.** `reduce` throws a `TypeError` "if the array contains no elements and initialValue is not provided"[^6]:

```javascript run id=javascript-reduce-empty
console.log([1, 2, 3].reduce((acc, x) => acc + x, 0));
console.log([].reduce((acc, x) => acc + x, 0));
try {
  [].reduce((acc, x) => acc + x);
} catch (e) {
  console.log(e.name);
}
```

```output for=javascript-reduce-empty
6
0
TypeError
```

- **Java int sums overflow with no exception.** In the example, the `int` sum of `Integer.MAX_VALUE` and 1 is `Integer.MIN_VALUE`. Add the items as `long` values:

```java run id=java-sum-overflow
import java.util.stream.IntStream;

public class Main {
  public static void main(String[] args) {
    int[] big = {Integer.MAX_VALUE, 1};
    // The int sum overflows with no exception.
    System.out.println(IntStream.of(big).sum());
    // A long sum holds the result.
    System.out.println(IntStream.of(big).asLongStream().sum());
  }
}
```

```output for=java-sum-overflow
-2147483648
2147483648
```

- **C# Aggregate without a seed fails for an empty sequence.** It throws an `InvalidOperationException` when the "source contains no elements"[^7]. Give a seed:

```csharp run id=csharp-aggregate-seed
using System;
using System.Linq;

int[] empty = [];
Console.WriteLine(empty.Aggregate(0, (acc, x) => acc + x));
try
{
    Console.WriteLine(empty.Aggregate((acc, x) => acc + x));
}
catch (InvalidOperationException e)
{
    Console.WriteLine(e.GetType().Name);
}
```

```output for=csharp-aggregate-seed
0
InvalidOperationException
```

- **Go integer accumulators wrap around.** "For signed integers, the operations +, -, *, /, and << may legally overflow"[^3]. In the example, the `int32` accumulator wraps to -2147483648, and the `int64` accumulator holds 2147483648:

```go run id=go-sum-wrap
package main

import "fmt"

func main() {
	items := []int32{2147483647, 1}
	var small int32
	var big int64
	for _, x := range items {
		small += x
		big += int64(x)
	}
	fmt.Println(small, big)
}
```

```output for=go-sum-wrap
-2147483648 2147483648
```

- **Rust sum can panic.** For `sum`, "this method will panic if the computation overflows and overflow checks are enabled"[^4]. A release build has no overflow checks by default. Use `try_fold` with `checked_add` to get `None`, or a wider accumulator:

```rust run id=rust-fold-checked
fn main() {
    let items: [i32; 2] = [i32::MAX, 1];
    let checked = items.iter().try_fold(0i32, |acc, &x| acc.checked_add(x));
    println!("{checked:?}");
    let wide = items.iter().fold(0i64, |acc, &x| acc + i64::from(x));
    println!("{wide}");
}
```

```output for=rust-fold-checked
None
2147483648
```

- **C signed overflow is undefined.** "When signed integer arithmetic operation overflows (the result does not fit in the result type), the behavior is undefined"[^8]. Thus a test cannot show the overflow of an `int` sum. The example adds the items to a `long long`, and the partial sum 2147483648 is correct:

```c run id=c-sum-wide
#include <limits.h>
#include <stdio.h>

int main(void) {
    int items[] = {INT_MAX, 1, -1};
    long long acc = 0;
    for (size_t i = 0; i < sizeof items / sizeof items[0]; i++) {
        acc += items[i];
        printf("%lld\n", acc);
    }
    return 0;
}
```

```output for=c-sum-wide
2147483647
2147483648
2147483647
```

- **C++ accumulate uses the type of the initial value.** "std::accumulate(v.begin(), v.end(), 0) likely does not give the result one wishes for when v is of type std::vector<double>"[^5]. In the example, the initial value `0` makes each partial sum an `int`, so the result is 0. The initial value `0.0` gives 1.5:

```cpp run id=cpp-accumulate-init
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::vector<double> v{0.5, 0.5, 0.5};
    std::cout << std::accumulate(v.begin(), v.end(), 0) << '\n';
    std::cout << std::accumulate(v.begin(), v.end(), 0.0) << '\n';
}
```

```output for=cpp-accumulate-init
0
1.5
```

- **Bash arithmetic wraps around.** "Evaluation is done in the largest fixed-width integers available, with no check for overflow"[^9]. In the example, the largest 64-bit value plus 1 gives the smallest 64-bit value:

```bash run id=bash-sum-wrap
total() {
  local acc=0 x
  for x in "$@"; do
    acc=$((acc + x))
  done
  printf '%s\n' "$acc"
}
total 1 2 3
total
echo $((9223372036854775807 + 1))
```

```output for=bash-sum-wrap
6
0
-9223372036854775808
```

## Related tasks

- [[map-over-a-list]]: make a new list from each item.
- [[filter-a-list]]: keep the items that pass a test.
- [[find-the-largest-element]]: a reduce that keeps the larger of two values.
- [[count-the-items-of-each-kind]]: a reduce to a map of counts.

## References
