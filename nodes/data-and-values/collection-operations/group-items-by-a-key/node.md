---
tag: 1WZ2MP
kind: task
slug: group-items-by-a-key
name: Group items by a key
aliases: [group by, group a list by a key, bucket items by a key, groupBy, group words by length]
area: data-and-values/collection-operations
level: intro
summary: Group a list of words by their length, with the groups in ascending order of the length and the words of each group in input order.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/collections.html#collections.defaultdict
  title: The Python Standard Library, collections
  locator: defaultdict objects
- url: https://doc.rust-lang.org/std/collections/struct.BTreeMap.html
  title: Rust standard library, struct BTreeMap
  locator: Description
- url: https://en.cppreference.com/w/cpp/container/map
  title: cppreference.com, std::map
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.groupby?view=net-10.0
  title: .NET API reference, Enumerable.GroupBy method
  locator: Remarks
- url: https://tiswww.case.edu/php/chet/bash/bashref.html
  title: Bash Reference Manual
  locator: 3.4 Shell Parameters; 3.5.3 Shell Parameter Expansion
- url: https://docs.python.org/3/library/itertools.html#itertools.groupby
  title: The Python Standard Library, itertools
  locator: itertools.groupby()
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Map/groupBy
  title: MDN Web Docs, Map.groupBy()
  locator: Return value
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort
  title: MDN Web Docs, Array.prototype.sort()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Collectors.html
  title: Java SE 25 API, Class Collectors
  locator: groupingBy(Function)
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements with range clause; Appending to and copying slices
- url: https://doc.rust-lang.org/std/collections/struct.HashMap.html
  title: Rust standard library, struct HashMap
  locator: keys
- url: https://en.cppreference.com/w/c/algorithm/qsort
  title: cppreference.com, qsort (C)
  locator: Description
---

## Task

Group a list of words by their length. The function `group_by_length(words)` returns one pair `(length, words)` for each length that occurs. The pairs are in ascending order of the length. The words of a pair are in input order. For example, `group_by_length(["apple", "fig", "kiwi", "pear"])` is `[(3, ["fig"]), (4, ["kiwi", "pear"]), (5, ["apple"])]`.

Each word has 1 or more ASCII letters, so its length is the same in bytes, code units and code points. The list can be empty. Then the result is an empty list.

## Solutions

| Language | Group by the length |
|---|---|
| Python | a `defaultdict(list)`, then `sorted` of its items |
| JavaScript, TypeScript | `Map.groupBy`, then a sort with a number comparison |
| Java | `Collectors.groupingBy` into a `TreeMap` |
| C# | `GroupBy`, then `OrderBy` of the key |
| Go | a `map[int][]string`, then a loop over the sorted keys |
| Rust | a `BTreeMap` with `entry(..).or_default().push(..)` |
| C | a stable insertion sort by the length, then one group for each run of equal lengths |
| C++ | a `std::map<std::size_t, std::vector<std::string>>` |
| Bash | an associative array of strings, then `sort -n` |

- **The key.** Each solution calculates the length of a word once for each word, and uses it as the key of a map. A `defaultdict` gives an empty list for a new key: "If default_factory is not None, it is called without arguments to provide a default value for the given key"[^1].
- **The order of the groups.** A map of the language gives the keys in key order, or the solution sorts the keys. A `TreeMap`, a `BTreeMap` and a `std::map` keep the keys in order. A `BTreeMap` "stores its entries in key order"[^2], and `std::map` "is a sorted associative container"[^3].
- **The order of the words.** Each solution adds the words to their group in input order. For `GroupBy`, "Elements in a grouping are yielded in the order that the elements that produced them appear in source."[^4]
- **C.** The function writes the words to `sorted` in ascending order of the length. Each group is a start index and a count in `sorted`, so the function does not copy the words.
- **Bash.** The function prints one line for each group: the length, then the words, separated by spaces. `${#w}` "Substitutes the length in characters of the value of parameter"[^5].

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: an empty list gives no groups.
- `single`: one word gives one group with that word.
- `same-length`: `["cat", "dog", "cow"]` gives one group, with the words in input order.
- `order`: `["ccc", "a", "bb"]` gives the groups 1, 2 and 3, not the order of the first word of each group.
- `duplicates`: `["ab", "ab", "c"]` keeps the two words `"ab"` in the group 2.
- `long`: the lengths 1, 9 and 10 are in ascending order as numbers. As strings, "10" is before "9".

## Pitfalls

- **Python itertools.groupby groups only adjacent items.** "It generates a break or new group every time the value of the key function changes"[^6]. In the example, the two words of length 1 are in two groups. "Generally, the iterable needs to already be sorted on the same key function."[^6] A `defaultdict` keeps the keys in the order of their first word, so sort its items:

```python run id=python-groupby-adjacent
from collections import defaultdict
from itertools import groupby

words = ["a", "bb", "c"]
print([(k, list(g)) for k, g in groupby(words, len)])
print([(k, list(g)) for k, g in groupby(sorted(words, key=len), len)])
groups = defaultdict(list)
for w in ["apple", "fig", "kiwi", "pear"]:
    groups[len(w)].append(w)
print(dict(groups))
print(sorted(groups.items()))
```

```output for=python-groupby-adjacent
[(1, ['a']), (2, ['bb']), (1, ['c'])]
[(1, ['a', 'c']), (2, ['bb'])]
{5: ['apple'], 3: ['fig'], 4: ['kiwi', 'pear']}
[(3, ['fig']), (4, ['kiwi', 'pear']), (5, ['apple'])]
```

- **JavaScript sort compares strings.** `Map.groupBy` returns "A Map object with keys for each group"[^7], in the order of the first word of each group. But "The default sort order is ascending, built upon converting the elements into strings"[^8]. In the example, the length 10 is before 9. Give a comparison function:

```javascript run id=javascript-sort-strings
const words = ["abcdefghij", "a", "abcdefghi"];
const groups = Map.groupBy(words, (w) => w.length);
console.log([...groups].sort().map((p) => p[0]));
console.log([...groups].sort((p, q) => p[0] - q[0]).map((p) => p[0]));
```

```output for=javascript-sort-strings
[ 1, 10, 9 ]
[ 1, 9, 10 ]
```

- **Java groupingBy gives no order of the keys.** For `groupingBy(classifier)`, "There are no guarantees on the type, mutability, serializability, or thread-safety of the Map or List objects returned."[^9] Give `TreeMap::new` as the map factory, and the keys are in ascending order:

```java run id=java-groupingby-treemap
import java.util.List;
import java.util.TreeMap;
import java.util.stream.Collectors;

public class Main {
  public static void main(String[] args) {
    var words = List.of("apple", "fig", "kiwi", "pear", "plum", "date");
    TreeMap<Integer, List<String>> groups = words.stream()
        .collect(Collectors.groupingBy(String::length, TreeMap::new, Collectors.toList()));
    System.out.println(groups);
  }
}
```

```output for=java-groupingby-treemap
{3=[fig], 4=[kiwi, pear, plum, date], 5=[apple]}
```

- **C# GroupBy gives the groups in the order of their first item.** "The IGrouping<TKey,TElement> objects are yielded in an order based on the order of the elements in source that produced the first key of each IGrouping<TKey,TElement>."[^4] In the example, the first line is `5 3 4`. Add `OrderBy`:

```csharp run id=csharp-groupby-order
using System;
using System.Linq;

var words = new[] { "apple", "fig", "kiwi", "pear" };
Console.WriteLine(string.Join(" ", words.GroupBy(w => w.Length).Select(g => g.Key)));
Console.WriteLine(string.Join(" ", words.GroupBy(w => w.Length).OrderBy(g => g.Key).Select(g => g.Key)));
```

```output for=csharp-groupby-order
5 3 4
3 4 5
```

- **Go an append to a copy of the slice does not change the map.** `append` "returns the resulting slice"[^10]. In the first loop, `g` is a copy of the slice in the map, and the map stays empty. Store the result of `append` in the map. Also, "The iteration order over maps is not specified and is not guaranteed to be the same from one iteration to the next."[^10] Thus the solution sorts the keys:

```go run id=go-append-map
package main

import "fmt"

func main() {
	groups := make(map[int][]string)
	for _, w := range []string{"fig", "kiwi", "pear"} {
		g := groups[len(w)]
		g = append(g, w)
		_ = g
	}
	fmt.Println(groups, len(groups))
	for _, w := range []string{"fig", "kiwi", "pear"} {
		groups[len(w)] = append(groups[len(w)], w)
	}
	fmt.Println(groups, len(groups))
}
```

```output for=go-append-map
map[] 0
map[3:[fig] 4:[kiwi pear]] 2
```

- **Rust HashMap gives the keys in arbitrary order.** `keys` is "An iterator visiting all keys in arbitrary order."[^11] Use a `BTreeMap`, which "stores its entries in key order"[^2]:

```rust run id=rust-btreemap-order
use std::collections::BTreeMap;

fn main() {
    let words = ["apple", "fig", "kiwi", "pear"];
    let mut groups: BTreeMap<usize, Vec<&str>> = BTreeMap::new();
    for w in words {
        groups.entry(w.len()).or_default().push(w);
    }
    println!("{groups:?}");
}
```

```output for=rust-btreemap-order
{3: ["fig"], 4: ["kiwi", "pear"], 5: ["apple"]}
```

- **C qsort is not stable.** "If comp indicates two elements as equivalent, their order in the resulting sorted array is unspecified."[^12] Thus `qsort` by the length can change the order of the words in a group. The solution uses an insertion sort, which moves a word only past longer words:

```c run id=c-stable-sort
#include <stdio.h>
#include <string.h>

struct word_group {
    size_t len;
    size_t start;
    size_t count;
};

static size_t group_by_length(const char *const *words, size_t n, const char **sorted, struct word_group *out) {
    // An insertion sort is stable: it moves a word only past longer words.
    // qsort does not keep the order of equal items.
    for (size_t i = 0; i < n; i++) {
        size_t j = i;
        size_t len = strlen(words[i]);
        while (j > 0 && strlen(sorted[j - 1]) > len) {
            sorted[j] = sorted[j - 1];
            j--;
        }
        sorted[j] = words[i];
    }
    size_t k = 0;
    for (size_t i = 0; i < n; i++) {
        size_t len = strlen(sorted[i]);
        if (k > 0 && out[k - 1].len == len) {
            out[k - 1].count++;
        } else {
            out[k].len = len;
            out[k].start = i;
            out[k].count = 1;
            k++;
        }
    }
    return k;
}

int main(void) {
    const char *words[] = {"apple", "fig", "kiwi", "pear"};
    const char *sorted[4];
    struct word_group g[4];
    size_t k = group_by_length(words, 4, sorted, g);
    for (size_t i = 0; i < k; i++) {
        printf("%zu:", g[i].len);
        for (size_t j = g[i].start; j < g[i].start + g[i].count; j++) {
            printf(" %s", sorted[j]);
        }
        printf("\n");
    }
    return 0;
}
```

```output for=c-stable-sort
3: fig
4: kiwi pear
5: apple
```

- **C++ std::map sorts the keys.** "Keys are sorted by using the comparison function Compare."[^3] A loop over the map gives the groups in ascending order of the length:

```cpp run id=cpp-map-order
#include <iostream>
#include <map>
#include <string>
#include <vector>

int main() {
    std::map<std::size_t, std::vector<std::string>> groups;
    for (std::string w : {"apple", "fig", "kiwi", "pear"}) {
        groups[w.size()].push_back(w);
    }
    for (const auto &[len, ws] : groups) {
        std::cout << len << ':';
        for (const auto &w : ws) {
            std::cout << ' ' << w;
        }
        std::cout << '\n';
    }
}
```

```output for=cpp-map-order
3: fig
4: kiwi pear
5: apple
```

- **Bash an associative array needs declare -A.** "Associative arrays are created using declare -A name"[^5]. In a function, `local -A` makes a local associative array, because for `local`, "The option can be any of the options accepted by declare."[^5] Then `+=` adds to the string of a key: "When applied to a string-valued variable, value is expanded and appended to the variable’s value."[^5] The last line of the example is `${#w}` for `abc`:

```bash run id=bash-assoc-group
group_by_length() {
  local -A groups=()
  local w
  for w in "$@"; do
    groups[${#w}]+=" $w"
  done
  # sort -n puts the lines in ascending order of the length.
  for w in "${!groups[@]}"; do
    printf '%s%s\n' "$w" "${groups[$w]}"
  done | sort -n
}
group_by_length apple fig kiwi pear plum date
w=abc; echo "${#w}"
```

```output for=bash-assoc-group
3 fig
4 kiwi pear plum date
5 apple
3
```

## Related tasks

- [[count-the-items-of-each-kind]]: count the items for each key, not list them.
- [[partition-a-list]]: split a list into two groups by a test.
- [[iterate-in-chunks]]: split a list into groups of a fixed size, in input order.
- [[sort-with-a-custom-key]]: a sort by the length puts the words of each length together.

## References

