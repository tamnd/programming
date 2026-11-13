---
tag: D7CMAP
kind: task
slug: sort-with-a-custom-key
name: Sort with a custom key
aliases: [sort by a key, sort by length, custom sort order, sort with a comparator, sort by two keys]
area: data-and-values/collection-operations
level: intro
summary: Sort a list of words by a key of two parts, the length and then the word, and return a new list.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#list.sort
  title: The Python Standard Library, Built-in Types
  locator: list.sort()
- url: https://learn.microsoft.com/en-us/dotnet/api/system.stringcomparer.ordinal?view=net-10.0
  title: .NET API reference, StringComparer.Ordinal property
  locator: Definition
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
  title: MDN Web Docs, Array.prototype.sort()
  locator: Description; Parameters
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/toSorted
  title: MDN Web Docs, Array.prototype.toSorted()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/List.html
  title: Java SE 25 API, Interface List
  locator: Unmodifiable Lists
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.orderby?view=net-10.0
  title: .NET API reference, Enumerable.OrderBy method
  locator: Remarks
- url: https://pkg.go.dev/slices#SortFunc
  title: Go standard library, package slices
  locator: func SortFunc; func SortStableFunc
- url: https://pkg.go.dev/cmp#Or
  title: Go standard library, package cmp
  locator: func Or
- url: https://doc.rust-lang.org/std/primitive.slice.html#method.sort_by
  title: Rust standard library, primitive type slice
  locator: sort_by; sort_by_key
- url: https://doc.rust-lang.org/std/cmp/enum.Ordering.html#method.then_with
  title: Rust standard library, enum Ordering
  locator: then_with
- url: https://en.cppreference.com/w/c/algorithm/qsort
  title: cppreference.com, qsort (C)
  locator: Parameters
- url: https://en.cppreference.com/w/c/language/operator_arithmetic
  title: cppreference.com, Arithmetic operators (C)
  locator: Overflows
- url: https://en.cppreference.com/w/cpp/algorithm/ranges/sort
  title: cppreference.com, std::ranges::sort
  locator: Description
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/sort.html
  title: The Open Group Base Specifications Issue 8, sort
  locator: DESCRIPTION; OPTIONS
---

## Task

Sort a list of words by a key of two parts. The function `sort_by_length(words)` returns the words in ascending order of the length. Words with the same length are in ascending order of their bytes. For example, `sort_by_length(["pear", "fig", "apple", "kiwi"])` is `["fig", "kiwi", "pear", "apple"]`.

Each word has 1 or more lowercase ASCII letters. The list can be empty, and it can have the same word more than once. The function does not change its input, except in C, where the function sorts the array in place.

## Solutions

| Language | Sort by the length, then by the word |
|---|---|
| Python | `sorted` with the key `(len(w), w)` |
| JavaScript, TypeScript | `toSorted` with a comparison of the lengths, then of the words |
| Java | `sorted` with `Comparator.comparingInt(String::length).thenComparing(...)` |
| C# | `OrderBy(w => w.Length).ThenBy(w => w, StringComparer.Ordinal)` |
| Go | `slices.SortFunc` of a copy, with `cmp.Or` |
| Rust | `sort_by_key` of a copy, with the key `(w.len(), *w)` |
| C | `qsort` with a comparison function |
| C++ | `std::ranges::sort` with a projection to a `std::pair` |
| Bash | `sort -k1,1n -k2,2` of lines `LENGTH WORD` in the C locale, then `cut` |

- **A key or a comparison.** Python, Rust and C++ give a key for each word. A tuple or a pair compares its first part, then its second part. For Python, "The key corresponding to each item in the list is calculated once and then used for the entire sorting process."[^1] The other solutions give a comparison of two words, which compares the lengths, then the words.
- **The second part of the key.** Without it, the order of words with the same length depends on the sort. A stable sort keeps them in input order. An unstable sort can put them in any order.
- **The order of the bytes.** The words are lowercase ASCII letters, so the order of the bytes is also the alphabetical order. For C#, `StringComparer.Ordinal` "performs a case-sensitive ordinal string comparison"[^2]. For Bash, the sort uses `LC_ALL=C`, so the order does not depend on the locale of the user.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: an empty list gives an empty list.
- `single`: one word gives a list with that word.
- `same-length`: `["dog", "cat", "ant"]` gives `["ant", "cat", "dog"]`, from the second part of the key only.
- `duplicates`: `["b", "a", "b"]` gives `["a", "b", "b"]`, with the two words `"b"` next to each other.
- `sorted` and `reverse`: a list in order and a list in reverse order.
- `long`: the lengths 1, 9 and 10 are in ascending order as numbers. As strings, "10" is before "9".
- `prefix`: `["ab", "a", "abc", "b"]` gives `["a", "b", "ab", "abc"]`. The length comes first, so `"b"` is before `"ab"`.

## Pitfalls

- **Python list.sort returns None.** "This method modifies the sequence in place for economy of space when sorting a large sequence. To remind users that it operates by side effect, it does not return the sorted sequence"[^1]. Use `sorted` for a new list. Also, `key=len` alone keeps pear before kiwi, because "The sort() method is guaranteed to be stable."[^1]

```python run id=python-sort-none
words = ["pear", "fig", "kiwi"]
print(words.sort())
print(words)
print(sorted(["pear", "kiwi", "fig"], key=len))
print(sorted(["pear", "kiwi", "fig"], key=lambda w: (len(w), w)))
```

```output for=python-sort-none
None
['fig', 'kiwi', 'pear']
['fig', 'pear', 'kiwi']
['fig', 'kiwi', 'pear']
```

- **JavaScript sort changes the array and compares strings.** "The sort() method of Array instances sorts the elements of an array in place and returns the reference to the same array, now sorted."[^3] In the example, `sorted === words` is `true`. Without a comparison function, "The default sort order is ascending, built upon converting the elements into strings"[^3], so 10 is before 9. A comparison function "should return a number where: A negative value indicates that a should come before b."[^3] `toSorted` "returns a new array with the elements sorted in ascending order"[^4]:

```javascript run id=javascript-sort-in-place
const words = ["pear", "fig", "apple", "kiwi"];
const sorted = words.sort((a, b) => a.length - b.length);
console.log(sorted === words, words);
console.log(["ccc", "a", "bb"].toSorted());
console.log([10, 9, 1].toSorted(), [10, 9, 1].toSorted((a, b) => a - b));
```

```output for=javascript-sort-in-place
true [ 'fig', 'pear', 'kiwi', 'apple' ]
[ 'a', 'bb', 'ccc' ]
[ 1, 10, 9 ] [ 1, 9, 10 ]
```

- **Java a List.of list cannot be sorted in place.** For the lists of `List.of`, "Calling any mutator method on the List will always cause UnsupportedOperationException to be thrown."[^5] Sort a copy, or use `stream().sorted(...)`:

```java run id=java-list-of-sort
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class Main {
  public static void main(String[] args) {
    List<String> words = List.of("pear", "fig", "kiwi");
    try {
      Collections.sort(words);
    } catch (UnsupportedOperationException e) {
      System.out.println(e.getClass().getSimpleName());
    }
    List<String> copy = new ArrayList<>(words);
    copy.sort(Comparator.comparingInt(String::length).thenComparing(Comparator.naturalOrder()));
    System.out.println(copy);
  }
}
```

```output for=java-list-of-sort
UnsupportedOperationException
[fig, kiwi, pear]
```

- **C# OrderBy needs ThenBy for the second part.** OrderBy "performs a stable sort; that is, if the keys of two elements are equal, the order of the elements is preserved."[^6] Thus `OrderBy(w => w.Length)` keeps `pear` before `kiwi`. `ThenBy` and `ThenByDescending` "enable you to specify additional sort criteria to sort a sequence."[^6] Give `StringComparer.Ordinal` to `ThenBy`, so that the order of the words does not depend on a culture:

```csharp run id=csharp-thenby
using System;
using System.Linq;

var words = new[] { "pear", "fig", "kiwi" };
Console.WriteLine(string.Join(" ", words.OrderBy(w => w.Length)));
Console.WriteLine(string.Join(" ", words.OrderBy(w => w.Length).ThenBy(w => w, StringComparer.Ordinal)));
Console.WriteLine(string.Compare("a", "b", StringComparison.Ordinal) < 0);
```

```output for=csharp-thenby
fig pear kiwi
fig kiwi pear
True
```

- **Go SortFunc is not stable.** For `slices.SortFunc`, "This sort is not guaranteed to be stable."[^7] `SortStableFunc` "sorts the slice x while keeping the original order of equal elements"[^7], as in the first line of the example. For a full order, compare the words when the lengths are equal. `cmp.Or` "returns the first of its arguments that is not equal to the zero value."[^8]

```go run id=go-sortfunc-stable
package main

import (
	"cmp"
	"fmt"
	"slices"
	"strings"
)

func main() {
	words := []string{"pear", "fig", "kiwi", "apple"}
	slices.SortStableFunc(words, func(a, b string) int { return cmp.Compare(len(a), len(b)) })
	fmt.Println(words)
	slices.SortFunc(words, func(a, b string) int {
		return cmp.Or(cmp.Compare(len(a), len(b)), strings.Compare(a, b))
	})
	fmt.Println(words)
}
```

```output for=go-sortfunc-stable
[fig pear kiwi apple]
[fig kiwi pear apple]
```

- **Rust sort_by with the length only keeps the input order of ties.** `sort_by` is stable: "This sort is stable (i.e., does not reorder equal elements)"[^9]. Thus `pear` stays before `kiwi`. `then_with` "Returns self when it’s not Equal. Otherwise calls f and returns the result."[^10]

```rust run id=rust-sort-by-then-with
fn main() {
    let mut words = vec!["pear", "fig", "kiwi", "apple"];
    words.sort_by(|a, b| a.len().cmp(&b.len()));
    println!("{words:?}");
    words.sort_by(|a, b| a.len().cmp(&b.len()).then_with(|| a.cmp(b)));
    println!("{words:?}");
}
```

```output for=rust-sort-by-then-with
["fig", "pear", "kiwi", "apple"]
["fig", "kiwi", "pear", "apple"]
```

- **C a subtraction of two lengths cannot be negative.** The comparison function of `qsort` "returns a negative integer value if the first argument is less than the second"[^11]. But `strlen` gives a `size_t`, and "Unsigned integer arithmetic is always performed modulo 2n where n is the number of bits in that particular integer."[^12] In the example, the length of fig minus the length of pear is greater than 0, so the first line is 1. Compare the lengths with `<`. Also, `qsort` gives the function pointers to the items, so each argument is a pointer to a `const char *`:

```c run id=c-qsort-compare
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Each item of the array is a const char *, so qsort gives a pointer to it.
static int compare(const void *pa, const void *pb) {
    const char *a = *(const char *const *)pa;
    const char *b = *(const char *const *)pb;
    size_t la = strlen(a);
    size_t lb = strlen(b);
    if (la != lb) {
        return la < lb ? -1 : 1;
    }
    return strcmp(a, b);
}

int main(void) {
    const char *words[] = {"pear", "fig", "kiwi", "apple"};
    size_t la = strlen(words[1]);
    size_t lb = strlen(words[0]);
    printf("%d\n", la - lb > 0);
    qsort(words, 4, sizeof words[0], compare);
    for (size_t i = 0; i < 4; i++) {
        printf("%s%s", i > 0 ? " " : "", words[i]);
    }
    printf("\n");
    return 0;
}
```

```output for=c-qsort-compare
1
fig kiwi pear apple
```

- **C++ std::ranges::sort is not stable.** "The order of equivalent elements is not guaranteed to be preserved."[^13] `std::ranges::stable_sort` by the length keeps the input order of ties, as in the first line of the example. A projection to a pair of the length and the word gives a full order:

```cpp run id=cpp-ranges-sort-pair
#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

int main() {
    std::vector<std::string> words{"pear", "fig", "kiwi", "apple"};
    std::ranges::stable_sort(words, {}, [](const std::string &w) { return w.size(); });
    for (std::size_t i = 0; i < words.size(); i++) {
        std::cout << (i > 0 ? " " : "") << words[i];
    }
    std::cout << '\n';
    std::ranges::sort(words, {}, [](const std::string &w) { return std::pair{w.size(), std::string_view{w}}; });
    for (std::size_t i = 0; i < words.size(); i++) {
        std::cout << (i > 0 ? " " : "") << words[i];
    }
    std::cout << '\n';
}
```

```output for=cpp-ranges-sort-pair
fig pear kiwi apple
fig kiwi pear apple
```

- **Bash sort compares text.** Without `-n`, the comparisons "shall be performed using the collating sequence of the current locale"[^14], so 10 is before 9. With `-n`, sort uses "an initial numeric string"[^14]. `-k1,1n` gives "a key field that begins at field_start and ends at field_end inclusive"[^14], so the key is the length only. `LC_ALL`, if it is set, will "override the values of all the other internationalization variables"[^14], so the order of the second key does not depend on the locale of the user:

```bash run id=bash-sort-numeric
printf '%s\n' 10 9 1 | sort | paste -sd ' ' -
printf '%s\n' 10 9 1 | sort -n | paste -sd ' ' -
printf '%s\n' "4 pear" "3 fig" "4 kiwi" | LC_ALL=C sort -k1,1n -k2,2 | paste -sd ' ' -
```

```output for=bash-sort-numeric
1 10 9
1 9 10
3 fig 4 kiwi 4 pear
```

## Related tasks

- [[stable-sort]]: keep the input order of items with the same key.
- [[comparator]]: a function that compares two items.
- [[schwartzian-transform]]: calculate the key of each item once.
- [[group-items-by-a-key]]: put the words of each length in a group.

## References

