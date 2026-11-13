---
tag: 1MGHHC
kind: concept
slug: linked-list
name: Linked list
aliases: [singly linked list, doubly linked list, circular linked list, intrusive list, cons list, LinkedList, std::list, container/list]
area: data-and-values/arrays-and-lists
level: core
summary: >-
  A linked list keeps each item in its own node with a link to the next node. It inserts and removes at a known node in constant time, but it reaches item i only by following i links.
applies_to: core-languages
viz: optional
formal: none
see_also: [list, dynamic-array, array, pointers-and-references, iterators, ownership, garbage-collection, null-references]
sources:
- url: https://en.wikipedia.org/wiki/Linked_list
  title: Linked list, Wikipedia
  locator: Introduction; History; Basic concepts and nomenclature; Tradeoffs; Related data structures
- url: https://docs.kernel.org/core-api/list.html
  title: Linked Lists in Linux, The Linux Kernel documentation
  locator: Linux implementation of doubly linked lists; Declaring a node
- url: https://en.cppreference.com/cpp/container/list
  title: std::list, cppreference.com
  locator: Introduction
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/LinkedList.html
  title: Class LinkedList, Java SE 25 API
  locator: Class description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.linkedlist-1?view=net-10.0
  title: LinkedList<T> Class, .NET API reference
  locator: Remarks
- url: https://pkg.go.dev/container/list
  title: Package list, Go standard library
  locator: Overview
- url: https://en.cppreference.com/cpp/container/forward_list
  title: std::forward_list, cppreference.com
  locator: Introduction
- url: https://doc.rust-lang.org/std/collections/struct.LinkedList.html
  title: Struct LinkedList, The Rust Standard Library
  locator: Description
- url: https://doc.rust-lang.org/std/collections/index.html
  title: Module collections, The Rust Standard Library
  locator: Use a LinkedList when
- url: https://docs.python.org/3/library/collections.html
  title: collections, The Python Standard Library
  locator: deque objects
- url: https://github.com/python/cpython/blob/main/Modules/_collectionsmodule.c
  title: Modules/_collectionsmodule.c, CPython source code
  locator: deque objects, the comment on the block list
- url: https://en.wikipedia.org/wiki/Information_Processing_Language
  title: Information Processing Language, Wikipedia
  locator: Introduction
---

## Idea

A linked list stores a sequence as a chain of separate nodes. Wikipedia defines it: "a linked list is a linear collection of data elements whose order is not given by their physical placement in memory. Instead, each element points to the next"[^1]. It adds: "In its most basic form, each node contains data, and a reference (in other words, a link) to the next node in the sequence"[^1]. The program keeps a reference to the first node, the head, and the last node has an empty link, such as `null`, `None` or `NULL`.

The order lives in the links, not in the memory addresses. So the program can add or remove an item by changing one or two links, and no other item moves. But the program cannot go to item `i` in one step. It starts at the head and follows `i` links.

```python run id=idea
# Each node holds one item and a link to the next node. None ends the list.
class Node:
    def __init__(self, value, next=None):
        self.value = value
        self.next = next

def show(head):
    out = []
    while head is not None:
        out.append(head.value)
        head = head.next
    return out

# Build 1 -> 2 -> 4 by adding at the front, which changes only one link.
head = None
for v in (4, 2, 1):
    head = Node(v, head)
print(show(head))

# Insert 3 after the node that holds 2. The program needs that node, then it changes two links.
two = head.next
two.next = Node(3, two.next)
print(show(head))

# Remove the node after 1. No item moves; the link skips the node.
head.next = head.next.next
print(show(head))

# Reaching item i means following i links from the head.
def nth(head, i):
    steps = 0
    while i > 0:
        head, i, steps = head.next, i - 1, steps + 1
    return head.value, steps

print(nth(head, 2))
```

```output for=idea
[1, 2, 4]
[1, 2, 3, 4]
[1, 3, 4]
(4, 2)
```

Each change touches only the links next to it. The insert makes one node and changes the link of the node that holds 2. The remove changes the link of the head, so the node that holds 2 is no longer in the chain. To read item 2, `nth` follows two links.

## How it works

**Nodes and links.** A node is a record with an item and one or more links. A link is a [pointer or reference](/pointers-and-references/) to another node. The head is a reference to the first node, and an empty list is a head with no node. Many implementations also keep a reference to the last node, the tail, so that an append takes one step, and a count, so that the length takes one step.

**Singly and doubly linked.** In a singly linked list, each node links to the next node only, so a walk goes in one direction. Wikipedia describes the other form: "In a 'doubly linked list', each node contains, besides the link to the next node, a second link field pointing to the 'previous' node in the sequence"[^1]. With the second link, a program can walk backward and can remove a node when it has only that node. In a singly linked list, a remove needs the node before the one to remove.

**Insert and remove.** To insert node `n` after node `p`, set the link of `n` to the old next node of `p`, then set the link of `p` to `n`. To remove the node after `p`, set the link of `p` to the node after that one. In a doubly linked list, the program also updates the back links. The order of these steps matters: if the program changes the link of `p` first, it loses the rest of the list.

**Circular lists and sentinels.** In a circular list, the last node links back to the first node: "It is a list where the last node pointer points to the first node"[^1]. A sentinel is an extra node that holds no item. Wikipedia explains the reason: "This convention simplifies and accelerates some list-handling algorithms, by ensuring that all links can be safely dereferenced"[^1]. With a sentinel, every list, even an empty one, has a first and a last node. The Linux kernel uses both. Its list "is circular in nature. This means that to get from the head node to the tail, we can just travel one edge backwards"[^2], and the list head is a node with no item. The C example uses this design: an insert and a remove need no special case for the ends.

**Internal and external storage.** A list can keep the item in the node or keep a reference to the item. Wikipedia calls these "internal storage" and "external storage"[^1]. An intrusive list goes further: the link fields are part of the item. The kernel declares a node "by adding a struct list_head member to the data structure you wish to be contained in the list", and explains: "Linux chooses this approach because it allows for generic list modification code regardless of what data structure is contained within the list"[^2]. An intrusive list makes no allocation for an insert, and one item can be in several lists at the same time.

**Iterators and positions.** A position in a linked list is a reference to a node, not an index. Because no item moves, a position stays good while the program changes other parts of the list. cppreference says of `std::list`: "Adding, removing and moving the elements within the list or across several lists does not invalidate the iterators or references. An iterator is invalidated only when the corresponding element is deleted"[^3]. A program must still take care when it removes the node that it stands on. See [iterators](/iterators/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | a class with `value` and `next`; `collections.deque` is a doubly linked list of blocks |
| JavaScript | idiom | objects such as `{ value, next }`; no built-in linked list |
| TypeScript | idiom | a node type such as `{ value: T; next: Node<T> \| null }` |
| Java | stdlib | `java.util.LinkedList`, a doubly linked `List` and `Deque` |
| Kotlin | stdlib | `java.util.LinkedList` on the JVM; `ArrayDeque` is the usual queue |
| C# | stdlib | `LinkedList<T>` with `LinkedListNode<T>` nodes |
| Go | stdlib | `container/list`, a doubly linked list of `*Element` nodes |
| Rust | stdlib | `std::collections::LinkedList`; a hand-written list uses `Option<Box<Node>>` |
| C | idiom | a `struct` with a pointer to the next node, or an intrusive `list_head` |
| C++ | stdlib | `std::list` (doubly linked) and `std::forward_list` (singly linked) |
| Bash | n/a | no references between values; indexed arrays only |
| PowerShell | stdlib | `[System.Collections.Generic.LinkedList[T]]` from .NET |
| SQL | idiom | a table with a column that holds the key of the next row, read with a recursive query |
| Regex | n/a | a pattern matches text and has no data structures |
| HTML | n/a | markup; the DOM links sibling nodes, but HTML has no list type |
| CSS | n/a | style rules have no data structures |

The library lists are doubly linked. Java's `LinkedList` is a "Doubly-linked list implementation of the List and Deque interfaces", and "Operations that index into the list will traverse the list from the beginning or the end, whichever is closer to the specified index"[^4]. .NET says: "LinkedList<T> provides separate nodes of type LinkedListNode<T>, so insertion and removal are O(1) operations"[^5]. Go says: "Package list implements a doubly linked list"[^6]. C++ has both forms: `std::forward_list` "is implemented as a singly-linked list. Compared to std::list this container provides more space efficient storage when bidirectional iteration is not needed"[^7].

Rust has a linked list but tells programmers not to use it in most cases: "It is almost always better to use Vec or VecDeque because array-based containers are generally faster, more memory efficient, and make better use of CPU cache"[^8]. Its collection guide gives the cases for a `LinkedList`, and the last one is: "You are absolutely certain you really, truly, want a doubly linked list"[^9]. Python has no plain linked list class. Its deque supports "memory efficient appends and pops from either side of the deque with approximately the same O(1) performance in either direction"[^10], and it is built from linked blocks. The source says: "Data for deque objects is stored in a doubly-linked list of fixed length blocks. This assures that appends or pops never move any other data elements besides the one being appended or popped"[^11].

The example files show these forms. Python shows `collections.deque` at both ends, `rotate` and `maxlen`. JavaScript builds a list from plain objects, reverses it in place and finds a loop with a slow and a fast pointer. Java shows `LinkedList` as a list and a deque, a `ListIterator` that inserts as it walks and the exception when a loop changes the list behind its iterator. C# shows `LinkedListNode<T>`, `AddAfter` and a node that belongs to one list at a time. Go shows `container/list` with `MoveToFront` and the correct way to remove nodes while it walks. Rust writes a stack of `Option<Box<Node>>` and reverses it, then joins two `LinkedList` values. C builds an intrusive circular list with a sentinel, as the Linux kernel does. C++ shows `splice`, `erase` in a loop, `forward_list` and `list::sort`.

## Variations and related ideas

- **Cons lists.** The [list](/list/) of Lisp and Haskell is a singly linked list of immutable pairs. A new item at the front shares the old list as its tail. Wikipedia says that "if a new node is added at the beginning of a list, the former list remains available as the tail of the new one"[^1].
- **Stacks and queues.** "Both stacks and queues are often implemented using linked lists, and simply restrict the type of operations which are supported"[^1]. A stack uses the head only. A queue adds at the tail and removes at the head.
- **Unrolled lists.** "An unrolled linked list is a linked list in which each node contains an array of data values"[^1]. The Python deque is an example. It uses fewer links and keeps nearby items together in memory.
- **Skip lists.** "The skip list is a linked list augmented with layers of pointers for quickly jumping over large numbers of elements"[^1]. A search takes logarithmic time on average.
- **Arrays of nodes.** A program can keep the nodes in an [array](/array/) and use indices as links. Wikipedia notes that the link fields "may be stored in a separate array with the same indices as the data records"[^1]. This form needs no allocation per node and works in languages without pointers.
- **XOR lists.** "A technique known as XOR-linking allows a doubly linked list to be implemented using a single link field in each node", but it "requires the ability to do bit operations on addresses"[^1].

## Pitfalls

- **Indexing in a loop.** `get(i)` on a Java `LinkedList` walks from an end, so a `for` loop over `i` takes quadratic time. Use an iterator.
- **A lost tail.** An insert that changes the link of the old node before it saves the old next link cuts the list. Save the next link first.
- **A remove during a walk.** In Go, `Remove` clears the links of the removed element, so a loop that calls `e.Next()` after `Remove(e)` stops at once. The Go example removes 1 of 3 items with this bug. Save the next element first. In Java, a change to the list outside the iterator makes the next call to the iterator throw `ConcurrentModificationException`. In C++, use the iterator that `erase` returns.
- **Null links.** Code that follows a link without a check fails at the end of the list or on an empty list. A sentinel removes this case. See [null references](/null-references/).
- **Cycles.** A bug that links a node back to an earlier node makes a walk loop forever. Floyd's test moves one pointer by one link and another by two links, and they meet only if the list has a cycle. The JavaScript example shows it.
- **Memory in C.** Each node is a separate allocation. A program must free each node once, must not use a node after it frees it, and must not free a node that is still in a list. See [use after free](/use-after-free/).
- **Ownership in Rust.** A doubly linked list has two links to each node, so it does not fit the rule of one owner. Safe Rust code uses `Option<Box<Node>>` for a singly linked list, and the standard `LinkedList` uses unsafe code inside. See [ownership](/ownership/).
- **Deep recursion.** A recursive function, or a destructor that drops one node and then the next, uses one stack frame per node. A long list can overflow the stack.

## Cost

| Operation | Singly linked | Doubly linked | Dynamic array |
|---|---|---|---|
| get item `i` | linear in `i` | linear in the distance to the nearer end | constant |
| add or remove at the front | constant | constant | linear |
| add at the end | constant with a tail reference | constant | amortized constant |
| insert or remove at a known node | constant (after a node) | constant | linear |
| find a value | linear | linear | linear |
| memory per item | the item and one link | the item and two links | the item |

The table hides the cost of memory access. Wikipedia says: "Arrays have better cache locality compared to linked lists"[^1]. A walk over a linked list loads one node at a time from places all over memory, while a walk over an array reads one block. The links also cost space: Wikipedia notes that this "often makes them impractical for lists of small data items such as characters or Boolean values, because the storage overhead for the links may exceed" the size of the data[^1]. For most work, a [dynamic array](/dynamic-array/) is faster even for inserts in the middle, until the list is large. A linked list wins when the program keeps references to nodes and inserts or removes at those nodes, as in a cache that moves each used item to the front.

## History

Linked lists are older than most programming languages. Wikipedia says: "The first implementation of linked listing in the computer-science context was developed in 1955–1956 by Allen Newell, Cliff Shaw, and Herbert A. Simon at RAND Corporation and Carnegie Mellon University as the primary data structure for their Information Processing Language (IPL)"[^1]. IPL "included features to facilitate AI programming, specifically problem solving, such as lists, dynamic memory allocation, data types, recursion"[^12]. The authors used it for early artificial intelligence programs such as the Logic Theory Machine[^1]. Newell and Simon later won the ACM Turing Award for having "made basic contributions to artificial intelligence, the psychology of human cognition, and list processing"[^1].

IPL influenced Lisp[^12], which made the singly linked list of cons cells its main data structure in 1958. See [list](/list/). File systems and operating systems also used linked lists: "Many modern operating systems use doubly linked lists to maintain references to active processes, threads, and other dynamic objects"[^1]. Later, the cost of cache misses grew compared to the cost of a computation, and libraries such as Rust's started to advise array-based containers for most uses[^8].

## References
