---
tag: 4EEPYD
kind: task
slug: filter-a-list
name: Filter a list
aliases: [keep the items that match, select items, remove items that do not match, where, retain, copy_if, grep a list]
area: data-and-values/collection-operations
level: intro
summary: Keep the items of a list that pass a test, in their input order, and do not remove items from a list while a loop goes over it.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/filter
  title: MDN Web Docs, Array.prototype.filter()
  locator: Description
- url: https://docs.python.org/3/tutorial/controlflow.html#for-statements
  title: The Python Tutorial, More Control Flow Tools
  locator: 4.2 for Statements
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Remainder
  title: MDN Web Docs, Remainder (%)
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/ArrayList.html
  title: Java SE 25 API, Class ArrayList
  locator: Class description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.where?view=net-10.0
  title: .NET API reference, Enumerable.Where method
  locator: Remarks
- url: https://pkg.go.dev/slices#DeleteFunc
  title: Go standard library, package slices
  locator: DeleteFunc
- url: https://doc.rust-lang.org/std/vec/struct.Vec.html#method.retain
  title: Rust standard library, struct Vec
  locator: retain, swap_remove
- url: https://en.cppreference.com/w/cpp/algorithm/remove
  title: cppreference.com, std::remove, std::remove_if
  locator: Notes
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Arrays
  title: Bash Reference Manual
  locator: 6.7 Arrays
---

## Task

Keep the items of a list that pass a test. The function `keep_odd(items)` returns a new list of the odd items of a list of integers, in their input order. For example, `keep_odd([1, 2, 3, 4, 5])` is `[1, 3, 5]`, and `keep_odd([2, 4, 6])` is `[]`.

An item is odd if the remainder of the item divided by 2 is not 0. So -3 and -1 are odd too. The input list does not change.

## Solutions

| Language | Keep the odd items |
|---|---|
| Python | `[x for x in items if x % 2 != 0]` |
| JavaScript, TypeScript | `items.filter((x) => x % 2 !== 0)` |
| Java | `Arrays.stream(items).filter(x -> x % 2 != 0).toArray()` |
| C# | `items.Where(x => x % 2 != 0).ToArray()` |
| Go | a loop and `append` to a new slice |
| Rust | `items.iter().copied().filter(...).collect()` |
| C | a loop that copies each odd item to `out`, and returns the count |
| C++ | `std::ranges::copy_if` with `std::back_inserter` |
| Bash | a loop that prints each odd argument |

- **The test.** Each solution uses `x % 2 != 0`, not `x % 2 == 1`, because the remainder of a negative item can be negative. See the JavaScript pitfall below.
- **JavaScript and TypeScript.** `filter` "creates a shallow copy of a portion of a given array, filtered down to just the elements from the given array that pass the test implemented by the provided function"[^1]. The input array does not change.
- **C#.** `ToArray` runs the query at once and keeps the result.
- **Go.** The loop appends to a new slice, so the input slice does not change.
- **C.** The caller gives an `out` array with room for `n` items. The function returns the number of odd items.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: the filter of an empty list is an empty list.
- `none-odd` and `all-odd`: the result can be empty, or the same as the input.
- `negative`: -3 and -1 are odd. A test with `x % 2 == 1` drops them in JavaScript, where `-3 % 2` is -1.
- `zero`: 0 is even, so it is not in the result.
- `order`: the odd items keep their input order, also if they are not sorted.
- `extremes`: -2147483648 is even, and 2147483647 is odd.
- `duplicates`: each copy of an odd item stays.

## Pitfalls

- **Python list.remove in a for loop skips items.** "Code that modifies a collection while iterating over that same collection can be tricky to get right"[^2]. In the example, the loop removes 2, then the next item is 1, so 4 stays. Make a new list:

```python run id=python-remove-in-loop
items = [2, 4, 1, 3]
for x in items:
    if x % 2 == 0:
        items.remove(x)
print(items)
items = [2, 4, 1, 3]
print([x for x in items if x % 2 != 0])
```

```output for=python-remove-in-loop
[4, 1, 3]
[1, 3]
```

- **JavaScript % keeps the sign.** The remainder "always takes the sign of the dividend"[^3]. So `x % 2 === 1` is false for -3:

```javascript run id=javascript-remainder-sign
const items = [-3, -2, -1, 0, 1];
console.log(-3 % 2);
console.log(items.filter((x) => x % 2 === 1));
console.log(items.filter((x) => x % 2 !== 0));
```

```output for=javascript-remainder-sign
-1
[ 1 ]
[ -3, -1, 1 ]
```

- **Java remove in a for-each loop throws.** The iterators of `ArrayList` are fail-fast: "if the list is structurally modified at any time after the iterator is created, in any way except through the iterator's own remove or add methods, the iterator will throw a ConcurrentModificationException"[^4]. Use `removeIf`:

```java run id=java-concurrent-modification
import java.util.ArrayList;
import java.util.ConcurrentModificationException;
import java.util.List;

public class Main {
  public static void main(String[] args) {
    List<Integer> items = new ArrayList<>(List.of(1, 2, 3, 4));
    try {
      for (Integer x : items) {
        if (x % 2 == 0) {
          items.remove(x);
        }
      }
    } catch (ConcurrentModificationException e) {
      System.out.println("ConcurrentModificationException");
    }
    items = new ArrayList<>(List.of(1, 2, 3, 4));
    items.removeIf(x -> x % 2 == 0);
    System.out.println(items);
  }
}
```

```output for=java-concurrent-modification
ConcurrentModificationException
[1, 3]
```

- **C# Where runs later.** "This method is implemented by using deferred execution"[^5]. In the example, the query sees the item that the code adds after the call to `Where`. `ToList` keeps the result of that moment:

```csharp run id=csharp-where-deferred
var items = new List<int> { 1, 2, 3 };
var odd = items.Where(x => x % 2 != 0);
var copy = items.Where(x => x % 2 != 0).ToList();
items.Add(5);
Console.WriteLine(string.Join(" ", odd));
Console.WriteLine(string.Join(" ", copy));
```

```output for=csharp-where-deferred
1 3 5
1 3
```

- **Go slices.DeleteFunc changes the input.** It works in place, and "DeleteFunc zeroes the elements between the new length and the original length"[^6]. Clone the slice first if the caller needs it:

```go run id=go-deletefunc-input
package main

import (
	"fmt"
	"slices"
)

func main() {
	items := []int{1, 2, 3, 4}
	odd := slices.DeleteFunc(items, func(x int) bool { return x%2 == 0 })
	fmt.Println(odd, items)
	items = []int{1, 2, 3, 4}
	odd = slices.DeleteFunc(slices.Clone(items), func(x int) bool { return x%2 == 0 })
	fmt.Println(odd, items)
}
```

```output for=go-deletefunc-input
[1 3] [1 3 0 0]
[1 3] [1 2 3 4]
```

- **Rust swap_remove changes the order.** "The removed element is replaced by the last element of the vector. This does not preserve ordering of the remaining elements"[^7]. `retain` "preserves the order of the retained elements"[^7]:

```rust run id=rust-swap-remove-order
fn main() {
    let mut v = vec![2, 1, 3, 5];
    v.swap_remove(0);
    println!("{v:?}");
    let mut w = vec![2, 1, 3, 5];
    w.retain(|x| x % 2 != 0);
    println!("{w:?}");
}
```

```output for=rust-swap-remove-order
[5, 1, 3]
[1, 3, 5]
```

- **C removal in a loop skips items.** After a removal, the next item moves to index `i`, and `i++` goes past it. In the example, 4 stays. Copy each item to keep to the next free index:

```c run id=c-remove-skip
#include <stdio.h>

int main(void) {
    int a[] = {2, 4, 1};
    size_t n = 3;
    // Wrong: after a removal, the next item moves to index i, and i++ skips it.
    for (size_t i = 0; i < n; i++) {
        if (a[i] % 2 == 0) {
            for (size_t j = i; j + 1 < n; j++) {
                a[j] = a[j + 1];
            }
            n--;
        }
    }
    for (size_t i = 0; i < n; i++) {
        printf(i ? " %d" : "%d", a[i]);
    }
    printf("\n");
    // Right: copy each item to keep to the next free index k.
    int b[] = {2, 4, 1};
    size_t k = 0;
    for (size_t i = 0; i < 3; i++) {
        if (b[i] % 2 != 0) {
            b[k++] = b[i];
        }
    }
    for (size_t i = 0; i < k; i++) {
        printf(i ? " %d" : "%d", b[i]);
    }
    printf("\n");
    return 0;
}
```

```output for=c-remove-skip
4 1
1
```

- **C++ remove_if does not change the size.** "A call to remove or remove_if is typically followed by a call to a container's erase member function to actually remove elements from the container"[^8]. `std::erase_if` does both:

```cpp run id=cpp-remove-if-size
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    auto even = [](int x) { return x % 2 == 0; };
    std::vector<int> v{1, 2, 3, 4};
    auto rest = std::ranges::remove_if(v, even);
    std::cout << v.size() << " " << rest.size() << "\n";
    v.erase(rest.begin(), rest.end());
    std::cout << v.size() << "\n";
    std::vector<int> w{1, 2, 3, 4};
    std::erase_if(w, even);
    std::cout << w.size() << ":";
    for (int x : w) {
        std::cout << " " << x;
    }
    std::cout << "\n";
}
```

```output for=cpp-remove-if-size
4 2
2
2: 1 3
```

- **Bash unset leaves a gap.** "unset name[subscript] unsets the array element at index subscript"[^9]. The other items keep their indices, so a loop from 0 to `${#a[@]}` reads the gap and misses the last item. Make a new array:

```bash run id=bash-unset-gap
a=(2 4 1 3)
unset 'a[0]'
echo "count ${#a[@]}, indices ${!a[@]}"
for ((i = 0; i < ${#a[@]}; i++)); do printf '[%s]' "${a[i]}"; done
echo
a=("${a[@]}")
echo "indices ${!a[@]}"
```

```output for=bash-unset-gap
count 3, indices 1 2 3
[][4][1]
indices 0 1 2
```

## Related tasks

- [[map-over-a-list]]: change each item of a list.
- [[partition-a-list]]: split a list into the items that pass a test and the items that fail it.
- [[remove-duplicates-from-a-list]]: another filter, which keeps the first copy of each item.
- [[find-the-largest-element]]: go over a list to find one item.

## References
