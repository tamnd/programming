---
tag: T5W8S8
kind: task
slug: search-a-sorted-list
name: Search a sorted list
aliases: [binary search, bisect, find in a sorted array, lower bound, index of a value, search a sorted array]
area: data-and-values/collection-operations
level: core
summary: Find the first index of a value in a sorted list of integers with a binary search, or give -1 when the value is not in the list.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/bisect.html#bisect.bisect_left
  title: The Python Standard Library, bisect
  locator: bisect.bisect_left()
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Arrays.html
  title: Java SE 25 API, Class Arrays
  locator: binarySearch(int[], int)
- url: https://pkg.go.dev/slices#BinarySearch
  title: Go standard library, package slices
  locator: BinarySearch
- url: https://en.cppreference.com/w/c/algorithm/bsearch
  title: cppreference.com, bsearch, bsearch_s
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/indexOf
  title: MDN Web Docs, Array.prototype.indexOf()
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.array.binarysearch?view=net-10.0
  title: .NET API reference, Array.BinarySearch method
  locator: Returns
- url: https://doc.rust-lang.org/std/primitive.slice.html#method.binary_search
  title: Rust standard library, primitive type slice
  locator: binary_search, partition_point
- url: https://en.cppreference.com/w/cpp/algorithm/binary_search
  title: cppreference.com, std::binary_search
  locator: Return value
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Bash-Conditional-Expressions
  title: Bash Reference Manual
  locator: 3.2.5.2 Conditional Constructs, [[...]]
---

## Task

Find a value in a list of integers in ascending order. The function `index_of(items, target)` returns the first index of `target`, or -1 if no item is equal to it. For example, `index_of([1, 3, 5, 7, 9], 5)` is 2, and `index_of([1, 3, 5, 7, 9], 4)` is -1.

The list can have equal items. For `index_of([1, 2, 2, 2, 3], 2)`, the result is 1, the first of the equal items. The list can be empty, and the items are 32-bit signed integers.

A binary search uses the order of the list. It compares the target with the middle item, and then searches only the half that can contain the target.

## Solutions

| Language | Find the first index of target |
|---|---|
| Python | `bisect_left(items, target)`, then a check of the item |
| JavaScript, TypeScript | a binary search loop for the first item that is not less than target |
| Java | the same loop; `Arrays.binarySearch` can give any equal item |
| C# | the same loop; `Array.BinarySearch` can give any equal item |
| Go | `slices.BinarySearch(items, target)`, then a check of `found` |
| Rust | `items.partition_point(\|&x\| x < target)`, then a check of the item |
| C | the same loop as JavaScript, with `size_t` indexes |
| C++ | `std::ranges::lower_bound(items, target)`, then a check of the item |
| Bash | the same loop, with `(( ))` for the comparisons |

- **The loop.** The loop keeps two indexes, `lo` and `hi`. Each item before `lo` is less than the target, and each item from `hi` is not less than the target. At the end, `lo` is the first index of an item that is not less than the target. The middle index is `lo + (hi - lo) / 2`, because `lo + hi` can be too large for the integer type.
- **Python.** For `bisect_left`, "If x is already present in a, the insertion point will be before (to the left of) any existing entries"[^1]. Thus the result is the first of the equal items.
- **Java.** "If the array contains multiple elements with the specified value, there is no guarantee which one will be found."[^2] Thus the solution uses its own loop.
- **Go.** `slices.BinarySearch` "returns the earliest position where target is found, or the position where target would appear in the sort order"[^3].
- **C.** For `bsearch`, "it is unspecified which element the function will return as the result"[^4] when there are equal items. Thus the solution uses its own loop.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `first` and `last`: the target can be at index 0 or at the last index.
- `missing`, `below` and `above`: a target that is not in the list gives -1. It can be between two items, before the first item or after the last item.
- `empty`: an empty list gives -1 for each target.
- `single-hit` and `single-miss`: a list of one item.
- `duplicates` and `all-same`: with equal items, the result is the first index.
- `negative`: the list can have negative items.
- `extremes` and `min-target`: the items and the target can be -2147483648 or 2147483647.

## Pitfalls

- **Python bisect_left gives an insertion point.** It does not tell if the target is in the list. In the example, `bisect_left([1, 3, 5], 4)` is 2, but 4 is not in the list. Check the item at the index:

```python run id=python-bisect-insertion-point
from bisect import bisect_left

items = [1, 3, 5]
i = bisect_left(items, 4)
print(i)
print(i if i < len(items) and items[i] == 4 else -1)
```

```output for=python-bisect-insertion-point
2
-1
```

- **JavaScript indexOf is a linear search, and it does not find NaN.** JavaScript has no binary search function for arrays. `indexOf` "compares searchElement to elements of the array using strict equality"[^5], so it gives -1 for `NaN`. Use `findIndex(Number.isNaN)`:

```javascript run id=javascript-indexof-nan
const items = [1, NaN, 3];
console.log(items.indexOf(NaN));
console.log(items.findIndex(Number.isNaN));
```

```output for=javascript-indexof-nan
-1
1
```

- **Java binarySearch gives a negative number for a missing key.** The result is "(-(insertion point) - 1)"[^2], not -1. In the example, the result for 4 is -3:

```java run id=java-binarysearch-negative
import java.util.Arrays;

public class Main {
  public static void main(String[] args) {
    int[] items = {1, 3, 5};
    int r = Arrays.binarySearch(items, 4);
    System.out.println(r);
    System.out.println(r >= 0 ? r : -1);
  }
}
```

```output for=java-binarysearch-negative
-3
-1
```

- **C# BinarySearch gives a bitwise complement for a missing value.** The result is "the bitwise complement of the index of the first element that is larger than value"[^6]. `~r` gives the index back:

```csharp run id=csharp-binarysearch-complement
using System;

int[] items = [1, 3, 5];
int r = Array.BinarySearch(items, 4);
Console.WriteLine(r);
Console.WriteLine(~r);
```

```output for=csharp-binarysearch-complement
-3
2
```

- **Go BinarySearch gives a position also for a missing target.** It "also returns a bool saying whether the target is really found in the slice"[^3]. Check the bool:

```go run id=go-binarysearch-found
package main

import (
	"fmt"
	"slices"
)

func main() {
	i, found := slices.BinarySearch([]int{1, 3, 5}, 4)
	fmt.Println(i, found)
}
```

```output for=go-binarysearch-found
2 false
```

- **Rust binary_search can give any of the equal items.** "If there are multiple matches, then any one of the matches could be returned."[^7] Also, "The index is chosen deterministically, but is subject to change in future versions of Rust"[^7]. For the first index, use `partition_point`:

```rust run id=rust-binary-search-duplicates
fn main() {
    let items = [1, 2, 2, 2, 3];
    // Any of the indexes 1, 2 and 3 is a correct result of binary_search.
    let r = items.binary_search(&2);
    println!("{}", matches!(r, Ok(1..=3)));
    println!("{}", items.partition_point(|&x| x < 2));
}
```

```output for=rust-binary-search-duplicates
true
1
```

- **C bsearch gives a pointer.** It gives `NULL` for a missing key. For the index, subtract the start of the array from the pointer. The compare function must not subtract the two integers, because the subtraction can overflow:

```c run id=c-bsearch-pointer
#include <stdio.h>
#include <stdlib.h>

static int cmp(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int main(void) {
    int items[] = {1, 3, 5, 7};
    size_t n = sizeof items / sizeof items[0];
    int key = 5;
    const int *p = bsearch(&key, items, n, sizeof items[0], cmp);
    printf("%td\n", p - items);
    key = 4;
    p = bsearch(&key, items, n, sizeof items[0], cmp);
    printf("%s\n", p == NULL ? "NULL" : "found");
    return 0;
}
```

```output for=c-bsearch-pointer
2
NULL
```

- **C++ binary_search gives only a bool.** The result is "true if an element equivalent to value exists, false otherwise"[^8]. For the index, use `std::lower_bound`:

```cpp run id=cpp-binary-search-bool
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 2, 2, 3};
    std::cout << std::boolalpha << std::binary_search(v.begin(), v.end(), 2) << '\n';
    auto it = std::lower_bound(v.begin(), v.end(), 2);
    std::cout << (it - v.begin()) << '\n';
}
```

```output for=cpp-binary-search-bool
true
1
```

- **Bash [[ < ]] compares text.** The operators "sort lexicographically using the current locale"[^9]. In the example, `[[ 10 < 9 ]]` is true. Use `(( ))` to compare numbers:

```bash run id=bash-compare-text
if [[ 10 < 9 ]]; then
  echo "text: 10 < 9"
fi
if ((10 < 9)); then
  echo "number: 10 < 9"
else
  echo "number: 10 >= 9"
fi
```

```output for=bash-compare-text
text: 10 < 9
number: 10 >= 9
```

## Related tasks

- [[sort-a-list]]: sort the list before the search.
- [[check-whether-a-list-contains-a-value]]: test for a value in a list that is not sorted.
- [[find-the-largest-element]]: find a value without a sorted list.
- [[filter-a-list]]: find all the items that pass a test.
- [[rotate-a-list]]: a rotated sorted list is not sorted, so a binary search does not work on it.

## References
