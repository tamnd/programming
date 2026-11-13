---
tag: HTH996
kind: task
slug: flatten-a-nested-list
name: Flatten a nested list
aliases: [flatten a list of lists, concatenate lists, join lists, flat, flatMap, SelectMany]
area: data-and-values/collection-operations
level: intro
summary: Join a list of lists of integers into one list, one level deep, with the items in order.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/itertools.html#itertools.chain.from_iterable
  title: The Python Standard Library, itertools
  locator: chain.from_iterable()
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/flat
  title: MDN Web Docs, Array.prototype.flat()
  locator: Parameters
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.selectmany?view=net-10.0
  title: .NET API reference, Enumerable.SelectMany method
  locator: Definition
- url: https://doc.rust-lang.org/std/primitive.slice.html#method.concat
  title: Rust standard library, primitive type slice
  locator: concat
- url: https://pkg.go.dev/slices#Concat
  title: Go standard library, package slices
  locator: func Concat
- url: https://docs.python.org/3/reference/expressions.html#displays-for-lists-sets-and-dictionaries
  title: The Python Language Reference, Expressions
  locator: 6.2.4 Displays for lists, sets and dictionaries
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Stream.html
  title: Java SE 25 API, Interface Stream
  locator: flatMap(Function)
- url: https://go.dev/ref/spec#Appending_and_copying_slices
  title: The Go Programming Language Specification
  locator: Appending to and copying slices
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.flatten
  title: Rust standard library, trait Iterator
  locator: flatten; copied
- url: https://en.cppreference.com/w/c/language/operator_arithmetic
  title: cppreference.com, Arithmetic operators (C)
  locator: Overflows
- url: https://en.cppreference.com/w/cpp/ranges/join_view
  title: cppreference.com, std::ranges::join_view
  locator: Description
- url: https://tiswww.case.edu/php/chet/bash/bashref.html
  title: Bash Reference Manual
  locator: 6.7 Arrays; 4.2 Bash Builtin Commands (read)
---

## Task

Join a list of lists of integers into one list, one level deep. The function `flatten(lists)` returns the items of the first inner list, then the items of the second inner list, and so on. For example, `flatten([[1, 2], [3], [4, 5, 6]])` is `[1, 2, 3, 4, 5, 6]`.

The items are 32-bit signed integers. The outer list and each inner list can be empty. An item is not a list, so the function flattens one level only.

## Solutions

| Language | Join the inner lists |
|---|---|
| Python | `list(chain.from_iterable(lists))` |
| JavaScript, TypeScript | `lists.flat()` |
| Java | `Arrays.stream(lists).flatMapToInt(Arrays::stream).toArray()` |
| C# | `lists.SelectMany(xs => xs).ToArray()` |
| Go | `slices.Concat(lists...)` |
| Rust | `lists.concat()` |
| C | a check of the total length, then `memcpy` of each list |
| C++ | `reserve` of the total length, then `insert` of each list |
| Bash | `read -r -a` of each argument, then `printf` of all the items |

- **One level.** `chain.from_iterable` is an "Alternate constructor for chain(). Gets chained inputs from a single iterable argument that is evaluated lazily."[^1] `flat` takes a depth, which "Defaults to 1."[^2] `SelectMany` "Projects each element of a sequence to an IEnumerable<T> and flattens the resulting sequences into one sequence."[^3] `concat` "Flattens a slice of T into a single value Self::Output."[^4]
- **A new list.** Each solution makes a new list, so a change to the result does not change the input. For Go, "Concat returns a new slice concatenating the passed in slices."[^5] The Go test also checks this.
- **C.** The caller gives the array `out` and its capacity. The function returns -1 when the items do not fit, and then it writes nothing.
- **Bash.** An array cannot have arrays in it. Thus each argument is one inner list, with the items separated by spaces. The function prints one item on each line.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: no inner lists give an empty list. For Go, "If the concatenation is empty, the result is nil."[^5]
- `empty-inner`: `[[], [1], []]` gives `[1]`.
- `all-empty`: `[[], []]` gives an empty list.
- `single` and `one-list`: one inner list gives a copy of it.
- `negative` and `extremes`: negative items, -2147483648 and 2147483647 stay in their order.

## Pitfalls

- **Python the order of the for clauses.** The items of a comprehension come from "considering each of the for or if clauses a block, nesting from left to right"[^6]. Thus the outer loop comes first: `[x for xs in lists for x in xs]`. With the other order, `xs` is not defined yet. Also, `chain.from_iterable` flattens one level only, so `[2]` stays a list:

```python run id=python-comprehension-order
from itertools import chain

lists = [[1, 2], [3], [4, 5]]
print([x for xs in lists for x in xs])
try:
    print([x for x in xs for xs in lists])
except NameError as e:
    print(type(e).__name__, e)
print(list(chain.from_iterable(lists)))
print(list(chain.from_iterable([[1, [2]], [3]])))
```

```output for=python-comprehension-order
[1, 2, 3, 4, 5]
NameError name 'xs' is not defined
[1, 2, 3, 4, 5]
[1, [2], 3]
```

- **JavaScript flat flattens one level.** The depth is "The depth level specifying how deep a nested array structure should be flattened. Defaults to 1."[^2] In the example, `[2, [3]]` stays a list after `flat()`. `flat(Infinity)` flattens all the levels:

```javascript run id=javascript-flat-depth
const nested = [[1, [2, [3]]], [4]];
console.log(JSON.stringify(nested.flat()));
console.log(JSON.stringify(nested.flat(Infinity)));
```

```output for=javascript-flat-depth
[1,[2,[3]],4]
[1,2,3,4]
```

- **Java map gives a stream of streams.** `map(List::stream)` gives one item for each inner list, so the count is 2. `flatMap` "Returns a stream consisting of the results of replacing each element of this stream with the contents of a mapped stream"[^7]:

```java run id=java-map-flatmap
import java.util.List;

public class Main {
  public static void main(String[] args) {
    List<List<Integer>> lists = List.of(List.of(1, 2), List.of(3));
    System.out.println(lists.stream().map(List::stream).count());
    System.out.println(lists.stream().flatMap(List::stream).toList());
  }
}
```

```output for=java-map-flatmap
2
[1, 2, 3]
```

- **C# Select does not flatten.** `Select(xs => xs)` gives one array for each inner array, so the count is 2. `SelectMany` "flattens the resulting sequences into one sequence."[^3]

```csharp run id=csharp-select-selectmany
using System;
using System.Linq;

int[][] lists = { new[] { 1, 2 }, new[] { 3 } };
Console.WriteLine(lists.Select(xs => xs).Count());
Console.WriteLine(string.Join(" ", lists.SelectMany(xs => xs)));
```

```output for=csharp-select-selectmany
2
1 2 3
```

- **Go append to the first list can change it.** "If the capacity of s is not large enough to fit the additional values, append allocates a new, sufficiently large underlying array"[^8]. "Otherwise, append re-uses the underlying array."[^8] In the example, `a` has a capacity of 10, so `out` shares its array, and `out[0] = 99` changes `a`. `slices.Concat` "returns a new slice"[^5]:

```go run id=go-append-shares
package main

import (
	"fmt"
	"slices"
)

func main() {
	a := make([]int, 2, 10)
	b := []int{3}
	out := append(a, b...)
	out[0] = 99
	fmt.Println(a, out)

	a = make([]int, 2, 10)
	out = slices.Concat(a, b)
	out[0] = 99
	fmt.Println(a, out)
}
```

```output for=go-append-shares
[99 0] [99 0 3]
[0 0] [99 0 3]
```

- **Rust flatten of iter gives references.** `flatten` "Creates an iterator that flattens nested structure."[^9] But `lists.iter()` gives references, so the items are `&i32`. `copied` "Creates an iterator which copies all of its elements."[^9] `concat` gives a `Vec<i32>`:

```rust run id=rust-flatten-refs
fn main() {
    let lists = vec![vec![1, 2], vec![3]];
    let refs: Vec<&i32> = lists.iter().flatten().collect();
    println!("{refs:?}");
    let copies: Vec<i32> = lists.iter().flatten().copied().collect();
    println!("{copies:?} {:?}", lists.concat());
}
```

```output for=rust-flatten-refs
[1, 2, 3]
[1, 2, 3] [1, 2, 3]
```

- **C the sum of the lengths can wrap around.** "Unsigned integer arithmetic is always performed modulo 2n where n is the number of bits in that particular integer."[^10] In the example, `SIZE_MAX + 2` is 1, so a buffer of that size is too small. Compare each length with the room that is left, as the solution does:

```c run id=c-length-wrap
#include <stdint.h>
#include <stdio.h>

int main(void) {
    size_t a = SIZE_MAX;
    size_t b = 2;
    // The sum wraps around to 1.
    printf("%zu\n", a + b);
    // This test does not wrap around: 1 means that a + b does not fit.
    printf("%d\n", b > SIZE_MAX - a);
    return 0;
}
```

```output for=c-length-wrap
1
1
```

- **C++ views::join reads the lists in the loop.** `views::join` "represents view consisting of the sequence obtained from flattening a view of ranges."[^11] In the example, the change to `lists[0][0]` after the join shows in the loop. The solution copies the items into a new vector:

```cpp run id=cpp-join-view
#include <iostream>
#include <ranges>
#include <vector>

int main() {
    std::vector<std::vector<int>> lists{{1, 2}, {3}};
    auto joined = lists | std::views::join;
    lists[0][0] = 9;
    for (int x : joined) {
        std::cout << x << '\n';
    }
}
```

```output for=cpp-join-view
9
2
3
```

- **Bash arrays cannot nest.** "Bash provides one-dimensional indexed and associative array variables."[^12] Thus an item of `lists` is a string. With `read -a`, "The words are assigned to sequential indices of the array variable aname, starting at 0."[^12]

```bash run id=bash-array-nest
lists=("1 2" "3")
echo "${#lists[@]} ${lists[0]}"
read -r -a first <<< "${lists[0]}"
echo "${#first[@]} ${first[1]}"
```

```output for=bash-array-nest
2 1 2
2 2
```

## Related tasks

- [[iterate-in-chunks]]: split a list into lists of a fixed size, the reverse of this task.
- [[group-items-by-a-key]]: split a list into lists by a key.
- [[zip-two-lists]]: pair the items of two lists.
- [[compute-the-cartesian-product]]: a nested loop over two lists.

## References

