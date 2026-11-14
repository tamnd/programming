---
tag: Q9BDY4
kind: concept
slug: set-concept
name: Set
aliases: [set, hash set, sorted set, frozenset, HashSet, TreeSet, Set, "HashSet<T>", unordered_set, BTreeSet, set type, finite set]
area: data-and-values/maps-and-sets
level: intro
summary: >-
  A set holds distinct values with no order. Its main operation is a test of whether a value is in the set, and it also gives union, intersection and difference.
applies_to: core-languages
viz: optional
formal: none
see_also: [map-and-dictionary, hash-table, multiset, ordered-map, hash-codes, value-equality-and-identity, remove-duplicates-from-a-list, immutability]
sources:
- url: https://en.wikipedia.org/wiki/Set_(abstract_data_type)
  title: Set (abstract data type), Wikipedia
  locator: Introduction; Operations; Implementations; Language support
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Set.html
  title: Interface Set, Java SE 25 API
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/HashSet.html
  title: Class HashSet, Java SE 25 API
  locator: Description
- url: https://doc.rust-lang.org/std/collections/struct.HashSet.html
  title: Struct HashSet, The Rust Standard Library
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Set
  title: Set, MDN Web Docs
  locator: Description; Set composition
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, The Python Standard Library
  locator: Set Types, set, frozenset
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.hashset-1
  title: HashSet<T> class, .NET API reference
  locator: Remarks
- url: https://en.cppreference.com/cpp/container/set
  title: std::set, cppreference.com
  locator: Description
- url: https://en.cppreference.com/cpp/container/unordered_set
  title: std::unordered_set, cppreference.com
  locator: Description
- url: https://go.dev/blog/maps
  title: Andrew Gerrand, Go maps in action, The Go Blog, 2013
  locator: Exploiting zero values
---

## Idea

A set is a collection of values in which each value occurs once. Wikipedia defines it as "an abstract data type that can store distinct values, without any particular order"[^1]. A list answers the question: what is at position 3? A set answers a different question: is this value in the collection? In the words of Wikipedia, "rather than retrieving a specific element from a set, one typically tests a value for membership in a set"[^1].

A program uses a set to remember which items it has seen, to remove duplicates, and to compare two groups of items: which items are in both, and which are in only one.

```python run id=idea
# A set keeps one copy of each value, and it has no order.
s = {3, 1, 3, 2, 1}
print(len(s), s == {1, 2, 3}, 2 in s, 5 in s)

# add ignores a value that is there already.
s.add(2)
s.add(4)
print(sorted(s))

# The operations of set algebra.
a, b = {1, 2, 3}, {2, 3, 4}
print(sorted(a | b), sorted(a & b), sorted(a - b), sorted(a ^ b))
print({2, 3} <= a, a.isdisjoint({7, 8}))
```

```output for=idea
3 True True False
[1, 2, 3, 4]
[1, 2, 3, 4] [2, 3] [1] [1, 4]
True True
```

The literal `{3, 1, 3, 2, 1}` has five items but makes a set of three values. Two sets are equal when they have the same values, in any order. The example prints sorted lists because a set has no order of its own.

## How it works

**Equal values are one value.** A set decides that two values are the same with the equality of the language. Java says a set contains "no pair of elements e1 and e2 such that e1.equals(e2)"[^2]. So the rules for set elements are the same as the rules for map keys: a hash set needs a hash code that agrees with equality, and a sorted set needs an order. See [map and dictionary](/map-and-dictionary/) and [value equality and identity](/value-equality-and-identity/).

**A set is a map with no values.** Most languages build a set from the same parts as a map. Java `HashSet` is "backed by a hash table (actually a HashMap instance)"[^3]. Rust `HashSet` is "implemented as a HashMap where the value is ()"[^4]. Wikipedia says that sets "are commonly implemented in the same way as (partial) maps (associative arrays)"[^1]. So a hash set tests membership in constant time on average, and a tree set in logarithmic time. See [hash table](/hash-table/).

**The operations.** A set has the operations of set algebra. Wikipedia lists union, intersection, difference and subset: "union(S,T): returns the union of sets S and T"[^1], and "subset(S,T): a predicate that tests whether the set S is a subset of set T"[^1]. Python has operators for them: `|`, `&`, `-`, `^` and `<=`. JavaScript added methods: `union` "Takes a set and returns a new set containing elements which are in either or both of this set and the given set"[^5]. In Java and C#, the methods `addAll`, `retainAll`, `UnionWith` and `IntersectWith` change the set in place.

**Mutable and frozen sets.** A set that can change cannot be an element of another set or a key of a map, because its hash would change. Python has two types. The type `set` can change, and "Since it is mutable, it has no hash value and cannot be used as either a dictionary key or as an element of another set"[^6]. The type `frozenset` cannot change after the program makes it, so "it can therefore be used as a dictionary key or as an element of another set"[^6]. Java `Set.of` and C# `FrozenSet` are also sets that do not change.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `set` and `frozenset`; the literal `{1, 2}`; the operators `\|`, `&`, `-`, `^` and `<=` |
| JavaScript | native | `Set` in insertion order, with SameValueZero; `union`, `intersection` and the other ES2025 methods |
| TypeScript | native | the JavaScript `Set<T>` and `ReadonlySet<T>` |
| Java | stdlib | the interface `Set` with `HashSet`, `LinkedHashSet` and `TreeSet`; `Set.of` for a fixed set |
| Kotlin | stdlib | `setOf` and `mutableSetOf` (a `LinkedHashSet`); `union`, `intersect` and `subtract` |
| C# | stdlib | `HashSet<T>`, `SortedSet<T>` and `FrozenSet<T>`; `UnionWith`, `IntersectWith`, `SetEquals` |
| Go | idiom | no set type; `map[T]struct{}` or `map[T]bool` |
| Rust | stdlib | `HashSet<T>` and `BTreeSet<T>`; the operators `\|`, `&`, `-` and `^` on references |
| C | idiom | no set type; a bit set in an integer, a sorted array or a hand-written hash table |
| C++ | stdlib | `std::set` (sorted) and `std::unordered_set` (hash); `std::set_intersection` on sorted ranges |
| Bash | idiom | an associative array with the elements as keys |
| PowerShell | stdlib | the .NET `HashSet[T]`; a hash table with the elements as keys |
| SQL | native | `SELECT DISTINCT`; `UNION`, `INTERSECT` and `EXCEPT` on query results |
| Regex | idiom | a character class such as `[aeiou]` is a set of characters |
| HTML | n/a | markup; the `class` attribute is a list of names in text |
| CSS | n/a | no set values; a selector list matches a union, but it is a rule |

Python has a set literal, and Wikipedia notes the trap in it: "empty sets must be created using set(), because Python uses {} to represent the empty dictionary"[^1]. In Python, "A set object is an unordered collection of distinct hashable objects. Common uses include membership testing, removing duplicates from a sequence, and computing mathematical operations such as intersection, union, difference, and symmetric difference"[^6].

JavaScript `Set` keeps the insertion order: "A value in the set may only occur once; it is unique in the set's collection. You can iterate through the elements of a set in insertion order"[^5]. C# describes `HashSet<T>` as "a collection that contains no duplicate elements, and whose elements are in no particular order"[^7]. C++ has both kinds: "std::set is an associative container that contains a sorted set of unique objects of type Key"[^8], and for `std::unordered_set`, "Search, insertion, and removal have average constant-time complexity"[^9].

Go has no set type. The Go blog suggests a map: "a map of boolean values can be used as a set-like data structure (recall that the zero value for the boolean type is false)"[^10]. A `map[T]struct{}` does the same and stores nothing for each value.

The example files show these forms. Python shows `set()` and `{}`, `remove` and `discard`, a `frozenset` inside a set, and the difference between the methods and the operators. JavaScript shows `NaN`, arrays by identity and the ES2025 methods. Java shows `add` with its result, `TreeSet`, the in-place methods, `Set.of` with a duplicate and a list that changes inside a `HashSet`. C# uses a comparer that ignores case, `SortedSet` and `SetEquals`. Go writes a generic set type on a map. Rust uses the set operators and `BTreeSet::range`. C keeps a set of small integers in the bits of a 64-bit word. C++ uses `std::set`, the sorted range algorithms and `lower_bound`.

## Variations and related ideas

- **Sorted sets.** A tree set, such as Java `TreeSet`, Rust `BTreeSet` or C++ `std::set`, keeps the values in order. It can give the values in a range and find the next value after a given value. See [ordered map](/ordered-map/).
- **Insertion-ordered sets.** JavaScript `Set`, Java `LinkedHashSet` and Kotlin `setOf` keep the order in which the values were added. Python `set` does not, but `dict.fromkeys(items)` keeps the first order of each value. See [remove duplicates from a list](/remove-duplicates-from-a-list/).
- **Bit sets.** A set of small integers fits in the bits of a word or an array of words. "a subset of the integers 1,2,...,n can be implemented efficiently as an n-bit bit array, which also support very efficient union and intersection operations"[^1]. Java `BitSet`, C++ `std::bitset` and the C example use this form.
- **Multisets.** A multiset, or bag, counts how many times each value occurs. "A multiset is a special kind of set in which an element can appear multiple times in the set"[^1]. See [multiset](/multiset/).
- **Probabilistic sets.** A Bloom filter answers membership with a small chance of a wrong yes, and it takes much less memory than a set. "A Bloom map implements a set probabilistically, using a very compact representation but risking a small chance of false positives on queries"[^1].
- **Enum sets.** Java `EnumSet` and C# flag enums store a set of the values of an enumeration as bits.

## Pitfalls

- **The empty literal.** In Python, `{}` is an empty dict, not an empty set. Use `set()`.
- **Order.** A hash set has no order. Java `HashSet` "makes no guarantees as to the iteration order of the set; in particular, it does not guarantee that the order will remain constant over time"[^3]. A test that prints a set can fail. Sort the values, or use a sorted or insertion-ordered set.
- **Elements that change.** Java warns: "The behavior of a set is not specified if the value of an object is changed in a manner that affects equals comparisons while the object is an element in the set"[^2]. In the Java example, a list changes after it goes into a `HashSet`, and then `contains` returns false for it. Use immutable elements, such as tuples and frozensets in Python or records in Java.
- **Identity, not value.** In JavaScript, two arrays with the same items are two elements of a `Set`, because objects are compared by identity. A C# or Java class without `Equals` and `GetHashCode`, or `equals` and `hashCode`, has the same problem. Use a string key, a record or a custom comparer.
- **In-place operations.** Java `retainAll` and C# `IntersectWith` change the set that the program calls them on. Copy the set first if the program needs the original.
- **Membership in a list.** A test such as `x in items` on a list is a linear search. In a loop, it makes the whole loop quadratic. Make a set from the list once and test against the set.
- **Operators and iterables.** In Python, "the non-operator versions of union(), intersection(), difference(), symmetric_difference(), issubset(), and issuperset() methods will accept any iterable as an argument. In contrast, their operator based counterparts require their arguments to be sets"[^6]. So `s.union([4, 5])` works and `s | [4, 5]` raises `TypeError`.

## Cost

| Operation | Hash set | Tree set | Sorted array | Bit set (values below n) |
|---|---|---|---|---|
| membership test | constant on average | logarithmic | logarithmic (binary search) | constant |
| add or remove | constant on average | logarithmic | linear | constant |
| union or intersection of sizes m and n | linear in m + n | linear in m + n | linear in m + n (merge) | linear in n / 64 |
| loop in sorted order | needs a sort | linear | linear | linear in n / 64 |
| memory | the values and empty slots | the values and links | the values | n bits |

MDN notes that `has` on a `Set` "is, on average, faster than the Array.prototype.includes method when an array has a length equal to a set's size"[^5]. For a few values, a small array can still be the faster choice, because it needs no hashing.

## History

Wikipedia states that "One of the earliest languages to support sets was Pascal"[^1]. Pascal sets were bit sets of a small range of values. Later languages put general sets in their libraries: Java `HashSet` and `TreeSet` in the Collections Framework, C++ `std::set` and, from C++11, `std::unordered_set`[^1]. "Python has built-in set and frozenset types since 2.4, and since Python 3.0 and 2.7, supports non-empty set literals using a curly-bracket syntax"[^1]. JavaScript added `Set` in ES2015 and the set algebra methods in ES2025. Go still has no set type, and programs use maps.

## References
