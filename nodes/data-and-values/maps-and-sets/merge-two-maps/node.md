---
tag: 0SKGZV
kind: task
slug: merge-two-maps
name: Merge two maps
aliases: [combine two dictionaries, merge dictionaries, update a map from another map, union of two maps]
area: data-and-values/maps-and-sets
level: intro
summary: Make a new map with the entries of two maps, where the value of the second map wins for a key in both, and keep the two maps unchanged.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#mapping-types-dict
  title: The Python Standard Library, Built-in Types
  locator: Mapping Types, dict; d | other; update
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Spread_syntax
  title: MDN Web Docs, Spread syntax (...)
  locator: Spread in object literals; Copying an array
- url: https://pkg.go.dev/maps
  title: Go standard library, package maps
  locator: func Copy
- url: https://doc.rust-lang.org/std/collections/struct.HashMap.html
  title: Rust standard library, struct HashMap
  locator: impl Extend<(K, V)>
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Collectors.html
  title: Java SE 25 API, Class Collectors
  locator: toMap(Function, Function)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.todictionary?view=net-10.0
  title: .NET API reference, Enumerable.ToDictionary method
  locator: Exceptions
- url: https://go.dev/blog/maps
  title: The Go Blog, Go maps in action
  locator: Declaration and initialization
- url: https://doc.rust-lang.org/std/collections/hash_map/enum.Entry.html
  title: Rust standard library, enum Entry
  locator: or_insert
- url: https://en.cppreference.com/w/cpp/container/map/merge
  title: cppreference.com, std::map::merge
  locator: Description
- url: https://en.cppreference.com/w/c/string/byte/strcmp
  title: cppreference.com, strcmp (C)
  locator: Description
- url: https://tiswww.case.edu/php/chet/bash/bashref.html
  title: Bash Reference Manual
  locator: 6.7 Arrays
---

## Task

Merge two maps into a new map. The function `merge_maps(base, override)` returns the entries of both maps. For a key that is in both maps, the value of `override` wins. The function does not change `base` or `override`. It returns the entries as pairs `(key, value)`, in ascending order of the key by its bytes. For example, `merge_maps({"apple": 3, "pear": 5}, {"pear": 6, "fig": 2})` is `[("apple", 3), ("fig", 2), ("pear", 6)]`.

Each key has 1 or more ASCII letters. Each value fits in a signed 32-bit integer. Either map can be empty.

## Solutions

| Language | Copy the base map | Add the override map |
|---|---|---|
| Python | `base \| override` makes a new dict | (one operator) |
| JavaScript, TypeScript | `new Map([...base, ...override])` | (one constructor) |
| Java | `new TreeMap<>(base)` | `merged.putAll(override)` |
| C# | `new Dictionary<string, int>(baseMap)` | `merged[key] = value` for each entry |
| Go | `maps.Clone(base)` | `maps.Copy(merged, override)` |
| Rust | `base.clone()` | `merged.extend(...)` |
| C | (not needed) | one pass over both sorted arrays |
| C++ | `std::map<std::string, int> merged(base.begin(), base.end())` | `merged.insert_or_assign(key, value)` |
| Bash | `local -A merged=()` | `merged[key]=value` for each pair, base first |

- **Which value wins.** Each solution copies the base map, then puts each entry of the override map in the copy with an operation that replaces a value. In Python, `base | override` gives "a new dictionary with the merged keys and values of d and other"[^1], and "The values of other take priority when d and other share keys."[^1] In JavaScript, for an object spread, "the property takes the last value assigned"[^2]. In Go, for `maps.Copy`, "When a key in src is already present in dst, the value in dst will be overwritten by the value associated with the key in src."[^3] Rust `extend` "Inserts all new key-values from the iterator and replaces values with existing keys with new values returned from the iterator."[^4]
- **The inputs do not change.** A copy keeps the base map as it is. A solution that puts the entries of `override` straight into `base` is faster, but it changes the map of the caller.
- **The order.** The Java, C++ and C solutions build a sorted map. The others sort the entries at the end, as in [[iterate-over-a-map]].
- **C.** A map is an array of entries in key order, as in [[create-a-map]]. One pass over both arrays, like the merge step of a merge sort, gives the result in key order. For a key in both arrays, the pass takes the entry of `override` and skips the entry of `base`.
- **Bash.** The function takes the base pairs, then `--`, then the override pairs, as `KEY=VALUE` arguments. A later assignment to a key replaces the earlier value, so the override pairs win.
- **A deep merge.** When the values are maps too, a merge replaces the inner map of the base with the inner map of the override. A deep merge merges the two inner maps, and so on down. The standard libraries of these languages have no deep merge. Also, a copy of a map copies one level: in JavaScript, "no native operation in JavaScript does a deep clone."[^2]

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty-base`, `empty-override` and `both-empty`: an empty map adds nothing.
- `same-keys`: when the two maps have the same keys, the result has the values of `override`.
- `zero-wins`: the override value 0 wins over the base value 5. A test such as `override.get(key) or base[key]` in Python takes 5, because 0 is false.
- `case`: `a` and `A` are two different keys, so both stay.

## Pitfalls

- **Python update changes the dict and returns None.** `update` does this: "Update the dictionary with the key/value pairs from mapping or iterable and kwargs, overwriting existing keys. Return None."[^1] Thus `merged = base.update(override)` gives `None` and changes `base`. Use `base | override` for a new dict:

```python run id=python-update-none
base = {"apple": 3, "pear": 5}
over = {"pear": 6, "fig": 2}
merged = base.update(over)
print(merged, base)
base = {"apple": 3, "pear": 5}
print(base | over, base)
```

```output for=python-update-none
None {'apple': 3, 'pear': 6, 'fig': 2}
{'apple': 3, 'pear': 6, 'fig': 2} {'apple': 3, 'pear': 5}
```

- **JavaScript object spread of a Map gives an empty object, and a spread is shallow.** An object spread copies the own properties of a value, and a `Map` keeps its entries outside its properties. Spread the entries into a new `Map`. Also, "Spread syntax effectively goes one level deep while copying an array."[^2] The same is true for objects, so the copy `b` shares the inner object with `a`:

```javascript run id=javascript-spread-map
const base = new Map([["apple", 3]]);
console.log({ ...base });
const over = new Map([["pear", 6]]);
console.log(new Map([...base, ...over]));
const a = { size: { n: 1 } };
const b = { ...a };
b.size.n = 2;
console.log(a.size.n);
```

```output for=javascript-spread-map
{}
Map(2) { 'apple' => 3, 'pear' => 6 }
2
```

- **Java Collectors.toMap throws on a key in both maps.** "If the mapped keys contain duplicates (according to Object.equals(Object)), an IllegalStateException is thrown when the collection operation is performed."[^5] Give a merge function as the third argument. `(a, b) -> b` takes the later value:

```java run id=java-tomap-duplicate
import java.util.Map;
import java.util.TreeMap;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {
  public static void main(String[] args) {
    Map<String, Integer> base = Map.of("apple", 3, "pear", 5);
    Map<String, Integer> over = Map.of("pear", 6);
    try {
      Stream.concat(base.entrySet().stream(), over.entrySet().stream())
          .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue));
    } catch (IllegalStateException e) {
      System.out.println(e.getClass().getSimpleName());
    }
    Map<String, Integer> merged = Stream.concat(base.entrySet().stream(), over.entrySet().stream())
        .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue, (a, b) -> b, TreeMap::new));
    System.out.println(merged);
  }
}
```

```output for=java-tomap-duplicate
IllegalStateException
{apple=3, pear=6}
```

- **C# Concat then ToDictionary throws on a key in both maps.** `ToDictionary` throws an `ArgumentException` when the "keySelector produces duplicate keys for two elements."[^6] Copy the base dictionary, then set each entry of the override with the indexer:

```csharp run id=csharp-todictionary-duplicate
using System;
using System.Collections.Generic;
using System.Linq;

var baseMap = new Dictionary<string, int> { ["apple"] = 3, ["pear"] = 5 };
var over = new Dictionary<string, int> { ["pear"] = 6 };
try
{
    _ = baseMap.Concat(over).ToDictionary(p => p.Key, p => p.Value);
}
catch (ArgumentException e)
{
    Console.WriteLine(e.GetType().Name);
}
var merged = new Dictionary<string, int>(baseMap);
foreach (var (key, value) in over)
{
    merged[key] = value;
}
Console.WriteLine(string.Join(" ", merged.OrderBy(p => p.Key, StringComparer.Ordinal).Select(p => p.Key + "=" + p.Value)));
```

```output for=csharp-todictionary-duplicate
ArgumentException
apple=3 pear=6
```

- **Go an assignment of a map does not copy it.** "Map types are reference types, like pointers or slices"[^7]. After `merged := base`, both names refer to one map, so `maps.Copy(merged, ...)` also changes `base`. Copy the map with `maps.Clone` first:

```go run id=go-map-reference
package main

import (
	"fmt"
	"maps"
)

func main() {
	base := map[string]int{"apple": 3, "pear": 5}
	merged := base
	maps.Copy(merged, map[string]int{"pear": 6})
	fmt.Println(base)
	base = map[string]int{"apple": 3, "pear": 5}
	merged = maps.Clone(base)
	maps.Copy(merged, map[string]int{"pear": 6})
	fmt.Println(base, merged)
}
```

```output for=go-map-reference
map[apple:3 pear:6]
map[apple:3 pear:5] map[apple:3 pear:6]
```

- **Rust extend lets the new value win.** `extend` replaces the value of a key that is there[^4]. To keep the value of the base, use the entry API: `or_insert` "Ensures a value is in the entry by inserting the default if empty"[^8], so it does not touch a key that is there:

```rust run id=rust-extend-or-insert
use std::collections::{BTreeMap, HashMap};

fn main() {
    let base = HashMap::from([("apple", 3), ("pear", 5)]);
    let over = HashMap::from([("pear", 6), ("fig", 2)]);
    let mut a = base.clone();
    a.extend(over.clone());
    println!("{:?}", a.into_iter().collect::<BTreeMap<_, _>>());
    let mut b = base.clone();
    for (k, v) in over {
        b.entry(k).or_insert(v);
    }
    println!("{:?}", b.into_iter().collect::<BTreeMap<_, _>>());
}
```

```output for=rust-extend-or-insert
{"apple": 3, "fig": 2, "pear": 6}
{"apple": 3, "fig": 2, "pear": 5}
```

- **C++ merge keeps the old value and leaves the other in the source.** `std::map::merge` tries to take "each element in source and insert it into *this"[^9]. "If there is an element in *this with key equivalent to the key of an element from source, then that element is not extracted from source."[^9] `insert` also keeps the value that is there. Use `insert_or_assign` to let the new value win:

```cpp run id=cpp-map-merge
#include <iostream>
#include <map>
#include <string>

static void print(const std::map<std::string, int> &m) {
    const char *sep = "";
    for (const auto &[key, value] : m) {
        std::cout << sep << key << '=' << value;
        sep = " ";
    }
    std::cout << '\n';
}

int main() {
    std::map<std::string, int> base{{"apple", 3}, {"pear", 5}};
    std::map<std::string, int> over{{"pear", 6}, {"fig", 2}};
    base.merge(over);
    print(base);
    print(over);
}
```

```output for=cpp-map-merge
apple=3 fig=2 pear=5
pear=6
```

- **C == compares the addresses of two keys, not the text.** The merge must find equal keys, and `strcmp` "Compares two null-terminated byte strings lexicographically."[^10] Two strings with the same text in two arrays have two addresses, so `==` gives 0:

```c run id=c-strcmp-keys
#include <stdio.h>
#include <string.h>

int main(void) {
    char a[] = "pear";
    char b[] = "pear";
    const char *x = a;
    const char *y = b;
    printf("%d %d\n", x == y, strcmp(x, y) == 0);
    return 0;
}
```

```output for=c-strcmp-keys
0 1
```

- **Bash a copy of the values loses the keys.** `${name[@]}` gives the values, and the keys come from `${!name[@]}`: "It is possible to obtain the keys (indices) of an array as well as the values."[^11] A copy into a new array gets the values with the indices 0, 1 and so on. Copy each key with its value:

```bash run id=bash-copy-assoc
f() {
  local -A base=([apple]=3)
  local -a copy=("${base[@]}")
  echo "${!copy[*]} ${copy[0]}"
  local -A merged=()
  local key
  for key in "${!base[@]}"; do
    merged[$key]=${base[$key]}
  done
  echo "${!merged[*]} ${merged[apple]}"
}
f
```

```output for=bash-copy-assoc
0 3
apple 3
```

## Related tasks

- [[create-a-map]]: add and replace entries one at a time.
- [[iterate-over-a-map]]: walk the entries of the merged map.
- [[use-a-map-with-a-default-value]]: read a key that can be in one map or none.
- [[do-set-operations]]: the union of two sets is the merge of two maps without values.
- [[concatenate-two-lists]]: join two lists, where no item replaces another.

## References
