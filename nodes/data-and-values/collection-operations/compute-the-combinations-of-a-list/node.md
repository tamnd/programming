---
tag: MXGKYK
kind: task
slug: compute-the-combinations-of-a-list
name: Compute the combinations of a list
aliases: [combinations, choose k items, k-subsets of a list, n choose k, itertools.combinations, all subsets of size k]
area: data-and-values/collection-operations
level: core
summary: Make each choice of k items of a list of integers, with the items in list order, in the lexicographic order of the positions.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/itertools.html#itertools.combinations
  title: The Python Standard Library, itertools
  locator: itertools.combinations(); itertools.combinations_with_replacement()
- url: https://docs.python.org/3/library/math.html#math.comb
  title: The Python Standard Library, math
  locator: Number-theoretic functions
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/fill
  title: MDN Web Docs, Array.prototype.fill()
  locator: Parameters
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Math.html
  title: Java SE 25 API, Class Math
  locator: multiplyExact(long, int)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.range
  title: .NET API reference, Enumerable.Range(Int32, Int32) Method
  locator: Exceptions
- url: https://go.dev/ref/spec#Integer_operators
  title: The Go Programming Language Specification
  locator: Integer operators
- url: https://doc.rust-lang.org/std/primitive.slice.html#method.windows
  title: Rust standard library, primitive type slice
  locator: windows
- url: https://en.cppreference.com/w/c/language/operator_arithmetic
  title: cppreference.com, Arithmetic operators (C)
  locator: Division
- url: https://en.cppreference.com/w/cpp/algorithm/prev_permutation
  title: cppreference.com, std::prev_permutation
  locator: Description
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Shell-Arithmetic
  title: Bash Reference Manual
  locator: 6.5 Shell Arithmetic
---

## Task

Make each choice of k items of a list of integers. The function `combinations(items, k)` returns the choices in the lexicographic order of the positions, and the items of each choice are in the order of the list. For example, `combinations([1, 2, 3, 4], 2)` is `[[1, 2], [1, 3], [1, 4], [2, 3], [2, 4], [3, 4]]`.

The order of the items in a choice does not matter, so `[2, 1]` is the same choice as `[1, 2]`, and the function gives only `[1, 2]`. A list of n items has C(n, k) choices, which is n! / (k! (n - k)!). k is 0 or more. k = 0 gives one empty choice. k more than n gives no choices. The length is at most 16, and the items are 32-bit signed integers. Items can be equal, and equal items at different positions count as different items.

## Solutions

| Language | Make each choice |
|---|---|
| Python | `[list(c) for c in itertools.combinations(items, k)]` |
| JavaScript, TypeScript | recursion that adds each item after the last chosen position to a path, then a copy of the full path |
| Java | the same recursion, with an `int[]` path and `path.clone()` |
| C# | the same recursion, with `(int[])path.Clone()` |
| Go | the same recursion, with `slices.Clone(path)` |
| Rust | the same recursion, with `path.clone()` |
| C | the next set of k increasing indices, in a loop |
| C++ | `std::prev_permutation` over a mask of k ones and then zeros |
| Bash | recursion that takes each argument in turn and then chooses from the arguments after it |

- **The order.** "The combination tuples are emitted in lexicographic order according to the order of the input iterable. If the input iterable is sorted, the output tuples will be produced in sorted order."[^1] The recursion gives the same order, because it tries the positions from the first to the last at each depth.
- **Positions, not values.** "Elements are treated as unique based on their position, not on their value."[^1] Thus `[1, 1, 2]` with k = 2 gives `[1, 2]` two times.
- **Stop early.** The recursion stops a loop when too few items are left to fill the choice. Thus it never starts a path that cannot reach k items, and k more than n gives no choices with no special code.
- **Count before you make them.** `math.comb(n, k)` gives the number of choices. C(16, 8) is 12870, but C(40, 20) is 137846528820, so a list of all the choices can be too large. Use the iterator directly when you need only some of the choices.
- **C.** The caller gives the array `out` and its capacity in items. Choice r is at `out[r * k]` to `out[r * k + k - 1]`. The function returns -1 when n is more than 32, when C(n, k) does not fit in a `size_t`, or when the choices do not fit in `out`. Then it writes nothing.
- **Bash.** The first argument is k, and each other argument is one item. The function prints each choice on one line.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `zero-k`: `[1, 2]` with k = 0 gives one empty choice, `[[]]`, because C(2, 0) is 1.
- `empty`: `[]` with k = 0 also gives `[[]]`.
- `k-too-large`: `[1, 2]` with k = 3 gives `[]`, because C(2, 3) is 0.
- `all`: `[1, 2, 3]` with k = 3 gives `[[1, 2, 3]]`.
- `duplicates`: `[1, 1, 2]` with k = 2 gives `[[1, 1], [1, 2], [1, 2]]`.
- `unsorted`: `[3, 1, 2]` with k = 2 gives `[[3, 1], [3, 2], [1, 2]]`. The items of a choice keep the order of the list. They are not sorted.
- `extremes`: -2147483648 and 2147483647 stay as they are.

## Pitfalls

- **Python combinations, combinations_with_replacement and product are different.** `combinations_with_replacement` gives "subsequences of elements from the input iterable allowing individual elements to be repeated more than once."[^1] `product` gives each ordered pair. For 3 items and k = 2, they give 3, 6 and 9 tuples. `math.comb` gives the "Number of ways to choose k items from n items without repetition and without order"[^2], which agrees with `combinations`:

```python run id=python-combination-kinds
import itertools
import math

items = [1, 2, 3]
print(len(list(itertools.combinations(items, 2))), math.comb(3, 2))
print(len(list(itertools.combinations_with_replacement(items, 2))))
print(len(list(itertools.product(items, repeat=2))))
```

```output for=python-combination-kinds
3 3
6
9
```

- **JavaScript fill puts one array in each slot.** To group the choices by size, do not make the groups with `Array(n).fill([])`. If the value "is an object, each slot in the array will reference that object."[^3] Thus each push goes to the same array, and each group has all 4 choices. `Array.from` with a function makes a new array for each slot:

```javascript run id=javascript-fill-shared
const combos = [[], [1], [2], [1, 2]];

const shared = Array(3).fill([]);
for (const c of combos) shared[c.length].push(c);
console.log(shared.map((b) => b.length).join(" "));

const own = Array.from({ length: 3 }, () => []);
for (const c of combos) own[c.length].push(c);
console.log(own.map((b) => b.length).join(" "));
```

```output for=javascript-fill-shared
4 4 4
1 2 1
```

- **Java the factorial overflows a long.** C(n, k) as n! / (k! (n - k)!) needs 21! for n = 21, and 21! is more than `Long.MAX_VALUE`. The `*=` operator wraps with no error. `Math.multiplyExact` "Returns the product of the arguments, throwing an exception if the result overflows a long."[^4] The product form `c * (n - i) / (i + 1)` needs no factorial, and each division in it is exact:

```java run id=java-factorial-overflow
public class Main {
  public static void main(String[] args) {
    long f = 1;
    for (int i = 2; i <= 21; i++) {
      f *= i;
    }
    System.out.println(f);

    long g = 1;
    try {
      for (int i = 2; i <= 21; i++) {
        g = Math.multiplyExact(g, i);
      }
    } catch (ArithmeticException e) {
      System.out.println("ArithmeticException after " + g);
    }

    // C(21, 10) with no factorial: each step is exact.
    long c = 1;
    for (int i = 0; i < 10; i++) {
      c = c * (21 - i) / (i + 1);
    }
    System.out.println(c);
  }
}
```

```output for=java-factorial-overflow
-4249290049419214848
ArithmeticException after 2432902008176640000
352716
```

- **C# Enumerable.Range does not accept a negative count.** The first item of a choice is at a position from 0 to n - k. When k is more than n, `Enumerable.Range(0, n - k + 1)` gets a negative count and throws `ArgumentOutOfRangeException`, because "count is less than 0."[^5] Check k first, as the solution does:

```csharp run id=csharp-range-negative
using System;
using System.Linq;

int n = 2;
int k = 4;
try
{
    // The first item of a combination is at position 0 to n - k.
    Console.WriteLine(Enumerable.Range(0, n - k + 1).Count());
}
catch (ArgumentOutOfRangeException)
{
    Console.WriteLine("ArgumentOutOfRangeException");
}
Console.WriteLine(k > n ? 0 : Enumerable.Range(0, n - k + 1).Count());
```

```output for=csharp-range-negative
ArgumentOutOfRangeException
0
```

- **Go bit masks do not give the lexicographic order.** A loop over the masks from 0 to 2^n - 1 that keeps the masks with k ones gives all the choices, but `[2 3]` (mask 6) comes before `[1 4]` (mask 9). Also, there is no limit on the shift count: "Shifts behave as if the left operand is shifted n times by 1 for a shift count of n."[^6] Thus `uint64(1) << 64` is 0, and the loop runs zero times for 64 items:

```go run id=go-bitmask-order
package main

import (
	"fmt"
	"math/bits"
)

func main() {
	items := []int{1, 2, 3, 4}
	var picks [][]int
	for mask := uint(0); mask < 1<<len(items); mask++ {
		if bits.OnesCount(mask) != 2 {
			continue
		}
		var pick []int
		for i, x := range items {
			if mask&(1<<i) != 0 {
				pick = append(pick, x)
			}
		}
		picks = append(picks, pick)
	}
	fmt.Println(picks)

	var n uint = 64
	fmt.Println(uint64(1) << n)
}
```

```output for=go-bitmask-order
[[1 2] [1 3] [2 3] [1 4] [2 4] [3 4]]
0
```

- **Rust windows are not combinations.** `windows(2)` "Returns an iterator over all contiguous windows of length size. The windows overlap."[^7] Thus it gives only the 3 pairs of neighbours of 4 items, and not the 6 pairs:

```rust run id=rust-windows-not-combinations
fn main() {
    let items = [1, 2, 3, 4];
    let windows: Vec<_> = items.windows(2).collect();
    println!("{} {:?}", windows.len(), windows);

    let mut pairs = Vec::new();
    for i in 0..items.len() {
        for j in i + 1..items.len() {
            pairs.push([items[i], items[j]]);
        }
    }
    println!("{} {:?}", pairs.len(), pairs);
}
```

```output for=rust-windows-not-combinations
3 [[1, 2], [2, 3], [3, 4]]
6 [[1, 2], [1, 3], [1, 4], [2, 3], [2, 4], [3, 4]]
```

- **C divide after you multiply.** When both operands are integers, "the result is the algebraic quotient (not a fraction)"[^8], and the fraction part is lost. `c *= (n - i) / (i + 1)` divides first, so 5 / 2 gives 2 and 4 / 3 gives 1. `c = c * (n - i) / (i + 1)` multiplies first, and then the division is exact. For C(6, 3), the first form gives 12, and the second gives the correct 20:

```c run id=c-division-order
#include <stdio.h>

int main(void) {
    int n = 6;
    int k = 3;
    int wrong = 1;
    int right = 1;
    for (int i = 0; i < k; i++) {
        wrong *= (n - i) / (i + 1); // 6 / 1, then 5 / 2 is 2, then 4 / 3 is 1
        right = right * (n - i) / (i + 1);
    }
    printf("%d %d\n", wrong, right);
    return 0;
}
```

```output for=c-division-order
12 20
```

- **C++ next_permutation on a mask gives the reverse order.** A mask of zeros and then k ones, with `next_permutation`, gives all the choices, but from the last to the first. `prev_permutation` "Permutes the target range [first, last) into the lexicographically previous permutation."[^9] Thus a mask of k ones and then zeros, with `prev_permutation`, gives the lexicographic order of the positions, as the solution does:

```cpp run id=cpp-next-permutation-mask
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// pick returns the chosen positions of mask, for example "{0,3}".
static std::string pick(const std::vector<char> &mask) {
    std::string s = "{";
    for (std::size_t i = 0; i < mask.size(); i++) {
        if (mask[i]) {
            s += (s.size() > 1 ? "," : "") + std::to_string(i);
        }
    }
    return s + "}";
}

int main() {
    std::vector<char> up{0, 0, 1, 1};
    std::string line;
    do {
        line += (line.empty() ? "" : " ") + pick(up);
    } while (std::next_permutation(up.begin(), up.end()));
    std::cout << line << '\n';

    std::vector<char> down{1, 1, 0, 0};
    line.clear();
    do {
        line += (line.empty() ? "" : " ") + pick(down);
    } while (std::prev_permutation(down.begin(), down.end()));
    std::cout << line << '\n';
}
```

```output for=cpp-next-permutation-mask
{2,3} {1,3} {1,2} {0,3} {0,2} {0,1}
{0,1} {0,2} {0,3} {1,2} {1,3} {2,3}
```

- **Bash arithmetic overflows with no error.** "Evaluation is done in the largest fixed-width integers available, with no check for overflow"[^10]. C(62, 31) is 465428353255261088, which fits in 64 bits. But from step 27 on, the product `c * (n - i)` is more than the largest 64-bit integer, so the result is a wrong negative number. Use a tool with big integers, such as `bc`, when the counts can be large:

```bash run id=bash-count-overflow
n=62
k=31
c=1
for ((i = 0; i < k; i++)); do
  ((c = c * (n - i) / (i + 1)))
done
echo "$c"
```

```output for=bash-count-overflow
-284401161134521734
```

## Related tasks

- [[compute-the-permutations-of-a-list]]: make each order of the items, where the order matters.
- [[compute-the-cartesian-product]]: take one item from each of several lists.
- [[remove-duplicates-from-a-list]]: remove equal items first to get each distinct choice once.

## References
