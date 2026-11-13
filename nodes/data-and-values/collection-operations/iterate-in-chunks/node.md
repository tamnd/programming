---
tag: GWQASC
kind: task
slug: iterate-in-chunks
name: Iterate in chunks
aliases: [split a list into chunks, batch a list, chunk a list, groups of n items, partition into fixed-size parts]
area: data-and-values/collection-operations
level: intro
summary: Split a list of integers into consecutive chunks of a fixed size, where the last chunk can be shorter.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/itertools.html#itertools.batched
  title: The Python Standard Library, itertools
  locator: itertools.batched()
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.chunk?view=net-10.0
  title: .NET API reference, Enumerable.Chunk method
  locator: Exceptions, Remarks
- url: https://docs.python.org/3/library/functions.html#zip
  title: The Python Standard Library, Built-in Functions
  locator: zip()
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/splice
  title: MDN Web Docs, Array.prototype.splice()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/List.html
  title: Java SE 25 API, Interface List
  locator: subList(int, int)
- url: https://pkg.go.dev/slices#Chunk
  title: Go standard library, package slices
  locator: func Chunk
- url: https://doc.rust-lang.org/std/primitive.slice.html#method.chunks_exact
  title: Rust standard library, primitive type slice
  locator: chunks_exact
- url: https://en.cppreference.com/w/c/language/operator_arithmetic
  title: cppreference.com, Arithmetic operators (C)
  locator: Overflows
- url: https://en.cppreference.com/w/cpp/container/span/subspan
  title: cppreference.com, std::span::subspan
  locator: Description
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Shell-Parameter-Expansion
  title: Bash Reference Manual
  locator: 3.5.3 Shell Parameter Expansion
---

## Task

Split a list of integers into consecutive chunks of a fixed size. The function `chunks(items, size)` returns the chunks in order. Each chunk has `size` items, except the last chunk, which can be shorter. For example, `chunks([1, 2, 3, 4, 5], 2)` is `[[1, 2], [3, 4], [5]]`.

The size is at least 1. The items are 32-bit signed integers, and the list can be empty. Then there are no chunks, so the result is an empty list, not a list with one empty chunk.

## Solutions

| Language | Split into chunks of size items |
|---|---|
| Python | `itertools.batched`, with a list of each chunk |
| JavaScript, TypeScript | a loop with `slice(i, i + size)` |
| Java | a loop with `Arrays.copyOfRange` |
| C# | `items.Chunk(size).ToArray()` |
| Go | `slices.Collect(slices.Chunk(items, size))` |
| Rust | `items.chunks(size)`, with a `Vec` of each chunk |
| C | a loop that writes a pointer and a length for each chunk |
| C++ | a loop with `std::span::subspan` |
| Bash | a loop with `${items[*]:i:size}` |

- **The last chunk.** `batched` gives tuples, and "The last batch may be shorter than n."[^1] For `Chunk`, "Each chunk except the last one will be of size size."[^2] The other solutions use the smaller of `size` and the number of items that are left.
- **The number of chunks.** The C and Java solutions calculate the number of chunks as `n / size` plus 1 when `n % size` is not 0.
- **C.** Each chunk is a pointer into the input and a length, so the function does not copy the items.
- **Bash.** The function prints one line for each chunk, with the items separated by spaces.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: an empty list gives no chunks.
- `size-one`: each chunk has 1 item.
- `size-larger`: `chunks([1, 2], 5)` is `[[1, 2]]`, one chunk with all the items.
- `size-equal`: `chunks([1, 2, 3], 3)` is `[[1, 2, 3]]`.
- `last-shorter`: `chunks([1, 2, 3, 4], 3)` is `[[1, 2, 3], [4]]`.
- `negative` and `extremes`: negative items, -2147483648 and 2147483647 stay in their chunks.

## Pitfalls

- **Python the zip idiom drops the last short chunk.** `zip(*[iter(s)]*n)` repeats one iterator n times. "This has the effect of dividing the input into n-length chunks."[^3] But `zip` stops at the shortest input, so the last item 5 has no chunk. Use `batched`:

```python run id=python-zip-chunks
from itertools import batched

items = [1, 2, 3, 4, 5]
print(list(zip(*[iter(items)] * 2)))
print(list(batched(items, 2)))
```

```output for=python-zip-chunks
[(1, 2), (3, 4)]
[(1, 2), (3, 4), (5,)]
```

- **JavaScript splice empties the input.** "The splice() method of Array instances changes the contents of an array by removing or replacing existing elements and/or adding new elements in place."[^4] In the example, the chunks are correct, but `items` is empty after the loop. Use `slice`, which makes a copy:

```javascript run id=javascript-splice-input
const items = [1, 2, 3, 4, 5];
const out = [];
while (items.length > 0) {
  out.push(items.splice(0, 2));
}
console.log(out);
console.log(items);
```

```output for=javascript-splice-input
[ [ 1, 2 ], [ 3, 4 ], [ 5 ] ]
[]
```

- **Java subList is a view of the list.** "The returned list is backed by this list, so non-structural changes in the returned list are reflected in this list, and vice-versa."[^5] In the example, `chunk.set(0, 99)` changes `items`. A new `ArrayList` of the sublist is a copy:

```java run id=java-sublist-view
import java.util.ArrayList;
import java.util.List;

public class Main {
  public static void main(String[] args) {
    List<Integer> items = new ArrayList<>(List.of(1, 2, 3, 4));
    List<Integer> chunk = items.subList(0, 2);
    chunk.set(0, 99);
    System.out.println(items);
    List<Integer> copy = new ArrayList<>(items.subList(2, 4));
    copy.set(0, 0);
    System.out.println(items);
  }
}
```

```output for=java-sublist-view
[99, 2, 3, 4]
[99, 2, 3, 4]
```

- **C# Chunk throws for a size below 1.** `Chunk` throws `ArgumentOutOfRangeException` when "size is below 1"[^2]:

```csharp run id=csharp-chunk-size
using System;
using System.Linq;

int[] items = { 1, 2, 3 };
Console.WriteLine(items.Chunk(2).Count());
try
{
    items.Chunk(0).ToArray();
}
catch (ArgumentOutOfRangeException e)
{
    Console.WriteLine(e.GetType().Name);
}
```

```output for=csharp-chunk-size
2
ArgumentOutOfRangeException
```

- **Go an append to a chunk can change the next chunk.** `a[0:2]` has the capacity of the rest of `a`, so `append` writes 99 into `a[2]`. For `slices.Chunk`, "All sub-slices are clipped to have no capacity beyond the length."[^6] Thus `append` makes a new array, and `b` does not change:

```go run id=go-chunk-capacity
package main

import (
	"fmt"
	"slices"
)

func main() {
	a := []int{1, 2, 3, 4}
	c := a[0:2]
	c = append(c, 99)
	fmt.Println(a, c)

	b := []int{1, 2, 3, 4}
	for d := range slices.Chunk(b, 2) {
		d = append(d, 99)
		fmt.Println(b, d)
		break
	}
}
```

```output for=go-chunk-capacity
[1 2 99 4] [1 2 99]
[1 2 3 4] [1 2 99]
```

- **Rust chunks_exact drops the last short chunk.** With `chunks_exact`, "the last up to chunk_size-1 elements will be omitted and can be retrieved from the remainder function of the iterator"[^7]. In the example, 5 is only in the remainder. Use `chunks` to keep the last short chunk:

```rust run id=rust-chunks-exact
fn main() {
    let items = [1, 2, 3, 4, 5];
    let exact = items.chunks_exact(2);
    println!("{:?}", exact.remainder());
    println!("{:?}", exact.collect::<Vec<_>>());
    println!("{:?}", items.chunks(2).collect::<Vec<_>>());
}
```

```output for=rust-chunks-exact
[5]
[[1, 2], [3, 4]]
[[1, 2], [3, 4], [5]]
```

- **C (n + size - 1) / size can wrap around.** "Unsigned integer arithmetic is always performed modulo"[^8] a power of 2. When `n` is `SIZE_MAX`, `n + size - 1` wraps around, and the number of chunks is 0. `n / size + (n % size != 0)` has no overflow:

```c run id=c-chunk-count-overflow
#include <stdint.h>
#include <stdio.h>

int main(void) {
    size_t n = SIZE_MAX;
    size_t size = 2;
    size_t wrong = (n + size - 1) / size;
    size_t right = n / size + (n % size != 0);
    printf("%zu %d\n", wrong, right == SIZE_MAX / 2 + 1);
    return 0;
}
```

```output for=c-chunk-count-overflow
0 1
```

- **C++ subspan refers to the input, and its count must stay in the span.** `subspan` "Obtains a subview over some consecutive elements of this span"[^9], so `first[0] = 9` changes `items[0]`. For a count past the end, "If the implementation is not hardened, the behavior is undefined."[^9] Use the smaller of the size and the number of items that are left:

```cpp run id=cpp-subspan-view
#include <algorithm>
#include <iostream>
#include <span>
#include <vector>

int main() {
    std::vector<int> items{1, 2, 3, 4, 5};
    std::span<int> all(items);
    auto first = all.subspan(0, 2);
    first[0] = 9;
    std::cout << items[0] << '\n';
    std::size_t i = 4;
    auto last = all.subspan(i, std::min<std::size_t>(2, all.size() - i));
    std::cout << last.size() << '\n';
}
```

```output for=cpp-subspan-view
9
1
```

- **Bash `${items:0:2}` is a part of the first item.** "Referencing an array variable without a subscript is equivalent to referencing with a subscript of 0."[^10] Thus `${items:0:2}` is the first 2 characters of `123`. With `@`, "the result is the length members of the array beginning with `${parameter[offset]}`"[^10]:

```bash run id=bash-array-slice
#!/usr/bin/env bash
items=(123 456 789)
echo "${items:0:2}"
echo "${items[@]:0:2}"
```

```output for=bash-array-slice
12
123 456
```

## Related tasks

- [[partition-a-list]]: split a list into two parts by a test.
- [[group-items-by-a-key]]: split a list into groups by a key.
- [[zip-two-lists]]: pair the items of two lists.
- [[rotate-a-list]]: move the first items to the end.

## References
