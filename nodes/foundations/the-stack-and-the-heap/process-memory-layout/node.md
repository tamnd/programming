---
tag: T638CX
kind: concept
slug: process-memory-layout
name: Memory layout of a process
aliases: [memory layout of a program, memory segments, text segment, data segment, bss segment, address space layout]
area: foundations/the-stack-and-the-heap
level: core
summary: >-
  A running process sees its memory as regions: code, data, bss, heap, stack and mapped files. Each region has its own use and its own permissions.
applies_to: core-languages
viz: optional
formal: none
see_also: [stack-and-heap, call-stack, stack-frame, stack-overflow-error, memory-address, processor-and-memory, explicit-memory-management, process]
same_as:
  wikidata: Q491179
  wikipedia: Data_segment
sources:
- url: https://man7.org/linux/man-pages/man5/proc_pid_maps.5.html
  title: proc_pid_maps(5), Linux manual page
  locator: DESCRIPTION
- url: https://en.wikipedia.org/wiki/Code_segment
  title: Code segment, Wikipedia
  locator: Introduction; Segment
- url: https://en.wikipedia.org/wiki/Data_segment
  title: Data segment, Wikipedia
  locator: Introduction; Program memory; Code; Data; BSS; Heap; Stack
- url: https://en.wikipedia.org/wiki/.bss
  title: .bss, Wikipedia
  locator: Introduction; Origin
- url: https://man7.org/linux/man-pages/man5/elf.5.html
  title: elf(5), Linux manual page
  locator: Section header, special sections (.bss, .data, .rodata, .text)
- url: https://man7.org/linux/man-pages/man2/brk.2.html
  title: brk(2), Linux manual page
  locator: DESCRIPTION; NOTES
- url: https://man7.org/linux/man-pages/man3/malloc.3.html
  title: malloc(3), Linux manual page
  locator: NOTES
- url: https://man7.org/linux/man-pages/man2/mmap.2.html
  title: mmap(2), Linux manual page
  locator: DESCRIPTION; MAP_ANONYMOUS
- url: https://en.wikipedia.org/wiki/Memory-mapped_file
  title: Memory-mapped file, Wikipedia
  locator: Introduction
- url: https://en.wikipedia.org/wiki/Address_space_layout_randomization
  title: Address space layout randomization, Wikipedia
  locator: Introduction; History; Linux
---

## Idea

A running program does not see one flat block of memory. Its address space is split into regions, and each region holds one kind of thing: the machine code, the global variables, the heap, the [call stack](/call-stack/), and files and libraries mapped into memory. Each region has its own permissions, so the code can be run but not written, and the data can be written but not run. On Linux, a program can list its own regions in the file `/proc/self/maps`.

```python run id=idea
def regions():
    """Yield (start, perms, path) for each region in /proc/self/maps."""
    with open("/proc/self/maps") as f:
        for line in f:
            fields = line.split(maxsplit=5)
            start = int(fields[0].split("-")[0], 16)
            path = fields[5].strip() if len(fields) > 5 else ""
            yield start, fields[1], path


found = {}
for start, perms, path in regions():
    if path in ("[heap]", "[stack]", "[vdso]") and path not in found:
        found[path] = start
        print(path, perms)

print("the heap is below the stack:", str(found["[heap]"] < found["[stack]"]).lower())
```

```output for=idea
[heap] rw-p
[stack] rw-p
[vdso] r-xp
the heap is below the stack: true
```

The heap and the stack can be read and written (`rw`) but not run. The `[vdso]` region is "The virtual dynamically linked shared object"[^1], and it can be run (`x`) but not written. The heap is at a lower address than the stack. The addresses themselves change from run to run, so the program prints only the order.

## How it works

**Code.** The code segment, "also known as a text segment or simply as text", is the part of the address space "that contains executable instructions"[^2]. "The code segment in memory is typically read-only and has a fixed size"[^2].

**Data and read-only data.** The data segment "contains initialized static variables, that is, global variables and static local variables"[^3]. "The data segment is read/write"[^3]. Constants go in a separate read-only data segment, `.rodata`, "which contains static constants rather than variables"[^3].

**BSS.** Static variables with no initial value go in the bss segment. "Typically only the length of the bss section, but no data, is stored in the object file"[^4]. The ELF format says that `.bss` "holds uninitialized data that contributes to the program's memory image. By definition, the system initializes the data with zeros when the program begins to run"[^5].

**Heap.** "The heap segment contains dynamically allocated memory, commonly begins at the end of the BSS segment and grows to larger addresses from there"[^3]. The end of this region is the program break: `brk` and `sbrk` "change the location of the program break, which defines the end of the process's data segment"[^6]. A program rarely calls them itself. "Normally, malloc() allocates memory from the heap, and adjusts the size of the heap as required, using sbrk(2)"[^7].

**Stack.** "The stack segment contains the call stack, a LIFO structure, typically located in the higher parts of memory"[^3]. "On the standard PC x86 architecture the stack grows toward address zero"[^3], so a deeper call has a lower address. "The stack segment traditionally adjoined the heap segment and they grew towards each other"[^3]. With virtual memory, the two regions "tend to be placed more freely, but they still typically grow in a converging direction"[^3].

**Mapped regions.** Not all memory comes from the heap. `mmap` makes a mapping "in the virtual address space of the calling process"[^8]. A mapping can show a file in memory: a memory-mapped file "has been assigned a direct byte-for-byte correspondence with some portion of a file or file-like resource"[^9]. The example in the maps man page shows the C library file, `libc-2.15.so`, mapped in this way[^1]. An anonymous mapping "is not backed by any file; its contents are initialized to zero"[^8]. The C library uses anonymous mappings for large blocks: "When allocating blocks of memory larger than MMAP_THRESHOLD bytes, the glibc malloc() implementation allocates the memory as a private anonymous mapping using mmap(2)"[^7].

**How to see it.** On Linux, `/proc/pid/maps` lists the "mapped memory regions and their access permissions"[^1]. Each line has an address range, the permissions, an offset, a device, an inode and a path[^1]. The permissions are "r = read w = write x = execute s = shared p = private (copy on write)"[^1]. Some paths are names, not files: `[stack]` is "The initial process's (also known as the main thread's) stack", and `[heap]` is "The process's heap"[^1]. "If the pathname field is blank, this is an anonymous mapping as obtained via mmap(2)"[^1]. An inode of 0 means that no file is behind the region, "as would be the case with BSS (uninitialized data)"[^1].

## In languages

| Language | Verdict | What the program can see of the layout |
|---|---|---|
| Python | native | no addresses; the program can read `/proc/self/maps` as a file |
| JavaScript | native | no addresses; Node.js can read `/proc/self/maps` as a file |
| TypeScript | native | as in JavaScript |
| Java | native | no addresses; the program can read `/proc/self/maps` as a file |
| Kotlin | native | as in Java |
| C# | native | `unsafe` code can take the address of a local or a field |
| Go | native | `unsafe.Pointer` turns a pointer into an address |
| Rust | native | a reference converts to a raw pointer and then to an address |
| C | native | `&` gives the address of a function, a global or a local |
| C++ | native | as in C |
| Bash | native | `/proc/$$/maps` lists the regions of the shell |
| PowerShell | native | runs on .NET, in a process with the same regions |
| SQL | n/a | a query does not see memory |
| Regex | n/a | a pattern does not see memory |
| HTML | n/a | a document does not see memory |
| CSS | n/a | a style sheet does not see memory |

The example files read `/proc/self/maps` in each language. Python, JavaScript, TypeScript, Java, C# and Bash print the heap, stack and vdso regions. C, C++ and Rust also find the region of a function, a global, a local, a small heap block and a large heap block. Go does the same, but its heap and its goroutine stacks are in anonymous mappings, not in `[heap]` and `[stack]`.

## Variations and related ideas

- **Address space layout randomization.** "ASLR randomly arranges the address space positions of key data areas of a process, including the base of the executable and the positions of the stack, heap and libraries"[^10]. This makes it harder for an attack to guess an address[^10]. The order of the regions stays the same, but the addresses change on each run.
- **Threads.** Each thread has its own stack. Only the stack of the main thread is named `[stack]`[^1]. Some kernels named the stack of a thread `[stack:tid]`, but the man page says that this field was removed, because it is expensive for a process with many threads[^1].
- **A runtime with its own heap.** A runtime can take memory with `mmap` and keep its own heap there. The Go example shows its heap blocks and its goroutine stack in anonymous mappings, not in `[heap]` and `[stack]`.
- **Segments in hardware.** "The Intel 8086 family of CPUs provided four segments: the code segment, the data segment, the stack segment and the extra segment"[^3]. Wikipedia calls the memory segment "a historical approach to memory management that has been succeeded by paging"[^2].

## Pitfalls

- **Addresses are not stable.** Because of ASLR, the address of a global or a function can change on each run. Do not store an address in a file or compare addresses from two runs.
- **The heap is more than `[heap]`.** A large `malloc` goes to an anonymous mapping, not to the `[heap]` region[^7]. A tool that only measures `[heap]` misses those blocks.
- **Writes to code.** The code segment is read-only on most systems, and "If the code segment is not read-only, then the particular architecture allows self-modifying code"[^2].
- **brk in a program.** The brk man page says "Avoid using brk() and sbrk(): the malloc(3) memory allocation package is the portable and comfortable way of allocating memory"[^6].

## Cost

The bss segment saves space in the file: "By placing variables with no value in the .bss section, instead of the .data or .rodata section which require initial value data, the size of the object file is reduced"[^4]. It also costs little at run time, because "Operating systems may use a technique called zero-fill-on-demand to efficiently implement the bss segment"[^4]. The C library uses `mmap` only for blocks larger than `MMAP_THRESHOLD`, which "is 128 kB by default, but is adjustable using mallopt(3)"[^7].

## History

The name bss comes from an assembler of the 1950s: "BSS (from Block Started by Symbol) is a pseudo-operation in UA-SAP (United Aircraft Symbolic Assembly Program), the assembler developed in the mid-1950s for the IBM 704"[^4]. The segments of the Intel 8086 let a 16-bit address register reach more memory: "This allowed a 16-bit address register, which would normally be able to access 64 KB of memory space, to access 1 MB of memory space"[^3]. Address space layout randomization came later. The PaX project "published the first design and implementation of ASLR in July 2001 as a patch for the Linux kernel"[^10], and "The first mainstream operating system to support ASLR by default was OpenBSD version 3.4 in 2003"[^10].
