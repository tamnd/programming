---
tag: 28F2RD
kind: pattern
slug: thread-confinement
name: Thread confinement
aliases: [thread confinement, confinement, stack confinement, thread-confined object]
area: software-design/concurrency-patterns
level: core
summary: >-
  Thread confinement keeps mutable data reachable from one thread only. No other thread can read or write it, so the data needs no lock. Local variables, UI threads and ownership in Rust use this idea.
family: concurrency
polarity: positive
intent: Avoid races on mutable data by keeping the data, and every reference to it, confined to a single thread.
also_known_as: [Confinement]
see_also: [thread-specific-storage, active-object, producer-consumer, event-driven-programming, data-races, channels, message-passing-concept, threads]
sources:
  - url: https://web.mit.edu/6.031/www/fa19/classes/20-thread-safety/
    title: "MIT 6.031 Software Construction, Reading 20: Thread Safety"
    locator: Strategy 1, Confinement; Avoid Global Variables
  - url: https://docs.oracle.com/javase/tutorial/uiswing/concurrency/dispatch.html
    title: The Event Dispatch Thread, The Java Tutorials
    locator: whole page
  - url: https://go.dev/doc/effective_go
    title: Effective Go
    locator: Concurrency, Share by communicating
  - url: https://doc.rust-lang.org/book/ch16-04-extensible-concurrency-sync-and-send.html
    title: The Rust Programming Language, 16.4 Extensible Concurrency with Send and Sync
    locator: Transferring Ownership Between Threads
  - url: https://developer.mozilla.org/en-US/docs/Web/API/Web_Workers_API/Transferable_objects
    title: Transferable objects, MDN Web Docs
    locator: introduction
---

## Intent

Thread confinement avoids races on reassignable references and mutable data by keeping the references or data confined to a single thread.[^1] Other threads do not get the ability to read or write them directly.[^1]

## Problem

Shared mutable state is the root cause of a race condition.[^1] A lock makes the access safe, but each access then pays for the lock, and each piece of code that uses the data must remember to take it. Many objects are not thread-safe at all. Most Swing object methods are not thread safe, and invoking them from multiple threads risks thread interference or memory consistency errors.[^2]

## Solution

Do not share the mutable state.[^1] Give each piece of mutable data one owner thread, and make sure that no reference to it is reachable from another thread. There are three common forms:

- **Stack confinement.** Local variables are always thread confined: each thread has its own stack, and each invocation of a method has its own private copy of the variable.[^1] If the variable refers to a mutable object, the object must be confined as well.[^1]
- **Owner thread.** One thread owns a set of objects, and other threads send it work. All Swing component methods, except the ones labelled thread safe, must be invoked from the event dispatch thread. Other code schedules tasks on that thread with `invokeLater` or `invokeAndWait`.[^2]
- **Transfer of ownership.** A thread gives the data to another thread and then stops using it. In Go, shared values are passed around on channels, and only one goroutine has access to the value at any given time.[^3]

```text
thread 1: counts = {}  -- fills counts --+
thread 2: counts = {}  -- fills counts --+--> join --> main thread merges
thread 3: counts = {}  -- fills counts --+     (ownership passes to main)
```

## Example

The example gives each of 4 threads a chunk of 10000 words. Each thread counts words into a local dictionary with no lock. When the threads end, the main thread owns the 4 dictionaries and merges them.

```text
chunks: 4
words counted: 40000
distinct words: 5
count of 'a': 8000
```

The examples for each language show the same program.

## Consequences

Benefits:

- Confinement solves the race condition by not sharing the mutable state.[^1] The confined code needs no locks.
- Code that is not thread-safe can still be used, if only its owner thread calls it.[^2]
- In Go, data races cannot occur, by design, when values pass on channels and only one goroutine has the value at a time.[^3]

Liabilities:

- In most languages the rule is a convention. Unlike local variables, static variables are not automatically thread confined: if a program has them, the programmer has to argue that only one thread will ever use them, and document it.[^1] Programs that ignore the Swing rule may function correctly most of the time, but are subject to unpredictable errors that are difficult to reproduce.[^2]
- An owner thread can become a bottleneck. Tasks on the event dispatch thread must finish quickly. If they do not, unhandled events back up and the user interface becomes unresponsive.[^2]

A reference that escapes breaks the confinement, and nothing reports it:

```python wrong id=escaped-reference
import threading

escaped = []  # a global: not confined to any thread
published = threading.Event()
changed = threading.Event()


def worker():
    items = [1, 2, 3]  # meant to be confined to this thread
    escaped.append(items)  # wrong: the reference escapes
    published.set()
    changed.wait()
    print("worker expected sum 6, got:", sum(items))


t = threading.Thread(target=worker)
t.start()
published.wait()
escaped[0].append(100)  # another thread changes the "confined" list
changed.set()
t.join()
```

```output for=escaped-reference
worker expected sum 6, got: 106
```

## When not to use it

Do not confine data that many threads must update often at the same time, such as a shared counter of requests. Each update must then go through the owner thread. Effective Go notes that the channel approach can be taken too far: reference counts may be best done by putting a mutex around an integer variable.[^3]

## Variants and related patterns

- **Owner thread with a task queue.** Other threads put functions on the owner's queue, the same form as `invokeLater` in Swing.[^2] The example confines a label to its owner thread:

```python run id=owner-thread
import queue
import threading

tasks = queue.Queue()
label = {"text": ""}  # confined to the owner thread


def owner():
    while (task := tasks.get()) is not None:
        task()


def invoke_later(fn):  # other threads send work instead of touching label
    tasks.put(fn)


owner_thread = threading.Thread(target=owner)
owner_thread.start()
ran_on_owner = []


def make_update(n):
    def update():
        ran_on_owner.append(threading.current_thread() is owner_thread)
        label["text"] = f"done {n}"
    return update


helpers = [threading.Thread(target=invoke_later, args=(make_update(n),)) for n in (1, 2, 3)]
for h in helpers:
    h.start()
    h.join()
tasks.put(None)
owner_thread.join()
print("label:", label["text"])
print("updates ran on the owner thread:", all(ran_on_owner) and len(ran_on_owner) == 3)
```

```output for=owner-thread
label: done 3
updates ran on the owner thread: True
```

- **Compiler-checked ownership.** In Rust, the `Send` marker trait indicates that ownership of values of the type can be transferred between threads.[^4] `Rc<T>` does not implement `Send`, so the type system ensures that a program can never accidentally send an `Rc<T>` value across threads unsafely.[^4]
- **Transferable objects.** In JavaScript, a transferable object such as an `ArrayBuffer` can move from one thread to another. Following a transfer, the original object is no longer usable, and any attempt to read or write it throws an exception.[^5]

[Thread-Specific Storage](thread-specific-storage) gives each thread its own copy of one logical object. [Active Object](active-object) confines an object to its own thread and turns method calls into requests on a queue.

## Known uses

- Swing confines its components to the event dispatch thread.[^2]
- Go programs pass values on channels, by the slogan "Do not communicate by sharing memory; instead, share memory by communicating."[^3]
- Rust types such as `Rc<T>` are confined to one thread by the compiler.[^4]
- Web workers transfer an `ArrayBuffer`, so that its memory is attached to only one thread at a time.[^5]

## References
