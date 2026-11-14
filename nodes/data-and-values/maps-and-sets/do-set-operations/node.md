---
tag: ASQ7VQ
kind: task
slug: do-set-operations
name: Do set operations
aliases: [set union, set intersection, set difference, symmetric difference, subset test]
area: data-and-values/maps-and-sets
level: intro
summary: Find the union, intersection, difference and symmetric difference of two sets of words, and test whether one set is a subset of the other.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#set-types-set-frozenset
  title: The Python Standard Library, Set Types
  locator: set and frozenset methods
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Set
  title: MDN Web Docs, Set
  locator: Set composition; Instance methods
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.hashset-1?view=net-10.0
  title: .NET API reference, HashSet<T> class
  locator: Methods
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Set.html
  title: Java SE 25 API, Interface Set
  locator: retainAll
- url: https://pkg.go.dev/maps
  title: Go standard library, package maps
  locator: func Equal
- url: https://doc.rust-lang.org/std/collections/struct.HashSet.html
  title: Rust standard library, struct HashSet
  locator: difference
- url: https://en.cppreference.com/w/cpp/algorithm/includes
  title: cppreference.com, std::includes
  locator: Description
- url: https://en.cppreference.com/w/c/language/operator_logical
  title: cppreference.com, Logical operators (C)
  locator: Logical AND
- url: https://tiswww.case.edu/php/chet/bash/bashref.html
  title: Bash Reference Manual
  locator: 6.7 Arrays
---

## Task

Make a set of the words of each of two lists, and compare the two sets. The function `compare_sets(a, b)` returns five values:

1. the union: the words that are in `a` or in `b`;
2. the intersection: the words that are in `a` and in `b`;
3. the difference: the words that are in `a` and not in `b`;
4. the symmetric difference: the words that are in exactly one of `a` and `b`;
5. `true` if each word of `a` is in `b`, that is, if the set of `a` is a subset of the set of `b`.

Each list of words is in ascending order by bytes and has no repeats. For example, `compare_sets(["red", "blue", "green"], ["blue", "yellow"])` is `(["blue", "green", "red", "yellow"], ["blue"], ["green", "red"], ["green", "red", "yellow"], false)`.

Each word has 1 or more ASCII letters. A list can be empty and can have a word more than one time.

## Solutions

| Language | Union | Intersection | Difference | Symmetric difference | Subset |
|---|---|---|---|---|---|
| Python | `a \| b` | `a & b` | `a - b` | `a ^ b` | `a <= b` |
| JavaScript, TypeScript | `a.union(b)` | `a.intersection(b)` | `a.difference(b)` | `a.symmetricDifference(b)` | `a.isSubsetOf(b)` |
| Java | `u.addAll(b)` | `i.retainAll(b)` | `d.removeAll(b)` | union minus intersection | `b.containsAll(a)` |
| C# | `u.UnionWith(b)` | `i.IntersectWith(b)` | `d.ExceptWith(b)` | `s.SymmetricExceptWith(b)` | `a.IsSubsetOf(b)` |
| Go | a loop | a loop | a loop | a loop | the difference is empty |
| Rust | `a.union(&b)` | `a.intersection(&b)` | `a.difference(&b)` | `a.symmetric_difference(&b)` | `a.is_subset(&b)` |
| C | a merge walk | a merge walk | a merge walk | a merge walk | the difference is empty |
| C++ | `std::ranges::set_union` | `std::ranges::set_intersection` | `std::ranges::set_difference` | `std::ranges::set_symmetric_difference` | `std::ranges::includes(b, a)` |
| Bash | a loop | a loop | a loop | a loop | the difference is empty |

- **Python.** Each operation has an operator and a method, and both make a new set. For example, `a ^ b` and `a.symmetric_difference(b)` each "Return a new set with elements in either the set or other but not both."[^1]
- **JavaScript and TypeScript.** A `Set` has a method for each operation. For example, `intersection` "Takes a set and returns a new set containing elements in both this set and the given set."[^2] An engine that does not have these methods needs a loop with `has`, as in the Go solution.
- **Java and C#.** The methods change the set that they are called on. For example, C# `IntersectWith` changes the set "to contain only elements that are present in that object and in the specified collection"[^3]. Thus each solution copies the first set before each operation.
- **Go, C and Bash.** These languages have no set operations. The Go and Bash solutions keep each set as the keys of a map and do each operation with a loop that tests membership. The C solution sorts the two arrays and walks them together, as in the merge step of merge sort. In each of them, the set of `a` is a subset of the set of `b` when the difference is empty.
- **C++.** The set algorithms work on sorted ranges, not on a set type. Thus the solution sorts each vector and removes the repeats first. The algorithms then also work on two `std::set` objects, which are sorted.
- **Order.** A Java `TreeSet` and a Rust `BTreeSet` keep their items in order, so these solutions do not sort the results. The other solutions sort each result.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `both-empty`: all four lists are empty, and the empty set is a subset of each set, so the subset value is `true`.
- `empty-a`: the empty set of `a` is a subset of the set of `b`.
- `empty-b`: the difference is all of `a`, and the set of `a` is not a subset.
- `equal`: two lists with the same words in a different order make the same set. The difference and the symmetric difference are empty, and each set is a subset of the other.
- `disjoint`: the sets have no word in common, so the intersection is empty and the symmetric difference is the union.
- `duplicates`: a word that is in a list two times is in the set one time.
- `case`: `a` and `A` are different words. `A` comes first, because its byte is smaller.

## Pitfalls

- **Python an operator needs a set, but a method takes any iterable.** "Note, the non-operator versions of union(), intersection(), difference(), symmetric_difference(), issubset(), and issuperset() methods will accept any iterable as an argument. In contrast, their operator based counterparts require their arguments to be sets."[^1] Also, `<` tests for a proper subset, which is false for two equal sets. Use `<=` to test for a subset:

```python run id=python-operator-needs-set
colors = {"red", "blue"}
try:
    colors | ["green"]
except TypeError as e:
    print(type(e).__name__)
print(sorted(colors.union(["green"])))
print(colors < {"red", "blue"}, colors <= {"red", "blue"})
```

```output for=python-operator-needs-set
TypeError
['blue', 'green', 'red']
False True
```

- **JavaScript `===` compares two sets by identity.** Two `Set` objects with the same items are two objects, so `===` gives `false`. To compare the items, compare the sizes and use `isSubsetOf`, which "Takes a set and returns a boolean indicating if all elements of this set are in the given set."[^2]

```javascript run id=javascript-set-equality
const a = new Set(["red", "blue"]);
const b = new Set(["blue", "red"]);
console.log(a === b);
console.log(a.size === b.size && a.isSubsetOf(b));
```

```output for=javascript-set-equality
false
true
```

- **Java `retainAll` changes the set.** `retainAll` "Retains only the elements in this set that are contained in the specified collection"[^4], and `addAll` and `removeAll` also change the set. A second variable for the same set does not make a copy. Copy the set first:

```java run id=java-retainall-changes-set
import java.util.Set;
import java.util.TreeSet;

public class Main {
  public static void main(String[] args) {
    Set<String> blue = Set.of("blue");
    Set<String> a = new TreeSet<>(Set.of("red", "blue"));
    Set<String> both = a;
    both.retainAll(blue);
    System.out.println(a);
    Set<String> c = new TreeSet<>(Set.of("red", "blue"));
    Set<String> common = new TreeSet<>(c);
    common.retainAll(blue);
    System.out.println(c + " " + common);
  }
}
```

```output for=java-retainall-changes-set
[blue]
[blue, red] [blue]
```

- **C# `IsProperSubsetOf` is false for equal sets.** `IsSubsetOf` and `IsProperSubsetOf` are different tests. The second one tells if the set is a "proper subset of the specified collection"[^3], so the other collection must have one or more items that the set does not have. To test if two sets have the same items, use `SetEquals`:

```csharp run id=csharp-proper-subset
using System;
using System.Collections.Generic;

var a = new HashSet<string> { "red", "blue" };
var b = new HashSet<string> { "blue", "red" };
Console.WriteLine(a.IsSubsetOf(b) + " " + a.IsProperSubsetOf(b) + " " + a.SetEquals(b));
```

```output for=csharp-proper-subset
True False True
```

- **Go `reflect.DeepEqual` tells a nil set from an empty set.** A nil map and an empty map are both empty sets, but `reflect.DeepEqual` gives `false` for them. `maps.Equal` "reports whether two maps contain the same key/value pairs."[^5] Use `maps.Equal` to compare two sets:

```go run id=go-nil-set-equal
package main

import (
	"fmt"
	"maps"
	"reflect"
)

func main() {
	var a map[string]struct{}
	b := map[string]struct{}{}
	fmt.Println(reflect.DeepEqual(a, b), maps.Equal(a, b))
}
```

```output for=go-nil-set-equal
false true
```

- **Rust `difference` is not symmetric.** `a.difference(&b)` "Visits the values representing the difference, i.e., the values that are in self but not in other."[^6] Thus `b.difference(&a)` gives other values. For the values that are in exactly one of the sets, use `symmetric_difference`. A `BTreeSet` has the same methods:

```rust run id=rust-difference-order
use std::collections::BTreeSet;

fn main() {
    let a = BTreeSet::from(["red", "blue"]);
    let b = BTreeSet::from(["blue", "green"]);
    println!("{:?}", a.difference(&b).collect::<Vec<_>>());
    println!("{:?}", b.difference(&a).collect::<Vec<_>>());
    println!("{:?}", a.symmetric_difference(&b).collect::<Vec<_>>());
}
```

```output for=rust-difference-order
["red"]
["green"]
["green", "red"]
```

- **C++ `std::includes` takes the larger range first.** `std::includes` "Checks if the sorted target range [first2, last2) is a subsequence of the sorted source range [first1, last1)."[^7] Thus `includes(x, y)` tells if `y` is in `x`. To test if `small` is a subset of `big`, write `includes(big, small)`:

```cpp run id=cpp-includes-order
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> small{"b"};
    std::vector<std::string> big{"a", "b", "c"};
    std::cout << std::boolalpha << std::ranges::includes(small, big) << ' '
              << std::ranges::includes(big, small) << '\n';
}
```

```output for=cpp-includes-order
false true
```

- **C `&&` is not the intersection of two bit sets.** An `unsigned` value can be a set of small numbers, with one bit for each number. Then `a & b` is the intersection and `a | b` is the union. But `&&` is a logical operator: it "has type int and the value 1 if both lhs and rhs compare unequal to zero."[^8] Thus `a && b` is 1 for two sets that have no item in common:

```c run id=c-bitset-and
#include <stdio.h>

int main(void) {
    unsigned a = 0x3u; /* the set {0, 1} */
    unsigned b = 0x4u; /* the set {2} */
    printf("%d %u %u\n", a && b, a & b, a | b);
    return 0;
}
```

```output for=c-bitset-and
1 0 7
```

- **Bash an array without `declare -A` is not a set of words.** An array that is not declared as associative is an indexed array, and for an indexed array, "The subscript is treated as an arithmetic expression that must evaluate to a number greater than or equal to zero."[^9] In an arithmetic expression, a word such as `red` is the name of a variable, and an unset variable is 0. Thus each word is the index 0. Use `declare -A`:

```bash run id=bash-declare-a
colors=()
colors[red]=1
colors[blue]=1
echo "${#colors[@]}"
declare -A names=()
names[red]=1
names[blue]=1
echo "${#names[@]}"
```

```output for=bash-declare-a
1
2
```

## Related tasks

- [[create-a-set]]: create a set, add and remove items, and test membership.
- [[remove-duplicates-from-a-list]]: a set keeps one copy of each item.
- [[merge-two-maps]]: the union of the keys of two maps, with a rule for the values.
- [[use-a-custom-key-in-a-map]]: the rules for a key of a user type are the same for an item of a set.

## References
