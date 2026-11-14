---
tag: VJ920A
kind: task
slug: create-a-map
name: Create a map
aliases: [create a dictionary, make a hash map, build a map, add and remove map entries, dict literal]
area: data-and-values/maps-and-sets
level: intro
summary: Put key and value pairs in a new map, so a later pair replaces the value of its key, delete some keys, and list the entries in key order.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#mapping-types-dict
  title: The Python Standard Library, Built-in Types
  locator: Mapping Types, dict
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Composite literals; Map types; Deletion of map elements
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.dictionary-2.item?view=net-10.0
  title: .NET API reference, Dictionary<TKey,TValue>.Item[TKey] property
  locator: Remarks
- url: https://doc.rust-lang.org/std/collections/struct.HashMap.html
  title: Rust standard library, struct HashMap
  locator: insert; impl Index
- url: https://en.cppreference.com/w/cpp/container/unordered_map/insert_or_assign
  title: cppreference.com, std::unordered_map::insert_or_assign
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Map
  title: MDN Web Docs, Map
  locator: Setting object properties
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Map.html
  title: Java SE 25 API, Interface Map
  locator: Unmodifiable Maps
- url: https://en.cppreference.com/w/cpp/container/unordered_map/operator_at
  title: cppreference.com, std::unordered_map::operator[]
  locator: Description
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/hsearch.html
  title: POSIX.1-2024, hcreate, hdestroy, hsearch
  locator: DESCRIPTION
- url: https://tiswww.case.edu/php/chet/bash/bashref.html
  title: Bash Reference Manual
  locator: 6.7 Arrays; 6.5 Shell Arithmetic
---

## Task

Create a map, then change it. The function `build_map(pairs, remove)` puts each pair `(key, value)` of `pairs` in a new, empty map, in input order. A later pair with the same key replaces the value of that key. Then the function deletes each key of `remove` from the map. A key in `remove` that is not in the map does nothing. The function returns the entries of the map as pairs `(key, value)`, in ascending order of the key by its bytes. For example, `build_map([("a", 1), ("b", 2), ("a", 7), ("c", 3)], ["b"])` is `[("a", 7), ("c", 3)]`.

Each key has 1 or more ASCII letters. Each value fits in a signed 32-bit integer. Both lists can be empty, and both can repeat a key.

## Solutions

| Language | Create | Add or replace | Delete |
|---|---|---|---|
| Python | `{}` or `dict()` | `table[key] = value` | `table.pop(key, None)` |
| JavaScript, TypeScript | `new Map()` | `map.set(key, value)` | `map.delete(key)` |
| Java | `new HashMap<>()` | `map.put(key, value)` | `map.remove(key)` |
| C# | `new Dictionary<string, int>()` | `map[key] = value` | `map.Remove(key)` |
| Go | `make(map[string]int)` | `m[key] = value` | `delete(m, key)` |
| Rust | `HashMap::new()` | `map.insert(key, value)` | `map.remove(key)` |
| C | a sorted array of entries | a binary search, then a change or a `memmove` | a binary search, then a `memmove` |
| C++ | `std::unordered_map<std::string, int>` | `map.insert_or_assign(key, value)` | `map.erase(key)` |
| Bash | `local -A map=()` | `map[key]=value` | `unset 'map[key]'` |

- **Create.** Each solution starts with an empty map and adds the pairs one at a time. A literal is the other way to create a map when the entries are known. In Python, "Use a comma-separated list of key: value pairs within braces"[^1], for example `{'jack': 4098, 'sjoerd': 4127}`. In Go, "For map literals, each element must have a key."[^2] A Go literal with the same constant key two times does not compile: "It is an error to specify multiple elements with the same field selector or constant key value."[^2]
- **Add or replace.** One operation adds a new key and replaces the value of a key that is there. For the C# indexer, "If the key is not in the Dictionary<TKey,TValue>, the key and value are added to the dictionary."[^3] For Rust `insert`, "If the map did have this key present, the value is updated, and the old value is returned."[^4] For C++ `insert_or_assign`, "If the key does not exist, inserts the new value as if by insert"[^5].
- **Delete.** Each delete operation accepts a key that is not in the map. In Go, "If the map m is nil or the element m[k] does not exist, delete is a no-op."[^2] Python `pop` with a default does not raise an error: "If key is in the dictionary, remove it and return its value, else return default."[^1]
- **The order of the result.** A hash map does not keep its keys in order, so each solution sorts the entries by key at the end. The keys are ASCII, so a comparison of chars, code units or bytes gives the same order, and `B` comes before `a`.
- **C.** The C standard library has no map. The solution keeps the entries in an array in key order, finds a key with a binary search, and moves the entries after it with `memmove` to add or delete an entry.
- **Bash.** The function takes the pairs as `KEY=VALUE` arguments, then `--`, then the keys to delete. It prints one line `KEY VALUE` for each entry, and `LC_ALL=C sort` puts the lines in byte order.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: no pairs and no keys to delete give an empty map.
- `replace`: `[("k", 1), ("k", 2), ("k", 3)]` gives one entry, `("k", 3)`.
- `delete-absent`: a delete of `z`, which is not in the map, does nothing.
- `delete-all`: a delete of every key gives an empty map.
- `delete-twice`: a second delete of the same key does nothing.
- `zero-value`: the values 0 and -4 are values like any other, and stay in the map.
- `case`: the keys `b`, `B`, `a` and `A` are four different keys, in the order `A`, `B`, `a`, `b`.

## Pitfalls

- **Python a read or a del of a missing key raises KeyError.** For `d[key]`, the dict "Raises a KeyError if key is not in the map."[^1] `del d[key]` does the same. Use `get` to read and `pop(key, None)` to delete a key that can be missing:

```python run id=python-missing-key
table = {"apple": 3}
try:
    print(table["pear"])
except KeyError as e:
    print(type(e).__name__, e)
print(table.get("pear"), table.get("pear", 0))
print(table.pop("pear", None), table)
```

```output for=python-missing-key
KeyError 'pear'
None 0
None {'apple': 3}
```

- **JavaScript map[key] = value sets a property, not an entry.** "Setting Object properties works for Map objects as well, and can cause considerable confusion."[^6] "The value of 'bla' is not stored in the Map for queries."[^6] The size stays 0 and `has` gives false. Use `set` and `get`:

```javascript run id=javascript-map-property
const wrong = new Map();
wrong["apple"] = 3;
console.log(wrong.size, wrong.has("apple"));
const map = new Map();
map.set("apple", 3);
console.log(map.size, map.has("apple"), map.get("apple"));
```

```output for=javascript-map-property
0 false
1 true 3
```

- **Java Map.of gives a map that cannot change.** "Keys and values cannot be added, removed, or updated. Calling any mutator method on the Map will always cause UnsupportedOperationException to be thrown."[^7] Also, "Duplicate keys passed to a static factory method result in IllegalArgumentException."[^7] Copy the entries to a `HashMap` to change them:

```java run id=java-map-of
import java.util.HashMap;
import java.util.Map;

public class Main {
  public static void main(String[] args) {
    Map<String, Integer> fixed = Map.of("apple", 3);
    try {
      fixed.put("pear", 5);
    } catch (UnsupportedOperationException e) {
      System.out.println(e.getClass().getSimpleName());
    }
    try {
      Map.of("apple", 3, "apple", 4);
    } catch (IllegalArgumentException e) {
      System.out.println(e.getClass().getSimpleName());
    }
    Map<String, Integer> map = new HashMap<>(fixed);
    map.put("pear", 5);
    System.out.println(map.get("pear"));
  }
}
```

```output for=java-map-of
UnsupportedOperationException
IllegalArgumentException
5
```

- **C# a read of a missing key throws, and Add of a key that is there throws.** "If the specified key is not found, a get operation throws a KeyNotFoundException, and a set operation creates a new element with the specified key."[^3] "In contrast, the Add method does not modify existing elements."[^3] It throws an `ArgumentException`. Use `TryGetValue` to read and the indexer to add or replace:

```csharp run id=csharp-missing-key
using System;
using System.Collections.Generic;

var map = new Dictionary<string, int> { ["apple"] = 3 };
try
{
    Console.WriteLine(map["pear"]);
}
catch (KeyNotFoundException e)
{
    Console.WriteLine(e.GetType().Name);
}
try
{
    map.Add("apple", 4);
}
catch (ArgumentException e)
{
    Console.WriteLine(e.GetType().Name);
}
map["apple"] = 4;
Console.WriteLine(map.TryGetValue("pear", out var n) + " " + n + " " + map["apple"]);
```

```output for=csharp-missing-key
KeyNotFoundException
ArgumentException
False 0 4
```

- **Go an assignment to a nil map panics.** The zero value of a map type is nil. "A nil map is equivalent to an empty map except that no elements may be added."[^2] A read and a delete work, but an assignment panics. Create the map with `make` or a literal:

```go run id=go-nil-map
package main

import "fmt"

func main() {
	var m map[string]int
	fmt.Println(m["apple"], len(m), m == nil)
	delete(m, "apple")
	defer func() {
		fmt.Println("panic:", recover())
		ok := make(map[string]int)
		ok["apple"] = 3
		fmt.Println(ok)
	}()
	m["apple"] = 3
}
```

```output for=go-nil-map
0 0 true
panic: assignment to entry in nil map
map[apple:3]
```

- **Rust map[&key] panics for a missing key.** For the index operation, the docs say "Panics if the key is not present in the HashMap."[^4] Use `get`, which gives an `Option`. `insert` also gives an `Option`, the old value:

```rust run id=rust-get-insert
use std::collections::HashMap;

fn main() {
    let mut map = HashMap::new();
    println!("{:?}", map.insert("apple", 3));
    println!("{:?}", map.insert("apple", 4));
    println!("{:?} {:?}", map.get("apple"), map.get("pear"));
    println!("{}", map["apple"]);
}
```

```output for=rust-get-insert
None
Some(3)
Some(4) None
4
```

- **C++ operator[] adds a key when it reads.** It returns "a reference to the value that is mapped to a key equivalent to key or x respectively, performing an insertion if such key does not already exist."[^8] A read of a missing key adds the key with the value 0. Use `find`, `contains` or `at` to read:

```cpp run id=cpp-operator-insert
#include <iostream>
#include <string>
#include <unordered_map>

int main() {
    std::unordered_map<std::string, int> map{{"apple", 3}};
    std::cout << map.contains("pear") << ' ' << map.size() << '\n';
    int n = map["pear"];
    std::cout << n << ' ' << map.contains("pear") << ' ' << map.size() << '\n';
}
```

```output for=cpp-operator-insert
0 1
0 1 2
```

- **C hsearch ENTER does not replace a value.** POSIX gives a hash table, but only one for each process, and no function that deletes one entry. The action ENTER acts "if it cannot be found in the table"[^9]: "ENTER indicates that the item should be inserted in the table at an appropriate point."[^9] For a key that is there, `hsearch` returns the old entry and does not change it. Change the `data` of the returned entry:

```c run id=c-hsearch-enter
#define _XOPEN_SOURCE 700
#include <search.h>
#include <stdio.h>

int main(void) {
    static int three = 3;
    static int four = 4;
    if (hcreate(8) == 0) {
        return 1;
    }
    char key[] = "apple";
    hsearch((ENTRY){key, &three}, ENTER);
    ENTRY *e = hsearch((ENTRY){key, &four}, ENTER);
    printf("%d\n", *(int *)e->data);
    e->data = &four;
    e = hsearch((ENTRY){key, NULL}, FIND);
    printf("%d\n", *(int *)e->data);
    hdestroy();
    return 0;
}
```

```output for=c-hsearch-enter
3
4
```

- **Bash an array without declare -A uses numbers as keys.** "Indexed arrays are referenced using arithmetic expressions that must expand to an integer"[^10]. In an arithmetic expression, `apple` is the name of a variable, and "A shell variable that is null or unset evaluates to 0 when referenced by name in an expression."[^10] Thus `apple` and `pear` are both the index 0. "Associative arrays are created using declare -A name"[^10]:

```bash run id=bash-declare-assoc
f() {
  local wrong=()
  wrong[apple]=3
  wrong[pear]=5
  echo "${#wrong[@]} ${!wrong[*]} ${wrong[0]}"
  local -A map=()
  map[apple]=3
  map[pear]=5
  echo "${#map[@]} ${map[apple]} ${map[pear]}"
}
f
```

```output for=bash-declare-assoc
1 0 5
2 3 5
```

## Related tasks

- [[iterate-over-a-map]]: list the keys, the values and the entries of a map.
- [[merge-two-maps]]: combine the entries of two maps.
- [[use-a-map-with-a-default-value]]: read a missing key with a default value.
- [[use-a-custom-key-in-a-map]]: use a value of your own type as a key.
- [[create-a-set]]: a set is a map with keys and no values.
- [[create-an-array]]: the same task for an array.

## References
