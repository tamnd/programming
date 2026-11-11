---
tag: AKH190
kind: concept
slug: call-stack-and-heap
name: The call stack and the heap
aliases: [call stack, stack frame, activation record, heap, stack allocation, heap allocation, automatic storage, dynamic storage, stack overflow, escape analysis]
area: foundations/the-stack-and-the-heap
level: core
summary: >-
  The call stack holds a frame for each active call and frees it on return. The heap holds data that outlives its call or grows, until the program or a collector frees it.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [Edsger W. Dijkstra, Friedrich L. Bauer, Klaus Samelson]
  note: Bauer and Samelson described the stack principle in 1957, and Dijkstra showed in 1960 how a stack of activation records supports recursive procedures.
see_also: [recursion, garbage-collection, ownership, pointers-and-references, use-after-free, memory-leaks, reference-counting, async-await]
same_as:
  wikidata: Q759899
  wikipedia: Call_stack
sources:
- url: https://doc.rust-lang.org/book/ch04-01-what-is-ownership.html
  title: What Is Ownership?, The Rust Programming Language
  locator: The Stack and the Heap
- url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-2.html
  title: Chapter 2, The Structure of the Java Virtual Machine, The Java Virtual Machine Specification, Java SE 25 Edition
  locator: 2.5.2 Java Virtual Machine Stacks; 2.5.3 Heap
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Execution_model
  title: JavaScript execution model, MDN Web Docs
  locator: The engine and the host
- url: https://go.dev/doc/faq
  title: Frequently Asked Questions (FAQ), The Go Programming Language
  locator: How do I know whether a variable is allocated on the heap or the stack?
- url: https://learn.microsoft.com/en-us/archive/blogs/ericlippert/the-stack-is-an-implementation-detail-part-one
  title: The Stack Is An Implementation Detail, Part One, Eric Lippert, Fabulous Adventures In Coding
  locator: Whole post
- url: https://docs.python.org/3/library/sys.html#sys.getrecursionlimit
  title: sys, System-specific parameters and functions, The Python Standard Library
  locator: sys.getrecursionlimit
---

## Idea

A running program keeps its data in two main regions of memory. The call stack holds one frame for each function call that has not yet returned, with the call's arguments, its local variables and the place to return to. A call pushes a frame and a return pops it, so the stack grows and shrinks with the nesting of calls, and its memory comes back the moment a function returns. The heap holds data whose size or lifetime does not fit that pattern: objects that outlive the call that made them, buffers that grow, and structures that many parts of a program share. Heap memory lives until the program frees it, or until a garbage collector finds that nothing refers to it.

```python run id=idea
stack = []  # one frame for each call that has not returned
heap = {}  # address -> object, kept until the program frees it


def new(obj):
    addr = 100 + len(heap)
    heap[addr] = obj
    return addr


def show():
    print(" > ".join(f"{f['fn']}{f['locals']}" for f in stack))


def fact(n):
    stack.append({"fn": "fact", "locals": {"n": n}})
    if n <= 1:
        show()  # the deepest point of the recursion
        result = 1
    else:
        result = n * fact(n - 1)
    stack.pop()  # returning frees the frame at once
    return result


def make_list(size):
    stack.append({"fn": "make_list", "locals": {"size": size}})
    p = new(list(range(size)))  # the list goes on the heap
    stack[-1]["locals"]["p"] = p
    show()
    stack.pop()
    return p  # the frame is gone, the list is not


stack.append({"fn": "main", "locals": {}})
print(fact(4))
xs = make_list(3)
stack[-1]["locals"]["xs"] = xs
show()
print(heap)
```

```output for=idea
main{} > fact{'n': 4} > fact{'n': 3} > fact{'n': 2} > fact{'n': 1}
24
main{} > make_list{'size': 3, 'p': 100}
main{'xs': 100}
{100: [0, 1, 2]}
```

The program models the two regions with a list and a dictionary. Each call to `fact` pushes a frame that holds its own `n`, so at the deepest point four frames of `fact` sit on top of `main`, and each return pops one. `make_list` puts a list on the heap and keeps only its address in the frame. When `make_list` returns, its frame is gone, but the list is still on the heap, and `main` reaches it through the address that the call returned.

## How it works

**Frames on a stack.** The Rust book describes the order: "The stack stores values in the order it gets them and removes the values in the opposite order. This is referred to as last in, first out (LIFO)."[^1] Function calls nest in the same order, because a function returns before its caller does. A frame holds the arguments, the local variables, temporary values and the return address, and on most machines one register, the stack pointer, marks the top. The Java Virtual Machine Specification describes the same structure for the JVM: "Each Java Virtual Machine thread has a private Java Virtual Machine stack, created at the same time as the thread. A Java Virtual Machine stack stores frames".[^2]

**Why the stack is fast.** A function allocates its whole frame by moving the stack pointer once, and frees it by moving the pointer back. The Rust book explains: "Pushing to the stack is faster than allocating on the heap because the allocator never has to search for a place to store new data; that location is always at the top of the stack."[^1] The top of the stack is used again and again, so it stays in the processor cache. The price is a rule: the size of a frame is fixed when the function starts, in most languages, and its data dies when the function returns.

**The heap.** Data that must outlive its call, or whose size is known only at run time, goes on the heap. An allocator, such as `malloc` in C, finds a free block of the right size and records which blocks are in use. The JVM specification defines the heap as shared: "The Java Virtual Machine has a heap that is shared among all Java Virtual Machine threads. The heap is the run-time data area from which memory for all class instances and arrays is allocated."[^2] MDN describes the heap of a JavaScript engine as "just a name to denote a large (mostly unstructured) region of memory. It gets populated as objects get created in the program."[^3] The word heap here names a pool of memory, not the heap data structure. See [garbage collection](/garbage-collection/) and [ownership](/ownership/).

**Lifetime is the real difference.** A local variable in a frame dies when its function returns, so a pointer to it that survives the return points to memory that the next call will reuse. In C and C++ this is a dangling pointer, and reading through it is undefined behavior. Rust rejects such a program at compile time, and C# rejects a `Span` that would outlive its `stackalloc` memory. Go removes the question: the compiler moves a variable to the heap when its address escapes the function. The Go FAQ states the rule: "From a correctness standpoint, you don’t need to know. Each variable in Go exists as long as there are references to it. The storage location chosen by the implementation is irrelevant to the semantics of the language."[^4] See [use after free](/use-after-free/).

**An implementation detail in managed languages.** In Python, JavaScript, Java and C#, the language decides where data lives, and the program sees only references and values. JIT compilers place objects that do not escape in registers or in the frame, and C# structs can live on the heap inside other objects. Eric Lippert, who worked on the C# compiler, argued that the place of storage is the wrong thing to teach: "Surely the most relevant fact about value types is not the implementation detail of how they are allocated, but rather the by-design semantic meaning of “value type”, namely that they are always copied “by value”."[^5] See [pointers and references](/pointers-and-references/).

**Stack overflow.** Each thread has a stack of a fixed maximum size, commonly 8 MiB for the main thread on Linux and 1 MiB on Windows, and less for other threads on some systems. Deep or unbounded recursion fills it. Native programs then crash, often with a segmentation fault, while managed runtimes raise an error that the program can catch: `StackOverflowError` in Java, `RangeError` in JavaScript and `RecursionError` in Python. The Python documentation explains its limit on recursion: "This limit prevents infinite recursion from causing an overflow of the C stack and crashing Python."[^6] Go avoids a fixed size: each goroutine starts with a small stack that the runtime copies to a larger block when it fills. See [recursion](/recursion/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | frames managed by CPython; every object on the heap |
| JavaScript | native | call stack of execution contexts; garbage-collected heap |
| TypeScript | native | as JavaScript |
| Java | native | a stack of frames per thread; a shared garbage-collected heap |
| Kotlin | native | as Java on the JVM |
| C# | native | value types and `stackalloc`; garbage-collected heap |
| Go | native | escape analysis; stacks that grow for each goroutine |
| Rust | native | values in frames; `Box`, `Vec` and `String` own heap memory |
| C | native | automatic storage; `malloc` and `free` |
| C++ | native | automatic storage; `new`, containers and smart pointers |
| Bash | native | a scope for each function call; the shell manages memory |
| PowerShell | native | as C# on .NET; `Get-PSCallStack` |
| SQL | n/a | no frames or heap memory in queries |
| Regex | n/a | no calls |
| HTML | n/a | no calls |
| CSS | n/a | no calls |

Every language with function calls has a call stack, and every language that can build data at run time has a heap, so the difference between languages is who decides where data lives. In C, C++ and Rust the programmer decides: a local variable is in the frame, and heap memory comes from an explicit call or from a type that owns it, such as `Box` or `std::vector`. In Go the compiler decides by escape analysis, and in Python, JavaScript and Java the runtime decides, with objects on the heap and the frames holding references to them. C# lets the programmer ask for frame memory with `stackalloc` and choose value types, but keeps the final placement to the runtime.

Each example file shows a value that outlives its frame by moving to the heap, the limit on the depth of the stack, and the form of frame or heap memory that the language exposes. The fails blocks show the two ways a language can stop a bug at the boundary: Python stops recursion that would overflow the stack, and Rust and C# reject a reference to frame memory that would outlive its frame.

## Variations and related ideas

- **Growable and segmented stacks.** Go goroutines start with a small stack and grow it by copying. Early Go and Rust used segmented stacks, linked chunks that caused slow calls at the chunk edges.
- **Frames on the heap.** Generators, coroutines and closures keep their state after a call returns, so their frames, or the captured variables, live on the heap. Scheme continuations and Python generators work in this way. See [iterators](/iterators/) and [async and await](/async-await/).
- **Variable-size frame memory.** C's `alloca` and variable-length arrays and C#'s `stackalloc` make a frame larger at run time, with the risk of overflow for large sizes.
- **Escape analysis.** Go, the HotSpot JVM and the .NET JIT find objects that never leave a function and keep them in the frame or in registers.
- **Tail calls.** A call in tail position can reuse the frame of its caller, so a recursion in tail form runs in constant stack space. Scheme requires this; Python, Java and most C compilers at low optimization do not do it. See [recursion](/recursion/).
- **Arenas and regions.** An arena hands out heap memory by moving a pointer, as a stack does, and frees all of it at once, which suits data with one shared lifetime.
- **Static storage.** Global variables and constants live in a third region for the whole run of the program.
- **Garbage collection.** A collector frees heap objects that the program can no longer reach, starting from the references in the stack frames and globals. See [garbage collection](/garbage-collection/).

## Pitfalls

- **Recursion on deep input.** A recursive function over a long list, a deep tree or nested user input can overflow the stack in production after passing every small test. The fails block in the Python file shows this. Use a loop with an explicit stack for input of unbounded depth.
- **Pointers into a dead frame.** Returning or storing the address of a local variable in C or C++ leaves a dangling pointer, which can seem to work until the next call reuses the memory. Rust rejects the program, as the fails block in the Rust file shows.
- **Large arrays in a frame.** An array of a few MiB in a local variable can overflow the stack, especially in a thread with a small stack. Put large buffers on the heap.
- **Small thread stacks.** Threads other than the main thread often get smaller stacks, so code that works in `main` can overflow in a worker thread.
- **Value types are not always on the stack.** In C# and Go, a value that is a field of a heap object, or that escapes, lives on the heap. Reason about copies and lifetimes, not about where the bytes are.
- **Many small heap allocations.** A loop that allocates a new object on each pass costs allocator time and garbage collection, and scatters data across memory. Reuse buffers, or keep data in arrays of values.

## Cost

A stack allocation costs one change to the stack pointer for the whole frame, and freeing costs nothing beyond the return. A heap allocation in C costs tens of nanoseconds to search the allocator's free lists, plus a call to `free` later. The JVM and .NET allocate by moving a pointer in a thread-local buffer, which is almost as cheap as a stack, and pay later in collection time, which grows with the number of live objects. The stack also wins on locality: the top frames are almost always in the cache, while heap objects reached through pointers can each cost a cache miss.

## History

Friedrich L. Bauer and Klaus Samelson described the stack principle for evaluating expressions in 1957, and Edsger W. Dijkstra showed in his 1960 paper "Recursive Programming" how a stack of activation records lets procedures call themselves, which Algol 60 required. The Burroughs B5000 of 1961 built a stack into its hardware. John McCarthy's Lisp, from 1960, kept its lists in a heap of cells and freed them with the first garbage collector.

C and Unix made the split between automatic storage in frames and memory from `malloc` the common model in the 1970s. Java and the JVM in 1995 put every object on a garbage-collected heap and gave each thread its own stack of frames.[^2] Go used segmented stacks for goroutines at its launch in 2009 and changed to contiguous stacks that grow by copying in Go 1.3 in 2014. Rust, from 2015, kept values in frames by default and made each heap allocation belong to an owner.[^1]

## References
