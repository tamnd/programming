---
tag: 8WW7BE
kind: concept
slug: stack-and-heap
name: Stack and heap
aliases: [call stack, stack frame, activation record, automatic storage, heap allocation, dynamic storage, escape analysis]
area: foundations/the-stack-and-the-heap
level: core
summary: >-
  Each active function call keeps its locals in a frame on the stack, which goes away at the return. Data that must outlive the call goes on the heap, where an allocator finds room for it.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [Edsger W. Dijkstra]
  note: In "Recursive Programming" Dijkstra described a stack with a stack pointer that holds the working space of each subroutine call, so that a subroutine can call itself, and applied it to the object programs of an ALGOL 60 compiler.
see_also: [explicit-memory-management, garbage-collection, pointers-and-references, use-after-free, closures, threads, memory-leaks, reference-counting]
same_as:
  wikidata: Q7595944
  wikipedia: Stack-based_memory_allocation
sources:
  - paper: doi:10.1007/BF01386232
    title: Edsger W. Dijkstra, Recursive Programming, Numerische Mathematik 2, 1960
    locator: sections The Aim and The Stack (the stack pointer that points to the first free place)
  - paper: doi:10.1145/320384.320386
    title: Jong-Deok Choi, Manish Gupta, Mauricio Serrano, Vugranam C. Sreedhar and Sam Midkiff, Escape analysis for Java, OOPSLA 1999
    locator: the paper as a whole, cited by the HotSpot documentation as the algorithm of its server compiler
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3096.pdf
    title: N3096, ISO/IEC 9899:2023 working draft (C23)
    locator: 6.2.4 Storage durations of objects, paragraphs 1, 2, 5, 6 and 7; 7.24.3 Memory management functions, paragraph 1
  - url: https://eel.is/c++draft/basic.stc.auto
    title: "Working Draft, Programming Languages: C++, [basic.stc.auto]"
    locator: 6.8.6.4 Automatic storage duration, paragraph 1
  - url: https://eel.is/c++draft/basic.stc.dynamic.general
    title: "Working Draft, Programming Languages: C++, [basic.stc.dynamic.general]"
    locator: 6.8.6.5.1 Dynamic storage duration, General, paragraph 1
  - url: https://clang.llvm.org/docs/AddressSanitizer.html#stack-use-after-return-uar
    title: Clang documentation, AddressSanitizer
    locator: Usage (the list of detected errors); Stack Use After Return (UAR)
  - url: https://clang.llvm.org/docs/DiagnosticsReference.html#wreturn-stack-address
    title: Clang documentation, Diagnostic flags in Clang
    locator: -Wreturn-stack-address
  - url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-2.html#jvms-2.5.2
    title: The Java Virtual Machine Specification, Java SE 25 Edition, Chapter 2
    locator: 2.5.2 Java Virtual Machine Stacks; 2.5.3 Heap; 2.6 Frames
  - url: https://docs.oracle.com/en/java/javase/25/vm/java-hotspot-virtual-machine-performance-enhancements.html#GUID-6BD8FCB5-995B-4AE9-BFAA-B2C7DE2BA5CD
    title: Java SE 25 Java Virtual Machine Guide, Java HotSpot Virtual Machine Performance Enhancements
    locator: Escape Analysis
  - url: https://docs.oracle.com/en/java/javase/25/docs/specs/man/java.html#extra-options-for-java
    title: Java SE 25 tool specifications, The java Command
    locator: Extra Options for Java, -Xss; Advanced JIT Compiler Options for java, -XX:+DoEscapeAnalysis
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-5.html#jls-5.1.7
    title: The Java Language Specification, Java SE 25 Edition, Chapter 5
    locator: 5.1.7 Boxing Conversion
  - url: https://go.dev/doc/faq#stack_or_heap
    title: Go FAQ
    locator: How do I know whether a variable is allocated on the heap or the stack?
  - url: https://go.dev/doc/faq#goroutines
    title: Go FAQ
    locator: Why goroutines instead of threads?
  - url: https://go.dev/doc/go1.3#stacks
    title: Go 1.3 Release Notes
    locator: Changes to the implementations and tools, Stack
  - url: https://go.dev/doc/devel/release#go1.3
    title: Go release history
    locator: go1.3 (released 2014-06-18)
  - url: https://pkg.go.dev/runtime/debug#SetMaxStack
    title: Go standard library, package runtime/debug
    locator: func SetMaxStack
  - url: https://pkg.go.dev/testing#AllocsPerRun
    title: Go standard library, package testing
    locator: func AllocsPerRun
  - url: https://pkg.go.dev/cmd/compile#hdr-Command_Line
    title: Go command documentation, cmd/compile
    locator: Command Line, flag -m
  - url: https://docs.python.org/3.14/library/sys.html#sys.setrecursionlimit
    title: The Python Standard Library, sys, Python 3.14
    locator: sys.getrecursionlimit and sys.setrecursionlimit
  - url: https://docs.python.org/3.14/library/exceptions.html#RecursionError
    title: The Python Standard Library, Built-in Exceptions, Python 3.14
    locator: RecursionError
  - url: https://docs.python.org/3.14/c-api/memory.html#overview
    title: Python/C API Reference Manual, Memory Management, Python 3.14
    locator: Overview; The pymalloc allocator
  - url: https://docs.python.org/3.14/reference/datamodel.html#objects-values-and-types
    title: The Python Language Reference, Data model, Python 3.14
    locator: 3.1 Objects, values and types (the CPython note on id)
  - url: https://docs.python.org/3.11/whatsnew/3.11.html#inlined-python-function-calls
    title: What's New In Python 3.11
    locator: Faster CPython, Cheaper, lazy Python frames; Inlined Python function calls
  - url: https://peps.python.org/pep-0664/#schedule
    title: "PEP 664: Python 3.11 Release Schedule"
    locator: Release Schedule, 3.11.0 final
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/value-types
    title: C# language reference, Value types
    locator: the introduction (copy on assignment, argument passing and return)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/types/boxing-and-unboxing#performance
    title: C# programming guide, Boxing and Unboxing
    locator: the introduction; Performance; Boxing
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/ref-struct
    title: C# language reference, ref structure types
    locator: the introduction and the list of restrictions
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/stackalloc
    title: C# language reference, stackalloc expression
    locator: the introduction; the rules on the amount of stack memory
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.stackoverflowexception#remarks
    title: .NET API, StackOverflowException Class
    locator: Remarks
  - url: https://doc.rust-lang.org/std/boxed/index.html
    title: The Rust Standard Library, module std::boxed
    locator: module documentation, Examples (moving a value from the stack to the heap; recursive data structures)
  - url: https://doc.rust-lang.org/std/thread/index.html#stack-size
    title: The Rust Standard Library, module std::thread
    locator: Stack size
---

## Idea

A running program needs memory for two kinds of data. Some data belongs to one call of a function: its arguments, its local variables, and the place to go back to when the call ends. This data lives exactly as long as the call. Other data must outlive the call that makes it. Examples are a record that a function builds and returns, a list that grows while the program runs, and a buffer that another thread reads. Most language implementations keep the first kind on the call stack and the second kind on the heap.

The stack is a region of memory that grows and shrinks at one end only. Each call adds a frame at the top, and each return removes the frame at the top. The heap is a pool of memory that an allocator manages. The program asks for a block of some size and uses it as long as it needs. The block comes back later, by an explicit release or through a garbage collector. The two differ in lifetime, in cost, and in who decides when the memory comes back.

In CPython every value is an object in a private heap, and a frame holds only references to objects.[^21][^22] In this run, `make_order` builds a dictionary and returns it. The frame of the call goes away at the return, but the dictionary stays, because it was never in the frame.

```python run id=idea
import inspect


def frames_below():
    return len(inspect.stack()) - 1


def make_order(item):
    order = {"item": item, "frames": frames_below()}
    return order


order = make_order("book")
print(order)
print("frames at module level:", frames_below())
```

```output
{'item': 'book', 'frames': 2}
frames at module level: 1
```

`frames_below` counts the frames under its own call. Inside `make_order` there are two (the call of `make_order` and the module), and at module level there is one.

## How it works

**The call stack.** Each thread has a stack of its own.[^8] A register, the stack pointer, holds the address of the top. A call moves the stack pointer to make room for a frame, and the return moves it back. Dijkstra described this scheme in 1960. A block of storage holds the stack, and the stack pointer points to the first free place in it.[^1] A frame usually holds these items:

- the return address, the place in the caller where execution goes on after the call;
- the registers of the caller that the called function must save before it uses them;
- the local variables of the call;
- the arguments that the calling convention does not pass in registers, and room for temporary values.

The calling convention of the platform fixes the exact layout. The diagram shows a program in the middle of a call of `total` from `main`.

```text
stack                                         heap

+-------------------------------+
| frame of main                 |
|   return address (into start) |
|   order  -------------------------------->  Order { id: 7, items: ---> }
+-------------------------------+                                    |
| frame of total(order)         |                                    v
|   return address (into main)  |                         [ "book", "pen" ]
|   saved registers             |
|   sum = 0                     |
+-------------------------------+  <- stack pointer (the top)
```

Frames come and go in last-in, first-out order, so the lifetime of each frame lies inside the lifetime of its caller's frame. A function can pass the address of a local variable to the functions that it calls. It cannot use that address after it returns, because the next call reuses the same memory. A recursive call gets a frame of its own, so each level has its own copy of the locals. The C standard says this directly: a block that is entered recursively gets a separate instance of each of its automatic objects.[^3]

**The heap.** Heap blocks have no such order. The allocator keeps track of the free parts of the heap. For each request it finds a free block that is large enough, marks it as used, and returns its address. A release marks the block as free again. The block lives from the allocation until the release, no matter which functions do them.[^3][^5] The internals of allocators, such as free lists and size classes, are on [[explicit-memory-management]]. The pseudocode puts the two side by side.

```pseudo
CALL f(arguments)
L1  put the arguments in registers or on the stack
L2  push the return address
L3  sp ← sp − framesize(f)              ▷ room for saved registers and locals
L4  run the body of f
RETURN
L5  sp ← sp + framesize(f)              ▷ the locals of f are gone
L6  pop the return address and jump to it

ALLOCATE(n)                              ▷ the heap
L7  find a free block b with size(b) ≥ n
L8  mark b as used and return its address
RELEASE(b)
L9  mark b as free                       ▷ by the program, the owner, or a collector
```

**Who decides where a value lives.** Languages split into three groups.

- In C, C++, Rust and C#, the program says it. C has automatic storage for locals and allocated storage for `malloc` blocks, and its standard never uses the words stack or heap.[^3] C++ calls the second kind dynamic storage, which `new` and `delete` manage.[^4][^5] Rust keeps values in the frame unless the program puts them in a `Box`, a `Vec` or another owner of a heap block.[^30] In C#, a variable of a value type holds an instance of the type. A variable of a class type holds a reference to an instance.[^25] Boxing stores a value type on the managed heap.[^26]
- In Go and Java, the compiler decides. The Go FAQ says that the program does not need to know: each variable exists as long as there are references to it. The compiler keeps a local in the frame when it can prove that no reference outlives the call. Otherwise it moves the local to the garbage-collected heap.[^12] The compiler flag `-m` prints these decisions.[^18] For `newOrder` in the Go example, `go build -gcflags=-m` reports `moved to heap: o`. The Java Virtual Machine allocates every class instance and array on the heap.[^8] The HotSpot compiler runs escape analysis and can remove the allocation of an object that does not escape. It does not move such an object to the stack.[^9]
- In Python and JavaScript, the program has no say. Objects live on the heap of the runtime, and the engine manages the frames.

## In languages

Every example file shows the two places. It shows a value whose lifetime is one call and a value that outlives the call. Some files also show the copy rules that follow from where a value lives. Several files also run into the stack limit.

| Language | Verdict | How the language exposes the stack and the heap |
|---|---|---|
| Python | native | every value is an object on the heap; frames hold references; the interpreter limits the depth of the stack[^19][^21] |
| JavaScript | native | objects on the heap of the engine; Node.js throws a `RangeError` when the stack is full |
| TypeScript | native | the runtime model of the JavaScript that it compiles to |
| Java | native | one stack of frames per thread; every object and array on the heap; primitives in locals; boxing makes objects[^8][^11] |
| C | native | automatic and allocated storage durations; `malloc` and `free`[^3] |
| C++ | native | automatic and dynamic storage durations; `new`, `delete`, `std::unique_ptr`[^4][^5] |
| C# | native | value types (`struct`) are copied; class instances are reached through references; boxing; `stackalloc` and `ref struct`[^25][^26][^27][^28] |
| Go | native | the compiler picks the place by escape analysis; goroutine stacks grow[^12][^13] |
| Rust | native | values in the frame by default; `Box` and `Vec` own heap blocks[^30] |
| Kotlin | native | on the JVM, the model of Java |
| PowerShell | native | the model of .NET, as for C# |
| Bash | n/a | the shell stores every value as a string and the script has no say in where |
| SQL | n/a | a query has no frames or heap objects of its own |
| Regex | n/a | a pattern has no storage |
| HTML/CSS | n/a | markup and style rules have no storage |

**Values and boxed objects.** Where a value lives also decides what an assignment copies. A Java `int` local holds the number, and assignment copies it. An array or any other object lives on the heap, and assignment copies only the reference. Boxing turns a primitive into an `Integer` or another wrapper object. The Java Language Specification notes that a boxing conversion can throw `OutOfMemoryError` when it must allocate a wrapper.[^11] C# has the same split between `struct` and `class`. Boxing a value type allocates an object on the heap and copies the value into it.[^26] Go and Rust treat structs as values that the program can copy, and a pointer or a `Box` is the way to share one. CPython has no unboxed values: an integer is an object, and the CPython `id` of an object is its address in memory.[^22]

**Managed runtimes.** In Java, Kotlin, C#, PowerShell, Go and JavaScript, a garbage collector frees heap objects. A reference from a live frame keeps an object alive, because the locals of the active frames are roots of the collector (see [[garbage-collection]]). In C and C++ the program frees heap blocks itself. In Rust the owner of the block frees it at the end of its scope (see [[rust/ownership]]).

## Variations and related ideas

- **Escape analysis.** The compiler proves that no reference to an object outlives the function or the thread. Go then keeps the variable in the frame.[^12] HotSpot instead replaces the fields of an object that does not escape with plain locals ("scalar replacement") and drops the allocation. It also removes locks on objects that do not escape globally.[^9] The flag `-XX:+DoEscapeAnalysis` is on by default.[^10]
- **Growable stacks.** A Go goroutine starts with a stack of a few kilobytes. The runtime grows and shrinks it as the calls need.[^13] To grow it, the runtime copies the whole stack to a larger contiguous block.[^14]
- **Stack-only types.** C# `stackalloc` puts a block of memory in the frame. The block goes away when the method returns.[^28] A `ref struct` can never be boxed, stored in a class field, or captured by a lambda, so it can never reach the heap.[^27] C has variable length arrays: automatic objects whose size is known only at run time, which live until the program leaves their scope.[^3]
- **Static storage.** Global variables and C `static` locals live in a third place, for the whole run of the program.[^3]
- **Closures.** A function that captures a local of its enclosing function can outlive that function, so the captured variable cannot stay in the frame. Go moves it to the heap, as the Cost section shows. See [[closures]].
- **Threads.** Each thread has its own stack, and all threads share the heap.[^8] So a value in a frame belongs to one thread, and an object on the heap needs synchronization when several threads use it. See [[threads]].
- **Pointers into the stack.** A pointer to a local is valid only while the frame exists. [[pointers-and-references]] covers the rules, and [[use-after-free]] covers the bugs.

## Pitfalls

### Returning the address of a local

A C function that keeps the address of a local after it returns leaves a dangling pointer. The C standard makes any use of it undefined behaviour.[^3] Clang warns about a plain `return &total;` by default.[^7] It does not warn when the address goes through a global variable, as here. AddressSanitizer finds the read as a `stack-use-after-return`. The check is on by default on Linux, and the flag `-fsanitize-address-use-after-return=always` turns it on everywhere.[^6]

```c fails expect=sanitizer:address timeout=30 id=keep-local
#include <stdio.h>

static int *last;

void make_total(int a, int b) {
    int total = a + b;
    last = &total; // the address outlives the frame
}

int main(void) {
    make_total(2, 3);
    printf("%d\n", *last);
}
```

Return the value itself, or put the value in a heap block whose lifetime the caller controls.

```c run id=return-value
#include <stdio.h>
#include <stdlib.h>

int make_total(int a, int b) {
    return a + b;
}

int *make_total_on_heap(int a, int b) {
    int *total = malloc(sizeof *total);
    if (total != NULL) {
        *total = a + b;
    }
    return total;
}

int main(void) {
    printf("%d\n", make_total(2, 3));
    int *total = make_total_on_heap(2, 3);
    if (total == NULL) {
        return 1;
    }
    printf("%d\n", *total);
    free(total);
}
```

```output
5
5
```

Go needs no fix for the same code: the compiler moves the local to the heap.[^12]

### Deep recursion overflows the stack

Each call takes a frame, so the depth of recursion is limited by the size of the stack. A recursive walk over a long linked structure fails long before the heap is full. CPython stops at a depth that `sys.getrecursionlimit()` returns, and raises `RecursionError`.[^19][^20]

```python fails expect=exception:RecursionError id=deep-recursion
def length(node):
    if node is None:
        return 0
    return 1 + length(node[1])


chain = None
for index in range(5000):
    chain = (index, chain)
print(length(chain))
```

The other runtimes fail in different ways. Java throws `StackOverflowError`, which a program can catch.[^8] .NET terminates the process, and no `catch` block can stop it.[^29] Go stops with a fatal error when a goroutine stack passes its maximum, which is 1 GB on 64-bit systems by default.[^16] Rust aborts the process. `sys.setrecursionlimit` raises the limit of CPython, but a limit that is too high can crash the interpreter.[^19] `-Xss` raises the stack size of the JVM threads.[^10] Both move the failure and do not remove it. A loop keeps one frame for the whole walk.

```python run id=loop
def length(node):
    count = 0
    while node is not None:
        count += 1
        node = node[1]
    return count


chain = None
for index in range(5000):
    chain = (index, chain)
print(length(chain))
```

```output
5000
```

For a tree, keep the nodes that wait for a visit in a list on the heap, and use the list as an explicit stack.

### A value type is a copy

A C# `struct` is copied on assignment, on a call, and on a return.[^25] The indexer of a `List<T>` returns a copy of the element. A method call on that copy changes the copy, and the list keeps the old value.

```csharp wrong id=struct-copy
using System;
using System.Collections.Generic;

var totals = new List<Total> { new Total() };
totals[0].Add(5);
Console.WriteLine(totals[0].Value);

struct Total
{
    public int Value;

    public void Add(int amount) => Value += amount;
}
```

```output
0
```

The program prints 0, not 5. Read the element into a local, change the local, and store it back. A `class` also fixes it, at the cost of a heap object for each element.

```csharp run id=struct-store
using System;
using System.Collections.Generic;

var totals = new List<Total> { new Total() };
var first = totals[0];
first.Add(5);
totals[0] = first;
Console.WriteLine(totals[0].Value);

struct Total
{
    public int Value;

    public void Add(int amount) => Value += amount;
}
```

```output
5
```

Boxing has the same trap. A boxed struct is a separate copy on the heap, so a change to the original does not reach it (see the C# example).[^26]

## Cost

**A stack allocation.** A call moves the stack pointer by the size of the frame, and the return moves it back. The JVM specification notes that the sizes of the local variable array and the operand stack are known at compile time. So the whole frame can be allocated at once at the call.[^8] The release costs nothing more than the return.

**A heap allocation.** The program calls the allocator. The allocator searches its free blocks and updates its records, and it can ask the operating system for more memory. Later a release, a reference count, or a collector gives the block back. The C# guide calls boxing computationally expensive compared with a plain assignment, because it must allocate and build an object.[^26] CPython uses pymalloc for objects of 512 bytes or less, and pymalloc takes them from arenas of 1 MiB on 64-bit platforms.[^21] In a collected language each heap allocation also adds work for the collector, so fewer allocations mean fewer collections.

**Counting allocations.** Go gives an exact count. `testing.AllocsPerRun` runs a function many times and returns the average number of heap allocations per run, which is always a whole number.[^17] In this run, returning a value allocates nothing. Returning a pointer to a local allocates the local. Returning a closure allocates the function value and the captured variable.

```go run id=allocs
package main

import (
	"fmt"
	"testing"
)

type order struct{ id, total int }

func newOrder(id int) *order {
	o := order{id: id, total: 100}
	return &o
}

func makeOrder(id int) order {
	return order{id: id, total: 100}
}

func counter() func() int {
	count := 0
	return func() int {
		count++
		return count
	}
}

var (
	sinkPointer *order
	sinkValue   order
	sinkFunc    func() int
)

func main() {
	fmt.Println("return a value:", testing.AllocsPerRun(100, func() { sinkValue = makeOrder(1) }))
	fmt.Println("return a pointer to a local:", testing.AllocsPerRun(100, func() { sinkPointer = newOrder(1) }))
	fmt.Println("return a closure:", testing.AllocsPerRun(100, func() { sinkFunc = counter() }))
}
```

```output
return a value: 0
return a pointer to a local: 1
return a closure: 2
```

This page shows no allocation count for Java. HotSpot escape analysis can remove allocations from compiled code, so a count depends on the compiler and on when it runs.[^9]

**Stack size.** The stack of each thread has a limit. HotSpot uses 1024 KB by default on Linux/x64 and 2048 KB on Linux/AArch64 and macOS/AArch64, and `-Xss` changes it.[^10] A thread that Rust spawns gets 2 MiB on all Tier-1 platforms, and Rust does not decide the stack size of the main thread.[^31] A goroutine starts with a few kilobytes. Its stack can grow to 1 GB on 64-bit systems and 250 MB on 32-bit systems.[^13][^16] A large local array can use up the stack at once: C# documents a `StackOverflowException` for a `stackalloc` that is too large.[^28]

## History

- **1960.** Edsger Dijkstra published "Recursive Programming". He described a stack with a stack pointer that holds the working space of each subroutine call. With it, a subroutine can be called again before its earlier calls end. He applied it to the object programs of an ALGOL 60 compiler.[^1]
- **1999.** Choi, Gupta, Serrano, Sreedhar and Midkiff presented escape analysis for Java at OOPSLA.[^2] The HotSpot server compiler implements their flow-insensitive algorithm and has it on by default from Java SE 6u23.[^9]
- **2014.** Go 1.3, released on 18 June 2014, replaced segmented goroutine stacks with contiguous stacks. A goroutine that needs more stack gets a larger single block, and the runtime copies the old stack into it.[^14][^15]
- **2022.** CPython 3.11, released on 24 October 2022, stopped calling the C interpreter function for each call from Python code to Python code. It also builds frame objects only when a debugger or an introspection function asks for them. Most Python calls since then use no C stack space.[^23][^24]

## References
