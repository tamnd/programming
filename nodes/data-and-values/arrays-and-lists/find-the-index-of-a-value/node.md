---
tag: TPB6E4
kind: task
slug: find-the-index-of-a-value
name: Find the index of a value
aliases: [index of, indexOf, position of a value, find the position of an item, first occurrence, last occurrence, findIndex, linear search for an index]
area: data-and-values/arrays-and-lists
level: intro
summary: Find the first position of a value in a list, or a not-found result, and know how each language reports a value that is not there, -1, an error, an end iterator or an empty option.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#common-sequence-operations
  title: The Python Standard Library, Built-in Types
  locator: sequence.index()
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/indexOf
  title: MDN Web Docs, Array.prototype.indexOf()
  locator: Return value
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/findIndex
  title: MDN Web Docs, Array.prototype.findIndex()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/List.html#indexOf(java.lang.Object)
  title: Java SE 25 API, Interface List
  locator: indexOf(Object o)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.array.indexof
  title: .NET API reference, Array.IndexOf Method
  locator: IndexOf(Array, Object), Returns
- url: https://pkg.go.dev/slices#Index
  title: Go standard library, package slices
  locator: func Index
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.position
  title: Rust standard library, Trait Iterator
  locator: position and rposition
- url: https://en.cppreference.com/w/cpp/algorithm/find
  title: cppreference.com, std::find
  locator: Description
- url: https://en.cppreference.com/w/cpp/iterator/distance
  title: cppreference.com, std::distance
  locator: Description
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Exit-Status
  title: Bash Reference Manual
  locator: 3.7.5 Exit Status
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/splice
  title: MDN Web Docs, Array.prototype.splice()
  locator: Parameters, start
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Arrays.html#binarySearch(int%5B%5D,int)
  title: Java SE 25 API, Class Arrays
  locator: binarySearch(int[] a, int key)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.double.equals
  title: .NET API reference, Double.Equals Method
  locator: Remarks, NaN
- url: https://go.dev/ref/spec#Index_expressions
  title: The Go Programming Language Specification
  locator: Index expressions
- url: https://en.cppreference.com/w/c/language/conversion
  title: cppreference.com, C implicit conversions
  locator: Integer conversions
---

## Task

Find the position of a value in a list. The function `index_of(items, value)` returns the smallest index `i` with `items[i]` equal to `value`, or -1 if no item is equal to `value`. The positions start at 0. For example, `index_of([4, 7, 1], 7)` is 1, and `index_of([4, 7, 1], 9)` is -1.

The list can be in any order, so the search looks at the items one by one. The function must not change `items`. Each item and the value are 32-bit signed integers, and the list has 0 to 1000 items.

## Solutions

| Language | First index | Not found |
|---|---|---|
| Python | `items.index(value)` | `ValueError` |
| JavaScript, TypeScript | `items.indexOf(value)` | -1 |
| Java | a loop, or `list.indexOf(value)` | -1 |
| C# | `Array.IndexOf(items, value)` | -1, the lower bound minus 1 |
| Go | `slices.Index(items, value)` | -1 |
| Rust | `items.iter().position(\|&x\| x == value)` | `None` |
| C | a loop | a value that the caller agrees on |
| C++ | `std::ranges::find(items, value)` | the end iterator |
| Bash | a loop that prints the index | status 1 |

- **Python.** `sequence.index(value)` will "Return the index of the first occurrence of value in sequence"[^1], and it "Raises ValueError if value is not found in sequence"[^1]. The solution catches the error and returns -1.
- **JavaScript.** `indexOf` returns "The first index of searchElement in the array; -1 if not found"[^2]. `findIndex` takes a test function: it "returns the index of the first element in an array that satisfies the provided testing function"[^3]. TypeScript uses the same method on a `readonly number[]`.
- **Java.** `List.indexOf` "Returns the index of the first occurrence of the specified element in this list, or -1 if this list does not contain the element"[^4]. An `int[]` has no such method, so the solution uses a loop.
- **C#.** `Array.IndexOf` returns "The index of the first occurrence of value in array, if found; otherwise, the lower bound of the array minus 1"[^5]. The lower bound of a C# array is 0, so the result is -1.
- **Go.** "Index returns the index of the first occurrence of v in s, or -1 if not present"[^6].
- **Rust.** `position` "Searches for an element in an iterator, returning its index"[^7], as an `Option<usize>`. The solution turns `None` into -1 to match the contract, but Rust code often keeps the `Option`.
- **C.** The solution returns a `ptrdiff_t`, a signed type, so it can return -1.
- **C++.** `std::find` returns "an iterator to the first element in the source range [first, last) that satisfies specific criteria (or last if there is no such iterator)"[^8]. `std::distance` "Returns the number of hops from first to last"[^9], which is the index. The solution returns -1 when the iterator is the end.
- **Bash.** "Exit statuses fall between 0 and 255"[^10], so a function cannot return an index as its status. The solution prints the index and returns the status 1 when the value is not there.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: the index of any value in an empty list is -1.
- `single` and `single-absent`: the value is at index 0, or it is not there.
- `first` and `last`: the value at index 0 and at the last index. The result 0 is a valid index, so a caller must not use it as a truth value.
- `duplicates` and `all-same`: the value is in the list more than once, and the result is the first index.
- `negative`: the value is negative, and the result is not -1. The result -1 means only "not found".
- `limit` and `limit-absent`: the smallest 32-bit integer is found, or is not found in a list that has only the largest.
- Each test checks that `items` does not change. The task [check whether a list contains a value](/check-whether-a-list-contains-a-value/) gives only a yes or no answer.

## Pitfalls

- **Python `index` raises an error, and the index 0 is falsy.** `index` "Raises ValueError if value is not found in sequence"[^1], so a search for a missing value stops the program unless the code catches the error. Its optional start argument finds a later occurrence. Also, `if a.index(x):` is false when `x` is the first item:

```python run id=python-index-error
a = [5, 7, 5]
print(a.index(5), a.index(5, 1))
try:
    a.index(9)
except ValueError as e:
    print(type(e).__name__)
i = a.index(5)
print("found" if i else "not found", i)
```

```output for=python-index-error
0 2
ValueError
not found 0
```

- **JavaScript `splice` with the -1 from `indexOf` removes the last item.** For the start of `splice`, "Negative index counts back from the end of the array"[^11]. So `a.splice(a.indexOf(x), 1)` removes the last item when `x` is not there. Test for -1 first:

```javascript run id=javascript-splice-minus-one
const a = [1, 2, 3];
a.splice(a.indexOf(9), 1);
console.log(a);
const b = [1, 2, 3];
const i = b.indexOf(9);
if (i !== -1) {
  b.splice(i, 1);
}
console.log(b);
```

```output for=javascript-splice-minus-one
[ 1, 2 ]
[ 1, 2, 3 ]
```

- **Java `binarySearch` returns a negative number, not -1.** It returns the "index of the search key, if it is contained in the array; otherwise, (-(insertion point) - 1)"[^12]. So a test with `== -1` misses most values that are not there. Test with `< 0`. The array must also be sorted:

```java run id=java-binary-search-negative
import java.util.Arrays;

public class Main {
  public static void main(String[] args) {
    int[] a = {1, 3, 5};
    System.out.println(Arrays.binarySearch(a, 3) + " " + Arrays.binarySearch(a, 4) + " " + Arrays.binarySearch(a, 9));
    int r = Arrays.binarySearch(a, 4);
    System.out.println((r == -1) + " " + (r < 0) + " " + (-r - 1));
  }
}
```

```output for=java-binary-search-negative
1 -3 -4
false true 2
```

- **C# `IndexOf` finds NaN, but `==` does not.** `Array.IndexOf` uses `Equals`, and "If two Double.NaN values are tested for equality by calling the Equals method, the method returns true. However, if two Double.NaN values are tested for equality by using the equality operator, the operator returns false"[^13]. So `IndexOf` and a test function with `==` give different answers:

```csharp run id=csharp-indexof-nan
double[] a = [1.0, double.NaN];
Console.WriteLine(Array.IndexOf(a, double.NaN));
Console.WriteLine(Array.FindIndex(a, x => x == double.NaN));
Console.WriteLine(Array.FindIndex(a, double.IsNaN));
```

```output for=csharp-indexof-nan
1
-1
1
```

- **Go the -1 from `slices.Index` is not a valid index.** For an index expression `a[x]`, "if x is out of range at run time, a run-time panic occurs"[^14]. So `a[slices.Index(a, x)]` panics when `x` is not there. Test for -1 first:

```go run id=go-index-minus-one
package main

import (
	"fmt"
	"slices"
)

func main() {
	a := []int{5, 7}
	i := slices.Index(a, 9)
	fmt.Println(i)
	defer func() {
		fmt.Println("panic:", recover())
	}()
	fmt.Println(a[i])
}
```

```output for=go-index-minus-one
-1
panic: runtime error: index out of range [-1]
```

- **Rust `rev().position()` counts from the end.** After `rev`, the first item is the last item of the slice, so `position` gives the distance from the end. `rposition` "Searches for an element in an iterator from the right, returning its index"[^7], and the index counts from the front:

```rust run id=rust-rposition
fn main() {
    let a = [5, 7, 5, 8];
    println!("{:?}", a.iter().position(|&x| x == 5));
    println!("{:?}", a.iter().rev().position(|&x| x == 5));
    println!("{:?}", a.iter().rposition(|&x| x == 5));
}
```

```output for=rust-rposition
Some(0)
Some(1)
Some(2)
```

- **C a `size_t` function that returns -1 returns `SIZE_MAX`.** When a value goes to an unsigned type, "the value 2b, where b is the number of value bits in the target type, is repeatedly subtracted or added to the source value until the result fits in the target type"[^15]. So `return -1;` in a function that returns `size_t` gives `SIZE_MAX`, and a test with `r < 0` is never true. Compare with `(size_t)-1`, or return a signed type. The output is from a machine with a 64-bit `size_t`:

```c run id=c-size-minus-one
#include <stddef.h>
#include <stdio.h>

static size_t find(const int *a, size_t n, int v) {
    for (size_t i = 0; i < n; i++) {
        if (a[i] == v) {
            return i;
        }
    }
    return -1;
}

int main(void) {
    const int a[] = {5, 7};
    size_t r = find(a, 2, 9);
    printf("%zu\n", r);
    printf("%d\n", r == (size_t)-1);
    return 0;
}
```

```output for=c-size-minus-one
18446744073709551615
1
```

- **C++ "not found" is the end, not -1.** `std::find` returns the end iterator when no item matches[^8], and its distance from the start is the size of the range, a valid number. A string search returns `std::string::npos`, the largest `size_t`. Compare with the end or with `npos`, not with -1:

```cpp run id=cpp-find-end
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main() {
    std::vector<int> v{5, 7};
    auto it = std::ranges::find(v, 9);
    std::cout << (it == v.end()) << " " << std::distance(v.begin(), it) << "\n";
    std::string s = "abc";
    std::cout << (s.find('z') == std::string::npos) << " " << s.find('z') << "\n";
}
```

```output for=cpp-find-end
1 2
1 18446744073709551615
```

- **Bash a return status wraps above 255.** "Exit statuses fall between 0 and 255"[^10], so `return 300` gives the status 44. A function that returns an index in its status gives a wrong index for a long list, and the index 0 looks like success. Print the index instead:

```bash run id=bash-return-wraps
index_bad() { return "$1"; }
index_bad 3
echo $?
index_bad 300
echo $?
```

```output for=bash-return-wraps
3
44
```

## Related tasks

- [[check-whether-a-list-contains-a-value]]: answer only yes or no.
- [[search-a-sorted-list]]: find the index in a sorted list with a binary search.
- [[find-the-largest-element]]: find the position of the largest item.
- [[filter-a-list]]: keep all the items that match, not only the first.
- [[slice-a-list]]: take the part of the list before or after the index.

## References
