---
tag: HR3SP3
kind: task
slug: count-the-items-of-each-kind
name: Count the items of each kind
aliases: [count occurrences, frequency count, histogram of a list, tally, Counter, count distinct items]
area: data-and-values/collection-operations
level: intro
summary: Count how many times each distinct item is in a list of integers, and give the counts in ascending order of the item.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/collections.html#collections.Counter
  title: The Python Standard Library, collections
  locator: Counter objects
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/for...in
  title: MDN Web Docs, for...in
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Map.html
  title: Java SE 25 API, Interface Map
  locator: get(Object)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.dictionary-2.item?view=net-10.0
  title: .NET API reference, Dictionary<TKey,TValue>.Item[TKey] property
  locator: Exceptions
- url: https://go.dev/ref/spec#Map_types
  title: The Go Programming Language Specification
  locator: Map types
- url: https://doc.rust-lang.org/std/collections/struct.HashMap.html
  title: Rust standard library, struct HashMap
  locator: keys
- url: https://en.cppreference.com/w/c/algorithm/qsort
  title: cppreference.com, qsort, qsort_s (C)
  locator: Description
- url: https://en.cppreference.com/w/cpp/container/map/operator_at
  title: cppreference.com, std::map::operator[]
  locator: Description
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Arrays
  title: Bash Reference Manual
  locator: 6.7 Arrays
---

## Task

Count how many times each distinct item is in a list of integers. The function `count_items(items)` returns one pair `(item, count)` for each distinct item, in ascending order of the item. For example, `count_items([3, 1, 3, 2, 1, 3])` is `[(1, 2), (2, 1), (3, 3)]`.

The items are 32-bit signed integers, and the list can be empty. Then the result is an empty list. Each count is at least 1, because an item that is not in the list has no pair.

## Solutions

| Language | Count the items, then put them in order |
|---|---|
| Python | `sorted(Counter(items).items())` |
| JavaScript, TypeScript | a `Map`, then `sort` with a function that compares the numbers |
| Java | a `TreeMap` and `merge(x, 1, Integer::sum)` |
| C# | `GroupBy`, `OrderBy` and `Count` |
| Go | a map, then a sorted slice of the keys |
| Rust | a `BTreeMap` and `entry(x).or_insert(0)` |
| C | a sorted copy of the items, then a count of each run of equal items |
| C++ | a `std::map` and `++counts[x]` |
| Bash | an associative array, then `sort -n` |

- **The order.** The task gives the pairs in ascending order of the item. Thus the result does not depend on the order of the keys in a hash map. Java, Rust and C++ use a map that keeps its keys in order. The other solutions sort the keys.
- **C.** C has no map in its standard library. The solution sorts a copy of the items with `qsort`, so equal items are next to each other. Then one loop counts each run of equal items. The caller gives an output array with room for n pairs, because each item can be different.
- **Bash.** The function prints one line `ITEM COUNT` for each distinct item. It prints nothing for an empty list.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: an empty list gives an empty list.
- `single`: `[7]` gives `[(7, 1)]`.
- `negative`: `[-1, 2, -1, 0]` gives `[(-1, 2), (0, 1), (2, 1)]`.
- `distinct`: when no two items are equal, each count is 1.
- `order`: `[9, 8, 9]` gives `[(8, 1), (9, 2)]`, in the order of the items, not the order of the first time that each item is in the list.
- `zero`: 0 is an item, and its count is 2 in `[0, 0]`.
- `extremes`: -2147483648 and 2147483647 are items.

## Pitfalls

- **Python a dict raises KeyError for a missing key.** `counts[x] += 1` reads `counts[x]` first. A `Counter` has "a dictionary interface except that they return a zero count for missing items instead of raising a KeyError"[^1]. A read of a missing item does not add it, so `len(c)` stays 1:

```python run id=python-missing-key
from collections import Counter

counts = {}
try:
    for x in [1, 1]:
        counts[x] += 1
except KeyError as e:
    print("KeyError", e)

c = Counter([1, 1])
print(c[5], 5 in c, len(c))
```

```output for=python-missing-key
KeyError 1
0 False 1
```

- **JavaScript the keys of an object are strings in a special order.** For an object, "all non-negative integer keys (those that can be array indices) will be traversed first in ascending order by value, then other string keys in ascending chronological order of property creation"[^2]. In the example, the keys are `'1'`, `'2'` and `'-1'`, as strings. A `Map` keeps the numbers and the order of insertion:

```javascript run id=javascript-object-keys
const obj = {};
for (const x of [2, -1, 1, 2]) {
  obj[x] = (obj[x] ?? 0) + 1;
}
console.log(Object.keys(obj));

const map = new Map();
for (const x of [2, -1, 1, 2]) {
  map.set(x, (map.get(x) ?? 0) + 1);
}
console.log([...map.keys()]);
```

```output for=javascript-object-keys
[ '1', '2', '-1' ]
[ 2, -1, 1 ]
```

- **Java get gives null for a missing key.** `get` "Returns the value to which the specified key is mapped, or null if this map contains no mapping for the key"[^3]. Thus `counts.get(1) + 1` throws a `NullPointerException` when it unboxes the null. Use `merge`:

```java run id=java-get-null
import java.util.Map;
import java.util.TreeMap;

public class Main {
  public static void main(String[] args) {
    Map<Integer, Integer> counts = new TreeMap<>();
    try {
      counts.put(1, counts.get(1) + 1);
    } catch (NullPointerException e) {
      System.out.println("NullPointerException");
    }
    for (int x : new int[] {1, 1}) {
      counts.merge(x, 1, Integer::sum);
    }
    System.out.println(counts);
  }
}
```

```output for=java-get-null
NullPointerException
{1=2}
```

- **C# the indexer throws for a missing key.** The indexer throws `KeyNotFoundException` when "The property is retrieved and key does not exist in the collection"[^4]. `counts[1]++` reads the value first. Use `GetValueOrDefault`:

```csharp run id=csharp-missing-key
using System;
using System.Collections.Generic;

var counts = new Dictionary<int, int>();
try
{
    counts[1]++;
}
catch (KeyNotFoundException)
{
    Console.WriteLine("KeyNotFoundException");
}
foreach (int x in new[] { 1, 1 })
{
    counts[x] = counts.GetValueOrDefault(x) + 1;
}
Console.WriteLine(counts[1]);
```

```output for=csharp-missing-key
KeyNotFoundException
2
```

- **Go a write to a nil map panics.** "A nil map is equivalent to an empty map except that no elements may be added."[^5] In the example, a read of `counts[5]` gives 0, but `counts[5]++` panics. Make the map with `make`:

```go run id=go-nil-map
package main

import "fmt"

func main() {
	var counts map[int]int
	fmt.Println(counts[5], len(counts))
	defer func() {
		fmt.Println("panic:", recover())
	}()
	counts[5]++
}
```

```output for=go-nil-map
0 0
panic: assignment to entry in nil map
```

- **Rust a HashMap gives its keys in an arbitrary order.** `keys` gives "An iterator visiting all keys in arbitrary order"[^6]. A `BTreeMap` gives its keys in ascending order, so the result does not need a sort. `get` gives `None` for a missing key:

```rust run id=rust-btreemap-order
use std::collections::BTreeMap;

fn main() {
    let mut counts = BTreeMap::new();
    for x in [2, -1, 0, 2] {
        *counts.entry(x).or_insert(0) += 1;
    }
    println!("{:?}", counts);
    println!("{:?}", counts.get(&5));
}
```

```output for=rust-btreemap-order
{-1: 1, 0: 1, 2: 2}
None
```

- **C qsort changes the input.** `qsort` "Sorts the given array pointed to by ptr in ascending order"[^7], in place. In the example, the order of `items` changes. The solution sorts a copy, so the input of the caller does not change:

```c run id=c-qsort-in-place
#include <stdio.h>
#include <stdlib.h>

static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int main(void) {
    int items[] = {3, 1, 2, 1};
    size_t n = sizeof items / sizeof items[0];
    qsort(items, n, sizeof items[0], cmp_int);
    for (size_t i = 0; i < n; i++) {
        printf(i + 1 < n ? "%d " : "%d\n", items[i]);
    }
    return 0;
}
```

```output for=c-qsort-in-place
1 1 2 3
```

- **C++ operator[] adds a missing key.** `operator[]` gives a reference to the value, "performing an insertion if such key does not already exist"[^8]. In the example, the test `counts[5] == 0` adds the key 5, so the size changes from 1 to 2. To test for a key, use `contains` or `find`:

```cpp run id=cpp-map-insert
#include <iostream>
#include <map>

int main() {
    std::map<int, int> counts{{1, 2}};
    std::cout << counts.size() << '\n';
    if (counts[5] == 0) {
        std::cout << "5 is not in the list\n";
    }
    std::cout << counts.size() << '\n';
    std::cout << counts.contains(7) << ' ' << counts.size() << '\n';
}
```

```output for=cpp-map-insert
1
5 is not in the list
2
0 2
```

- **Bash an indexed array counts back from the end for a negative key.** "Associative arrays are created using declare -A name"[^9]. Without `-A`, the array is an indexed array. For an index less than zero, "negative indices count back from the end of the array"[^9]. In the example, `plain[-1]=1` fails because the array is empty. With `declare -A`, -1 is a key:

```bash run id=bash-negative-key
#!/usr/bin/env bash
plain=()
(plain[-1]=1) 2>/dev/null
echo "status $?"
declare -A counts=()
counts[-1]=1
echo "${!counts[@]}"
```

```output for=bash-negative-key
status 1
-1
```

## Related tasks

- [[group-items-by-a-key]]: keep the items of each group, not only the count.
- [[partition-a-list]]: split a list into two groups.
- [[sort-a-list]]: put the items in order.
- [[find-the-largest-element]]: find one item from the whole list.

## References
