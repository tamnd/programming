---
tag: QYEZ3S
kind: concept
slug: deque
name: Deque
aliases: [double-ended queue, dequeue at both ends, ArrayDeque, VecDeque, collections.deque, std::deque, deck]
area: data-and-values/stacks-queues-and-trees
level: core
summary: >-
  A deque, or double-ended queue, adds and removes items at both ends. It can serve as a stack and as a queue, and most libraries build it on a ring buffer or on linked blocks.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1965
  people: [Donald Knuth]
  note: A CTSS technical note of 1965 already had subroutines that added users at both ends of a queue. Knuth described deques with stacks and queues in The Art of Computer Programming, volume 1, section 2.2.1.
see_also: [stack-data-structure, queue-concept, linked-list, dynamic-array, iterator-invalidation, list, rotate-a-list, iterators]
same_as:
  wikidata: Q1192005
  wikipedia: Double-ended_queue
sources:
  - url: https://en.wikipedia.org/wiki/Double-ended_queue
    title: Double-ended queue, Wikipedia
    locator: the introduction; Implementations; Language support; Applications
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/util/Deque.html
    title: Deque (Java SE 27 and JDK 27)
    locator: the interface description
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/collections/struct.VecDeque.html
    title: Struct VecDeque, The Rust Standard Library
    locator: the description; rotate_left
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/container/deque
    title: std::deque, cppreference.com
    locator: the introduction
    accessed: 2026-11-16
  - url: https://pkg.go.dev/container/list
    title: list package, container/list, Go standard library
    locator: Overview; type List
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/collections.html
    title: collections, Container datatypes, Python documentation
    locator: deque objects
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/util/ArrayDeque.html
    title: ArrayDeque (Java SE 27 and JDK 27)
    locator: the class description
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/collections/index.html
    title: Module std::collections, The Rust Standard Library
    locator: When Should You Use Which Collection?
    accessed: 2026-11-16
---

## Idea

A deque is a queue that is open at both ends. Wikipedia describes it as "a generalization of both the stack and the queue".[^1] A program can add an item at the front or at the back, and remove an item from the front or from the back. Used at one end only, a deque is a stack. Added at one end and removed at the other, it is a queue.

The name is short for "double ended queue", and it "is usually pronounced" like the word deck.[^2]

```python run id=idea
from collections import deque


def is_palindrome(text: str) -> bool:
    letters = deque(ch.lower() for ch in text if ch.isalnum())
    while len(letters) > 1:
        if letters.popleft() != letters.pop():
            return False
    return True


for text in ["Never odd or even", "deque", "A man, a plan, a canal: Panama"]:
    print(text, "yes" if is_palindrome(text) else "no")
```

```output for=idea
Never odd or even yes
deque no
A man, a plan, a canal: Panama yes
```

## How it works

**Operations.** A deque has an add, a remove and a peek operation at each end. Java's `Deque` is "A linear collection that supports element insertion and removal at both ends."[^2] Its methods come in pairs, such as `addFirst` and `addLast`, `pollFirst` and `pollLast`.

**Implementations.** Wikipedia names "at least two common ways to efficiently implement a deque": "with a doubly linked list or with a modified dynamic array".[^1]

- A ring buffer keeps the items in one array with a front index. Both indexes wrap around, and the array grows when it is full. Java's `ArrayDeque` and Rust's `VecDeque` work this way. `VecDeque` is "A double-ended queue implemented with a growable ring buffer."[^3]
- A list of fixed-size blocks adds a new block when one end is full. C++ `std::deque` uses "a sequence of individually allocated fixed-size arrays".[^4] Python's deque is built in a similar way, as "a doubly linked list of fixed-length subarrays".[^1]
- A doubly linked list has a constant cost at both ends, but each item is a separate node. Go's `container/list` is one.[^5]

**Index access.** Some deques also give access by index. Python: "Indexed access is O(1) at both ends but slows to O(n) in the middle."[^6] `std::deque` "is an indexed sequence container that allows fast insertion and deletion at both its beginning and its end".[^4] Java's `ArrayDeque` has no index access, "contrary to its name".[^1]

## In languages

| Language | Verdict | Form |
|---|---|---|
| Python | stdlib | `collections.deque`, with an optional `maxlen`[^6] |
| JavaScript | idiom | an array with `push`, `pop`, `shift` and `unshift`[^1] |
| TypeScript | idiom | an array, as in JavaScript |
| Java | stdlib | the `Deque` interface, with `ArrayDeque` and `LinkedList`[^2][^7] |
| C | idiom | a ring buffer in an array, or a doubly linked list |
| C++ | stdlib | `std::deque`[^4] |
| C# | stdlib | `LinkedList<T>`; no array-based deque type |
| Go | stdlib | `container/list`, a doubly linked list[^5] |
| Rust | stdlib | `VecDeque`[^3] |
| Kotlin | stdlib | `ArrayDeque` |
| PowerShell | stdlib | `[System.Collections.Generic.LinkedList[T]]` from .NET |
| Bash | idiom | an indexed array |
| SQL | n/a | a table has no ends; an order column can emulate them |
| Regex | n/a | a pattern has no data structures |
| HTML/CSS | n/a | markup and style rules have no data structures |

**Python.** The documentation says that "Deques are a generalization of stacks and queues".[^6] They "support thread-safe, memory efficient appends and pops from either side of the deque with approximately the same O(1) performance in either direction".[^6] A deque with a `maxlen` drops items from the other end when it is full: "Bounded length deques provide functionality similar to the tail filter in Unix."[^6]

**Java.** The `Deque` documentation says: "Deques can also be used as LIFO (Last-In-First-Out) stacks. This interface should be used in preference to the legacy Stack class."[^2] `ArrayDeque` "is likely to be faster than Stack when used as a stack, and faster than LinkedList when used as a queue".[^7] It does not accept `null`: "Null elements are prohibited."[^7]

**C++.** "As opposed to std::vector, the elements of a deque are not stored contiguously".[^4] An insert or a removal at either end "never invalidates pointers or references to the rest of the elements".[^4]

**Rust.** The collections guide says to use a `VecDeque` when "You want a Vec that supports efficient insertion at both ends of the sequence."[^8] Because the items are in a ring buffer, they can be in two parts of the array. `as_slices` "Returns a pair of slices which contain, in order, the contents of the deque", and `make_contiguous` moves the items into one part.[^3]

**JavaScript and Go.** JavaScript arrays have `push`, `pop`, `shift` and `unshift`, so an array can act as a deque.[^1] `shift` and `unshift` move the other items, so they are slow for a large array. Go's `container/list` "implements a doubly linked list" with `PushFront`, `PushBack`, `Remove`, `Front` and `Back`.[^5]

The example files add items at both ends, remove items from both ends, and show one extra feature of each library type.

## Variations and related ideas

- **Bounded deque.** A deque with a maximum size. Python's `maxlen` drops the oldest item at the other end. See [[ring-buffer]].
- **Input-restricted and output-restricted deques.** One end only adds, or one end only removes.[^1]
- **Work stealing.** Each processor keeps a deque of tasks. It takes tasks from its own end, and another processor with no work takes tasks from the other end.[^1]
- **Monotonic queue.** A deque that keeps its items in increasing or decreasing order. It finds the minimum or maximum of a sliding window in linear time.[^1]
- **Stack and queue.** The two restricted forms of a deque. See [stack-data-structure](/stack-data-structure/) and [queue-concept](/queue-concept/).

## Pitfalls

### Java's ArrayDeque rejects null

`ArrayDeque` prohibits `null` items, and an add of `null` throws `NullPointerException`.[^7] `LinkedList` accepts them. Code that moves from `LinkedList` to `ArrayDeque` can fail at run time.

```java wrong id=java-null
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.LinkedList;

public class Main {
    public static void main(String[] args) {
        Deque<String> linked = new LinkedList<>();
        linked.addLast(null);
        System.out.println("LinkedList size " + linked.size());

        Deque<String> array = new ArrayDeque<>();
        try {
            array.addLast(null);
        } catch (NullPointerException e) {
            System.out.println("ArrayDeque: NullPointerException");
        }
    }
}
```

```output for=java-null
LinkedList size 1
ArrayDeque: NullPointerException
```

Use a marker object or an `Optional` in place of `null`.

### Index access in the middle of a deque is slow

A Python deque reads items near its ends in constant time, but `d[i]` in the middle walks the blocks: "Indexed access is O(1) at both ends but slows to O(n) in the middle. For fast random access, use lists instead."[^6] A loop such as `for i in range(len(d)): d[i]` is then quadratic. Iterate over the deque directly, or copy it to a list first.

### A bounded deque drops items without an error

A Python deque with `maxlen` never raises an error when it is full. It discards an item from the other end.[^6]

```python wrong id=python-maxlen
from collections import deque

recent = deque(maxlen=3)
for n in range(1, 6):
    recent.append(n)
print(list(recent), "length", len(recent))

recent.appendleft(0)
print(list(recent))
```

```output for=python-maxlen
[3, 4, 5] length 3
[0, 3, 4]
```

This is right for a log of recent events. It is wrong for a work queue, where each item must be done.

## Cost

| Operation | Ring buffer (`ArrayDeque`, `VecDeque`) | Blocks (`std::deque`, Python `deque`) | Doubly linked list |
|---|---|---|---|
| add or remove at either end | amortized constant | constant | constant, with one allocation |
| peek at either end | constant | constant | constant |
| item at index i | constant, or none in Java | constant in C++; linear in the middle in Python | linear |
| insert in the middle | linear | linear | constant at a known node |

`VecDeque::rotate_left(n)` takes time in proportion to the smaller of `n` and `len() - n`, "and no extra space".[^3]

## History

Wikipedia reports that a CTSS technical note of 1965 had subroutines that placed a user at the end or at the beginning of a queue, "before even the deque is named".[^1] Donald Knuth treated stacks, queues and deques together in The Art of Computer Programming, volume 1, section 2.2.1, "Stacks, Queues, and Deques".[^1] "As of Java 6, Java's Collections Framework provides a new Deque interface", and "Python 2.4 introduced the collections module with support for deque objects".[^1]

## References
