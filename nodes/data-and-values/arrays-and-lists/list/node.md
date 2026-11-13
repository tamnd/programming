---
tag: PE2ZXX
kind: concept
slug: list
name: List
aliases: [sequence, list type, List interface, IList, cons list, Lisp list, linked list, array list]
area: data-and-values/arrays-and-lists
level: core
summary: >-
  A list is a finite collection of items in order, where one value can appear more than once. It is a dynamic array in some languages and a linked list in others.
applies_to: core-languages
viz: optional
formal: none
see_also: [array, dynamic-array, interfaces, program-to-an-interface, immutability, sum-types, recursion, value-equality-and-identity]
sources:
- url: https://en.wikipedia.org/wiki/List_(abstract_data_type)
  title: List (abstract data type), Wikipedia
  locator: Introduction; Operations; Implementations; Programming language support; Abstract definition
- url: https://www.haskell.org/onlinereport/haskell2010/haskellch6.html
  title: Predefined Types and Classes, Haskell 2010 Language Report
  locator: 6.1.3 Lists
- url: https://www.lispworks.com/documentation/HyperSpec/Body/14_ab.htm
  title: Section 14.1.2 Conses as Lists, Common Lisp HyperSpec
  locator: 14.1.2
- url: https://docs.python.org/3/faq/design.html
  title: Design and History FAQ, Python documentation
  locator: How are lists implemented in CPython?
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/List.html
  title: Interface List, Java SE 25 API
  locator: Interface description; Unmodifiable Lists
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.ilist-1?view=net-10.0
  title: IList<T> Interface, .NET API reference
  locator: Definition; Remarks
- url: https://kotlinlang.org/docs/collections-overview.html
  title: Collections overview, Kotlin documentation
  locator: Collection types
- url: https://en.cppreference.com/w/cpp/container/list.html
  title: std::list, cppreference.com
  locator: Introduction
- url: https://pkg.go.dev/container/list
  title: Package list, Go standard library
  locator: Overview
- url: https://doc.rust-lang.org/std/collections/index.html
  title: Module collections, The Rust Standard Library
  locator: When Should You Use Which Collection?
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, The Python Standard Library
  locator: Sequence Types, list, tuple, range; Lists
- url: https://docs.python.org/3/tutorial/datastructures.html
  title: Data Structures, The Python Tutorial
  locator: 5.1 More on Lists; 5.3 Tuples and Sequences
- url: https://en.wikipedia.org/wiki/Lisp_(programming_language)
  title: Lisp (programming language), Wikipedia
  locator: Introduction; History
---

## Idea

A list is the simplest collection with an order. Wikipedia defines it: "a list or sequence is a collection of items that are finite in number and in a particular order", and "A list may contain the same value more than once, and each occurrence is considered a distinct item"[^1]. A shopping list, the lines of a file and the arguments of a command are lists. The order matters, so `[1, 2]` and `[2, 1]` are different lists, and the duplicates matter, so `[1, 1]` has two items.

The word has two uses in programming:

- an **abstract type**: a set of operations, such as add an item at the end, get the item at position `i` and get the number of items, with no rule about the storage;
- a **concrete type** in a language, with a fixed storage. In Python, Java `ArrayList`, C# `List<T>` and Rust `Vec`, the storage is a [dynamic array](/dynamic-array/). In Lisp and Haskell, a list is a chain of pairs, which is a [linked list](/linked-list/). Wikipedia notes: "In some contexts, such as in Lisp programming, the term list may refer specifically to a linked list rather than an array"[^1].

The two forms give the same answers but at different costs. In an array list, item `i` is one step away. In a chain of pairs, a new first item costs one step, but item `i` is `i` steps away.

```python run id=idea
# A list keeps items in order, and an item can appear more than once.
xs = ["b", "a", "b"]
xs.append("c")
xs.insert(0, "z")
print(xs, len(xs), xs[1], xs.count("b"))

# The same list as Lisp builds it: a chain of pairs (first, rest), ended by an empty list.
NIL = None

def cons(first, rest):
    return (first, rest)

def to_python(cell):
    out = []
    while cell is not NIL:
        first, cell = cell
        out.append(first)
    return out

ys = cons("b", cons("a", cons("b", NIL)))
print(to_python(ys), ys[0], to_python(ys[1]))

# Prepending makes one new pair and shares the old chain. The old list does not change.
zs = cons("z", ys)
print(to_python(zs), to_python(ys), zs[1] is ys)

# Item i of a chain needs i steps, but item i of a Python list needs one step.
def nth(cell, i):
    for _ in range(i):
        cell = cell[1]
    return cell[0]

print(nth(zs, 2), xs[2])
```

```output for=idea
['z', 'b', 'a', 'b', 'c'] 5 b 2
['b', 'a', 'b'] b ['a', 'b']
['z', 'b', 'a', 'b'] ['b', 'a', 'b'] True
a a
```

The Python list grows at both ends and keeps the two `"b"` items. The chain of pairs holds the same items. A pair has the first item and the rest of the list, so `ys[0]` is `"b"` and `ys[1]` is the list `["a", "b"]`. The longer list `zs` reuses all of `ys`, and `ys` does not change. To reach item 2, `nth` follows two links, but the Python list goes to the item at once.

## How it works

**The operations.** A list type gives some of these operations. Wikipedia lists them: "Create an empty list", "Test for the list being empty", "Prepend an item to the list", "Append an item at the list's end", "get the first or the last item of a list", "get the remainder of the list past its first or last item" and "get an item by index of its position in a list"[^1]. Most libraries add operations to insert and remove at a position, to find an item, to sort and to iterate.

**The abstract definition.** A list can also be defined by two constructors and two functions. Wikipedia gives the signature: "nil: () → L", "cons: E × L → L", "first: L → E" and "rest: L → L", "with the axioms first (cons (e, l)) = e rest (cons (e, l)) = l for any element e and any list l"[^1]. The empty list is `nil`, and `cons(e, l)` is a list whose first item is `e` and whose rest is `l`. Haskell defines its list type in this way: "data [a] = [] | a : [a]". The report says: "Lists are an algebraic datatype of two constructors", the null list `[]` and the cons operator `:`[^2]. A function on such a list handles the two cases: the empty list, and a first item with the rest. See [sum types](/sum-types/) and [recursion](/recursion/).

**Lisp lists.** Common Lisp defines a list as a chain of cons cells: "A list is a chain of conses in which the car of each cons is an element of the list, and the cdr of each cons is either the next link in the chain or a terminating atom. A proper list is a list terminated by the empty list"[^3]. Wikipedia describes this form as the standard one: "The standard way of implementing lists, originating with the programming language Lisp, is to have each element of the list contain both its value and a pointer indicating the location of the next element in the list"[^1].

**Array lists.** Most other languages keep a list in a dynamic array. The Python FAQ says: "CPython’s lists are really variable-length arrays, not Lisp-style linked lists"[^4]. Wikipedia says that lists "are typically implemented either as linked lists (either singly or doubly linked) or as arrays, usually variable length or dynamic arrays"[^1].

**Interfaces and types.** Some languages separate the list interface from its implementations, so a function can accept any list. Java's `List` is "An ordered collection, where the user has precise control over where in the list each element is inserted. The user can access elements by their integer index (position in the list), and search for elements in the list"[^5]. `ArrayList` and `LinkedList` implement it. C# `IList<T>` "Represents a collection of objects that can be individually accessed by index", and it "is the base interface of all generic lists"[^6]. Kotlin splits the interface in two: "Each main collection type has a read-only interface, such as List, and a corresponding mutable interface, such as MutableList"[^7]. Python names the interface with the abstract base classes `Sequence` and `MutableSequence`. See [interfaces](/interfaces/) and [program to an interface](/program-to-an-interface/).

An interface hides the cost of each operation. Java warns: "Note that these operations may execute in time proportional to the index value for some implementations (the LinkedList class, for example). Thus, iterating over the elements in a list is typically preferable to indexing through it if the caller does not know the implementation"[^5].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `list`, a mutable dynamic array; `tuple` for an immutable sequence; `Sequence` and `MutableSequence` as interfaces |
| JavaScript | native | `Array` is the list type; `toSorted`, `toReversed` and `with` return a changed copy |
| TypeScript | native | `T[]` and `ReadonlyArray<T>` over the JavaScript `Array` |
| Java | stdlib | the interface `java.util.List` with `ArrayList`, `LinkedList` and the unmodifiable `List.of` |
| Kotlin | stdlib | read-only `List<T>` and `MutableList<T>`, made with `listOf` and `mutableListOf` |
| C# | stdlib | the interface `IList<T>`, the class `List<T>`, `ReadOnlyCollection<T>` and arrays, which also implement `IList<T>` |
| Go | native | slices are the list type; `container/list` is a doubly linked list |
| Rust | stdlib | `Vec<T>` is the usual list; `LinkedList<T>` exists; a cons list is an `enum` with `Box` |
| C | idiom | no list type; an array with a count, or a struct with a pointer to the next item |
| C++ | stdlib | `std::vector` is the usual list; `std::list` and `std::forward_list` are linked lists |
| Bash | native | indexed arrays such as `a=(x y z)` and the list of words in `"$@"` |
| PowerShell | native | arrays `@(...)` and `[System.Collections.Generic.List[T]]` |
| SQL | idiom | rows of a table with an ordering column; `ORDER BY` gives the order, and a table has no order of its own |
| Regex | n/a | a pattern matches text and has no lists |
| HTML | native | `<ol>` and `<ul>` with `<li>` items display a list; they are markup, not a data type |
| CSS | n/a | style rules have no lists; `list-style` only styles HTML lists |

The name list means different things in different languages, so a reader must check the storage. In Python, JavaScript, Go and the usual Java and C# code, the list is an array, and item `i` costs one step. In Lisp, Scheme and Haskell, the list is a chain of cons cells. Wikipedia says: "In several dialects, including Scheme, a list is a set of pairs, consisting of a value and a pointer to the next pair (or null value), making a singly linked list"[^1]. C++ uses the name for a linked list. cppreference says that `std::list` "is a container that supports constant time insertion and removal of elements from anywhere in the container. Fast random access is not supported. It is usually implemented as a doubly-linked list"[^8]. Go has the same name in its library: "Package list implements a doubly linked list"[^9], but Go code uses slices for most lists.

The documentation of several languages says which list to choose. Rust says: "To get this out of the way: you should probably just use Vec or HashMap"[^10], and it gives the reasons to use a `Vec`: "You want a sequence of elements in a particular order, and will only be appending to (or near) the end"[^10]. Python has "three basic sequence types: lists, tuples, and range objects"[^11], and "Lists are mutable sequences, typically used to store collections of homogeneous items"[^11]. The tutorial separates the mutable list from the immutable tuple by use: "Tuples are immutable, and usually contain a heterogeneous sequence of elements that are accessed via unpacking", while "Lists are mutable, and their elements are usually homogeneous and are accessed by iterating over the list"[^12].

The example files show these forms. Python shows `list` and `tuple` as two sequences with the same read operations and the interface classes. JavaScript shows `Array` with `splice`, the copying methods and a frozen array. Java shows one function that accepts an `ArrayList` and a `LinkedList`, `equals` across the two and `List.of`. C# shows `IList<T>` over a `List<T>` and an array, a fixed-size array that refuses `Add` and a read-only wrapper. Go shows a slice with the `slices` package and `container/list`. Rust shows `Vec`, a cons list as an `enum` and `LinkedList`. C builds a cons list with `malloc` and shares its tail. C++ shows `std::vector`, `std::list` and `std::forward_list`.

## Variations and related ideas

- **Mutable and immutable lists.** An immutable list does not change after the program makes it. Java's `List.of` gives such a list, and "Calling any mutator method on the List will always cause UnsupportedOperationException to be thrown"[^5]. A read-only view, such as a Kotlin `List` or a C# `ReadOnlyCollection<T>`, only hides the mutators, and the list behind it can still change. See [immutability](/immutability/).
- **Persistent lists.** A cons list shares its tail, so a new list with one more item at the front costs one cell, and the old list stays the same. Functional languages use this property to keep old versions of a list at no extra cost.
- **Tuples.** A tuple also has an order, but it usually has a fixed length and items of different types, such as a point `(x, y)`.
- **Sets and multisets.** A list keeps duplicates and order. Wikipedia notes that "A finite set in the mathematical sense can be realized as a list with additional restrictions; that is, duplicate elements are disallowed and order is irrelevant"[^1].
- **Streams and lazy lists.** A lazy list makes its items when the program reads them, so it can be infinite. Wikipedia calls a stream "the potentially infinite analog of a list"[^1].
- **Arrays.** An [array](/array/) has a fixed length. A list can grow: "Unlike in an array, a list can expand and shrink"[^1].

## Pitfalls

- **The wrong idea of the cost.** `xs[i]` on a Lisp list, a Haskell list, a Java `LinkedList` or a C++ `std::list` walks `i` links. A loop that reads `get(i)` for each `i` on a linked list takes quadratic time. Iterate instead.
- **An insert at the front of an array list.** `insert(0, x)` on a Python list or `add(0, x)` on an `ArrayList` moves every item. A loop of such inserts takes quadratic time. Append and reverse, or use a deque.
- **A read-only view is not a copy.** In the C# example, the `ReadOnlyCollection` shows the new item 7 after the program adds it to the list behind it. Make a copy when the receiver must not see later changes.
- **An unmodifiable list throws when the program runs.** A `List.of` list in Java and an array used as an `IList<T>` in C# have the type of a mutable list, so the compiler accepts `add`, and the call fails at run time.
- **Equality.** Python `==` and Java `equals` compare the items in order, and Java's rule works across `ArrayList` and `LinkedList`. C# `Equals` on a `List<T>` and JavaScript `===` compare references. In Python, a list never equals a tuple with the same items.
- **Aliasing.** `ys = xs` makes a second name for one list. A change through one name is visible through the other. Copy the list when the code needs its own version. See [value equality and identity](/value-equality-and-identity/).

## Cost

| Operation | Array list | Singly linked list |
|---|---|---|
| get item `i` | constant | linear in `i` |
| add at the end | amortized constant | constant with a tail pointer, else linear |
| add at the front | linear | constant |
| insert after a known position | linear | constant |
| length | constant | constant if stored, else linear |
| memory per item | the item or a reference | the item and a link |

An array list keeps its items next to each other, so a scan uses the cache well. A linked list needs one allocation for each item, and each item can be anywhere in memory. For most work, an array list is faster, and the Rust documentation tells programmers to use `Vec` unless they have a specific reason[^10].

## History

Lists came before arrays in high-level programming. Wikipedia says that John McCarthy "began developing Lisp in 1958", and that "He was inspired by Information Processing Language, which was also based on list processing"[^13]. It also says: "The name LISP derives from "List Processor". Linked lists are one of Lisp's major data structures, and Lisp source code is made of lists"[^13]. The names of the two parts of a cons cell come from the IBM 704: "car (Contents of the Address part of Register number) and cdr (Contents of the Decrement part of Register number)"[^13], and "Lisp dialects still use car and cdr" for the first item and the rest of a list[^13].

Haskell and the ML family kept the cons list as the basic list type[^2]. Python, Java, C# and JavaScript chose arrays for their default lists, and the Python FAQ explains why indexing is fast: "This makes indexing a list a[i] an operation whose cost is independent of the size of the list or the value of the index"[^4]. Java added the `List` interface in the Collections Framework, so that one function can accept both forms[^5].

## References
