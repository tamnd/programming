---
tag: QTM1MV
kind: pattern
slug: immutable-object
name: Immutable object
aliases: [immutable object, immutable, unchangeable object, value object]
area: software-design/concurrency-patterns
level: core
summary: >-
  An immutable object cannot change after its constructor ends. Threads can share it and read it with no lock, and an update makes a new object instead of a change to the shared one.
family: concurrency
polarity: positive
intent: Make shared state safe for many threads by making it not mutable, so that each thread that holds a reference sees the same values for the whole life of the object.
also_known_as: [Unchangeable object]
see_also: [thread-confinement, double-checked-locking, data-races, atomic-operations, mutual-exclusion, threads, observer-pattern]
sources:
  - url: https://en.wikipedia.org/wiki/Immutable_object
    title: Immutable object, Wikipedia
    locator: lead; Weak vs strong immutability; Referencing vs copying objects; Thread safety; Violating immutability; Language-specific details
  - url: https://web.mit.edu/6.031/www/fa19/classes/20-thread-safety/
    title: "MIT 6.031 Software Construction, Reading 20: Thread Safety"
    locator: Strategy 2, Immutability
  - url: https://docs.oracle.com/javase/specs/jls/se21/html/jls-17.html
    title: The Java Language Specification, Java SE 21, Chapter 17 Threads and Locks
    locator: 17.5 final Field Semantics
  - url: https://docs.python.org/3/library/dataclasses.html
    title: dataclasses, Data Classes, Python documentation
    locator: "@dataclass, frozen; replace(); FrozenInstanceError"
---

## Intent

An immutable object is an object whose state cannot be modified after it is created.[^1] Immutability tackles the shared-mutable-state cause of a race condition and solves it simply by making the shared state not mutable.[^2]

## Problem

Several threads read an object while another thread changes it. If the change writes two fields, a reader can run between the two writes and see the new value of one field with the old value of the other. A lock around every read and every write prevents this, but each reader then pays for the lock and waits for the writer.

## Solution

Set all the state of the object in its constructor, and give the object no method that changes it. To change a value, make a new object and publish a reference to it. The MIT 6.031 reading gives a stronger definition of immutability for threads:[^2]

- no mutator methods,
- all fields are private and final,
- no representation exposure,
- no mutation whatsoever of mutable objects in the representation, not even beneficent mutation such as a cache.[^2]

```text
reader 1 --+
reader 2 --+--> current --> Settings(8000)      (never changes)
reader 3 --+        \
writer: current = Settings(8001)   a new object; readers that hold
                                   the old reference still see 8000
```

In Java, `final` fields allow programmers to implement thread-safe immutable objects without synchronization.[^3] A thread that can only see a reference to an object after its constructor finishes is guaranteed to see the correctly initialized values of the object's `final` fields.[^3]

## Example

The example has 4 readers that each read a shared `Settings` object 1000 times, and a writer that publishes 100 new objects. Each reader takes the current reference once and then checks that the port and the tag of that object agree.

```text
reads: 4000
reads that saw a mixed pair: 0
settings published: 101
first settings unchanged: True
```

The examples for each language show the same program.

## Consequences

Benefits:

- Multiple threads can act on data represented by immutable objects without concern of the data being changed by other threads.[^1]
- A program can copy a reference to an immutable object instead of the whole object. This conserves memory and avoids calls to constructors.[^1]
- Immutable objects are simpler to understand and reason about than mutable objects.[^1]

Liabilities:

- Each change makes a new object, and the writer must still publish the new reference safely. In Java, the constructor must not write a reference to the object being constructed in a place where another thread can see it before the constructor ends.[^3]
- Immutability is a compile-time construct for the normal interface of the object. Code can still change the memory by circumventing the type system or violating const correctness in C or C++.[^1]
- Hidden mutation, such as caching or lazy computation, is not safe for concurrency. An immutable type that uses it must make itself threadsafe using locks.[^2]

A mutable object that the writer changes in place lets a reader see half of the update:

```python wrong id=mutable-half-update
import threading


class Settings:  # mutable: the writer changes the shared object in place
    def __init__(self, port):
        self.port = port
        self.tag = f"port {port}"


shared = Settings(8000)
half_done = threading.Event()
seen = threading.Event()


def writer():
    shared.port = 8001
    half_done.set()
    seen.wait()  # the reader runs between the two writes
    shared.tag = "port 8001"


t = threading.Thread(target=writer)
t.start()
half_done.wait()
print("reader saw:", shared.port, "with tag", repr(shared.tag))
seen.set()
t.join()
```

```output for=mutable-half-update
reader saw: 8001 with tag 'port 8000'
```

## When not to use it

Do not use an immutable object for large data that changes often in small steps, such as a big array that a loop updates one element at a time. Each step would copy the whole object. Keep such data confined to one thread, or guard it with a lock.

## Variants and related patterns

- **Weak and strong immutability.** If only some fields cannot change, the object is weakly immutable. If all fields are immutable, the object is immutable, and if the class also cannot be extended, the object is called strongly immutable.[^1]
- **Copy with changes.** A method returns a new object with some fields changed. Python's `dataclasses.replace` creates a new object of the same type, replacing fields with values from its arguments.[^4] With `frozen=True`, assigning to fields generates an exception.[^4]

```python run id=frozen-dataclass
from dataclasses import FrozenInstanceError, dataclass, replace


@dataclass(frozen=True)
class Settings:
    port: int
    tag: str


s = Settings(8000, "port 8000")
try:
    s.port = 9000
except FrozenInstanceError:
    print("assignment to a field: FrozenInstanceError")
t = replace(s, port=9000, tag="port 9000")  # a changed copy
print("old:", s.port, s.tag)
print("new:", t.port, t.tag)
```

```output for=frozen-dataclass
assignment to a field: FrozenInstanceError
old: 8000 port 8000
new: 9000 port 9000
```

- **Copy-on-write.** The system shares one object until a user modifies it, and then makes a real copy for that user.[^1] The [copy-on-write](copy-on-write) page describes this pattern for threads.

[Thread confinement](thread-confinement) is the other way to avoid a lock: it does not share the mutable state at all.[^2]

## Known uses

- In Python, Java and the .NET Framework, strings are immutable objects.[^1]
- All the primitive wrapper classes in Java are immutable.[^1]
- In Python, numbers, Booleans, strings, tuples and frozensets are immutable.[^1]
- C# has records, which are immutable.[^1]

## References
