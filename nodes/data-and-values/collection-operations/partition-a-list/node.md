---
tag: PVEFE0
kind: task
slug: partition-a-list
name: Partition a list
aliases: [split a list by a predicate, partition by a test, separate odd and even, partitioningBy, split into two lists]
area: data-and-values/collection-operations
level: intro
summary: Split a list into the items that pass a test and the items that do not, here the odd and the even integers, and keep the order of the items in each part.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.partition
  title: Rust standard library, trait Iterator
  locator: partition
- url: https://docs.python.org/3/library/itertools.html#itertools.filterfalse
  title: The Python Standard Library, itertools
  locator: itertools.filterfalse()
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/groupBy
  title: MDN Web Docs, Object.groupBy()
  locator: Return value
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Collectors.html
  title: Java SE 25 API, Class Collectors
  locator: partitioningBy(Predicate)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.lookup-2.item?view=net-10.0
  title: .NET API reference, Lookup<TKey,TElement>.Item[TKey] property
  locator: Remarks
- url: https://go.dev/wiki/SliceTricks
  title: Go Wiki, SliceTricks
  locator: Filtering without allocating
- url: https://en.cppreference.com/w/c/language/operator_arithmetic
  title: cppreference.com, Arithmetic operators (C)
  locator: Multiplicative operators
- url: https://en.cppreference.com/w/cpp/algorithm/partition
  title: cppreference.com, std::partition
  locator: See also
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Conditional-Constructs
  title: Bash Reference Manual
  locator: 3.2.5.2 Conditional Constructs, ((...))
---

## Task

Split a list of integers into two lists: the odd items and the even items. The function `partition_odd(items)` returns the two lists as a pair, the odd items first. Each list keeps the input order. For example, `partition_odd([1, 2, 3, 4, 5])` is `([1, 3, 5], [2, 4])`.

An item is odd if the remainder of the item divided by 2 is not 0, so -3 is odd. Each item goes into exactly one of the two lists. The items are 32-bit signed integers, and the list can be empty. Then the result is two empty lists.

## Solutions

| Language | Split into the odd and the even items |
|---|---|
| Python | a loop that appends each item to one of two lists |
| JavaScript, TypeScript | a loop that pushes each item to one of two arrays |
| Java | two `filter` calls on `Arrays.stream(items)` |
| C# | a loop that adds each item to one of two lists |
| Go | a loop that appends each item to one of two new slices |
| Rust | `items.iter().copied().partition(\|x\| x % 2 != 0)` |
| C | a loop that writes to two output arrays and sets two counts |
| C++ | `std::ranges::partition_copy` with two `std::back_inserter` |
| Bash | a loop that adds each item to one of two arrays, then two lines of output |

- **The test.** Each solution uses `x % 2 != 0`, not `x % 2 == 1`. The remainder of a negative odd item is -1 in each of these languages, except Python. In Python, `-3 % 2` is 1, so both tests work there.
- **Rust.** `partition` "Consumes an iterator, creating two collections from it"[^1]. The first collection has the items for which the predicate returns true.
- **C.** The caller gives two output arrays. Each must have room for n items, because all the items can go into one of them.
- **Bash.** A function can only print text, so the solution prints the odd items on the first line and the even items on the second line. A line is empty when it has no items.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: an empty list gives two empty lists.
- `one-side`: all the items are odd, or all are even, so one of the two lists is empty.
- `negative`: -3 and -1 are odd, and -2 is even.
- `zero`: 0 is even.
- `order`: `[9, 8, 1, 2]` gives `[9, 1]` and `[8, 2]`, not sorted lists.
- `extremes`: 2147483647 is odd, and -2147483648 is even.
- `duplicates`: equal items stay in the result.

## Pitfalls

- **Python an iterator gives its items only once.** `filterfalse` returns "only those for which the predicate returns a false value"[^2]. In the example, `filter` uses all the items of the iterator, so `filterfalse` gets no items. Use a list, or one loop:

```python run id=python-iterator-once
from itertools import filterfalse


def is_odd(x):
    return x % 2 != 0


items = iter([1, 2, 3, 4])
odd = list(filter(is_odd, items))
even = list(filterfalse(is_odd, items))
print(odd, even)
items = [1, 2, 3, 4]
print(list(filter(is_odd, items)), list(filterfalse(is_odd, items)))
```

```output for=python-iterator-once
[1, 3] []
[1, 3] [2, 4]
```

- **JavaScript Object.groupBy has no property for an empty group.** The result is "A null-prototype object with properties for all groups, each assigned to an array containing the elements of the associated group"[^3]. In the example, no item is odd, so `groups.odd` is `undefined`. Use `?? []`:

```javascript run id=javascript-groupby-missing
const groups = Object.groupBy([2, 4], (x) => (x % 2 !== 0 ? "odd" : "even"));
console.log(groups.odd);
console.log(groups.odd ?? []);
```

```output for=javascript-groupby-missing
undefined
[]
```

- **Java groupingBy can give null.** For `partitioningBy`, "The returned Map always contains mappings for both false and true keys"[^4]. In the example, `groupingBy` with the same test gives `null` for the key `true`:

```java run id=java-partitioningby
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {
  public static void main(String[] args) {
    Map<Boolean, List<Integer>> parts =
        Stream.of(2, 4).collect(Collectors.partitioningBy(x -> x % 2 != 0));
    System.out.println(parts.get(true) + " " + parts.get(false));
    Map<Boolean, List<Integer>> groups =
        Stream.of(2, 4).collect(Collectors.groupingBy(x -> x % 2 != 0));
    System.out.println(groups.get(true) + " " + groups.get(false));
  }
}
```

```output for=java-partitioningby
[] [2, 4]
null [2, 4]
```

- **C# a dictionary of groups has no key for an empty group.** For a lookup, "If the key is not found in the collection, an empty sequence is returned"[^5]. In the example, the dictionary from `GroupBy` has no key `true`:

```csharp run id=csharp-lookup-empty
using System;
using System.Linq;

int[] items = [2, 4];
var lookup = items.ToLookup(x => x % 2 != 0);
Console.WriteLine(lookup[true].Count());
var groups = items.GroupBy(x => x % 2 != 0).ToDictionary(g => g.Key, g => g.ToArray());
Console.WriteLine(groups.ContainsKey(true));
```

```output for=csharp-lookup-empty
0
False
```

- **Go a filter into a[:0] changes the input.** "This trick uses the fact that a slice shares the same backing array and capacity as the original"[^6]. In the example, the filter writes into the array of `a`, so `a` changes. For a partition that keeps the input, append to new slices:

```go run id=go-filter-in-place
package main

import "fmt"

func main() {
	a := []int{1, 2, 3, 4}
	// b shares the array of a, so the appends change a.
	b := a[:0]
	for _, x := range a {
		if x%2 != 0 {
			b = append(b, x)
		}
	}
	fmt.Println(b, a)
}
```

```output for=go-filter-in-place
[1 3] [1 3 3 4]
```

- **Rust x % 2 == 1 misses negative odd items.** In the example, `-3 % 2` is -1, so the test `x % 2 == 1` puts no item in the first collection:

```rust run id=rust-negative-remainder
fn main() {
    let items = [-3, -2, -1, 0];
    let (wrong, _): (Vec<i32>, Vec<i32>) = items.iter().copied().partition(|x| x % 2 == 1);
    let (odd, even): (Vec<i32>, Vec<i32>) = items.iter().copied().partition(|x| x % 2 != 0);
    println!("{}", -3 % 2);
    println!("{wrong:?} {odd:?} {even:?}");
}
```

```output for=rust-negative-remainder
-1
[] [-3, -1] [-2, 0]
```

- **C the remainder has the sign of the dividend.** The quotient is "truncated towards zero"[^7], and "(a/b)*b + a%b == a"[^7]. Thus `-3 / 2` is -1 and `-3 % 2` is -1:

```c run id=c-negative-remainder
#include <stdio.h>

int main(void) {
    int x = -3;
    printf("%d %d\n", x / 2, x % 2);
    printf("%s\n", x % 2 == 1 ? "odd" : "not odd");
    printf("%s\n", x % 2 != 0 ? "odd" : "not odd");
    return 0;
}
```

```output for=c-negative-remainder
-1 -1
not odd
odd
```

- **C++ std::partition changes the range in place.** To keep the order of the items in each part, use `std::stable_partition`, which "divides elements into two groups while preserving their relative order within each group"[^8]. For two new containers and an unchanged input, use `std::partition_copy`. In the example, the result iterator is the start of the second part:

```cpp run id=cpp-stable-partition
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};
    auto mid = std::stable_partition(v.begin(), v.end(), [](int x) { return x % 2 != 0; });
    for (std::size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << (i + 1 < v.size() ? " " : "\n");
    }
    std::cout << (mid - v.begin()) << '\n';
}
```

```output for=cpp-stable-partition
1 3 5 2 4
3
```

- **Bash (( )) gives the status 1 for the value 0.** "If the value of the expression is non-zero, the return status is 0; otherwise the return status is 1."[^9] Thus `if ((x % 2 != 0))` works as a test, but `((count++))` has the status 1 when `count` was 0:

```bash run id=bash-arithmetic-status
count=0
((count++))
echo "status $?"
((count++))
echo "status $?"
```

```output for=bash-arithmetic-status
status 1
status 0
```

## Related tasks

- [[filter-a-list]]: keep only the items that pass a test.
- [[group-items-by-a-key]]: split a list into more than two groups.
- [[count-the-items-of-each-kind]]: count the items of each group.
- [[iterate-in-chunks]]: split a list into parts of a fixed size.

## References
