---
tag: 5Q5PMY
kind: concept
slug: queue-concept
name: Queue
aliases: [queue, FIFO, first in first out, enqueue, dequeue, FIFO queue, Queue<T>, std::queue, blocking queue]
area: data-and-values/stacks-queues-and-trees
level: intro
summary: >-
  A queue is a collection that gives back its items in the order they arrived: the first item in is the first item out. Programs use queues for work that waits its turn.
applies_to: core-languages
viz: optional
formal: none
see_also: [stack-data-structure, linked-list, dynamic-array, channels, worker-queue, threads, list, iterators]
same_as:
  wikidata: Q220543
  wikipedia: Queue_(abstract_data_type)
sources:
  - url: https://en.wikipedia.org/wiki/Queue_(abstract_data_type)
    title: Queue (abstract data type), Wikipedia
    locator: the introduction; Queue implementation; Purely functional implementation
    accessed: 2026-11-16
  - url: https://docs.python.org/3/tutorial/datastructures.html
    title: The Python Tutorial, 5. Data Structures
    locator: 5.1.2 Using Lists as Queues
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/collections.html
    title: collections, Container datatypes, Python documentation
    locator: deque objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/queue.html
    title: queue, A synchronized queue class, Python documentation
    locator: the introduction; class queue.Queue
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/util/Queue.html
    title: Queue (Java SE 27 and JDK 27)
    locator: the interface description; Summary of Queue methods
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/util/ArrayDeque.html
    title: ArrayDeque (Java SE 27 and JDK 27)
    locator: the class description
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/container/queue
    title: std::queue, cppreference.com
    locator: the introduction
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.queue-1?view=net-11.0
    title: Queue<T> Class, .NET API reference
    locator: the summary; Remarks
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Channel types
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/collections/struct.VecDeque.html
    title: Struct VecDeque, The Rust Standard Library
    locator: the description; pop_front
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/shift
    title: Array.prototype.shift(), MDN Web Docs
    locator: Description
    accessed: 2026-11-16
---

## Idea

A queue is a line of items that wait their turn. Wikipedia: "The name queue is an analogy to the words used to describe people in line to wait for goods or services."[^1] New items join at the back, and the program takes items from the front. "The operations of a queue make it a first-in-first-out (FIFO) data structure as the first element added to the queue is the first one removed."[^1]

```python run id=idea
from collections import deque

jobs = deque()
for name in ["print report", "send mail", "resize image"]:
    jobs.append(name)
    print("enqueue", name, "waiting", len(jobs))

while jobs:
    print("run", jobs.popleft(), "waiting", len(jobs))
```

```output for=idea
enqueue print report waiting 1
enqueue send mail waiting 2
enqueue resize image waiting 3
run print report waiting 2
run send mail waiting 1
run resize image waiting 0
```

## How it works

**Operations.** A queue has two main operations.[^1]

- enqueue "adds one element to the rear of the queue";
- dequeue "removes one element from the front of the queue".

Many queues also have a peek or front operation, which returns the next item and does not remove it.[^1] A dequeue on an empty queue is an underflow. A queue with a fixed size is a bounded queue, and an enqueue on a full bounded queue fails or waits.[^1]

**Implementations.** A queue changes at both ends, so a plain array is a poor fit: a removal at the front moves every other item. Wikipedia names the usual choices: "A queue may be implemented as circular buffers and linked lists".[^1]

- A ring buffer keeps the items in an array, with an index for the front and a count. Both indexes wrap around at the end of the array. See [[ring-buffer]].
- A linked list adds at the tail and removes at the head. See [linked-list](/linked-list/).
- Two stacks also make a queue. One stack takes new items. When the other stack is empty, the program moves all items across, which reverses their order. Wikipedia describes this purely functional queue: "the amortized time is O(1), but individual operations can take O(n)".[^1]

**Where programs use queues.** A queue holds work that arrives faster than the program can do it: print jobs, network messages, events in a user interface, and the nodes to visit in a breadth-first walk of a graph. A queue that threads share is a channel between producers and consumers. See [channels](/channels/) and [worker-queue](/worker-queue/).

## In languages

| Language | Verdict | Form |
|---|---|---|
| Python | stdlib | `collections.deque`, with `append` and `popleft`; `queue.Queue` between threads[^2][^3][^4] |
| JavaScript | idiom | an array with `push` and `shift`, or a class around a ring buffer |
| TypeScript | idiom | an array, as in JavaScript |
| Java | stdlib | the `Queue` interface, with `ArrayDeque` or `LinkedList`[^5][^6] |
| C | idiom | a ring buffer in an array, or a linked list |
| C++ | stdlib | `std::queue`, an adaptor over `std::deque` by default[^7] |
| C# | stdlib | `Queue<T>`, a circular array[^8] |
| Go | idiom | a slice, or `container/list`; a buffered channel between goroutines[^9] |
| Rust | stdlib | `VecDeque`, with `push_back` and `pop_front`[^10] |
| Kotlin | stdlib | `ArrayDeque` |
| PowerShell | stdlib | `[System.Collections.Generic.Queue[T]]` from .NET |
| Bash | idiom | an indexed array and a front index |
| SQL | idiom | a table of jobs with an order column |
| Regex | n/a | a pattern has no data structures |
| HTML/CSS | n/a | markup and style rules have no data structures |

**Python.** A list can work as a queue, but the tutorial warns that "lists are not efficient for this purpose".[^2] "While appends and pops from the end of list are fast, doing inserts or pops from the beginning of a list is slow (because all of the other elements have to be shifted by one)."[^2] It says: "To implement a queue, use collections.deque which was designed to have fast appends and pops from both ends."[^2] Deques "support thread-safe, memory efficient appends and pops from either side of the deque with approximately the same O(1) performance in either direction".[^3] For threads, the `queue` module "implements multi-producer, multi-consumer queues".[^4] A `queue.Queue` with a `maxsize` is bounded: "Insertion will block once this size has been reached, until queue items are consumed."[^4]

**Java.** The `Queue` interface has two forms of each operation: "one throws an exception if the operation fails, the other returns a special value (either null or false , depending on the operation)".[^5] So `add`, `remove` and `element` throw, and `offer`, `poll` and `peek` return a value.[^5] `ArrayDeque` is "faster than LinkedList when used as a queue".[^6] Methods that wait "for elements to appear or for space to become available" are in the `BlockingQueue` interface, which extends `Queue`.[^5]

**C++.** "The std::queue class template is a container adaptor that gives the functionality of a queue - specifically, a FIFO (first-in, first-out) data structure."[^7] It "pushes the elements on the back of the underlying container and pops them from the front".[^7]

**C#.** `Queue<T>` "Represents a first-in, first-out collection of objects."[^8] The remarks say: "This class implements a generic queue as a circular array."[^8]

**Rust.** `VecDeque` is "A double-ended queue implemented with a growable ring buffer."[^10] The usual way to use it as a queue is to "use push_back to add to the queue, and pop_front to remove from the queue".[^10] `pop_front` returns `None` when the deque is empty.[^10]

**JavaScript.** JavaScript has no queue type. `shift` removes the first item of an array: it "shifts all values to the left by 1 and decrements the length by 1".[^11] That costs time in proportion to the length, so a large queue needs a ring buffer or a head index.

**Go.** Go has no queue type. A slice works for a small queue. Between goroutines, a buffered channel is a queue: "Channels act as first-in-first-out queues."[^9]

The example files enqueue three items, dequeue one, peek at the next one and show what happens on an empty queue.

## Variations and related ideas

- **Bounded queue.** A queue with a fixed capacity. When it is full, an enqueue fails, waits, or drops an item. See [[ring-buffer]].
- **Blocking queue.** A queue for threads. A dequeue on an empty queue waits for an item, and an enqueue on a full queue waits for space.[^4] The details are in the concurrency area.
- **Deque.** A queue that works at both ends. See [[deque]].
- **Priority queue.** A queue that gives back the item with the best priority first, not the oldest item. See [[priority-queue]].
- **Stack.** The opposite order: last in, first out. See [stack-data-structure](/stack-data-structure/).
- **Message queue.** A queue between programs, often on another machine, that stores messages until a consumer reads them.

## Pitfalls

### A list as a queue is slow

`list.pop(0)` in Python and `shift()` in JavaScript move every other item one place to the left.[^2][^11] A loop that empties a queue of n items this way does work in proportion to n squared. The example times both ways on the same 50,000 items.

```python wrong id=python-list-queue
import time
from collections import deque

n = 50_000

items = list(range(n))
start = time.perf_counter()
while items:
    items.pop(0)
list_time = time.perf_counter() - start

items = deque(range(n))
start = time.perf_counter()
while items:
    items.popleft()
deque_time = time.perf_counter() - start

print("list slower than deque:", "yes" if list_time > deque_time else "no")
```

```output for=python-list-queue
list slower than deque: yes
```

Use `collections.deque` in Python. In JavaScript, keep a head index and read `items[head++]`, or use a ring buffer.

### A null item looks like an empty Java queue

`poll` and `peek` return `null` when the queue is empty. `LinkedList` accepts `null` items, so a program cannot tell a `null` item from an empty queue. The `Queue` documentation says that "null should not be inserted into a Queue , as null is also used as a special return value by the poll method to indicate that the queue contains no elements".[^5]

```java wrong id=java-null-item
import java.util.LinkedList;
import java.util.Queue;

public class Main {
    public static void main(String[] args) {
        Queue<String> queue = new LinkedList<>();
        queue.add(null);
        queue.add("b");
        String first = queue.poll();
        System.out.println(first == null ? "queue looks empty" : first);
        System.out.println("size " + queue.size());
    }
}
```

```output for=java-null-item
queue looks empty
size 1
```

`ArrayDeque` rejects `null` items, so it avoids the problem.[^6]

## Cost

| Operation | Ring buffer | Linked list | Array with shift |
|---|---|---|---|
| enqueue | amortized constant | constant, with one allocation | amortized constant |
| dequeue | constant | constant | linear |
| peek | constant | constant | constant |
| memory per item | the item, plus spare capacity | the item and one or two links | the item, plus spare capacity |

Library queues in Python, Java, C#, C++ and Rust do enqueue and dequeue in constant or amortized constant time. Wikipedia calls a queue efficient when its implementation "can perform the operations" of enqueue and dequeue in O(1) time.[^1]

## History

The word comes from everyday life: a queue of people waits for a service, and the first person in the line is served first.[^1] Today most standard libraries have a queue type or a deque that serves as one.[^3][^5][^7][^8][^10]

## References
