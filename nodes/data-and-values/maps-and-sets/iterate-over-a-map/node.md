---
tag: 1JP4KA
kind: task
slug: iterate-over-a-map
name: Iterate over a map
aliases: [loop over a dictionary, iterate over map entries, walk the keys and values of a map, for each entry in a map]
area: data-and-values/maps-and-sets
level: intro
summary: Walk the entries of a map in key order to print one line for each entry, then walk the values to add them up.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#dictionary-view-objects
  title: The Python Standard Library, Built-in Types
  locator: Dictionary view objects
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Map
  title: MDN Web Docs, Map
  locator: Key order
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements with range clause
- url: https://doc.rust-lang.org/std/collections/struct.HashMap.html
  title: Rust standard library, struct HashMap
  locator: iter; retain
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.dictionary-2?view=net-10.0
  title: .NET API reference, Dictionary<TKey,TValue> class
  locator: Remarks
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/for...in
  title: MDN Web Docs, for...in
  locator: Description, Iteration order
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Map.html
  title: Java SE 25 API, Interface Map
  locator: keySet()
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/ConcurrentModificationException.html
  title: Java SE 25 API, Class ConcurrentModificationException
  locator: Description
- url: https://en.cppreference.com/w/cpp/container/unordered_map/erase
  title: cppreference.com, std::unordered_map::erase
  locator: Description; Return value
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/tdelete.html
  title: POSIX.1-2024, tdelete, tfind, tsearch, twalk
  locator: DESCRIPTION; APPLICATION USAGE
- url: https://tiswww.case.edu/php/chet/bash/bashref.html
  title: Bash Reference Manual
  locator: 6.7 Arrays
---

## Task

Walk the entries of a map. The function `score_lines(scores)` takes a map from names to scores. It returns one line `key=value` for each entry, in ascending order of the key by its bytes, then one line `total=N`, where `N` is the sum of the values. For example, `score_lines({"bob": 7, "amy": 9, "cal": 4})` is `["amy=9", "bob=7", "cal=4", "total=20"]`.

Each key has 1 or more ASCII letters. Each value is from -1000000 to 1000000, and the map has at most 1000 entries, so the total fits in a signed 32-bit integer. The map can be empty. Then the result is `["total=0"]`.

## Solutions

| Language | Entries in key order | Values |
|---|---|---|
| Python | `sorted(scores.items())` | `sum(scores.values())` |
| JavaScript, TypeScript | `[...scores.keys()].sort()`, then `scores.get(key)` | `for (const value of scores.values())` |
| Java | `new TreeMap<>(scores).entrySet()` | `for (int value : scores.values())` |
| C# | `scores.OrderBy(p => p.Key, StringComparer.Ordinal)` | `foreach (var value in scores.Values)` |
| Go | `slices.Sorted(maps.Keys(scores))`, then `scores[k]` | `for _, v := range scores` |
| Rust | `scores.iter().collect()`, then `sort()` | `scores.values().sum()` |
| C | `qsort` of an array of pointers to the entries | a loop over the sorted pointers |
| C++ | a `std::map` copy of the `std::unordered_map` | `for (const auto &[key, value] : scores)` |
| Bash | `"${!map[@]}"`, then `LC_ALL=C sort` | `"${map[@]}"` |

- **Three ways to walk.** A map gives its keys, its values and its pairs. In Python, `items()`, `keys()` and `values()` give views, and "Keys and values are iterated over in insertion order."[^1] A JavaScript `Map` "iterates entries, keys, and values in the order of entry insertion."[^2] Use the pairs when the loop needs the key and the value, so it does not look up each key again.
- **The order.** Insertion order is not key order, and a hash map gives no order at all. In Go, "The iteration order over maps is not specified and is not guaranteed to be the same from one iteration to the next."[^3] A Rust `HashMap` gives "An iterator visiting all key-value pairs in arbitrary order."[^4] For a .NET `Dictionary`, "The order in which the items are returned is undefined."[^5] Thus each solution sorts the keys, or copies the entries to a sorted map, before it makes the lines.
- **The total.** The sum does not depend on the order, so each solution walks the values in the order of the map.
- **C.** The input is an array of entries with different keys. The function sorts an array of pointers to the entries, so the input does not change, then writes the lines to a buffer with `snprintf`. It returns -1 if the text does not fit.
- **Bash.** The function takes the entries as `KEY=VALUE` arguments and prints the lines. `"${!map[@]}"` gives the keys, and `"${map[@]}"` gives the values.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: an empty map gives only `total=0`.
- `case`: the keys `A`, `B`, `a` and `b` are in byte order, so the upper case keys come first.
- `negative`: the values -5 and 5 give `a=-5`, `b=5` and `total=0`.
- `prefix`: the key `a` comes before `ab`, and `ab` comes before `abc`.
- `reverse`: ten keys inserted from `j` down to `a` come out from `a` to `j`.

## Pitfalls

- **Python a change of size during a loop raises RuntimeError.** "Iterating views while adding or deleting entries in the dictionary may raise a RuntimeError or fail to iterate over all entries."[^1] In the example, the first loop deletes `bob` and then fails. Loop over `list(scores)`, a copy of the keys:

```python run id=python-dict-changed-size
scores = {"amy": 9, "bob": 3, "cal": 4}
try:
    for name in scores:
        if scores[name] < 5:
            del scores[name]
except RuntimeError as e:
    print(type(e).__name__)
print(scores)
for name in list(scores):
    if scores[name] < 5:
        del scores[name]
print(scores)
```

```output for=python-dict-changed-size
RuntimeError
{'amy': 9, 'cal': 4}
{'amy': 9}
```

- **JavaScript an object puts integer keys first.** For an object, "all non-negative integer keys (those that can be array indices) will be traversed first in ascending order by value, then other string keys in ascending chronological order of property creation."[^6] A `Map` keeps the insertion order of all keys. Neither order is key order, so sort the keys:

```javascript run id=javascript-object-key-order
const object = { b: 1, 10: 2, a: 3, 2: 4 };
console.log(Object.keys(object));
const map = new Map([["b", 1], ["10", 2], ["a", 3], ["2", 4]]);
console.log([...map.keys()]);
console.log([...map.keys()].sort());
```

```output for=javascript-object-key-order
[ '2', '10', 'b', 'a' ]
[ 'b', '10', 'a', '2' ]
[ '10', '2', 'a', 'b' ]
```

- **Java a change during a for-each loop throws ConcurrentModificationException.** For `keySet()`, "If the map is modified while an iteration over the set is in progress (except through the iterator's own remove operation), the results of the iteration are undefined."[^7] A `HashMap` iterator is fail-fast: "if a thread modifies a collection directly while it is iterating over the collection with a fail-fast iterator, the iterator will throw this exception."[^8] Use `removeIf` on a view, or the remove method of the iterator:

```java run id=java-concurrent-modification
import java.util.ConcurrentModificationException;
import java.util.HashMap;
import java.util.Map;
import java.util.TreeMap;

public class Main {
  public static void main(String[] args) {
    Map<String, Integer> scores = new HashMap<>(Map.of("a", 1, "b", 7, "c", 3));
    try {
      for (String key : scores.keySet()) {
        if (scores.get(key) < 5) {
          scores.remove(key);
        }
      }
    } catch (ConcurrentModificationException e) {
      System.out.println(e.getClass().getSimpleName());
    }
    scores.values().removeIf(v -> v < 5);
    System.out.println(new TreeMap<>(scores));
  }
}
```

```output for=java-concurrent-modification
ConcurrentModificationException
{b=7}
```

- **C# an Add during foreach throws InvalidOperationException.** "The foreach statement is a wrapper around the enumerator, which allows only reading from the collection, not writing to it."[^5] In the example, the first `Add` works, and the next step of the loop throws. Loop over a copy of the keys, made with `ToList`:

```csharp run id=csharp-modify-during-foreach
using System;
using System.Collections.Generic;
using System.Linq;

var scores = new Dictionary<string, int> { ["amy"] = 9, ["bob"] = 3 };
try
{
    foreach (var key in scores.Keys)
    {
        scores.Add("cal", 4);
    }
}
catch (InvalidOperationException e)
{
    Console.WriteLine(e.GetType().Name);
}
foreach (var key in scores.Keys.ToList())
{
    if (scores[key] < 5)
    {
        scores.Remove(key);
    }
}
Console.WriteLine(string.Join(" ", scores.Keys.Order(StringComparer.Ordinal)));
```

```output for=csharp-modify-during-foreach
InvalidOperationException
amy
```

- **Go a new entry during a range loop can be skipped or not.** A delete in the loop is safe: "If a map entry that has not yet been reached is removed during iteration, the corresponding iteration value will not be produced."[^3] But "If a map entry is created during iteration, that entry may be produced during the iteration or may be skipped."[^3] Thus do not add entries in the loop. The example deletes in the loop, then sorts the keys for a fixed order:

```go run id=go-delete-during-range
package main

import (
	"fmt"
	"maps"
	"slices"
)

func main() {
	scores := map[string]int{"amy": 9, "bob": 3, "cal": 4, "dan": 8}
	for k, v := range scores {
		if v < 5 {
			delete(scores, k)
		}
	}
	fmt.Println(slices.Sorted(maps.Keys(scores)))
}
```

```output for=go-delete-during-range
[amy dan]
```

- **Rust a loop over a HashMap cannot change it.** The loop borrows the map, so the body cannot call `remove` on it. `retain` deletes entries as it walks them: it will "remove all pairs (k, v) for which f(&k, &mut v) returns false."[^4] The order is still arbitrary, so sort the keys:

```rust run id=rust-retain-sort
use std::collections::HashMap;

fn main() {
    let mut scores = HashMap::from([("amy", 9), ("bob", 3), ("cal", 4), ("dan", 8)]);
    scores.retain(|_, v| *v >= 5);
    let mut keys: Vec<&str> = scores.keys().copied().collect();
    keys.sort();
    println!("{keys:?}");
}
```

```output for=rust-retain-sort
["amy", "dan"]
```

- **C++ erase makes the iterator invalid.** "References and iterators to the erased elements are invalidated."[^9] A `++it` after `erase(it)` uses an invalid iterator. Use the iterator that `erase` returns, or `std::erase_if`:

```cpp run id=cpp-erase-in-loop
#include <iostream>
#include <string>
#include <unordered_map>

int main() {
    std::unordered_map<std::string, int> scores{{"amy", 9}, {"bob", 3}, {"cal", 4}, {"dan", 8}};
    for (auto it = scores.begin(); it != scores.end();) {
        if (it->second < 5) {
            it = scores.erase(it);
        } else {
            ++it;
        }
    }
    std::cout << scores.size() << ' ' << scores.contains("amy") << scores.contains("bob") << '\n';
    std::erase_if(scores, [](const auto &p) { return p.first == "dan"; });
    std::cout << scores.size() << '\n';
}
```

```output for=cpp-erase-in-loop
2 10
1
```

- **C twalk calls postorder the visit between the children.** POSIX `twalk` visits each node of a `tsearch` tree up to three times. It uses "preorder, postorder, and endorder to refer respectively to visiting a node before any of its children, after its left child and before its right, and after both its children."[^10] Other texts call the middle visit inorder, "which could result in some confusion over the meaning of postorder."[^10] For the keys in order, print a node on `postorder` and on `leaf`:

```c run id=c-twalk-order
#define _XOPEN_SOURCE 700
#include <search.h>
#include <stdio.h>
#include <string.h>

static int cmp(const void *a, const void *b) {
    return strcmp(a, b);
}

static void first_visit(const void *node, VISIT which, int depth) {
    (void)depth;
    if (which == preorder || which == leaf) {
        printf("%s", *(char *const *)node);
    }
}

static void in_order(const void *node, VISIT which, int depth) {
    (void)depth;
    if (which == postorder || which == leaf) {
        printf("%s", *(char *const *)node);
    }
}

int main(void) {
    void *root = NULL;
    char *keys[] = {"b", "a", "c"};
    for (int i = 0; i < 3; i++) {
        tsearch(keys[i], &root, cmp);
    }
    twalk(root, first_visit);
    printf("\n");
    twalk(root, in_order);
    printf("\n");
    return 0;
}
```

```output for=c-twalk-order
bac
abc
```

- **Bash an unquoted list of keys splits a key with a space.** `${!map[@]}` and `${!map[*]}` "expand to the indices assigned in array variable name."[^11] Only in double quotes does `${name[@]}` give one word for each item: it "expands each element of name to a separate word."[^11] In the example, the key `new york` gives two words without the quotes:

```bash run id=bash-quote-keys
f() {
  local -A map=(["new york"]=8 [oslo]=2)
  local key n=0
  for key in ${!map[@]}; do
    n=$((n + 1))
  done
  echo "$n"
  n=0
  for key in "${!map[@]}"; do
    n=$((n + 1))
  done
  echo "$n"
}
f
```

```output for=bash-quote-keys
3
2
```

## Related tasks

- [[create-a-map]]: create the map that this task walks.
- [[merge-two-maps]]: combine the entries of two maps.
- [[sort-with-a-custom-key]]: sort the entries by the value, not the key.
- [[group-items-by-a-key]]: build a map from a list, then walk it in key order.
- [[iterate-over-the-characters-of-a-string]]: the same task for the characters of a string.

## References
