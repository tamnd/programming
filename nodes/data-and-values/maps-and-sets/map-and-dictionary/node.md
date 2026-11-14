---
tag: S42X6B
kind: concept
slug: map-and-dictionary
name: Map and dictionary
aliases: [map, dictionary, associative array, hash map, symbol table, key-value store, dict, Map, HashMap, "Dictionary<TKey,TValue>", unordered_map, "declare -A"]
area: data-and-values/maps-and-sets
level: intro
summary: >-
  A map, or dictionary, stores pairs of a key and a value, with each key at most once. A program uses the key to find, change or remove its value.
applies_to: core-languages
viz: optional
formal: none
see_also: [hash-table, set-concept, ordered-map, multimap, hash-codes, value-equality-and-identity, group-items-by-a-key, json]
sources:
- url: https://en.wikipedia.org/wiki/Associative_array
  title: Associative array, Wikipedia
  locator: Introduction; Operations; Implementation; Ordered dictionary; Language support
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, The Python Standard Library
  locator: Mapping Types, dict
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.dictionary-2.item
  title: Dictionary<TKey,TValue>.Item[TKey] property, .NET API reference
  locator: Remarks
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Map.html
  title: Interface Map, Java SE 25 API
  locator: Description; get
- url: https://go.dev/blog/maps
  title: Andrew Gerrand, Go maps in action, The Go Blog, 2013
  locator: Declaration and initialization; Working with maps; Iteration order
- url: https://en.cppreference.com/cpp/container/unordered_map/operator_at
  title: std::unordered_map::operator[], cppreference.com
  locator: Description
- url: https://docs.python.org/3/glossary.html
  title: Glossary, Python documentation
  locator: hashable
- url: https://doc.rust-lang.org/std/collections/struct.HashMap.html
  title: Struct HashMap, The Rust Standard Library
  locator: Description; insert; keys
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Map types
- url: https://en.cppreference.com/cpp/container/unordered_map
  title: std::unordered_map, cppreference.com
  locator: Description
- url: https://www.gnu.org/software/gawk/manual/html_node/Array-Intro.html
  title: Introduction to Arrays, The GNU Awk User's Guide
  locator: Full text
- url: https://tiswww.case.edu/php/chet/bash/bashref.html
  title: Bash Reference Manual
  locator: Arrays
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/hcreate.html
  title: hcreate, hdestroy, hsearch, The Open Group Base Specifications Issue 8
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Map
  title: Map, MDN Web Docs
  locator: Description; Key equality; Objects vs. Maps
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Property_accessors
  title: Property accessors, MDN Web Docs
  locator: Property names
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.dictionary-2
  title: Dictionary<TKey,TValue> class, .NET API reference
  locator: Definition; Remarks
---

## Idea

A map connects keys to values. A phone book is a map: the key is a name and the value is a number. Wikipedia defines it as "an abstract data type that stores a collection of key/value pairs, such that each possible key appears at most once in the collection"[^1]. Other names for the same idea are associative array, dictionary, hash map, symbol table and key-value store. Python calls it a dict, Java and Go call it a map, and C# calls it a Dictionary.

An array finds an item by its position. A map finds a value by its key, and the key can be a string, a number or another value that the map can compare.

```python run id=idea
# A dict maps each key to one value.
ages = {"ada": 36, "alan": 41}
ages["grace"] = 85  # insert a new key
ages["ada"] = 37  # update: the key "ada" is still there only once
print(ages["alan"], len(ages), "ada" in ages, "bob" in ages)

# A missing key: d[key] raises KeyError, and get gives a default.
print(ages.get("bob"), ages.get("bob", 0))
try:
    ages["bob"]
except KeyError as e:
    print("KeyError", e)

del ages["alan"]
print(ages)
```

```output for=idea
41 3 True False
None 0
KeyError 'bob'
{'ada': 37, 'grace': 85}
```

The second write to `"ada"` does not add a pair. It replaces the value, and the map still has three keys. A read of the key `"bob"`, which is not in the map, is an error with `d[key]` and gives a default with `get`.

## How it works

**The operations.** Wikipedia lists the operations of a map[^1]:

- insert or put: "add a new (key,value) pair to the collection, mapping the key to its new value. Any existing mapping is overwritten"[^1];
- remove or delete: "remove a (key,value) pair from the collection, unmapping a given key from its value"[^1];
- lookup, find or get: "find the value (if any) that is bound to a given key"[^1].

Most map types also give the number of pairs, a test for a key, and a loop over the keys, the values or the pairs.

**A missing key.** The languages do not agree on a read of a key that is not in the map. This is the most important difference between the map types:

- an error. Python `d[key]` "Raises a KeyError if key is not in the map"[^2]. In C#, "If the specified key is not found, a get operation throws a KeyNotFoundException, and a set operation creates a new element with the specified key"[^3]. Rust `m[key]` panics;
- a null value. Java `get` "Returns the value to which the specified key is mapped, or null if this map contains no mapping for the key"[^4]. JavaScript `Map.prototype.get` gives `undefined`;
- the zero value. In Go, "If the requested key doesn’t exist, we get the value type’s zero value"[^5];
- an optional value. Rust `get` gives `None` for a missing key, and Python `get` gives a default: "Return the value for key if key is in the dictionary, else default"[^2];
- an insert. C++ `operator[]` returns a reference to the value "performing an insertion if such key does not already exist"[^6].

**Rules for keys.** A map must be able to tell whether two keys are the same key. A hash map, which is the most common kind, also needs a hash code for each key. Python says "A mapping object maps hashable values to arbitrary objects"[^2], and "Hashable objects which compare equal must have the same hash value"[^7]. Rust states the same rule as "k1 == k2 -> hash(k1) == hash(k2)"[^8]. Go requires that "The comparison operators == and != must be fully defined for operands of the key type; thus the key type must not be a function, map, or slice"[^9]. So a list is not a key in Python, and a slice is not a key in Go. See [hash codes](/hash-codes/) and [value equality and identity](/value-equality-and-identity/).

**How a map stores its pairs.** "The two major solutions to the dictionary problem are hash tables and search trees"[^1]. A [hash table](/hash-table/) uses the hash code of a key to find a bucket, so a lookup takes constant time on average. C++ `std::unordered_map` is an example: "Search, insertion, and removal of elements have average constant-time complexity. Internally, the elements are not sorted in any particular order, but organized into buckets"[^10]. A search tree keeps the keys sorted and takes logarithmic time. For a few pairs, a list of pairs with a linear search is enough: Wikipedia notes that this "is easy to implement and the constant factors in its running time are small"[^1].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `dict` with the literal `{k: v}`; `d[k]` raises `KeyError`, `get` and `setdefault` give defaults; insertion order |
| JavaScript | native | `Map` for any key type, compared with SameValueZero; a plain object with string and symbol keys |
| TypeScript | native | the JavaScript `Map<K, V>` and the object type `Record<K, V>` |
| Java | stdlib | the interface `Map` with `HashMap`, `LinkedHashMap` and `TreeMap`; `get` returns `null` for a missing key |
| Kotlin | stdlib | `mapOf` and `mutableMapOf`, a `LinkedHashMap` by default; `m[k]` gives `null` for a missing key |
| C# | stdlib | `Dictionary<TKey,TValue>`; the indexer throws `KeyNotFoundException`, `TryGetValue` does not |
| Go | native | `map[K]V`; a missing key gives the zero value, and `v, ok := m[k]` tells the cases apart |
| Rust | stdlib | `HashMap<K, V>` and `BTreeMap<K, V>`; `get` returns an `Option`, the entry API inserts or updates |
| C | idiom | no map type; a hand-written hash table, a library, or POSIX `hsearch` |
| C++ | stdlib | `std::unordered_map` (hash) and `std::map` (tree); `operator[]` inserts a missing key |
| Bash | native | associative arrays with `declare -A`, with string keys and string values |
| PowerShell | native | the hash table literal `@{}` and `[ordered]@{}` for insertion order |
| SQL | idiom | a table with a primary key; `json` and `hstore` values in PostgreSQL |
| Regex | n/a | patterns match text; named groups give a fixed set of names, not a map |
| HTML | n/a | markup; attributes on an element are names and values, but not a data structure |
| CSS | n/a | style rules have no maps; custom properties are names with values in a cascade |

Many scripting languages made the map a built-in type. "Built-in syntactic support for associative arrays was introduced in 1969 by SNOBOL4"[^1], which called a map a table. AWK arrays are maps: in AWK, "any number or string, not just consecutive integers, may be used as an array index"[^11]. In Bash, "Associative arrays are created using declare -A name"[^12]. C has no map type. POSIX gives `hcreate`, `hsearch` and `hdestroy`, which "shall manage hash search tables"[^13], but `hsearch` takes no table argument, so a program can use only one such table at a time.

JavaScript has two kinds of map. "The Map object holds key-value pairs and remembers the original insertion order of the keys. Any value (both objects and primitive values) may be used as either a key or a value"[^14]. A plain object is also used as a map, but "The keys of an Object must be either a String or a Symbol"[^14]. For property names, "Any other value, including a number, is coerced to a string"[^15]. So `obj[1]` and `obj['1']` are the same entry, and a `Map` keeps them apart.

The example files show these forms. Python uses `get`, `setdefault` and `pop`, a tuple as a key, a list as a key (a `TypeError`), the keys `1`, `1.0` and `True` that are one key, and a change during a loop (a `RuntimeError`). JavaScript compares a `Map` with an object: number keys, `NaN` as a key, and an object that has a key from its prototype. Java shows `put` and its old value, the `null` problem and `Map.of`. C# shows the indexer, `TryGetValue`, `Add` with a key that is there, and `GetValueOrDefault`. Go shows the zero value, `v, ok`, a nil map and a struct key. Rust shows `insert`, `get`, `remove` and `entry`. C keeps an array of pairs. C++ shows how `[]` inserts a key and how `find`, `contains` and `at` do not.

## Variations and related ideas

- **Ordered maps.** A tree map, such as Java `TreeMap`, Rust `BTreeMap` or C++ `std::map`, keeps its keys sorted. It can find the first key after a given key. See [ordered map](/ordered-map/).
- **Insertion order.** Some maps keep the order in which the keys were added. In Python, "Dictionaries preserve insertion order. Note that updating a key does not affect the order. Keys added after deletion are inserted at the end"[^2]. Java `LinkedHashMap` and JavaScript `Map` do the same. See [insertion-ordered map](/insertion-ordered-map/).
- **Sets.** A set is a map with keys and no values. It answers whether an item is present. See [set](/set-concept/).
- **Multimaps.** A multimap can have more than one value for a key. A map of lists does the same job. See [multimap](/multimap/).
- **Maps with defaults.** Python `defaultdict`, Java `getOrDefault` and the Rust entry API make the counting and grouping of items short. See [count word frequencies](/count-word-frequencies/) and [group items by a key](/group-items-by-a-key/).
- **Immutable maps.** Java `Map.of` and Python `types.MappingProxyType` give maps that a program cannot change. See [immutability](/immutability/).
- **Records and JSON objects.** A JSON object is a map with string keys. A record or a struct has a fixed set of field names that the compiler knows. A map has keys that the program adds at run time. See [JSON](/json/).

## Pitfalls

- **A read that inserts.** In C++, `m[k]` on a missing key inserts `k` with a value of 0. A test such as `if (m[k] == 0)` makes the map larger. Use `find`, `contains` or `count` to test for a key.
- **Null or missing.** In Java, "a return value of null does not necessarily indicate that the map contains no mapping for the key; it's also possible that the map explicitly maps the key to null"[^4]. Use `containsKey`. A missing value in a `Map<String, Integer>` also throws `NullPointerException` when the program unboxes it to `int`.
- **A zero that is not there.** In Go, `m["w"]` gives 0 for a missing key and for a key with the value 0. Use the two-value form `v, ok := m["w"]`.
- **Keys that become strings.** A JavaScript object turns each key into a string, so the keys `1` and `'1'` collide. An object also has keys from its prototype, such as `toString`. MDN warns that "An Object has a prototype, so it contains default keys that could collide with your own keys if you're not careful"[^14]. Use a `Map`.
- **A key that changes.** A hash map stores a key by its hash code. If the key changes after the insert, the map looks in the wrong bucket and does not find it. Java says "The behavior of a map is not specified if the value of an object is changed in a manner that affects equals comparisons while the object is a key in the map"[^4]. C# says that a key "must not change in any way that affects its hash value"[^16]. Use immutable values as keys.
- **Equal keys of different types.** In Python, `1`, `1.0` and `True` are equal and have the same hash, so they are one key. After `d = {1: "int"}`, `d[1.0] = "float"` and `d[True] = "bool"`, the dict is `{1: 'bool'}`. The first key stays and the last value wins.
- **The nil map in Go.** "A nil map behaves like an empty map when reading, but attempts to write to a nil map will cause a runtime panic"[^5]. Make the map with `make` or a literal before the first write.
- **Order.** A hash map has no defined order. In Go, "the iteration order is not specified and is not guaranteed to be the same from one iteration to the next"[^5]. Rust `keys` visits "all keys in arbitrary order"[^8]. A test that compares printed maps can fail. Sort the keys before the program prints them.
- **A change during a loop.** In Python, "Iterating views while adding or deleting entries in the dictionary may raise a RuntimeError or fail to iterate over all entries"[^2]. Java usually throws `ConcurrentModificationException`, and C# throws `InvalidOperationException` when a loop adds a key. Collect the changes in a list and apply them after the loop.

## Cost

| Operation | Hash map | Tree map | List of pairs |
|---|---|---|---|
| lookup | constant on average, linear in the worst case | logarithmic | linear |
| insert | constant on average (amortized), linear in the worst case | logarithmic | linear (constant with no test for a duplicate) |
| remove | constant on average, linear in the worst case | logarithmic | linear |
| loop over all pairs | linear, in no defined order | linear, in key order | linear, in list order |
| memory | the pairs and empty buckets | the pairs and two links for each node | the pairs |

The numbers follow the comparison table of Wikipedia[^1]. A hash map is linear in the worst case when many keys have the same hash code. Most languages use a hash map as their default map. .NET says that a lookup is "very fast, close to O(1), because the Dictionary<TKey,TValue> class is implemented as a hash table"[^16]. A hash map also spends time to compute the hash code of each key, so a short list of pairs can be faster for a few keys.

## History

SNOBOL4 had tables in 1969, and the scripting languages made maps a part of the language. Wikipedia lists the languages with built-in maps as "Most modern scripting languages, starting with AWK"[^1], and then Rexx, Perl, PHP, Tcl and JavaScript. Perl calls them hashes, and Python calls them dicts. Compiled languages put maps in the standard library: Java `HashMap` and C++ `std::map`, and later C++ `std::unordered_map` in C++11. Go made `map` a built-in type.

The order of a map changed over time. Python made insertion order a rule in version 3.7: "Dictionary order is guaranteed to be insertion order. This behavior was an implementation detail of CPython from 3.6"[^2]. Go went the other way. The order of a loop over a map is not specified, and it can change from one loop to the next[^5]. Rust uses a hash function with a random seed: "The algorithm is randomly seeded"[^8].

## References
