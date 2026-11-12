---
tag: 81CRZ8
kind: concept
slug: hash-codes
name: Hash codes
aliases: [hash code, hash value, hashing, hashCode, GetHashCode, __hash__, hash function for keys, hash flooding, HashDoS, seeded hash]
area: data-and-values/hashing-in-practice
level: core
summary: >-
  A hash code is a number computed from a key that picks its bucket in a hash table. Equal keys must hash equal, keys must not change, and hashes on input need random seeds.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1953
  people: [Hans Peter Luhn, Arnold Dumey, Scott Crosby, Dan Wallach, Jean-Philippe Aumasson, Daniel J. Bernstein]
  note: Hans Peter Luhn used hashing in an IBM memo in 1953. Crosby and Wallach showed attacks by hash collisions in 2003, and SipHash, from 2012, became the seeded default of several languages.
see_also: [value-equality-and-identity, immutability, remove-duplicates-from-a-list, count-word-frequencies, floating-point-arithmetic, generate-random-numbers]
same_as:
  wikidata: Q183427
  wikipedia: Hash_function
sources:
- url: https://docs.python.org/3/reference/datamodel.html
  title: Data model, The Python Language Reference
  locator: object.__hash__
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Object.html
  title: Class Object, Java SE 25 API documentation
  locator: hashCode()
- url: https://doc.rust-lang.org/std/hash/trait.Hash.html
  title: Trait Hash, Rust standard library documentation
  locator: Hash and Eq
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Class String, Java SE 25 API documentation
  locator: hashCode()
- url: https://doc.rust-lang.org/std/collections/struct.HashMap.html
  title: Struct HashMap, Rust standard library documentation
  locator: Description
- url: https://ocert.org/advisories/ocert-2011-003.html
  title: oCERT-2011-003, multiple implementations denial-of-service via hash algorithm collision
  locator: Description
- url: https://peps.python.org/pep-0456/
  title: PEP 456, Secure and interchangeable hash algorithm
  locator: Abstract and Rationale
- url: https://pkg.go.dev/hash/maphash
  title: Package maphash, Go standard library documentation
  locator: Overview and type Seed
- url: https://en.cppreference.com/w/cpp/utility/hash.html
  title: std::hash, cppreference.com
  locator: Notes
- url: https://learn.microsoft.com/en-us/dotnet/api/system.object.gethashcode
  title: Object.GetHashCode Method, .NET API documentation
  locator: Remarks
---

## Idea

A hash table finds a key in about one step because it does not search: it computes a number from the key, the hash code, and uses that number to pick a bucket. Each language gives its types a hash function, such as `__hash__` in Python, `hashCode` in Java or the `Hash` trait in Rust, and a program that uses its own types as keys must supply one or derive it. Three rules decide if the table works. Keys that are equal must have equal hash codes. A key must not change while it is in a table. The hash must spread keys over the buckets, also for keys that an attacker chooses. [Value equality and identity](/value-equality-and-identity/) states the basic rule; this page covers the practice.

```python run id=idea
from itertools import permutations


class Table:  # a hash table with separate chaining
    def __init__(self, hash_fn, eq, size=64):
        self.hash, self.eq, self.compares = hash_fn, eq, 0
        self.buckets = [[] for _ in range(size)]

    def _bucket(self, key):
        return self.buckets[self.hash(key) % len(self.buckets)]

    def put(self, key, value):
        bucket = self._bucket(key)
        for i, (k, _) in enumerate(bucket):
            self.compares += 1
            if self.eq(k, key):
                bucket[i] = (key, value)
                return
        bucket.append((key, value))

    def get(self, key):
        for k, v in self._bucket(key):
            self.compares += 1
            if self.eq(k, key):
                return v
        return None


def fnv1a(s):  # FNV-1a, 32 bits: a simple hash with good spread
    h = 2166136261
    for byte in s.encode():
        h = ((h ^ byte) * 16777619) % 2**32
    return h


same_name = lambda a, b: a.lower() == b.lower()  # case-insensitive equality
broken = Table(fnv1a, same_name)  # hashes the raw text: breaks the contract
fixed = Table(lambda s: fnv1a(s.lower()), same_name)
for t in (broken, fixed):
    t.put("Alan", 1912)
print("broken get('ALAN'):", broken.get("ALAN"), " fixed get('ALAN'):", fixed.get("ALAN"))

key = ["x", "y"]  # a mutable key, hashed by its contents
t = Table(lambda k: fnv1a("".join(k)), lambda a, b: a == b)
t.put(key, "point")
key.append("z")
print("after the key changed:", t.get(key), t.get(["x", "y"]))

keys = ["".join(p) for p in permutations("abcdefg")][:500]
weak = Table(lambda s: sum(map(ord, s)), str.__eq__)  # anagrams collide
strong = Table(fnv1a, str.__eq__)
for t in (weak, strong):
    for k in keys:
        t.put(k, True)
print(f"500 anagram keys: weak hash {weak.compares} compares, FNV-1a {strong.compares}")
```

```output for=idea
broken get('ALAN'): None  fixed get('ALAN'): 1912
after the key changed: None None
500 anagram keys: weak hash 124750 compares, FNV-1a 3910
```

The table keeps a list of entries in each bucket and counts the comparisons of keys. The first table treats names as equal without regard to case but hashes the raw text, so `"ALAN"` goes to a different bucket from `"Alan"` and the lookup fails; hashing the lower-case text fixes it. The second part changes a list after it became a key, and then neither the changed list nor a copy of the old one finds the entry. The last part puts 500 anagrams into two tables. A hash that adds the character codes gives all of them one code, so every insert compares against every earlier key, 124750 comparisons in total; FNV-1a spreads them and needs 3910.

## How it works

**The contract.** Every language states the same rule. Python: "The only required property is that objects which compare equal have the same hash value".[^1] Java: "If two objects are equal according to the equals method, then calling the hashCode method on each of the two objects must produce the same integer result."[^2] Rust writes it as `k1 == k2 -> hash(k1) == hash(k2)`, "if two keys are equal, their hashes must also be equal", and adds: "Violating this property is a logic error."[^3] The reverse is not required: unequal keys can have one hash code, and the table then compares the keys. So a hash function must use only data that equality uses. A case-insensitive equality needs a case-insensitive hash, and an `Account` that is equal by `id` must hash only the `id`.

**Deriving hashes.** Writing the two functions by hand lets them drift apart when a field is added, so languages generate both from one list of fields. Python's frozen dataclasses, Java and C# records, Rust's `#[derive(PartialEq, Eq, Hash)]` and Kotlin's data classes do this. Go hashes every comparable struct by all its fields and does not let a program change that. C++ needs a specialization of `std::hash` or a hash object passed to the container, and C needs a function written by hand.

**Combining fields.** A hash of several fields must mix them, so that a change in any field changes the result, and so that `(1, 2)` and `(2, 1)` differ. Adding or XOR-ing the field hashes fails both tests, as the anagrams in the example show. The usual forms multiply the running value by an odd number before each field, as Java's `Objects.hash` and the formula of `String.hashCode` do, or call a mixing function such as `HashCode.Combine` in C# or the hash_combine idiom of Boost. Java fixes the string formula in its specification: "The hash code for a String object is computed as s[0]*31^(n-1) + s[1]*31^(n-2) + ... + s[n-1] using int arithmetic".[^4]

**Keys must not change.** A table stores an entry in the bucket for its hash at the time of the insert. If the key changes later, the entry stays in the old bucket, and lookups by the new value or by an equal copy of the old value both fail, as the Java wrong block shows. Rust states it: "It is also a logic error for a key to be modified in such a way that the key’s hash, as determined by the Hash trait, or its equality, as determined by the Eq trait, changes while it is in the map."[^5] Python refuses to hash its mutable built-in types, and Go refuses slices, maps and functions as keys at compile time. Java and C# allow mutable keys and leave the rule to the programmer.

**Hash flooding.** If an attacker can predict the hash function, the attacker can send many keys with one hash code, and each insert then compares against all earlier keys, so n keys cost about n squared steps. In 2011 an advisory reported that "A variety of programming languages suffer from a denial-of-service (DoS) condition against storage functions of key/value pairs in hash data structures, the condition can be leveraged by exploiting predictable collisions in the underlying hashing algorithms."[^6] A web form with a few megabytes of such keys could keep a server busy for minutes. Java's `String.hashCode` makes such keys easy to build: `"Aa"` and `"BB"` collide, so every string made of these two parts collides with every other of the same length.

**Random seeds.** The defense is a hash function with a secret key chosen when the program starts, so that an attacker cannot predict collisions. Python salts the hashes of `str` and `bytes`: "Although they remain constant within an individual Python process, they are not predictable between repeated invocations of Python."[^1] PEP 456 then made SipHash the default, because "The current hash algorithm and its randomization is not resilient against attacks."[^7] Rust's `HashMap` uses an "algorithm selected to provide resistance against HashDoS attacks", and "The algorithm is randomly seeded".[^5] Go seeds each map, and its `hash/maphash` package gives the same seeded hashes to programs: "Two Hash objects with different seeds will very likely behave differently."[^8] .NET randomizes string hashes and the seed of `HashCode` for each process.

**Hashes are not stable.** With random seeds, the hash of a value changes from one run to the next. The C++ standard allows this: "Hash functions are only required to produce the same result for the same input within a single execution of a program; this allows salted hashes that prevent collision denial-of-service attacks."[^9] The .NET documentation states the rules for programs: "Do not serialize hash code values or store them in databases." and "Do not send hash codes across application domains or processes."[^10] A file name, a cache key or a shard number that must stay the same across runs needs a fixed hash function, such as FNV-1a or a cryptographic hash, not the built-in hash code.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `__hash__`, `hash()`, frozen dataclasses; salted `str` hashes |
| JavaScript | idiom | `Map` and `Set` by SameValueZero; string keys built by hand |
| TypeScript | idiom | as JavaScript |
| Java | native | `hashCode` and `equals`, records, `Objects.hash` |
| Kotlin | native | `hashCode`, data classes |
| C# | native | `GetHashCode`, records, `IEqualityComparer<T>`, `HashCode` |
| Go | native | comparable keys hashed by the runtime; `hash/maphash` |
| Rust | stdlib | the `Hash` trait, `#[derive(Hash)]`, `RandomState` |
| C | idiom | hash functions and tables written by hand |
| C++ | stdlib | `std::hash` specializations, unordered containers |
| Bash | native | `declare -A` with string keys |
| PowerShell | native | hashtables on .NET hashing |
| SQL | n/a | hash joins and indexes inside the engine |
| Regex | n/a | no hash tables |
| HTML | n/a | no hash tables |
| CSS | n/a | no hash tables |

Python, Java, Kotlin, C# and Go hash keys in the language or in its core object model, so they are native. Rust and C++ put the hash trait or template and the tables in the standard library. JavaScript has hash tables but no way to define equality or a hash for a type, so the idiom builds a string key from the fields. C has neither tables nor hash functions in its standard library.

The example files show the three rules. Java and C# show derived hashes in records, a hash written by hand on one field, and C# comparers that change equality and the hash together. The Java wrong block changes a key in a set, and the Go and Rust fails blocks show compilers that reject keys that cannot be hashed. Python and Rust show random seeds, Java shows the colliding strings of a fixed formula, and C++ shows that `std::hash<const char*>` hashes the address and not the text. The C file writes FNV-1a and a table with open addressing.

## Variations and related ideas

- **Separate chaining and open addressing.** A table either keeps a list in each bucket, as the idea block does, or stores keys in the array and probes the next slots, as the C file does. Swiss tables, used by Abseil, Rust and recent versions of Go, probe groups of slots with SIMD instructions.
- **Tree bins.** Since Java 8, `HashMap` turns a long chain into a balanced tree when the keys are comparable, so many collisions cost log n steps, not n.
- **Perfect hashing.** For a fixed set of keys, such as the keywords of a language, a generator such as gperf builds a hash function with no collisions.
- **Consistent hashing.** Distributed caches assign keys to servers so that adding a server moves few keys.
- **Cryptographic hashes.** SHA-256 and BLAKE3 resist collisions even when every input is chosen by an attacker, and they are stable across runs, but they are slower than table hashes. See [generate random numbers](/generate-random-numbers/) for the related use of hashes as random sources.
- **Identity hashes.** Objects with no equality of their own hash by identity, as Java's `System.identityHashCode` and Python's default `__hash__` do.
- **Uses.** Hash tables underlie [removing duplicates](/remove-duplicates-from-a-list/), [counting words](/count-word-frequencies/), caches, sets and joins.

## Pitfalls

- **Equality and hash that disagree.** Overriding `equals` or `__eq__` without the hash, or hashing a field that equality ignores, makes equal keys miss each other. Derive both from one list of fields.
- **Mutable keys.** Changing a field that the hash uses, while the object is a key, loses the entry. Use immutable types as keys; see [immutability](/immutability/).
- **Floating-point keys.** NaN is not equal to itself, so each NaN key in a Go map is a new entry that no lookup finds. In Python `0.0` and `-0.0` are one key, but Java's `Double.equals` treats them as two keys, and values that differ in the last bit are always different keys. See [floating-point arithmetic](/floating-point-arithmetic/).
- **Keys of mixed types.** In Python, `1`, `1.0` and `True` are one key; in a JavaScript object, `1` and `"1"` are one key.
- **Hashing pointers.** `std::hash<const char*>` and the default hash of most objects use the address, so two equal strings at different places differ.
- **Stored hash codes.** A built-in hash saved to a file or a database, or sent to another process, will not match the next run. Use a fixed, documented function.
- **Predictable hashes on input.** A table filled from requests, JSON objects or headers with a fixed hash invites flooding. Keep the seeded default, and limit the number of keys per request.
- **Weak combining.** Summing or XOR-ing the hashes of fields makes all permutations collide, and XOR makes `(x, x)` hash to zero.

## Cost

A good hash makes a lookup cost one hash computation and about one comparison. The hash itself reads the whole key, so a long string costs time in proportion to its length; Java caches the hash in each `String` for this reason. With a bad or attacked hash every key lands in one bucket and each lookup costs n comparisons, or log n with Java's tree bins. Seeded hashes such as SipHash are a few times slower than FNV or a multiply and shift for short keys, which is why Rust lets a program choose a faster hasher when its keys are not from untrusted input.

## History

Hans Peter Luhn of IBM described hashing in an internal memo in 1953, and Arnold Dumey published the idea in 1956. Glenn Fowler, Landon Curt Noll and Phong Vo designed the FNV hash in 1991. Scott Crosby and Dan Wallach showed in 2003 that predictable hashes let an attacker slow a program down, and Perl randomized its hashes in the same year. In December 2011, a talk at the 28C3 conference and an advisory showed the attack on PHP, Java, Python, Ruby and others,[^6] and Java 8 later added tree bins to `HashMap`.

Jean-Philippe Aumasson and Daniel J. Bernstein published SipHash, a fast keyed hash for short inputs, in 2012. Python 3.3 turned on hash randomization by default in the same year, and Python 3.4 made SipHash its string hash through PEP 456, which "proposes SipHash as default string and bytes hash algorithm to properly fix hash randomization once and for all".[^7] Rust used SipHash from its first release and moved to SipHash 1-3,[^5] .NET Core randomized string hashes, and Go added `hash/maphash` in 1.14.[^8]

## References
