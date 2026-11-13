---
tag: 3HDHGW
kind: task
slug: rotate-a-list
name: Rotate a list
aliases: [rotate an array, left rotation, circular shift, cyclic shift, shift a list, rotate left]
area: data-and-values/collection-operations
level: intro
summary: Move the first k items of a list to its end, keep the order of the other items, and make a k that is larger than the list work.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://learn.microsoft.com/en-us/dotnet/csharp/tutorials/ranges-indexes
  title: Microsoft Learn, Indices and ranges (C#)
  locator: Language support for indices and ranges
- url: https://doc.rust-lang.org/std/primitive.slice.html#method.rotate_left
  title: Rust standard library, primitive type slice
  locator: rotate_left
- url: https://en.cppreference.com/w/cpp/algorithm/rotate
  title: cppreference.com, std::rotate
  locator: Description
- url: https://docs.python.org/3/library/collections.html#collections.deque.rotate
  title: The Python Standard Library, collections
  locator: deque.rotate()
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Remainder
  title: MDN Web Docs, Remainder (%)
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Collections.html
  title: Java SE 25 API, Class Collections
  locator: rotate(List, int)
- url: https://go.dev/ref/spec#Arithmetic_operators
  title: The Go Programming Language Specification
  locator: Integer operators
- url: https://en.cppreference.com/w/c/string/byte/memcpy
  title: cppreference.com, memcpy, memcpy_s
  locator: Description
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Shell-Parameter-Expansion
  title: Bash Reference Manual
  locator: 3.5.3 Shell Parameter Expansion
---

## Task

Rotate a list of integers to the left by k positions. The function `rotate_left(items, k)` moves the first k items to the end, and keeps the order of the other items. For example, `rotate_left([1, 2, 3, 4, 5], 2)` is `[3, 4, 5, 1, 2]`.

The function returns a new list, and the input does not change. k is 0 or more, and it can be larger than the length of the list. A rotation by the length of the list gives the same list, so the function uses the remainder of k divided by the length. An empty list gives an empty list for each k.

## Solutions

| Language | Rotate to the left by k |
|---|---|
| Python | `items[s:] + items[:s]`, with `s = k % len(items)` |
| JavaScript, TypeScript | `[...items.slice(s), ...items.slice(0, s)]` |
| Java | two `System.arraycopy` calls into a new array |
| C# | `[.. items[s..], .. items[..s]]` |
| Go | `slices.Concat(items[s:], items[:s])` |
| Rust | `rotate_left(k % v.len())` on `items.to_vec()` |
| C | `out[i] = items[(i + s) % n]` for each index |
| C++ | `std::ranges::rotate` on a copy |
| Bash | `"${items[@]:s}" "${items[@]:0:s}"` |

- **Each language.** Each solution first checks for an empty list, because the remainder of a division by 0 is not defined. Then it computes `s`, the remainder of k divided by the length.
- **C#.** "The start of the range is inclusive, but the end of the range is exclusive"[^1]. Thus `items[..s]` is the first s items, and `items[s..]` is the other items.
- **Rust.** After `rotate_left(mid)`, "the element previously at index mid will become the first element in the slice"[^2]. The rotation is in place, so the solution rotates a copy.
- **C++.** `std::rotate` "Performs a left rotation on the target range [first, last)"[^3]. The middle iterator is the item that becomes the first item.
- **Bash.** `${items[@]:s}` expands to the items from index s to the end, and `${items[@]:0:s}` expands to the first s items.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `zero` and `full-turn`: a rotation by 0 or by the length gives the same list.
- `more-than-length` and `much-larger`: a rotation by 4 of a list of 3 items is a rotation by 1. A k of 2000000000 also works, because the remainder is 2.
- `empty` and `empty-zero`: an empty list gives an empty list. A division by its length 0 is not possible, so the solutions check for it first.
- `single`: a list of one item does not change.
- `duplicates` and `negative-items`: the values of the items do not change the rotation.

## Pitfalls

- **Python deque.rotate rotates to the right.** "Rotate the deque n steps to the right. If n is negative, rotate to the left."[^4] For a left rotation by k, give `-k`:

```python run id=python-deque-rotate
from collections import deque

d = deque([1, 2, 3, 4, 5])
d.rotate(2)
print(list(d))
d = deque([1, 2, 3, 4, 5])
d.rotate(-2)
print(list(d))
```

```output for=python-deque-rotate
[4, 5, 1, 2, 3]
[3, 4, 5, 1, 2]
```

- **JavaScript % can give a negative result.** The remainder "always takes the sign of the dividend"[^5]. For a negative k, `k % n` is negative. If a negative k is a right rotation, use `((k % n) + n) % n`:

```javascript run id=javascript-negative-remainder
const n = 3;
const k = -1;
console.log(k % n);
console.log(((k % n) + n) % n);
```

```output for=javascript-negative-remainder
-1
2
```

- **Java Collections.rotate rotates to the right.** After the call, "the element at index i will be the element previously at index (i - distance) mod list.size()"[^6]. Thus a positive distance moves the items to the right. For a left rotation by k, give `-k`:

```java run id=java-collections-rotate
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Main {
  public static void main(String[] args) {
    List<Integer> right = new ArrayList<>(List.of(1, 2, 3, 4, 5));
    Collections.rotate(right, 2);
    System.out.println(right);
    List<Integer> left = new ArrayList<>(List.of(1, 2, 3, 4, 5));
    Collections.rotate(left, -2);
    System.out.println(left);
  }
}
```

```output for=java-collections-rotate
[4, 5, 1, 2, 3]
[3, 4, 5, 1, 2]
```

- **C# a range past the end throws.** In the example, `items[4..]` on an array of 3 items throws an `ArgumentOutOfRangeException`. Use the remainder of k first:

```csharp run id=csharp-range-past-end
using System;

int[] items = [1, 2, 3];
int k = 4;
try
{
    int[] rest = items[k..];
    Console.WriteLine(rest.Length);
}
catch (ArgumentOutOfRangeException e)
{
    Console.WriteLine(e.GetType().Name);
}
int s = k % items.Length;
Console.WriteLine(string.Join(" ", [.. items[s..], .. items[..s]]));
```

```output for=csharp-range-past-end
ArgumentOutOfRangeException
2 3 1
```

- **Go % can give a negative result.** Integer division in Go has "x / y truncated towards zero"[^7], so `-1 % 3` is -1. Add n and use % again:

```go run id=go-negative-remainder
package main

import "fmt"

func main() {
	n, k := 3, -1
	fmt.Println(k % n)
	fmt.Println((k%n + n) % n)
}
```

```output for=go-negative-remainder
-1
2
```

- **Rust rotate_left panics for a large k.** "This function will panic if mid is greater than the length of the slice."[^2] Use `k % len`:

```rust run id=rust-rotate-panic
fn main() {
    // Hide the panic message, so the example prints only its own lines.
    std::panic::set_hook(Box::new(|_| {}));
    let result = std::panic::catch_unwind(|| {
        let mut v = vec![1, 2, 3];
        v.rotate_left(4);
        v
    });
    println!("panic: {}", result.is_err());
    let mut v = vec![1, 2, 3];
    let k = 4;
    let len = v.len();
    v.rotate_left(k % len);
    println!("{v:?}");
}
```

```output for=rust-rotate-panic
panic: true
[2, 3, 1]
```

- **C memcpy must not copy to an overlapping area.** "If the objects overlap (which is a violation of the restrict contract)"[^8], the behavior of `memcpy` is undefined. An in-place rotation moves items inside one array, so use `memmove` for that step:

```c run id=c-rotate-memmove
#include <stdio.h>
#include <string.h>

int main(void) {
    int a[] = {1, 2, 3, 4, 5};
    size_t n = sizeof a / sizeof a[0];
    size_t s = 2;
    int first[2];
    memcpy(first, a, s * sizeof a[0]);
    // The source and the destination overlap, so use memmove, not memcpy.
    memmove(a, a + s, (n - s) * sizeof a[0]);
    memcpy(a + n - s, first, s * sizeof a[0]);
    for (size_t i = 0; i < n; i++) {
        printf("%d%s", a[i], i + 1 < n ? " " : "\n");
    }
    return 0;
}
```

```output for=c-rotate-memmove
3 4 5 1 2
```

- **C++ std::rotate rotates to the left.** For a right rotation, give reverse iterators:

```cpp run id=cpp-rotate-reverse
#include <algorithm>
#include <iostream>
#include <vector>

static void print(const std::vector<int> &v) {
    for (std::size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << (i + 1 < v.size() ? " " : "\n");
    }
}

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};
    // The middle iterator is the item that becomes the first item.
    std::rotate(v.begin(), v.begin() + 2, v.end());
    print(v);
    std::vector<int> w{1, 2, 3, 4, 5};
    // With reverse iterators, the rotation is to the right.
    std::rotate(w.rbegin(), w.rbegin() + 2, w.rend());
    print(w);
}
```

```output for=cpp-rotate-reverse
3 4 5 1 2
4 5 1 2 3
```

- **Bash needs a space before a negative offset.** "a negative offset must be separated from the colon by at least one space"[^9]. In the example, `${a[@]: -2}` gives the last two items. `${a[@]:-2}` is a default value expansion, so it gives all the items:

```bash run id=bash-negative-offset
a=(1 2 3 4 5)
echo "${a[@]: -2}"
echo "${a[@]:-2}"
echo "${a[@]:2}" "${a[@]:0:2}"
```

```output for=bash-negative-offset
4 5
1 2 3 4 5
3 4 5 1 2
```

## Related tasks

- [[iterate-in-chunks]]: split a list into parts of a fixed size.
- [[partition-a-list]]: split a list by a test.
- [[zip-two-lists]]: combine the items of two lists at the same index.
- [[map-over-a-list]]: make a new list from each item.

## References
