---
tag: TPWA33
kind: task
slug: slice-a-list
name: Slice a list
aliases: [sublist, subarray, slice an array, get a range of a list, copy part of a list, take a sublist, extract a range, list slicing]
area: data-and-values/arrays-and-lists
level: intro
summary: Copy the items from a start position up to, but not including, a stop position into a new list, and know which languages copy, which share storage, and which clamp a bad range.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#common-sequence-operations
  title: The Python Standard Library, Built-in Types
  locator: Common Sequence Operations, notes 3, 4 and 5
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/slice
  title: MDN Web Docs, Array.prototype.slice()
  locator: Description and Parameters
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Arrays.html#copyOfRange(int%5B%5D,int,int)
  title: Java SE 25 API, Class Arrays
  locator: copyOfRange(int[] original, int from, int to)
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/member-access-operators#range-operator-
  title: C# language reference, Member access operators and expressions
  locator: Range operator ..
- url: https://go.dev/ref/spec#Slice_expressions
  title: The Go Programming Language Specification
  locator: Slice expressions
- url: https://doc.rust-lang.org/std/primitive.slice.html#method.to_vec
  title: Rust standard library, Primitive Type slice
  locator: to_vec and get
- url: https://en.cppreference.com/w/cpp/container/vector/vector
  title: cppreference.com, std::vector constructor
  locator: Description, overload 5
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Shell-Parameter-Expansion
  title: Bash Reference Manual
  locator: 3.5.3 Shell Parameter Expansion
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/splice
  title: MDN Web Docs, Array.prototype.splice()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/List.html#subList(int,int)
  title: Java SE 25 API, Interface List
  locator: subList(int fromIndex, int toIndex)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.getrange
  title: .NET API reference, List<T>.GetRange(Int32, Int32)
  locator: Definition and Parameters
- url: https://doc.rust-lang.org/std/ops/struct.Range.html
  title: Rust standard library, Struct Range
  locator: Description
- url: https://en.cppreference.com/w/c/language/operator_arithmetic
  title: cppreference.com, C arithmetic operators
  locator: Overflows
- url: https://en.cppreference.com/w/cpp/container/span/subspan
  title: cppreference.com, std::span::subspan
  locator: Description
---

## Task

Copy a part of a list into a new list. The function `slice(items, start, stop)` returns the items from position `start` up to, but not including, position `stop`. For example, `slice([1, 2, 3, 4, 5], 1, 3)` is `[2, 3]`. The result has `stop - start` items.

The positions start at 0, and `0 <= start <= stop <= len(items)`. When `start` equals `stop`, the result is empty. A range out of these bounds is an error, and the function must not cut it to fit. The result is a new list: a change to it must not change `items`, and `items` must not change. Each item is a 32-bit signed integer, and the list has 0 to 1000 items.

## Solutions

| Language | A copy of `items[start:stop]` | Bad range |
|---|---|---|
| Python | `items[start:stop]` | the slice clamps it, so check first |
| JavaScript, TypeScript | `items.slice(start, stop)` | the slice clamps it, so check first |
| Java | `Arrays.copyOfRange(items, start, stop)` | `Objects.checkFromToIndex` throws |
| C# | `items[start..stop]` | `ArgumentOutOfRangeException` |
| Go | `slices.Clone(items[start:stop])` | a run-time panic |
| Rust | `items[start..stop].to_vec()` | a panic, or `None` from `get` |
| C | `malloc` and `memcpy` of `stop - start` items | check first |
| C++ | `std::vector<int>(first + start, first + stop)` | check first |
| Bash | `("${items[@]:start:stop-start}")` | check first |

- **Python.** "The slice of s from i to j is defined as the sequence of items with index k such that i <= k < j"[^1]. A slice of a list is a new list. The solution checks the bounds first, because a slice does not raise an error for a bad range.
- **JavaScript.** `slice` "returns a shallow copy of a portion of an array into a new array object selected from start to end (end not included)"[^2]. "The original array will not be modified"[^2]. TypeScript uses the same method on a `readonly number[]`.
- **Java.** `copyOfRange` copies a range of an array, and its `to` argument is "the final index of the range to be copied, exclusive. (This index may lie outside the array.)"[^3]. Past the end, the copy gets zeros, so the solution checks `stop` with `Objects.checkFromToIndex` first.
- **C#.** In `start..stop`, "The left-hand operand is an inclusive start of a range. The right-hand operand is an exclusive end of a range"[^4]. A range of an array is a new array, and a bad range throws `ArgumentOutOfRangeException`.
- **Go.** "For arrays or strings, the indices are in range if 0 <= low <= high <= len(a), otherwise they are out of range"[^5]. For a slice, the upper bound is the capacity. The result of `items[start:stop]` "shares its underlying array with the operand"[^5], so the solution copies it with `slices.Clone`. The solution uses `items[start:stop:stop]`, so that a `stop` past the length panics, even when the capacity is larger.
- **Rust.** `&items[start..stop]` borrows a part of the slice, and `to_vec` "Copies self into a new Vec"[^6]. The method `get` returns "the subslice corresponding to that range, or None if out of bounds"[^6].
- **C.** C has no slice. The solution checks the range, allocates `stop - start` items, and copies them with `memcpy`. The caller must free the result.
- **C++.** The constructor with two iterators "Constructs a vector with the contents of the range [first, last)"[^7]. The solution checks the range and makes the vector from a `std::span`.
- **Bash.** For `${items[@]:offset:length}`, "the result is the length members of the array beginning with ${parameter[offset]}"[^8]. The second number is a length, so the solution passes `stop-start`. The solution prints one item on each line.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty-list`: the only range of an empty list is `0` to `0`, and the result is empty.
- `empty-slice`: when `start` equals `stop`, the result is empty, and the list does not need to be empty.
- `whole`: `slice(items, 0, len(items))` is a copy of the whole list.
- `single` and `last-one`: a range of one item gives a list of one item, not the item.
- `at-end`: `start` and `stop` can both be `len(items)`. The result is empty, and this is not an error.
- `limit`: the items can be the largest and the smallest 32-bit integers.
- Each test also changes the first item of the result and checks that `items` does not change. This test finds a result that shares storage with the input. The tests also check that `stop` past the end and `start` after `stop` are errors. The concept page [slice](/slice-concept/) tells which languages share storage.

## Pitfalls

- **Python slices clamp a bad range and count negative positions from the end.** "If i or j is greater than len(s), use len(s)"[^1], and "If i is greater than or equal to j, the slice is empty"[^1]. So a range past the end gives a short list and no error. "If i or j is negative, the index is relative to the end of sequence s"[^1], so `-1` is the last item. Check the bounds when a bad range is a bug:

```python run id=python-slice-clamps
a = [1, 2, 3]
print(a[1:10], a[5:9], a[2:1])
print(a[-1:], a[:-1])
```

```output for=python-slice-clamps
[2, 3] [] []
[3] [1, 2]
```

- **JavaScript `splice` removes the items.** `splice` "changes the contents of an array by removing or replacing existing elements and/or adding new elements in place"[^9]. Its second argument is a count, not a stop position. `slice` does not change the array:

```javascript run id=javascript-splice-changes
const a = [1, 2, 3, 4];
console.log(a.slice(1, 3), a);
console.log(a.splice(1, 3), a);
```

```output for=javascript-splice-changes
[ 2, 3 ] [ 1, 2, 3, 4 ]
[ 2, 3, 4 ] [ 1 ]
```

- **Java `subList` is a view, not a copy.** "The returned list is backed by this list, so non-structural changes in the returned list are reflected in this list, and vice-versa"[^10]. Also, "The semantics of the list returned by this method become undefined if the backing list (i.e., this list) is structurally modified in any way other than via the returned list"[^10]. For an `ArrayList`, the view then throws `ConcurrentModificationException`. Copy the view to keep it:

```java run id=java-sublist-view
import java.util.ArrayList;
import java.util.ConcurrentModificationException;
import java.util.List;

public class Main {
  public static void main(String[] args) {
    List<Integer> a = new ArrayList<>(List.of(1, 2, 3, 4));
    List<Integer> view = a.subList(1, 3);
    view.set(0, 9);
    System.out.println(a);
    List<Integer> copy = new ArrayList<>(view);
    a.add(5);
    try {
      System.out.println(view.size());
    } catch (ConcurrentModificationException e) {
      System.out.println("ConcurrentModificationException");
    }
    System.out.println(copy);
  }
}
```

```output for=java-sublist-view
[1, 9, 3, 4]
ConcurrentModificationException
[9, 3]
```

- **C# `GetRange` takes a count, but a range takes an end.** `GetRange(index, count)` "Creates a shallow copy of a range of elements"[^11], and its second argument is the number of items. The range `1..3` stops before position 3[^4]. So `GetRange(1, 3)` has three items and `[1..3]` has two:

```csharp run id=csharp-getrange-count
var list = new List<int> { 1, 2, 3, 4, 5 };
int[] array = [1, 2, 3, 4, 5];
Console.WriteLine(string.Join(",", list.GetRange(1, 3)));
Console.WriteLine(string.Join(",", array[1..3]));
Console.WriteLine(string.Join(",", array[^2..]));
```

```output for=csharp-getrange-count
2,3,4
2,3
4,5
```

- **Go a slice expression does not copy.** The result "shares its underlying array with the operand"[^5]. So a change to the slice changes the original, and `append` to the slice can write over the next item of the original. Copy the slice with `slices.Clone` when the parts must be separate:

```go run id=go-slice-shares
package main

import (
	"fmt"
	"slices"
)

func main() {
	a := []int{1, 2, 3, 4}
	s := a[1:3]
	s[0] = 9
	fmt.Println(a)
	s = append(s, 7)
	fmt.Println(a)
	c := slices.Clone(a[1:3])
	c[0] = 0
	fmt.Println(a, c)
}
```

```output for=go-slice-shares
[1 9 3 4]
[1 9 3 7]
[1 9 3 7] [0 3]
```

- **Rust a backward range is empty, but a slice with it panics.** "The range start..end contains all values with start <= x < end. It is empty if start >= end"[^12]. But `items[2..1]` panics, and so does a range past the end. Use `get` to receive `None` for a bad range[^6]:

```rust run id=rust-get-range
fn main() {
    let a = [1, 2, 3];
    println!("{} {}", (2..1).is_empty(), (2..1).count());
    println!("{:?} {:?} {:?}", a.get(1..3), a.get(2..5), a.get(2..1));
}
```

```output for=rust-get-range
true 0
Some([2, 3]) None None
```

- **C `stop - start` wraps when `start` is larger.** For an unsigned type such as `size_t`, the arithmetic is modulo 2 to the power of the bit count, and "subtracting one from 0 gives UINT_MAX"[^13]. So `stop - start` with `start > stop` is a very large count, not a negative one. Check `start <= stop` before the subtraction. The output is from a machine with a 64-bit `size_t`:

```c run id=c-size-wraps
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    size_t start = 3;
    size_t stop = 1;
    size_t k = stop - start;
    printf("%zu\n", k);
    printf("%d\n", k == SIZE_MAX - 1);
    return 0;
}
```

```output for=c-size-wraps
18446744073709551614
1
```

- **C++ `subspan` takes a count and does not copy.** With two arguments, "the subview contains Count elements starting from the Offsetth"[^14]. So `subspan(1, 3)` has three items, not the items from 1 to 3. A span is a view: a change through it changes the vector. Make a vector from the span to copy the items:

```cpp run id=cpp-subspan-count
#include <iostream>
#include <span>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};
    std::span<int> part = std::span<int>(v).subspan(1, 3);
    std::cout << part.size() << " " << part[0] << " " << part[2] << "\n";
    part[0] = 9;
    std::cout << v[1] << "\n";
    std::vector<int> copy(part.begin(), part.end());
    copy[0] = 0;
    std::cout << v[1] << " " << copy[0] << "\n";
}
```

```output for=cpp-subspan-count
3 2 4
9
9 0
```

- **Bash `${a[@]:-1}` is not the last item.** The `:-` form gives a default value when the variable is unset or null, so it gives the whole array here. "Note that a negative offset must be separated from the colon by at least one space to avoid being confused with the ‘:-’ expansion"[^8]. Put a space before the minus sign:

```bash run id=bash-negative-offset
a=(1 2 3)
echo "${a[@]:-1}"
echo "${a[@]: -1}"
echo "${a[@]:1:2}"
```

```output for=bash-negative-offset
1 2 3
3
2 3
```

## Related tasks

- [[concatenate-two-lists]]: join two lists, which is the reverse of a cut into two slices.
- [[iterate-in-chunks]]: cut a list into slices of a fixed size.
- [[rotate-a-list]]: join the slice after a position and the slice before it.
- [[get-the-length-of-an-array]]: get the length that bounds `stop`.
- [[append-to-a-list]]: add an item, which can write into a shared array in Go.

## References
