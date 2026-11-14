---
tag: DEB5SJ
kind: concept
slug: multiset
name: Multiset
aliases: [bag, mset, counter, Counter, std::multiset, Multiset, frequency map, count map]
area: data-and-values/maps-and-sets
level: advanced
summary: >-
  A multiset is a set in which an item can appear more than once. Programs store it as a map from each item to its count, as Python Counter does.
applies_to: core-languages
viz: optional
formal: none
see_also: [set-concept, map-and-dictionary, multimap, count-word-frequencies, remove-duplicates-from-a-list, hash-codes]
sources:
- url: https://en.wikipedia.org/wiki/Multiset
  title: Multiset, Wikipedia
  locator: Introduction
- url: https://docs.python.org/3/library/collections.html
  title: collections, Container datatypes, The Python Standard Library
  locator: Counter objects
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Map.html
  title: Interface Map, Java SE 25 API
  locator: merge
- url: https://en.cppreference.com/cpp/container/multiset
  title: std::multiset, cppreference.com
  locator: Description
- url: https://github.com/google/guava/wiki/NewCollectionTypesExplained
  title: New Collection Types Explained, Guava wiki
  locator: Multiset
---

## Idea

A [set](/set-concept/) holds each item once. A list holds items in an order. A multiset is between the two: it can hold an item more than once, like a list, but the order does not matter, like a set. The letters of a word, the words of a text, the coins in a pocket and the items in a shopping cart are multisets.

Wikipedia defines a multiset, also called a bag, as "a modification of the concept of a set"[^1] that "allows for multiple instances for each of its elements"[^1]. "The number of instances given for each element is called the multiplicity of that element in the multiset."[^1] A program rarely stores every copy. It stores each item once, with its count. Python `Counter` works this way: "It is a collection where elements are stored as dictionary keys and their counts are stored as dictionary values."[^2]

```python run id=idea
from collections import Counter

# A multiset is a set that can hold an item more than once.
# Counter stores each item once, with the number of times it appears.
bag = Counter(["apple", "pear", "apple", "fig", "apple", "pear"])
print(bag)
print(bag["apple"], bag["kiwi"])

# Order does not matter: two lists with the same items the same number of times are equal as multisets.
print(Counter("listen") == Counter("silent"), Counter("aab") == Counter("ab"))

# The size counts every copy. The number of distinct items is smaller.
print(bag.total(), len(bag))

# Union takes the larger count, intersection the smaller, sum adds the counts.
a, b = Counter("aabc"), Counter("abbd")
print(sorted((a | b).elements()), sorted((a & b).elements()), sorted((a + b).elements()))
```

```output for=idea
Counter({'apple': 3, 'pear': 2, 'fig': 1})
3 0
True False
6 3
['a', 'a', 'b', 'b', 'c', 'd'] ['a', 'b'] ['a', 'a', 'a', 'b', 'b', 'b', 'c', 'd']
```

The words listen and silent use the same letters the same number of times, so their multisets are equal. The strings aab and ab use the same letters, so their sets are equal, but their multisets are not.

## How it works

**A map from item to count.** The usual form is a [map](/map-and-dictionary/) whose values are counts. To add an item, add 1 to its count, or put 1 if the item is new. To remove one copy, take 1 from the count, and remove the key when the count reaches 0. The count of an item that is not in the map is 0. Python `Counter` returns this 0: "Counter objects have a dictionary interface except that they return a zero count for missing items instead of raising a KeyError"[^2]. The Java `Map.merge` method does the update in one call, and it removes the key when the function returns `null`: "If the remapping function returns null, the mapping is removed."[^3]

**Every copy in a sorted container.** C++ `std::multiset` stores every copy. It "is an associative container that contains a sorted set of objects of type Key. Unlike set, multiple keys with equivalent values are allowed."[^4] The copies of a value sit next to each other in the tree. This form is useful when the copies are equivalent by the order but different in other fields, for example records sorted by date.

**Operations.** The operations of sets have multiset forms that work on the counts. For Python, "Addition and subtraction combine counters by adding or subtracting the counts of corresponding elements. Intersection and union return the minimum and maximum of corresponding counts."[^2] Inclusion asks whether each count in one multiset is less than or equal to the count in the other. It answers the question: can the letters of this note be cut from that page?

**Size.** A multiset has two sizes: the number of copies and the number of distinct items. In the idea above, the bag has six fruits but three kinds of fruit. Guava says its `Multiset` helps with "collecting a variety of useful statistics, like the total number of words"[^5], which a plain map of counts does not give.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `collections.Counter`, with `most_common`, `total`, `elements` and `+ - \| &` |
| JavaScript | idiom | a `Map` from item to count |
| TypeScript | idiom | the JavaScript idiom, typed as `Map<T, number>` |
| Java | idiom | `Map<E, Integer>` with `merge`; `Collectors.counting`; Guava `Multiset` |
| Kotlin | idiom | `groupingBy { it }.eachCount()`; a `MutableMap` of counts |
| C# | idiom | `CountBy` in .NET 9; a `Dictionary<T, int>` of counts |
| Go | idiom | `map[T]int`; a missing key reads as 0, so `m[k]++` works |
| Rust | idiom | `HashMap<T, usize>` with `*m.entry(k).or_insert(0) += 1` |
| C | idiom | an array of counts for a small alphabet, or a hash table of counts |
| C++ | stdlib | `std::multiset` and `std::unordered_multiset`; `std::map<T, int>` of counts |
| Bash | idiom | an associative array of counts; `sort \| uniq -c` for lines |
| PowerShell | idiom | `Group-Object`, or a hashtable of counts |
| SQL | native | tables and query results are multisets of rows; `GROUP BY` with `COUNT(*)` |
| Regex | n/a | patterns match text and have no collections |
| HTML | n/a | markup with no data structures |
| CSS | n/a | style rules with no data structures |

Python names the idea directly: "The Counter class is similar to bags or multisets in other languages."[^2] C++ has two multiset containers, a sorted one and a hashed one. For the sorted one, "Search, insertion, and removal operations have logarithmic complexity."[^4]

Java has no multiset in its standard library. Guava describes the plain Java form, a `Map<String, Integer>` updated by hand, as "awkward, prone to mistakes, and doesn't support collecting a variety of useful statistics"[^5], and adds a `Multiset` type. With Java 8 and later, `merge` and `Collectors.counting` make the map form short. Guava's `count` method returns "the number of occurrences of an element that have been added to this multiset"[^5].

The example files show these forms. Python uses the `Counter` methods and shows that a count set to 0 stays in the counter. JavaScript builds a `Map` of counts and writes equality and the removal of one copy by hand. Java uses `merge` and `Collectors.counting`. C# uses `CountBy` and `CollectionsMarshal.GetValueRefOrAddDefault`. Go uses `m[k]++` and `maps.Equal`. Rust uses `entry` and compares two `BTreeMap` values. C uses an array of 26 counts to test anagrams and inclusion. C++ uses `std::multiset` with `count`, `equal_range` and both forms of `erase`.

## Variations and related ideas

- **Word counts.** Counting the words of a text builds a multiset. See [count word frequencies](/count-word-frequencies/).
- **Sets.** A multiset whose counts are all 1 is a set. The keys of a count map are the set of distinct items. See [remove duplicates from a list](/remove-duplicates-from-a-list/).
- **Multimaps.** A [multimap](/multimap/) keeps a list of values for each key. A multiset keeps only the number of values.
- **Sorted multisets.** A sorted multiset can count the items in a range. Guava `TreeMultiset` and C++ `std::multiset` do this, and so does an [ordered map](/ordered-map/) of counts.
- **Histograms.** A histogram is a multiset of the values that fall into each bin. A count map of the bin numbers builds one.
- **Signed counts.** Python `Counter` allows counts of zero and below, so it can hold a difference between two multisets. A true multiset has only positive counts.

## Pitfalls

- **Keys with a count of zero.** Python says: "Setting a count to zero does not remove an element from a counter. Use del to remove it entirely"[^2]. Then `len` and `in` count an item that has no copies. In any count map, remove the key when its count reaches 0.
- **Negative counts.** Python `subtract` and a hand-written update can make a count below zero. The `-` operator drops counts of zero or less, but `subtract` keeps them. Check which one the program needs.
- **Equality of count maps.** Two count maps are equal multisets only when no key has a count of 0. A map with `{"a": 1, "b": 0}` is not equal to `{"a": 1}` as a dict, but it holds the same multiset.
- **Two sizes.** `len` of a `Counter` and `size` of a `Map` give the number of distinct items. The number of copies is the sum of the counts, which Python 3.10 gives with `total`.
- **Erase in C++.** `m.erase(value)` on a `std::multiset` removes every copy. To remove one copy, use `m.erase(m.find(value))`, after a check that `find` did not return `end()`.
- **Ties in the top items.** Python `most_common` orders items with equal counts by first appearance. Other languages give no order for ties. Sort by count and then by the item to get a stable result.

## Cost

| Operation | Hash map of counts | Tree map of counts | `std::multiset` (every copy) |
|---|---|---|---|
| add one copy | constant on average | logarithmic in the distinct items | logarithmic in the copies |
| count of an item | constant on average | logarithmic | logarithmic + the count |
| remove one copy | constant on average | logarithmic | logarithmic |
| union, intersection, sum | linear in the distinct items | linear in the distinct items | linear in the copies |
| top k items | n log k with a heap, or a sort | the same | the same |
| memory | one entry for each distinct item | one node for each distinct item | one node for each copy |

A map of counts is much smaller than a list of copies when items repeat often. A count array, as in the C example, is the smallest and fastest form when the items are small integers or letters.

## History

The idea is old, but the name is new. Wikipedia: "Nicolaas Govert de Bruijn coined the word multiset in the 1970s, according to Donald Knuth."[^1] Smalltalk-80 had a `Bag` class. The C++ Standard Template Library had `multiset`, and it became part of the C++ standard in 1998. SQL tables and query results have always been multisets of rows, and `UNION ALL` keeps the copies that `UNION` removes. Python added `collections.Counter` in 2.7 and 3.1, and `total` in 3.10. Guava added `Multiset` for Java, and .NET 9 added `CountBy` in 2024.

## References
