---
tag: CRWTGM
kind: concept
slug: ring-buffer
name: Ring buffer
aliases: [ring buffer, circular buffer, circular queue, cyclic buffer, bounded buffer, deque maxlen]
area: data-and-values/stacks-queues-and-trees
level: core
summary: >-
  A ring buffer is a fixed-size array used as a queue. The read and write positions wrap around to the start, so no item moves, and a full buffer refuses new items or overwrites the oldest.
applies_to: core-languages
viz: optional
formal: none
see_also: [queue-concept, deque, array, dynamic-array, producer-consumer, channels, stack-data-structure, structured-logging]
same_as:
  wikidata: Q1224994
  wikipedia: Circular_buffer
sources:
  - url: https://en.wikipedia.org/wiki/Circular_buffer
    title: Circular buffer, Wikipedia
    locator: the introduction; Overview; Uses; Circular buffer mechanics
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/collections.html
    title: collections, Container datatypes, Python documentation
    locator: deque objects
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/util/ArrayDeque.html
    title: ArrayDeque (Java SE 27 and JDK 27)
    locator: the class description
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.queue-1?view=net-11.0
    title: Queue<T> Class, .NET API reference
    locator: Remarks
    accessed: 2026-11-16
  - url: https://pkg.go.dev/container/ring
    title: ring package, container/ring, Go standard library
    locator: Overview; type Ring; func (*Ring) Len
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/collections/struct.VecDeque.html
    title: Struct VecDeque, The Rust Standard Library
    locator: the description
    accessed: 2026-11-16
---

## Idea

A ring buffer is a fixed array that acts as if its last slot were joined to its first. Wikipedia: "a circular buffer, circular queue, cyclic buffer or ring buffer is a data structure that uses a single, fixed-size buffer as if it were connected end-to-end."[^1] A program writes at one position and reads at another. When a position passes the last slot, it goes back to slot 0.

This class keeps the newest three readings. When the buffer is full, a new reading overwrites the oldest one.

```python run id=idea
class RingBuffer:
    def __init__(self, capacity):
        self.items = [None] * capacity
        self.start = 0
        self.count = 0

    def push(self, item):
        end = (self.start + self.count) % len(self.items)
        self.items[end] = item
        if self.count < len(self.items):
            self.count += 1
        else:
            self.start = (self.start + 1) % len(self.items)

    def __iter__(self):
        for i in range(self.count):
            yield self.items[(self.start + i) % len(self.items)]


ring = RingBuffer(3)
for reading in [10, 20, 30, 40, 50]:
    ring.push(reading)
    print("push", reading, "->", list(ring), "slots", ring.items, "start", ring.start)
```

```output for=idea
push 10 -> [10] slots [10, None, None] start 0
push 20 -> [10, 20] slots [10, 20, None] start 0
push 30 -> [10, 20, 30] slots [10, 20, 30] start 0
push 40 -> [20, 30, 40] slots [40, 20, 30] start 1
push 50 -> [30, 40, 50] slots [40, 50, 30] start 2
```

The slots show that no item moves. Only the start index changes.

## How it works

**The parts.** A ring buffer has an array of fixed capacity, a read index (the start) and either a write index (the end) or a count.[^1]

- A write puts the item at the end index and moves the end index to the next slot.
- A read takes the item at the start index and moves the start index to the next slot.
- "The next slot" is `(index + 1) % capacity`, so the index goes from the last slot back to slot 0.

Wikipedia: "When elements get removed, only the read-pointer moves to the next element. Removed elements are still in the buffer, waiting to get overwritten by new elements."[^1]

**Full and empty.** If the buffer keeps only the two indexes, a full buffer and an empty buffer both have start equal to end. Wikipedia: "The start and end indexes alone are not enough to distinguish between buffer full or empty state while also utilizing all buffer slots".[^1] There are two common fixes:

- Keep one slot always empty. The buffer is then full when it holds capacity minus one items.[^1]
- Keep a count. "Then checking for emptiness means testing count equals 0 and checking for fullness means testing count equals Length."[^1]

**When the buffer is full.** A write to a full buffer must do one of two things. It can overwrite the oldest item: "when it is full and a subsequent write is performed, then it starts overwriting the oldest data."[^1] Or it can refuse: "the routines that manage the buffer could prevent overwriting the data and return an error or raise an exception."[^1] A log of recent events usually overwrites. A queue of orders must refuse, or it loses orders.

**Why it is used.** "The useful property of a circular buffer is that it does not need to have its elements shuffled around when one is consumed."[^1] For a queue with a maximum size, "all queue operations are constant time".[^1] The memory is allocated once, so a ring buffer suits streams of data, audio, keyboard input and other places that cannot allocate memory at each item.

## In languages

| Language | Verdict | Form |
|---|---|---|
| Python | stdlib | `collections.deque(maxlen=n)` overwrites the oldest item[^2] |
| JavaScript | idiom | an array, a start index and a count |
| TypeScript | idiom | as JavaScript |
| Java | idiom | an array and two indexes; `ArrayDeque` is a ring that grows[^3] |
| C | idiom | an array, a start index and a count |
| C++ | idiom | a `std::array` and two indexes |
| C# | idiom | an array and two indexes; `Queue<T>` is a circular array that grows[^4] |
| Go | idiom | a slice and two indexes; `container/ring` is a circular list[^5] |
| Rust | stdlib | `VecDeque`, a growable ring buffer, with a length check[^6] |
| Kotlin | idiom | an array and two indexes, or `ArrayDeque` with a size check |
| PowerShell | idiom | a .NET array and two indexes |
| Bash | idiom | an indexed array and an index with `%` |
| SQL | idiom | a table that deletes its oldest rows after each insert |
| Regex | n/a | a pattern has no data structures |
| HTML/CSS | n/a | markup and style rules have no data structures |

**Python.** A deque with `maxlen` is a ring buffer that overwrites. "Once a bounded length deque is full, when new items are added, a corresponding number of items are discarded from the opposite end."[^2] The documentation adds: "Bounded length deques provide functionality similar to the tail filter in Unix."[^2]

**Rust.** `VecDeque` is "A double-ended queue implemented with a growable ring buffer."[^6] It grows when it is full. For a fixed size, the program calls `pop_front` before `push_back` when the length reaches the limit.

**C#.** The `Queue<T>` class "implements a generic queue as a circular array."[^4] It also grows when it is full.

**Java.** `ArrayDeque` is a "Resizable-array implementation of the Deque interface." "Array deques have no capacity restrictions; they grow as necessary to support usage."[^3] A fixed ring with overwrite is a small class over an array.

**C, C++, Go and JavaScript.** These programs write the ring by hand. The example files show the same buffer of capacity 3 in each language.

## Variations and related ideas

- **Growable ring buffer.** When the buffer is full, it allocates a bigger array and copies the items in order. `VecDeque` and .NET `Queue<T>` work like this.[^6][^4] Wikipedia notes that "expanding a circular buffer requires shifting memory, which is comparatively costly."[^1]
- **Bounded buffer between threads.** A producer writes and a consumer reads, with locks or atomic operations around the indexes. Wikipedia mentions the ring buffer as "the bounded buffer in the producer–consumer problem".[^1] See [producer-consumer](/producer-consumer/). Concurrent ring buffers belong to concurrency.
- **Go channels.** A buffered channel is a bounded first in, first out queue between goroutines. See [channels](/channels/).
- **Circular linked list.** Go's `container/ring` is a list whose last node links to the first. "Rings do not have a beginning or end; a pointer to any ring element serves as reference to the entire ring."[^5] It is not an array, and its `Len` "executes in time proportional to the number of elements."[^5]
- **Items that wrap.** The items of a ring buffer can sit in two parts of the array. Rust says that "its elements are not necessarily contiguous in memory".[^6] See [deque](/deque/) for `as_slices` and `make_contiguous`.

## Pitfalls

### Full and empty look the same

With only a start index and an end index, a buffer that holds capacity items has start equal to end, as an empty buffer has.

```python wrong id=python-full-empty
class Ring:
    def __init__(self, capacity):
        self.items = [None] * capacity
        self.start = 0
        self.end = 0

    def is_empty(self):
        return self.start == self.end

    def push(self, item):
        self.items[self.end] = item
        self.end = (self.end + 1) % len(self.items)


ring = Ring(3)
print("empty at start:", "yes" if ring.is_empty() else "no")
for item in "abc":
    ring.push(item)
print("empty after 3 pushes:", "yes" if ring.is_empty() else "no")
```

```output for=python-full-empty
empty at start: yes
empty after 3 pushes: yes
```

Keep a count, as in the Idea example, or treat the buffer as full when it holds capacity minus one items.[^1]

### A bounded deque drops items without an error

`deque(maxlen=n).append` never fails. When the deque is full, the oldest item goes away.[^2] That is right for a window of recent values, and wrong for work that must not be lost.

```python wrong id=python-silent-drop
from collections import deque

orders = deque(maxlen=3)
for order in ["A1", "A2", "A3", "A4", "A5"]:
    orders.append(order)
print("kept:", list(orders))
print("lost: A1 and A2, with no error")
```

```output for=python-silent-drop
kept: ['A3', 'A4', 'A5']
lost: A1 and A2, with no error
```

For work that must not be lost, check the length first and refuse or wait when the buffer is full.

### The remainder of a negative number

To step an index back by one, a program may write `(index - 1) % capacity`. In Java, when the index is 0, the result is -1, which is not a valid index. Other languages with the same `%` rule fail in the same way.

```java wrong id=java-negative-index
public class Main {
    public static void main(String[] args) {
        int capacity = 4;
        int index = 0;
        System.out.println("(index - 1) % capacity = " + (index - 1) % capacity);
        System.out.println("Math.floorMod(index - 1, capacity) = " + Math.floorMod(index - 1, capacity));
        System.out.println("(index - 1 + capacity) % capacity = " + (index - 1 + capacity) % capacity);
    }
}
```

```output for=java-negative-index
(index - 1) % capacity = -1
Math.floorMod(index - 1, capacity) = 3
(index - 1 + capacity) % capacity = 3
```

Add the capacity before the remainder, or use a floor modulo function. For Python, see [integer division and modulo](/python/integer-division-and-modulo/).

### A growable ring buffer is not bounded

`VecDeque`, `ArrayDeque` and .NET `Queue<T>` use a ring inside, but they grow without limit.[^6][^4][^3] A program that needs a fixed memory size must check the length itself.

## Cost

| Operation | Ring buffer | Array with shift at the front |
|---|---|---|
| write at the end | constant | constant (amortized) |
| read from the front | constant | linear |
| memory | fixed at creation | grows |
| grow | linear copy | linear copy |

Wikipedia: for a queue with a maximum size, "all queue operations are constant time".[^1] Without the ring, "it would be necessary to shift all elements when one is consumed."[^1]

## History

Wikipedia states that "There were early circular buffer implementations in hardware."[^1] The read-only `maxlen` attribute of a Python deque was "Added in version 3.1."[^2]

## References
