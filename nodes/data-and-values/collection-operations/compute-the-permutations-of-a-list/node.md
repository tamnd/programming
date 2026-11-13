---
tag: 12763Q
kind: task
slug: compute-the-permutations-of-a-list
name: Compute the permutations of a list
aliases: [permutations, all orders of a list, all arrangements of the items, next permutation, itertools.permutations, permute a list]
area: data-and-values/collection-operations
level: core
summary: Make each order of the items of a list of integers, in the lexicographic order of the positions.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/itertools.html#itertools.permutations
  title: The Python Standard Library, itertools
  locator: itertools.permutations()
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
  title: MDN Web Docs, Array.prototype.sort()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/ArrayList.html
  title: Java SE 25 API, Class ArrayList
  locator: ArrayList(Collection)
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/arithmetic-operators
  title: C# language reference, Arithmetic operators
  locator: Integer arithmetic overflow
- url: https://go.dev/ref/spec#Appending_and_copying_slices
  title: The Go Programming Language Specification
  locator: Appending to and copying slices
- url: https://doc.rust-lang.org/std/primitive.slice.html#method.swap
  title: Rust standard library, primitive type slice
  locator: swap
- url: https://en.cppreference.com/w/c/language/operator_arithmetic
  title: cppreference.com, Arithmetic operators (C)
  locator: Overflows
- url: https://en.cppreference.com/w/cpp/algorithm/next_permutation
  title: cppreference.com, std::next_permutation
  locator: Description
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Shell-Functions
  title: Bash Reference Manual
  locator: 3.3 Shell Functions
---

## Task

Make each order of the items of a list of integers. The function `permutations(items)` returns the orders in the lexicographic order of the positions: the input order comes first, and the reverse order comes last. For example, `permutations([1, 2, 3])` is `[[1, 2, 3], [1, 3, 2], [2, 1, 3], [2, 3, 1], [3, 1, 2], [3, 2, 1]]`.

A list of n items has n! orders, so the length is at most 8, which gives 40320 orders. The empty list has one order, the empty list. Items can be equal, and equal items at different positions count as different items. The items are 32-bit signed integers.

## Solutions

| Language | Make each order |
|---|---|
| Python | `[list(p) for p in itertools.permutations(items)]` |
| JavaScript, TypeScript | recursion that adds each unused item to a path, then a copy of the full path |
| Java | the same recursion, with a `boolean[]` of used positions and `path.clone()` |
| C# | the same recursion, with `(int[])path.Clone()` |
| Go | the same recursion, with `slices.Clone(path)` |
| Rust | the same recursion, with `path.clone()` |
| C | the next order of an array of indices, in a `do` loop |
| C++ | `std::next_permutation` over a vector of indices from `std::iota` |
| Bash | recursion that takes each argument in turn, with `local` variables |

- **The order.** "The permutation tuples are emitted in lexicographic order according to the order of the input iterable. If the input iterable is sorted, the output tuples will be produced in sorted order."[^1] The recursion gives the same order, because it tries the positions from the first to the last at each depth.
- **Positions, not values.** "Elements are treated as unique based on their position, not on their value."[^1] Thus `[1, 1, 2]` gives 6 orders, and some of them are equal. The C and C++ solutions get this rule from the indices: the indices are all different, so each order of the indices is one order of the positions.
- **The count grows fast.** The number of orders of n items is n!. For 10 items it is 3628800, and for 13 items it is more than a 32-bit integer can hold. Python can count the orders with `math.perm(n)` before it makes them.
- **Lazy orders.** `itertools.permutations` makes one order at a time. Use the iterator directly when you need only some of the orders, or when you can stop early. The solution makes a list because the task asks for all the orders.
- **C.** The caller gives the array `out` and its capacity in items. Order r is at `out[r * n]` to `out[r * n + n - 1]`. The function returns -1 when n is more than 12 or when the orders do not fit, and then it writes nothing.
- **Bash.** Each argument is one item. The function prints each order on one line.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: `[]` gives one order, `[[]]`, because 0! is 1. Python agrees: `list(itertools.permutations([]))` is `[()]`.
- `single`: `[7]` gives `[[7]]`.
- `duplicates`: `[1, 1, 2]` gives 6 orders, with each distinct order two times.
- `unsorted`: `[3, 1, 2]` gives `[3, 1, 2]` first and `[2, 1, 3]` last. The orders are not sorted by value.
- `extremes`: -2147483648 and 2147483647 stay as they are.

## Pitfalls

- **Python equal items give equal orders.** `permutations` treats items "as unique based on their position, not on their value."[^1] Thus `[1, 1, 2]` gives 6 orders, and the first and the third are equal. To get each distinct order once, put the orders in a set:

```python run id=python-equal-items
from itertools import permutations

orders = list(permutations([1, 1, 2]))
print(len(orders), orders[0] == orders[2])
print(sorted(set(orders)))
```

```output for=python-equal-items
6 True
[(1, 1, 2), (1, 2, 1), (2, 1, 1)]
```

- **JavaScript sort compares numbers as text.** To get the orders in sorted order, sort the input first. But "The default sort order is ascending, built upon converting the elements into strings, then comparing their sequences of UTF-16 code unit values."[^2] Thus 10 comes before 9. Give a compare function:

```javascript run id=javascript-sort-numbers
const items = [10, 9, 1];
console.log(JSON.stringify([...items].sort()));
console.log(JSON.stringify([...items].sort((a, b) => a - b)));
```

```output for=javascript-sort-numbers
[1,10,9]
[1,9,10]
```

- **Java the result holds the path, not a copy.** In the example, the first run adds `path` itself to `out` six times. The recursion then removes all the items from `path`, so each item of `out` is the same empty list. The copy constructor "Constructs a list containing the elements of the specified collection"[^3], so the second run keeps each order:

```java run id=java-path-copy
import java.util.ArrayList;
import java.util.List;

public class Main {
  static void visit(List<Integer> items, List<Integer> path, List<List<Integer>> out, boolean copy) {
    if (path.size() == items.size()) {
      out.add(copy ? new ArrayList<>(path) : path);
      return;
    }
    for (Integer x : items) {
      if (path.contains(x)) {
        continue;
      }
      path.add(x);
      visit(items, path, out, copy);
      path.remove(path.size() - 1);
    }
  }

  public static void main(String[] args) {
    for (boolean copy : new boolean[] {false, true}) {
      List<List<Integer>> out = new ArrayList<>();
      visit(List.of(1, 2, 3), new ArrayList<>(), out, copy);
      System.out.println(out);
    }
  }
}
```

```output for=java-path-copy
[[], [], [], [], [], []]
[[1, 2, 3], [1, 3, 2], [2, 1, 3], [2, 3, 1], [3, 1, 2], [3, 2, 1]]
```

- **C# the count of orders overflows with no error.** "By default, arithmetic operations occur in an unchecked context."[^4] There, "the result is truncated by discarding any high-order bits that don't fit in the destination type."[^4] Thus 13! in an `int` is a wrong number. `checked` gives an `OverflowException`:

```csharp run id=csharp-factorial-overflow
using System;

int f = 1;
for (int n = 1; n <= 13; n++)
{
    f *= n;
}
Console.WriteLine(f);

int g = 1;
try
{
    for (int n = 1; n <= 13; n++)
    {
        g = checked(g * n);
    }
}
catch (OverflowException)
{
    Console.WriteLine($"OverflowException after {g}");
}
```

```output for=csharp-factorial-overflow
1932053504
OverflowException after 479001600
```

- **Go append can share the array of the prefix.** "If the capacity of s is not large enough to fit the additional values, append allocates a new, sufficiently large underlying array"[^5]. "Otherwise, append re-uses the underlying array."[^5] In the example, `prefix` has room for 4 items, so `a` and `b` use its array, and the second `append` changes `a`. A recursion that gives `append(prefix, x)` to each call has the same problem. Copy each full order, as the solution does with `slices.Clone`:

```go run id=go-append-prefix
package main

import "fmt"

func main() {
	prefix := make([]int, 1, 4)
	prefix[0] = 1
	a := append(prefix, 2)
	b := append(prefix, 3)
	fmt.Println(a, b)
}
```

```output for=go-append-prefix
[1 3] [1 3]
```

- **Rust swaps do not give the lexicographic order.** A common recursion swaps the item at depth k with each later item, and `swap` "Swaps two elements in the slice."[^6] This makes all n! orders, but in a different order: `321` comes before `312`. Sort the result, or use the recursion with a list of used positions, as the solution does:

```rust run id=rust-swap-order
fn swaps(v: &mut Vec<i32>, k: usize, out: &mut Vec<String>) {
    if k == v.len() {
        out.push(v.iter().map(|x| x.to_string()).collect());
        return;
    }
    for i in k..v.len() {
        v.swap(k, i);
        swaps(v, k + 1, out);
        v.swap(k, i);
    }
}

fn main() {
    let mut out = Vec::new();
    swaps(&mut vec![1, 2, 3], 0, &mut out);
    println!("{}", out.join(" "));
    let mut sorted = out.clone();
    sorted.sort();
    println!("{}", sorted.join(" "));
}
```

```output for=rust-swap-order
123 132 213 231 321 312
123 132 213 231 312 321
```

- **C n! does not fit in an int for n of 13 or more.** "When signed integer arithmetic operation overflows (the result does not fit in the result type), the behavior is undefined"[^7]. 13! is 6227020800, which is more than `INT32_MAX`. The example counts in `uint64_t`, so it does not overflow. The solution limits n to 12, and counts in `size_t`:

```c run id=c-factorial-size
#include <stdint.h>
#include <stdio.h>

int main(void) {
    uint64_t f = 1;
    for (int n = 1; n <= 13; n++) {
        f *= (uint64_t)n;
        if (n >= 12) {
            // 1 means that n! does not fit in a 32-bit int.
            printf("%d! = %llu %d\n", n, (unsigned long long)f, f > INT32_MAX);
        }
    }
    return 0;
}
```

```output for=c-factorial-size
12! = 479001600 0
13! = 6227020800 1
```

- **C++ next_permutation starts from the given order.** It "Permutes the target range [first, last) into the lexicographically next permutation."[^8] After the last order, it "transforms the range into the lexicographically first permutation"[^8] and returns false. Thus a loop from `{2, 1, 3}` sees only 4 orders, the ones that come after it. Also, it orders by value, so `{1, 1, 2}` gives 3 orders, not 6. Sort the input first to get all the distinct orders, or permute indices, as the solution does:

```cpp run id=cpp-next-permutation-start
#include <algorithm>
#include <iostream>
#include <vector>

static int count_orders(std::vector<int> v) {
    int n = 0;
    do {
        n++;
    } while (std::next_permutation(v.begin(), v.end()));
    return n;
}

int main() {
    std::cout << count_orders({1, 2, 3}) << '\n';
    std::cout << count_orders({2, 1, 3}) << '\n';
    std::cout << count_orders({1, 1, 2}) << '\n';
}
```

```output for=cpp-next-permutation-start
6
4
3
```

- **Bash a recursive function needs local variables.** "Ordinarily, variables and their values are shared between a function and its caller."[^9] In `perm`, the inner call changes `i` and `rest` of the outer call, so the loop stops after the first order. `perm_local` declares them with `local`:

```bash run id=bash-local-recursion
perm() { # perm PREFIX ITEM...: no local variables
  prefix=$1
  shift
  if (($# == 0)); then
    echo "$prefix"
    return
  fi
  rest=("$@")
  for ((i = 0; i < ${#rest[@]}; i++)); do
    perm "$prefix${rest[i]}" "${rest[@]:0:i}" "${rest[@]:i+1}"
  done
}

perm_local() { # perm_local PREFIX ITEM...
  local prefix=$1 i
  shift
  if (($# == 0)); then
    echo "$prefix"
    return
  fi
  local -a rest=("$@")
  for ((i = 0; i < ${#rest[@]}; i++)); do
    perm_local "$prefix${rest[i]}" "${rest[@]:0:i}" "${rest[@]:i+1}"
  done
}

perm "" 1 2 3 | paste -sd ' ' -
perm_local "" 1 2 3 | paste -sd ' ' -
```

```output for=bash-local-recursion
123
123 132 213 231 312 321
```

## Related tasks

- [[compute-the-combinations-of-a-list]]: choose k items, where the order does not matter.
- [[compute-the-cartesian-product]]: take one item from each of several lists.
- [[sort-a-list]]: sort the input to get the orders in sorted order.
- [[remove-duplicates-from-a-list]]: keep each distinct order once.

## References
