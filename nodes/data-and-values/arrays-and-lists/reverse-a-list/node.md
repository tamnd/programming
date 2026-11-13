---
tag: PDKKSZ
kind: task
slug: reverse-a-list
name: Reverse a list
aliases: [reverse an array, reversed copy, reverse in place, invert the order of a list, flip a list, iterate backwards, reverse iterator, toReversed]
area: data-and-values/arrays-and-lists
level: intro
summary: Make a new list with the items of a list in the opposite order, and know which reverse functions change the list in place, which return a view, and which return a lazy iterator.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#mutable-sequence-types
  title: The Python Standard Library, Built-in Types
  locator: Mutable Sequence Types, sequence.reverse()
- url: https://docs.python.org/3/library/functions.html#reversed
  title: The Python Standard Library, Built-in Functions
  locator: reversed()
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/reverse
  title: MDN Web Docs, Array.prototype.reverse()
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/toReversed
  title: MDN Web Docs, Array.prototype.toReversed()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Collections.html#reverse(java.util.List)
  title: Java SE 25 API, Class Collections
  locator: reverse(List<?> list)
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/List.html#reversed()
  title: Java SE 25 API, Interface List
  locator: reversed()
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.reverse
  title: .NET API reference, List<T>.Reverse Method
  locator: Overloads
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.reverse
  title: .NET API reference, Enumerable.Reverse Method
  locator: Overloads and Remarks
- url: https://pkg.go.dev/slices#Reverse
  title: Go standard library, package slices
  locator: func Reverse
- url: https://doc.rust-lang.org/std/primitive.slice.html#method.reverse
  title: Rust standard library, Primitive Type slice
  locator: reverse
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.rev
  title: Rust standard library, Trait Iterator
  locator: rev
- url: https://en.cppreference.com/w/cpp/algorithm/reverse
  title: cppreference.com, std::reverse
  locator: Description
- url: https://en.cppreference.com/w/cpp/ranges/reverse_view
  title: cppreference.com, std::ranges::reverse_view
  locator: Description
- url: https://en.cppreference.com/w/c/language/operator_arithmetic
  title: cppreference.com, C arithmetic operators
  locator: Overflows
- url: https://en.cppreference.com/w/cpp/iterator/reverse_iterator/base
  title: cppreference.com, std::reverse_iterator::base
  locator: Notes
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Arrays
  title: Bash Reference Manual
  locator: 6.7 Arrays
---

## Task

Make a new list with the items of a list in the opposite order. The function `reverse(items)` returns a list whose first item is the last item of `items`. For example, `reverse([1, 2, 3])` is `[3, 2, 1]`.

The result is a new list, and `items` must not change. Each item is a 32-bit signed integer, and the list has 0 to 1000 items.

## Solutions

| Language | A reversed copy | Reverse in place | Walk backwards |
|---|---|---|---|
| Python | `items[::-1]` | `items.reverse()` | `reversed(items)` |
| JavaScript, TypeScript | `items.toReversed()` | `items.reverse()` | a loop from `length - 1` |
| Java | a copy loop, or `new ArrayList<>(list.reversed())` | `Collections.reverse(list)` | `list.reversed()` |
| C# | `Enumerable.Reverse(items).ToArray()` | `Array.Reverse(items)`, `list.Reverse()` | `Enumerable.Reverse(items)` |
| Go | `slices.Clone`, then `slices.Reverse` | `slices.Reverse(items)` | `slices.Backward(items)` |
| Rust | `items.iter().rev().copied().collect()` | `items.reverse()` | `items.iter().rev()` |
| C | `malloc` and a copy loop | a swap loop | a loop from `n` down |
| C++ | `std::vector<int>(items.rbegin(), items.rend())` | `std::reverse` | `std::views::reverse` |
| Bash | a loop from the last position | none | a loop from the last position |

- **Python.** `sequence.reverse()` will "Reverse the items of sequence in place"[^1], and "To remind users that it operates by side-effect, it returns None"[^1]. The solution uses the slice `items[::-1]`, which is a new list. `reversed(items)` will "Return a reverse iterator"[^2], so `list(reversed(items))` is also a new list.
- **JavaScript.** `reverse` "reverses an array in place and returns the reference to the same array"[^3]. `toReversed` "is the copying counterpart of the reverse() method. It returns a new array with the elements in reversed order"[^4]. TypeScript uses the same method on a `readonly number[]`.
- **Java.** `Collections.reverse` "Reverses the order of the elements in the specified list"[^5], in place. Since Java 21, `List.reversed()` "Returns a reverse-ordered view of this collection"[^6]. The solution fills a new `int[]` from the end of the input.
- **C#.** The method `List<T>.Reverse()` "Reverses the order of the elements in the entire List<T>"[^7], in place. `Enumerable.Reverse` "Inverts the order of the elements in a sequence"[^8], and it does not change the source. The solution copies the array and calls `Array.Reverse` on the copy.
- **Go.** "Reverse reverses the elements of the slice in place"[^9]. The solution reverses a clone.
- **Rust.** `reverse` "Reverses the order of elements in the slice, in place"[^10]. `rev` "Reverses an iterator’s direction"[^11], and the solution collects the reversed iterator into a new vector.
- **C.** The solution allocates `n` items and copies item `n - 1 - i` to position `i`. The caller must free the result.
- **C++.** `std::reverse` "Reverses the order of the elements in the target range [first, last)"[^12]. `std::views::reverse` is "a view of underlying view with reversed order"[^13], and it does not copy. The solution makes a vector from the reverse iterators.
- **Bash.** Bash has no reverse command. The solution walks the positional parameters from `$#` down to 1 and prints one item on each line.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: the reverse of an empty list is an empty list.
- `single`: the reverse of one item is the same list, but it must still be a new list.
- `two` and `four`: a list with an even count has no middle item, so each item moves. A swap loop must stop at `n / 2`, or it swaps each pair back.
- `three`, `duplicates` and `palindrome`: with an odd count, the middle item stays. The reverse of `[1, 2, 1]` is equal to the input, so a test that compares only the values cannot find an impl that returns the input. The tests also check that the result is not the input list.
- `negative` and `limit`: the items can be negative, and the largest and the smallest 32-bit integers.
- Each test checks that `items` does not change. The concept page [in-place and copying operations](/in-place-and-copying-operations/) compares the two forms.

## Pitfalls

- **Python `reverse()` returns `None`, and `reversed()` gives one pass.** `items.reverse()` changes the list and returns `None`[^1], so `b = a.reverse()` sets `b` to `None`. The iterator from `reversed`[^2] is empty after one pass. Use `a[::-1]` or `list(reversed(a))` for a copy:

```python run id=python-reverse-none
a = [1, 2, 3]
b = a.reverse()
print(a, b)
r = reversed(a)
print(list(r), list(r))
print(a[::-1], a)
```

```output for=python-reverse-none
[3, 2, 1] None
[1, 2, 3] []
[1, 2, 3] [3, 2, 1]
```

- **JavaScript `reverse()` changes the array and returns it.** It "returns the reference to the same array"[^3], so `const b = a.reverse()` gives two names for one changed array. Use `toReversed()`[^4] to keep the original:

```javascript run id=javascript-reverse-same
const a = [1, 2, 3];
const b = a.reverse();
console.log(a, b === a);
const c = a.toReversed();
console.log(a, c);
```

```output for=javascript-reverse-same
[ 3, 2, 1 ] true
[ 3, 2, 1 ] [ 1, 2, 3 ]
```

- **Java `reversed()` is a view, not a copy.** `reversed()` "Returns a reverse-ordered view of this collection"[^6], so a later change to the list shows in the view. `Collections.reverse` changes the list, and it throws `UnsupportedOperationException` for an unmodifiable list such as `List.of(...)`[^5]. Copy the view to keep it:

```java run id=java-reversed-view
import java.util.ArrayList;
import java.util.List;

public class Main {
  public static void main(String[] args) {
    List<Integer> a = new ArrayList<>(List.of(1, 2, 3));
    List<Integer> view = a.reversed();
    List<Integer> copy = new ArrayList<>(a.reversed());
    a.add(4);
    System.out.println(view + " " + copy);
    try {
      java.util.Collections.reverse(List.of(1, 2, 3));
    } catch (UnsupportedOperationException e) {
      System.out.println("UnsupportedOperationException");
    }
  }
}
```

```output for=java-reversed-view
[4, 3, 2, 1] [3, 2, 1]
UnsupportedOperationException
```

- **C# `Enumerable.Reverse` runs when the code reads it.** "This method is implemented by using deferred execution"[^8]. So the result of `Enumerable.Reverse(list)` shows the list as it is when a loop reads it, not when the call runs. Also, `list.Reverse()` on a `List<T>` is the method that changes the list in place[^7], not the LINQ method. Call `ToArray()` or `ToList()` to keep a copy:

```csharp run id=csharp-reverse-deferred
var list = new List<int> { 1, 2, 3 };
var later = Enumerable.Reverse(list);
var now = Enumerable.Reverse(list).ToArray();
list.Add(4);
Console.WriteLine(string.Join(",", later) + " " + string.Join(",", now));
list.Reverse();
Console.WriteLine(string.Join(",", list) + " " + string.Join(",", later));
```

```output for=csharp-reverse-deferred
4,3,2,1 3,2,1
4,3,2,1 1,2,3,4
```

- **Go `slices.Reverse` changes the array under the slice.** It "reverses the elements of the slice in place"[^9], and a slice shares the array of the slice that it came from. So the reverse of `a[:2]` changes `a`. Reverse a clone:

```go run id=go-reverse-shared
package main

import (
	"fmt"
	"slices"
)

func main() {
	a := []int{1, 2, 3, 4}
	slices.Reverse(a[:2])
	fmt.Println(a)
	c := slices.Clone(a)
	slices.Reverse(c)
	fmt.Println(a, c)
}
```

```output for=go-reverse-shared
[2 1 3 4]
[2 1 3 4] [4 3 1 2]
```

- **Rust the order of `rev` and `enumerate` changes the indexes.** "After using rev(), an iterator will instead iterate from right to left"[^11]. So `rev().enumerate()` counts from 0 at the last item, but `enumerate().rev()` keeps the position of each item in the slice:

```rust run id=rust-rev-enumerate
fn main() {
    let v = [10, 20, 30];
    let a: Vec<(usize, &i32)> = v.iter().rev().enumerate().collect();
    let b: Vec<(usize, &i32)> = v.iter().enumerate().rev().collect();
    println!("{a:?}");
    println!("{b:?}");
}
```

```output for=rust-rev-enumerate
[(0, 30), (1, 20), (2, 10)]
[(2, 30), (1, 20), (0, 10)]
```

- **C a `size_t` that counts down never goes below 0.** For an unsigned type, "subtracting one from 0 gives UINT_MAX"[^14], and for `size_t` it gives `SIZE_MAX`. So `for (size_t j = n - 1; j >= 0; j--)` never stops, and with `n` equal to 0 it starts at `SIZE_MAX`. The form `for (size_t j = n; j-- > 0;)` tests before it subtracts. The output is from a machine with a 64-bit `size_t`; the first loop stops only because of the count:

```c run id=c-size-countdown
#include <stddef.h>
#include <stdio.h>

int main(void) {
    const int a[] = {1, 2, 3};
    size_t n = 3;
    int steps = 0;
    for (size_t j = n - 1; j >= 0; j--) {
        printf("%zu ", j);
        if (++steps == 4) {
            break;
        }
    }
    printf("\n");
    for (size_t j = n; j-- > 0;) {
        printf("%d ", a[j]);
    }
    printf("\n");
    return 0;
}
```

```output for=c-size-countdown
2 1 0 18446744073709551615
3 2 1
```

- **C++ `base()` of a reverse iterator is one item off.** "The base iterator refers to the element that is next (from the iterator_type perspective) to the element the reverse_iterator is currently pointing to"[^15]. So `v.rbegin()` reads the last item, but `v.rbegin().base()` is `v.end()`. Subtract one from `base()` to reach the same item, for example before `erase`:

```cpp run id=cpp-reverse-base
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3};
    auto r = v.rbegin();
    std::cout << *r << " " << (r.base() == v.end()) << " " << *(r.base() - 1) << "\n";
    v.erase(r.base() - 1);
    std::cout << v.size() << " " << v.back() << "\n";
}
```

```output for=cpp-reverse-base
3 1 3
2 2
```

- **Bash `${#a[@]} - 1` is not always the last index.** "There is no maximum limit on the size of an array, nor any requirement that members be indexed or assigned contiguously"[^16]. After `unset`, an array has gaps, so a loop from `${#a[@]} - 1` down to 0 reads empty items and misses the last one. Walk the list of indexes from `${!a[@]}` instead:

```bash run id=bash-sparse-reverse
a=(1 2 3 4)
unset 'a[1]'
echo "${#a[@]} ${!a[@]}"
r=()
for ((i = ${#a[@]} - 1; i >= 0; i--)); do r+=("${a[i]}"); done
echo "${#r[@]} [${r[*]}]"
idx=("${!a[@]}")
r=()
for ((k = ${#idx[@]} - 1; k >= 0; k--)); do r+=("${a[${idx[k]}]}"); done
echo "${#r[@]} [${r[*]}]"
```

```output for=bash-sparse-reverse
3 0 2 3
3 [3  1]
3 [4 3 1]
```

## Related tasks

- [[slice-a-list]]: copy a part of a list; in Python, `items[::-1]` is a slice with a step of -1.
- [[rotate-a-list]]: three reverses rotate a list in place.
- [[sort-a-list]]: sort in the reverse order, which is not the same as a reverse.
- [[iterate-in-chunks]]: walk a list in parts.
- [[concatenate-two-lists]]: join two lists, then reverse to put the second first.

## References
