---
tag: C7HPNH
kind: concept
slug: insertion-ordered-map
name: Insertion-ordered map
aliases: [ordered dictionary, linked hash map, LinkedHashMap, OrderedDict, OrderedDictionary, index map, IndexMap]
area: data-and-values/maps-and-sets
level: core
summary: >-
  An insertion-ordered map gives its keys back in the order in which they were added. Python dict, JavaScript Map and Java LinkedHashMap work this way, and Go and C++ maps do not.
applies_to: core-languages
viz: optional
formal: none
see_also: [map-and-dictionary, hash-table, ordered-map, linked-list, iterators, json, group-items-by-a-key]
sources:
- url: https://en.wikipedia.org/wiki/Associative_array
  title: Associative array, Wikipedia
  locator: Ordered dictionary
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, The Python Standard Library
  locator: Mapping types, dict
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/LinkedHashMap.html
  title: Class LinkedHashMap, Java SE 25 API
  locator: Description
- url: https://docs.python.org/3/whatsnew/3.7.html
  title: What's New In Python 3.7
  locator: Summary, release highlights
- url: https://docs.python.org/3/library/collections.html
  title: collections, Container datatypes, The Python Standard Library
  locator: OrderedDict objects
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Map
  title: Map, MDN Web Docs
  locator: Description, Objects vs. Maps, Key Order
- url: https://go.dev/blog/maps
  title: Go maps in action, The Go Blog
  locator: Iteration order
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.dictionary-2
  title: Dictionary<TKey,TValue> class, .NET API reference
  locator: Remarks
- url: https://doc.rust-lang.org/std/collections/struct.HashMap.html
  title: Struct HashMap, The Rust Standard Library
  locator: keys, iter
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.ordereddictionary-2
  title: OrderedDictionary<TKey,TValue> class, .NET API reference
  locator: Definition
- url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_hash_tables
  title: about_Hash_Tables, PowerShell documentation
  locator: Long description; Syntax
- url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.collections/mutable-map-of.html
  title: mutableMapOf, Kotlin Standard Library API
  locator: Description
- url: https://www.php.net/manual/en/language.types.array.php
  title: Arrays, PHP Manual
  locator: Introduction
---

## Idea

A [hash table](/hash-table/) puts each key in a bucket chosen by its hash code, so a loop over the table visits the keys in an order that looks random. An insertion-ordered map remembers a second thing: the order in which the keys were added. A loop gives the oldest key first and the newest key last. This makes the output of a program stable and easy to read, and it keeps the order of fields when a program reads and writes JSON or a configuration file.

Wikipedia names two kinds of ordered dictionary. In one, the keys are sorted, which is an [ordered map](/ordered-map/). In the other, "The order of enumeration is key-independent and is instead based on the order of insertion"[^1]. Wikipedia gives the .NET Framework ordered dictionary, the Java `LinkedHashMap` and Python as examples. In Python every dict works this way: "Dictionaries preserve insertion order. Note that updating a key does not affect the order. Keys added after deletion are inserted at the end."[^2]

```python run id=idea
# A Python dict remembers the order in which its keys were added.
stock = {}
stock["pear"] = 4
stock["apple"] = 7
stock["fig"] = 2
print(list(stock))

# Changing the value of a key does not move the key.
stock["pear"] = 9
print(list(stock.items()))

# A key that is removed and added again goes to the end.
del stock["pear"]
stock["pear"] = 1
print(list(stock))

# The first and the last key, and the keys in reverse order.
print(next(iter(stock)), next(reversed(stock)), list(reversed(stock)))
```

```output for=idea
['pear', 'apple', 'fig']
[('pear', 9), ('apple', 7), ('fig', 2)]
['apple', 'fig', 'pear']
apple pear ['pear', 'fig', 'apple']
```

The keys come back as pear, apple, fig, which is the order of the code, not the order of the alphabet. The update of `pear` keeps its place. The delete and the new insert move it to the end.

## How it works

**A hash table and a linked list.** Java `LinkedHashMap` is a "Hash table and linked list implementation of the Map interface, with well-defined encounter order"[^3]. Each entry is in a bucket for fast lookup, and each entry is also on a doubly linked list: "This linked list defines the encounter order (the order of iteration), which is normally the order in which keys were inserted into the map"[^3]. An insert appends the entry to the end of the list. A delete unlinks it from the list in constant time, because the entry knows its neighbors.

**An array of entries and a table of indexes.** CPython and the Rust crate indexmap keep the entries in an array in insertion order. A separate hash table maps each key to its position in the array. A loop reads the array from start to end. A delete must leave a hole in the array, or move the later entries down, or move the last entry into the hole. The last choice is fast but changes the order. The example files show this.

**Rules for updates and deletes.** The languages agree on two rules. First, a new value for a key that is already in the map does not move the key. Java says that the "encounter order is not affected if a key is re-inserted into the map with the put method"[^3]. Second, a key that is removed and then added again is a new key, so it goes to the end. To move a key to the end on purpose, remove it and add it again, or use a method such as Python `OrderedDict.move_to_end`.

**Access order.** A variant moves a key to the end each time a program reads it. Then the first key is the one that was not used for the longest time. Java `LinkedHashMap` has a constructor for this, and "This kind of map is well-suited to building LRU caches"[^3]. The method `removeEldestEntry` can remove the oldest entry after each insert, which keeps the cache at a fixed size.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | every `dict`; `collections.OrderedDict` adds `move_to_end` and order-aware equality |
| JavaScript | native | `Map` iterates in insertion order; a plain object puts index-like keys first |
| TypeScript | native | the JavaScript `Map`, typed as `Map<K, V>` |
| Java | stdlib | `LinkedHashMap`, a `SequencedMap` since Java 21; `HashMap` has no order |
| Kotlin | stdlib | `mutableMapOf` and `mapOf` keep the order; on the JVM they are a `LinkedHashMap` |
| C# | stdlib | `OrderedDictionary<TKey,TValue>` since .NET 9; `Dictionary` has no defined order |
| Go | idiom | a map has no order; keep a slice of keys next to the map |
| Rust | idiom | `HashMap` has no order; a `Vec` of entries with an index, or the indexmap crate |
| C | idiom | a hash table whose entries are also on a linked list |
| C++ | idiom | `std::unordered_map` has no order; a `std::list` with a map of iterators into it |
| Bash | idiom | an indexed array of keys next to an associative array |
| PowerShell | stdlib | `[ordered]@{...}` makes an `OrderedDictionary`; `@{...}` has no fixed order |
| SQL | idiom | rows have no order without `ORDER BY`; an identity column records the insert order |
| Regex | n/a | patterns match text and have no maps |
| HTML | n/a | markup with no data structures |
| CSS | n/a | style rules with no data structures |

Python made the order a rule of the language in 3.7: "Dictionary order is guaranteed to be insertion order. This behavior was an implementation detail of CPython from 3.6."[^2] The release notes say that "the insertion-order preservation nature of dict objects has been declared to be an official part of the Python language spec"[^4]. `OrderedDict` is still in the standard library. Its documentation says "The regular dict was designed to be very good at mapping operations. Tracking insertion order was secondary."[^5] and "The OrderedDict was designed to be good at reordering operations."[^5]

JavaScript has two kinds of map with different rules. MDN says that "The Map object holds key-value pairs and remembers the original insertion order of the keys"[^6], and that "A Map object iterates entries, keys, and values in the order of entry insertion"[^6]. A plain object also has an order, but MDN warns: "Although the keys of an ordinary Object are ordered now, this was not always the case, and the order is complex. As a result, it's best not to rely on property order."[^6]

Go, C# and Rust make no promise for their main map type. The Go blog says that with a range loop "the iteration order is not specified and is not guaranteed to be the same from one iteration to the next"[^7], and that a program that needs a stable order "must maintain a separate data structure that specifies that order"[^7]. For the C# `Dictionary`, "The order in which the items are returned is undefined"[^8]. Rust `HashMap` gives "An iterator visiting all keys in arbitrary order"[^9]. C# added `OrderedDictionary<TKey,TValue>` in .NET 9. It "Represents a collection of key/value pairs that are accessible by the key or index"[^10].

PowerShell has a short form for the ordered type. "Beginning in PowerShell 3.0, you can use the [ordered] type accelerator to create an [System.Collections.Specialized.OrderedDictionary] object in PowerShell. Ordered dictionaries differ from hashtables in that the keys always appear in the order in which you list them. The order of keys in a hashtable isn't deterministic."[^11] Kotlin `mutableMapOf` "Returns an empty new MutableMap. The returned map preserves the entry iteration order."[^12] In PHP, every array keeps insertion order: "An array in PHP is actually an ordered map."[^13]

The example files show these forms. Python compares `dict` and `OrderedDict` equality and builds a small LRU cache with `move_to_end`. JavaScript shows the order of a `Map`, an LRU cache with `delete` and `set`, and the different key order of a plain object. Java uses `LinkedHashMap`, the Java 21 `SequencedMap` methods and an access-order LRU cache. C# uses the index methods of `OrderedDictionary<TKey,TValue>`. Go keeps a slice of keys. Rust builds a small index map and compares `swap_remove` with `shift_remove`. C links the entries of a hash table on a list. C++ builds an LRU cache from `std::list` and `std::unordered_map`.

## Variations and related ideas

- **Sorted order.** An [ordered map](/ordered-map/) keeps the keys in sorted order, not in insertion order. It costs logarithmic time for each operation. An insertion-ordered map keeps the constant average time of a hash table.
- **LRU caches.** An access-order map is the usual base for a cache that drops the least recently used entry. Python `functools.lru_cache` and many web caches work this way.
- **Insertion-ordered sets.** Java `LinkedHashSet` and JavaScript `Set` keep insertion order for a [set](/set-concept/). In Python, `dict.fromkeys(items)` removes duplicates from a list and keeps the first order of the items. See [remove duplicates from a list](/remove-duplicates-from-a-list/).
- **JSON objects.** The JSON format does not require a parser to keep the order of the members of an object. Parsers that read into an insertion-ordered map, such as the Python `json` module and JavaScript `JSON.parse`, keep the order of the text, so a program that reads and writes a file keeps the order of its fields. See [JSON](/json/).
- **Index access.** Some ordered maps also let a program read the entry at a position, for example C# `OrderedDictionary<TKey,TValue>.GetAt` and the indexmap crate. Python `dict` and Java `LinkedHashMap` do not.

## Pitfalls

- **Relying on an order that is not promised.** A C# `Dictionary` often gives the keys in insertion order when no key was removed, but the documentation says the order is undefined[^8]. A program that depends on the order can break after a delete or in another version. Go goes further and makes the order of a range loop differ from one loop to the next[^7].
- **Equality ignores the order.** Two Python dicts with the same pairs in a different order are equal. Two `OrderedDict` objects are equal only when the order is also the same. In Java, a `LinkedHashMap` equals a `HashMap` with the same pairs.
- **An update does not move the key.** A program that wants the most recent key last must remove the key and add it again, or use an access-order map.
- **Plain objects in JavaScript.** An object puts keys such as `"10"` and `"2"` before all other keys, in number order. Use a `Map` when the order of the keys matters.
- **Changes during a loop.** Python raises `RuntimeError` when a dict changes size during a loop over it, and Java iterators throw `ConcurrentModificationException`. In an access-order `LinkedHashMap`, even a `get` counts as a change, because it moves the entry.
- **Deletes that change the order.** A fast remove in an array-based map moves the last entry into the hole. The indexmap crate calls this `swap_remove`. Use the slower `shift_remove` to keep the order.

## Cost

| Operation | Hash table with a linked list (`LinkedHashMap`) | Array of entries with an index (`dict`, indexmap) | Map with a separate key list (Go idiom) |
|---|---|---|---|
| lookup | constant on average | constant on average | constant on average |
| insert a new key | constant on average | constant on average (amortized) | constant on average (amortized) |
| update a value | constant on average | constant on average | constant on average |
| delete, keep the order | constant on average | constant with a hole, or linear with a shift | linear |
| loop in insertion order | linear in the size | linear in the size and the holes | linear in the size |
| first and last key | constant | constant, after the holes are skipped | constant |
| entry at a position | linear | constant when there are no holes | constant |
| extra memory | two links for each entry | one index for each slot | one key for each entry |

Java says that the performance of `LinkedHashMap` "is likely to be just slightly below that of HashMap, due to the added expense of maintaining the linked list, with one exception: Iteration over the collection-views of a LinkedHashMap requires time proportional to the size of the map, regardless of its capacity."[^3] A loop over a plain hash table must visit the empty buckets too.

## History

PHP arrays have kept insertion order from the start, and PHP calls its array "actually an ordered map"[^13]. Java added `LinkedHashMap` in version 1.4 in 2002. The .NET Framework 2.0 had a non-generic `OrderedDictionary`. Ruby hashes keep insertion order since Ruby 1.9. Python added `collections.OrderedDict` in 2.7 and 3.1. In CPython 3.6, a new compact dict kept the order as a side effect, and Python 3.7 made the order part of the language[^4]. JavaScript added `Map` with insertion order in ECMAScript 2015, and ECMAScript 2020 completed the rules for the key order of plain objects[^6]. PowerShell 3.0 added `[ordered]`[^11], Java 21 added `SequencedMap` in 2023, and .NET 9 added the generic `OrderedDictionary<TKey,TValue>` in 2024.

## References
