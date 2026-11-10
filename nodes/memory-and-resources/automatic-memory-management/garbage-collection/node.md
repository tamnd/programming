---
tag: NEW-garbage-collection
kind: concept
slug: garbage-collection
name: Garbage collection
aliases: [GC, automatic memory management, tracing garbage collection]
area: memory-and-resources/automatic-memory-management
level: core
summary: >-
  Garbage collection frees the memory of objects that the program can no longer reach. The program allocates objects but never frees them, and the runtime finds the dead ones.
applies_to: core-languages
viz: expected
formal: optional
history:
  year: 1960
  people: [John McCarthy, George E. Collins]
  note: McCarthy described a reclamation cycle for LISP that marks every list cell reachable from the program and returns the rest to the free list. In the same year Collins described reference counts for list cells.
see_also: [closures]
same_as:
  wikidata: Q322202
  wikipedia: Garbage_collection_(computer_science)
sources:
  - paper: doi:10.1145/367177.367199
    title: John McCarthy, Recursive functions of symbolic expressions and their computation by machine, Part I, Communications of the ACM 3(4), 1960
    locator: section 4c, the free-storage list and the reclamation cycle
  - paper: doi:10.1145/367487.367501
    title: George E. Collins, A method for overlapping and erasure of lists, Communications of the ACM 3(12), 1960
    locator: the reference count of each list cell and erasure when the count reaches zero
  - paper: doi:10.1145/362790.362798
    title: C. J. Cheney, A nonrecursive list compacting algorithm, Communications of the ACM 13(11), 1970
    locator: the copying algorithm that scans the copied cells as a queue
  - paper: doi:10.1145/359642.359655
    title: Edsger W. Dijkstra, Leslie Lamport, A. J. Martin, C. S. Scholten and E. F. M. Steffens, On-the-fly garbage collection, an exercise in cooperation, Communications of the ACM 21(11), 1978
    locator: the white, grey and black marking of nodes and the invariant that no black node points to a white node
  - paper: doi:10.1145/358141.358147
    title: Henry Lieberman and Carl Hewitt, A real-time garbage collector based on the lifetimes of objects, Communications of the ACM 26(6), 1983
    locator: the division of the heap into regions by object age
  - paper: doi:10.1145/800020.808261
    title: David Ungar, Generation Scavenging, a non-disruptive high performance storage reclamation algorithm, ACM SIGSOFT/SIGPLAN Software Engineering Symposium on Practical Software Development Environments, 1984
    locator: the copying of young objects only, and the remembered set of old objects that point to young ones
  - url: https://docs.python.org/3.15/library/gc.html#gc.set_threshold
    title: The Python Standard Library, gc, Garbage Collector interface, Python 3.15
    locator: gc.collect, gc.disable, gc.get_threshold, gc.set_threshold, gc.is_tracked
  - url: https://github.com/python/cpython/blob/main/InternalDocs/garbage_collector.md
    title: CPython InternalDocs, Garbage collector design
    locator: Identifying reference cycles; Optimization, generations; GC for the free-threaded build
  - url: https://docs.python.org/3.15/whatsnew/3.15.html#gc
    title: What's New In Python 3.15
    locator: Improved modules, gc
  - url: https://docs.python.org/3.14/reference/datamodel.html#object.__del__
    title: The Python Language Reference, Data model, Python 3.14
    locator: Basic customization, object.__del__
  - url: https://docs.python.org/3.14/library/weakref.html
    title: The Python Standard Library, weakref, Weak references, Python 3.14
    locator: the introduction, weakref.ref and WeakValueDictionary
  - url: https://docs.python.org/3.14/reference/compound_stmts.html#the-with-statement
    title: The Python Language Reference, Compound statements, Python 3.14
    locator: 8.5 The with statement
  - url: https://tc39.es/ecma262/2026/#sec-weak-ref-objects
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 26.1 WeakRef Objects
  - url: https://tc39.es/ecma262/2026/#sec-finalization-registry-objects
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 26.2 FinalizationRegistry Objects
  - url: https://tc39.es/ecma262/2026/#sec-liveness
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 9.9.2 Liveness and 9.9.3 Execution
  - url: https://tc39.es/ecma262/2026/#sec-clear-kept-objects
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 9.10 ClearKeptObjects and 9.11 AddToKeptObjects
  - url: https://docs.oracle.com/en/java/javase/25/gctuning/ergonomics.html
    title: Java SE 25 HotSpot Virtual Machine Garbage Collection Tuning Guide
    locator: 2 Ergonomics, Garbage Collector, Heap, and Runtime Compiler Default Selection
  - url: https://docs.oracle.com/en/java/javase/25/gctuning/garbage-first-g1-garbage-collector1.html
    title: Java SE 25 HotSpot Virtual Machine Garbage Collection Tuning Guide
    locator: 7 Garbage-First (G1) Garbage Collector, Basic Concepts, Remembered Set, and Table 7-1 (MaxGCPauseMillis)
  - url: https://openjdk.org/jeps/333
    title: "JEP 333: ZGC: A Scalable Low-Latency Garbage Collector (Experimental)"
    locator: Goals
  - url: https://openjdk.org/jeps/376
    title: "JEP 376: ZGC: Concurrent Thread-Stack Processing"
    locator: Summary and Goals
  - url: https://openjdk.org/jeps/490
    title: "JEP 490: ZGC: Remove the Non-Generational Mode"
    locator: Summary
  - url: https://openjdk.org/jeps/523
    title: "JEP 523: Make G1 the Default Garbage Collector in All Environments"
    locator: Summary
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/System.html#gc()
    title: Java SE 25 API, java.lang.System
    locator: gc()
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/ref/WeakReference.html
    title: Java SE 25 API, java.lang.ref.WeakReference
    locator: class description
  - url: https://openjdk.org/jeps/421
    title: "JEP 421: Deprecate Finalization for Removal"
    locator: Summary and Motivation
  - url: https://go.dev/doc/gc-guide#GOGC
    title: A Guide to the Go Garbage Collector
    locator: Tracing garbage collection; GOGC; Latency
  - url: https://go.dev/doc/gc-guide#Memory_limit
    title: A Guide to the Go Garbage Collector
    locator: Memory limit
  - url: https://go.dev/doc/go1.26#runtime
    title: Go 1.26 Release Notes
    locator: Runtime, New garbage collector
  - url: https://pkg.go.dev/runtime#GC
    title: Go standard library, package runtime
    locator: func GC, func AddCleanup, func SetFinalizer
  - url: https://pkg.go.dev/weak
    title: Go standard library, package weak
    locator: type Pointer, func Make
  - url: https://learn.microsoft.com/en-us/dotnet/standard/garbage-collection/fundamentals
    title: .NET documentation, Fundamentals of garbage collection
    locator: Generations; What happens during a garbage collection
  - url: https://learn.microsoft.com/en-us/dotnet/standard/garbage-collection/large-object-heap
    title: .NET documentation, The large object heap on Windows systems
    locator: How an object ends up on the LOH; When is a large object collected
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.gc.collect
    title: .NET API, GC.Collect Method
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/using
    title: C# language reference, using keyword
    locator: the using statement
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p2186r2.html
    title: "P2186R2: Removing Garbage Collection Support"
    locator: Abstract and Wording
  - url: https://timsong-cpp.github.io/cppwp/n4950/util.smartptr.shared
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[util.smartptr.shared], class template shared_ptr"
  - url: https://doc.rust-lang.org/std/rc/index.html
    title: The Rust Standard Library, module std::rc
    locator: module documentation, the paragraphs on cycles and Weak
  - url: https://doc.rust-lang.org/std/rc/struct.Weak.html
    title: The Rust Standard Library, struct std::rc::Weak
    locator: struct description
  - url: https://kotlinlang.org/docs/native-memory-manager.html
    title: Kotlin documentation, Kotlin/Native memory management
    locator: Garbage collector
  - url: https://www.hboehm.info/gc/
    title: A garbage collector for C and C++ (Boehm, Demers, Weiser)
    locator: the project page
  - url: https://docs.python.org/3.15/library/sys.html#sys.getsizeof
    title: The Python Standard Library, sys, Python 3.15
    locator: sys.getsizeof
---

## Idea

A program makes objects on the heap all the time: strings, lists, records, closures. Each object uses memory until something frees it. With manual memory management, the programmer frees each object. A free that comes too early gives a dangling pointer, and a free that never comes gives a leak. Garbage collection gives this work to the runtime. The program allocates objects and never frees them. The runtime finds the objects that the program can no longer use and frees their memory.

The runtime cannot know which objects the program will use in the future. It uses a safe approximation: an object that no chain of references reaches from the running program is garbage, because no code can ever touch it again. The two main methods are reference counting and tracing.[^1][^2] CPython uses both. Reference counting frees most objects at once, and a tracing pass finds groups of objects that refer to each other in a cycle.[^8] In this run, the automatic cycle pass is off, so the cycle stays in memory until `gc.collect()` runs.

```python run id=idea
import gc
import weakref


class Item:
    def __init__(self):
        self.peer = None


gc.disable()  # no automatic cycle collection during this demo

single = Item()
single_probe = weakref.ref(single)
del single
print("single freed at once:", single_probe() is None)

a, b = Item(), Item()
a.peer, b.peer = b, a
cycle_probe = weakref.ref(a)
del a, b
print("cycle freed by del:", cycle_probe() is None)
gc.collect()
print("cycle freed by gc:", cycle_probe() is None)
```

```output for=idea
single freed at once: True
cycle freed by del: False
cycle freed by gc: True
```

A weak reference (`weakref.ref`) lets the program look at an object without keeping it alive.[^11] The probe returns `None` after the object is freed.

## How it works

The heap is a directed graph. Each object is a node, and each reference from one object to another is an edge. The roots are the references that the program holds directly: local variables in the active stack frames, global variables, and CPU registers. An object is reachable when a path of edges leads to it from a root. A collector frees only unreachable objects. A reachable object can still be useless to the program, and the collector keeps it. So garbage collection removes dangling pointers, but it does not remove all leaks.

```text
roots                       heap

local   order   ---------> Order ---------> Customer
global  cache   ---------> Cache

                           Item a <-------> Item b
```

`Order`, `Customer` and `Cache` are reachable. `Item a` and `Item b` refer to each other, but no path from a root leads to them, so both are garbage.

**Reference counting** stores in each object the number of references to it. Each copy of a reference adds one, and each removal subtracts one. When the count reaches zero, the object is freed, and the counts of the objects it refers to go down in turn.[^2] Memory comes back at once and in small steps. In the diagram, `Item a` and `Item b` each have a count of 1, from each other, so reference counting alone never frees them.

**Tracing** starts from the roots and follows every reference. The mark phase marks each object that it reaches. The sweep phase walks the whole heap and frees each unmarked object.[^1] Cycles are no problem, because only reachability counts. Tracing collectors differ in what they do with the live objects:

| Method | When memory comes back | Frees cycles | Moves objects | Main cost |
|---|---|---|---|---|
| Reference counting | when the count of an object reaches zero[^2] | no | no | a count update at each copy of a reference |
| Mark-sweep | at each collection[^1] | yes | no | the sweep visits the whole heap; free memory becomes fragmented |
| Copying | at each collection; live objects move to an empty space[^3] | yes | yes | half of the space is empty; the work grows with the live data, not with the garbage |
| Mark-compact | at each collection | yes | yes | extra passes to slide objects together and fix every pointer to them |
| Generational | often for young objects, rarely for old ones[^5][^6] | yes | usually | a record of the old objects that point to young ones |

A generational collector divides the heap by age. Most objects die soon after they are made, so a collection of the young part frees much memory for little work. The collector must also know about references from old objects into the young part. It keeps them in a remembered set, which code that the compiler adds to each pointer store (a write barrier) keeps up to date.[^6][^18]

A concurrent collector marks objects while the program runs, so the program changes the graph during the trace. The usual model colours each object white (not reached yet), grey (reached, but its references not yet scanned) or black (reached and scanned). The collector is correct when no black object points to a white object, because then every white object that is left at the end is garbage. A write barrier keeps this invariant when the program stores a pointer.[^4]

The pseudocode shows a mark-sweep collection with the three colours. `DROP` is the reference counting update, for comparison. The scene below runs both on small heaps.

```pseudo
DROP(x)                                   ▷ reference counting: one reference to x goes away
L1  count(x) ← count(x) − 1
L2  if count(x) = 0 then
L3      for each reference from x to an object y do DROP(y)
L4      free x

COLLECT()                                 ▷ tracing: mark from the roots, then sweep
L5  colour each object of the heap white
L6  for each root r do
L7      if r refers to a white object y then colour y grey
L8  while some object is grey do
L9      take a grey object x
L10     for each reference from x to an object y do
L11         if y is white then colour y grey
L12     colour x black                    ▷ no black object refers to a white object
L13 for each object x of the heap do       ▷ the sweep
L14     if x is white then free x
L15     else colour x white               ▷ ready for the next collection
```

CPython finds cycles with a variant of tracing that does not need the roots. For each container object in the generation that it collects, it copies the reference count and then subtracts each reference that comes from inside the set. An object with a count above zero after this step has a reference from outside, so it is reachable. Everything that these objects reach is reachable too. The rest is a group of objects that refer only to each other, and the collector frees them.[^8] A collection of the youngest generation starts when the number of allocations minus deallocations passes the first threshold.[^7]

```python run id=threshold
import gc

print(gc.get_threshold())
```

```output for=threshold
(2000, 10, 10)
```

## In languages

The example files show the same test in each language with a collector: make two objects that refer to each other, drop the references, ask for a collection, and check through a weak reference that the pair is gone. The Rust and C++ files show that reference counting alone leaks the cycle.

| Language | Verdict | Collector |
|---|---|---|
| Python | native | CPython: reference counting, plus a tracing cycle collector with three generations; the free-threaded build scans the whole heap at each collection[^8] |
| JavaScript | native | tracing; the specification defines only which objects the program can still observe, not the algorithm[^15] |
| TypeScript | native | the collector of the JavaScript engine that runs the output |
| Java | native | HotSpot: G1 (generational, region-based, mostly concurrent, moves objects) on server-class machines and Serial on small ones; JEP 523 makes G1 the default everywhere; ZGC and Parallel are options[^17][^18][^22] |
| C# | native | .NET: generational (0, 1, 2), mark and compact; large objects are swept[^31][^32] |
| Go | native | concurrent mark-sweep that does not move objects[^26] |
| Kotlin | native | the JVM collector on Kotlin/JVM; on Kotlin/Native, a concurrent mark and sweep collector without generations[^39] |
| PowerShell | native | the .NET collector, as for C# |
| Rust | stdlib | no collector; ownership frees each value at the end of its scope; `Rc` and `Arc` count references[^37] |
| C++ | stdlib | no collector; `std::shared_ptr` counts owners[^36] |
| C | absent | `malloc` and `free` by hand |
| Bash | n/a | the shell manages its own strings and arrays; a script has no references to objects |
| SQL | n/a | the database engine manages storage; a query has no object references |
| Regex | n/a | a pattern has no objects |
| HTML/CSS | n/a | markup and style have no objects; the DOM belongs to the JavaScript engine |

C has no collector in the standard. Programs free memory by hand, use arenas that free many objects at once, or link the Boehm-Demers-Weiser collector. That library is a conservative collector: it treats any word that looks like a heap address as a pointer.[^40] C++11 added a minimal interface for such collectors (`std::declare_reachable` and related functions). The authors of the removal paper knew of no implementation of its strict mode, and C++23 removed the interface.[^35]

Rust and C++ free memory at known points instead. A value is dropped (Rust) or destroyed (C++) at the end of the scope of its owner. Shared ownership uses a reference count. A cycle of `Rc` or `shared_ptr` values never reaches a count of zero, so it leaks. A weak pointer (`rc::Weak`, `std::weak_ptr`) does not add to the count, and it breaks the cycle.[^37][^38]

## Variations and related ideas

- **Conservative collection.** The collector does not know which words are pointers, so it treats each word that could be one as a pointer. It can keep some garbage alive, and it cannot move objects. The Boehm collector for C works this way.[^40]
- **Incremental and concurrent collection.** The collector does its work in small steps between program steps (incremental), or on other threads while the program runs (concurrent). Both need a barrier to keep the colour invariant.[^4]
- **Regions and evacuation.** G1 divides the heap into equal regions. It collects the regions with the most garbage first and copies their live objects into empty regions.[^18]
- **Weak references and finalization.** A weak reference does not keep its target alive. Python has `weakref`, JavaScript has `WeakRef` and `FinalizationRegistry`, Java has `java.lang.ref`, and Go has the `weak` package.[^11][^13][^14][^24][^30]
- **Ownership.** Rust decides at compile time where each value is freed. This needs no collector, but the program must state its ownership structure.
- **Arenas.** A program allocates a group of objects in one region and frees the region in one step.
- [[closures]] keep the variables that they capture reachable, so a closure that stays alive keeps its captured objects alive.

## Pitfalls

### Reference counting leaks cycles

A reference count never reaches zero for objects in a cycle. Rust and C++ have no collector to find the cycle, so the memory leaks until the process exits (see the Rust and C++ examples). The fix is to make one direction of each cycle weak: a child points to its parent through `Weak` or `std::weak_ptr`.[^37][^38] CPython has a cycle collector, but code that calls `gc.disable()` leaves cycles in memory, as in the Idea.

### A finalizer does not release a resource on time

A finalizer (`__del__` in Python, `finalize` in Java, a `FinalizationRegistry` callback in JavaScript) runs when the collector frees the object. The collector decides when that is. In this run, two connections form a cycle, and their `__del__` methods run only when the cycle collector runs.

```python wrong id=finalizer
import gc


class Connection:
    def __init__(self, name):
        self.name = name
        self.peer = None

    def __del__(self):
        print("closed", self.name)


gc.disable()
a = Connection("a")
b = Connection("b")
a.peer, b.peer = b, a
del a, b
print("after del")
gc.collect()
print("after collect")
```

```output for=finalizer
after del
closed a
closed b
after collect
```

Python does not guarantee that `__del__` runs for objects that still exist when the interpreter exits.[^10] Java deprecated finalization for removal because finalizers run late, on unknown threads, or not at all.[^25] The ECMAScript specification makes the cleanup job of a `FinalizationRegistry` optional.[^14][^15] Release a file, a socket or a lock with a scope that ends at a known point: `with` in Python, try-with-resources in Java, `using` in C#.[^12][^34]

```python run id=closing
from contextlib import closing


class Connection:
    def __init__(self, name):
        self.name = name

    def close(self):
        print("closed", self.name)


with closing(Connection("a")) as conn:
    print("using", conn.name)
print("after with")
```

```output for=closing
using a
closed a
after with
```

### A request for a collection is only a request

`System.gc()` in Java suggests a collection to the virtual machine.[^23] `GC.Collect()` in .NET forces a blocking collection of all generations, so every managed thread waits for it.[^33][^31] `runtime.GC()` in Go runs a collection and blocks the caller until it ends, and it can block the whole program.[^29] JavaScript has no standard way to start a collection; `globalThis.gc` exists only when Node.js or V8 starts with `--expose-gc`. The examples on this page call these functions to show a result. Production code should not depend on them, because each call can cost a full collection.

### A JavaScript WeakRef keeps its target alive until the job ends

When a script creates a `WeakRef` or reads its target, the engine keeps the target alive until the current synchronous run of code ends.[^16] So a collection in the same job cannot free the target.

```javascript wrong flags="--expose-gc" id=kept
let target = { name: "report" };
const probe = new WeakRef(target);
target = null;
globalThis.gc();
console.log("collected in the same job:", probe.deref() === undefined);
```

```output for=kept
collected in the same job: false
```

The JavaScript example waits for a timer before it calls `gc()`, so the job has ended and the target can go.

### Reachable is not the same as used

A collector frees only unreachable objects. An object in a global cache, in a list of event handlers, or in the variables of a live closure stays in memory even when the program never reads it again. Remove entries that the program no longer needs, give caches a size limit, or hold the values through weak references, for example in `weakref.WeakValueDictionary`.[^11]

## Cost

- **Memory per object.** In the default CPython build, an object of a type that can form cycles carries a header for the cycle collector. A plain `object()` is 16 bytes and is not tracked. An empty list is 40 bytes plus a 16-byte header, 56 bytes in all. `sys.getsizeof` includes this header and `__sizeof__` does not.[^7][^41]

```python run id=header
import gc
import sys

orders = []
print(sys.getsizeof(object()), gc.is_tracked(object()))
print(orders.__sizeof__(), sys.getsizeof(orders), gc.is_tracked(orders))
```

```output for=header
16 False
40 56 True
```

- **Heap headroom.** A tracing collector lets garbage build up between collections, so the heap is larger than the live data. Go sets the heap target to the live heap plus (live heap + GC roots) × GOGC / 100. With the default GOGC of 100, a program with 10 MiB of live data and few roots gets a target of about 20 MiB. The minimum target is 4 MiB.[^26] `GOMEMLIMIT` sets a soft limit on total memory; it has no limit by default.[^27]
- **CPU time.** The Go collector uses 25% of the CPU while it marks. When the program is near its memory limit, Go caps the collector at about 50% of the CPU, and lets memory pass the limit instead of spending more.[^26][^27]
- **Pauses.** G1 aims at a maximum pause of 200 ms by default (`-XX:MaxGCPauseMillis`).[^18] ZGC was designed for pauses under 10 ms, and it later moved stack scanning out of the pauses to spend less than 1 ms in them.[^19][^20] A .NET collection suspends all managed threads except the thread that started it.[^31] The free-threaded CPython build pauses the other threads during each collection.[^8]
- **Large objects.** .NET puts an object of 85,000 bytes or more on the large object heap. That heap is collected only with generation 2, and it is swept, not compacted, unless the program asks for compaction.[^32]
- **Reference counts.** Each copy and each drop of a reference changes a count. `Rc` changes it with plain operations, so it is cheap but cannot go to another thread; `Arc` uses atomic operations.[^37] Several threads can copy the same `shared_ptr` owner group without a data race, so its count also needs atomic updates.[^36]

## History

- **1960.** John McCarthy described the first garbage collector, for LISP. When the free-storage list was empty, the system marked every cell reachable from the program and put the other cells back on the list.[^1]
- **1960.** George Collins described reference counts for list cells, so that a cell could be erased when nothing referred to it.[^2]
- **1970.** C. J. Cheney published a copying collector that copies the live cells into a second space and scans them as a queue, with no recursion and no stack.[^3]
- **1978.** Dijkstra, Lamport, Martin, Scholten and Steffens described a collector that runs at the same time as the program, with the white, grey and black marking that concurrent collectors still use.[^4]
- **1983 and 1984.** Lieberman and Hewitt proposed dividing the heap by object age. Ungar's generation scavenging collected only the young objects and kept a remembered set of old objects that point to young ones.[^5][^6]
- **2018.** JDK 11 added ZGC, a concurrent collector with a pause goal of 10 ms.[^19]
- **2021.** P2186R2 removed the garbage collection support of C++11 from C++23.[^35]
- **2025.** JDK 24 removed the non-generational mode of ZGC, so ZGC is always generational.[^21]
- **2026.** Go 1.26 made the Green Tea collector the default. It marks and scans small objects with better locality, and its release notes expect 10 to 40% less collector overhead in programs that use the collector heavily.[^28] Python 3.14.5 went back to the generational cycle collector of Python 3.13, after the incremental collector of Python 3.14.0 to 3.14.4 used too much memory in some production programs.[^9]

## References
