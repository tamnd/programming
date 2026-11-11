---
tag: 3EMVDQ
kind: concept
slug: atomic-operations
name: Atomic operations and memory models
aliases: [atomics, atomic operations, memory model, memory ordering, compare-and-swap, CAS, happens-before, sequential consistency, acquire and release]
area: concurrency-and-parallelism/atomics-and-memory-models
level: core
summary: >-
  An atomic operation reads or changes shared memory in one indivisible step. A memory model states which writes of other threads a read can see, and which orders of memory access a program can rely on.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1979
  people: [Leslie Lamport]
  note: Lamport defined sequential consistency, the model in which all threads see one interleaving of the operations of all processors in program order.
see_also: [data-races, mutual-exclusion, threads, channels, undefined-behavior]
same_as:
  wikidata: Q6815743
  wikipedia: Memory_model_(programming)
sources:
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/concurrent/atomic/package-summary.html
  title: java.util.concurrent.atomic package summary, Java SE 25 API documentation
  locator: package description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.interlocked
  title: Interlocked Class, .NET API reference, Microsoft Learn
  locator: definition
- url: https://en.cppreference.com/w/cpp/atomic/memory_order
  title: std::memory_order, cppreference.com
  locator: Explanation
- url: https://www.microsoft.com/en-us/research/uploads/prod/2016/12/How-to-Make-a-Multiprocessor-Computer-That-Correctly-Executes-Multiprocess-Programs.pdf
  title: How to Make a Multiprocessor Computer That Correctly Executes Multiprocess Programs, L. Lamport, IEEE Transactions on Computers C-28(9), 1979
  locator: page 690
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-17.html#jls-17.4.5
  title: The Java Language Specification, Java SE 25 Edition
  locator: 17.4.5 Happens-before Order
- url: https://go.dev/ref/mem
  title: The Go Memory Model, version of June 6, 2022
  locator: Advice, Informal Overview and Atomic Values
- url: https://en.cppreference.com/w/cpp/atomic/atomic
  title: std::atomic, cppreference.com
  locator: description
- url: https://en.cppreference.com/w/c/atomic
  title: Concurrency support library, atomic operations, C, cppreference.com
  locator: Atomic operations
- url: https://doc.rust-lang.org/std/sync/atomic/index.html
  title: Module std::sync::atomic, Rust standard library documentation
  locator: Atomic types
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Atomics
  title: Atomics, MDN Web Docs
  locator: introduction
- url: https://docs.python.org/3.14/howto/free-threading-python.html
  title: Python support for free threading, Python 3.14 documentation
  locator: Thread safety
---

## Idea

When two threads change the same variable, an operation that looks like one step can be several steps. `counter += 1` reads the value, adds one, and writes the result. If two threads read the same old value before either one writes, one of the increments is lost. An atomic operation is a read, a write or a read-modify-write that other threads cannot see half done: it occurs completely or not at all. A memory model is the set of rules that says which values a read in one thread can see, when other threads write to memory at the same time:

```python run id=idea
counter = 0

a = counter
b = counter
counter = a + 1
counter = b + 1
print("two increments, interleaved:", counter)

import threading

counter = 0
lock = threading.Lock()


def work():
    global counter
    for _ in range(100_000):
        with lock:
            counter += 1


threads = [threading.Thread(target=work) for _ in range(4)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("with a lock:", counter)
```

```output for=idea
two increments, interleaved: 1
with a lock: 400000
```

The first part shows by hand the order in which two threads can run the steps of `counter += 1`. The second part makes the steps indivisible with a lock. An atomic instruction does the same for one memory word, with no lock: the processor does the read, the change and the write as one operation.

## How it works

**Indivisible operations.** Processors provide atomic loads and stores of aligned words, and atomic read-modify-write instructions, such as fetch-and-add, exchange, and compare-and-swap. A compare-and-swap writes a new value only if the variable still holds an expected value, and it reports if it succeeded. With it, a program can make any update atomic: read the value, compute the new value, and retry if another thread changed the variable in the meantime. Languages expose these instructions as atomic types. Java describes its atomic classes as "a small toolkit of classes that support lock-free thread-safe programming on single variables."[^1] C# `Interlocked` "provides atomic operations for variables that are shared by multiple threads."[^2]

**Reordering.** Atomicity is not enough. Compilers reorder memory accesses that are independent in one thread, and processors with store buffers and caches make writes visible to other cores in a different order from the program order. cppreference states: "Absent any constraints on a multi-core system, when multiple threads simultaneously read and write to several variables, one thread can observe the values change in an order different from the order another thread wrote them."[^3] A thread that writes data and then sets a flag can have the flag seen first.

**Sequential consistency.** The simplest model is the one that Leslie Lamport defined in 1979: "the result of any execution is the same as if the operations of all the processors were executed in some sequential order, and the operations of each individual processor appear in this sequence in the order specified by its program."[^4] Real hardware does not give this model for plain accesses, because it would make every memory access slow.

**Data races and the DRF guarantee.** Language memory models solve the problem with a contract. A data race is two accesses to the same location from different threads, at least one of them a write, that are not ordered by synchronization. The Java specification states: "When a program contains two conflicting accesses that are not ordered by a happens-before relationship, it is said to contain a data race."[^5] In return for no data races, the language gives sequential consistency: "If a program is correctly synchronized, then all executions of the program will appear to be sequentially consistent".[^5] C and C++ go further, and a data race is undefined behavior. Go defines a data race in the same way, "unless all the accesses involved are atomic data accesses as provided by the sync/atomic package."[^6] See [data races](/data-races/).

**Happens-before.** Synchronization operations create the order between threads. A release store to an atomic variable and an acquire load that reads the stored value are the basic pair. cppreference states that then "the store in thread A synchronizes-with the load in thread B. All memory writes (including non-atomic and relaxed atomic) that happened-before the atomic store in thread A also happened-before the atomic load in thread B."[^3] Locks, thread start and join, and channel operations create the same edges.

**Memory orders.** C, C++ and Rust let each atomic operation choose its order. `relaxed` operations "are not synchronization operations; they do not impose an order among concurrent memory accesses. They only guarantee atomicity and modification order consistency."[^3] `release` and `acquire` give the pairing above. `seq_cst`, the default, also gives "a single total order ... in which all threads observe all modifications in the same order".[^3] Java, Go and JavaScript give sequentially consistent atomics only, which is simpler and in most programs fast enough.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | absent | `threading.Lock`, the GIL, queues |
| JavaScript | native | `Atomics` on a `SharedArrayBuffer` |
| TypeScript | native | as JavaScript |
| Java | stdlib | `java.util.concurrent.atomic`, `volatile`, `VarHandle` |
| Kotlin | stdlib | the Java atomic classes on the JVM |
| C# | stdlib | `Interlocked`, `Volatile`, `volatile` fields |
| Go | stdlib | `sync/atomic` types, all sequentially consistent |
| Rust | stdlib | `std::sync::atomic` types with an `Ordering` per operation |
| C | stdlib | `_Atomic`, `<stdatomic.h>`, memory orders |
| C++ | stdlib | `std::atomic`, `std::atomic_ref`, `std::memory_order` |
| Bash | n/a | no shared memory; `flock` for files |
| PowerShell | stdlib | `[System.Threading.Interlocked]` |
| SQL | n/a | atomic statements and transactions |
| Regex, HTML, CSS | n/a | no shared memory |

**C** and **C++** define the reference model. `std::atomic<T>` makes concurrent access well defined: "If one thread writes to an atomic object while another thread reads from it, the behavior is well-defined".[^7] C11 has the same operations in `<stdatomic.h>`.[^8] **Rust** uses the C++ model: "Atomic types provide primitive shared-memory communication between threads, and are the building blocks of other concurrent types."[^9]

**Java** has atomic classes and `volatile` fields, and the Java memory model in chapter 17 of the specification defines happens-before.[^5] **C#** has `Interlocked` and `Volatile`.[^2]

**Go** has `sync/atomic`, and "all the atomic operations executed in a program behave as though executed in some sequentially consistent order." The memory model adds that this has "the same semantics as C++'s sequentially consistent atomics and Java's volatile variables."[^6] The same document also says: "If you must read the rest of this document to understand the behavior of your program, you are being too clever. Don't be clever."[^6]

**JavaScript** shares memory between workers only through a `SharedArrayBuffer`. "The Atomics namespace object contains static methods for carrying out atomic operations."[^10]

**Python** has no atomic types. The global interpreter lock protects the interpreter, not the programs that run on it. The free-threaded build, in which the GIL is disabled, keeps the same guarantees for built-in types: "Built-in types like dict, list, and set use internal locks to protect against concurrent modifications in ways that behave similarly to the GIL."[^11] A program that needs a read-modify-write step to be atomic uses a lock.

## Variations and related ideas

**Locks.** A [mutex](/mutual-exclusion/) makes a whole block of code atomic. It is built from atomic instructions and from support in the operating system to put waiting threads to sleep.

**Lock-free and wait-free algorithms.** A lock-free data structure uses compare-and-swap loops so that some thread always makes progress, even if other threads stop. A wait-free structure guarantees progress for every thread. Both are hard to write correctly.

**The ABA problem.** A compare-and-swap checks only the value. If another thread changes a value from A to B and back to A, the check passes, although the data changed. Version counters and safe memory reclamation schemes, such as hazard pointers and epochs, solve it.

**Fences.** A fence, or memory barrier, orders accesses without a specific atomic variable. `std::atomic_thread_fence` and `VarHandle.fullFence` are examples.

**Transactional memory.** A transaction makes a block of reads and writes on many locations atomic, and retries the block if another thread changed the same data. Haskell has software transactional memory, and some processors have had hardware support.

**Message passing.** [Channels](/channels/) and actors avoid shared variables, so most code needs no atomics or memory orders.

## Pitfalls

**Check then act.** Two atomic operations are not one atomic operation. A load followed by a store, such as `if (x.load() == 0) x.store(1)`, lets another thread act between them. Use a compare-and-swap or a lock.

**`volatile` in C and C++.** `volatile` stops the compiler from removing or merging accesses, which is for memory-mapped hardware. It does not make an access atomic, and it does not order other accesses. In Java and C#, `volatile` has a different meaning, and it does give visibility and ordering.

**Relaxed order for a flag.** A relaxed store to a flag does not publish the data written before it. Use release and acquire, or the default sequentially consistent order.

**Tests that pass on x86.** x86 processors reorder memory less than ARM and POWER processors. Code with a missing barrier can pass all tests on x86 and fail on ARM. Use the race detector of the language and tools such as ThreadSanitizer.

**Contention and false sharing.** Many threads that update one atomic variable force the cache line to move between cores, so the update can be much slower than a plain increment. Two variables on the same cache line slow each other down even when they are different variables. Per-thread counters that are summed at the end, such as Java `LongAdder`, avoid this.

**Torn values.** A plain access to a value wider than a machine word, such as a 64-bit integer on a 32-bit system or a structure, can be split into parts. Another thread can see half of an old value and half of a new one.

## Cost

An atomic load or store with relaxed or acquire and release order often costs the same as a plain access on x86, and a little more on ARM. A read-modify-write instruction or a sequentially consistent store is slower, typically some tens of cycles without contention, because the processor must drain its store buffer and own the cache line. With contention, the cost grows with the number of cores that fight for the line. An atomic operation is still much cheaper than a lock that makes a thread sleep, but it is far more expensive than code that shares nothing.

## History

Leslie Lamport defined sequential consistency in 1979.[^4] Atomic instructions such as test-and-set and compare-and-swap were part of mainframe instruction sets in the 1960s and 1970s. Maurice Herlihy showed in 1991 that compare-and-swap can implement any wait-free object. Java was the first mainstream language with a memory model in its specification, and Jeremy Manson, William Pugh and Sarita Adve revised it in 2004 for Java 5, with happens-before and the guarantee for programs without data races. Hans Boehm and Sarita Adve led the model for C++11, which C11 shares, and which Rust uses. Go published a revised memory model in 2022 that follows the same approach.[^6]

## References
