---
tag: MK0C11
kind: task
slug: zip-two-lists
name: Zip two lists
aliases: [zip, pair the items of two lists, loop over two lists at the same time, iterate in parallel, combine two lists into pairs]
area: data-and-values/collection-operations
level: intro
summary: Pair the items of two lists of integers at the same index, and stop at the end of the shorter list.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/functions.html#zip
  title: The Python Standard Library, Built-in Functions
  locator: zip()
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array
  title: MDN Web Docs, Array
  locator: Examples, Access an array item by its index
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Object.html
  title: Java SE 25 API, Class Object
  locator: equals(Object)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.zip?view=net-10.0
  title: .NET API reference, Enumerable.Zip method
  locator: Remarks
- url: https://go.dev/ref/spec#Index_expressions
  title: The Go Programming Language Specification
  locator: Index expressions
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.zip
  title: Rust standard library, trait Iterator
  locator: zip
- url: https://en.cppreference.com/w/c/language/operator_arithmetic
  title: cppreference.com, Arithmetic operators (C)
  locator: Overflows
- url: https://en.cppreference.com/w/cpp/ranges/zip_view
  title: cppreference.com, std::ranges::zip_view, std::views::zip
  locator: Description
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Arrays
  title: Bash Reference Manual
  locator: 6.7 Arrays
---

## Task

Pair the items of two lists of integers at the same index. The function `zip_pairs(a, b)` returns the pair `(a[i], b[i])` for each index i that is in both lists, in index order. For example, `zip_pairs([1, 2, 3], [4, 5, 6])` is `[(1, 4), (2, 5), (3, 6)]`.

The two lists can have different lengths. Then the result stops at the end of the shorter list, so `zip_pairs([1, 2, 3], [9])` is `[(1, 9)]`. The items are 32-bit signed integers, and each list can be empty.

## Solutions

| Language | Pair the items up to the end of the shorter list |
|---|---|
| Python | `list(zip(a, b))` |
| JavaScript, TypeScript | `Array.from` with the length `Math.min(a.length, b.length)` |
| Java | `IntStream.range(0, Math.min(a.length, b.length))` and `mapToObj` |
| C# | `a.Zip(b).ToArray()` |
| Go | a loop to `min(len(a), len(b))` |
| Rust | `a.iter().copied().zip(b.iter().copied()).collect()` |
| C | a loop to the smaller length, which writes to an output array |
| C++ | a loop over `std::views::zip(a, b)` |
| Bash | two arrays from `read -ra`, then a loop to the smaller length |

- **The length.** Python `zip`, C# `Zip`, Rust `zip` and C++ `views::zip` stop at the end of the shorter list. The other solutions calculate the smaller length first, and then use an index that is in both lists.
- **C.** The caller gives an output array with room for the smaller length. The function returns the number of pairs.
- **Bash.** A function can only get strings, so each list is one argument with the items separated by spaces. The function prints one line `A_ITEM B_ITEM` for each pair.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: two empty lists give an empty list.
- `one-empty`: when one of the two lists is empty, the result is empty.
- `shorter`: `[1]` and `[7, 8]` give `[(1, 7)]`, and `[1, 2, 3]` and `[9]` give `[(1, 9)]`.
- `negative`: negative items stay in their pairs.
- `extremes`: -2147483648 and 2147483647 are items.
- `duplicates`: equal items stay in the result.

## Pitfalls

- **Python zip stops at the shortest list without an error.** "By default, zip() stops when the shortest iterable is exhausted."[^1] If the lists must have the same length, use `strict=True`: "If strict is True and one of the iterables is exhausted before the others, a ValueError is raised."[^1]

```python run id=python-zip-strict
a = [1, 2, 3]
b = [9]
print(list(zip(a, b)))
try:
    list(zip(a, b, strict=True))
except ValueError as e:
    print("ValueError:", e)
```

```output for=python-zip-strict
[(1, 9)]
ValueError: zip() argument 2 is shorter than argument 1
```

- **JavaScript an index past the end gives undefined.** MDN shows that "Using an index number larger than the array's length" gives `undefined`[^2]. Thus `a.map((x, i) => [x, b[i]])` makes pairs with `undefined` when `b` is shorter. Use the smaller length:

```javascript run id=javascript-index-past-end
const a = [1, 2, 3];
const b = [9];
console.log(a.map((x, i) => [x, b[i]]));

const n = Math.min(a.length, b.length);
console.log(Array.from({ length: n }, (_, i) => [a[i], b[i]]));
```

```output for=javascript-index-past-end
[ [ 1, 9 ], [ 2, undefined ], [ 3, undefined ] ]
[ [ 1, 9 ] ]
```

- **Java a list of arrays compares the arrays by reference.** "All objects, including arrays, implement the methods of this class."[^3] The `equals` method of `Object` "returns true if and only if x and y refer to the same object"[^3]. In the example, two lists of equal pairs are not equal. `Arrays.deepEquals` compares the items:

```java run id=java-array-equals
import java.util.Arrays;
import java.util.List;

public class Main {
  public static void main(String[] args) {
    List<int[]> x = List.of(new int[] {1, 9});
    List<int[]> y = List.of(new int[] {1, 9});
    System.out.println(x.equals(y));
    System.out.println(Arrays.deepEquals(x.toArray(), y.toArray()));
  }
}
```

```output for=java-array-equals
false
true
```

- **C# Zip runs when the result is enumerated.** "This method is implemented by using deferred execution."[^4] In the example, `a[0] = 100` changes the first pair of `lazy`, but not of `copy`, because `ToArray` made the pairs before the change. Also, "If the sequences do not have the same number of elements, the method merges sequences until it reaches the end of one of them."[^4]

```csharp run id=csharp-zip-deferred
using System;
using System.Linq;

int[] a = { 1, 2, 3 };
int[] b = { 9 };
var lazy = a.Zip(b);
var copy = a.Zip(b).ToArray();
a[0] = 100;
Console.WriteLine(lazy.First());
Console.WriteLine(copy[0]);
```

```output for=csharp-zip-deferred
(100, 9)
(1, 9)
```

- **Go an index past the end panics.** For a slice, "if x is out of range at run time, a run-time panic occurs"[^5]. In the example, the loop over `a` uses `b[1]`, but `b` has 1 item. Loop to `min(len(a), len(b))`:

```go run id=go-index-out-of-range
package main

import "fmt"

func main() {
	a := []int{1, 2, 3}
	b := []int{9}
	defer func() {
		fmt.Println("panic:", recover())
	}()
	for i, x := range a {
		fmt.Println(x, b[i])
	}
}
```

```output for=go-index-out-of-range
1 9
panic: runtime error: index out of range [1] with length 1
```

- **Rust zip stops at the shorter iterator without an error.** "If either iterator returns None, next from the zipped iterator will return None."[^6] If the lists must have the same length, compare the lengths:

```rust run id=rust-zip-shorter
fn main() {
    let a = [1, 2, 3];
    let b = [9];
    let pairs: Vec<(i32, i32)> = a.iter().copied().zip(b.iter().copied()).collect();
    println!("{:?}", pairs);
    if a.len() != b.len() {
        println!("{} items of a have no pair", a.len() - pairs.len());
    }
}
```

```output for=rust-zip-shorter
[(1, 9)]
2 items of a have no pair
```

- **C a subtraction of two size_t values does not go below 0.** "Unsigned integer arithmetic is always performed modulo"[^7] a power of 2. In the example, `na - nb` is a large number, not -2, so `na - nb > 0` is true and `wrong` is 3. Compare the two lengths with `<`:

```c run id=c-unsigned-subtraction
#include <stddef.h>
#include <stdio.h>

int main(void) {
    size_t na = 1;
    size_t nb = 3;
    size_t wrong = na - nb > 0 ? nb : na;
    size_t right = na < nb ? na : nb;
    printf("%zu %zu\n", wrong, right);
    printf("%d\n", na - nb > na);
    return 0;
}
```

```output for=c-unsigned-subtraction
3 1
1
```

- **C++ views::zip gives references to the items.** "The size of produced view is the minimum of sizes of all adapted views."[^8] In the example, each element of the view refers to the items of `a` and `b`, so `x = y` changes `a[0]` to 9:

```cpp run id=cpp-zip-references
#include <iostream>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{9};
    auto z = std::views::zip(a, b);
    std::cout << z.size() << '\n';
    for (auto [x, y] : z) {
        x = y;
    }
    for (int x : a) {
        std::cout << x << (x == a.back() ? '\n' : ' ');
    }
}
```

```output for=cpp-zip-references
1
9 2 3
```

- **Bash ${#b} is not the number of items.** "${#name[subscript]} expands to the length of ${name[subscript]}. If subscript is ‘@’ or ‘*’, the expansion is the number of elements in the array."[^9] Also, "Referencing an array variable without a subscript is equivalent to referencing with a subscript of 0."[^9] Thus `${#b}` is the length of the text `10`:

```bash run id=bash-array-length
#!/usr/bin/env bash
b=(10 20 30)
echo "${#b} ${#b[@]}"
```

```output for=bash-array-length
2 3
```

## Related tasks

- [[map-over-a-list]]: change each item of one list.
- [[iterate-in-chunks]]: take the items of one list in groups.
- [[compute-the-cartesian-product]]: pair each item of one list with each item of the other list.
- [[partition-a-list]]: split one list into two lists.

## References
