---
tag: Z72B24
kind: task
slug: compute-the-cartesian-product
name: Compute the Cartesian product
aliases: [cartesian product, cross product of lists, all pairs of two lists, all combinations of one item from each list, nested loops over lists, itertools.product]
area: data-and-values/collection-operations
level: core
summary: Make each tuple that takes one item from each of several lists of integers, in the order of nested loops.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/itertools.html#itertools.product
  title: The Python Standard Library, itertools
  locator: itertools.product()
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/flatMap
  title: MDN Web Docs, Array.prototype.flatMap()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Stream.html
  title: Java SE 25 API, Interface Stream
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.selectmany?view=net-10.0
  title: .NET API reference, Enumerable.SelectMany method
  locator: Remarks
- url: https://go.dev/ref/spec#Integer_overflow
  title: The Go Programming Language Specification
  locator: Integer overflow
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.fold
  title: Rust standard library, trait Iterator
  locator: fold
- url: https://en.cppreference.com/w/c/language/operator_arithmetic
  title: cppreference.com, Arithmetic operators (C)
  locator: Overflows
- url: https://en.cppreference.com/w/cpp/container/vector/reserve
  title: cppreference.com, std::vector::reserve
  locator: Exceptions
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Brace-Expansion
  title: Bash Reference Manual
  locator: 3.5.1 Brace Expansion
---

## Task

Make each tuple that takes one item from each of several lists of integers. The function `cartesian_product(lists)` returns the tuples in the order of nested loops: the first list is the outer loop, and the last list is the inner loop. For example, `cartesian_product([[1, 2], [3, 4]])` is `[[1, 3], [1, 4], [2, 3], [2, 4]]`.

The number of tuples is the product of the lengths of the lists. Thus the product of no lists is one empty tuple, `[[]]`, and a product with an empty list has no tuples. The items are 32-bit signed integers. The product of the lengths is at most 100000.

## Solutions

| Language | Add the items of each list to each tuple |
|---|---|
| Python | `[list(t) for t in product(*lists)]` |
| JavaScript, TypeScript | `reduce` from `[[]]`, with `flatMap` over the tuples and `map` over the items |
| Java | a loop over the lists, with `Arrays.copyOf` for each new tuple |
| C# | `SelectMany` over the tuples, once for each list |
| Go | a loop over the lists, with `make` and `copy` for each new tuple |
| Rust | `fold` from `vec![Vec::new()]`, with `flat_map` over the tuples |
| C | the digits of each row number, with the last list as the lowest digit |
| C++ | a loop over the lists, with `reserve` and `push_back` |
| Bash | a loop over the arguments, with `read -r -a` for the items of each list |

- **Nested loops.** `product` is "Roughly equivalent to nested for-loops in a generator expression. For example, product(A, B) returns the same as ((x,y) for x in A for y in B)."[^1] It also says: "The nested loops cycle like an odometer with the rightmost element advancing on every iteration."[^1] The other solutions do the same with one loop for each list, so the number of lists can change at run time.
- **Sorted input gives sorted output.** "This pattern creates a lexicographic ordering so that if the input’s iterables are sorted, the product tuples are emitted in sorted order."[^1]
- **The size grows fast.** Each list multiplies the number of tuples by its length. Ten lists of ten items give 10,000,000,000 tuples. `product` also reads all its input first: "Before product() runs, it completely consumes the input iterables, keeping pools of values in memory to generate the products."[^1]
- **C.** The caller gives the array `out` and its capacity in items. Tuple r is at `out[r * n]` to `out[r * n + n - 1]`. The function returns -1 when the tuples do not fit, and then it writes nothing. It finds the item of list k from the digits of r, so it needs no more memory.
- **Bash.** Each argument is one list, with the items separated by spaces. The function prints each tuple on one line.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `no-lists`: no lists give one empty tuple, `[[]]`. Python agrees: `list(product())` is `[()]`. A solution that starts with no tuples, and not with one empty tuple, gives `[]` for all the inputs.
- `one-empty`: `[[1, 2], []]` gives no tuples, because there is no item to take from the empty list.
- `single`: one list gives a tuple of one item for each item.
- `duplicates`: `[[1, 1], [2]]` gives `[[1, 2], [1, 2]]`. The function does not remove equal tuples.
- `negative` and `extremes`: negative items, -2147483648 and 2147483647 stay as they are.

## Pitfalls

- **Python product needs the star.** `product(*lists)` gives each list as one argument. `product(lists)` gives the product of one list, so each tuple has one item, which is an inner list. `product()` with no arguments gives one empty tuple:

```python run id=python-product-star
from itertools import product

lists = [[1, 2], [3, 4]]
print(list(product(*lists)))
print(list(product(lists)))
print(list(product()))
```

```output for=python-product-star
[(1, 3), (1, 4), (2, 3), (2, 4)]
[([1, 2],), ([3, 4],)]
[()]
```

- **JavaScript map gives nested arrays.** With `map` on the outer list, each item of `a` gives one array of pairs. `flatMap` "returns a new array formed by applying a given callback function to each element of the array, and then flattening the result by one level."[^2] Thus it gives one flat array of pairs:

```javascript run id=javascript-map-flatmap
const a = [1, 2];
const b = [3, 4];
console.log(JSON.stringify(a.map((x) => b.map((y) => [x, y]))));
console.log(JSON.stringify(a.flatMap((x) => b.map((y) => [x, y]))));
```

```output for=javascript-map-flatmap
[[[1,3],[1,4]],[[2,3],[2,4]]]
[[1,3],[1,4],[2,3],[2,4]]
```

- **Java a stream can be used only once.** "A stream should be operated on (invoking an intermediate or terminal stream operation) only once."[^3] Also, "A stream implementation may throw IllegalStateException if it detects that the stream is being reused."[^3] In the example, the inner stream `bs` is made once, outside `flatMap`, so the second item of `a` uses it again. Make a new inner stream for each outer item:

```java run id=java-stream-reuse
import java.util.List;
import java.util.stream.Stream;

public class Main {
  public static void main(String[] args) {
    List<Integer> a = List.of(1, 2);
    List<Integer> b = List.of(3, 4);
    System.out.println(a.stream().flatMap(x -> b.stream().map(y -> x * 10 + y)).toList());
    Stream<Integer> bs = b.stream();
    try {
      System.out.println(a.stream().flatMap(x -> bs.map(y -> x * 10 + y)).toList());
    } catch (IllegalStateException e) {
      System.out.println(e.getClass().getSimpleName());
    }
  }
}
```

```output for=java-stream-reuse
[13, 14, 23, 24]
IllegalStateException
```

- **C# SelectMany runs later.** "This method is implemented by using deferred execution."[^4] Also, "The query represented by this method is not executed until the object is enumerated"[^4]. In the example, `later` reads `b` when `Join` enumerates it, after `b.Add(5)`, so it has the pairs with 5. `ToArray` runs the query at once:

```csharp run id=csharp-selectmany-deferred
using System;
using System.Collections.Generic;
using System.Linq;

var a = new List<int> { 1, 2 };
var b = new List<int> { 3, 4 };
var later = a.SelectMany(x => b, (x, y) => x * 10 + y);
var now = a.SelectMany(x => b, (x, y) => x * 10 + y).ToArray();
b.Add(5);
Console.WriteLine(string.Join(" ", later));
Console.WriteLine(string.Join(" ", now));
```

```output for=csharp-selectmany-deferred
13 14 15 23 24 25
13 14 23 24
```

- **Go the count of tuples can wrap around.** "For signed integers, the operations +, -, *, /, and << may legally overflow and the resulting value exists and is deterministically defined by the signed integer representation, the operation, and its operands. Overflow does not cause a run-time panic."[^5] In the example, on a 64-bit platform, 2^32 times 2^32 is 0. Thus a size from the product of the lengths can be wrong with no error. Compare with `math.MaxInt` divided by the length:

```go run id=go-count-overflow
package main

import (
	"fmt"
	"math"
)

func main() {
	n := 1 << 32
	fmt.Println(n * n)
	fmt.Println(n > math.MaxInt/n)
}
```

```output for=go-count-overflow
0
true
```

- **Rust fold from an empty vector gives nothing.** For `fold`, "The initial value is the value the accumulator will have on the first call."[^6] An empty vector has no tuple to extend, so each step gives an empty vector again. Start from one empty tuple:

```rust run id=rust-fold-start
fn main() {
    let lists: Vec<Vec<i32>> = vec![vec![1, 2], vec![3]];
    let step = |acc: Vec<Vec<i32>>, xs: &Vec<i32>| -> Vec<Vec<i32>> {
        acc.iter()
            .flat_map(|t| xs.iter().map(move |&x| [t.clone(), vec![x]].concat()))
            .collect()
    };
    let wrong = lists.iter().fold(Vec::new(), step);
    let right = lists.iter().fold(vec![Vec::new()], step);
    println!("{wrong:?}");
    println!("{right:?}");
}
```

```output for=rust-fold-start
[]
[[1, 3], [2, 3]]
```

- **C the product of the lengths can wrap around.** "Unsigned integer arithmetic is always performed modulo 2n where n is the number of bits in that particular integer."[^7] In the example, with a 64-bit `size_t`, 2^32 times 2^32 is 0, so a buffer of that size is too small. Compare with `SIZE_MAX` divided by the length before you multiply, as the solution does:

```c run id=c-product-wrap
#include <stdint.h>
#include <stdio.h>

int main(void) {
    size_t a = (size_t)1 << 32;
    size_t b = (size_t)1 << 32;
    // The product wraps around to 0.
    printf("%zu\n", a * b);
    // This test does not wrap around: 1 means that a * b does not fit.
    printf("%d\n", a > SIZE_MAX / b);
    return 0;
}
```

```output for=c-product-wrap
0
1
```

- **C++ reserve does not check the product.** `reserve` throws "std::length_error if new_cap > max_size()."[^8] But a product that wraps around is a small number. In the example, `reserve(a * a)` gets 0 and does not fail. Only a count that is too large gives the error:

```cpp run id=cpp-reserve-wrap
#include <cstddef>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

int main() {
    std::vector<std::vector<int>> out;
    std::size_t a = std::size_t{1} << 32;
    // a * a wraps around to 0, so reserve gets 0 and does not fail.
    out.reserve(a * a);
    std::cout << out.capacity() << '\n';
    try {
        out.reserve(std::numeric_limits<std::size_t>::max());
    } catch (const std::length_error &) {
        std::cout << "length_error\n";
    }
}
```

```output for=cpp-reserve-wrap
0
length_error
```

- **Bash brace expansion does not read variables.** Brace expansion makes a product of words: `{1,2}{a,b}` gives four words. But "Brace expansion is performed before any other expansions"[^9], so `{$x}` has no comma when Bash expands the braces. Then `$x` expands, and the braces stay in the words. Use loops for lists in variables, as the solution does:

```bash run id=bash-brace-product
echo {1,2}{a,b}
x="1,2"
echo {$x}{a,b}
```

```output for=bash-brace-product
1a 1b 2a 2b
{1,2}a {1,2}b
```

## Related tasks

- [[zip-two-lists]]: pair the items at the same index, and not each item with each item.
- [[compute-the-permutations-of-a-list]]: all the orders of the items of one list.
- [[compute-the-combinations-of-a-list]]: all the choices of k items from one list.
- [[flatten-a-nested-list]]: join a list of lists into one list.

## References
