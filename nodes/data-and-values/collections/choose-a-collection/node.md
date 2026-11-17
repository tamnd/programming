---
tag: XYNGGR
kind: practice
slug: choose-a-collection
name: Choose a collection
aliases: [choosing a collection, which collection to use, choose a data structure]
area: data-and-values/collections
level: core
summary: >-
  Pick a collection by the operations the code does most and by their cost: a list for order, a set for membership, a map for lookup by key, then the default type of the language.
see_also: [collection, data-structure, collection-interfaces, list, set-concept, map-and-dictionary, deque, priority-queue]
sources:
- url: https://doc.rust-lang.org/std/collections/index.html
  title: Module std::collections, Rust standard library
  locator: When Should You Use Which Collection?; Performance
- url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/doc-files/coll-overview.html
  title: Collections Framework Overview, Java SE 21
  locator: Collection Implementations
- url: https://en.cppreference.com/w/cpp/container.html
  title: Containers library, C++ reference, cppreference.com
  locator: Sequence containers; Associative containers; Unordered associative containers
- url: https://docs.python.org/3.14/tutorial/datastructures.html
  title: Data Structures, The Python Tutorial
  locator: 5.1.2 Using Lists as Queues; 5.4 Sets
- url: https://learn.microsoft.com/en-us/dotnet/standard/collections/
  title: Collections and Data Structures, .NET documentation
  locator: Choose a collection; Algorithmic complexity of collections
- url: https://docs.python.org/3.14/library/collections.html
  title: collections, Python 3.14 documentation
  locator: deque objects
- url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/ArrayDeque.html
  title: ArrayDeque, Java SE 21 API
  locator: Class description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.sorteddictionary-2
  title: SortedDictionary<TKey,TValue> Class, .NET API reference
  locator: Remarks
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Map types; For statements with range clause
- url: https://go.dev/doc/effective_go
  title: Effective Go
  locator: Maps
- url: https://pkg.go.dev/container/list
  title: container/list, Go standard library
  locator: Overview
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Keyed_collections
  title: Keyed collections, MDN Web Docs
  locator: Map object; Object and Map compared; Set object
- url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/ArrayList.html
  title: ArrayList, Java SE 21 API
  locator: Class description
- url: https://docs.python.org/3.14/library/stdtypes.html
  title: Built-in Types, Python 3.14 documentation
  locator: Mapping Types, dict
---

## Statement

Choose a collection by the operations that the code does most often, and by what each operation costs in that collection. The Rust documentation puts it this way: "Choosing the right collection for the job requires an understanding of what each collection is good at"[^1]. First decide the kind of collection: a [list](/list/) when the values have positions, a [set](/set-concept/) when the main question is whether a value is present, and a [map](/map-and-dictionary/) when values are found by a key. Then use the default type of that kind in the language, and change to a special type only when an operation that the code needs is slow in the default or missing from it.

## Meaning

**Ask three questions.**

1. Do the values have an order that matters, or do you read them by position? Use a list. In Java, Rust and C++ the usual list is a [dynamic array](/dynamic-array/), a resizable array[^2][^1][^3].
2. Is each value there only once, and do you mostly ask whether a value is present? Use a set. The Python tutorial says that the basic uses of a set "include membership testing and eliminating duplicate entries"[^4].
3. Do you look up a value by a key? Use a map. The .NET table gives a dictionary for the case "Store items as key/value pairs for quick look-up by key"[^5].

**Then look at the ends.** A list is fast at its end and slow at its front. The Python tutorial says that a list used as a queue is not efficient: "doing inserts or pops from the beginning of a list is slow (because all of the other elements have to be shifted by one)"[^4]. Python's `deque` has appends and pops "from either side of the deque with approximately the same O(1) performance in either direction"[^6]. Use a list for a [stack](/stack-data-structure/) and a [deque](/deque/) for a queue. Rust gives the same advice: use a `Vec` when "You want a stack" and a `VecDeque` when "You want a queue"[^1].

**Then look at order.** A hash set or a hash map keeps its keys unsorted: C++ calls its hash containers "unsorted (hashed) data structures"[^3]. Use a tree when the code needs the keys in order, or the smallest key, or all the keys in a range. Rust says to use a `BTreeMap` when "You want a map sorted by its keys" or "You want to be able to get a range of entries on-demand"[^1]. To take only the largest or the smallest value each time, use a [priority queue](/priority-queue/). Rust says to use a `BinaryHeap` when "You want a priority queue"[^1].

**Then check the cost.** The documentation of each language gives the cost of the main operations. The table below uses the Rust table[^1] and the .NET table[^5]. The *n* is the number of elements and *i* is a position.

| Operation | Array list | Deque | Linked list | Hash map or set | Tree map or set |
|---|---|---|---|---|---|
| read by position | O(1) | O(1) | O(min(i, n-i)) | - | - |
| add or remove at position *i* | O(n-i) | O(min(i, n-i)) | O(min(i, n-i)) | - | - |
| add at the end | O(1) amortized | O(1) amortized | O(1) | - | - |
| find, add or remove by key | - | - | - | O(1) expected | O(log n) |
| find a range of keys | - | - | - | - | O(log n) |

**The defaults in each language.** Each row of this table is a common need, and each cell is the type that the standard library gives for it.

| Need | Python | JavaScript | Java | C# | Go | Rust | C++ |
|---|---|---|---|---|---|---|---|
| a list | `list` | `Array` | `ArrayList` | `List<T>` | slice | `Vec` | `std::vector` |
| a queue at both ends | `collections.deque` | none | `ArrayDeque` | `Queue<T>` | `container/list` | `VecDeque` | `std::deque` |
| a set | `set` | `Set` | `HashSet` | `HashSet<T>` | `map[T]bool` | `HashSet` | `std::unordered_set` |
| a map | `dict` | `Map` | `HashMap` | `Dictionary<TKey,TValue>` | `map[K]V` | `HashMap` | `std::unordered_map` |
| a sorted map | none | none | `TreeMap` | `SortedDictionary<TKey,TValue>` | none | `BTreeMap` | `std::map` |

The sources for the table are these. Java names `HashSet`, `ArrayList`, `ArrayDeque`, `HashMap` and `TreeMap` as general purpose implementations[^2], and says that `ArrayDeque` "is likely to be faster than Stack when used as a stack, and faster than LinkedList when used as a queue"[^7]. The .NET table gives `Dictionary<TKey,TValue>`, `List<T>`, `Queue<T>` and `HashSet<T>`[^5], and `SortedDictionary<TKey,TValue>` "is a binary search tree with O(log n) retrieval"[^8]. Go has slices and maps as built-in types[^9]. Effective Go says that "A set can be implemented as a map with value type bool"[^10], and `container/list` "implements a doubly linked list"[^11]. In JavaScript, a `Map` has keys of any type, and MDN says to "Use maps over objects when keys are unknown until run time"[^12]. In C++, `vector` is a "resizable contiguous array", `deque` is a "double-ended queue", `map` is a "collection of key-value pairs, sorted by keys, keys are unique", and `unordered_map` is a "collection of key-value pairs, hashed by keys, keys are unique"[^3]. The Rust names are from the Rust guide[^1].

## Examples

Each example file keeps the words of one sentence in five collections, one for each row of the second table: a list of all the words, a set of the different words, a map from each word to its count, a queue of the first words, and a sorted map of the counts.

```python run id=five-needs
from collections import Counter, deque

words = "the cat saw the dog and the cat ran".split()

seen = set(words)                 # membership, no duplicates
counts = Counter(words)           # a dict from word to count
line = deque(words[:3])           # a queue: fast at both ends
first = line.popleft()
second = line.popleft()

print(f"list: {len(words)} words, third is {words[2]}")
print(f"set: {len(seen)} distinct")
print(f"map: the={counts['the']} cat={counts['cat']}")
print(f"queue: {first}, then {second}")
print("sorted:", " ".join(sorted(counts)))  # Python has no sorted map: sort the keys
```

```output for=five-needs
list: 9 words, third is saw
set: 6 distinct
map: the=3 cat=2
queue: the, then cat
sorted: and cat dog ran saw the
```

The files for JavaScript, Java, C#, Go, Rust and C++ use the types in the defaults table. Python, JavaScript and Go have no sorted map in the standard library, so their files sort the keys when they print them.

## Criticism and limits

- **The big O hides the constants.** Two collections with the same cost can have different speeds. Rust says that "where ties occur, Vec is generally going to be faster than VecDeque, and VecDeque is generally going to be faster than LinkedList"[^1]. For Java's `ArrayList`, "The constant factor is low compared to that for the LinkedList implementation"[^13]. When speed matters, measure with real data.
- **Expected and amortized costs are averages.** A hash map is O(1) only on average. Rust says that "It is theoretically possible, though very unlikely, for HashMap to experience significantly worse performance than the expected cost"[^1], and the .NET table gives O(n) as the worst case for `HashSet<T>.Add`[^5]. An add at the end of an array list is O(1) amortized, but one add can copy the whole array when the list grows[^1].
- **Order is not the same in every language.** A Python `dict` keeps the order in which the keys were added: "Dictionaries preserve insertion order"[^14]. A JavaScript `Map` also iterates "in insertion order"[^12]. In Go, "The iteration order over maps is not specified and is not guaranteed to be the same from one iteration to the next"[^9]. Do not depend on the order of a hash map unless the language guarantees it.
- **Other needs change the choice.** Code that more than one thread uses, or code that must not change a collection, needs other types. The .NET table has a separate column for thread-safe and immutable collections, and says that "Often immutable collection types are less performant but provide immutability"[^5].
- **A sorted map is not the only way to get order.** When the code sorts only once, at the end, a hash map and one sort can be enough. The Python, JavaScript and Go examples do this.

## References
