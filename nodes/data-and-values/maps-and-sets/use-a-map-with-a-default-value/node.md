---
tag: QX274K
kind: task
slug: use-a-map-with-a-default-value
name: Use a map with a default value
aliases: [get with a default, getOrDefault, dict get default, map lookup with fallback, defaultdict]
area: data-and-values/maps-and-sets
level: intro
summary: Read keys from a map and use a fallback value for a key that is not in the map, without adding the key and without mixing up a missing key with a stored 0.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#mapping-types-dict
  title: The Python Standard Library, Built-in Types
  locator: Mapping Types, dict; get
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Map.html
  title: Java SE 25 API, Interface Map
  locator: get; getOrDefault
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Index expressions
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.dictionary-2?view=net-10.0
  title: .NET API reference, Dictionary<TKey,TValue> class
  locator: Examples; Extension Methods, GetValueOrDefault
- url: https://doc.rust-lang.org/std/collections/hash_map/enum.Entry.html
  title: Rust standard library, enum Entry
  locator: or_insert; or_insert_with
- url: https://en.cppreference.com/w/c/algorithm/bsearch
  title: cppreference.com, bsearch, bsearch_s (C)
  locator: Return value
- url: https://docs.python.org/3/library/collections.html#collections.defaultdict
  title: The Python Standard Library, collections
  locator: defaultdict objects; __missing__
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Nullish_coalescing
  title: MDN Web Docs, Nullish coalescing operator (??)
  locator: Description
- url: https://en.cppreference.com/w/cpp/container/unordered_map/at
  title: cppreference.com, std::unordered_map::at
  locator: Description
- url: https://tiswww.case.edu/php/chet/bash/bashref.html
  title: Bash Reference Manual
  locator: 3.5.3 Shell Parameter Expansion
---

## Task

Read keys from a map, and use a fallback value for a key that is not in the map. The function `lookup(table, keys, fallback)` returns one value for each key, in the order of `keys`. The value is the value of the key in `table` if `table` has the key, else `fallback`. The function does not change `table`. For example, `lookup({"apple": 3, "pear": 5}, ["apple", "fig", "pear"], 0)` is `[3, 0, 5]`.

Each key has 1 or more ASCII letters. Each value and the fallback fit in a signed 32-bit integer. The map and the list of keys can be empty, and the list can have a key more than one time.

## Solutions

| Language | Read with a default | Count with a default |
|---|---|---|
| Python | `table.get(key, fallback)` | `counts[w] = counts.get(w, 0) + 1` |
| JavaScript, TypeScript | `table.get(key) ?? fallback` | `counts.set(w, (counts.get(w) ?? 0) + 1)` |
| Java | `table.getOrDefault(key, fallback)` | `counts.merge(w, 1, Integer::sum)` |
| C# | `table.GetValueOrDefault(key, fallback)` | `counts[w] = counts.GetValueOrDefault(w) + 1` |
| Go | `v, ok := table[key]`, then the fallback if `!ok` | `counts[w]++` |
| Rust | `table.get(key).copied().unwrap_or(fallback)` | `*counts.entry(w).or_insert(0) += 1` |
| C | `bsearch`, then the fallback for a null pointer | (no map type) |
| C++ | `find`, then the fallback for `end()` | `counts[w]++` |
| Bash | `${table[$key]-$fallback}` | `counts[$w]=$(( ${counts[$w]-0} + 1 ))` |

- **Read with a default.** Each solution uses an operation that tells a missing key from a key that is there, and that does not add the key. Python `get` does this: "Return the value for key if key is in the dictionary, else default."[^1] Java `getOrDefault` "Returns the value to which the specified key is mapped, or defaultValue if this map contains no mapping for the key."[^2] In Go, the special form `v, ok := a[x]` gives a second value, and "The value of ok is true if the key x is present in the map, and false otherwise."[^3] C# `GetValueOrDefault` "Tries to get the value associated with the specified key in the dictionary"[^4], and gives the fallback if that fails.
- **Count with a default.** To count, read the count with the default 0, add 1, and write the count back. Go and C++ do not need the default: a missing key in Go gives "the zero value for the element type"[^3], and C++ `operator[]` adds a missing key with the value 0. Python also has `collections.Counter` and `defaultdict(int)`. See [[count-the-items-of-each-kind]] and [[count-word-frequencies]].
- **Group with a default.** To group, the default is an empty list. Get the list of a key, or add an empty list for a missing key, then append to it. Rust `or_insert_with` "Ensures a value is in the entry by inserting the result of the default function if empty"[^5], and Python `defaultdict(list)` does the same. See [[group-items-by-a-key]].
- **A default that is added or not.** A read with a default does not change the map. The entry API in Rust, `computeIfAbsent` in Java, `defaultdict` in Python and `operator[]` in C++ add the key with the default. Use these to count and group, but not to read only.
- **C.** C has no map type. Here a map is an array of entries in key order, and `bsearch` finds a key. It returns a "Pointer to an element in the array that compares equal to *key, or null pointer if such element has not been found."[^6]

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty-table`: each key gets the fallback.
- `empty-keys`: the result is empty.
- `zero-value`: the key `a` has the value 0 and the fallback is 5. The result for `a` is 0. A test of the value for truth, such as `table.get(key) or fallback` in Python, gives 5.
- `repeat`: a missing key two times gets the fallback two times. A solution that adds the key with the fallback changes the map, and the test finds that change.
- `case`: `A` is not the key `a`, so it gets the fallback.

## Pitfalls

- **Python `or` replaces a stored 0, and `defaultdict` adds the key on a read.** `get(key) or fallback` gives the fallback for each false value, and 0 is false. For a `defaultdict`, a read of a missing key calls the factory, then "this value is inserted in the dictionary for the key, and returned."[^7] Thus a read only of a `defaultdict` makes it grow:

```python run id=python-or-defaultdict
from collections import defaultdict

stock = {"apple": 3, "pear": 0}
print(stock.get("pear") or 5, stock.get("pear", 5))
counts = defaultdict(int)
print(counts["fig"], len(counts))
```

```output for=python-or-defaultdict
5 0
0 1
```

- **JavaScript `||` replaces a stored 0.** The `??` operator "returns its right-hand side operand when its left-hand side operand is null or undefined"[^8]. But `||` "returns the right-hand side operand if the left operand is any falsy value, not only null or undefined."[^8] 0 is falsy, so use `??` for a default:

```javascript run id=javascript-or-nullish
const stock = new Map([["apple", 3], ["pear", 0]]);
console.log(stock.get("pear") || 5, stock.get("pear") ?? 5);
console.log(stock.get("fig") ?? 5);
```

```output for=javascript-or-nullish
5 0
5
```

- **Java `get` gives null, and an int cannot hold null.** `get` "Returns the value to which the specified key is mapped, or null if this map contains no mapping for the key."[^2] An assignment of the `Integer` null to an `int` throws `NullPointerException`. Use `getOrDefault`:

```java run id=java-get-null-unbox
import java.util.Map;

public class Main {
  public static void main(String[] args) {
    Map<String, Integer> stock = Map.of("apple", 3);
    try {
      int n = stock.get("fig");
      System.out.println(n);
    } catch (NullPointerException e) {
      System.out.println(e.getClass().getSimpleName());
    }
    System.out.println(stock.getOrDefault("fig", 0));
  }
}
```

```output for=java-get-null-unbox
NullPointerException
0
```

- **C# `counts[w]++` throws for a new word.** The indexer reads the old count first, and "a KeyNotFoundException is thrown when a requested key is not present"[^4]. Read the count with `GetValueOrDefault`:

```csharp run id=csharp-count-indexer
using System;
using System.Collections.Generic;

var counts = new Dictionary<string, int>();
try
{
    counts["fig"]++;
}
catch (KeyNotFoundException e)
{
    Console.WriteLine(e.GetType().Name);
}
foreach (var word in new[] { "fig", "pear", "fig" })
{
    counts[word] = counts.GetValueOrDefault(word) + 1;
}
Console.WriteLine("fig=" + counts["fig"] + " pear=" + counts["pear"]);
```

```output for=csharp-count-indexer
KeyNotFoundException
fig=2 pear=1
```

- **Go a missing key and a stored 0 look the same.** If the map "does not contain such an entry, a[x] is the zero value for the element type of M"[^3]. Thus a read of the stored 0 of `pear` and a read of the missing key `fig` both give 0. Use the second value to tell them apart:

```go run id=go-zero-value-ok
package main

import "fmt"

func main() {
	stock := map[string]int{"apple": 3, "pear": 0}
	fmt.Println(stock["pear"], stock["fig"])
	_, inPear := stock["pear"]
	_, inFig := stock["fig"]
	fmt.Println(inPear, inFig)
}
```

```output for=go-zero-value-ok
0 0
true false
```

- **Rust the argument of `or_insert` is made each time.** `or_insert` "Ensures a value is in the entry by inserting the default if empty"[^5], but Rust makes the argument before the call, also when the key is there. For a default that costs something to make, use `or_insert_with`, which calls its function only for a missing key:

```rust run id=rust-or-insert-eager
use std::collections::HashMap;

fn fallback() -> Vec<i32> {
    println!("made a default");
    Vec::new()
}

fn main() {
    let mut groups: HashMap<&str, Vec<i32>> = HashMap::new();
    groups.insert("odd", vec![1]);
    groups.entry("odd").or_insert(fallback()).push(3);
    groups.entry("odd").or_insert_with(fallback).push(5);
    println!("{:?}", groups["odd"]);
}
```

```output for=rust-or-insert-eager
made a default
[1, 3, 5]
```

- **C++ `at` throws for a missing key.** For `at`, "If no such element exists, an exception of type std::out_of_range is thrown."[^9] `operator[]` does not throw, but it adds the key, and it does not compile for a `const` map. Use `find` and compare the result with `end()`:

```cpp run id=cpp-at-throws
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

int main() {
    const std::unordered_map<std::string, int> stock{{"apple", 3}};
    try {
        std::cout << stock.at("fig") << '\n';
    } catch (const std::out_of_range &) {
        std::cout << "out_of_range\n";
    }
    auto it = stock.find("fig");
    std::cout << (it == stock.end() ? 0 : it->second) << '\n';
}
```

```output for=cpp-at-throws
out_of_range
0
```

- **C a function that returns the default loses the null pointer.** A helper that returns 0 for a missing key gives 0 for a stored 0 too. Keep the pointer from `bsearch`, or give the fallback to the helper as an argument:

```c run id=c-bsearch-zero
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct entry {
    const char *key;
    int value;
};

static const struct entry stock[] = {{"apple", 3}, {"pear", 0}};

static int cmp(const void *key, const void *elem) {
    const struct entry *e = elem;
    return strcmp(key, e->key);
}

static int get_or_zero(const char *key) {
    const struct entry *e = bsearch(key, stock, 2, sizeof stock[0], cmp);
    return e == NULL ? 0 : e->value;
}

int main(void) {
    printf("%d %d\n", get_or_zero("pear"), get_or_zero("fig"));
    const struct entry *pear = bsearch("pear", stock, 2, sizeof stock[0], cmp);
    const struct entry *fig = bsearch("fig", stock, 2, sizeof stock[0], cmp);
    printf("%d %d\n", pear != NULL, fig != NULL);
    return 0;
}
```

```output for=c-bsearch-zero
0 0
1 0
```

- **Bash `:-` also replaces an empty value.** "Omitting the colon results in a test only for a parameter that is unset."[^10] Thus `${table[$key]:-fallback}` gives the fallback for a key with an empty value, and `${table[$key]-fallback}` gives the empty value:

```bash run id=bash-colon-dash
declare -A note=([apple]=ripe [pear]=)
echo "[${note[pear]:-none}] [${note[pear]-none}] [${note[fig]-none}]"
```

```output for=bash-colon-dash
[none] [] [none]
```

## Related tasks

- [[create-a-map]]: add, replace and delete keys.
- [[merge-two-maps]]: a map of defaults merged with a map of settings gives each key a value.
- [[count-the-items-of-each-kind]]: count with the default 0.
- [[group-items-by-a-key]]: group with the default of an empty list.
- [[use-a-custom-key-in-a-map]]: read a map with keys of a user type.

## References
