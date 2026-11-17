---
tag: KEJMDR
kind: concept
slug: collection
name: Collection
aliases: [container, collection type, collections library, collections framework, sequence, set, map]
area: data-and-values/collections
level: intro
summary: >-
  A collection is a value that holds other values, its elements. The main kinds are sequences such as lists, sets of unique elements, and maps from keys to values.
applies_to: core-languages
viz: optional
formal: none
see_also: [data-structure, abstract-data-type, collection-interfaces, choose-a-collection, list, set-concept, map-and-dictionary, generic-containers]
same_as:
  wikidata: Q17008256
  wikipedia: Collection_(abstract_data_type)
sources:
- url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/doc-files/coll-overview.html
  title: Collections Framework Overview, Java SE 21
  locator: Introduction; Collection Interfaces; Collection Implementations; Design Goals
- url: https://kotlinlang.org/docs/collections-overview.html
  title: Collections overview, Kotlin documentation
  locator: Whole page
- url: https://en.wikipedia.org/wiki/Collection_(abstract_data_type)
  title: Collection (abstract data type), Wikipedia
  locator: Introduction; Subtypes; Implementation
- url: https://doc.rust-lang.org/std/collections/index.html
  title: Module std::collections, Rust standard library
  locator: Introduction; Performance
- url: https://learn.microsoft.com/en-us/dotnet/standard/collections/
  title: Collections and Data Structures, .NET documentation
  locator: Introduction; Common collection features
- url: https://docs.python.org/3.14/library/collections.abc.html
  title: collections.abc, Python 3.14 documentation
  locator: class collections.abc.Collection
- url: https://en.cppreference.com/w/cpp/container.html
  title: Containers library, C++ reference, cppreference.com
  locator: Introduction
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Map types; For statements with range clause
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Keyed_collections
  title: Keyed collections, MDN Web Docs
  locator: Introduction
- url: https://www.gnu.org/software/bash/manual/html_node/Arrays.html
  title: Arrays, Bash Reference Manual
  locator: 6.7 Arrays
- url: https://docs.python.org/3.14/library/stdtypes.html
  title: Built-in Types, Python 3.14 documentation
  locator: Sequence Types; Set Types; Mapping Types
- url: https://en.wikipedia.org/wiki/Standard_Template_Library
  title: Standard Template Library, Wikipedia
  locator: History
- url: https://en.wikipedia.org/wiki/Java_collections_framework
  title: Java collections framework, Wikipedia
  locator: History
---

## Idea

Programs seldom work on one value at a time. They keep the lines of a file, the users who are logged in, or the price of each product. A collection is a value that holds other values, called its elements. The program can add elements, remove them, look for one, count them and go through them one by one.

Most languages have three main kinds of collection. A sequence keeps its elements in an order, and the program gets each one by its position. A set keeps each element at most once. A map keeps pairs of a key and a value, and the program gets a value by its key.

```python run id=idea
words = ["red", "blue", "red", "green"]  # a sequence: order and duplicates
unique = set(words)                       # a set: each element once
counts = {}                               # a map: key to value
for w in words:
    counts[w] = counts.get(w, 0) + 1

print(len(words), words[1])
print(len(unique), "red" in unique)
print(counts["red"], sorted(counts))
```

```output for=idea
4 blue
3 True
2 ['blue', 'green', 'red']
```

The list keeps all four words in order. The set keeps three, because it drops the second `"red"`. The map counts each word. All three collections support `len`, the `in` test and a loop, but each one answers a different question fast.

## How it works

**A group of elements.** In the Java documentation, "A collection is an object that represents a group of objects"[^1]. Kotlin uses the same words: "Objects in a collection are called elements or items"[^2]. In theory, a collection is "a grouping of items that can be used in a polymorphic way"[^3]: code that counts the elements or goes through them works for every kind of collection.

**The main kinds.** Wikipedia divides collections into linear and associative kinds. A linear collection keeps an order, and "Some collections are interpreted as a sort of function: given an input, the collection yields an output"[^3]. Kotlin names three main types. "List is an ordered collection"[^2]; "Set is a collection of unique elements"[^2]; and a "Map (or dictionary) is a set of key-value pairs"[^2]. Rust puts its standard collections into four groups: sequences (`Vec`, `VecDeque`, `LinkedList`), maps (`HashMap`, `BTreeMap`), sets (`HashSet`, `BTreeSet`) and one other, `BinaryHeap`[^4]. Stacks, queues and priority queues are linear collections with a fixed rule for which element comes out next. See [list](/list/), [set](/set-concept/) and [map](/map-and-dictionary/).

**The common operations.** In .NET, "All collections provide methods for adding, removing, or finding items in the collection"[^5]. Nearly all collections also know their size and can give their elements one at a time. Python names this common part: `Collection` is the "ABC for sized iterable container classes"[^6], so a collection has `len`, a loop and the `in` test. A program that uses only these operations works with any collection. See [iterators](/iterators/) and [collection interfaces](/collection-interfaces/).

**The kind is not the storage.** A collection says what the program can do with its elements. A [data structure](/data-structure/) says how the elements are stored. One kind can have many data structures: "a priority queue is often implemented as a heap, while an associative array is often implemented as a hash table"[^3]. The collection is an [abstract data type](/abstract-data-type/), and each library class is one implementation of it.

**Collection libraries.** Each language puts its collections in a library, or builds some of them into the language. Java has "a unified architecture for representing and manipulating collections"[^1] in `java.util`. In C++, "The Containers library is a generic collection of class templates and algorithms that allow programmers to easily implement common data structures like queues, lists and stacks"[^7]. Go builds slices and maps into the language: "A map is an unordered group of elements of one type, called the element type, indexed by a set of unique keys of another type, called the key type"[^8]. JavaScript has arrays and the keyed collections, where "Map and Set objects contain elements which are iterable in the order of insertion"[^9]. "Bash provides one-dimensional indexed and associative array variables"[^10].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | built-in `list`, `tuple`, `set`, `dict`; more in `collections`; `collections.abc` for the interfaces |
| JavaScript | native | `Array`, `Map`, `Set`, typed arrays; `WeakMap` and `WeakSet` |
| TypeScript | native | the JavaScript collections with item types, such as `string[]` and `Map<string, number>` |
| Java | stdlib | the collections framework in `java.util`: `List`, `Set`, `Map`, `Queue`, `Deque`; arrays are separate |
| Kotlin | stdlib | `List`, `Set`, `Map` and their mutable forms in `kotlin.collections` |
| C# | stdlib | `System.Collections.Generic`: `List<T>`, `HashSet<T>`, `Dictionary<TKey,TValue>`, `Queue<T>` |
| Go | native | built-in arrays, slices and maps; `container/list`, `container/heap` and `container/ring` in the library |
| Rust | stdlib | `std::collections`: `Vec`, `VecDeque`, `HashMap`, `BTreeMap`, `HashSet`, `BinaryHeap` |
| C | idiom | built-in fixed arrays; the program writes other collections with structs and pointers |
| C++ | stdlib | the containers library: `std::vector`, `std::list`, `std::set`, `std::map`, `std::unordered_map` |
| Bash | native | indexed arrays and associative arrays of strings |
| PowerShell | native | arrays and hash tables, and the .NET collections |
| SQL | n/a | a table holds rows, but SQL has no collection values in program code |
| Regex | n/a | a pattern does not hold values |
| HTML | n/a | HTML has no collection values |
| CSS | n/a | CSS has no collection values |

Python, JavaScript, Go and Bash build their main collections into the language, with a literal syntax for each. Java, C#, Rust and C++ put them in the standard library, as generic types. C has only fixed arrays, so a C program writes its own list or map, or uses a library.

The example files do the same work in each language: keep a list of words, make a set of the unique words, and count each word in a map. Python uses `list`, `set` and `dict`. JavaScript and TypeScript use `Array`, `Set` and `Map`. Java uses `ArrayList`, `TreeSet` and `TreeMap`. C# uses `List<T>`, `SortedSet<T>` and `SortedDictionary<TKey,TValue>`. Go uses a slice and a map, and sorts the keys to print them. Rust uses `Vec`, `BTreeSet` and `BTreeMap`. C uses an array and a small table that it searches by hand. C++ uses `std::vector`, `std::set` and `std::map`. Bash uses an indexed array and an associative array.

## Variations and related ideas

- **Mutable and read-only.** "Each main collection type has a read-only interface, such as List, and a corresponding mutable interface, such as MutableList"[^2] in Kotlin. Java uses the terms unmodifiable and immutable: "Collections that do not support modification operations (such as add, remove and clear) are referred to as unmodifiable"[^1]. See [immutability](/immutability/).
- **Generic and non-generic.** An older library stores elements of the most general type. In .NET, "Non-generic collections store items as Object, require casting"[^5]. Generic collections take the element type as a parameter. See [generic containers](/generic-containers/).
- **Arrays.** In some languages the array is not one of the library collections. In Kotlin, "Arrays aren't a type of collection"[^2]. Java arrays "cannot be made to implement the Collection interface directly without changing the language"[^1], so the framework has methods to change one into the other. See [array](/array/).
- **Concurrent collections.** Some libraries have collections that many threads can use at once, such as `ConcurrentHashMap` in Java and `System.Collections.Concurrent` in .NET[^1][^5].

## Pitfalls

- **Order that is not part of the contract.** A set or a hash map may give its elements in any order. In Go, "The iteration order over maps is not specified and is not guaranteed to be the same from one iteration to the next"[^8]. Python dicts keep the order in which keys were added: "Dictionaries preserve insertion order"[^11]. A Python set has no order: it is "an unordered collection of distinct hashable objects"[^11]. Sort the elements, or use an ordered collection, when the output must be in an order.
- **A map is not always a collection.** In Java, the map interfaces "are not true collections"[^1], so a `Map` cannot go where the code expects a `Collection`. Use `keySet()`, `values()` or `entrySet()` to get a collection view.
- **Operations that are not supported.** In Java, "Many of the modification methods in the collection interfaces are labeled optional"[^1]. A call to `add` on an unmodifiable list throws `UnsupportedOperationException` when the program runs.
- **Changing a collection during a loop.** A loop that adds or removes elements of the collection that it goes through can skip elements or fail. The general-purpose Java collections "have fail-fast iterators, which detect invalid concurrent modification"[^1]. Collect the changes in a second collection, or loop over a copy.
- **The name of the implementation.** Programmers often call a priority queue a heap and a map a hash, but "this is incorrect conceptually"[^3]. Choose the kind of collection by what the program does with it, and then choose the implementation by its cost.

## Cost

Each kind of collection makes some operations fast and others slow. The Rust documentation gives a table: getting the element at an index is O(1) for a `Vec` and O(min(i, n-i)) for a `LinkedList`, and a lookup in a `BTreeMap` is O(log(n))[^4]. A collection that grows must sometimes move its elements. In .NET, "if Count is less than Capacity, adding an item is an O(1) operation"[^5]; when the capacity must grow, "The memory is reallocated, and the elements are copied from the old collection to the new one"[^5]. See [choose a collection](/choose-a-collection/).

## History

The C++ containers come from the Standard Template Library (STL). "In November 1993 Alexander Stepanov presented a library based on generic programming to the ANSI/ISO committee for C++ standardization"[^12], and "A proposal received final approval at the July 1994 ANSI/ISO committee meeting"[^12]. In Java, "The collections framework was designed and developed primarily by Joshua Bloch, and was introduced in JDK 1.2"[^13]. The older classes `Vector` and `Hashtable` "were retrofitted to implement the collection interfaces"[^1].
