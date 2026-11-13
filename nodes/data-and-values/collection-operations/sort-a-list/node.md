---
tag: GZXT2V
kind: task
slug: sort-a-list
name: Sort a list
aliases: [sort an array, sort numbers, order a list, ascending order, sort integers, numeric sort]
area: data-and-values/collection-operations
level: intro
summary: Sort a list of integers in ascending numeric order, and do not compare the numbers as text.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Arrays.html
  title: Java SE 25 API, Class Arrays
  locator: sort(int[])
- url: https://pkg.go.dev/slices#Sort
  title: Go standard library, package slices
  locator: Sort, Clone
- url: https://en.cppreference.com/w/c/algorithm/qsort
  title: cppreference.com, qsort, qsort_s
  locator: Example
- url: https://man7.org/linux/man-pages/man1/sort.1.html
  title: Linux manual page, sort(1)
  locator: -n, --numeric-sort
- url: https://docs.python.org/3/library/stdtypes.html#list.sort
  title: The Python Standard Library, Built-in Types
  locator: list.sort()
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
  title: MDN Web Docs, Array.prototype.sort()
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.order?view=net-10.0
  title: .NET API reference, Enumerable.Order method
  locator: Remarks
- url: https://doc.rust-lang.org/std/primitive.slice.html#method.sort_unstable
  title: Rust standard library, primitive type slice
  locator: sort_unstable, sort_by
- url: https://en.cppreference.com/w/cpp/algorithm/sort
  title: cppreference.com, std::sort
  locator: Type requirements
---

## Task

Sort a list of integers in ascending order. The function `sort_ints(items)` returns the items from the smallest to the largest. For example, `sort_ints([3, 1, 2])` is `[1, 2, 3]`, and `sort_ints([10, 9, 100, 1])` is `[1, 9, 10, 100]`.

The comparison is by value, not by the text of the numbers, so 9 comes before 10. Equal items stay, so the result has the same length as the input. The items are 32-bit signed integers.

## Solutions

| Language | Sort the integers |
|---|---|
| Python | `sorted(items)` |
| JavaScript, TypeScript | `[...items].sort((a, b) => a - b)` |
| Java | `Arrays.sort` on a copy |
| C# | `Array.Sort` on a copy |
| Go | `slices.Sort` on `slices.Clone(items)` |
| Rust | `sort_unstable` on `items.to_vec()` |
| C | `qsort` with a compare function, in place |
| C++ | `std::ranges::sort` |
| Bash | `printf '%s\n' "$@" \| sort -n` |

- **Java.** `Arrays.sort(int[])` "Sorts the specified array into ascending numerical order"[^1]. The solution sorts a copy, so the input does not change.
- **Go.** "Sort sorts a slice of any ordered type in ascending order"[^2]. `slices.Clone` makes the copy.
- **Rust.** Equal integers are the same value, so a stable sort and an unstable sort give the same result.
- **C.** The compare function returns `(x > y) - (x < y)`. The cppreference example calls `arg1 - arg2` an "erroneous shortcut: undefined behavior in case of integer overflow"[^3].
- **C++.** The solution takes the vector by value, sorts it and returns it.
- **Bash.** `sort -n` compares "according to string numerical value"[^4]. `LC_ALL=C` gives the same result on each machine.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty` and `single`: an empty list and a list of one item are already sorted.
- `sorted` and `reversed`: the input can be in order, or in the opposite order.
- `duplicates`: equal items stay next to each other in the result.
- `negative`: -10 comes before -1.
- `digits`: 10, 9, 100 and 1 sort to 1, 9, 10, 100. A text sort gives 1, 10, 100, 9.
- `extremes`: -2147483648 and 2147483647 are the two ends. A compare function with `x - y` overflows for them.

## Pitfalls

- **Python list.sort returns None.** "To remind users that it operates by side effect, it does not return the sorted sequence"[^5]. Use `sorted` for a new list:

```python run id=python-sort-none
items = [3, 1, 2]
result = items.sort()
print(result, items)
print(sorted([3, 1, 2]))
```

```output for=python-sort-none
None [1, 2, 3]
[1, 2, 3]
```

- **JavaScript sort compares text.** "The default sort order is ascending, built upon converting the elements into strings, then comparing their sequences of UTF-16 code unit values"[^6]. Give a compare function for numbers:

```javascript run id=javascript-sort-text
console.log([10, 9, 100, 1].sort());
console.log([10, 9, 100, 1].sort((a, b) => a - b));
```

```output for=javascript-sort-text
[ 1, 10, 100, 9 ]
[ 1, 9, 10, 100 ]
```

- **Java a compare function with x - y overflows.** In the example, `Integer.MIN_VALUE - 1` overflows, so the sort puts `Integer.MIN_VALUE` after 1. Use `Integer::compare`:

```java run id=java-compare-overflow
import java.util.Arrays;

public class Main {
  public static void main(String[] args) {
    Integer[] a = {Integer.MIN_VALUE, 1, 0};
    Arrays.sort(a, (x, y) -> x - y);
    System.out.println(Arrays.toString(a));
    Arrays.sort(a, Integer::compare);
    System.out.println(Arrays.toString(a));
  }
}
```

```output for=java-compare-overflow
[0, 1, -2147483648]
[-2147483648, 0, 1]
```

- **C# Order makes a new sequence.** It does not change the list. "This method is implemented by using deferred execution"[^7], so a call that drops the result does nothing. Keep the result, or use `List.Sort`:

```csharp run id=csharp-order-new
var items = new List<int> { 3, 1, 2 };
items.Order();
Console.WriteLine(string.Join(" ", items));
var sorted = items.Order().ToList();
Console.WriteLine(string.Join(" ", sorted));
```

```output for=csharp-order-new
3 1 2
1 2 3
```

- **Go slices.Sort changes the shared array.** In the example, `b := a` does not copy the items, so the sort of `b` also changes `a`. Sort a clone:

```go run id=go-sort-shared
package main

import (
	"fmt"
	"slices"
)

func main() {
	a := []int{3, 1, 2}
	b := a
	slices.Sort(b)
	fmt.Println(a, b)
	c := []int{3, 1, 2}
	d := slices.Clone(c)
	slices.Sort(d)
	fmt.Println(c, d)
}
```

```output for=go-sort-shared
[1 2 3] [1 2 3]
[3 1 2] [1 2 3]
```

- **Rust sort of f64 needs total_cmp.** "Sorting types that only implement PartialOrd such as f32 and f64 require additional precautions"[^8]. In the example, `sort_by(f64::total_cmp)` puts NaN at the end:

```rust run id=rust-sort-floats
fn main() {
    let mut xs = [2.5, f64::NAN, -1.0, 0.0];
    xs.sort_by(f64::total_cmp);
    println!("{xs:?}");
}
```

```output for=rust-sort-floats
[-1.0, 0.0, 2.5, NaN]
```

- **C compare functions must not subtract.** The example sorts `INT_MIN` and `INT_MAX` with the safe compare function:

```c run id=c-qsort-compare
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int compare(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int main(void) {
    int a[] = {1, INT_MIN, 0, INT_MAX};
    qsort(a, 4, sizeof a[0], compare);
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);
    return 0;
}
```

```output for=c-qsort-compare
-2147483648 0 1 2147483647
```

- **C++ std::sort needs random access.** "RandomIt must meet the requirements of LegacyRandomAccessIterator"[^9]. A `std::list` does not have such iterators, so use its member `sort`. For descending order, give `std::greater{}`:

```cpp run id=cpp-sort-list
#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <vector>

int main() {
    std::list<int> l{3, 1, 2};
    // std::sort(l.begin(), l.end()) does not compile: list iterators are not
    // random access. The member function sort works.
    l.sort();
    for (int x : l) {
        std::cout << x << (x == l.back() ? "\n" : " ");
    }
    std::vector<int> v{3, 1, 2};
    std::ranges::sort(v, std::greater{});
    for (int x : v) {
        std::cout << x << (x == v.back() ? "\n" : " ");
    }
}
```

```output for=cpp-sort-list
1 2 3
3 2 1
```

- **Bash sort without -n compares text.** Give `-n` for numbers. It also sorts negative numbers in the correct order:

```bash run id=bash-sort-numeric
printf '%s\n' 10 9 100 1 | LC_ALL=C sort | paste -s -d ' ' -
printf '%s\n' 10 9 100 1 | LC_ALL=C sort -n | paste -s -d ' ' -
printf '%s\n' 5 -10 -1 | LC_ALL=C sort -n | paste -s -d ' ' -
```

```output for=bash-sort-numeric
1 10 100 9
1 9 10 100
-10 -1 5
```

## Related tasks

- [[sort-with-a-custom-key]]: sort by a value that the code computes from each item.
- [[sort-a-list-of-records]]: sort records by a field, with a stable sort.
- [[search-a-sorted-list]]: find an item in a sorted list.
- [[find-the-largest-element]]: find the largest item without a sort.

## References
