---
tag: D93GJH
kind: concept
slug: data-structure
name: Data structure
aliases: [data structures, concrete data structure, representation, contiguous structure, linked structure]
area: data-and-values/collections
level: intro
summary: >-
  A data structure is a way to lay out data in memory so that some operations are fast. Arrays, linked lists, hash tables and trees store the same elements at different costs.
applies_to: core-languages
viz: optional
formal: none
see_also: [collection, abstract-data-type, choose-a-collection, array, dynamic-array, linked-list, hash-table, tree-concept]
same_as:
  wikidata: Q175263
  wikipedia: Data_structure
sources:
- url: https://xlinux.nist.gov/dads/HTML/datastructur.html
  title: data structure, Dictionary of Algorithms and Data Structures, NIST
  locator: Definition; Note
- url: https://en.wikipedia.org/wiki/Data_structure
  title: Data structure, Wikipedia
  locator: Introduction; Usage; Implementation; Examples
- url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/doc-files/coll-overview.html
  title: Collections Framework Overview, Java SE 21
  locator: Collection Implementations
- url: https://docs.python.org/3.14/faq/design.html
  title: Design and History FAQ, Python 3.14 documentation
  locator: How are lists implemented in CPython?; How are dictionaries implemented in CPython?
- url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/LinkedList.html
  title: Class LinkedList, Java SE 21
  locator: Class description
- url: https://doc.rust-lang.org/std/collections/index.html
  title: Module std::collections, Rust standard library
  locator: Performance; Cost of Collection Operations
- url: https://docs.python.org/3.14/library/collections.abc.html
  title: collections.abc, Python 3.14 documentation
  locator: class collections.abc.Sequence
- url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/ArrayList.html
  title: Class ArrayList, Java SE 21
  locator: Class description
- url: https://en.wikipedia.org/wiki/Algorithms_%2B_Data_Structures_%3D_Programs
  title: Algorithms + Data Structures = Programs, Wikipedia
  locator: Introduction
---

## Idea

The same five numbers can sit in memory in more than one way. An array puts them side by side, so the program finds the fourth number by arithmetic on its address. A linked list puts each number in its own node with a pointer to the next node, so the program must follow three pointers to reach the fourth number. But the linked list can add a number at the front without moving the others. A data structure is such a layout, together with the code that keeps it in order.

```python run id=idea
class Node:
    def __init__(self, value, next=None):
        self.value = value
        self.next = next


array = [10, 20, 30, 40, 50]
linked = None
for v in reversed(array):
    linked = Node(v, linked)


def linked_get(head, i):
    steps = 0
    while i > 0:
        head, i, steps = head.next, i - 1, steps + 1
    return head.value, steps


print("array:", array[3], "after 0 pointer steps")
print("linked:", *linked_get(linked, 3), "pointer steps")

linked = Node(5, linked)  # add at the front: no element moves
array.insert(0, 5)        # add at the front: every element moves one place
print(linked.value, array)
```

```output for=idea
array: 40 after 0 pointer steps
linked: 40 3 pointer steps
5 [5, 10, 20, 30, 40, 50]
```

Both hold the same values and both answer "get element 3" and "add at the front". The cost of each answer differs. Reading by position is fast in the array and slow in the linked list. Adding at the front is the other way round.

## How it works

**A layout for some operations.** NIST defines a data structure as "An organization of information, usually in memory, for better algorithm efficiency"[^1]. Wikipedia says it is "a way to organize and store data that is usually chosen for efficient access to data"[^2]. No layout is fast for every operation, so the choice of a data structure is a choice of which operations to make cheap.

**Contiguous and linked.** Most data structures are built from two methods. In the first, "arrays and records store elements in contiguous memory locations, requiring a rigid layout but allowing fast indexed access by computing the address through arithmetic operations"[^2]. In the second, "linked data structures (such as linked lists and trees) store addresses of related elements within their structure, enabling flexible memory usage and dynamic resizing"[^2]. A hash table combines them: it keeps an array of slots, and it uses "a hashing function to map keys to indexes in an array, allowing for constant-time access in the average case"[^2].

**Rules that the code keeps.** "Most data structures have associated algorithms to perform operations, such as search, insert, or balance, that maintain the properties of the data structure"[^1]. A sorted array stays sorted after each insert. A balanced tree rotates its nodes so that no path becomes too long. A data structure "may include redundant information, such as length of the list or number of nodes in a subtree"[^1], and each operation must keep that information correct.

**The structure behind a collection.** A [collection](/collection/) or an [abstract data type](/abstract-data-type/) says which operations exist. The data structure is how they are done. "There may be multiple concrete data structures for the same ADT, for example a linked list or a resizable array for the list ADT"[^2]. Java puts the data structure in the class name: the general-purpose classes are hash tables (`HashSet`, `HashMap`), resizable arrays (`ArrayList`, `ArrayDeque`), balanced trees (`TreeSet`, `TreeMap`) and linked lists (`LinkedList`)[^3]. Python does not show the structure in the name, but its documentation says it. CPython lists "are really variable-length arrays, not Lisp-style linked lists"[^4], and CPython dictionaries "are implemented as resizable hash tables"[^4].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `list` is a dynamic array, `dict` and `set` are hash tables; `collections.deque` and `heapq` for others; classes for custom structures |
| JavaScript | native | `Array`, `Map`, `Set` and typed arrays; objects and classes for custom structures |
| TypeScript | native | the JavaScript structures; classes and interfaces with item types for custom structures |
| Java | stdlib | `ArrayList`, `LinkedList`, `ArrayDeque`, `HashMap`, `TreeMap`, `PriorityQueue`; classes for custom structures |
| Kotlin | stdlib | `ArrayList`, `ArrayDeque`, `HashMap`, `LinkedHashMap`; the Java classes on the JVM |
| C# | stdlib | `List<T>`, `LinkedList<T>`, `Dictionary<TKey,TValue>`, `SortedDictionary<TKey,TValue>`, `PriorityQueue<TElement,TPriority>` |
| Go | native | arrays, slices and maps in the language; structs and pointers for custom structures |
| Rust | stdlib | `Vec`, `VecDeque`, `LinkedList`, `HashMap`, `BTreeMap`, `BinaryHeap`; structs, enums and `Box` for custom structures |
| C | idiom | arrays, structs and pointers; each structure is written by hand or taken from a library |
| C++ | stdlib | `std::vector`, `std::deque`, `std::list`, `std::unordered_map`, `std::map`, `std::priority_queue` |
| Bash | idiom | indexed and associative arrays; other structures are built from arrays of indices |
| PowerShell | stdlib | arrays and hash tables, and the .NET classes |
| SQL | n/a | the database chooses the storage of a table; indexes are outside the language of a query |
| Regex | n/a | a pattern does not store data |
| HTML | n/a | HTML has no data structures in program code |
| CSS | n/a | CSS has no data structures in program code |

Each language has an array and a hash table that are ready to use. Java, C#, Rust and C++ add linked lists, trees and heaps in the standard library. C has only arrays, structs and pointers, so a C program builds the rest itself.

The example files build the same two structures as the idea: an array and a singly linked list of five numbers. Each one reads element 3 from both and counts the pointer steps, then adds a number at the front of the list. Python, JavaScript and TypeScript use a class for the node. Java and C# use a small class with a `next` field. Go and C use a struct and a pointer. Rust uses `Option<Box<Node>>`. C++ uses a struct with `std::unique_ptr`. Bash keeps the values and the next indices in two arrays.

## Variations and related ideas

- **Static and dynamic.** A fixed array has a size that does not change. A [dynamic array](/dynamic-array/) moves to a bigger block when it is full. A linked structure grows one node at a time.
- **Persistent structures.** Some structures keep the old version after an update, so that both versions stay valid. NIST lists the persistent data structure as one kind of data structure[^1].
- **The structure in the API.** Java names its classes by structure, and Rust names `Vec`, `VecDeque`, `HashMap` and `BTreeMap` by structure too. Python, JavaScript and Go give one name for each kind, such as `list` or `map`, and hide the structure.

## Pitfalls

- **Picking a linked list for fast inserts.** A linked list inserts in O(1) only when the code already holds the node. To find the place, "Operations that index into the list will traverse the list from the beginning or the end, whichever is closer to the specified index"[^5]. The Rust documentation says that "Vec is generally going to be faster than VecDeque, and VecDeque is generally going to be faster than LinkedList"[^6] when the costs in its table are the same.
- **Code that assumes the wrong structure.** An operation that is fast on one structure can be slow on another. The Python sequence mixins call `__getitem__` many times. If that method "is linear (as it would be with a linked list), the mixins will have quadratic performance"[^7].
- **Average cost is not worst cost.** A hash table is fast on average. "It is theoretically possible, though very unlikely, for HashMap to experience significantly worse performance than the expected cost"[^6].
- **Calling a structure by the name of a type.** A stack is a type with push and pop. An array and a linked list are two structures that can give it. Choose the type first, and then the structure. See [abstract data type](/abstract-data-type/).

## Cost

The cost of an operation depends on the structure, not on the name of the collection. For a Java `ArrayList`, "The add operation runs in amortized constant time, that is, adding n elements requires O(n) time"[^8], `get` and `set` take constant time, and "All of the other operations run in linear time (roughly speaking)"[^8]. For CPython lists, the array layout "makes indexing a list a[i] an operation whose cost is independent of the size of the list or the value of the index"[^4]. The Rust table gives O(1) for `get(i)` on a `Vec`, O(min(i, n-i)) on a `LinkedList`, and O(log(n)) for a lookup in a `BTreeMap`[^6]. Layout matters too: "the contiguous memory allocation in arrays facilitates rapid access and modification operations, leading to optimized performance in sequential data processing scenarios"[^2]. The analysis of these costs is on [the algorithm site](https://algorithm.tamnd.com/).

## History

*Algorithms + Data Structures = Programs* is "a 1976 book written by Niklaus Wirth"[^9]. Its title states the idea of this page: a program is made of its algorithms and of the data structures that they work on. Wikipedia reports that Rob Pike said that the choice of data structure almost always has a greater effect on efficiency than the choice of algorithm[^2].
