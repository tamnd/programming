---
tag: TVB6S4
kind: concept
slug: multimap
name: Multimap
aliases: [multidict, multi-value map, map of lists, dictionary of lists, std::multimap, ILookup, Multimap]
area: data-and-values/maps-and-sets
level: advanced
summary: >-
  A multimap is a map in which one key can have more than one value. Most languages build it as a map whose values are lists, and C++ and C# have types for it.
applies_to: core-languages
viz: optional
formal: none
see_also: [map-and-dictionary, multiset, ordered-map, group-items-by-a-key, list, hash-table]
sources:
- url: https://en.wikipedia.org/wiki/Multimap
  title: Multimap, Wikipedia
  locator: Introduction; Examples; Language support
- url: https://github.com/google/guava/wiki/NewCollectionTypesExplained
  title: New Collection Types Explained, Guava wiki
  locator: Multimap
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Map.html
  title: Interface Map, Java SE 25 API
  locator: computeIfAbsent
- url: https://en.cppreference.com/cpp/container/multimap
  title: std::multimap, cppreference.com
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.lookup-2
  title: Lookup<TKey,TElement> class, .NET API reference
  locator: Remarks
- url: https://docs.python.org/3/library/collections.html
  title: collections, Container datatypes, The Python Standard Library
  locator: defaultdict examples
- url: https://docs.python.org/3/library/urllib.parse.html
  title: urllib.parse, Parse URLs into components, The Python Standard Library
  locator: parse_qs
- url: https://developer.mozilla.org/en-US/docs/Web/API/URLSearchParams/getAll
  title: URLSearchParams getAll() method, MDN Web Docs
  locator: Description; Return value
---

## Idea

A [map](/map-and-dictionary/) has one value for each key. A second `put` with the same key replaces the first value. Much real data has more than one value for a key: a student takes many courses, a word appears on many pages of a book, and a web form sends many values for one check box field. A multimap keeps all of them.

Wikipedia defines it as "a generalization of a map or associative array abstract data type in which more than one value may be associated with and returned for a given key"[^1]. It gives the student example: "If a student is enrolled in three courses, there will be three associations containing the same key."[^1] The usual way to build one is also in the definition: "Often the multimap is implemented as a map with lists or sets as the map values."[^1]

```python run id=idea
# A multimap can hold more than one value for a key.
# Here each student has a list of courses.
enrolled = {}


def add(student, course):
    enrolled.setdefault(student, []).append(course)


add("ada", "math")
add("bob", "art")
add("ada", "physics")
add("ada", "music")

print(enrolled["ada"], enrolled["bob"])
print(enrolled.get("cy", []))

# The number of keys and the number of key-value pairs are different.
print(len(enrolled), sum(len(v) for v in enrolled.values()))

# The same data, seen as one pair for each value.
print([(s, c) for s, cs in enrolled.items() for c in cs])
```

```output for=idea
['math', 'physics', 'music'] ['art']
[]
2 4
[('ada', 'math'), ('ada', 'physics'), ('ada', 'music'), ('bob', 'art')]
```

The map has two keys but four pairs. A student with no courses gives an empty list, not an error.

## How it works

**Two views of the same data.** The Guava library describes two ways to see a multimap. It is "a collection of mappings from single keys to single values"[^2], such as `a -> 1`, `a -> 2` and `b -> 3`, or it is "a mapping from unique keys to collections of values"[^2], such as `a -> [1, 2]` and `b -> [3]`. The first view counts pairs. The second view counts keys. Guava warns that its `size` method "returns the number of entries in the entire multimap, not the number of distinct keys"[^2].

**A map of collections.** Most programs build a multimap from a normal map whose values are lists or sets. To add a value, the program gets the list for the key, makes an empty list if the key is new, and appends the value. Each language has a short form for this step: `setdefault` or `defaultdict` in Python, `computeIfAbsent` in Java, `entry().or_default()` in Rust, and `append` on a missing key in Go. The Java `Map` documentation shows the step for "a multi-value map, Map<K,Collection<V>>, supporting multiple values per key"[^3]. To remove a value, the program removes it from the list, and then removes the key if the list is empty.

**Lists or sets as values.** A list keeps every value, in the order of the inserts, and it can hold the same value twice. A set keeps each value once for each key. Guava has both kinds: a `ListMultimap` and a `SetMultimap`. Choose by the question that the data must answer. A list of the orders of a customer can repeat a product. A set of the tags of a page cannot repeat a tag.

**One container for all pairs.** C++ `std::multimap` does not use a list for each key. It "is an associative container that contains a sorted list of key-value pairs, while permitting multiple entries with the same key"[^4]. All pairs are in one tree. The pairs with equal keys are next to each other, and "The order of the key-value pairs whose keys compare equivalent is the order of insertion and does not change."[^4] The method `equal_range` returns the run of pairs for one key.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | a dict of lists with `setdefault` or `collections.defaultdict(list)` |
| JavaScript | idiom | a `Map` of arrays; `Map.groupBy`; `URLSearchParams` with `getAll` |
| TypeScript | idiom | the JavaScript idioms, typed as `Map<K, V[]>` |
| Java | idiom | `Map<K, List<V>>` with `computeIfAbsent`; `Collectors.groupingBy`; Guava `Multimap` |
| Kotlin | idiom | `MutableMap<K, MutableList<V>>` with `getOrPut`; `groupBy` |
| C# | stdlib | `ILookup<TKey,TElement>` from `ToLookup`, which cannot change; `Dictionary<TKey, List<TValue>>` |
| Go | idiom | `map[K][]V`; `url.Values` and `http.Header` are maps of string slices |
| Rust | idiom | `HashMap<K, Vec<V>>` with `entry(k).or_default().push(v)` |
| C | idiom | a sorted array of pairs, with a binary search for the first pair of a key |
| C++ | stdlib | `std::multimap` and `std::unordered_multimap` with `equal_range` and `count` |
| Bash | idiom | one string of joined values for each key, or one array for each key |
| PowerShell | idiom | a hashtable of lists, or `Group-Object` |
| SQL | native | a table whose key column is not unique; `WHERE`, `GROUP BY` |
| Regex | n/a | patterns match text and have no maps |
| HTML | n/a | markup with no data structures |
| CSS | n/a | style rules with no data structures |

C++ has two multimap types. Wikipedia: "The C++ standard library provides the std::multimap container for the sorted multimap using a self-balancing binary search tree"[^1], and C++ also has "the std::unordered_multimap for the unordered multimap implemented with a hash table"[^1]. Search, insert and remove in `std::multimap` "have logarithmic complexity"[^4].

C# has a multimap in LINQ, but it is read-only. `Lookup<TKey,TElement>` "Represents a collection of keys each mapped to one or more values"[^5], and "Lookup<TKey,TElement> objects are immutable, that is, you cannot add or remove elements or keys from a Lookup<TKey,TElement> object after it has been created"[^5]. A program that adds values over time uses a `Dictionary` of lists.

Python shows the `defaultdict` form in its documentation: "Using list as the default_factory, it is easy to group a sequence of key-value pairs into a dictionary of lists"[^6]. Query strings are a common multimap. For Python `parse_qs`, "The dictionary keys are the unique query variable names and the values are lists of values for each name."[^7] In JavaScript, the `URLSearchParams` method `getAll` "returns all the values associated with a given search parameter as an array"[^8].

The example files show these forms. Python uses `defaultdict(list)` and `defaultdict(set)`, removes a key when its list is empty, and reads a query string with `parse_qs`. JavaScript uses a `Map` of arrays, `Map.groupBy` and `URLSearchParams`. Java uses `computeIfAbsent`, `computeIfPresent` and `Collectors.groupingBy`. C# uses `ToLookup` and a `Dictionary` of lists. Go appends to a missing key and reads `url.Values`. Rust uses `entry().or_default()` on a `BTreeMap`. C sorts an array of pairs and finds the run of each key. C++ uses `std::multimap` with `equal_range`, `count` and `erase`.

## Variations and related ideas

- **Grouping.** Building a multimap from a list is grouping: [group items by a key](/group-items-by-a-key/). `itertools.groupby`, `Collectors.groupingBy`, `GroupBy` in LINQ and `GROUP BY` in SQL do it.
- **Multisets.** A [multiset](/multiset/) keeps only the number of times each item appears. A multimap whose values are all the same is a multiset of keys.
- **Inverse index.** A multimap from each word to the documents that contain it is an inverted index, the base of a search engine. The index of a book is the same idea.
- **Bimaps.** A bimap holds a map and its inverse, so a program can look up a key by its value. Guava has `BiMap`. Unlike a multimap, a bimap does not allow two keys to have the same value.
- **Graphs.** A map from each node to the list of its neighbors is an adjacency list. Guava notes that a map of sets "is a typical way to represent an unlabeled directed graph"[^2].
- **HTTP headers.** A header name can appear more than once in a request, so header types in Go, Java and .NET hold a list of values for each name.

## Pitfalls

- **Keys with empty lists.** A program that removes the last value of a key but keeps the key gets a key with an empty list. Then a test such as `key in map` says yes, but there are no values. Remove the key when its list becomes empty.
- **Reading adds a key.** In Python, `d[k]` on a `defaultdict` adds `k` with an empty list. Use `d.get(k)` or `k in d` when the program must only read.
- **The first value or all values.** `URLSearchParams.get`, Go `url.Values.Get` and `http.Header.Get` return only the first value of a name. A program that must see all values uses `getAll` or the slice. Converting a multimap to a plain map, for example with `Object.fromEntries`, keeps only one value for each key.
- **Size.** The number of keys and the number of pairs are different numbers. Check which one a method returns.
- **Shared lists.** In Python, `dict.fromkeys(keys, [])` gives every key the same list, so an append to one key changes them all. Use `defaultdict(list)` or a new list for each key.
- **No index operator in C++.** `std::multimap` has no `operator[]`, and `erase(key)` removes all pairs with that key. To remove one pair, erase an iterator.

## Cost

| Operation | Hash map of lists | Tree map of lists | `std::multimap` (one tree) |
|---|---|---|---|
| add a value | constant on average | logarithmic in the keys | logarithmic in the pairs |
| all values of a key | constant on average to find, then k | logarithmic in the keys, then k | logarithmic in the pairs, then k |
| count values of a key | constant on average | logarithmic in the keys | logarithmic + k |
| remove one value | find, then linear in the list | find, then linear in the list | logarithmic, with an iterator constant |
| remove a key | constant on average | logarithmic | logarithmic + k |
| loop over all pairs | linear | linear, in key order | linear, in key order |
| memory | one list for each key | one list for each key | one tree node for each pair |

Here k is the number of values of one key. A map of lists stores each key once, so it uses less memory when keys have many values. One tree for all pairs, as in `std::multimap`, stores the key in each pair.

## History

Multimaps came into wide use with the C++ Standard Template Library, which had `multimap` and `multiset` in its first version and entered the C++ standard in 1998. The hash form `std::unordered_multimap` came with C++11[^1], and C++11 also fixed the order of the pairs with equal keys as the order of insertion[^4]. Java never added a multimap to its standard library. Apache Commons Collections and Google Guava filled the gap, and Java 8 in 2014 added `computeIfAbsent` and `Collectors.groupingBy`, which make the map of lists short to write. C# added `ToLookup` with LINQ in 2007, and JavaScript added `Map.groupBy` in ECMAScript 2024.

## References
