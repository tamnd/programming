---
tag: YSDZN3
kind: concept
slug: priority-queue
name: Priority queue
aliases: [priority queue, heap queue, min-heap, max-heap, PriorityQueue, heapq, BinaryHeap, std::priority_queue, container/heap]
area: data-and-values/stacks-queues-and-trees
level: core
summary: >-
  A priority queue gives back the item with the best priority first, not the oldest item. Libraries build it on a binary heap, so a push or a pop takes logarithmic time.
applies_to: core-languages
viz: optional
formal: none
see_also: [queue-concept, stack-data-structure, comparator, sort-a-list, priority-queue-pattern, dynamic-array, worker-queue, sort-with-a-custom-key]
same_as:
  wikidata: Q629283
  wikipedia: Priority_queue
sources:
  - url: https://en.wikipedia.org/wiki/Priority_queue
    title: Priority queue, Wikipedia
    locator: the introduction; Operations; Implementation; Libraries; Applications
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/heapq.html
    title: heapq, Heap queue algorithm, Python documentation
    locator: the introduction; max-heap functions; Priority Queue Implementation Notes
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/util/PriorityQueue.html
    title: PriorityQueue (Java SE 27 and JDK 27)
    locator: the class description; Implementation note
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/container/priority_queue
    title: std::priority_queue, cppreference.com
    locator: the introduction
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.priorityqueue-2?view=net-11.0
    title: PriorityQueue<TElement,TPriority> Class, .NET API reference
    locator: the summary; Remarks
    accessed: 2026-11-16
  - url: https://pkg.go.dev/container/heap
    title: heap package, container/heap, Go standard library
    locator: Overview; Example (PriorityQueue)
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/collections/struct.BinaryHeap.html
    title: Struct BinaryHeap, The Rust Standard Library
    locator: the description; Min-heap; Time complexity
    accessed: 2026-11-16
---

## Idea

A plain queue serves items in the order they arrive. A priority queue serves the most important item first. Wikipedia: "a priority queue is an abstract data type similar to a regular queue where each element has an associated priority determining its order of service."[^1] A hospital that treats the worst injury first, a scheduler that runs the most urgent task first, and a search that expands the cheapest path first all use one.

```python run id=idea
import heapq

tasks = []
heapq.heappush(tasks, (3, "update docs"))
heapq.heappush(tasks, (1, "fix outage"))
heapq.heappush(tasks, (2, "review patch"))
heapq.heappush(tasks, (1, "call customer"))

while tasks:
    priority, name = heapq.heappop(tasks)
    print(priority, name)
```

```output for=idea
1 call customer
1 fix outage
2 review patch
3 update docs
```

The two tasks with priority 1 come out in the order of their names, because Python compares the tuples item by item. The Pitfalls section shows why that can fail.

## How it works

**Operations.** A priority queue has three main operations: insert an item with a priority, remove the item with the best priority, and peek at that item.[^1] In a min-priority queue the best item has the smallest value, and in a max-priority queue it has the largest. Wikipedia notes that "higher priority can be given either to the lesser or to the greater values".[^1]

**The binary heap.** Wikipedia: "To improve performance, priority queues are typically based on a heap". A heap gives logarithmic time for an insert and a removal, and linear time to build the heap from n items.[^1] A binary heap is a complete binary tree stored in an array. In a min-heap, each parent is less than or equal to its children, so the smallest item is at index 0. The Python documentation states the rule as `heap[k] <= heap[2*k+1]` and `heap[k] <= heap[2*k+2]`.[^2]

- An insert puts the new item at the end of the array and moves it up while it is smaller than its parent.
- A removal takes the item at index 0, moves the last item to index 0, and moves that item down while a child is smaller.

How heaps work in detail is on the algorithm site: [binary heap](https://algorithm.tamnd.com/binary-heap/).

**A priority queue is not a sorted list.** Only the first item is in its final place. Wikipedia: "While priority queues are often implemented using heaps, they are conceptually distinct."[^1] The rest of the array is in heap order, which is not sorted order.

## In languages

| Language | Verdict | Form |
|---|---|---|
| Python | stdlib | `heapq` functions on a plain list, smallest first[^2] |
| JavaScript | idiom | no built-in type; a hand-written binary heap |
| TypeScript | idiom | as JavaScript |
| Java | stdlib | `PriorityQueue`, smallest first[^3] |
| C | idiom | a hand-written heap in an array |
| C++ | stdlib | `std::priority_queue`, largest first[^4] |
| C# | stdlib | `PriorityQueue<TElement, TPriority>`, smallest priority first[^5] |
| Go | stdlib | `container/heap` over a type that implements `heap.Interface`[^6] |
| Rust | stdlib | `BinaryHeap`, largest first[^7] |
| Kotlin | stdlib | `java.util.PriorityQueue` on the JVM |
| PowerShell | stdlib | `[System.Collections.Generic.PriorityQueue[TElement,TPriority]]` from .NET |
| Bash | n/a | no priority queue; `sort -n` sorts lines once |
| SQL | idiom | `ORDER BY priority` with `LIMIT 1` over an indexed table |
| Regex | n/a | a pattern has no data structures |
| HTML/CSS | n/a | markup and style rules have no data structures |

**Python.** `heapq` works on a list. "The interesting property of a min-heap is that its smallest element is always the root".[^2] That is `heap[0]`. Python 3.14 added max-heap functions such as `heappush_max` and `heappop_max`.[^2] On older versions, a program pushes negated numbers or wraps the items.

**Java.** `PriorityQueue` is "An unbounded priority queue based on a priority heap."[^3] "The head of this queue is the least element with respect to the specified ordering."[^3] The items are ordered "by a Comparator provided at queue construction time" if the program gives one.[^3] A reversed comparator gives a max-heap.

**C++.** `std::priority_queue` "is a container adaptor that provides constant time lookup of the largest (by default) element, at the expense of logarithmic insertion and extraction".[^4] For a min-heap, "using std::greater<T> would cause the smallest element to appear as the top()".[^4]

**C#.** `PriorityQueue<TElement, TPriority>` "Represents a collection of items that have a value and a priority. On dequeue, the item with the lowest priority value is removed."[^5] It "Implements an array-backed, quaternary min-heap."[^5] The element and the priority are separate, so the element type does not need an order.

**Rust.** `BinaryHeap` is "A priority queue implemented with a binary heap. This will be a max-heap."[^7] "Either core::cmp::Reverse or a custom Ord implementation can be used to make BinaryHeap a min-heap."[^7]

**Go.** "Package heap provides heap operations for any type that implements heap.Interface."[^6] The program writes `Len`, `Less`, `Swap`, `Push` and `Pop` for its own slice type. "A heap is a common way to implement a priority queue."[^6]

The example files push four tasks with priorities, pop them in priority order, and show how to get the other order.

## Variations and related ideas

- **Min and max.** The same structure with the opposite comparison. See [comparator](/comparator/).
- **Indexed priority queue.** A priority queue that can change the priority of an item that is already in it. Wikipedia notes that Dijkstra's algorithm needs "the ability to alter the priority of a particular vertex in the priority queue efficiently".[^1] Most standard libraries do not offer this. The heapq documentation suggests to "mark the entry as removed and add a new entry with the revised priority".[^2]
- **Bounded top-k.** A heap of size k keeps the k largest items of a stream: push each item, and pop the smallest when the heap has more than k items. Python has `heapq.nlargest` and `heapq.nsmallest` for this.[^2]
- **Concurrent priority queues.** Java has `PriorityBlockingQueue` for threads.[^3]
- **Priority queues between services.** See [priority-queue-pattern](/priority-queue-pattern/).

## Pitfalls

### Equal priorities are not served first in, first out

A binary heap does not keep the arrival order of items with equal priority. Wikipedia: "In other implementations, the order of elements with the same priority is undefined."[^1] Java says that "ties are broken arbitrarily".[^3] The .NET type "does not guarantee first-in-first-out semantics for elements of equal priority".[^5]

In Python, a tie also compares the second item of the tuple. If that item cannot be compared, the push fails.

```python wrong id=python-tie
import heapq

tasks = []
heapq.heappush(tasks, (1, {"name": "fix outage"}))
try:
    heapq.heappush(tasks, (1, {"name": "call customer"}))
except TypeError as error:
    print("TypeError:", error)
```

```output for=python-tie
TypeError: '<' not supported between instances of 'dict' and 'dict'
```

The fix is a counter as the second item. The heapq documentation stores "the priority, an entry count, and the task". "The entry count serves as a tie-breaker so that two tasks with the same priority are returned in the order they were added."[^2]

```python run id=python-counter
import heapq
from itertools import count

tasks = []
order = count()
for priority, name in [(1, "fix outage"), (2, "review patch"), (1, "call customer")]:
    heapq.heappush(tasks, (priority, next(order), {"name": name}))

while tasks:
    priority, _, task = heapq.heappop(tasks)
    print(priority, task["name"])
```

```output for=python-counter
1 fix outage
1 call customer
2 review patch
```

### Printing or iterating a priority queue does not give sorted order

The array of a heap is in heap order. Java warns that the iterator is "not guaranteed to traverse the elements of the priority queue in any particular order".[^3] Only repeated `poll` calls give sorted order.

```java wrong id=java-iteration
import java.util.List;
import java.util.PriorityQueue;

public class Main {
    public static void main(String[] args) {
        PriorityQueue<Integer> queue = new PriorityQueue<>();
        for (int n : List.of(5, 1, 4, 2, 3)) {
            queue.add(n);
        }
        System.out.println("printed " + queue);

        StringBuilder polled = new StringBuilder();
        while (!queue.isEmpty()) {
            polled.append(polled.isEmpty() ? "" : " ").append(queue.poll());
        }
        System.out.println("polled " + polled);
    }
}
```

```output for=java-iteration
printed [1, 2, 4, 5, 3]
polled 1 2 3 4 5
```

### The default order differs between languages

`std::priority_queue` and Rust's `BinaryHeap` give the largest item first.[^4][^7] Python, Java and C# give the smallest first.[^2][^3][^5] Code that is ported between these languages must change the comparison, or it serves the items in reverse.

### A changed item breaks the heap

The heap places an item when it is pushed. If the program later changes a field that the comparison uses, the item stays where it is, and the heap order is wrong. Rust says that "It is a logic error for an item to be modified" in this way.[^7] To change a priority, remove the item and push it again, or push a new entry and skip the old one when it comes out.

## Cost

| Operation | Binary heap | Sorted array | Unsorted array |
|---|---|---|---|
| insert | logarithmic | linear | constant |
| remove the best | logarithmic | constant | linear |
| peek at the best | constant | constant | linear |
| build from n items | linear | | |

The heap figures come from Wikipedia and the library documents. Wikipedia gives the figures for the two naive arrays.[^1]

Java's implementation note gives the same figures: "O(log(n)) time for the enqueuing and dequeuing methods", "linear time for the remove(Object) and contains(Object) methods", and "constant time for the retrieval methods".[^3] Rust gives the cost of `push` as O(1) and calls it "an expected cost".[^7]

## History

The C++ standard of 1998 "specifies std::priority_queue as one of the STL container adaptor class templates".[^1] Java added `PriorityQueue` in version 1.5.[^3] Python 3.14 added max-heap functions to `heapq`.[^2]

## References
