---
tag: 4ZZDHS
kind: concept
slug: ordered-map
name: Ordered map
aliases: [sorted map, tree map, sorted dictionary, navigable map, TreeMap, BTreeMap, SortedDictionary, SortedList, std::map, SortedMap, NavigableMap]
area: data-and-values/maps-and-sets
level: core
summary: >-
  An ordered map keeps its keys sorted. It finds a key in logarithmic time, and it also gives the smallest key, the next key after a value and all keys in a range.
applies_to: core-languages
viz: optional
formal: none
see_also: [map-and-dictionary, hash-table, insertion-ordered-map, set-concept, sort-with-a-custom-key, range, iterators, value-equality-and-identity]
sources:
- url: https://en.wikipedia.org/wiki/Associative_array
  title: Associative array, Wikipedia
  locator: Self-balancing binary search trees; Comparison; Ordered dictionary
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/TreeMap.html
  title: Class TreeMap, Java SE 25 API
  locator: Description
- url: https://en.cppreference.com/cpp/container/map
  title: std::map, cppreference.com
  locator: Description
- url: https://doc.rust-lang.org/std/collections/struct.BTreeMap.html
  title: Struct BTreeMap, The Rust Standard Library
  locator: Description; range
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.sorteddictionary-2
  title: SortedDictionary<TKey,TValue> class, .NET API reference
  locator: Remarks
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/NavigableMap.html
  title: Interface NavigableMap, Java SE 25 API
  locator: Description
- url: https://docs.python.org/3/library/bisect.html
  title: bisect, Array bisection algorithm, The Python Standard Library
  locator: Introduction
---

## Idea

A [map](/map-and-dictionary/) answers one question: what is the value for this key? An ordered map also knows the order of its keys. It can give the smallest key and the largest key, the closest key below or above a value that is not in the map, and all keys between two values. A program uses an ordered map for a timeline of events, a price list, a leaderboard or any data that it reads in key order.

Wikipedia describes the main kind: "The order of enumeration is always deterministic for a given set of keys by sorting. This is the case for tree-based implementations, one representative being the std::map (a tree map) container of C++"[^1]. Python has no ordered map in its standard library, so the example below builds a small one from a dict and a sorted list of keys.

```python run id=idea
import bisect

# An ordered map keeps its keys sorted. Python has no such type, so this one
# keeps a dict for the values and a sorted list for the keys.
prices = {}
keys = []


def put(k, v):
    if k not in prices:
        bisect.insort(keys, k)
    prices[k] = v


for k, v in [(30, "c"), (10, "a"), (50, "e"), (20, "b"), (40, "d")]:
    put(k, v)
print(keys)

# Floor and ceiling: the largest key <= 35 and the smallest key >= 35.
i = bisect.bisect_right(keys, 35)
j = bisect.bisect_left(keys, 35)
print(keys[i - 1], keys[j])

# A range query: all keys from 20 up to but not including 45.
lo, hi = bisect.bisect_left(keys, 20), bisect.bisect_left(keys, 45)
print([(k, prices[k]) for k in keys[lo:hi]])
print(keys[0], keys[-1])
```

```output for=idea
[10, 20, 30, 40, 50]
30 40
[(20, 'b'), (30, 'c'), (40, 'd')]
10 50
```

The keys go in as 30, 10, 50, 20, 40 and come out sorted. The key 35 is not in the map, but its floor is 30 and its ceiling is 40. A hash map cannot answer these questions without a look at every key.

## How it works

**A tree or a sorted array.** Most ordered maps are balanced search trees. Java `TreeMap` is "A Red-Black tree based NavigableMap implementation"[^2], and for C++ `std::map`, "Search, removal, and insertion operations have logarithmic complexity. Maps are usually implemented as Red–black trees"[^3]. Rust uses a B-tree, where each node holds an array of keys: "B-Trees represent a fundamental compromise between cache-efficiency and actually minimizing the amount of work performed in a search"[^4]. A sorted array of keys with a binary search is also an ordered map. It is small and fast to search, but an insert moves all later keys. C# has both: `SortedDictionary` is a tree and `SortedList` is two sorted arrays[^5].

**Order, not hash.** An ordered map compares keys with an order, not with a hash code. Rust says "keys must be of a type that implements the Ord trait, such that two keys can always be compared to determine their Ordering"[^4]. Two keys are the same key when neither is less than the other. For C++, "two objects a and b are considered equivalent (not unique) if neither compares less than the other: !comp(a, b) && !comp(b, a)"[^3]. So a comparator that ignores case makes `apple` and `APPLE` one key. Java warns that the order "must be consistent with equals if this sorted map is to correctly implement the Map interface"[^2].

**Navigation.** The extra operations of an ordered map find keys near a value. Java `NavigableMap` is "A SortedMap extended with navigation methods returning the closest matches for given search targets"[^6]: `lowerKey`, `floorKey`, `ceilingKey` and `higherKey` return the keys "less than, less than or equal, greater than or equal, and greater than"[^6] a value. C++ has `lower_bound` and `upper_bound`, and Rust has `range` with a start and an end. Each of these takes logarithmic time, because the search goes down one path of the tree.

**Tree map or hash map.** A hash map is faster on average for a single lookup. A tree map has a better worst case and keeps order. Wikipedia: "The worst-case performance of self-balancing binary search trees is significantly better than that of a hash table, with a time complexity in big O notation of O(log n)"[^1], and "like all binary search trees, self-balancing binary search trees keep their elements in order"[^1]. Choose a tree map when the program needs order or ranges, and a [hash table](/hash-table/) when it only looks up single keys.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | no sorted map in the standard library; `sorted(d)`, or a sorted list of keys with `bisect` |
| JavaScript | idiom | no sorted map; sort the entries of a `Map`, or keep a sorted array of keys |
| TypeScript | idiom | the JavaScript idioms |
| Java | stdlib | `TreeMap` with `NavigableMap`: `floorKey`, `ceilingKey`, `subMap`, `headMap`, `tailMap` |
| Kotlin | stdlib | `sortedMapOf` and `toSortedMap`, which make a `java.util.TreeMap` |
| C# | stdlib | `SortedDictionary<TKey,TValue>` (a tree) and `SortedList<TKey,TValue>` (sorted arrays) |
| Go | idiom | no ordered map; `slices.Sorted(maps.Keys(m))` and `slices.BinarySearch` |
| Rust | stdlib | `BTreeMap<K, V>` with `range`, `first_key_value`, `last_key_value`, `split_off` |
| C | idiom | a sorted array with `qsort` and `bsearch`; POSIX `tsearch` for a binary tree |
| C++ | stdlib | `std::map` with `lower_bound`, `upper_bound` and `equal_range`; `std::flat_map` in C++23 |
| Bash | idiom | sort the keys of an associative array with `sort` |
| PowerShell | stdlib | the .NET `SortedDictionary` and `SortedList` |
| SQL | native | an index on the key column, usually a B-tree; `ORDER BY` and `BETWEEN` |
| Regex | n/a | patterns match text and have no maps |
| HTML | n/a | markup with no data structures |
| CSS | n/a | style rules with no data structures |

C# documents the choice between its two types. "The SortedDictionary<TKey,TValue> generic class is a binary search tree with O(log n) retrieval"[^5]. Then: "SortedList<TKey,TValue> uses less memory than SortedDictionary<TKey,TValue>. SortedDictionary<TKey,TValue> has faster insertion and removal operations for unsorted data: O(log n) as opposed to O(n) for SortedList<TKey,TValue>"[^5].

Python, JavaScript and Go have no ordered map. A program sorts the keys when it needs the order, or it keeps a sorted list of keys next to a hash map. Python's `bisect` module helps with the list: it gives "support for maintaining a list in sorted order without having to sort the list after each insertion"[^7].

The example files show these forms. Python sorts the items of a dict by key and by value, uses `bisect` for ceilings and ranges, and shows that keys of mixed types cannot be sorted. JavaScript shows that the default sort compares strings, and that an object puts integer keys first. Java uses `TreeMap` with floor and ceiling, its range views, and a `null` key that throws. C# compares `SortedDictionary` and `SortedList`. Go and C keep sorted keys and use binary search. Rust uses `BTreeMap::range` with bounds and `split_off`. C++ uses `lower_bound`, `upper_bound`, a range `erase` and a comparator that ignores case.

## Variations and related ideas

- **Sorted sets.** A sorted set is an ordered map with no values: Java `TreeSet`, Rust `BTreeSet`, C# `SortedSet` and C++ `std::set`. See [set](/set-concept/).
- **Insertion order.** An ordered map keeps the order of the keys. A map that keeps the order in which the keys were added is a different thing. Python `dict`, Java `LinkedHashMap` and JavaScript `Map` keep insertion order. See [insertion-ordered map](/insertion-ordered-map/).
- **Range views.** Java `subMap`, `headMap` and `tailMap` return views, not copies. A change to the view changes the map. Rust `range` and C++ iterator pairs also refer to the map and do not copy it.
- **Flat maps.** C++23 `std::flat_map` and C# `SortedList` keep the keys in a sorted array. They use less memory and loop faster, but inserts are linear.
- **Concurrent sorted maps.** Java `ConcurrentSkipListMap` is a sorted map that many threads can change at the same time. It uses a skip list instead of a tree.
- **Multimaps.** C++ `std::multimap` is an ordered map that allows the same key more than once. See [multimap](/multimap/).

## Pitfalls

- **An order that disagrees with equality.** A comparator that ignores case, or that compares only one field, makes two different keys into one key. The second `put` replaces the first value. Java says the result "must be consistent with equals"[^2], and the C++ map keeps one key for each class of equivalent keys.
- **Keys that change.** The tree puts a key in a place by its order at the time of the insert. If the key changes so that its order changes, the map cannot find it. Rust calls this "a logic error for a key to be modified in such a way that the key’s ordering relative to any other key, as determined by the Ord trait, changes while it is in the map"[^4].
- **Floating-point keys.** `NaN` has no place in an order. Rust does not let `f64` be a key of a `BTreeMap`. In other languages, a `NaN` key can break the order of the tree. Use integers, such as cents instead of dollars, or a wrapper with a total order.
- **String order.** The order of strings depends on the comparer. JavaScript sorts numbers as strings by default, so `[10, 9, 1].sort()` gives `[1, 10, 9]`. C# and Java can sort with culture rules that differ between machines. Give an explicit comparer, such as `StringComparer.Ordinal`.
- **Floor at the edge.** A key below the smallest key has no floor, and a key above the largest has no ceiling. Java returns `null`, Rust returns `None`, and C++ returns `end()` or needs a check before `std::prev`. A program that does not check reads past the end.
- **A null key.** Java `TreeMap` must compare each key, so `put(null, v)` throws `NullPointerException` with the natural order.
- **Linear inserts.** A sorted array, as in `SortedList` or a Python list with `insort`, moves the later keys on each insert. For many inserts in random order, use a tree.

## Cost

| Operation | Balanced tree (`TreeMap`, `std::map`, `BTreeMap`) | Sorted array (`SortedList`, `bisect`) | Hash map, then sort |
|---|---|---|---|
| lookup | logarithmic | logarithmic | constant on average |
| insert or remove | logarithmic | linear | constant on average |
| smallest or largest key | logarithmic (constant in some) | constant | linear |
| floor or ceiling | logarithmic | logarithmic | linear |
| range of k keys | logarithmic + k | logarithmic + k | n log n for the sort |
| loop in key order | linear | linear | n log n for the sort |
| memory | the pairs and two or more links for each node | the pairs | the pairs and empty slots |

Java `TreeMap` gives "guaranteed log(n) time cost for the containsKey, get, put and remove operations"[^2]. A tree has a pointer for each child and visits nodes that are far apart in memory, so a lookup is slower than a lookup in a hash map of the same size. A B-tree keeps many keys in each node, which makes better use of the processor cache.

## History

The balanced search trees came first in the 1960s and 1970s: the AVL tree in 1962, the B-tree in 1970 and the red-black tree in 1978. Java added `TreeMap` in 1998 with the Collections Framework, and Java 6 added `NavigableMap`. C++ had `std::map` in the first standard library in 1998. Rust chose a B-tree for its `BTreeMap`, and C++23 added `std::flat_map`, a map on sorted arrays. Python, JavaScript and Go have no ordered map in the standard library. In Python, the third-party package sortedcontainers fills the gap.

## References
