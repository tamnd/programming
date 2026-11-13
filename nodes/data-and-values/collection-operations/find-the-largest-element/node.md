---
tag: WE5KZ7
kind: task
slug: find-the-largest-element
name: Find the largest element
aliases: [maximum of a list, max element, greatest element of a list, largest number in an array, argmax]
area: data-and-values/collection-operations
level: intro
summary: Find the largest item of a non-empty list of integers, and know what each language does for an empty list.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/functions.html#max
  title: The Python Standard Library, Built-in Functions
  locator: max()
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math/max
  title: MDN Web Docs, Math.max()
  locator: Return value, Getting the maximum element of an array
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.max?view=net-10.0
  title: .NET API reference, Enumerable.Max method
  locator: Exceptions
- url: https://pkg.go.dev/slices#Max
  title: Go standard library, package slices
  locator: Max
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.max
  title: Rust standard library, trait Iterator
  locator: max, max_by_key
- url: https://en.cppreference.com/w/cpp/algorithm/max_element
  title: cppreference.com, std::max_element
  locator: Return value
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Collections.html
  title: Java SE 25 API, Class Collections
  locator: max(Collection)
- url: https://en.cppreference.com/w/c/language/operator_arithmetic
  title: cppreference.com, C arithmetic operators
  locator: Overflows
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Bash-Conditional-Expressions
  title: Bash Reference Manual
  locator: 6.4 Bash Conditional Expressions
---

## Task

Find the largest item of a list of integers. The function `largest(items)` returns the largest value. For example, `largest([3, 1, 4, 1, 5])` is 5, and `largest([-3, -1, -2])` is -1.

The list must have at least one item, because an empty list has no largest item. Each solution fails for an empty list: it raises an exception or it panics. The items are 32-bit signed integers.

## Solutions

| Language | Find the largest item |
|---|---|
| Python | `max(items)` |
| JavaScript, TypeScript | a loop over the items |
| Java | `Arrays.stream(items).max().orElseThrow()` |
| C# | `items.Max()` |
| Go | `slices.Max(items)` |
| Rust | `items.iter().max()`, which gives an `Option` |
| C | a loop that starts from `items[0]` |
| C++ | `std::ranges::max_element(items)` |
| Bash | a loop with `((x > best))` |

- **Python.** "If multiple items are maximal, the function returns the first one encountered"[^1]. Equal integers are the same value, so the result does not change.
- **JavaScript and TypeScript.** `Math.max(...items)` gives each item as an argument. MDN says that "both spread (...) and apply will either fail or return the wrong result if the array has too many elements"[^2]. The solution uses a loop, and it throws a `RangeError` for an empty array.
- **Java.** The solution uses `IntStream.max` and `orElseThrow`. In the tests, an empty array throws `NoSuchElementException`.
- **C#.** `Max` throws `InvalidOperationException` if the "source contains no elements"[^3].
- **Go.** `slices.Max` "returns the maximal value in x. It panics if x is empty"[^4].
- **Rust.** "If the iterator is empty, None is returned"[^5]. The solution calls `expect`, so an empty slice panics.
- **C.** The loop starts from `items[0]`, not from 0. A start value of 0 gives 0 for a list of negative items.
- **C++.** `max_element` returns an iterator. "If the target range is empty, returns last"[^6], so the solution tests for an empty span before it reads the item.
- **Bash.** `((x > best))` compares the arguments as numbers.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `single`: the one item is the largest item.
- `all-negative` and `zero`: the largest of -3, -1 and -2 is -1. A start value of 0 gives a wrong result for these lists.
- `first` and `last`: the largest item can be at each end of the list.
- `duplicates` and `all-same`: the largest value can occur more than once. The result is that value.
- `extremes` and `min-only`: the items can be -2147483648 and 2147483647, the limits of a 32-bit signed integer.

The native tests of Python, JavaScript, TypeScript, Java, C#, Go, Rust and C++ also check that an empty list fails.

## Pitfalls

- **Python max of an empty list raises ValueError.** "If the iterable is empty and default is not provided, a ValueError is raised"[^1]. Give `default` if the list can be empty:

```python run id=python-max-empty
items = []
try:
    max(items)
except ValueError as e:
    print(type(e).__name__)
print(max(items, default=None))
```

```output for=python-max-empty
ValueError
None
```

- **JavaScript Math.max with no arguments is -Infinity.** `Math.max` returns "-Infinity if there are no parameters"[^2]. In the example, a spread of 10,000,000 items throws a `RangeError`, and `reduce` works:

```javascript run id=javascript-max-spread
console.log(Math.max(...[]));
const big = Array.from({ length: 10_000_000 }, (_, i) => i);
try {
  console.log(Math.max(...big));
} catch (e) {
  console.log(e.name);
}
console.log(big.reduce((a, b) => Math.max(a, b), -Infinity));
```

```output for=javascript-max-spread
-Infinity
RangeError
9999999
```

- **Java start value 0 is wrong for negative items.** In the example, the loop that starts from 0 gives 0 for `{-3, -1, -2}`. Also, `Collections.max` throws "NoSuchElementException - if the collection is empty"[^7]:

```java run id=java-max-start
import java.util.ArrayList;
import java.util.Collections;
import java.util.NoSuchElementException;

public class Main {
  public static void main(String[] args) {
    int[] items = {-3, -1, -2};
    int wrong = 0;
    for (int x : items) {
      if (x > wrong) {
        wrong = x;
      }
    }
    int best = items[0];
    for (int x : items) {
      if (x > best) {
        best = x;
      }
    }
    System.out.println(wrong + " " + best);
    try {
      Collections.max(new ArrayList<Integer>());
    } catch (NoSuchElementException e) {
      System.out.println("NoSuchElementException");
    }
  }
}
```

```output for=java-max-start
0 -1
NoSuchElementException
```

- **C# Max of an empty array throws.** In the example, `Max` of an empty `int[]` throws, and `Max` of an empty `int?[]` gives `null`:

```csharp run id=csharp-max-empty
int[] empty = [];
try
{
    Console.WriteLine(empty.Max());
}
catch (InvalidOperationException)
{
    Console.WriteLine("InvalidOperationException");
}
int?[] none = [];
Console.WriteLine(none.Max() is null);
```

```output for=csharp-max-empty
InvalidOperationException
True
```

- **Go slices.Max panics for an empty slice.** If the slice can be empty, test its length first, and return a second value that tells if there is a result:

```go run id=go-max-empty
package main

import (
	"fmt"
	"slices"
)

func largest(items []int) (int, bool) {
	if len(items) == 0 {
		return 0, false
	}
	return slices.Max(items), true
}

func main() {
	fmt.Println(largest(nil))
	defer func() { fmt.Println("panic:", recover()) }()
	slices.Max([]int{})
}
```

```output for=go-max-empty
0 false
panic: slices.Max: empty list
```

- **Rust max gives the last of equal items.** "If several elements are equally maximum, the last element is returned"[^5]. In the example, `max_by_key` with the length gives `"cc"`, not `"bb"`. For `f64` items, the example uses `fold` with `f64::max`:

```rust run id=rust-max-last
fn main() {
    let empty: [i32; 0] = [];
    println!("{:?}", empty.iter().max());
    let words = ["bb", "a", "cc"];
    println!("{:?}", words.iter().max_by_key(|w| w.len()));
    let xs = [2.5_f64, -1.0, 7.0];
    println!("{}", xs.iter().copied().fold(f64::NEG_INFINITY, f64::max));
}
```

```output for=rust-max-last
None
Some("cc")
7
```

- **C n - 1 for an empty list.** The count `n` is a `size_t`, which is unsigned. "Unsigned integer arithmetic is always performed modulo"[^8] a power of 2, so `0 - 1` is the largest `size_t` value. Test `n` first:

```c run id=c-size-wrap
#include <stdio.h>

int main(void) {
    size_t n = 0;
    // n - 1 wraps around. A loop "i <= n - 1" runs for a very long time.
    printf("%zu\n", n - 1);
    // Test n first, and loop with i < n.
    if (n == 0) {
        printf("empty\n");
    }
    return 0;
}
```

```output for=c-size-wrap
18446744073709551615
empty
```

- **C++ max_element of an empty range is the end.** Do not dereference the end iterator. Also, "If there are multiple greatest elements, returns the iterator to the first one of them"[^6]. In the example, the index of the first 9 is 1:

```cpp run id=cpp-max-element-end
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> empty;
    auto it = std::ranges::max_element(empty);
    std::cout << (it == empty.end()) << "\n";
    std::vector<int> v{3, 9, 9, 1};
    std::cout << std::ranges::distance(v.begin(), std::ranges::max_element(v)) << "\n";
}
```

```output for=cpp-max-element-end
1
1
```

- **Bash [[ > ]] compares text.** "string1 > string2 True if string1 sorts after string2 lexicographically"[^9]. The text 10 sorts before the text 9. Use `(( ))` for numbers:

```bash run id=bash-text-compare
best=9
x=10
if [[ $x > $best ]]; then echo "text: 10 > 9"; else echo "text: 10 < 9"; fi
if ((x > best)); then echo "number: 10 > 9"; else echo "number: 10 < 9"; fi
```

```output for=bash-text-compare
text: 10 < 9
number: 10 > 9
```

## Related tasks

- [[sort-a-list]]: the last item of a sorted list is the largest, but the sort does more work.
- [[reduce-a-list-to-a-value]]: the largest item is a reduce with the larger of two values.
- [[filter-a-list]]: keep the items that pass a test.
- [[map-over-a-list]]: change each item of a list.

## References
