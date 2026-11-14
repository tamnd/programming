---
tag: 4091XS
kind: concept
slug: hash-table
name: Hash table
aliases: [hash map, hashmap, hash, hashtable, unordered map, bucket array, scatter table, HashMap, Hashtable, unordered_map]
area: data-and-values/maps-and-sets
level: core
summary: >-
  A hash table stores keys in an array of buckets and uses the hash code of a key to find its bucket. Lookup, insert and remove take constant time on average.
applies_to: core-languages
viz: optional
formal: none
see_also: [map-and-dictionary, set-concept, hash-codes, value-equality-and-identity, dynamic-array, linked-list, array]
sources:
- url: https://en.wikipedia.org/wiki/Hash_table
  title: Hash table, Wikipedia
  locator: Introduction; Load factor; Collision resolution; Dynamic resizing; History
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Object.html
  title: Class Object, Java SE 25 API
  locator: hashCode
- url: https://docs.python.org/3/reference/datamodel.html
  title: Data model, The Python Language Reference
  locator: object.__hash__
- url: https://learn.microsoft.com/en-us/dotnet/api/system.object.gethashcode
  title: Object.GetHashCode method, .NET API reference
  locator: Remarks
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/HashMap.html
  title: Class HashMap, Java SE 25 API
  locator: Description
- url: https://en.cppreference.com/cpp/container/unordered_map/max_load_factor
  title: std::unordered_map::max_load_factor, cppreference.com
  locator: Description
- url: https://doc.rust-lang.org/std/collections/struct.HashMap.html
  title: Struct HashMap, The Rust Standard Library
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Map
  title: Map, MDN Web Docs
  locator: Description; Key equality
- url: https://en.cppreference.com/cpp/container/unordered_map
  title: std::unordered_map, cppreference.com
  locator: Description
- url: https://go.dev/blog/swisstable
  title: Michael Pratt, Faster Go maps with Swiss Tables, The Go Blog, 2025
  locator: Full text
---

## Idea

A hash table is the usual way to build a [map](/map-and-dictionary/) or a [set](/set-concept/). It keeps an array of buckets. To store a key, the table computes a number from the key, the hash code, and uses the number to choose a bucket. To find the key again, it computes the same number and looks in the same bucket only. Wikipedia says: "A hash table uses a hash function to compute an index, also called a hash code, into an array of buckets or slots, from which the desired value can be found"[^1].

This page is about hash tables as a programmer uses them: what a key must do, why the order of the keys changes, and when a table grows. The algorithms inside a hash table are on the algorithm site: [hash table](https://algorithm.tamnd.com/hash-table/), [separate chaining](https://algorithm.tamnd.com/separate-chaining/) and [open addressing](https://algorithm.tamnd.com/open-addressing/).

```python run id=idea
# A small hash table with chaining: each bucket is a list of (key, value) pairs.
class Table:
    def __init__(self, size=8):
        self.buckets = [[] for _ in range(size)]
        self.count = 0

    def index(self, key):
        return hash(key) % len(self.buckets)

    def put(self, key, value):
        bucket = self.buckets[self.index(key)]
        for i, (k, _) in enumerate(bucket):
            if k == key:
                bucket[i] = (key, value)
                return
        bucket.append((key, value))
        self.count += 1
        if self.count / len(self.buckets) > 0.75:
            self.resize(2 * len(self.buckets))

    def get(self, key):
        for k, v in self.buckets[self.index(key)]:
            if k == key:
                return v
        raise KeyError(key)

    def resize(self, size):
        old = self.buckets
        self.buckets = [[] for _ in range(size)]
        for bucket in old:
            for k, v in bucket:
                self.buckets[self.index(k)].append((k, v))

    def show(self):
        return {i: [k for k, _ in b] for i, b in enumerate(self.buckets) if b}


# hash(n) is n for a small int, so the bucket is n % 8. 3, 11 and 19 collide.
t = Table()
for key in [3, 11, 19, 5, 8]:
    t.put(key, key * 10)
print(t.show())
print(t.get(19), t.count / len(t.buckets))

# The seventh key makes the load factor 7/8, which is more than 0.75.
t.put(4, 40)
t.put(6, 60)
print(len(t.buckets), t.show())
```

```output for=idea
{0: [8], 3: [3, 11, 19], 5: [5]}
190 0.625
16 {3: [3, 19], 4: [4], 5: [5], 6: [6], 8: [8], 11: [11]}
```

The keys 3, 11 and 19 have the same remainder after division by 8, so they share bucket 3. A lookup of 19 looks at bucket 3 only and compares 19 with the three keys there. After the seventh key, the table doubles to 16 buckets and puts each key again. Now 11 is in bucket 11, and the order of the keys in the table is different.

## How it works

**Hash, then compare.** A lookup has two steps. The table hashes the key to find a bucket, and then it compares the key with each key in that bucket. Two different keys can have the same hash code or the same bucket. Wikipedia notes that "Most hash table designs employ an imperfect hash function"[^1], so the table must handle these collisions. "Common strategies to handle hash collisions include separate chaining, which stores multiple elements in the same slot using linked lists, and open addressing, which searches for the next available slot according to a probing sequence"[^1]. The example above uses separate chaining. Python, Rust and Go use open addressing.

**The rule for keys.** Because the table compares keys only inside one bucket, two equal keys must go to the same bucket. So equal keys must have equal hash codes. Java states the rule: "If two objects are equal according to the equals method, then calling the hashCode method on each of the two objects must produce the same integer result"[^2]. Python says "The only required property is that objects which compare equal have the same hash value"[^3]. The reverse is not necessary. .NET notes that "equal hash codes do not imply object equality, because different (unequal) objects can have identical hash codes"[^4]. A type that breaks the rule gives a table that cannot find its own keys. See [hash codes](/hash-codes/) and [value equality and identity](/value-equality-and-identity/).

**The load factor.** "The efficiency of a hash table depends on the load factor (α), defined as the ratio of the number of stored elements to the number of available slots"[^1]. With more keys for each bucket, a lookup compares more keys. So a table grows when the load factor passes a limit. Java `HashMap` grows when "the number of entries in the hash table exceeds the product of the load factor and the current capacity"[^5], to "approximately twice the number of buckets"[^5], and "the default load factor (.75) offers a good tradeoff between time and space costs"[^5]. C++ grows "if the load factor exceeds this threshold"[^6], and its default maximum load factor is 1.

**Resize and order.** A larger table moves every key. "The items cannot simply be copied over into the same indices since the hash function depends on the table size"[^1]. This is why a hash table has no stable order: the order of a loop follows the buckets, and the buckets change when the table grows. Java `HashMap` "makes no guarantees as to the order of the map; in particular, it does not guarantee that the order will remain constant over time"[^5]. A program that needs an order uses an [insertion-ordered map](/insertion-ordered-map/) or an [ordered map](/ordered-map/).

**Seeds.** Some languages add a random seed to the hash function. Then the same keys go to different buckets in each run. Rust: "By default, HashMap uses a hashing algorithm selected to provide resistance against HashDoS attacks. The algorithm is randomly seeded"[^7]. In Python, "the __hash__() values of str and bytes objects are “salted” with an unpredictable random value"[^3]. The seed protects a server from an attacker who sends many keys with the same hash code. The attack is on the algorithm site: [algorithmic complexity attack](https://algorithm.tamnd.com/algorithmic-complexity-attack/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `dict` and `set` with open addressing; `__hash__` and `__eq__`; str hashes salted for each process |
| JavaScript | native | `Map` and `Set`; no hash code for a program; keys compared with SameValueZero, objects by identity |
| TypeScript | native | the JavaScript `Map` and `Set` |
| Java | stdlib | `HashMap`, `HashSet` and `LinkedHashMap` with chaining; `hashCode` and `equals`; load factor 0.75 |
| Kotlin | stdlib | the JVM `HashMap` and `HashSet`; a data class defines `equals` and `hashCode` |
| C# | stdlib | `Dictionary<TKey,TValue>` and `HashSet<T>`; `GetHashCode` and `Equals`, or an `IEqualityComparer<T>` |
| Go | native | the built-in `map`, a Swiss table; keys of any comparable type |
| Rust | stdlib | `HashMap` and `HashSet`; the traits `Hash` and `Eq`; SipHash 1-3 with a random seed by default |
| C | idiom | no hash table in the standard library; POSIX `hsearch`, or a hand-written table |
| C++ | stdlib | `std::unordered_map` and `std::unordered_set` with buckets; `std::hash` or a user hash functor |
| Bash | native | associative arrays from `declare -A`, with no access to the buckets |
| PowerShell | native | `@{}` is a `System.Collections.Hashtable` with keys that ignore case |
| SQL | idiom | hash indexes and hash joins inside the database |
| Regex | n/a | patterns match text and have no hash tables |
| HTML | n/a | markup with no data structures |
| CSS | n/a | style rules with no data structures |

JavaScript does not say how a `Map` works inside. MDN quotes the specification: a map must be implemented with mechanisms "that, on average, provide access times that are sublinear on the number of elements in the collection"[^8]. In practice the engines use hash tables. C++ is the most open: `std::unordered_map` keeps "key-value pairs with unique keys", and "Which bucket an element is placed into depends entirely on the hash of its key. Keys with the same hash code appear in the same bucket"[^9]. A program can read the number of buckets, the bucket of a key and the load factor.

Go changed its map in Go 1.24. "Go 1.24 includes a completely new implementation of the built-in map type, based on the Swiss Table design"[^10]. A Swiss table uses open addressing, but it breaks the array "into logical groups of 8 slots each"[^10], and each group has a control word with 7 bits of the hash for each slot, so a lookup tests 8 slots with a few instructions. The blog calls this "an operation supported by SIMD hardware"[^10]. The [Swiss table](https://algorithm.tamnd.com/swiss-table/) page on the algorithm site has the details.

The example files show these forms. Python shows the hash of small ints, the rule for `__eq__` and `__hash__`, and a constant hash that makes each lookup a scan. JavaScript shows SameValueZero and object keys by identity. Java shows that `"Aa"` and `"BB"` both have the hash code 2112, a class that overrides `equals` only, a record, and `HashMap.newHashMap`. C# counts the calls to `Equals` for a constant hash code: 1000 inserts make 499500 calls, and one lookup makes 500 more. Go shows `NaN` keys, `0.0` and `-0.0`, a size hint and a key that cannot be hashed. Rust shows `derive(Hash)`, two `RandomState` seeds and `with_capacity`. C writes a chained table that doubles. C++ shows the bucket interface, `reserve` and `rehash`.

## Variations and related ideas

- **Separate chaining.** Each bucket holds a list of entries. Java `HashMap` and C++ `std::unordered_map` use chains. When many keys collide and the keys are `Comparable`, Java `HashMap` "may use comparison order among keys to help break ties"[^5]. See [separate chaining](https://algorithm.tamnd.com/separate-chaining/).
- **Open addressing.** All entries are in one array. A collision moves to another slot by a probe sequence. Python `dict`, Rust `HashMap` and Go `map` use open addressing. See [open addressing](https://algorithm.tamnd.com/open-addressing/).
- **Hash sets.** A [set](/set-concept/) is a hash table with keys and no values. `HashSet` in Java, C# and Rust and `set` in Python work the same way as their maps.
- **Size hints.** A program that knows the number of keys can size the table at the start: `HashMap.newHashMap(n)` in Java, `EnsureCapacity` in C#, `make(map[K]V, n)` in Go, `with_capacity` in Rust and `reserve` in C++. This avoids the copies of each resize.
- **Custom hashers.** Rust and C++ take the hash function as a type parameter, and C# takes an `IEqualityComparer<T>`. A program can use a faster hash for trusted keys, or a hash that ignores case.
- **Perfect hashing.** For a fixed set of keys, such as the keywords of a language, a generator such as gperf makes a hash function with no collisions. See [perfect hashing](https://algorithm.tamnd.com/perfect-hashing/).

## Pitfalls

- **Equals without hash.** In Java and C#, a class that overrides `equals` or `Equals` and not `hashCode` or `GetHashCode` breaks the rule. Two equal keys usually go to different buckets, so `get` returns `null` for a key that looks present. Define both, or use a record. In Python, a class that defines `__eq__` and not `__hash__` cannot be a key at all.
- **A key that changes.** The table stores a key in the bucket of its hash at the time of the insert. If a field that the hash uses changes, the key is in the wrong bucket and the table does not find it. Use immutable keys. See [map and dictionary](/map-and-dictionary/).
- **A bad hash.** A hash function that gives the same value to many keys puts them in one bucket, and each lookup becomes a linear scan. Java warns that "using many keys with the same hashCode() is a sure way to slow down performance of any hash table"[^5]. In the C# example, 1000 keys with a constant hash code make 499500 calls to `Equals`.
- **Depending on the order.** The order of a loop over a hash table depends on the hash codes, the size of the table and, in some languages, a random seed. It can change between runs, between versions and after a resize. Sort the keys, or use an ordered map.
- **Stored hash codes.** A hash code is for one table in one process. Java says that the hash code "need not remain consistent from one execution of an application to another execution of the same application"[^2], and .NET randomizes string hash codes in each process. Do not write hash codes to a file or use them as IDs.
- **NaN keys.** `NaN` is not equal to itself. In Go, each `m[NaN] = v` adds a new entry that no lookup finds. Rust does not let `f64` be a key at all. JavaScript `Map` treats `NaN` as one key.
- **Iterators after a resize.** An insert can resize the table and move the entries. In C++, a rehash invalidates the iterators of an `unordered_map`. Pointers and references to the elements stay valid.

## Cost

| Operation | Average | Worst case |
|---|---|---|
| lookup | constant | linear in the number of keys |
| insert | constant, amortized | linear |
| remove | constant | linear |
| resize | linear (rare) | linear |
| loop over all keys | linear in the keys plus the buckets | same |
| memory | the entries plus empty slots: at a load factor of 0.75 or less, at least 4 slots for each 3 entries | same |

"In a well-dimensioned hash table, the average time complexity for each lookup is independent of the number of elements stored in the table"[^1]. The worst case comes from many keys in one bucket. In Java, a loop over a `HashMap` takes time in proportion to the capacity, which is the number of buckets, plus the number of entries[^5]. A table that was large and is now almost empty is slow to loop over.

A hash table also has costs that the table above does not show. Each operation computes a hash, and a long string key takes time linear in its length. Each lookup compares the key with at least one stored key. For a few keys, a linear search in an array can be faster. The resizes cost linear time in total, as for a [dynamic array](/dynamic-array/), because the table doubles each time.

## History

"In January 1953, Hans Peter Luhn wrote an internal IBM memorandum that used hashing with chaining"[^1]. About the same time, a team at IBM Research "implemented hashing for the IBM 701 assembler"[^1]. Wikipedia credits the first published use of the word hashing to an article by Robert Morris[^1].

Scripting languages made hash tables a part of the language: AWK arrays, Perl hashes and Python dicts. Java had `Hashtable` in version 1.0 and added `HashMap` in 1.2, and C++ added `std::unordered_map` in C++11. Newer designs use the cache of the processor better. "In 2017, Sam Benzaquen, Alkis Evlogimenos, Matt Kulukundis, and Roman Perepelitsa at Google presented a new C++ hash table design, dubbed “Swiss Tables”"[^10]. Rust's `HashMap` is "implemented with quadratic probing and SIMD lookup"[^7], and Go moved to Swiss tables in Go 1.24[^10].

## References
