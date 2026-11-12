---
tag: GQZT4H
kind: pattern
slug: copy-on-write
name: Copy-on-write
aliases: [copy-on-write, cow, implicit sharing, shadowing, clone-on-write]
area: software-design/concurrency-patterns
level: core
summary: >-
  Copy-on-write shares one copy of data among its users until one of them writes. The writer makes a private copy and changes that, so readers keep a stable snapshot and need no lock.
family: concurrency
polarity: positive
intent: Share data between users until one of them tries to modify it, and make a copy only then, so that each writer gets its own version and the readers are unaffected.
also_known_as: [Implicit sharing, Shadowing]
see_also: [immutable-object, thread-confinement, observer-pattern, flyweight-pattern, proxy-pattern, data-races, atomic-operations, lazy-initialization]
sources:
  - url: https://en.wikipedia.org/wiki/Copy-on-write
    title: Copy-on-write, Wikipedia
    locator: lead; In virtual memory management; In software; In computer storage
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/CopyOnWriteArrayList.html
    title: CopyOnWriteArrayList, Java SE 21 API documentation
    locator: class description
  - url: https://doc.rust-lang.org/std/borrow/enum.Cow.html
    title: Enum Cow, Rust standard library documentation
    locator: description
---

## Intent

Copy-on-write shares the same data between users until one tries to modify it.[^1] If no changes are made, no private copy is created. A copy is only made when needed, so that each user has its own version when it modifies the data.[^1]

## Problem

A list of listeners is read by many threads and changed only now and then. A thread that iterates over the list while another thread adds an item can fail or see a half-changed list. A lock around each iteration makes the readers wait for each other and for the writer, although reads are almost all of the work. A copy of the list for each reader wastes time and memory, because most readers never change it.

## Solution

Let all users share one copy. A writer does not change the shared copy: it makes a fresh copy, changes the copy, and then makes the copy the shared one. A reader takes a reference to the current copy and uses it with no lock. The copy it holds never changes.

Java's `CopyOnWriteArrayList` works this way. All mutative operations, such as `add` and `set`, are implemented by making a fresh copy of the underlying array.[^2] Its iterator uses a reference to the state of the array at the point that the iterator was created. That array never changes during the lifetime of the iterator, so interference is impossible.[^2]

```text
before:  readers A, B --> [l0]
add(l1): writer copies     [l0] -> [l0, l1], then publishes it
after:   reader A (old)  --> [l0]          still a valid snapshot
         reader C (new)  --> [l0, l1]
```

## Example

The example keeps a list of listeners as a tuple that never changes. 4 readers iterate over the current snapshot 1000 times each with no lock, while a writer adds 100 listeners. Each `add` copies the tuple under a lock that only writers take.

```text
iterations: 4000
iterations that failed: 0
listeners at the end: 101
length of the first snapshot: 1
```

The examples for each language show the same program.

## Consequences

Benefits:

- Readers need no lock, and a snapshot iterator is guaranteed not to throw `ConcurrentModificationException`.[^2]
- Data that is never changed is never copied, which saves resources.[^1]
- The approach is useful when the program cannot or does not want to synchronize traversals, yet needs to preclude interference among concurrent threads.[^2]

Liabilities:

- Each write copies the whole structure. The Java documentation calls this ordinarily too costly, and more efficient than alternatives only when traversal operations vastly outnumber mutations.[^2]
- A snapshot is out of date. The iterator does not reflect additions, removals or changes to the list since it was created.[^2]
- Writers still need a lock or an atomic compare-and-swap, so that two writers do not both copy the same old version and lose one change.

Without the copy, a reader that iterates over a shared dictionary fails when another thread adds an item:

```python wrong id=change-during-iteration
import threading

listeners = {"listener 0": True, "listener 1": True}  # shared and changed in place
first_seen = threading.Event()
added = threading.Event()


def writer():
    first_seen.wait()
    listeners["listener 2"] = True  # changes the dict during the iteration
    added.set()


t = threading.Thread(target=writer)
t.start()
try:
    for name in listeners:
        first_seen.set()
        added.wait()
except RuntimeError as e:
    print("reader:", e)
t.join()
```

```output for=change-during-iteration
reader: dictionary changed size during iteration
```

## When not to use it

Do not use copy-on-write for data that changes often or that is large and changes in small steps. Every change then pays for a full copy. A read-write lock or a concurrent collection fits such data better. Do not use it when every reader must see the latest value at once, because a reader can hold an old snapshot.

## Variants and related patterns

- **Clone-on-write in one thread.** Rust's `Cow` type can enclose borrowed data and provide immutable access to it, and clone the data lazily when mutation or ownership is required.[^3] `Rc::make_mut` and `Arc::make_mut` give clone-on-write for reference-counted pointers.[^3] The example returns its input when nothing changes and copies it at the first write:

```python run id=clone-on-first-write
copies = 0


def without_negatives(items):
    """Return items itself if nothing changes, else a changed copy."""
    global copies
    result = items
    for i, x in enumerate(items):
        if x < 0:
            if result is items:  # the first write makes the copy
                result = list(items)
                copies += 1
            result[i] = -x
    return result


calls = [[1, 2, 3], [4, -5, 6], [7, 8]]
outputs = [without_negatives(c) for c in calls]
print("outputs:", outputs)
print("copies made:", copies, "of", len(calls), "calls")
print("first call returned its own list:", outputs[0] is calls[0])
```

```output for=clone-on-first-write
outputs: [[1, 2, 3], [4, 5, 6], [7, 8]]
copies made: 1 of 3 calls
first call returned its own list: True
```

- **Virtual memory.** The `fork()` system call shares the physical pages of the parent with the child. The kernel marks the pages read-only and, when one process writes to a page, allocates a new physical page for it.[^1]
- **Storage.** In a copy-on-write file system, a change writes only the affected blocks to new locations and updates the metadata to point to them. This enables snapshots.[^1]

An [Immutable object](immutable-object) is the shared copy: copy-on-write is the rule for how a writer makes the next one. Qt types that are implicitly shared use atomic compare-and-swap operations on their reference counter.[^1]

## Known uses

- Java: `CopyOnWriteArrayList`.[^2]
- Rust: `std::borrow::Cow`, `Rc::make_mut` and `Arc::make_mut`.[^3]
- Qt: many types are copy-on-write, "implicitly shared" in Qt's terms.[^1]
- PHP implements all types except references as copy-on-write.[^1]
- The file systems ZFS, Btrfs, ReFS, APFS and Bcachefs, and Docker image layers.[^1]

## References
