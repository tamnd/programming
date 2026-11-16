---
tag: F48CB6
kind: feature
owner: python
slug: lists-as-queues
name: Using lists as queues
aliases: [queue, FIFO, first-in first-out, collections.deque, deque, popleft, double-ended queue, list pop(0) slow]
area: languages/python/collections
level: core
summary: >-
  A list can work as a queue, but a pop or insert at the front moves every other item. collections.deque has fast appends and pops at both ends, so use it for a first-in, first-out queue.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/collections.html#collections.deque", locator: "deque objects"}
see_also: [python/lists-as-stacks, python/lists, python/the-collections-module]
sources:
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#using-lists-as-queues
    title: The Python Tutorial, 5. Data Structures
    locator: 5.1.2. Using Lists as Queues
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/collections.html#collections.deque
    title: collections, Python 3.15 documentation
    locator: deque objects
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Channel_types
    title: The Go Programming Language Specification
    locator: Channel types
    accessed: 2026-11-16
---

## Synopsis

```python run id=deque-queue
from collections import deque

queue = deque(["Eric", "John", "Michael"])
queue.append("Terry")
queue.append("Graham")
print(queue.popleft(), queue.popleft())
print(list(queue))
```

```output for=deque-queue
Eric John
['Michael', 'Terry', 'Graham']
```

Items go in at the right end and come out at the left end, in the order they arrived.

## Semantics

**r1.** "It is also possible to use a list as a queue, where the first element added is the first element retrieved (“first-in, first-out”); however, lists are not efficient for this purpose. While appends and pops from the end of list are fast, doing inserts or pops from the beginning of a list is slow (because all of the other elements have to be shifted by one)."[^1]

**r2.** "To implement a queue, use collections.deque which was designed to have fast appends and pops from both ends."[^1]

**r3.** "Deques support thread-safe, memory efficient appends and pops from either side of the deque with approximately the same O(1) performance in either direction."[^2]

**r4.** "Once a bounded length deque is full, when new items are added, a corresponding number of items are discarded from the opposite end."[^2] Example: [`bounded`](#bounded).

**r5.** extendleft() adds items at the left end. "Note, the series of left appends results in reversing the order of elements in the iterable argument."[^2] Example: [`extendleft`](#extendleft).

## Examples

```python run id=list-queue
queue = ["a", "b", "c"]
queue.append("d")
first = queue.pop(0)
print(first, queue)
```

```output for=list-queue
a ['b', 'c', 'd']
```

This works, but each `pop(0)` moves all the other items (rule r1).

```python run id=bounded
from collections import deque

last_three = deque(maxlen=3)
for reading in [10, 20, 30, 40, 50]:
    last_three.append(reading)
print(list(last_three), last_three.maxlen)
```

```output for=bounded
[30, 40, 50] 3
```

The deque keeps only the three newest items (rule r4).

```python run id=bfs
from collections import deque

graph = {"a": ["b", "c"], "b": ["d"], "c": ["d"], "d": []}
seen = {"a"}
order = []
todo = deque(["a"])
while todo:
    node = todo.popleft()
    order.append(node)
    for nxt in graph[node]:
        if nxt not in seen:
            seen.add(nxt)
            todo.append(nxt)
print(order)
```

```output for=bfs
['a', 'b', 'c', 'd']
```

A breadth-first search visits the nodes in the order that it finds them, so it uses a queue.

## Pitfalls

extendleft() reverses the order of the new items:

```python wrong id=extendleft
from collections import deque

d = deque([4, 5])
d.extendleft([1, 2, 3])
print(list(d))
```

```output for=extendleft
[3, 2, 1, 4, 5]
```

Use `d.extendleft(reversed([1, 2, 3]))` to keep the order (rule r5).

popleft() on an empty deque is an error:

```python fails id=empty expect=exception:IndexError
from collections import deque

deque().popleft()
```

## Comparison

In Go, a buffered channel is a queue: "Channels act as first-in-first-out queues. For example, if one goroutine sends values on a channel and a second goroutine receives them, the values are received in the order sent."[^3] A channel is made for goroutines that run at the same time. A Python deque is a container in one thread of code, with fast operations at both ends (rule r3).
