---
tag: P2C8VN
kind: task
slug: create-a-set
name: Create a set
aliases: [make a set, HashSet, add to a set, remove from a set, test set membership]
area: data-and-values/maps-and-sets
level: intro
summary: Create an empty set, add and remove items and test membership, by adding each word that is not in the set and removing each word that is in it.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://doc.rust-lang.org/std/collections/struct.HashSet.html
  title: Rust standard library, struct HashSet
  locator: insert; remove
- url: https://docs.python.org/3/tutorial/datastructures.html#sets
  title: The Python Tutorial, Data Structures
  locator: 5.4 Sets
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Set/Set
  title: MDN Web Docs, Set() constructor
  locator: Parameters
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Set.html
  title: Java SE 25 API, Interface Set
  locator: Unmodifiable Sets
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.hashset-1?view=net-10.0
  title: .NET API reference, HashSet<T> class
  locator: Constructors
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Map types
- url: https://doc.rust-lang.org/std/vec/struct.Vec.html
  title: Rust standard library, struct Vec
  locator: dedup
- url: https://en.cppreference.com/w/cpp/container/set
  title: cppreference.com, std::set
  locator: Description
- url: https://en.cppreference.com/w/c/string/byte/strcmp
  title: cppreference.com, strcmp (C)
  locator: Return value
- url: https://tiswww.case.edu/php/chet/bash/bashref.html
  title: Bash Reference Manual
  locator: 6.7 Arrays
---

## Task

Create a set of words. The function `toggle(words)` starts with an empty set. For each word in `words`, it removes the word if the set has it, else it adds the word. It returns the words of the set in ascending order by their bytes. Thus the result has each word that is in `words` an odd number of times. For example, `toggle(["red", "blue", "red", "green"])` is `["blue", "green"]`.

Each word has 1 or more ASCII letters. The list can be empty.

## Solutions

| Language | Empty set | Test | Add | Remove |
|---|---|---|---|---|
| Python | `set()` | `w in s` | `s.add(w)` | `s.remove(w)` |
| JavaScript, TypeScript | `new Set()` | `s.has(w)` | `s.add(w)` | `s.delete(w)` |
| Java | `new HashSet<>()` | `s.contains(w)` | `s.add(w)` | `s.remove(w)` |
| C# | `new HashSet<string>()` | `s.Contains(w)` | `s.Add(w)` | `s.Remove(w)` |
| Go | `make(map[string]struct{})` | `_, ok := s[w]` | `s[w] = struct{}{}` | `delete(s, w)` |
| Rust | `HashSet::new()` | `s.contains(w)` | `s.insert(w)` | `s.remove(w)` |
| C | a sorted array | binary search | `memmove`, then store | `memmove` |
| C++ | `std::unordered_set<std::string>` | `s.contains(w)` | `s.insert(w)` | `s.erase(w)` |
| Bash | `declare -A s=()` | `[[ -v s[$w] ]]` | `s[$w]=1` | `unset 's[$w]'` |

- **Remove tells if the item was there.** In most of these languages, the remove operation returns whether the set had the item, so one call tests and removes. Java and Rust `remove` and C# `Remove` return a bool, JavaScript `delete` returns a bool, and C++ `erase` returns the number of items that it removed. In the same way, Rust `insert` returns "whether the value was newly inserted"[^1]. Python `remove` raises `KeyError` for a missing item, and `discard` does not.
- **Order.** A hash set has no useful order, so each solution sorts the items at the end. A sorted set, such as Java `TreeSet`, Rust `BTreeSet` and C++ `std::set`, keeps its items in order. See [[ordered-map]].
- **Go.** Go has no set type. A map whose values have the type `struct{}` is a set, and the values take no memory.
- **C.** C has no set type. A sorted array is a set: a binary search tests membership, and `memmove` opens a gap for a new item or closes the gap of a removed item. Each change moves up to n pointers, so the array is good for small sets.
- **Bash.** The keys of an associative array are the set. The quotes in `unset 's[$w]'` keep the shell from reading `s[...]` as a glob pattern.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: no words give an empty set.
- `two-times`: the second `a` removes the first, so the set is empty.
- `three-times`: the third `a` adds it again.
- `case`: `a` and `A` are two items. `A` comes first, because its byte is smaller.
- `prefix`: `a` comes before `ab`.

## Pitfalls

- **Python `{}` is an empty dict, and a set made from a string is a set of letters.** "to create an empty set you have to use set(), not {}; the latter creates an empty dictionary"[^2]. Also, `set` takes an iterable, and a string is an iterable of its characters. Put the string in a list or in braces:

```python run id=python-empty-braces
print(type({}).__name__, type(set()).__name__)
print(sorted(set("red")), sorted({"red"}))
```

```output for=python-empty-braces
dict set
['d', 'e', 'r'] ['red']
```

- **JavaScript a Set made from a string is a set of letters.** For the constructor, if an "iterable object (such as an array) is passed, all of its elements will be added to the new Set"[^3]. A string is iterable, and its elements are its characters:

```javascript run id=javascript-set-of-string
console.log(new Set("red").size, new Set(["red"]).size);
console.log([...new Set("red")]);
```

```output for=javascript-set-of-string
3 1
[ 'r', 'e', 'd' ]
```

- **Java `Set.of` throws for a duplicate.** The sets of `Set.of` "reject duplicate elements at creation time. Duplicate elements passed to a static factory method result in IllegalArgumentException."[^4] To make a set from data that can have duplicates, copy the data into a `HashSet` or a `TreeSet`:

```java run id=java-set-of-duplicate
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

public class Main {
  public static void main(String[] args) {
    try {
      Set.of("red", "blue", "red");
    } catch (IllegalArgumentException e) {
      System.out.println(e.getClass().getSimpleName());
    }
    System.out.println(new TreeSet<>(List.of("red", "blue", "red")));
  }
}
```

```output for=java-set-of-duplicate
IllegalArgumentException
[blue, red]
```

- **C# a string set is case-sensitive by default.** `new HashSet<string>()` "uses the default equality comparer for the set type"[^5], and for strings that comparer is ordinal and case-sensitive. To make `red` and `Red` one item, give the constructor a comparer:

```csharp run id=csharp-set-comparer
using System;
using System.Collections.Generic;

var a = new HashSet<string> { "red", "Red" };
var b = new HashSet<string>(StringComparer.OrdinalIgnoreCase) { "red", "Red" };
Console.WriteLine(a.Count + " " + b.Count + " " + b.Contains("RED"));
```

```output for=csharp-set-comparer
2 1 True
```

- **Go a `map[string]bool` with a false value still has the key.** A set of type `map[string]bool` reads `false` for a missing key, so `s[w] = false` looks like a removal. But the key stays, and "The number of map elements is called its length."[^6] Use `delete`:

```go run id=go-bool-set-false
package main

import "fmt"

func main() {
	seen := map[string]bool{"red": true}
	seen["red"] = false
	fmt.Println(seen["red"], len(seen))
	delete(seen, "red")
	fmt.Println(len(seen))
}
```

```output for=go-bool-set-false
false 1
0
```

- **Rust `dedup` removes only duplicates that are next to each other.** `dedup` "Removes consecutive repeated elements in the vector according to the PartialEq trait implementation."[^7] Also: "If the vector is sorted, this removes all duplicates."[^7] Sort first, or collect the items into a set:

```rust run id=rust-dedup-consecutive
fn main() {
    let mut words = vec!["red", "blue", "red"];
    words.dedup();
    println!("{:?}", words);
    words.sort();
    words.dedup();
    println!("{:?}", words);
}
```

```output for=rust-dedup-consecutive
["red", "blue", "red"]
["blue", "red"]
```

- **C++ a set of `const char *` compares addresses.** "std::set is an associative container that contains a sorted set of unique objects of type Key. Sorting is done using the key comparison function Compare."[^8] The default `Compare` is `std::less<Key>`, and for a pointer type it compares the addresses. Two arrays with the text `red` are two items. Use `std::string`:

```cpp run id=cpp-set-of-pointers
#include <iostream>
#include <set>
#include <string>

int main() {
    char a[] = "red";
    char b[] = "red";
    std::set<const char *> ptrs{a, b};
    std::set<std::string> strs{a, b};
    std::cout << ptrs.size() << ' ' << strs.size() << '\n';
}
```

```output for=cpp-set-of-pointers
2 1
```

- **C `strcmp` gives 0 for equal strings.** `strcmp` returns "Zero if lhs and rhs compare equal."[^9] Thus `if (strcmp(a, b))` is true when the strings are different, which is the opposite of a membership test. Compare the result with 0:

```c run id=c-strcmp-zero
#include <stdio.h>
#include <string.h>

int main(void) {
    const char *word = "red";
    if (strcmp(word, "red")) {
        puts("strcmp is true");
    } else {
        puts("strcmp is 0");
    }
    printf("%d\n", strcmp(word, "red") == 0);
    return 0;
}
```

```output for=c-strcmp-zero
strcmp is 0
1
```

- **Bash `${#s}` is not the size of the set.** "Referencing an array variable without a subscript is equivalent to referencing with a subscript of 0."[^10] Thus `${#s}` is the length of the item with the key `0`. But "If subscript is ‘@’ or ‘*’, the expansion is the number of elements in the array."[^10] Use `${#s[@]}`:

```bash run id=bash-set-size
declare -A colors=([red]=1 [blue]=1)
echo "${#colors} ${#colors[@]}"
```

```output for=bash-set-size
0 2
```

## Related tasks

- [[do-set-operations]]: union, intersection and difference of two sets.
- [[use-a-custom-key-in-a-map]]: the rules for a key of a user type are the same for an item of a set.
- [[create-a-map]]: a map is a set of keys with a value for each key.
- [[check-whether-a-list-contains-a-value]]: a membership test on a list takes linear time.

## References
