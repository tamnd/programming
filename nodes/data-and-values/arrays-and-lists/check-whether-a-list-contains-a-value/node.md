---
tag: B5BWVX
kind: task
slug: check-whether-a-list-contains-a-value
name: Check whether a list contains a value
aliases: [list contains, array includes, membership test, in operator, value in list, linear search, find an item in a list, includes]
area: data-and-values/arrays-and-lists
level: intro
summary: Tell whether a value is one of the items of a list, and know which equality each language uses, how NaN and object items behave, and when a set is the better structure.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/reference/expressions.html#membership-test-operations
  title: The Python Language Reference, Expressions
  locator: 6.10.2 Membership test operations
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/includes
  title: MDN Web Docs, Array.prototype.includes()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/List.html#contains(java.lang.Object)
  title: Java SE 25 API, Interface List
  locator: contains(Object o)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.contains
  title: .NET API reference, List<T>.Contains(T)
  locator: Remarks
- url: https://pkg.go.dev/slices#Contains
  title: Go standard library, package slices
  locator: func Contains and func ContainsFunc
- url: https://doc.rust-lang.org/std/primitive.slice.html#method.contains
  title: Rust standard library, Primitive Type slice
  locator: contains
- url: https://en.cppreference.com/w/cpp/algorithm/ranges/contains
  title: cppreference.com, std::ranges::contains
  locator: Description
- url: https://en.cppreference.com/w/cpp/algorithm/find
  title: cppreference.com, std::find
  locator: Description
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Conditional-Constructs
  title: Bash Reference Manual
  locator: 3.2.5.2 Conditional Constructs, [[...]]
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/indexOf
  title: MDN Web Docs, Array.prototype.indexOf()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Arrays.html#asList(T...)
  title: Java SE 25 API, Class Arrays
  locator: asList(T... a)
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/record
  title: C# language reference, Records
  locator: Value equality
- url: https://go.dev/ref/spec#Comparison_operators
  title: The Go Programming Language Specification
  locator: Comparison operators
- url: https://en.cppreference.com/w/c/string/byte/strcmp
  title: cppreference.com, strcmp
  locator: Return value
- url: https://en.cppreference.com/w/cpp/algorithm/binary_search
  title: cppreference.com, std::binary_search
  locator: Description
---

## Task

Tell whether a value is in a list. The function `contains(items, value)` returns true if some item of `items` is equal to `value`, and false if not. For example, `contains([1, 2, 3], 2)` is true, and `contains([1, 2, 3], 4)` is false.

The function must not change `items`. Each item and the value are 32-bit signed integers, and the list has 0 to 1000 items.

## Solutions

| Language | Contains | Equality used |
|---|---|---|
| Python | `value in items` | `is`, then `==` |
| JavaScript, TypeScript | `items.includes(value)` | SameValueZero |
| Java | `Arrays.stream(items).anyMatch(x -> x == value)`, `list.contains(value)` | `==` for `int`, `Objects.equals` for a `List` |
| C# | `Array.IndexOf(items, value) >= 0`, `list.Contains(value)` | `EqualityComparer<T>.Default` |
| Go | `slices.Contains(items, value)` | `==` |
| Rust | `items.contains(&value)` | `PartialEq` |
| C | a loop with `==` | `==`, or `strcmp` for strings |
| C++ | `std::ranges::contains(items, value)` | `==` |
| Bash | a loop with `[[ $item == "$value" ]]` | string equality |

- **Python.** "For container types such as list, tuple, set, frozenset, dict, or collections.deque, the expression x in y is equivalent to any(x is e or x == e for e in y)"[^1].
- **JavaScript.** "The includes() method compares searchElement to elements of the array using the SameValueZero algorithm"[^2]. TypeScript uses the same method on a `readonly number[]`.
- **Java.** `List.contains` returns true "if and only if this list contains at least one element e such that Objects.equals(o, e)"[^3]. The input is an `int[]`, so the solution uses a stream of `int`.
- **C#.** `List<T>.Contains` "determines equality using the default equality comparer EqualityComparer<T>.Default for T"[^4], and "This method performs a linear search"[^4]. The solution uses `Array.IndexOf` on the array.
- **Go.** "Contains reports whether v is present in s"[^5]. The item type must be comparable. For other tests, `slices.ContainsFunc` takes a function.
- **Rust.** `contains` takes a reference to the value, as in `items.contains(&value)`[^6].
- **C.** C has no contains function for an `int` array. The solution compares each item with `==` and returns at the first match.
- **C++.** `std::ranges::contains` "Checks whether or not the source range contains the target value value"[^7]. With an older standard library, compare the result of `std::find` with the end: `std::find` returns "an iterator to the first element in the source range [first, last) that satisfies specific criteria (or last if there is no such iterator)"[^8].
- **Bash.** For `[[ ... == ... ]]`, "the string to the right of the operator is considered a pattern"[^9]. The solution puts the value in double quotes, so the right side is a string. The function returns the status 0 for found and 1 for not found, so a caller can use it as the test of an `if`.

Each solution is a linear search: it compares the value with the items in order, so it takes time in proportion to the length of the list. A program that tests many values against one list can put the items in a set once, and then each test takes about the same time for any size.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: an empty list contains no value, so the result is false.
- `single-yes` and `single-no`: a list of one item.
- `first` and `last`: the value at the first and at the last position. A loop with a bound off by one misses one of them.
- `duplicates`: the value can be in the list more than once, and the result is still true.
- `negative` and `zero-absent`: negative items, and 0 when the list has no 0. A test that uses a position as a truth value fails here.
- `limit` and `limit-absent`: the smallest 32-bit integer is found in a list that has it, and is not found in a list that has only the largest.
- Each test checks that `items` does not change.

## Pitfalls

- **Python `in` tests identity before equality.** Because `x in y` is `any(x is e or x == e for e in y)`[^1], a NaN object is found in a list that holds the same object, though `nan == nan` is false. A different NaN object is not found. Also, `True == 1` in Python, so `True in [1]` is true:

```python run id=python-in-identity
nan = float("nan")
print(nan == nan, nan in [nan], float("nan") in [nan])
print(True in [1], 1.0 in [1])
```

```output for=python-in-identity
False True False
True True
```

- **JavaScript `indexOf` does not find NaN, and 0 is falsy.** SameValueZero works "like strict equality === (where -0 and +0 are considered equal), with the exception that NaN is considered equal to itself"[^2]. But for `indexOf`, "NaN values are never compared as equal, so indexOf() always returns -1 when searchElement is NaN"[^10]. Also, `if (a.indexOf(x))` is false when `x` is the first item, because the position 0 is falsy. Use `includes` for a yes or no answer:

```javascript run id=javascript-indexof-nan
const a = [1, NaN];
console.log(a.includes(NaN), a.indexOf(NaN));
console.log(a.indexOf(1) ? "found" : "not found");
console.log(a.includes(1) ? "found" : "not found");
```

```output for=javascript-indexof-nan
true -1
not found
found
```

- **Java `Arrays.asList` of an `int[]` is a list of one array.** `asList(T... a)` "Returns a fixed-size list backed by the specified array"[^11], but `T` cannot be `int`. So `Arrays.asList(new int[] {1, 2, 3})` is a `List<int[]>` with one item, and `contains(1)` is false. Also, `contains` uses `Objects.equals`[^3], and an `Integer` is never equal to a `Long`, so `List.of(1L).contains(1)` is false:

```java run id=java-aslist-int-array
import java.util.Arrays;
import java.util.List;

public class Main {
  public static void main(String[] args) {
    int[] a = {1, 2, 3};
    System.out.println(Arrays.asList(a).contains(1) + " " + Arrays.asList(a).size());
    System.out.println(Arrays.stream(a).anyMatch(x -> x == 1));
    List<Long> b = List.of(1L, 2L);
    System.out.println(b.contains(1) + " " + b.contains(1L));
  }
}
```

```output for=java-aslist-int-array
false 1
true
false true
```

- **C# `Contains` compares class objects by reference.** `Contains` uses the default equality comparer[^4]. "For class types, two objects are equal if they refer to the same object in memory"[^12], unless the class overrides `Equals`. A record has value equality. So a new object with the same fields is not found in a list of a class, but it is found in a list of a record:

```csharp run id=csharp-contains-reference
var classes = new List<PointClass> { new() { X = 1, Y = 2 } };
var records = new List<PointRecord> { new(1, 2) };
Console.WriteLine(classes.Contains(new PointClass { X = 1, Y = 2 }));
Console.WriteLine(records.Contains(new PointRecord(1, 2)));

class PointClass
{
    public int X { get; init; }
    public int Y { get; init; }
}

record PointRecord(int X, int Y);
```

```output for=csharp-contains-reference
False
True
```

- **Go `slices.Contains` does not find NaN.** `Contains` uses `==`, and "Two floating-point values are compared as defined by the IEEE 754 standard"[^13]. In IEEE 754, NaN is not equal to any value, NaN too. Use `slices.ContainsFunc`[^5] with `math.IsNaN`:

```go run id=go-contains-nan
package main

import (
	"fmt"
	"math"
	"slices"
)

func main() {
	a := []float64{1, math.NaN()}
	fmt.Println(slices.Contains(a, math.NaN()))
	fmt.Println(slices.ContainsFunc(a, math.IsNaN))
}
```

```output for=go-contains-nan
false
true
```

- **Rust `contains` needs a `&T`, so a `&str` does not work on a `Vec<String>`.** A call such as `v.contains(&s)` with a `&str` named `s` does not compile, because the item type is `String`. "If you do not have a &T, but some other value that you can compare with one (for example, String implements PartialEq<str>), you can use iter().any"[^6]:

```rust run id=rust-contains-string
fn main() {
    let v = vec![String::from("a"), String::from("b")];
    println!("{}", v.contains(&String::from("b")));
    println!("{}", v.iter().any(|s| s == "b"));
}
```

```output for=rust-contains-string
true
true
```

- **C `==` on strings compares the pointers.** Two arrays with the same characters are at different addresses, so `==` is false. `strcmp` returns "Zero if lhs and rhs compare equal"[^14]:

```c run id=c-strcmp-contains
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    const char *names[] = {"ann", "bob"};
    char want[] = "bob";
    bool by_pointer = false;
    bool by_text = false;
    for (size_t i = 0; i < 2; i++) {
        if (names[i] == want) {
            by_pointer = true;
        }
        if (strcmp(names[i], want) == 0) {
            by_text = true;
        }
    }
    printf("%d %d\n", by_pointer, by_text);
    return 0;
}
```

```output for=c-strcmp-contains
0 1
```

- **C++ `std::binary_search` needs a sorted range.** It "Checks if an element equivalent to value exists in the partitioned source range [first, last)"[^15], and the behavior is undefined if the range is not partitioned[^15]. On an unsorted vector, it can miss an item that is there. Sort the vector first, or use `std::ranges::contains`. The output is from the GCC library:

```cpp run id=cpp-binary-search-unsorted
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{5, 1, 4, 2, 3};
    std::cout << std::ranges::contains(v, 1) << " " << std::binary_search(v.begin(), v.end(), 1) << "\n";
    std::ranges::sort(v);
    std::cout << std::binary_search(v.begin(), v.end(), 1) << "\n";
}
```

```output for=cpp-binary-search-unsorted
1 0
1
```

- **Bash an unquoted right side is a pattern, and a joined string finds parts of items.** The right side of `==` is a pattern[^9], so with `x='*'`, `[[ $item == $x ]]` is true for each item. A test on the joined list, `[[ " ${a[*]} " == *" $x "* ]]`, finds `York` in the item `New York`. Loop over the items and quote the value:

```bash run id=bash-contains-pattern
a=("New York" Paris)
x='*'
for item in "${a[@]}"; do
  if [[ $item == $x ]]; then echo "unquoted: $item"; fi
  if [[ $item == "$x" ]]; then echo "quoted: $item"; fi
done
x=York
if [[ " ${a[*]} " == *" $x "* ]]; then echo "joined: found"; fi
```

```output for=bash-contains-pattern
unquoted: New York
unquoted: Paris
joined: found
```

## Related tasks

- [[filter-a-list]]: keep all the items that match, not only test for one.
- [[search-a-sorted-list]]: find a value in a sorted list in fewer steps.
- [[remove-duplicates-from-a-list]]: uses a membership test for each item.
- [[count-the-items-of-each-kind]]: count the matches instead of stopping at the first.

## References
