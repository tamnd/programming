---
tag: B506TN
kind: concept
slug: collection-interfaces
name: Collection interfaces
aliases: [collection hierarchy, Iterable, Collection interface, Sequence and Mapping, collections.abc, IEnumerable, ICollection, programming to the interface]
area: data-and-values/collections
level: core
summary: >-
  Collection interfaces such as Iterable, Collection, Sequence and Mapping name what a kind of collection can do. Code written to the interface works with each class that implements it.
applies_to: core-languages
viz: optional
formal: none
see_also: [collection, abstract-data-type, interfaces, iterators, choose-a-collection, generic-containers, immutability, subtyping]
sources:
- url: https://docs.oracle.com/javase/tutorial/collections/interfaces/index.html
  title: Interfaces, The Java Tutorials (Collections)
  locator: Whole page
- url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/Collection.html
  title: Interface Collection, Java SE 21
  locator: Interface description
- url: https://docs.python.org/3.14/library/collections.abc.html
  title: collections.abc, Python 3.14 documentation
  locator: Introduction; Collections Abstract Base Classes
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.ienumerable-1
  title: IEnumerable<T> Interface, .NET API reference
  locator: Definition
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.icollection-1
  title: ICollection<T> Interface, .NET API reference
  locator: Remarks
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.ireadonlycollection-1
  title: IReadOnlyCollection<T> Interface, .NET API reference
  locator: Definition
- url: https://kotlinlang.org/docs/collections-overview.html
  title: Collections overview, Kotlin documentation
  locator: Read-only and mutable collections
- url: https://doc.rust-lang.org/std/iter/index.html
  title: Module std::iter, Rust standard library
  locator: for loops and IntoIterator
- url: https://doc.rust-lang.org/std/iter/trait.FromIterator.html
  title: Trait FromIterator, Rust standard library
  locator: Introduction
- url: https://pkg.go.dev/iter
  title: Package iter, Go standard library
  locator: Overview; type Seq
- url: https://www.typescriptlang.org/docs/handbook/iterators-and-generators.html
  title: Iterators and Generators, The TypeScript Handbook
  locator: Iterables
- url: https://en.cppreference.com/w/cpp/named_req/Container.html
  title: C++ named requirements, Container, cppreference.com
  locator: Introduction
- url: https://docs.oracle.com/javase/tutorial/collections/interfaces/collection.html
  title: The Collection Interface, The Java Tutorials
  locator: Whole page
- url: https://en.wikipedia.org/wiki/Java_collections_framework
  title: Java collections framework, Wikipedia
  locator: History
---

## Idea

A function that adds up some numbers does not care whether they come in a list, a set or the values of a map. It needs only one operation: give me the numbers one by one. A function that reports the size needs one more: tell me how many there are. Collection interfaces give names to these sets of operations. The weakest interface asks for the least, and more of the collections in the program pass it.

```python run id=idea
from collections.abc import Collection, Iterable, Mapping, Sequence


def total(xs: Iterable[int]) -> int:      # needs only a loop
    return sum(xs)


def describe(c: Collection[int]) -> str:  # needs a loop, len and in
    return f"size {len(c)}, total {total(c)}, has 3: {3 in c}"


prices = {"tea": 3, "cake": 4}
for c in ([1, 2, 3], {4, 5}, prices.values()):
    print(type(c).__name__, describe(c))

print(total(n * n for n in range(4)))     # a generator is Iterable only
print(isinstance([], Sequence), isinstance(prices, Mapping), isinstance({4}, Sequence))
```

```output for=idea
list size 3, total 6, has 3: True
set size 2, total 9, has 3: False
dict_values size 2, total 7, has 3: True
14
True True False
```

`total` takes any iterable, so it works with a generator too. `describe` takes a `Collection`, so it can call `len` and use `in`, but a generator does not pass. The `isinstance` tests show the finer interfaces: a list is a `Sequence`, a dict is a `Mapping`, and a set is not a `Sequence`.

## How it works

**A hierarchy of interfaces.** In Java, "The core collection interfaces encapsulate different types of collections"[^1], and "These interfaces allow collections to be manipulated independently of the details of their representation"[^1]. They "form a hierarchy"[^1]: `Collection` is "The root interface in the collection hierarchy"[^2], `List`, `Set` and `Queue` extend it, and `SortedSet` extends `Set`. Each level adds operations. Most languages have the same three levels:

- an iterable: the elements can be visited one by one;
- a collection: the elements can also be counted and tested for membership;
- a sequence, a set or a map: each adds its own operations, such as access by index, set algebra or lookup by key.

**Python: abstract base classes.** The module `collections.abc` gives the levels as classes. `Iterable` needs `__iter__`. `Collection` inherits from `Sized`, `Iterable` and `Container`, so it needs `__len__`, `__iter__` and `__contains__`[^3]. `Sequence`, `Set` and `Mapping` inherit from `Collection`, and each has a mutable form: `MutableSequence`, `MutableSet` and `MutableMapping`[^3]. A class that inherits from one of them writes the abstract methods and gets the rest: "The remaining mixin methods come from inheritance and can be overridden if desired"[^3].

**C# and Kotlin.** In .NET, `IEnumerable<T>` "Exposes the enumerator, which supports a simple iteration over a collection of a specified type"[^4]. "The ICollection<T> interface extends IEnumerable<T>; IDictionary<TKey,TValue> and IList<T> are more specialized interfaces that extend ICollection<T>"[^5]. .NET also has a read-only family: `IReadOnlyCollection<T>` "Represents a strongly-typed, read-only collection of elements"[^6], and `IReadOnlyList<T>` and `IReadOnlyDictionary<TKey,TValue>` build on it. In Kotlin, "Collection<T> is the common superinterface of List and Set"[^7], and "Each main collection type has a read-only interface, such as List, and a corresponding mutable interface, such as MutableList"[^7].

**Rust, Go and C++: smaller pieces.** Rust has no `Collection` trait. It has a trait for each ability. A `for` loop works with any type that implements `IntoIterator`, and "This trait has one method, into_iter, which converts the thing implementing IntoIterator into an iterator"[^8]. `FromIterator` is the other direction: "By implementing FromIterator for a type, you define how it will be created from an iterator"[^9]. Go has no collection interfaces. Its `iter` package defines `Seq`, "an iterator over sequences of individual values"[^10], and functions in the library give a `Seq` for the built-in collections: "maps.Keys returns an iterator over the keys of a map"[^10]. In TypeScript and JavaScript, "An object is deemed iterable if it has an implementation for the Symbol.iterator property"[^11]. C++ writes its interfaces as requirements on template arguments, not as base classes: "A Container is an object used to store other objects and taking care of the management of the memory used by the objects it contains"[^12].

**Programming to the interface.** A parameter, a field or a return type should name the weakest interface that the code needs. "The Collection interface is used to pass around collections of objects where maximum generality is desired"[^13]. For example, each general-purpose Java collection has a constructor that takes a `Collection`. "This constructor, known as a conversion constructor, initializes the new collection to contain all of the elements in the specified collection, whatever the given collection's subinterface or implementation type"[^13]. The caller can then pass a list, a set or a queue.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `collections.abc`: `Iterable`, `Collection`, `Sequence`, `Set`, `Mapping` and their mutable forms |
| JavaScript | native | the iterable protocol (`Symbol.iterator`); no interface names |
| TypeScript | native | `Iterable<T>`, `ReadonlyArray<T>`, `ReadonlySet<T>`, `ReadonlyMap<K, V>` as structural types |
| Java | stdlib | `Iterable`, `Collection`, `List`, `Set`, `Queue`, `Deque`, `Map` in `java.lang` and `java.util` |
| Kotlin | stdlib | `Iterable`, `Collection`, `List`, `Set`, `Map` and the `Mutable` forms |
| C# | stdlib | `IEnumerable<T>`, `ICollection<T>`, `IList<T>`, `IDictionary<TKey,TValue>` and the `IReadOnly` forms |
| Go | stdlib | no collection interfaces; `iter.Seq` and `iter.Seq2` for iteration |
| Rust | stdlib | traits for each ability: `IntoIterator`, `FromIterator`, `Extend`, `Index` |
| C | absent | no interfaces; a library can use a struct of function pointers |
| C++ | stdlib | named requirements and concepts, such as `std::ranges::range`, checked on templates |
| Bash | absent | no interfaces; arrays only |
| PowerShell | stdlib | the .NET interfaces, such as `IEnumerable` and `ICollection` |
| SQL | n/a | a query works on tables, not on collection interfaces |
| Regex | n/a | a pattern has no collections |
| HTML | n/a | HTML has no collections in program code |
| CSS | n/a | CSS has no collections in program code |

Java, C#, Kotlin and Python have a full hierarchy of named interfaces. TypeScript has the same ideas as structural types, so a class needs no `implements` clause to match. Rust and C++ check the abilities at compile time through traits and concepts, and Go asks only for an iterator. C and Bash have no way to name a collection interface.

The example files write two functions in each language: `total`, which needs only iteration, and `describe`, which also needs the size. Each file calls them with a list, a set and the values of a map. Python uses `collections.abc`. JavaScript passes any iterable. TypeScript uses `Iterable<number>` and `ReadonlySet<number>`. Java uses `Iterable<Integer>` and `Collection<Integer>`. C# uses `IEnumerable<int>` and `IReadOnlyCollection<int>`. Go uses `iter.Seq[int]`. Rust uses `IntoIterator` and `ExactSizeIterator`. C++ uses the concepts `std::ranges::input_range` and `std::ranges::sized_range`. C and Bash have no interfaces, so their files show the usual workaround: a struct of function pointers in C, and a function that takes the name of an array in Bash.

## Variations and related ideas

- **Nominal and structural.** In Java, C# and Kotlin, a class matches an interface only when it declares it. In TypeScript and Go, a type matches when it has the right members. Python allows both: a class can inherit from an ABC, or be registered as a virtual subclass, and some ABCs such as `Iterable` match any class with the right methods[^3].
- **Read-only interfaces.** A read-only interface gives no way to change the collection. It is a promise of the code that takes the parameter, not of the collection itself. See [immutability](/immutability/).
- **Optional operations.** Java keeps one interface for each kind and makes some methods optional. "To keep the number of core collection interfaces manageable, the Java platform doesn't provide separate interfaces for each variant of each collection type"[^1]. An unmodifiable list implements `List`, but its `add` throws an exception.
- **Iterators.** The lowest level of the hierarchy is the iterator protocol, which also covers generators and streams. See [iterators](/iterators/).

## Pitfalls

- **Asking for too much.** A parameter of type `List` or `ArrayList` refuses a set or a queue, even when the function only loops over its argument. Ask for `Iterable` or `Collection`, or for the read-only form.
- **A map is not a collection.** In Java, "the hierarchy consists of two distinct trees"[^1], and a `Map` does not extend `Collection`. In Kotlin, "Map doesn't extend Collection"[^7]. Pass `keySet()`, `values()` or `entrySet()` when the code needs a collection.
- **Read-only is not immutable.** Another reference to the same collection can still change it. Kotlin says this plainly: "By contrast, an immutable collection doesn't allow you to change its elements after creation"[^7]. Copy the collection, or use an immutable one, when the elements must not change.
- **An iterable that runs once.** A Python generator passes the `Iterable` test, but a second loop over it finds nothing. A function that loops twice must ask for a `Collection`, or copy its argument into a list first.
- **Mixins with the wrong cost.** A mixin method is correct for every class but fast only for some. In `collections.abc`, if `__getitem__` "is linear (as it would be with a linked list), the mixins will have quadratic performance and will likely need to be overridden"[^3].

## Cost

A call through an interface in Java, C# or Kotlin is a virtual call, because the code does not know the class of the collection. A generic function in Rust or C++ is compiled for each concrete type, so its calls through `IntoIterator` or a range concept are direct. See [templates and generics](/templates-and-generics/). In Python, the ABCs cost nothing until the code tests them: they are classes "that can be used to test whether a class provides a particular interface"[^3], and the test runs only when the code calls `isinstance` or `issubclass`.

## History

"The collections framework was designed and developed primarily by Joshua Bloch, and was introduced in JDK 1.2"[^14]. "Core collection interfaces are the foundation of the Java Collections Framework"[^1]. Python moved its abstract collection classes to their own module, `collections.abc`; before that, "this module was part of the collections module"[^3].
