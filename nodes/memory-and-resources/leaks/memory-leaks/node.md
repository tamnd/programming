---
tag: NEW-memory-leaks
kind: concept
slug: memory-leaks
name: Memory leak
aliases: [leak, memory leakage, CWE-401]
area: memory-and-resources/leaks
level: core
summary: >-
  A memory leak is memory that a program keeps after it can no longer use it. The memory comes back only when the process exits, so a long-running program grows until it slows down or fails.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1992
  people: [Reed Hastings, Bob Joyce]
  note: Purify instrumented object code to find memory access errors and leaks in C programs. It found leaked blocks with an adaptation of garbage collection techniques, which is the method that leak checkers still use at exit.
see_also: [garbage-collection, reference-counting, explicit-memory-management, resource-acquisition-is-initialization, stack-and-heap, observer-pattern, use-after-free, threads]
same_as:
  wikidata: Q751740
  wikipedia: Memory_leak
sources:
  - url: https://cwe.mitre.org/data/definitions/401.html
    title: "CWE-401: Missing Release of Memory after Effective Lifetime"
    locator: the weakness id, its Variant abstraction, and its parents CWE-772 and CWE-404
    accessed: 2026-11-10
  - url: https://web.stanford.edu/class/cs343/resources/purify.pdf
    title: "Reed Hastings and Bob Joyce, Purify: Fast Detection of Memory Leaks and Access Errors, Proceedings of the Winter 1992 USENIX Conference"
    locator: Abstract; Introduction; section 5, Memory Leaks
  - url: https://valgrind.org/docs/manual/mc-manual.html#mc-manual.leaks
    title: Valgrind User Manual, Memcheck, a memory error detector
    locator: 4.2.8 Memory leak detection
    accessed: 2026-11-10
  - url: https://valgrind.org/docs/manual/quick-start.html
    title: The Valgrind Quick Start Guide
    locator: Running your program under Memcheck
    accessed: 2026-11-10
  - url: https://valgrind.org/docs/valgrind2007.pdf
    title: "Nicholas Nethercote and Julian Seward, Valgrind: A Framework for Heavyweight Dynamic Binary Instrumentation, PLDI 2007"
    locator: section 3, How Valgrind Works
  - url: https://clang.llvm.org/docs/LeakSanitizer.html
    title: Clang documentation, LeakSanitizer
    locator: Introduction; Usage; Suppressing memory leaks; Supported Platforms
    accessed: 2026-11-10
  - url: https://github.com/google/sanitizers/wiki/AddressSanitizerLeakSanitizerDesignDocument
    title: google/sanitizers wiki, LeakSanitizer design document
    locator: Operation
    accessed: 2026-11-10
  - url: https://clang.llvm.org/docs/AddressSanitizer.html
    title: Clang documentation, AddressSanitizer
    locator: Introduction; Memory leak detection
    accessed: 2026-11-10
  - url: https://releases.llvm.org/3.4/tools/clang/docs/ReleaseNotes.html
    title: Clang 3.4 Release Notes
    locator: What's New in Clang 3.4
  - url: https://releases.llvm.org/
    title: LLVM Download Page
    locator: the release list, 02 Jan 2014, 3.4
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: ISO/IEC 9899:2024 working draft N3220, Programming languages, C
    locator: 7.24.3.7 The realloc function, paragraphs 3 and 4
  - url: https://eel.is/c++draft/except.ctor
    title: Working Draft, Programming Languages, C++
    locator: "[except.ctor] Stack unwinding, paragraphs 1 and 2"
    accessed: 2026-11-10
  - url: https://eel.is/c++draft/unique.ptr.general
    title: Working Draft, Programming Languages, C++
    locator: "[unique.ptr.general] paragraph 1 and Note 1"
    accessed: 2026-11-10
  - url: https://docs.python.org/3.14/library/tracemalloc.html
    title: The Python Standard Library, tracemalloc, Trace memory allocations, Python 3.14
    locator: the introduction; Snapshot.compare_to; tracemalloc.start and get_tracemalloc_memory
  - url: https://docs.python.org/3.14/library/functools.html#functools.lru_cache
    title: The Python Standard Library, functools, Python 3.14
    locator: functools.cache and functools.lru_cache
  - url: https://docs.python.org/3.14/library/weakref.html
    title: The Python Standard Library, weakref, Weak references, Python 3.14
    locator: the introduction; WeakKeyDictionary; WeakValueDictionary; WeakMethod
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-12.html#jls-12.6.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 12.6.1 Implementing Finalization
  - url: https://docs.oracle.com/en/java/javase/25/troubleshoot/troubleshooting-memory-leaks.html
    title: Java Platform, Standard Edition Troubleshooting Guide, Release 25, Troubleshoot Memory Leaks
    locator: The java.lang.OutOfMemoryError Error; Diagnosing Java Memory Leaks, Heap Dumps and Java Flight Recorder
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/WeakHashMap.html
    title: Java SE 25 API, java.util.WeakHashMap
    locator: class description and implementation note
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/LinkedHashMap.html#removeEldestEntry(java.util.Map.Entry)
    title: Java SE 25 API, java.util.LinkedHashMap
    locator: class description, access order; removeEldestEntry
  - url: https://tc39.es/ecma262/2026/#sec-weakmap-objects
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 24.3 WeakMap Objects
  - url: https://tc39.es/ecma262/2026/#sec-weak-ref-objects
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 26.1 WeakRef Objects
  - url: https://dom.spec.whatwg.org/#dom-eventtarget-addeventlistener
    title: DOM Living Standard
    locator: 2.7 Interface EventTarget, addEventListener, removeEventListener, AddEventListenerOptions signal and once
    accessed: 2026-11-10
  - url: https://nodejs.org/api/events.html#eventsdefaultmaxlisteners
    title: Node.js documentation, Events
    locator: events.defaultMaxListeners; events.getEventListeners
    accessed: 2026-11-10
  - url: https://nodejs.org/api/cli.html#--heapsnapshot-signalsignal
    title: Node.js documentation, Command-line API
    locator: --heapsnapshot-signal=signal
    accessed: 2026-11-10
  - url: https://nodejs.org/api/v8.html#v8writeheapsnapshotfilenameoptions
    title: Node.js documentation, V8
    locator: v8.writeHeapSnapshot
    accessed: 2026-11-10
  - url: https://developer.chrome.com/docs/devtools/memory-problems/heap-snapshots
    title: Chrome DevTools, Record heap snapshots
    locator: Comparison view; The Retainers section; Objects retained by detached nodes
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/events/how-to-subscribe-to-and-unsubscribe-from-events#unsubscribing
    title: C# Programming Guide, How to subscribe to and unsubscribe from events
    locator: Unsubscribing
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/dotnet/core/diagnostics/dotnet-gcdump
    title: .NET documentation, Heap analysis tool (dotnet-gcdump)
    locator: the introduction; dotnet-gcdump collect
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/dotnet/core/diagnostics/dotnet-dump
    title: .NET documentation, Dump collection and analysis utility (dotnet-dump)
    locator: Analyze SOS commands, dumpheap and gcroot
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.runtime.compilerservices.conditionalweaktable-2
    title: .NET API, ConditionalWeakTable<TKey,TValue>
    locator: Remarks
    accessed: 2026-11-10
  - url: https://pkg.go.dev/runtime#NumGoroutine
    title: Go standard library, package runtime
    locator: func NumGoroutine
    accessed: 2026-11-10
  - url: https://pkg.go.dev/context
    title: Go standard library, package context
    locator: Overview; Example WithCancel
    accessed: 2026-11-10
  - url: https://pkg.go.dev/runtime/pprof#Profile
    title: Go standard library, package runtime/pprof
    locator: type Profile, the predefined profiles and the heap profile
    accessed: 2026-11-10
  - url: https://pkg.go.dev/weak#Pointer
    title: Go standard library, package weak
    locator: type Pointer
    accessed: 2026-11-10
  - url: https://pkg.go.dev/time#NewTicker
    title: Go standard library, package time
    locator: func NewTicker
    accessed: 2026-11-10
  - url: https://go.dev/doc/go1.26#goroutineleak-profiles
    title: Go 1.26 Release Notes
    locator: Goroutine leak profiles
  - url: https://go.dev/doc/go1.27#goroutineleak-profiles
    title: Go 1.27 Release Notes
    locator: Goroutine leak profiles
  - url: https://doc.rust-lang.org/std/mem/fn.forget.html
    title: Rust standard library, std::mem::forget
    locator: Safety
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/std/boxed/struct.Box.html#method.leak
    title: Rust standard library, std::boxed::Box
    locator: Box::leak
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/book/ch15-06-reference-cycles.html
    title: The Rust Programming Language, 15.6 Reference Cycles Can Leak Memory
    locator: the introduction
    accessed: 2026-11-10
---

## Idea

A program asks for memory, uses it, and must give it back when it is done. A memory leak is memory that the program never gives back after its last use. CWE lists the weakness as CWE-401, Missing Release of Memory after Effective Lifetime.[^1] A short program that leaks a little is harmless, because the operating system takes all the memory back at exit. A server that leaks a little on each request grows until it slows down or fails.

There are two kinds of leak. In C and C++, the program frees each block by hand. A block leaks when the program loses its last pointer to the block, so no code can free it. A collector frees unreachable objects, so this kind cannot happen in Python, Java or Go. There, an object leaks when the program can still reach it but never uses it again. A cache that only grows is the usual case.

```python run id=idea
import weakref


class Report:
    def __init__(self, title):
        self.title = title
        self.rows = [0] * 100_000


_cache = {}


def load(title):
    report = Report(title)
    _cache[title] = report  # the cache keeps a reference
    return report


report = load("q3")
probe = weakref.ref(report)
del report
print("alive after del:", probe() is not None)
print("cache entries:", len(_cache))
_cache.pop("q3")
print("alive after pop:", probe() is not None)
```

```output
alive after del: True
cache entries: 1
alive after pop: False
```

The weak reference shows whether the object still exists, without keeping it alive.[^16] After `del report`, the program has no name for the report, but `_cache` still refers to it. The memory comes back only when the cache entry goes.

## How it works

A leak checker and a collector both start at the roots: global variables, thread stacks, CPU registers and thread-local storage. They follow each pointer from the roots to the blocks it reaches, and then the pointers in those blocks. The diagram shows the three groups that this walk finds.

```text
roots                 heap

stack   main   -----> Session ----> Buffer     live: the program uses it
global  cache  -----> Entry   ----> Report     reachable, never used again
                      Block A ----> Block B    unreachable, never freed
```

A collector frees Block A and Block B, because no path from a root reaches them. In C, nothing frees them, so they leak. The middle row leaks in every language. No tool can tell that the program will never read `Report` again, because that depends on what the program does in the future.

Valgrind Memcheck uses four names for these blocks. Block A is "definitely lost": no pointer to it remains. Block B is "indirectly lost": only a lost block points to it. A block that only an interior pointer reaches is "possibly lost". The middle row is "still reachable". By default, Memcheck lists neither indirectly lost nor still reachable blocks one by one. A fix for the definitely lost block also removes the indirect leak.[^3] LeakSanitizer reports Block A as a direct leak and Block B as an indirect leak.[^7]

LeakSanitizer does its work when the process exits. It stops the process and scans the roots for words that look like pointers into the heap. A pointer into the middle of a block counts.[^7]

```pseudo
procedure check_leaks_at_exit()
    stop all threads
    marked <- empty set
    for each word w in the globals, stacks, registers and thread-local storage
        mark(the heap block that w points into)
    for each allocated block b
        if b is not in marked
            report b with the stack trace of its allocation

procedure mark(b)
    if b is a block and b is not in marked
        add b to marked
        for each word w in b
            mark(the heap block that w points into)
```

The report gives the stack trace of each leaked allocation. It does not show where the program lost the last pointer. Memcheck has the same limit.[^3]

A collected language needs a different method. The leaked objects are reachable, so no scan at exit can find them. Instead, take two heap snapshots, one before and one after a repeated action, and compare them. Object counts that grow show what leaks. The path from a root to one of those objects shows what keeps it alive. Chrome DevTools calls this path the retainers, and the .NET command `gcroot` prints it.[^27][^30]

## In languages

| Language | Verdict | How leaks happen and how to find them |
|---|---|---|
| Python | native | reference counting and a cycle collector free unreachable objects; reachable caches leak; `tracemalloc` compares snapshots[^14] |
| JavaScript | native | the engine collects unreachable objects; listeners and closures that stay registered leak; heap snapshots in DevTools[^27] |
| TypeScript | native | the collector of the JavaScript engine that runs the output; the same tools apply |
| Java | native | the collector frees objects that no live thread can reach; static maps leak; heap dumps and JFR[^17][^18] |
| C# | native | the .NET collector; an event keeps each subscriber alive; `dotnet-gcdump` and `dotnet-dump`[^28][^29][^30] |
| Go | native | the collector frees memory, but a blocked goroutine keeps its stack; `runtime.NumGoroutine` and pprof profiles[^32][^34] |
| Kotlin | native | the JVM collector on Kotlin/JVM, with the same leaks as Java |
| PowerShell | native | the .NET collector, as for C# |
| Rust | native | ownership frees each value at the end of its scope; a leak is safe, so `Box::leak` and `mem::forget` need no `unsafe`[^39][^40] |
| C++ | stdlib | `std::unique_ptr` and other owners free on each exit path; a raw `new` leaks on a skipped `delete`; LeakSanitizer[^13][^6] |
| C | absent | `malloc` and `free` by hand; LeakSanitizer and Valgrind find lost blocks[^6][^3] |
| Bash | n/a | the shell manages its own strings and arrays; a script has no pointers |
| SQL | n/a | the database engine manages its memory; a query has no pointers |
| Regex | n/a | a pattern does not allocate memory itself |
| HTML/CSS | n/a | markup and style do not allocate memory; DOM leaks belong to the JavaScript engine |

**Manual memory.** In C, each `malloc` needs exactly one `free` on each path. Leaks come from a few patterns. An assignment overwrites the last pointer to a block. An early `return` on an error path skips the `free`. A call `p = realloc(p, n)` loses the old block when it fails, because a failed `realloc` returns a null pointer and keeps the old block.[^11] C++ adds exceptions. Stack unwinding destroys the local objects of each frame it leaves, but a raw pointer is not an owner, so its target stays.[^12] `std::unique_ptr` deletes its object when the owner goes out of scope, and that includes unwinding.[^13] See [[resource-acquisition-is-initialization]] for the general rule and [[explicit-memory-management]] for the allocator side.

**Collected languages.** Python, JavaScript, Java, C# and Go free objects that the program cannot reach. What leaks is a reference that the program forgot. It is usually in a global or static collection or in a listener list on a long-lived object. It can also be a captured variable of a closure, or the stack of a thread or goroutine that never ends. The Java specification defines a reachable object as one that any potential continuing computation from any live thread can access.[^17] So a thread that never ends keeps alive all that its stack refers to. A Java leak ends in an `OutOfMemoryError` with the detail "Java heap space".[^18]

**Ownership.** Rust frees each value when its owner goes out of scope. The language does not promise that destructors run. A cycle of `Rc` values, a call to `mem::forget`, or `process::exit` skips them, and none of these is `unsafe`.[^39][^41] `Box::leak` turns a box into a reference that lives for the rest of the program. It suits data that the program needs until it exits.[^40]

**Tools.** In C and C++, build tests with `-fsanitize=address`. AddressSanitizer includes LeakSanitizer and enables it by default on Linux; on macOS, set `ASAN_OPTIONS=detect_leaks=1`.[^8] `-fsanitize=leak` gives LeakSanitizer alone, and a suppression file can hide known leaks in third-party code.[^6] Valgrind needs no rebuild: `valgrind --leak-check=full ./prog` gives details for each lost block.[^3][^4] In Python, `tracemalloc` takes snapshots and `Snapshot.compare_to` gives the difference by file or line.[^14] In Go, the heap profile of `runtime/pprof` shows the allocation sites of live objects as of the last collection.[^34] In Java, `jcmd <pid> GC.heap_dump` writes a heap dump. The Old Object Sample event of Flight Recorder records the path to a GC root for sampled objects.[^18] In Node.js, `--heapsnapshot-signal` and `v8.writeHeapSnapshot()` write snapshots for Chrome DevTools.[^25][^26] In .NET, `dotnet-gcdump` collects a heap graph and `dotnet-dump` with the SOS commands `dumpheap -stat` and `gcroot` analyzes a full dump.[^29][^30]

## Variations and related ideas

- **Weak references and weak maps.** A weak reference lets a program find an object without keeping it alive. A weak map holds its keys weakly, so an entry goes when the object that it is stored under goes. JavaScript has `WeakMap` and `WeakRef`, Python has `WeakKeyDictionary` and `WeakValueDictionary`, Java has `WeakHashMap`, Go has `weak.Pointer`, and .NET has `ConditionalWeakTable`.[^21][^22][^16][^19][^35][^31] The ECMAScript specification says that `WeakMap` can associate state with an object without leaking memory.[^21]
- **The lapsed listener.** A subject in the [[observer-pattern]] keeps a list of its observers. An observer that never unsubscribes stays alive as long as the subject does. The DOM `addEventListener` takes an `AbortSignal`, and aborting it removes the listener.[^23] A C# event keeps each subscriber until it unsubscribes with `-=`.[^28] Node.js prints a warning about a possible memory leak when an emitter gets more than 10 listeners for one event.[^24]
- **Captured variables.** A [[closures|closure]] keeps each variable that it captures. A callback that stays registered can keep a large object alive, even when it reads only one field of it.
- **Goroutine and thread leaks.** A goroutine that blocks forever keeps its stack and the objects that the stack refers to. `runtime.NumGoroutine` returns the number of goroutines that exist.[^32] A `context` cancel function ends the work of the goroutines that use the context. Code that never calls it leaks the context until its parent ends, and `go vet` checks that each path calls it.[^33] The same applies to [[threads]] in other languages.
- **Timers.** A timer that is still scheduled is reachable from the runtime, and so is its callback. The Go documentation of `time.NewTicker` says the collector can recover a ticker that no code refers to, even when nobody stopped it.[^36]
- **Reference cycles.** [[reference-counting]] cannot free a cycle of objects that refer to each other. Python has a cycle collector for this; Rust and C++ need a weak pointer on one side. [[garbage-collection]] has examples.
- **Resource leaks.** A file, socket or lock that the program never releases is a leak of a different resource. CWE-401 is a child of CWE-772, Missing Release of Resource after Effective Lifetime, and of CWE-404, Improper Resource Shutdown or Release.[^1]
- **The opposite error.** A leak frees memory too late or never. A [[use-after-free]] frees it too early. A fix for one can cause the other, so each heap object needs one clear owner. See [[stack-and-heap]] for where objects live.

## Pitfalls

### An early return skips the free

Each exit path of a function must free what the function allocated. Here, the error path returns before `free`.

```c fails expect=sanitizer:leak timeout=30 id=early-return
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns 0 and stores the port in *port when text is a valid port number.
int parse_port(const char *text, int *port) {
    char *copy = malloc(strlen(text) + 1);
    if (copy == NULL) {
        return -1;
    }
    strcpy(copy, text);
    char *end = NULL;
    long value = strtol(copy, &end, 10);
    if (*end != '\0' || value <= 0 || value > 65535) {
        return -1; // copy leaks on this path
    }
    free(copy);
    *port = (int)value;
    return 0;
}

int main(void) {
    int port = 0;
    printf("8080: %d\n", parse_port("8080", &port));
    printf("80x: %d\n", parse_port("80x", &port));
}
```

LeakSanitizer reports the copy from the second call. The fix gives the function one exit, and the `free` comes before it.

```c run id=one-exit
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns 0 and stores the port in *port when text is a valid port number.
int parse_port(const char *text, int *port) {
    char *copy = malloc(strlen(text) + 1);
    if (copy == NULL) {
        return -1;
    }
    strcpy(copy, text);
    char *end = NULL;
    long value = strtol(copy, &end, 10);
    int status = -1;
    if (*end == '\0' && value > 0 && value <= 65535) {
        *port = (int)value;
        status = 0;
    }
    free(copy);
    return status;
}

int main(void) {
    int port = 0;
    printf("8080: %d\n", parse_port("8080", &port));
    printf("80x: %d\n", parse_port("80x", &port));
}
```

```output
8080: 0
80x: -1
```

In C++, make the owner a local object, so each exit path frees it. The C++ example shows the same leak on a path that throws.

### A cache without a limit

`functools.cache` saves every result it computes. It is the same as `lru_cache(maxsize=None)`, and that cache can grow without bound.[^15]

```python wrong id=unbounded
import functools


@functools.cache
def render(page_id):
    return f"<html>page {page_id}</html>"


for page_id in range(10_000):
    render(page_id)
print("cached results:", render.cache_info().currsize)
```

```output
cached results: 10000
```

A cache in a long-running process needs a limit. `lru_cache(maxsize=256)` keeps the 256 results that the program used last.[^15]

```python run id=bounded
import functools


@functools.lru_cache(maxsize=256)
def render(page_id):
    return f"<html>page {page_id}</html>"


for page_id in range(10_000):
    render(page_id)
print("cached results:", render.cache_info().currsize)
```

```output
cached results: 256
```

The Java example does the same with a `LinkedHashMap` that drops its eldest entry.[^20]

### A listener that nobody removes

A publisher keeps a reference to each subscriber. When a subscriber closes but does not unsubscribe, the publisher keeps it alive.

```python wrong id=lapsed
import weakref


class Publisher:
    def __init__(self):
        self.listeners = []

    def subscribe(self, listener):
        self.listeners.append(listener)


class Panel:
    def __init__(self, publisher):
        self.pixels = bytearray(100_000)
        publisher.subscribe(self.refresh)

    def refresh(self):
        pass

    def close(self):
        self.pixels = None


feed = Publisher()
panel = Panel(feed)
probe = weakref.ref(panel)
panel.close()
del panel
print("panel alive after close:", probe() is not None)
```

```output
panel alive after close: True
```

The bound method `self.refresh` refers to the panel, so the list keeps the panel. The fix removes the listener in `close`. `weakref.WeakMethod` is another fix: it refers to a bound method without keeping its object alive.[^16]

```python run id=unsubscribe
import weakref


class Publisher:
    def __init__(self):
        self.listeners = []

    def subscribe(self, listener):
        self.listeners.append(listener)

    def unsubscribe(self, listener):
        self.listeners.remove(listener)


class Panel:
    def __init__(self, publisher):
        self.publisher = publisher
        self.pixels = bytearray(100_000)
        publisher.subscribe(self.refresh)

    def refresh(self):
        pass

    def close(self):
        self.publisher.unsubscribe(self.refresh)
        self.pixels = None


feed = Publisher()
panel = Panel(feed)
probe = weakref.ref(panel)
panel.close()
del panel
print("panel alive after close:", probe() is not None)
print("listeners left:", len(feed.listeners))
```

```output
panel alive after close: False
listeners left: 0
```

The JavaScript, TypeScript and C# examples show the same leak with `EventTarget`, a typed emitter and a C# event.

### A closure keeps a large object

A closure keeps the whole object that it captures, not only the fields that it reads.

```python wrong id=captured
import weakref


class Upload:
    def __init__(self, name):
        self.name = name
        self.data = bytearray(1_000_000)


def on_done(upload):
    return lambda: f"done: {upload.name}"


upload = Upload("a.png")
probe = weakref.ref(upload)
callback = on_done(upload)
del upload
print(callback(), "| upload alive:", probe() is not None)
```

```output
done: a.png | upload alive: True
```

Copy the field that the callback needs into a local variable, and let the closure capture only that.

```python run id=copy-field
import weakref


class Upload:
    def __init__(self, name):
        self.name = name
        self.data = bytearray(1_000_000)


def on_done(upload):
    name = upload.name
    return lambda: f"done: {name}"


upload = Upload("a.png")
probe = weakref.ref(upload)
callback = on_done(upload)
del upload
print(callback(), "| upload alive:", probe() is not None)
```

```output
done: a.png | upload alive: False
```

### A goroutine that blocks forever

A goroutine that sends on a channel that nobody reads never ends. The collector does not stop it, and its stack stays in memory. The Go example starts ten workers, reads one result and leaks nine goroutines. The fix passes a `context` and calls its cancel function on return.[^33] A goroutine profile from `runtime/pprof` groups the goroutines by stack, so a stack whose count grows points to the leak.[^34] The Go runtime also has a `goroutineleak` profile. It uses the collector to find goroutines that wait on a channel or lock that no runnable goroutine can reach.[^37][^38] It can miss a leak when a global variable reaches the channel.[^38]

### A weak map whose values refer to their keys

A weak map holds its keys weakly but its values strongly. If a value refers to the object that it is stored under, the map keeps that object alive through the value. Then the entry never goes. The Java documentation of `WeakHashMap` warns about this.[^19]

```python wrong id=value-holds-key
import weakref


class Session:
    pass


class Info:
    def __init__(self, session):
        self.session = session


info = weakref.WeakKeyDictionary()
session = Session()
info[session] = Info(session)
del session
print("entries after del:", len(info))
```

```output
entries after del: 1
```

Keep the values free of references to their keys. If a value needs the object that it belongs to, give it a weak reference to that object.

```python run id=value-weak
import weakref


class Session:
    pass


class Info:
    def __init__(self, session):
        self.session = weakref.ref(session)


info = weakref.WeakKeyDictionary()
session = Session()
info[session] = Info(session)
del session
print("entries after del:", len(info))
```

```output
entries after del: 0
```

## Cost

- **LeakSanitizer.** It adds almost no overhead until the process ends. Then it runs one extra leak detection phase.[^6] With AddressSanitizer, the typical slowdown is 2x.[^8]
- **Valgrind Memcheck.** The program runs much slower under Memcheck, for example 20 to 30 times, and it uses a lot more memory.[^4] It needs no rebuild, because it instruments the machine code at run time.[^5]
- **Purify.** Its authors measured a slowdown of less than a factor of three for programs with its checks.[^2]
- **tracemalloc.** Each traced frame costs memory and CPU time, so `tracemalloc.start(nframe)` with a larger `nframe` costs more. `tracemalloc.get_tracemalloc_memory()` reports the memory that the tracer itself uses.[^14]
- **Heap snapshots in Node.js.** `v8.writeHeapSnapshot()` needs about twice the size of the heap. It is synchronous, so it blocks the event loop until the file is complete.[^26]
- **.NET heap dumps.** `dotnet-gcdump` starts a full generation 2 collection, which can suspend the runtime for a long time in a large heap.[^29]
- **The leak itself.** A leak in a collected language also makes the collector work harder. The JVM reports "GC overhead limit exceeded" after five collections in a row that use over 98% of the time. Each of them recovers less than 2% of the heap. When the heap is full, the error is "Java heap space".[^18]

## History

- **1992.** Reed Hastings and Bob Joyce presented Purify at the Winter USENIX Conference. It inserted checks into object code and found leaked blocks with "a novel adaptation of garbage collection techniques".[^2]
- **2002.** Valgrind had its first release. Nicholas Nethercote and Julian Seward described its design at PLDI in 2007.[^5]
- **2014.** Clang 3.4, released on 2 January 2014, added LeakSanitizer as an experimental leak detector to use with AddressSanitizer.[^9][^10]
- **2024.** Go 1.23 let the collector recover tickers and timers that no code refers to, even when nobody stopped them. Earlier, such a ticker stayed in memory.[^36]
- **2026.** Go 1.26 added an experimental `goroutineleak` profile, enabled with `GOEXPERIMENT=goroutineleakprofile`. The notes credit Vlad Saioc of Uber for the work.[^37] Go 1.27 made the profile generally available.[^38]

## References
