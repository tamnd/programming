---
tag: HF28E8
kind: task
slug: use-a-queue
name: Use a queue
aliases: [enqueue and dequeue, fifo queue, first in first out, round robin, list pop 0 is slow]
area: data-and-values/stacks-queues-and-trees
level: intro
summary: Run jobs in turn with round-robin scheduling, with enqueue and dequeue on the standard queue type of each tier 1 language, and see why list.pop(0) is slow.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
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
    locator: the introduction
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/shift
    title: Array.prototype.shift(), MDN Web Docs
    locator: Description
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/util/ArrayDeque.html
    title: ArrayDeque (Java SE 27 and JDK 27)
    locator: the class description
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/util/Queue.html
    title: Queue (Java SE 27 and JDK 27)
    locator: the interface description
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/container/queue
    title: std::queue, cppreference.com
    locator: the introduction
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.queue-1?view=net-11.0
    title: Queue<T> Class, .NET API reference
    locator: the summary; Methods
    accessed: 2026-11-16
  - url: https://pkg.go.dev/container/list
    title: Package list, Go standard library
    locator: Overview
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/collections/struct.VecDeque.html
    title: Struct VecDeque, The Rust Standard Library
    locator: the description; pop_front
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/container/deque/front
    title: std::deque::front, cppreference.com
    locator: the description
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.queue-1.dequeue?view=net-11.0
    title: Queue<T>.Dequeue Method, .NET API reference
    locator: Exceptions
    accessed: 2026-11-16
  - url: https://tiswww.case.edu/php/chet/bash/bashref.html
    title: Bash Reference Manual
    locator: 6.7 Arrays
    accessed: 2026-11-16
---

## Task

Use a queue to run jobs in turn. This is round-robin scheduling. The function `finish_order(work, slice)` gets a list of jobs: job `i` needs `work[i]` time units. At the start, the jobs wait in a queue in the order 0, 1, 2 and so on. The scheduler takes the job at the front, and runs it for `slice` time units or until it is done. If the job needs more time, it goes to the back of the queue. The function returns the job numbers in the order in which the jobs finish.

For example, `finish_order([3, 1, 2], 1)` is `[1, 2, 0]`. Job 1 finishes in the first round. Job 2 finishes in the second round, and job 0 finishes in the third round. Each job gets the same share of time, and that is the order of a queue: the first job in is the first job out. For the idea, see [[queue-concept]].

## Solutions

| Language | Queue | Enqueue and dequeue |
|---|---|---|
| Python | `collections.deque` | `append(x)`, `popleft()` |
| JavaScript | an array and a head index | `push(x)`, `queue[head++]` |
| TypeScript | `number[]` and a head index | `push(x)`, `queue[head++]` |
| Java | `ArrayDeque<Integer>` as a `Queue` | `offer(x)`, `poll()` |
| C | a ring buffer: an array, a head index and a count | `queue[(head + count) % n] = x`, `queue[head]` |
| C++ | `std::queue<std::size_t>` | `push(x)`, `front()` and then `pop()` |
| C# | `Queue<int>` | `Enqueue(x)`, `TryDequeue(out x)` |
| Go | a slice `[]int` | `append(q, x)`, `q[0]` and then `q = q[1:]` |
| Rust | `VecDeque<usize>` | `push_back(x)`, `pop_front()` that gives an `Option` |
| Bash | an indexed array and a head index | `q+=("$x")`, `${q[head]}` |

- **Python uses `collections.deque`, not a list.** A list can be a queue, but the tutorial says that "lists are not efficient for this purpose."[^1] A deque supports "memory efficient appends and pops from either side of the deque with approximately the same O(1) performance in either direction."[^2] The module `queue` is a different tool. "It is especially useful in threaded programming when information must be exchanged safely between multiple threads."[^3] One thread does not need its locks.
- **JavaScript keeps a head index.** JavaScript has no queue type. `shift()` removes the first element of an array, but its description says that it "shifts all values to the left by 1 and decrements the length by 1"[^4]. The solution does not depend on how an engine does this. It reads `queue[head]` and moves `head` forward, so a dequeue does not move the other elements.
- **Java uses the `Queue` methods of `ArrayDeque`.** `ArrayDeque` is "faster than LinkedList when used as a queue"[^5]. Use `offer` and `poll`. "The remove() and poll() methods remove and return the head of the queue."[^6] They differ only when the queue is empty: `poll()` returns null, and `remove()` throws an exception.
- **C++ and C# have a queue type.** `std::queue` gives "the functionality of a queue - specifically, a FIFO (first-in, first-out) data structure."[^7] In .NET, `Queue<T>` "Represents a first-in, first-out collection of objects."[^8] The C# solution uses `TryDequeue` in the loop condition, so the loop stops when the queue is empty.
- **C uses a ring buffer.** The queue never holds more than `n` jobs, so an array of `n` slots is sufficient. The index of the back is `(head + count) % n`. For the idea, see [[ring-buffer]].
- **Go uses a slice.** `q[0]` is the front, and `q = q[1:]` removes it. The package `container/list` is another choice: "Package list implements a doubly linked list."[^9]
- **Rust uses `VecDeque`.** `pop_front` "Removes the first element and returns it, or None if the deque is empty."[^10] So `while let Some(job) = queue.pop_front()` is the loop.
- **Bash keeps a head index.** The solution adds with `q+=("$x")` and reads `${q[head]}`. It does not remove items from the array, so the indices stay 0, 1, 2 and so on.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: no jobs give an empty list. The C solution returns at once when `n` is 0, before it gets memory.
- `one`: one job finishes after enough rounds, with no other job between them.
- `big-slice`: each job finishes in its first turn, so the order is the queue order, 0, 1, 2.
- `slice-one`: the jobs take turns one time unit at a time.
- `equal-work`: jobs with the same work finish in queue order.
- `long-job-first`: a long job at the front goes to the back again and again, and the short jobs after it finish first.

## Pitfalls

- **Python `list.pop(0)` is slow.** The tutorial says: "While appends and pops from the end of list are fast, doing inserts or pops from the beginning of a list is slow (because all of the other elements have to be shifted by one)."[^1] With 20,000 items, the time to empty a list with `pop(0)` is much more than the time to empty a deque with `popleft()`:

```python run id=python-pop-zero
import time
from collections import deque

n = 20_000
items = list(range(n))
start = time.perf_counter()
while items:
    items.pop(0)
list_time = time.perf_counter() - start

queue = deque(range(n))
start = time.perf_counter()
while queue:
    queue.popleft()
deque_time = time.perf_counter() - start

print("list.pop(0) more than 5 times slower:", "yes" if list_time > 5 * deque_time else "no")
```

```output for=python-pop-zero
list.pop(0) more than 5 times slower: yes
```

- **Java `poll()` cannot tell null from empty.** "Queue implementations generally do not allow insertion of null elements"[^6], but `LinkedList` allows them. The documentation continues: "null is also used as a special return value by the poll method to indicate that the queue contains no elements."[^6] With a null in a `LinkedList`, the two results of `poll()` look the same. `ArrayDeque` refuses null, so use it:

```java run id=java-poll-null
import java.util.LinkedList;
import java.util.Queue;

public class Main {
  public static void main(String[] args) {
    Queue<String> queue = new LinkedList<>();
    queue.offer(null);
    System.out.println("poll: " + queue.poll() + ", size: " + queue.size());
    System.out.println("poll: " + queue.poll() + ", size: " + queue.size());
  }
}
```

```output for=java-poll-null
poll: null, size: 0
poll: null, size: 0
```

- **C++ `front()` on an empty queue is not checked.** The default container of `std::queue` is `std::deque`. For `front()` on an empty container, "the behavior is undefined"[^11] (until C++26). `pop()` returns nothing, so read `front()` first, and test `empty()` before both.
- **C# `Dequeue` throws on an empty queue.** The `Dequeue` page gives `InvalidOperationException` for the case "The Queue<T> is empty."[^12] Use `TryDequeue` when the queue can be empty.
- **Bash `unset 'q[0]'` leaves a hole.** "unset name[subscript] unsets the array element at index subscript."[^13] The other elements keep their indices, so `${q[0]}` is then empty and the front is at index 1. Keep a head index, or read the indices with `${!q[@]}`:

```bash run id=bash-unset-front
q=(a b c)
unset 'q[0]'
echo "count: ${#q[@]}, q[0]: [${q[0]}]"
echo "indices: ${!q[@]}"
```

```output for=bash-unset-front
count: 2, q[0]: []
indices: 1 2
```

## Related tasks

- [[use-a-stack]]: the last item in is the first item out.
- [[use-a-priority-queue]]: the item with the best priority is the next item out.
- [[queue-concept]]: the idea behind this task and the operations.
- [[ring-buffer]]: a queue with a fixed size in an array.
- [[producer-consumer]]: a queue between threads.

## References
