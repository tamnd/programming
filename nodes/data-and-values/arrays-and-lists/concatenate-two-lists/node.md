---
tag: 726HFA
kind: task
slug: concatenate-two-lists
name: Concatenate two lists
aliases: [join two lists, join two arrays, array concatenation, merge two lists, extend a list, combine two lists, append a list to a list]
area: data-and-values/arrays-and-lists
level: intro
summary: Make a new list with the items of one list followed by the items of another, or add the second list to the end of the first in place.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#mutable-sequence-types
  title: The Python Standard Library, Built-in Types
  locator: Mutable Sequence Types
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/concat
  title: MDN Web Docs, Array.prototype.concat()
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.concat?view=net-10.0
  title: .NET API reference, Enumerable.Concat method
  locator: Remarks
- url: https://pkg.go.dev/slices#Concat
  title: Go standard library, package slices
  locator: func Concat
- url: https://doc.rust-lang.org/std/vec/struct.Vec.html#method.append
  title: Rust standard library, struct Vec
  locator: append
- url: https://en.cppreference.com/w/c/string/byte/memcpy
  title: cppreference.com, memcpy
  locator: Description
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Arrays
  title: Bash Reference Manual
  locator: 6.7 Arrays
- url: https://docs.python.org/3/library/stdtypes.html#common-sequence-operations
  title: The Python Standard Library, Built-in Types
  locator: Common Sequence Operations, note 6
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Addition
  title: MDN Web Docs, Addition (+)
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/List.html#unmodifiable
  title: Java SE 25 API, Interface List
  locator: Unmodifiable Lists
- url: https://go.dev/ref/spec#Appending_and_copying_slices
  title: The Go Programming Language Specification
  locator: Appending to and copying slices
- url: https://en.cppreference.com/w/cpp/container/vector/insert
  title: cppreference.com, std::vector::insert
  locator: Description
---

## Task

Make a new list with the items of `a` followed by the items of `b`. The function `concat(a, b)` returns the new list, and `a` and `b` do not change. For example, `concat([1, 2], [3, 4])` is `[1, 2, 3, 4]`, and `concat([], [5])` is `[5]`.

Each item is a 32-bit signed integer. The task also shows the in-place form, which adds the items of `b` to the end of `a`.

## Solutions

| Language | A new list | In place, at the end of `a` |
|---|---|---|
| Python | `a + b` | `a.extend(b)` or `a += b` |
| JavaScript, TypeScript | `[...a, ...b]` or `a.concat(b)` | `a.push(...b)` |
| Java | `new ArrayList<>(a)`, then `addAll(b)` | `a.addAll(b)` |
| C# | `[.. a, .. b]` or `a.Concat(b).ToList()` | `a.AddRange(b)` |
| Go | `slices.Concat(a, b)` | `a = append(a, b...)` |
| Rust | `[a, b].concat()` | `a.extend_from_slice(&b)` |
| C | `malloc` and two `memcpy` calls | `realloc` and one `memcpy` |
| C++ | copy `a`, then `insert(c.end(), b.begin(), b.end())` | `a.insert(a.end(), b.begin(), b.end())` |
| Bash | `c=("${a[@]}" "${b[@]}")` | `a+=("${b[@]}")` |

- **Python.** `a + b` makes a new list. `s += t` "extends s with the contents of t"[^1], so it changes the list in place.
- **JavaScript.** `concat` "does not change the existing arrays, but instead returns a new array"[^2]. Spread syntax in an array literal does the same.
- **Java.** `addAll` changes the list it is called on, so the solution copies `a` into a new `ArrayList` first.
- **C#.** `Concat` "is implemented by using deferred execution"[^3]. `ToList` or `ToArray` makes the new list at once.
- **Go.** `slices.Concat` "returns a new slice concatenating the passed in slices"[^4]. `append` can write into the array of `a` (see the pitfalls).
- **Rust.** `extend_from_slice` copies the items of `b`. `append` moves them and leaves `b` empty[^5].
- **C.** `memcpy` "Copies count characters"[^6], so the count is in bytes: `n * sizeof *a`.
- **C++.** `insert` with a range copies the items of `b` before the position `a.end()`.
- **Bash.** In double quotes, "${name[@]} expands each element of name to a separate word"[^7], so each item stays one item.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `both-empty`, `left-empty` and `right-empty`: an empty list adds nothing. `slices.Concat` in Go gives `nil` when "the concatenation is empty"[^4], and `nil` has length 0.
- `duplicates`: the result keeps equal items from both lists. A concatenation is not a set union.
- `order`: the result keeps the order of `a` and then the order of `b`. It does not sort.
- `limit`: the items can be the largest and the smallest 32-bit signed integers.

## Pitfalls

- **Python += changes the list for every name.** `a += b` extends the list in place, so another name for the same list sees the change. `a = a + b` makes a new list and binds `a` to it. "Concatenating immutable sequences always results in a new object"[^8], so a loop that builds a tuple or a string with `+` copies all of it each time:

```python run id=python-plus-equals-alias
a = [1, 2]
alias = a
a += [3]
print(alias)
b = [1, 2]
alias = b
b = b + [3]
print(alias, b)
```

```output for=python-plus-equals-alias
[1, 2, 3]
[1, 2] [1, 2, 3]
```

- **JavaScript a + b makes a string.** The `+` operator "first coerces both operands to primitives"[^9], and "If one side is a string, the other operand is also converted to a string and they are concatenated"[^9]. An array becomes the text of its items with commas:

```javascript run id=javascript-plus-arrays
const a = [1, 2];
const b = [3, 4];
console.log(a + b);
console.log(typeof (a + b));
console.log(a.concat(b), [...a, ...b]);
```

```output for=javascript-plus-arrays
1,23,4
string
[ 1, 2, 3, 4 ] [ 1, 2, 3, 4 ]
```

- **Java List.of cannot grow.** "Calling any mutator method on the List will always cause UnsupportedOperationException to be thrown"[^10]. So `addAll` on a list from `List.of` fails. Copy it into an `ArrayList` first:

```java run id=java-list-of-addall
import java.util.ArrayList;
import java.util.List;

public class Main {
  public static void main(String[] args) {
    List<Integer> a = List.of(1, 2);
    List<Integer> b = List.of(3, 4);
    try {
      a.addAll(b);
    } catch (UnsupportedOperationException e) {
      System.out.println("UnsupportedOperationException");
    }
    List<Integer> c = new ArrayList<>(a);
    c.addAll(b);
    System.out.println(a + " " + c);
  }
}
```

```output for=java-list-of-addall
UnsupportedOperationException
[1, 2] [1, 2, 3, 4]
```

- **C# Concat reads the lists later.** "The query represented by this method is not executed until the object is enumerated"[^3]. In the example, the item 9 is added to `a` after the call, and it shows in the lazy result before the items of `b`. `ToList` keeps the items of the moment:

```csharp run id=csharp-concat-deferred
var a = new List<int> { 1, 2 };
var b = new List<int> { 3 };
var lazy = a.Concat(b);
var now = a.Concat(b).ToList();
a.Add(9);
Console.WriteLine(string.Join(",", lazy));
Console.WriteLine(string.Join(",", now));
```

```output for=csharp-concat-deferred
1,2,9,3
1,2,3
```

- **Go append can write into a shared array.** "If the capacity of s is not large enough to fit the additional values, append allocates a new, sufficiently large underlying array"[^11]. "Otherwise, append re-uses the underlying array"[^11]. In the example, `a` has room for 4 items, so both `append` calls write to the same array, and the second one changes `x`. `slices.Concat` always makes a new slice:

```go run id=go-append-shared
package main

import (
	"fmt"
	"slices"
)

func main() {
	a := make([]int, 2, 4)
	x := append(a, 1)
	y := append(a, 2)
	fmt.Println(x, y)
	p := slices.Concat(a, []int{1})
	q := slices.Concat(a, []int{2})
	fmt.Println(p, q)
}
```

```output for=go-append-shared
[0 0 2] [0 0 2]
[0 0 1] [0 0 2]
```

- **Rust append empties the other vector.** `append` "Moves all the elements of other into self, leaving other empty"[^5]. Use `extend_from_slice` or `concat` when the code needs `b` after the call:

```rust run id=rust-append-empties
fn main() {
    let mut a = vec![1, 2];
    let mut b = vec![3, 4];
    a.append(&mut b);
    println!("{a:?} {b:?}");
    let mut c = vec![1, 2];
    let d = vec![3, 4];
    c.extend_from_slice(&d);
    println!("{c:?} {d:?} {:?}", [c.as_slice(), d.as_slice()].concat());
}
```

```output for=rust-append-empties
[1, 2, 3, 4] []
[1, 2, 3, 4] [3, 4] [1, 2, 3, 4, 3, 4]
```

- **C memcpy counts bytes, not items.** `memcpy` "Copies count characters"[^6]. A count of 3 copies 3 bytes, which is less than one 4-byte `int`. The example uses `calloc`, so the items that the short copy misses are 0. The output is from a little-endian machine with 4-byte `int`:

```c run id=c-memcpy-bytes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int a[] = {1, 2, 3};
    int *wrong = calloc(3, sizeof *wrong);
    int *right = calloc(3, sizeof *right);
    if (wrong == NULL || right == NULL) {
        return 1;
    }
    memcpy(wrong, a, 3);
    memcpy(right, a, 3 * sizeof *a);
    printf("%d %d %d\n", wrong[0], wrong[1], wrong[2]);
    printf("%d %d %d\n", right[0], right[1], right[2]);
    free(wrong);
    free(right);
    return 0;
}
```

```output for=c-memcpy-bytes
1 0 0
1 2 3
```

- **C++ insert of a vector into itself is undefined.** For the range form of `insert`, "If first and last are iterators into *this, the behavior is undefined"[^12]. So `a.insert(a.end(), a.begin(), a.end())` is a bug. Copy the vector first:

```cpp run id=cpp-insert-self
#include <iostream>
#include <vector>

int main() {
    std::vector<int> a{1, 2};
    const std::vector<int> copy = a;
    a.insert(a.end(), copy.begin(), copy.end());
    for (int x : a) {
        std::cout << x << " ";
    }
    std::cout << "size=" << a.size() << "\n";
}
```

```output for=cpp-insert-self
1 2 1 2 size=4
```

- **Bash without quotes splits the items.** Without double quotes, an item with a space becomes two items. In the example, the unquoted form gives 4 items, and the quoted form keeps the 3 items:

```bash run id=bash-concat-quotes
a=("New York" Paris)
b=(Rome)
c=(${a[@]} ${b[@]})
d=("${a[@]}" "${b[@]}")
echo "${#c[@]} ${#d[@]}"
printf '%s\n' "${d[@]}"
```

```output for=bash-concat-quotes
4 3
New York
Paris
Rome
```

## Related tasks

- [[append-to-a-list]]: add one item at the end or at a position.
- [[create-an-array]]: make an array of a given length.
- [[zip-two-lists]]: pair the items of two lists.
- [[flatten-a-nested-list]]: join the lists inside a list into one list.

## References
