---
tag: WAYDS7
kind: concept
slug: sequence
name: Sequence
aliases: [sequence type, ordered collection, sequenced collection, Sequence ABC, SequenceContainer, lazy sequence, iter.Seq, IEnumerable, iterable]
area: data-and-values/arrays-and-lists
level: core
summary: >-
  A sequence is a set of items in a fixed order, where one value can appear more than once. In code the word names indexed types, ordered collections and lazy streams of items.
applies_to: core-languages
viz: optional
formal: none
see_also: [list, array, slice-concept, string-concept, iterators, laziness, iterator-pattern, interfaces]
sources:
- url: https://en.wikipedia.org/wiki/Sequence
  title: Sequence, Wikipedia
  locator: Introduction; Computing; Streams
- url: https://docs.python.org/3/glossary.html
  title: Glossary, Python documentation
  locator: sequence
- url: https://docs.python.org/3/library/collections.abc.html
  title: collections.abc, The Python Standard Library
  locator: Collections Abstract Base Classes; Sequence and MutableSequence
- url: https://openjdk.org/jeps/431
  title: JEP 431, Sequenced Collections
  locator: Summary; Motivation
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/SequencedCollection.html
  title: Interface SequencedCollection, Java SE 25 API
  locator: Interface description
- url: https://en.cppreference.com/cpp/named_req/SequenceContainer
  title: C++ named requirements, SequenceContainer, cppreference.com
  locator: Introduction; Usage notes
- url: https://kotlinlang.org/docs/sequences.html
  title: Sequences, Kotlin documentation
  locator: Introduction
- url: https://doc.rust-lang.org/std/iter/index.html
  title: Module iter, The Rust Standard Library
  locator: Infinity; Laziness
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Iteration_protocols
  title: Iteration protocols, MDN Web Docs
  locator: The iterable protocol; The iterator protocol
- url: https://pkg.go.dev/iter
  title: Package iter, Go standard library
  locator: Iterators; type Seq
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.ienumerable-1?view=net-10.0
  title: IEnumerable<T> Interface, .NET API reference
  locator: Definition
- url: https://www.postgresql.org/docs/current/sql-createsequence.html
  title: CREATE SEQUENCE, PostgreSQL documentation
  locator: Description
- url: https://hackage.haskell.org/package/containers/docs/Data-Sequence.html
  title: Data.Sequence, the containers package, Hackage
  locator: Description; Implementation
---

## Idea

A sequence is the general idea behind arrays, lists, strings and streams: items that come one after another. Wikipedia gives the mathematical definition: "a sequence is a collection of objects possibly with repetition, that come in a specified order". It adds: "Unlike a set, the same elements can appear multiple times at different positions in a sequence, and unlike a set, the order does matter"[^1]. So `(1, 2, 1)` is a sequence of three items, and it is not the same sequence as `(1, 1, 2)`.

In programming, the word has three common meanings:

- an **indexed sequence**, which has a length and an item at each position from 0 to the length minus 1, such as an array, a [list](/list/), a tuple or a string;
- an **ordered collection**, which has a first item and a last item and can be read in order and in reverse, such as a list, a deque or an ordered set;
- a **lazy sequence** or stream, which gives one item at a time when a loop asks for it. It can be infinite, and it can have no length and no index.

Wikipedia connects these words: "In computer science, finite sequences are called lists. Potentially infinite sequences are called streams. Finite sequences of characters or digits are called strings"[^1].

```python run id=idea
from collections.abc import Sequence
from itertools import count, islice

# A sequence has a length and an item for each index from 0 to length - 1.
# This class stores no items. It computes item i when the program asks for it.
class Squares(Sequence):
    def __init__(self, n):
        self.n = n

    def __len__(self):
        return self.n

    def __getitem__(self, i):
        if isinstance(i, slice):
            return [self[j] for j in range(*i.indices(self.n))]
        if i < 0:
            i += self.n
        if not 0 <= i < self.n:
            raise IndexError(i)
        return i * i

sq = Squares(6)
# The Sequence base class builds in, iteration, reversed, index and count from the two methods.
print(len(sq), sq[2], sq[-1], sq[1:4])
print(list(sq), 16 in sq, sq.index(9), list(reversed(sq)))

# A lazy sequence can be infinite. It has an order but no length.
evens = (2 * k for k in count())
print(list(islice(evens, 5)), next(evens))
```

```output for=idea
6 4 25 [1, 4, 9]
[0, 1, 4, 9, 16, 25] True 3 [25, 16, 9, 4, 1, 0]
[0, 2, 4, 6, 8] 10
```

`Squares` stores no items, but it is a full indexed sequence, because it has a length and an item for each index. The Python base class gives it `in`, iteration, `reversed`, `index` and `count` for free. The generator `evens` is a lazy sequence: it has an order, but it never ends, so the program takes 5 items with `islice`, and the next item is 10.

## How it works

**Indexed sequences.** An indexed sequence needs two operations: the length and the item at a position. Python defines a sequence as "An iterable which supports efficient element access using integer indices via the __getitem__() special method and defines a __len__() method that returns the length of the sequence"[^2]. The abstract base class `Sequence` asks for `__getitem__` and `__len__` and gives the mixin methods "__contains__, __iter__, __reversed__, index, and count"[^3]. `MutableSequence` adds the methods that change the items, such as `insert`, `append` and `pop`[^3]. Every operation of an indexed sequence can be built from these few, so code that uses only them works on arrays, lists, strings, ranges and computed sequences.

A type can have a length and an index operator and still not be a sequence. Python notes that a `dict` "also supports __getitem__() and __len__(), but is considered a mapping rather than a sequence because the lookups use arbitrary hashable keys"[^2]. In a sequence, the keys are the positions.

**Ordered collections.** Some collections have an order but no fast index. Java added an interface for them in Java 21. JEP 431 says: "Java’s collections framework lacks a collection type that represents a sequence of elements with a defined encounter order"[^4]. The new interface `SequencedCollection` is "A collection that has a well-defined encounter order, that supports operations at both ends, and that is reversible"[^5]. `List`, `Deque` and `LinkedHashSet` implement it, but `HashSet` does not, because it has no defined order[^4].

**Sequence containers.** C++ uses the word for a group of container types. "A SequenceContainer is a Container that stores objects of the same type in a linear arrangement"[^6]. `std::vector`, `std::deque`, `std::list` and `std::forward_list` meet these requirements, so one template can call `insert`, `front` and `back` on any of them. The cppreference table lists the tradeoffs, for example `std::vector` has "Fast access, contiguous storage" and "Mostly inefficient insertions/deletions"[^6].

**Lazy sequences.** A lazy sequence does not store its items. It computes each item when the consumer asks for the next one. Kotlin explains the difference: "Unlike collections, sequences don't contain elements, they produce them while iterating"[^7]. Each step of a pipeline over a collection makes a whole intermediate collection, but "multistep processing of sequences is executed lazily when possible: actual computing happens only when the result of the whole processing chain is requested"[^7]. Rust iterators work in the same way: "Iterators (and iterator adapters) are lazy. This means that just creating an iterator doesn’t do a whole lot. Nothing really happens until you call next"[^8]. See [laziness](/laziness/) and [iterators](/iterators/).

A lazy sequence can be infinite. Rust gives an example: "an open-ended range is an infinite iterator", and "It is common to use the take iterator adapter to turn an infinite iterator into a finite one"[^8]. JavaScript describes its iterator protocol as "a standard way to produce a sequence of values (either finite or infinite)"[^9].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `list`, `tuple`, `str`, `bytes` and `range`; the `Sequence` and `MutableSequence` base classes; generators for lazy sequences |
| JavaScript | native | the iterable protocol (`Symbol.iterator`) for arrays, strings, `Map`, `Set` and generators |
| TypeScript | native | the JavaScript protocols with the types `Iterable<T>`, `ArrayLike<T>` and `ReadonlyArray<T>` |
| Java | stdlib | `SequencedCollection` (Java 21) for `List`, `Deque` and ordered sets; `Stream` for lazy sequences |
| Kotlin | stdlib | `List<T>` for indexed sequences; `Sequence<T>` for lazy ones, made with `sequenceOf` and `generateSequence` |
| C# | stdlib | `IEnumerable<T>` with `yield return` and LINQ; `IReadOnlyList<T>` and `IList<T>` for indexed sequences |
| Go | stdlib | slices for indexed sequences; `iter.Seq` and `iter.Seq2` functions with `range` over a function (Go 1.23) |
| Rust | stdlib | slices `[T]` for indexed sequences; the `Iterator` trait for lazy sequences |
| C | idiom | a pointer and a length; a lazy sequence is a struct with its state and a `next` function |
| C++ | stdlib | the SequenceContainer types; ranges and views (C++20) for lazy sequences |
| Bash | native | indexed arrays, the words of `"$@"` and the output of `seq` |
| PowerShell | native | arrays and the pipeline, which passes one object at a time |
| SQL | idiom | rows in the order of `ORDER BY`; `CREATE SEQUENCE` makes a number generator, which is a different thing |
| Regex | n/a | a pattern matches a sequence of characters but has no sequence values |
| HTML | n/a | markup; the order of elements in a document is a sequence, but HTML has no data type for it |
| CSS | n/a | style rules have no sequences |

The languages split the three meanings in different ways. Python, Java and C# have one interface for each meaning. Go and Rust use a slice for the indexed sequence and a separate type for the lazy one. In Go, "An iterator is a function that passes successive elements of a sequence to a callback function, conventionally named yield"[^10]. C# makes lazy sequences with `yield return`, and its base interface `IEnumerable<T>` "Exposes the enumerator, which supports a simple iteration over a collection of a specified type"[^11]. In JavaScript, "The iterable protocol allows JavaScript objects to define or customize their iteration behavior, such as what values are looped over in a for...of construct"[^9].

SQL uses the word for something else. In PostgreSQL, "CREATE SEQUENCE creates a new sequence number generator"[^12], an object that gives the next number of a counter, often for a key column. It does not store a sequence of values.

The example files show these forms. Python shows which built-in types are sequences, the common operations on lists, tuples and ranges, and a generator that gives its items once. JavaScript defines an iterable object, an infinite generator and the iterables of the language. Java shows one method for a list, a deque and a `LinkedHashSet` through `SequencedCollection`, a reversed view and an infinite `Stream`. C# shows a lazy `yield return` sequence, a deferred LINQ query that sees later changes, and `ToList`. Go writes an `iter.Seq` for Fibonacci numbers, stops it with `break`, and uses `slices.Values`, `slices.Backward` and `maps.Keys`. Rust shows `next`, a lazy `map` that runs only when the program collects, an infinite range and `successors`. C writes a lazy sequence as a struct and a `next` function. C++ uses one template on three containers, then a lazy pipeline of views.

## Variations and related ideas

- **Strings.** A string is a sequence of characters, bytes or code units. Python strings are sequences of strings of length 1. See [string](/string-concept/).
- **Tuples.** A tuple is a fixed-length sequence, usually with items of different types.
- **Ranges.** A [range](/range/) such as `range(0, 10, 2)` is an indexed sequence that stores only its start, stop and step. In the Python example, `len(range(10**18))` takes no memory for the items.
- **Persistent sequences.** Haskell's `Data.Seq` gives "General-purpose finite sequences"[^13] that are immutable and fast at both ends. "The implementation uses 2-3 finger trees annotated with sizes"[^13].
- **Streams.** Wikipedia uses stream for a sequence that can be infinite[^1]. Java `Stream`, Kotlin `Sequence` and Rust iterators are examples.
- **Slices.** A [slice](/slice-concept/) is a part of an indexed sequence, and in many languages it is a sequence of the same type.

## Pitfalls

- **One pass.** A generator, an iterator or a Java `Stream` gives its items once. In the Python and JavaScript examples, a second pass over the same generator finds nothing. Make a list if the code needs the items twice.
- **Deferred work.** A lazy query runs when the program reads it, not when the program writes it. In the C# example, a query made before `list.Add(10)` still sees 10, and it runs again each time the program counts it.
- **Infinite sequences.** A call that reads all items, such as `list`, `collect`, `ToList` or `count`, never ends on an infinite sequence. Take a fixed number of items first.
- **Side effects in lazy steps.** A `map` with a side effect does nothing until something consumes the sequence. In the Rust example, the counter is 0 before `collect` and 3 after it.
- **Not every ordered type is indexed.** `in` and `index` on a lazy sequence or a linked list walk the items one by one. Code that takes a general sequence must not assume that item `i` is one step away.
- **Different meanings of the word.** A Python sequence has an index, a Kotlin `Sequence` is lazy, a Java `SequencedCollection` has two ends, and an SQL sequence is a counter. Read the definition in each language.

## Cost

| Kind | Item `i` | Length | Memory | Passes |
|---|---|---|---|---|
| array or array list | constant | constant | all items | many |
| linked list | linear in `i` | constant if stored | all items and links | many |
| range | constant | constant | three numbers | many |
| lazy sequence | linear in `i` (compute the items before it) | unknown, maybe infinite | the state of one step | often one |

An indexed sequence keeps all its items, so random access is fast and the memory grows with the length. A lazy sequence keeps only its state, so it can handle data that does not fit in memory, and it stops early when the consumer has enough. The cost is per item: each step calls a function, and the sequence cannot go back.

## History

The idea comes from mathematics, where a sequence is a function from the natural numbers, or from the first `n` of them, to a set of values[^1]. Programming languages took the idea in two forms. Arrays and lists, from the 1950s, store finite sequences. Lazy streams came later, in functional languages and then in the generators of Python, the iterators of C# 2.0 with `yield return`, and the streams of Java 8.

Recent languages added names for the shared interface. Java added `SequencedCollection` in Java 21[^4], and Go added `iter.Seq` in Go 1.23. The Go documentation says that the name `Seq` is pronounced like seek, the first syllable of sequence[^10].

## References
