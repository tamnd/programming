---
tag: NEW-data-races
kind: concept
slug: data-races
name: Data race
aliases: [race on memory, unsynchronized access]
area: concurrency-and-parallelism/races-and-deadlocks
level: core
summary: >-
  A data race is two accesses to one memory location from different threads, at least one a write, with no synchronization that orders them. In C and C++ it is undefined behaviour.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1992
  people: [Robert H. B. Netzer, Barton P. Miller]
  note: Netzer and Miller published "What are race conditions?", which separates a general race, where the order of events is not fixed, from a data race, where accesses that must be atomic are not.
see_also: [threads, mutual-exclusion, deadlock, channels, async-await, rust/ownership]
sources:
  - url: https://timsong-cpp.github.io/cppwp/n4950/intro.races
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[intro.races], paragraph 2 (conflict), paragraph 21 and note 21 (data race, undefined behaviour, sequential consistency), paragraph 23 note 22 and paragraph 24 note 23 (compiler transformations)"
  - url: https://port70.net/~nsz/c/c11/n1570.html#5.1.2.4p25
    title: N1570, Committee Draft of ISO/IEC 9899:2011 (C11)
    locator: 5.1.2.4 Multi-threaded executions and data races, paragraphs 4, 25 and 26 (NOTE 12)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-17.html#jls-17.4.5
    title: The Java Language Specification, Java SE 25 Edition
    locator: 17.4.1 Shared Variables; 17.4.3 Programs and Program Order; 17.4.5 Happens-before Order
  - url: https://go.dev/ref/mem#overview
    title: The Go Memory Model
    locator: Overview (definition of a data race, DRF-SC); Advice
  - url: https://go.dev/ref/mem#restrictions
    title: The Go Memory Model
    locator: Implementation Restrictions for Programs Containing Data Races
  - url: https://docs.python.org/3.14/faq/library.html#what-kinds-of-global-value-mutation-are-thread-safe
    title: Python Library and Extension FAQ, Python 3.14
    locator: What kinds of global value mutation are thread-safe?
  - url: https://doc.rust-lang.org/nomicon/races.html
    title: The Rustonomicon
    locator: Data Races and Race Conditions
  - url: https://doc.rust-lang.org/nomicon/send-and-sync.html
    title: The Rustonomicon
    locator: Send and Sync
  - url: https://doc.rust-lang.org/std/thread/fn.spawn.html
    title: Rust standard library, std::thread::spawn
    locator: the bounds F, Send and 'static, and their explanation
  - url: https://doc.rust-lang.org/std/thread/fn.scope.html
    title: Rust standard library, std::thread::scope
    locator: the description (borrowing non-'static data; automatic join)
  - url: https://go.dev/doc/articles/race_detector
    title: Data Race Detector, Go documentation
    locator: How To Use; Report Format; Options; Requirements; Runtime Overheads
  - url: https://clang.llvm.org/docs/ThreadSanitizer.html
    title: Clang documentation, ThreadSanitizer
    locator: Introduction; Usage; Limitations
  - url: https://tc39.es/ecma262/2026/multipage/memory-model.html#sec-data-races
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 29.8 Races; 29.9 Data Races; 29.10 Data Race Freedom; 29.11 Shared Memory Guidelines, Note 1
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/variables#96-atomicity-of-variable-references
    title: C# language specification, Variables
    locator: 9.6 Atomicity of variable references
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-17.html#jls-17.7
    title: The Java Language Specification, Java SE 25 Edition
    locator: 17.7 Non-Atomic Treatment of double and long
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.5.2.4p2
    title: N1570, Committee Draft of ISO/IEC 9899:2011 (C11)
    locator: 6.5.2.4 Postfix increment and decrement operators, paragraph 2; 7.17.7.5 The atomic_fetch and modify generic functions
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/concurrent/atomic/package-summary.html
    title: Java SE 25 API, Package java.util.concurrent.atomic
    locator: Description (first sentence; compareAndSet as a read-modify-write operation)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.interlocked.increment
    title: .NET API reference, Interlocked.Increment Method
    locator: Definition; Increment(Int32), Remarks
  - url: https://doc.rust-lang.org/std/sync/atomic/index.html
    title: Rust standard library, Module std::sync::atomic
    locator: the module description; Memory model for atomic accesses
  - url: https://docs.python.org/3.14/howto/free-threading-python.html#thread-safety
    title: Python HOWTO, Python support for free threading, Python 3.14
    locator: Thread safety
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/foreach-object?view=powershell-7.5
    title: PowerShell reference, ForEach-Object
    locator: Example 14 (thread safe variable references); Notes, the Important box on the Using scope modifier
  - paper: doi:10.1145/359545.359563
    title: Leslie Lamport, Time, clocks, and the ordering of events in a distributed system, Communications of the ACM 21(7), 1978
    locator: the definition of the "happened before" relation
  - paper: doi:10.1145/325164.325100
    title: Sarita V. Adve and Mark D. Hill, Weak ordering, a new definition, ISCA 1990
    locator: the definition of weak ordering with respect to a synchronization model
  - paper: doi:10.1145/130616.130623
    title: Robert H. B. Netzer and Barton P. Miller, What are race conditions? Some issues and formalizations, ACM Letters on Programming Languages and Systems 1(1), 1992
    locator: the distinction between general races and data races
  - paper: doi:10.1145/265924.265927
    title: Stefan Savage, Michael Burrows, Greg Nelson, Patrick Sobalvarro and Thomas Anderson, Eraser, a dynamic data race detector for multithreaded programs, ACM Transactions on Computer Systems 15(4), 1997
    locator: the lockset algorithm
  - paper: doi:10.1145/1040305.1040336
    title: Jeremy Manson, William Pugh and Sarita V. Adve, The Java memory model, POPL 2005
    locator: the guarantee for correctly synchronized programs and the semantics of programs with data races
  - paper: doi:10.1145/1375581.1375591
    title: Hans-J. Boehm and Sarita V. Adve, Foundations of the C++ concurrency memory model, PLDI 2008
    locator: the model where a data race gives undefined semantics
  - paper: doi:10.1145/1791194.1791203
    title: Konstantin Serebryany and Timur Iskhodzhanov, ThreadSanitizer, data race detection in practice, WBIA 2009
    locator: the description of the detector
  - url: https://go.dev/blog/race-detector
    title: Dmitry Vyukov and Andrew Gerrand, Introducing the Go Race Detector, The Go Blog, 26 June 2013
    locator: the introduction (based on the ThreadSanitizer runtime library, integrated with Go in September 2012)
  - url: https://blog.rust-lang.org/2015/04/10/Fearless-Concurrency/
    title: Aaron Turon, Fearless Concurrency with Rust, Rust Blog, 10 April 2015
    locator: the introduction and the section on Send
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.10.8.3
    title: N1570, Committee Draft of ISO/IEC 9899:2011 (C11)
    locator: 6.10.8.3 Conditional feature macros (__STDC_NO_ATOMICS__, __STDC_NO_THREADS__)
  - url: https://timsong-cpp.github.io/cppwp/n4950/atomics.order
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[atomics.order], paragraph 1.1 and note 2 (memory_order::relaxed), paragraph 10 (read-modify-write operations)"
---

## Idea

Two threads share a variable. Each thread adds 1 to it. After both threads finish, the variable can hold 1, not 2. The second write replaced the first write, so one update is lost.

The memory models of C, C++, Java and Go give this hazard the name data race. Their definitions agree. Two accesses conflict when they use the same memory location and at least one of them is a write.[^1][^2][^3][^4] A data race is two conflicting accesses in different threads that no synchronization orders. In C, C++ and Go, a race also needs at least one access that is not atomic.[^1][^2][^4]

The program below forces the bad order. Each thread reads `count`, then waits at a barrier until the other thread has also read it. Then each thread writes its value plus 1. A barrier makes the order the same on each run, so the output is always 1.

```python wrong id=lost-update
import threading

count = 0
barrier = threading.Barrier(2)


def add_one():
    global count
    value = count
    barrier.wait()
    count = value + 1


workers = [threading.Thread(target=add_one) for _ in range(2)]
for worker in workers:
    worker.start()
for worker in workers:
    worker.join()
print(count)
```

```output for=lost-update
1
```

Without the barrier, the program usually prints 2. The wrong order then occurs only sometimes, under load or on another machine. This makes data races hard to find with tests.

A data race and a race condition are different things. A race condition is a bug where the result depends on the order or timing of events. A data race is a fact about memory accesses that a language defines. A program can have a race condition and no data race, when every access is atomic but the logic between the accesses is wrong. A program can also have a data race that by chance gives the correct result.[^7] The Pitfalls section shows a race condition with no data race. This page is about data races and the tools that find and prevent them. Locks are the topic of [[mutual-exclusion]], and the threads themselves are the topic of [[threads]].

## How it works

### The increment has three steps

The statement `count += 1` looks like one action. The machine does it in three steps: load the value from memory, add 1, store the result. Another thread can run between any two steps. The Python disassembler shows the three steps for a global variable.

```python run id=steps
import dis

count = 0


def add_one():
    global count
    count += 1


for instruction in dis.get_instructions(add_one):
    if instruction.opname in ("LOAD_GLOBAL", "BINARY_OP", "STORE_GLOBAL"):
        print(instruction.opname, instruction.argrepr)
```

```output for=steps
LOAD_GLOBAL count
BINARY_OP +=
STORE_GLOBAL count
```

The interpreter can switch threads between these instructions. The Python FAQ gives `i = i+1` and `D[x] = D[x] + 1` as operations that are not atomic, for this reason.[^6] The same split occurs in compiled code. The compiler turns `count++` into a load, an add and a store, unless the variable is atomic.

### Happens-before

The definitions use an order between events called happens-before. Inside one thread, an earlier statement happens before a later one. Between threads, synchronization makes the order. For example, the unlock of a lock happens before a later lock of the same lock. The start and the join of a thread also order events. If neither of two conflicting accesses happens before the other, they race.[^1][^3][^4] Lamport defined the "happened before" relation for events in a distributed system, and the memory models use the same idea.[^22]

The definition does not mention time. Two accesses one second apart still race when nothing orders them. For this reason a race detector can report a race that did not cause a wrong result in the run.

### Sequential consistency for programs with no data race

Each language makes a contract with the programmer. If a program has no data race, it behaves as if the threads take turns in one global order. Each read then sees the last write in that order. This is sequential consistency. The C, C++, Java, Go and JavaScript memory models all give this guarantee to programs with no data race.[^1][^2][^3][^4][^13] The Go memory model calls it DRF-SC.[^4]

The contract lets the compiler and the processor reorder memory accesses inside one thread. A reordering is invisible to a program with no data race. A program with a data race can see it. For example, a thread can see a flag set before it sees the data that the flag protects.

### What a data race does in each language

The languages differ in what happens when the contract is broken.

| Language | Result of a data race |
|---|---|
| C, C++ | undefined behaviour; the standard puts no limit on the result[^1][^2] |
| Java | defined but weak; a read can see the value of any racing write, and the program stays type safe[^3] |
| Go | limited for single words; a race on a multiword value can corrupt memory[^5] |
| JavaScript, TypeScript | defined for shared memory; racing accesses of the same size do not tear[^13] |
| C# | the specification lists which reads and writes are atomic; an increment is not among them[^14] |
| Python | the interpreter stays consistent, but compound updates such as `+=` are not atomic[^6] |
| Rust | safe code cannot have a data race; the compiler rejects it[^7] |

In C and C++, "undefined" is meant literally. The standard says that any data race results in undefined behaviour.[^1][^2] So a compiler can optimize as if no other thread writes a plain variable between two synchronization points. For example, it can load the variable once and reuse the value. The C++ standard also limits the other direction. It forbids most transformations that add a store to a location the program does not write, because that store could add a data race.[^1]

Java must keep the JVM safe even for programs with bugs. The JLS states that "a data race cannot cause incorrect behavior such as returning the wrong length for an array."[^3] A racing read still sees some value that a thread wrote to that variable. Writes to a `long` or a `double` that is not `volatile` are an exception. The JVM can treat each such write as two 32-bit writes, so a racing read can see half of each.[^15]

The Go memory model gives a middle ground. An implementation can report a data race and stop the program. A racing read of a value no larger than a machine word sees a value that some write stored. A race on an interface, a map, a slice or a string can lead to arbitrary memory corruption.[^5] The advice in the document is short: "Don't be clever."[^4]

ECMAScript defines data races only for shared memory, which is a `SharedArrayBuffer` that two agents (for example, a main thread and a worker) can see. Two events are in a race when happens-before does not order them, their memory ranges overlap, and both write or one reads from the other. The race is a data race if either access is not sequentially consistent, or if the ranges only partly overlap.[^13] The guidelines in the specification say that a racy program can still behave predictably. This holds if no atomic operation takes part in the race and all racing accesses have the same size. Such accesses do not tear, but their order is still not fixed.[^13]

Python has a global interpreter lock (GIL) in its default build. The GIL lets only one thread run Python bytecode at a time, and it switches threads only between bytecode instructions.[^6] So a single instruction, such as an append to a list, is atomic. The `+=` statement is three instructions, so the GIL does not make it atomic. The FAQ ends its answer with "When in doubt, use a mutex!"[^6] The free-threaded build has no GIL. Built-in types such as `list` and `dict` use internal locks, and the documentation still recommends `threading.Lock` for shared state.[^20]

### How Rust rejects a data race

Safe Rust guarantees that a program has no data race.[^7] The rules of [[rust/ownership]] do most of the work. A value can have many shared references or one mutable reference, never both. Two threads that both write a variable would each need a mutable reference, and the borrow checker allows only one.

```rust fails expect=compile id=two-writers
use std::thread;

fn main() {
    let mut count = 0;
    thread::scope(|s| {
        s.spawn(|| count += 1);
        s.spawn(|| count += 1);
    });
    println!("{count}");
}
```

The compiler stops with error E0499, "cannot borrow `count` as mutable more than once at a time". The function `thread::scope` lets threads borrow local variables, because it joins all of its threads before it returns.[^10]

Interior mutability changes a value through a shared reference, so the borrow rules alone do not stop a race. Two marker traits close this gap. A type is `Send` if a value of it can move to another thread. A type is `Sync` if threads can share a reference to it, and `T` is `Sync` exactly when `&T` is `Send`.[^8] `Cell` and `RefCell` are not `Sync`, and `Rc` is neither `Send` nor `Sync`.[^8] The function `thread::spawn` requires its closure and its result to be `Send`.[^9] The compiler infers both traits from the fields of a type. The Rust example file shares a `Cell<i32>` between two threads, and the compiler rejects it with E0277, "`Cell<i32>` cannot be shared between threads safely".

The Rustonomicon states the limit of this guarantee: "Rust does not prevent general race conditions."[^7] The check-then-act pitfall below compiles in Rust as well.

### Atomic operations

An atomic operation is a read, a write or a read-modify-write that no other thread can see half done. A data race needs at least one access that is not atomic, so atomics remove the race. Each tier 1 language with shared memory has them.

- C has the `_Atomic` qualifier and `<stdatomic.h>`. Postfix `++` on an atomic object is one read-modify-write with sequentially consistent order.[^16]
- C++ has `std::atomic<T>`, Rust has `std::sync::atomic`, and Go has `sync/atomic`. Rust follows the C++ rules for atomics.[^19]
- Java has `java.util.concurrent.atomic`, "a small toolkit of classes that support lock-free thread-safe programming on single variables."[^17]
- C# has the `Interlocked` class. `Interlocked.Increment` "increments a specified variable and stores the result, as an atomic operation."[^18]
- JavaScript has the `Atomics` object, which works on typed arrays over a `SharedArrayBuffer`.[^13]

Each example file shows the fix for its language. Two threads add 1 to an atomic counter 100000 times, and the program prints 200000 on each run.

An atomic variable protects one location. When an update changes two variables together, or reads one and writes another, atomics on each variable are not enough. The program then needs a lock, which [[mutual-exclusion]] describes, or it must give each piece of data one owner and pass messages through [[channels]].

### Race detectors

A race detector watches a running program. It records each memory access and each synchronization, and it reports two conflicting accesses that happens-before does not order. ThreadSanitizer (TSan) does this for C and C++. The program is compiled with `-fsanitize=thread`, and the documentation recommends `-O1` and `-g` as well.[^12] The report starts with `WARNING: ThreadSanitizer: data race` and shows the stack of each access.

Go builds the same runtime library into its tool chain.[^29] The flag `-race` works with `go run`, `go build` and `go test`. The report starts with `WARNING: DATA RACE`. By default the program continues after a report and exits with status 66.[^11] The detector needs cgo, and on systems other than Darwin it also needs a C compiler.[^11]

The C and Go example files use these tools. Each racy program there is a `fails` block that expects the report of the detector. A racy program never has an output block, because its output is not defined.

A detector finds only races that occur in the run it watches. The Go documentation says that the race detector "only finds races that happen at runtime".[^11] Code that a test does not reach is not checked. A clean run does not prove that the program has no data race.

## In languages

The verdict says how the language gives race-free access to shared memory: a language construct (`native`) or a type in the standard library (`stdlib`). Each example file shows a counter that two threads increment 100000 times, with an atomic or a lock.

| Language | Verdict | Form |
|---|---|---|
| C | native | `_Atomic` and `<stdatomic.h>`; TSan for detection |
| C++ | stdlib | `std::atomic<T>`; TSan for detection |
| Java | stdlib | `java.util.concurrent.atomic`; `volatile` fields |
| Kotlin | stdlib | the JDK atomic classes on the JVM |
| C# | stdlib | `System.Threading.Interlocked` |
| PowerShell | stdlib | the .NET concurrent collections |
| Go | stdlib | `sync/atomic`; `go run -race` for detection |
| Rust | native | `Send` and `Sync` checked by the compiler; `std::sync::atomic` |
| Python | stdlib | `threading.Lock`; no atomic integer type |
| JavaScript | stdlib | `Atomics` on a `SharedArrayBuffer` |
| TypeScript | stdlib | as JavaScript, with types |
| Bash | n/a | background jobs are processes that share no variables |
| SQL | n/a | the engine controls concurrent access to rows |
| Regex | n/a | a pattern has no threads |
| HTML/CSS | n/a | markup and style rules have no threads |

C11 makes threads and atomics optional. An implementation that lacks them defines `__STDC_NO_THREADS__` or `__STDC_NO_ATOMICS__`.[^31] The C library of the test host has no `<threads.h>`, so the C examples use POSIX threads.

PowerShell runs each script block of `ForEach-Object -Parallel` on its own thread. The documentation says that objects passed with `$using:` must be thread safe, for example the types of `System.Collections.Concurrent`.[^21] The PowerShell example file shows a trap: `[ref]` on an array element refers to a copy, so `Interlocked.Increment` does not change the array.

## Variations and related ideas

- **Race condition.** A bug where the result depends on the order of events. It can occur with no data race and with one thread (see Pitfalls). Netzer and Miller call this a general race.[^24]
- **Benign data race.** A race that the programmer believes is harmless, such as a statistics counter that may lose updates. C and C++ have no such category, because every data race is undefined behaviour.[^1]
- **Torn read or write.** A read that sees part of one write and part of another. Java allows it for plain `long` and `double`.[^15] Go allows memory corruption for multiword values.[^5] C# guarantees atomic access only for the types listed in its specification.[^14]
- **Memory ordering.** Atomics in C, C++ and Rust take an order argument. `memory_order::relaxed` makes the access atomic but orders no other memory.[^32][^19] The Rust example uses `Ordering::Relaxed`, because only the final total matters and the join of the threads orders the read.
- **Lock-free data structures.** Structures built from compare-and-swap and other atomics instead of locks. They have no data race, but they can still have race conditions.
- **Single-threaded concurrency.** Code in an event loop has no data race, because only one task runs at a time. It can still have race conditions at each `await` (see [[async-await]]).

## Pitfalls

### Check then act with atomic operations

Each access below is atomic, so the program has no data race. It still has a race condition. Two threads each check the balance and then withdraw 80. The barrier puts both checks before both withdrawals, so the balance goes below zero.

```java wrong id=check-then-act
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    static final AtomicInteger balance = new AtomicInteger(100);
    static final CyclicBarrier bothChecked = new CyclicBarrier(2);

    static void withdraw() {
        try {
            if (balance.get() >= 80) {
                bothChecked.await();
                balance.addAndGet(-80);
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public static void main(String[] args) throws InterruptedException {
        Thread first = new Thread(Main::withdraw);
        Thread second = new Thread(Main::withdraw);
        first.start();
        second.start();
        first.join();
        second.join();
        System.out.println(balance.get());
    }
}
```

```output for=check-then-act
-60
```

The check and the action must be one atomic step. A compare-and-set loop does this. `compareAndSet(current, current - 80)` changes the balance only if it still holds the value that the thread checked. If it fails, the thread reads again and checks again.[^17] In the fixed program, both threads still pass the barrier after their first check. One `compareAndSet` succeeds. The other fails, reads 20 and stops.

```java run id=compare-and-set
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    static final AtomicInteger balance = new AtomicInteger(100);
    static final CyclicBarrier bothChecked = new CyclicBarrier(2);

    static void withdraw() {
        try {
            boolean waited = false;
            while (true) {
                int current = balance.get();
                if (current < 80) {
                    return;
                }
                if (!waited) {
                    bothChecked.await();
                    waited = true;
                }
                if (balance.compareAndSet(current, current - 80)) {
                    return;
                }
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public static void main(String[] args) throws InterruptedException {
        Thread first = new Thread(Main::withdraw);
        Thread second = new Thread(Main::withdraw);
        first.start();
        second.start();
        first.join();
        second.join();
        System.out.println(balance.get());
    }
}
```

```output for=compare-and-set
20
```

A lock around the check and the action also works, and it is the usual fix when the step touches more than one variable.

### A race condition with one thread

JavaScript runs this code on one thread, so it has no data race. Each `await` lets other tasks run, so the check and the action can still interleave. Both calls check the balance, then both wait, then both subtract.

```javascript wrong id=await-gap
let balance = 100;

function audit() {
  return new Promise((resolve) => setTimeout(resolve, 10));
}

async function withdraw(amount) {
  if (balance >= amount) {
    await audit();
    balance -= amount;
  }
}

async function main() {
  await Promise.all([withdraw(80), withdraw(80)]);
  console.log(balance);
}

main();
```

```output for=await-gap
-60
```

The fix puts the check and the change in the same synchronous part of the function, before the `await`. No other task can run between them.

```javascript run id=no-await-gap
let balance = 100;

function audit() {
  return new Promise((resolve) => setTimeout(resolve, 10));
}

async function withdraw(amount) {
  if (balance < amount) {
    return false;
  }
  balance -= amount;
  await audit();
  return true;
}

async function main() {
  const results = await Promise.all([withdraw(80), withdraw(80)]);
  console.log(results, balance);
}

main();
```

```output for=no-await-gap
[ true, false ] 20
```

[[async-await]] describes where an async function can pause.

### A plain flag between threads

A thread often sets a flag to tell another thread that data is ready. With a plain `bool`, the write and the read of the flag race, and so do the accesses to the data. The C++ standard makes the program undefined.[^1] A compiler can read the flag once and loop forever, or the reader can see the flag before the data. Here ThreadSanitizer reports the race. The call to `yield` in the loop keeps this build from hanging, but it does not make the program correct.

```cpp fails expect=sanitizer:thread flags="-fsanitize=thread" id=plain-flag
#include <thread>

int main() {
    int data = 0;
    bool ready = false;
    std::thread producer([&] {
        data = 42;
        ready = true;
    });
    while (!ready) {
        std::this_thread::yield();
    }
    int seen = data;
    producer.join();
    return seen == 42 ? 0 : 1;
}
```

The fix makes the flag atomic. A store to a `std::atomic<bool>` and a load that sees it synchronize, so the write of `data` happens before the read of `data`.[^1]

```cpp run id=atomic-flag
#include <atomic>
#include <print>
#include <thread>

int main() {
    int data = 0;
    std::atomic<bool> ready = false;
    std::thread producer([&] {
        data = 42;
        ready = true;
    });
    while (!ready) {
        std::this_thread::yield();
    }
    std::println("{}", data);
    producer.join();
}
```

```output for=atomic-flag
42
```

In Java, a `volatile` field gives the same effect.[^3]

### A passing test proves nothing

A racy program usually gives the correct result. The bad interleaving needs exact timing, which a test machine may never produce. A race detector helps, but it sees only the code paths that the run executes.[^11] Run the tests with the detector on, and keep shared mutable state small, so that a reader can check each access.

## Cost

- **Atomic operations.** This page does not measure the cost of atomics. A relaxed order asks only for atomicity and orders no other memory, so it places the fewest limits on the compiler and the processor.[^32]
- **ThreadSanitizer.** The Clang documentation gives a typical slowdown of 5x to 15x and a memory overhead of 5x to 10x.[^12] It does not support static linking of `libc` or `libstdc++`.[^12]
- **The Go race detector.** The Go documentation gives a memory use of 5 to 10 times that of the normal program. The execution time is 2 to 20 times longer.[^11]
- **Rust.** The `Send` and `Sync` checks run in the compiler and add nothing to the running program.[^8]

## History

- **1978.** Leslie Lamport defined the "happened before" relation on the events of a distributed system.[^22]
- **1990.** Sarita Adve and Mark Hill defined weak ordering as a contract: hardware appears sequentially consistent to a program that obeys a synchronization model.[^23]
- **1992.** Robert Netzer and Barton Miller separated general races from data races.[^24]
- **1997.** Stefan Savage and others described Eraser, a dynamic detector that checks that each shared variable is protected by a consistent set of locks.[^25]
- **2005.** Jeremy Manson, William Pugh and Sarita Adve published the revised Java memory model. It gives a guarantee to programs with no data race and limits to programs with one.[^26]
- **2008.** Hans Boehm and Sarita Adve described the C++ memory model, where a data race gives undefined semantics.[^27]
- **2009.** Konstantin Serebryany and Timur Iskhodzhanov described ThreadSanitizer.[^28]
- **2012 and 2013.** The ThreadSanitizer runtime was integrated with Go, and the Go blog introduced the race detector in June 2013.[^29]
- **2015.** The Rust blog post "Fearless Concurrency with Rust" stated that the compiler prevents all data races in safe code.[^30]

## References
