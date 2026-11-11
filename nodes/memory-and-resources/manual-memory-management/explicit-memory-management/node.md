---
tag: 1T4M29
kind: concept
slug: explicit-memory-management
name: Manual memory management
aliases: [manual memory management, explicit memory management, malloc and free, dynamic memory allocation]
area: memory-and-resources/manual-memory-management
level: core
summary: >-
  The program asks an allocator for a block of memory and later gives it back with an explicit call. The programmer decides when each block is freed, and the language does not check it.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1979
  people: [Bjarne Stroustrup]
  note: The Seventh Edition Unix manual of 1979 documented malloc, free, realloc and calloc. In the same year Stroustrup began C with Classes, which added the new and delete operators so that classes did not need a garbage collector.
see_also: [stack-and-heap, resource-acquisition-is-initialization, garbage-collection, reference-counting, memory-leaks, use-after-free, pointers-and-references, null-pointer-dereference]
same_as:
  wikidata: Q6752316
  wikipedia: Manual_memory_management
sources:
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: ISO/IEC 9899:2024 (en), N3220 working draft (C23)
    locator: 6.2.4 Storage durations of objects; 7.24.3 Memory management functions, 7.24.3.1 to 7.24.3.7; Annex M.2 and M.4
    accessed: 2026-11-10
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/C99RationaleV5.10.pdf
    title: Rationale for International Standard, Programming Languages, C, Revision 5.10
    locator: 7.20.3 Memory management functions; 7.20.3.2 The free function; 7.20.3.4 The realloc function
    accessed: 2026-11-10
  - url: https://timsong-cpp.github.io/cppwp/n4950/expr.delete
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[expr.delete] 7.6.2.9, paragraph 2 and note 1"
  - url: https://timsong-cpp.github.io/cppwp/n4950/new.delete.single
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[new.delete.single] 17.6.3.2, the throwing and the nothrow forms of operator new"
  - url: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#ri-raw
    title: C++ Core Guidelines
    locator: "I.11: Never transfer ownership by a raw pointer (T*) or reference (T&)"
    accessed: 2026-11-10
  - url: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rr-ptr
    title: C++ Core Guidelines
    locator: "R.3: A raw pointer (a T*) is non-owning"
    accessed: 2026-11-10
  - url: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rr-mallocfree
    title: C++ Core Guidelines
    locator: "R.10: Avoid malloc() and free()"
    accessed: 2026-11-10
  - url: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rr-newdelete
    title: C++ Core Guidelines
    locator: "R.11: Avoid calling new and delete explicitly"
    accessed: 2026-11-10
  - url: https://clang.llvm.org/docs/AddressSanitizer.html
    title: Clang documentation, AddressSanitizer
    locator: Introduction; Memory leak detection
    accessed: 2026-11-10
  - url: https://github.com/google/sanitizers/wiki/AddressSanitizerFlags
    title: google/sanitizers wiki, AddressSanitizerFlags
    locator: the run-time flag alloc_dealloc_mismatch
    accessed: 2026-11-10
  - url: https://clang.llvm.org/docs/LeakSanitizer.html
    title: Clang documentation, LeakSanitizer
    locator: Introduction
    accessed: 2026-11-10
  - url: https://sourceware.org/glibc/manual/latest/html_node/Basic-Allocation.html
    title: The GNU C Library Reference Manual
    locator: 3.2.3.1 Basic Memory Allocation
    accessed: 2026-11-10
  - url: https://sourceware.org/glibc/manual/latest/html_node/Malloc-Examples.html
    title: The GNU C Library Reference Manual
    locator: 3.2.3.2 Examples of malloc
    accessed: 2026-11-10
  - url: https://sourceware.org/glibc/manual/latest/html_node/Freeing-after-Malloc.html
    title: The GNU C Library Reference Manual
    locator: 3.2.3.4 Freeing Memory Allocated with malloc
    accessed: 2026-11-10
  - url: https://sourceware.org/glibc/manual/latest/html_node/Changing-Block-Size.html
    title: The GNU C Library Reference Manual
    locator: 3.2.3.5 Changing the Size of a Block
    accessed: 2026-11-10
  - url: https://sourceware.org/glibc/manual/latest/html_node/The-GNU-Allocator.html
    title: The GNU C Library Reference Manual
    locator: 3.2.2 The GNU Allocator
    accessed: 2026-11-10
  - url: https://sourceware.org/glibc/wiki/MallocInternals
    title: glibc wiki, Overview of Malloc
    locator: What is a Chunk?; Arenas and Heaps; Thread Local Cache (tcache); Free Algorithm; Platform-specific Thresholds and Constants
    accessed: 2026-11-10
  - paper: doi:10.1007/3-540-60368-9_19
    title: Paul R. Wilson, Mark S. Johnstone, Michael Neely and David Boles, Dynamic Storage Allocation, A Survey and Critical Review, International Workshop on Memory Management (IWMM), LNCS 986, 1995
    locator: 1.3 Splitting and coalescing; 2.1 Internal and external fragmentation; 3.2 Header fields and boundary tags; 3.6 Segregated free lists; the section on obstacks and zones
  - url: http://man.cat-v.org/unix_7th/3/malloc
    title: Unix Programmer's Manual, Seventh Edition, malloc(3)
    locator: DESCRIPTION, DIAGNOSTICS and BUGS
    accessed: 2026-11-10
  - url: http://man.cat-v.org/unix-6th/3/alloc
    title: Unix Programmer's Manual, Sixth Edition, alloc(3)
    locator: DESCRIPTION and DIAGNOSTICS
    accessed: 2026-11-10
  - paper: doi:10.1145/154766.155580
    title: Dennis M. Ritchie, The Development of the C Language, HOPL-II, 1993
    locator: the sections Standardization and Critique
  - paper: doi:10.1145/154766.155375
    title: Bjarne Stroustrup, A History of C++, 1979-1991, HOPL-II, 1993
    locator: 2 C with Classes, 1979-1983; 3.3.6 Memory Management
  - url: https://doc.rust-lang.org/std/alloc/fn.alloc.html
    title: The Rust Standard Library, function std::alloc::alloc
    locator: function description and example
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/std/alloc/trait.GlobalAlloc.html#tymethod.dealloc
    title: The Rust Standard Library, trait std::alloc::GlobalAlloc
    locator: alloc and dealloc, Safety
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/std/boxed/struct.Box.html#method.from_raw
    title: The Rust Standard Library, struct std::boxed::Box
    locator: into_raw and from_raw
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.marshal.allochglobal
    title: .NET API, Marshal.AllocHGlobal Method
    locator: Remarks
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.nativememory
    title: .NET API, NativeMemory Class
    locator: Methods
    accessed: 2026-11-10
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/foreign/Arena.html
    title: Java SE 25 API, java.lang.foreign.Arena
    locator: the table of arena kinds; ofConfined(); close()
    accessed: 2026-11-10
  - url: https://openjdk.org/jeps/454
    title: "JEP 454: Foreign Function & Memory API"
    locator: Summary and History
    accessed: 2026-11-10
  - url: https://go.dev/doc/faq#garbage_collection
    title: Go FAQ
    locator: Why do garbage collection? Won't it be too expensive?
    accessed: 2026-11-10
  - url: https://pkg.go.dev/cmd/cgo
    title: Go command documentation, cgo
    locator: C.CString, C.free and C.malloc
    accessed: 2026-11-10
  - url: https://docs.python.org/3.14/c-api/memory.html#overview
    title: Python/C API Reference Manual, Memory Management, Python 3.14
    locator: Overview; PyMem_Malloc and PyMem_Free
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/native-c-interop.html#memory-allocation
    title: Kotlin documentation, Interoperability with C
    locator: Memory allocation, nativeHeap and memScoped
    accessed: 2026-11-10
---

## Idea

A program often needs memory whose size or lifetime it knows only at run time. Examples are a list of orders from a file, a message buffer and a growing tree. Such memory cannot live in a stack frame, because the frame ends when the function returns. The program asks an allocator for a block from the heap instead. With manual memory management, the program also gives the block back with an explicit call. Nothing frees the block for it.

C has four storage durations: static, thread, automatic and allocated. Allocated storage lives from the call that allocates it to the call that frees it.[^1] In the C program below, `malloc` returns a block for four `int` values, or a null pointer when it has no memory. The program checks the result, uses the block, and frees it.

```c run id=idea
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    size_t count = 4;
    int *totals = malloc(count * sizeof *totals);
    if (totals == NULL) {
        return 1;
    }
    for (size_t i = 0; i < count; i++) {
        totals[i] = (int)(i * 10);
    }
    printf("last total: %d\n", totals[count - 1]);
    free(totals);
    return 0;
}
```

```output
last total: 30
```

The model gives the programmer full control and full responsibility. The language does not track which pointer owns a block. A block that is never freed is a leak. A block that is freed twice, or used after it is freed, is undefined behaviour.[^1] Garbage collection, reference counting and ownership types are the other answers to the same question: who frees this block, and when?

## How it works

The C library has a small set of functions for allocated storage.[^1]

| Function | What it does |
|---|---|
| `malloc(size)` | returns a block of `size` bytes; the contents are indeterminate |
| `calloc(n, size)` | returns a block for `n` objects of `size` bytes, all bits zero; it returns a null pointer if `n * size` does not fit in a `size_t` |
| `realloc(ptr, size)` | returns a block of `size` bytes with the old contents up to the smaller size; the block can move |
| `aligned_alloc(align, size)` | as `malloc`, with a stated alignment |
| `free(ptr)` | gives the block back; `free(NULL)` does nothing |
| `free_sized(ptr, size)` | as `free`, and the caller also passes the size it asked for |

Each function returns a null pointer when it cannot allocate the memory. The program must check for it, or it risks a [[null-pointer-dereference]].[^13] The C standard does not say where the blocks are, or in what order they come.[^1] Calling `free` on a pointer that the allocator did not return, or on a block that is already free, is undefined behaviour.[^1] The rationale states the rule plainly. Free only what you allocated, free it only once, and do not touch it after the free.[^2]

In C++, a `new` expression calls an allocation function and then runs the constructor. A `delete` expression runs the destructor and then calls the deallocation function. The default `operator new` returns a valid pointer or throws `std::bad_alloc`; it never returns null. The `std::nothrow` forms return null instead.[^4] The array forms are separate. `new T[n]` must be paired with `delete[]`, and `new T` with `delete`; any other pairing is undefined behaviour.[^3]

### The allocator

The allocator is ordinary library code. It gets large regions from the operating system and cuts them into blocks. Each block starts with a header that stores at least its size, so that `free` needs only the pointer. A free block also holds the links of a free list in its own payload, so the list costs no extra memory.[^17][^18]

```text
            header        payload
block A   | size 32 | in use: order          |
block B   | size 48 | free: next -> D        |
block C   | size 32 | in use: user           |
block D   | size 64 | free: next -> (end)    |
top       | the rest of the region, not yet cut into blocks |

free list: B -> D
```

The pseudocode shows a first-fit allocator, the method of the Seventh Edition Unix `malloc`.[^19] `MALLOC` takes the first free block that is large enough and splits off the rest. `FREE` merges the block with free neighbours, which is called coalescing.[^18]

```pseudo
MALLOC(n)
L1  size ← n plus the header, rounded up to the alignment
L2  for each block b on the free list do              ▷ first fit
L3      if size(b) ≥ size then
L4          if size(b) − size ≥ the minimum block then split b; put the rest on the free list
L5          remove b from the free list; return the payload of b
L6  cut size bytes from the top, or ask the operating system for more
L7  if the operating system refuses then return null

FREE(p)
L8  if p = null then return
L9  b ← the block whose payload starts at p          ▷ the header is before p
L10 if the next block is free then merge it into b
L11 if the previous block is free then merge b into it ▷ needs a size at the end of each block
L12 put b on the free list
```

Line L11 needs the size of the previous block. Knuth's boundary tags store the size in a footer at the end of each block as well as in the header.[^18] Real allocators add more structure. glibc keeps chunks in bins by size class and keeps a small cache of free chunks per thread (tcache). It also gives groups of threads separate arenas to reduce lock contention.[^17] Some of its small bins (fastbins) are not coalesced at all, so a free is faster but memory stays split.[^17] Very large requests go directly to `mmap`, and their `free` returns the memory to the operating system at once.[^16] Most other freed memory stays in the free lists, so a process rarely gets smaller after `free`.[^14]

Fragmentation is the price of this design. Internal fragmentation is memory inside a block that the program did not ask for, from rounding and headers. External fragmentation is free memory split into pieces that are too small for the requests that come.[^18] C cannot move a live block to close the gaps, because the program holds raw addresses.[^18] A compacting garbage collector can move blocks; a C allocator cannot.

### Ownership conventions

The language does not record who frees a block, so the program must state it. Each function that returns or takes a pointer needs one of three contracts:

- **The caller frees.** The function allocates and returns the block, and the caller must free it. cgo documents `C.CString` this way: the string is in the C heap, and the caller must call `C.free`.[^31]
- **The callee keeps it.** The function borrows the pointer and does not free it.
- **Ownership moves.** The function takes the block and frees it later. Rust makes this explicit: `Box::into_raw` makes the caller responsible for the memory, and `Box::from_raw` takes the responsibility back.[^25]

Libraries often pair the functions: `order_create` and `order_destroy`, each documented in the header. The C++ Core Guidelines treat a raw pointer `T*` as non-owning (R.3). They forbid the transfer of ownership through a raw pointer (I.11). When the owner is in doubt, the result is a leak or an early destruction.[^5][^6] Where a raw owning pointer is necessary, the guidelines mark it as `owner<T*>`.[^5]

## In languages

| Language | Verdict | How a program frees memory by hand |
|---|---|---|
| C | native | `malloc`, `calloc`, `realloc`, `aligned_alloc` and `free`[^1] |
| C++ | native | `new` and `delete`, `new[]` and `delete[]`, plus the C functions[^3][^4] |
| Rust | stdlib | `std::alloc::alloc` and `dealloc` with a `Layout`, in unsafe code; `Box::into_raw` and `Box::from_raw`[^23][^24][^25] |
| C# | stdlib | `Marshal.AllocHGlobal` and `Marshal.FreeHGlobal`; `NativeMemory.Alloc` and `NativeMemory.Free` in unsafe code[^26][^27] |
| Java | stdlib | `Arena.ofConfined()` allocates memory outside the heap; `close()` frees all of it[^28] |
| Kotlin | stdlib | Kotlin/Native: `nativeHeap.alloc` and `nativeHeap.free`; on the JVM, the Java `Arena`[^33] |
| PowerShell | stdlib | the .NET `Marshal` methods, as in C# |
| Go | absent | the garbage collector frees all memory; cgo can call `C.malloc` and `C.free`[^30][^31] |
| Python | absent | the Python memory manager owns the private heap; C extensions use `PyMem_Malloc` and `PyMem_Free`[^32] |
| JavaScript | absent | no operation frees an object; the engine collects it |
| TypeScript | absent | as JavaScript |
| Bash | n/a | the shell manages its own strings and arrays |
| SQL | n/a | the database engine manages storage |
| Regex | n/a | a pattern has no memory to manage |
| HTML/CSS | n/a | markup and style have no memory to manage |

C and C++ are the languages where manual management is the base model. C++ adds constructors and destructors to the same idea, so a `new` expression gives an initialized object and not raw bytes. The Core Guidelines advise against `malloc` and `free` in C++ (R.10), and against explicit `new` and `delete` in application code (R.11). They prefer owners such as `std::unique_ptr` and containers that free their memory in a destructor.[^7][^8] This is the subject of [[resource-acquisition-is-initialization]].

Rust normally frees memory through ownership: a value is dropped when its owner goes out of scope (see [[rust/ownership]]). The `std::alloc` functions expose the global allocator for code that builds its own containers. The call to `dealloc` must pass the same `Layout` that `alloc` received, and both calls need `unsafe`.[^23][^24]

C# and Java run on a garbage-collected heap, so the manual APIs are for memory outside that heap, mostly for calls into native code. .NET describes `AllocHGlobal` as a legacy API and points to `NativeMemory`.[^26] The Java `Arena` frees a group of segments at once. A confined arena belongs to one thread, and an access after `close` throws `IllegalStateException`.[^28]

Go, Python, JavaScript and TypeScript give the program no free operation. The Go FAQ calls memory management by hand in C a frequent cause of bugs, and Go uses garbage collection alone.[^30] In Python, the user has no control over the private heap.[^32] Go and Python can still reach a C allocator through cgo or a C extension, but then the C rules apply.

## Variations and related ideas

- **Arenas and regions.** The program allocates many objects in one region and frees the whole region in one call. Wilson et al. describe Ross's zones and Hanson's obstacks as early forms.[^18] The Java `Arena` is a region in this sense.[^28]
- **Pools and size classes.** A pool serves objects of one size from a free list for that size. Segregated free lists apply the idea to the whole heap, with size classes that are often powers of two.[^18] Stroustrup found per-class allocators effective in early C++ programs.[^22]
- **Sized deallocation.** `free_sized` in C and `dealloc` in Rust take the size of the block, so the allocator need not look it up.[^1][^24]
- **Stack allocation.** Memory with a known lifetime can live in a stack frame and needs no free at all. See [[stack-and-heap]].
- **Automatic management.** [[reference-counting]] frees a block when the last reference goes away. [[garbage-collection]] frees blocks that the program can no longer reach. Both remove the explicit free, at a cost in time or memory.
- **Use after free.** A pointer to a freed block is dangling. [[use-after-free]] explains this weakness in detail.

## Pitfalls

### Double free

Two paths free the same block. The second call is undefined behaviour, and it can corrupt the free list of the allocator.[^1] Here `release_order` frees the block, and then `main` frees it again.

```c fails expect=sanitizer:address timeout=30 id=double-free
#include <stdio.h>
#include <stdlib.h>

struct order {
    int id;
    int quantity;
};

void release_order(struct order *order) {
    printf("order %d released\n", order->id);
    free(order);
}

int main(void) {
    struct order *order = malloc(sizeof *order);
    if (order == NULL) {
        return 1;
    }
    order->id = 7;
    order->quantity = 2;
    release_order(order);
    free(order); // the block is already free
    return 0;
}
```

AddressSanitizer reports `attempting double-free`.[^9] Without the sanitizer, the second `free` can seem to work, and the damage shows up later. The fix is one owner for each block. Here `release_order` takes the address of the pointer, frees the block, and clears the pointer. The second call then frees a null pointer, which does nothing.[^1]

```c run id=single-owner
#include <stdio.h>
#include <stdlib.h>

struct order {
    int id;
    int quantity;
};

// Takes ownership: frees the order and clears the caller's pointer.
void release_order(struct order **order) {
    if (*order == NULL) {
        return;
    }
    printf("order %d released\n", (*order)->id);
    free(*order);
    *order = NULL;
}

int main(void) {
    struct order *order = malloc(sizeof *order);
    if (order == NULL) {
        return 1;
    }
    order->id = 7;
    order->quantity = 2;
    release_order(&order);
    release_order(&order);
    printf("order is null: %s\n", order == NULL ? "yes" : "no");
    return 0;
}
```

```output
order 7 released
order is null: yes
```

Clearing one pointer does not clear its copies. The rule must hold for the whole program: one owner frees, and the other pointers do not outlive it.

### `new[]` with `delete`

An array from `new[]` must go back through `delete[]`.[^3] The plain `delete` runs one destructor and passes the wrong pointer or size to the deallocation function.

```cpp fails expect=compile flags="-Werror=mismatched-new-delete" id=array-delete
#include <print>

struct Line {
    int quantity = 0;
};

int main() {
    Line* lines = new Line[3];
    lines[0].quantity = 5;
    std::println("first quantity: {}", lines[0].quantity);
    delete lines;
}
```

Clang sees both expressions in one function and warns with `-Wmismatched-new-delete`. This block turns the warning into an error. When the `new[]` and the `delete` are in different functions, the compiler cannot see the mismatch. AddressSanitizer then reports `alloc-dealloc-mismatch` at run time, but this check is off by default on macOS and Windows.[^10] The fix is `delete[]`, or a container that owns the array.

```cpp run id=vector-owns
#include <print>
#include <vector>

struct Line {
    int quantity = 0;
};

int main() {
    Line* lines = new Line[3];
    lines[0].quantity = 5;
    std::println("first quantity: {}", lines[0].quantity);
    delete[] lines;

    std::vector<Line> owned(3); // frees its array in the destructor
    owned[0].quantity = 5;
    std::println("first quantity: {}", owned[0].quantity);
}
```

```output
first quantity: 5
first quantity: 5
```

### `realloc` that loses the old block

When `realloc` cannot grow a block, it returns a null pointer and leaves the old block where it was.[^1][^15] The line `buffer = realloc(buffer, size)` writes that null pointer over the only copy of the old address. The old block is then lost. The program below stands in a failing function for `realloc`, so that the out-of-memory path runs on every machine.

```c fails expect=sanitizer:leak timeout=30 id=realloc-overwrite
#include <stdio.h>
#include <stdlib.h>

// Stands in for realloc when the system has no memory left.
static void *realloc_out_of_memory(void *ptr, size_t size) {
    (void)ptr;
    (void)size;
    return NULL;
}

int main(void) {
    int *buffer = malloc(4 * sizeof *buffer);
    if (buffer == NULL) {
        return 1;
    }
    buffer[0] = 42;
    buffer = realloc_out_of_memory(buffer, 8 * sizeof *buffer);
    if (buffer == NULL) {
        printf("grow failed\n");
        return 0; // the old block is unreachable
    }
    free(buffer);
    return 0;
}
```

LeakSanitizer reports the block of 16 bytes from `malloc`. The fix keeps the result in a second variable and replaces the old pointer only on success. On failure, the program still has its data.

```c run id=realloc-temp
#include <stdio.h>
#include <stdlib.h>

// Stands in for realloc when the system has no memory left.
static void *realloc_out_of_memory(void *ptr, size_t size) {
    (void)ptr;
    (void)size;
    return NULL;
}

int main(void) {
    int *buffer = malloc(4 * sizeof *buffer);
    if (buffer == NULL) {
        return 1;
    }
    buffer[0] = 42;
    int *grown = realloc_out_of_memory(buffer, 8 * sizeof *buffer);
    if (grown == NULL) {
        printf("grow failed, first value still %d\n", buffer[0]);
    } else {
        buffer = grown;
    }
    free(buffer);
    return 0;
}
```

```output
grow failed, first value still 42
```

The C99 rationale explains that `realloc` deallocates the old object and allocates another one, even when the address stays the same.[^2] So any other pointer into the old block is dangling after a successful call.

### A leak on an early return

A function with several exits must free its blocks on each exit. An error path is the usual place to forget one. A leak does no visible harm at once; the process only grows. A long-running server can grow until the system stops it. See [[memory-leaks]] for the general problem and its tools.

```c fails expect=sanitizer:leak timeout=30 id=early-return
#include <stdio.h>
#include <stdlib.h>

int sum_quantities(const int *quantities, size_t count, int *sum) {
    int *copy = malloc(count * sizeof *copy);
    if (copy == NULL) {
        return -1;
    }
    *sum = 0;
    for (size_t i = 0; i < count; i++) {
        if (quantities[i] < 0) {
            return -1; // copy is not freed
        }
        copy[i] = quantities[i];
        *sum += copy[i];
    }
    free(copy);
    return 0;
}

int main(void) {
    const int quantities[] = {3, -1, 4};
    int sum = 0;
    if (sum_quantities(quantities, 3, &sum) != 0) {
        printf("bad quantity\n");
    }
    return 0;
}
```

LeakSanitizer finds the block when the process exits. It has almost no cost until then.[^11] The fix sends every exit through one cleanup label, a common pattern in C.

```c run id=one-exit
#include <stdio.h>
#include <stdlib.h>

int sum_quantities(const int *quantities, size_t count, int *sum) {
    int status = -1;
    int *copy = malloc(count * sizeof *copy);
    if (copy == NULL) {
        return -1;
    }
    *sum = 0;
    for (size_t i = 0; i < count; i++) {
        if (quantities[i] < 0) {
            goto done;
        }
        copy[i] = quantities[i];
        *sum += copy[i];
    }
    status = 0;
done:
    free(copy);
    return status;
}

int main(void) {
    const int quantities[] = {3, -1, 4};
    int sum = 0;
    if (sum_quantities(quantities, 3, &sum) != 0) {
        printf("bad quantity\n");
    }
    return 0;
}
```

```output
bad quantity
```

In C++ and Rust, a destructor or a drop does this work on each exit, which is the main argument for [[resource-acquisition-is-initialization]].

### The size of the pointer instead of the object

`malloc(sizeof order)` asks for the size of the pointer, 8 bytes on a 64-bit machine, and not the size of the structure. The writes then go past the end of the block, an [[out-of-bounds-write]].

```c fails expect=sanitizer:address timeout=30 id=sizeof-pointer
#include <stdio.h>
#include <stdlib.h>

struct order {
    int id;
    int quantity;
    double price;
};

int main(void) {
    struct order *order = malloc(sizeof order); // the size of the pointer
    if (order == NULL) {
        return 1;
    }
    order->id = 7;
    order->quantity = 2;
    order->price = 9.5;
    printf("total: %.2f\n", order->quantity * order->price);
    free(order);
    return 0;
}
```

AddressSanitizer reports a `heap-buffer-overflow` on the write to `price`. The glibc manual recommends the form `malloc(sizeof *ptr)`, which takes the size from the object that the pointer points to.[^12] It stays correct when the type of the pointer changes.

```c run id=sizeof-object
#include <stdio.h>
#include <stdlib.h>

struct order {
    int id;
    int quantity;
    double price;
};

int main(void) {
    struct order *order = malloc(sizeof *order);
    if (order == NULL) {
        return 1;
    }
    order->id = 7;
    order->quantity = 2;
    order->price = 9.5;
    printf("total: %.2f\n", order->quantity * order->price);
    free(order);
    return 0;
}
```

```output
total: 19.00
```

## Cost

- **Header and rounding.** Each block carries a header. Wilson et al. give an example: a one-word header on objects of about 10 words costs about 10% of the memory.[^18] The glibc wiki lists typical 64-bit values: blocks are aligned to 16 bytes, and the smallest chunk is 32 bytes.[^17]
- **Time per call.** A call is usually a few list operations, but it is not constant. A search, a split or a coalesce can run, and a request can reach the operating system. Stroustrup reports early C++ programs that spent 50% or more of their time in `malloc`.[^22]
- **Large blocks.** glibc serves a request above the `mmap` threshold with its own mapping. The threshold changes with the allocation pattern of the program. The glibc wiki lists a default of 128 KiB and a maximum of 32 MiB on 64-bit systems.[^16][^17]
- **Memory that stays in the process.** `free` puts most blocks back on a free list, not back to the operating system.[^14] Fragmentation can keep the process large after the program frees most of its data.
- **Threads.** glibc gives threads separate arenas to reduce lock contention, up to eight times as many arenas as there are CPUs.[^17]
- **Checking.** AddressSanitizer finds double frees and invalid frees with a typical slowdown of about two times.[^9] Release builds usually run without it, so tests must find these bugs.

## History

- **1979.** The Seventh Edition Unix manual, edited by McIlroy and Kernighan, documented `malloc`, `free`, `realloc` and `calloc`. The allocator used first fit and coalesced free neighbours during its search.[^19][^21] Its BUGS section warned that when `realloc` returned 0, the old block could be destroyed. The Sixth Edition library before it had `alloc` and `free`, which returned -1 when no memory was left.[^20]
- **1979 to 1983.** Stroustrup built C with Classes and added the `new` and `delete` operators. `new` allocates and runs a constructor, an idea from Simula. `delete` was added because he did not want the language to depend on a garbage collector.[^22]
- **1989.** The ANSI committee X3J11 finished the C standard X3.159-1989, which ISO accepted as ISO/IEC 9899-1990.[^21] The rationale records that the committee rejected zero-length objects for these functions.[^2]
- **1993.** Ritchie wrote that C provides allocated storage only through a library routine, and the programmer carries the burden. In his words, "C is hostile to automatic garbage collection."[^21]
- **1995.** Wilson, Johnstone, Neely and Boles published a survey of allocators that set the terms for fragmentation, splitting, coalescing and segregated free lists.[^18]
- **1999.** C99 defined `realloc` as the deallocation of the old object and the allocation of another one. C89 had treated the result as the same object.[^2]
- **2011.** C11 added `aligned_alloc`.[^1]
- **2024.** C23, ISO/IEC 9899:2024, added `free_sized` and `free_aligned_sized`, and made a `realloc` to size zero undefined behaviour.[^1] JEP 454 completed the Java Foreign Function and Memory API in JDK 22, after previews in JDK 19, 20 and 21. Its `Arena` manages memory outside the heap.[^29]

## References
