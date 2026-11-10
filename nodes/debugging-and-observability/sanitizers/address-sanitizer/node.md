---
tag: NEW-address-sanitizer
kind: concept
slug: address-sanitizer
name: AddressSanitizer
aliases: [ASan, address sanitizer, -fsanitize=address]
area: debugging-and-observability/sanitizers
level: core
summary: >-
  AddressSanitizer (ASan) is a compiler instrumentation that stops a C or C++ program at an out-of-bounds access or a use of freed memory. It marks bad bytes in shadow memory.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2012
  people: [Konstantin Serebryany, Derek Bruening, Alexander Potapenko, Dmitry Vyukov]
  note: The USENIX ATC 2012 paper describes the shadow encoding, the allocator with redzones and quarantine, and the use of the tool on Chromium, where it found over 300 bugs that were not known before.
see_also: [use-after-free, out-of-bounds-write, out-of-bounds-read, memory-leaks, data-races, integer-overflow, fuzz-testing, stack-and-heap]
same_as:
  wikidata: Q16001773
sources:
  - url: https://www.usenix.org/system/files/conference/atc12/atc12-final39.pdf
    title: Konstantin Serebryany, Derek Bruening, Alexander Potapenko and Dmitry Vyukov, AddressSanitizer, A Fast Address Sanity Checker, USENIX ATC 2012
    locator: Abstract; 1 Introduction; 3.1 Shadow Memory; 3.2 Instrumentation; 3.3 Run-time Library; 3.4 Stack And Globals; 3.5 False Negatives; 3.6 False Positives; 4 Evaluation; 4.1 Comparison; 4.2 AddressSanitizer Deployment; 4.3 Tuning
  - url: https://clang.llvm.org/docs/AddressSanitizer.html
    title: Clang documentation, AddressSanitizer
    locator: Introduction; Usage; Container Overflow Detection; Limitations; Supported Platforms
  - url: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
    title: Using the GNU Compiler Collection, Program Instrumentation Options
    locator: -fsanitize=address and -fsanitize=thread
  - url: https://learn.microsoft.com/en-us/cpp/sanitizers/asan
    title: Microsoft C++ documentation, AddressSanitizer
    locator: the overview (the /fsanitize=address option and the supported targets)
  - url: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
    title: Clang documentation, UndefinedBehaviorSanitizer
    locator: Introduction; Available checks
  - url: https://clang.llvm.org/docs/ThreadSanitizer.html
    title: Clang documentation, ThreadSanitizer
    locator: Introduction
  - url: https://clang.llvm.org/docs/MemorySanitizer.html
    title: Clang documentation, MemorySanitizer
    locator: Introduction; Supported Platforms; Limitations
  - url: https://clang.llvm.org/docs/LeakSanitizer.html
    title: Clang documentation, LeakSanitizer
    locator: Introduction; Usage; Supported Platforms
  - url: https://doc.rust-lang.org/beta/unstable-book/compiler-flags/sanitizer.html
    title: The Rust Unstable Book, sanitizer
    locator: the introduction (-Zsanitizer and the supported targets) and AddressSanitizer
  - url: https://pkg.go.dev/cmd/go#hdr-Compile_packages_and_dependencies
    title: Command go, Compile packages and dependencies
    locator: the build flags -asan and -race
  - url: https://github.com/llvm/llvm-project/blob/main/compiler-rt/include/sanitizer/asan_interface.h
    title: LLVM compiler-rt, sanitizer/asan_interface.h
    locator: __asan_poison_memory_region, __asan_address_is_poisoned, ASAN_POISON_MEMORY_REGION
---

## Idea

C and C++ do not check memory accesses. A read past the end of an array, a write through a pointer to freed memory and a write past the end of a stack buffer are undefined behavior. The program does not have to fail. It often prints the right answer in a test and corrupts memory in production. These errors are [[out-of-bounds-write]], [[out-of-bounds-read]] and [[use-after-free]], and they cause many security holes.

AddressSanitizer (ASan) turns these silent errors into loud ones. The compiler adds a check before each load and store. The check asks one question: may the program touch this byte now? If the answer is no, ASan stops the program and prints a report with the kind of error, the address, the stack of the bad access and the stacks where the memory was allocated and freed.[^1]

The program below writes one byte past the end of a heap buffer. Without ASan, it usually prints `alice`. With `-fsanitize=address`, it stops in `strcpy` with a `heap-buffer-overflow` report.

```c fails expect=sanitizer:address timeout=30 id=idea
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    const char *name = "alice";
    char *copy = malloc(strlen(name)); // no room for the '\0'
    if (copy == NULL) {
        return 1;
    }
    strcpy(copy, name);
    printf("%s\n", copy);
    free(copy);
}
```

ASan is a test tool. A team builds its tests, its fuzzers and its debug builds with it, and ships a build without it. It finds errors only on the paths that a test runs, so it works together with [[unit-testing]] and [[fuzz-testing]].

## How it works

ASan has two parts. A compiler pass adds the checks to the code. A run-time library replaces `malloc` and `free`, keeps the shadow memory and prints the reports.[^1]

### Shadow memory

Allocators return blocks aligned to at least 8 bytes. So each aligned group of 8 bytes, a granule, is in one of 9 states: the first k bytes are addressable and the other 8 − k are not, for k from 0 to 8. One byte can encode this state. ASan keeps one shadow byte for each 8 bytes of application memory, so the shadow uses one eighth of the address space. The address of the shadow byte is a shift and an add:[^1]

```text
shadow address = (address >> 3) + Offset

shadow value    meaning for the 8 bytes of the granule
0               all 8 bytes are addressable
1 to 7          only the first k bytes are addressable
negative        no byte is addressable; the value tells the kind:
                heap redzone, stack redzone, global redzone, freed memory
```

On 64-bit Linux the paper uses an offset of 2⁴⁴. The offset is fixed for each platform, so the check needs no table lookup.[^1]

### The check

For an 8-byte access, the compiler inserts a load of the shadow byte and a test for zero. For a 1-, 2- or 4-byte access, it also compares the position inside the granule with k:[^1]

```text
shadow = *((address >> 3) + Offset)
if (shadow != 0 && (address & 7) + size > shadow)
    report the error and stop
```

Each access in the original code gets exactly one extra memory read. The pass runs at the end of the LLVM optimization pipeline, so it checks only the accesses that the optimizer keeps.[^1]

### Redzones

A check needs poisoned bytes next to each object. These bytes are the redzones.

- **Heap.** The run-time library replaces `malloc`. Blocks of one size class sit in a row, with a redzone between each pair, so n blocks need n + 1 redzones. The left redzone also holds the allocation size, the thread id and the stack of the `malloc` call. The paper gives a minimum heap redzone of 32 bytes.[^1]
- **Stack.** The compiler puts a 32-byte redzone, plus up to 31 bytes for alignment, around each local array in the frame, and poisons it when the function starts.[^1]
- **Globals.** The compiler adds redzones after global variables, and the run-time library poisons them at start-up.[^1]

The C example asks the run-time library about each byte near a 13-byte block. The left redzone is poisoned. Bytes 13 to 15 are poisoned through the partial value of the second granule, and the right redzone follows.

```text
buf-2 buf-1 | buf+0 ... buf+12 | buf+13 ... buf+17
  1     1   |   0   ...   0    |   1    ...   1      (1 = poisoned)
```

### Quarantine

`free` poisons the whole block and puts it in a quarantine, a FIFO queue of freed memory. A later `malloc` does not get the block until it leaves the queue. A use through a dangling pointer then hits poisoned memory and ASan reports a `heap-use-after-free`, with the stacks of the allocation, the free and the use.[^1]

### What it finds and what it misses

Clang lists these errors: out-of-bounds accesses to the heap, the stack and globals, use after free, use after return, use after scope, double free, invalid free, and memory leaks.[^2] The paper says that ASan has no false positives: the reports that the authors judged undesirable during development were all fixed.[^1]

ASan misses some errors by design:[^1]

- An unaligned access that is partly out of bounds, for example a 4-byte write at offset 6 of an 8-byte block.
- An access far past the end of an object, which jumps over the redzone and lands in another valid block. The paper's example is `a[500]` for a 100-byte `a`.
- A use after free when so much memory was freed in between that the block left the quarantine and was reused.

ASan checks only code that the compiler instrumented. A library built without ASan is not checked, although the replaced `malloc` and the intercepted functions such as `strcpy` still are. ASan also finds nothing on a path that the tests do not run.

## In languages

The example files show heap and stack overflows in C and C++, and the checks that Rust makes without a sanitizer. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| C | library | `-fsanitize=address` in Clang and GCC, `/fsanitize=address` in MSVC |
| C++ | library | as C; containers can annotate their buffers |
| Rust | library | `-Zsanitizer=address` on a nightly compiler, for `unsafe` code and foreign code |
| Go | n/a | Go code is checked; `go build -asan` checks C code linked through cgo, on Linux only |
| Python | n/a | the interpreter checks each access; ASan is for C extensions and for CPython itself |
| Java | n/a | the JVM checks array bounds and collects garbage |
| C# | n/a | the runtime checks bounds and collects garbage |
| JavaScript | n/a | the engine checks bounds and collects garbage |
| TypeScript | n/a | as JavaScript |
| Kotlin | n/a | the JVM checks array bounds and collects garbage |
| PowerShell | n/a | .NET checks bounds and collects garbage |
| Bash | n/a | no pointers and no manual memory |
| SQL, Regex, HTML, CSS | n/a | no memory access in the language |

Clang and GCC both take `-fsanitize=address`.[^2][^3] MSVC takes `/fsanitize=address` for x86 and x64 targets on Windows.[^4] The Clang documentation asks for `-O1` or higher and `-fno-omit-frame-pointer` for better stack traces.[^2] By default, ASan stops at the first error.[^2]

Rust has the flag `-Zsanitizer=address`. It is unstable, so it needs a nightly compiler, and the documentation recommends a rebuild of the standard library with `-Zbuild-std`.[^9] Safe Rust checks indices and rejects dangling references at compile time, so ASan in Rust is for `unsafe` blocks and for the C and C++ code that Rust calls.

The `go` command has `-asan`, which links Go code with C code built with ASan. It works only on linux/arm64, linux/amd64 and linux/loong64.[^10] Go code itself is bounds-checked and garbage-collected.

The languages with a garbage collector and checked indices do not have these errors in their own code. Their runtimes and native extensions are written in C or C++, and those are built and tested with ASan.

## Variations and related ideas

ASan is one of a family of sanitizers in Clang and GCC. Each one has its own shadow encoding and its own run-time library.

- **UndefinedBehaviorSanitizer (UBSan)** finds undefined behavior such as signed [[integer-overflow]], misaligned and null pointers, shifts out of range and array indices out of bounds. Clang says that it has a small run-time cost and no effect on the address space layout or the ABI.[^5] It can be in the same build as ASan, as in the fixed C example (`-fsanitize=address,undefined`).
- **ThreadSanitizer (TSan)** finds [[data-races]]. Clang gives a typical slowdown of 5x to 15x and a memory overhead of 5x to 10x.[^6] It cannot be in the same build as ASan.[^3]
- **MemorySanitizer (MSan)** finds reads of uninitialized memory. Clang gives a typical slowdown of 3x. It needs all the code of the program, the C library included, to be instrumented, and it runs on Linux, NetBSD and FreeBSD only.[^7]
- **LeakSanitizer (LSan)** finds [[memory-leaks]]. It runs at the end of the process and reports blocks that no pointer reaches. It is part of ASan, and it also runs alone with `-fsanitize=leak`.[^8] Clang turns it on by default with ASan on Linux.[^2] The Apple toolchain that recorded the examples does not support it.
- **Manual poisoning.** A program can poison its own memory through `<sanitizer/asan_interface.h>`. A pool allocator marks free blocks with `ASAN_POISON_MEMORY_REGION`, so a use of a free block is reported as if the system allocator had freed it. The macros do nothing in a build without ASan.[^11] Container libraries use the same interface to poison the unused capacity of a vector.[^2]
- **Valgrind Memcheck** finds similar errors by binary translation, with no recompilation. The paper reports slowdowns of about 20x for Valgrind and 10x for Dr. Memory on SPEC CPU2006, against 73% for ASan. These tools also find uninitialized reads and leaks, but they miss most overflows of stack and global arrays.[^1]

## Pitfalls

### A clean run does not prove the code is correct

ASan reports only the errors that happen in the run. A buffer overflow on a path that no test takes stays hidden. A far out-of-bounds access can land in another valid block and pass.[^1] Use ASan with tests that reach the edge cases, and with a fuzzer that generates inputs.

### Mixing sanitizers in one build

`-fsanitize=address` and `-fsanitize=thread` cannot be combined, and GCC says the same for `thread` and `leak`.[^3] A project that wants both runs its tests twice, once with ASan and UBSan, and once with TSan. The C example file shows a data race that only the TSan build reports.

### Shipping an ASan build

ASan replaces the allocator, maps a large shadow region and keeps freed memory in a quarantine. These are test features. The Clang documentation lists the extra memory, up to 3 times more stack, and more than 16 terabytes of mapped virtual address space as limits.[^2] Ship the build without the sanitizer.

### Errors in code that ASan does not see

Memory that a custom allocator hands out from one large `malloc` block has no redzones between the small objects. An overflow from one object into the next is not reported. The allocator must poison its free blocks and gaps by hand.[^11]

## Cost

The paper measured SPEC CPU2006 with Clang `-O2`:[^1]

- **Time.** The average slowdown was 73%. The largest were 2.60x for perlbench and 2.67x for xalancbmk. With only writes checked, the average slowdown was 26%.
- **Memory.** The average memory use grew 3.37 times, mostly because of the heap redzones. The quarantine adds a fixed amount on top.
- **Defaults.** The paper's run-time defaults were a stack depth of 30 frames for `malloc` and `free`, a 256 MB quarantine and 128-byte heap redzones.

The Clang documentation gives a typical slowdown of 2x.[^2] The numbers differ because the workloads and the settings differ. A larger quarantine finds more uses after free and costs memory, not time.[^1]

## History

- **2011.** The AddressSanitizer team released the tool in May 2011. In the first 10 months it found over 300 bugs in Chromium: 210 heap uses after free, 73 heap buffer overflows, 8 global buffer overflows, 7 stack buffer overflows and 1 overlap of `memcpy` arguments.[^1]
- **2012.** Serebryany, Bruening, Potapenko and Vyukov published the design at USENIX ATC 2012. The paper describes the shadow encoding, the allocator with redzones and quarantine, and the measurements above.[^1]

## References
