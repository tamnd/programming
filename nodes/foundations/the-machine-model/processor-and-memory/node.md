---
tag: EY9XHF
kind: concept
slug: processor-and-memory
name: The machine model, processor and memory
aliases: [von Neumann architecture, stored-program computer, von Neumann machine, instruction cycle, fetch decode execute, program counter, memory hierarchy, von Neumann bottleneck, word size]
area: foundations/the-machine-model
level: core
summary: >-
  A computer runs a program with a processor that fetches, decodes and executes instructions from a memory of addressed words. The program and its data share that memory.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1945
  people: [John von Neumann, J. Presper Eckert, John Mauchly, Alan Turing]
  note: The 1945 First Draft of a Report on the EDVAC described a computer that keeps its program and its data in one memory, and the Manchester Baby ran the first stored program in 1948.
see_also: [bits-and-bytes, call-stack-and-heap, compilers-and-interpreters, bytecode, characters-and-code-points, integer-overflow, functional-programming]
same_as:
  wikidata: Q189088
  wikipedia: Von_Neumann_architecture
sources:
- url: https://www.cs.cmu.edu/~crary/819-f09/Backus78.pdf
  title: John Backus, Can Programming Be Liberated from the von Neumann Style? A Functional Style and Its Algebra of Programs, Communications of the ACM, 1978
  locator: Section 3, von Neumann Computers
- url: https://en.cppreference.com/w/cpp/language/memory_model.html
  title: Memory model, cppreference.com
  locator: Byte and Memory location
- url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-2.html
  title: Chapter 2, The Structure of the Java Virtual Machine, The Java Virtual Machine Specification, Java SE 25 Edition
  locator: 2.5.1 The pc Register
- url: https://people.freebsd.org/~lstewart/articles/cpumemory.pdf
  title: Ulrich Drepper, What Every Programmer Should Know About Memory, 2007
  locator: 1 Introduction and 3 CPU Caches
- url: https://doc.rust-lang.org/reference/type-layout.html
  title: Type layout, The Rust Reference
  locator: Size and Alignment
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number
  title: Number, MDN Web Docs
  locator: Description
---

## Idea

A computer that runs a program has a processor and a memory. The memory is a long row of numbered cells, and the number of a cell is its address. The processor has a few registers, a program counter that holds the address of the next instruction, and circuits that carry out a small set of instructions: load a word from memory, add, compare, store a word back, and jump to another address. It runs a loop: fetch the instruction at the program counter, decode it, execute it, and move to the next one. The program is itself a set of numbers in the same memory as the data, which is the stored-program idea of the late 1940s. Every high-level language, however far it hides it, finally runs as such a loop.

```python run id=idea
HALT, LOAD, ADD, SUB, STORE, JZ, JMP = range(7)
NAMES = ["HALT", "LOAD", "ADD", "SUB", "STORE", "JZ", "JMP"]

# One memory holds the program (cells 0 to 19) and the data (20 to 22).
mem = [LOAD, 20, JZ, 18, LOAD, 21, ADD, 20, STORE, 21,
       LOAD, 20, SUB, 22, STORE, 20, JMP, 0, HALT, 0,
       3, 0, 1]  # n = 3, sum = 0, the constant 1

pc, acc = 0, 0  # the two registers of the processor
steps = fetches = loads = stores = 0
while True:
    op, addr = mem[pc], mem[pc + 1]  # fetch: two words come from memory
    fetches += 2
    if steps < 6:
        print(f"pc={pc:2} acc={acc:2}  {NAMES[op]} {addr}")
    pc += 2
    steps += 1
    if op == HALT:  # decode and execute
        break
    elif op == LOAD:
        acc = mem[addr]; loads += 1
    elif op == ADD:
        acc += mem[addr]; loads += 1
    elif op == SUB:
        acc -= mem[addr]; loads += 1
    elif op == STORE:
        mem[addr] = acc; stores += 1
    elif op == JZ and acc == 0:
        pc = addr
    elif op == JMP:
        pc = addr

print("...")
print(f"sum={mem[21]} steps={steps}")
print(f"memory traffic: {fetches} words of instructions, {loads + stores} words of data")
```

```output for=idea
pc= 0 acc= 0  LOAD 20
pc= 2 acc= 3  JZ 18
pc= 4 acc= 3  LOAD 21
pc= 6 acc= 0  ADD 20
pc= 8 acc= 3  STORE 21
pc=10 acc= 3  LOAD 20
...
sum=6 steps=30
memory traffic: 60 words of instructions, 22 words of data
```

The machine has one accumulator register, a program counter, and 23 cells of memory. Cells 0 to 19 hold the program as pairs of numbers, an operation and an address, and cells 20 to 22 hold the data: a counter `n`, a running sum and the constant 1. The program adds `n` to the sum, subtracts 1 from `n`, and jumps back until `n` is 0. The trace shows the first six instructions. The result is 6 after 30 instructions, and the counts at the end show where the time goes: the processor read 60 words of instructions and moved only 22 words of data.

## How it works

**Processor and memory.** John Backus described the model in his 1977 Turing Award lecture: "In its simplest form a von Neumann computer has three parts: a central processing unit (or CPU), a store, and a connecting tube that can transmit a single word between the CPU and the store (and send an address to the store)."[^1] The C++ standard gives programs the same view of memory: "The memory available to a C++ program is one or more contiguous sequences of bytes. Each byte in memory has a unique address."[^2] A word is the unit that the processor handles in one step, 64 bits on most processors today, and the width of an address limits how much memory a program can reach.

**The instruction cycle.** The processor repeats one cycle. It fetches the instruction at the address in the program counter, decodes the operation and its operands, executes it, and advances the program counter, unless the instruction was a jump that set the program counter to another address. A conditional jump, which jumps only if a register holds zero or a comparison came out a certain way, is enough to build `if` statements and loops. Function calls are jumps that save the return address, usually on the call stack; see [the call stack and heap](/call-stack-and-heap/).

**The stored program.** Instructions are numbers in memory, so a program can be loaded, copied, generated and changed like any other data. Loaders, compilers, linkers and JIT compilers all depend on this. Abstract machines follow the same plan. The Java Virtual Machine specification says: "Each Java Virtual Machine thread has its own pc (program counter) register."[^3] It holds the address of the JVM instruction that the thread is executing. Bytecode interpreters for Python, Java and .NET run the same fetch, decode and execute loop in software; see [compilers and interpreters](/compilers-and-interpreters/).

**The bottleneck.** Every instruction and every operand crosses the connection between processor and memory. Backus named it: "I propose to call this tube the von Neumann bottleneck."[^1] He noted that much of the traffic is not data at all: "Ironically, a large part of the traffic in the bottleneck is not useful data but merely names of data, as well as operations and data used only to compute such names."[^1] The counts at the end of the idea program show the same effect: most of the words that moved were instructions. Backus used the point to argue for programming with whole values, as in [functional programming](/functional-programming/), instead of one word at a time.

**The memory hierarchy.** Processors became faster much more quickly than memory. Ulrich Drepper describes how the components of a computer stopped being balanced, and says this "was especially true for mass storage and memory subsystems which, for cost reasons, improved more slowly relative to other components."[^4] Modern machines answer with a hierarchy: registers, then several levels of small, fast cache, then main memory, then disk. Caches work because programs have locality: "over short periods of time, there is a good chance that the same code or data gets reused."[^4] A cache moves memory in lines, usually of 64 bytes, so reading one word also brings its neighbours, and code that walks an array in order runs much faster than code that jumps around memory.

**Words, sizes and layout.** Languages map their types onto the words of the machine. C, C++, Go and Rust expose the sizes and the alignment of types, and a compiler inserts padding so that each field starts at an address that is a multiple of its alignment. The Rust reference defines the size of a value as "the offset in bytes between successive elements in an array with that item type including alignment padding."[^5] Higher-level languages hide the words. JavaScript has one number type for all numeric values: "The JavaScript Number type is a double-precision 64-bit binary format IEEE 754 value".[^6] It cannot hold every 64-bit integer exactly, as the wrong block of the JavaScript file shows; see [bits, bytes and bitwise operations](/bits-and-bytes/).

**What the simple model leaves out.** A real processor keeps the meaning of the cycle but not its order. It runs several instructions at once in a pipeline, executes them out of order when their inputs are ready, predicts the direction of branches before it knows them, and has several cores that each run their own cycle on a shared memory. Compilers also reorder and remove instructions. The language guarantees that a single thread sees the effects in program order; for several threads, a memory model states which orders are allowed.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `ctypes`, `struct`, `memoryview`; no addresses in the language |
| JavaScript | idiom | `ArrayBuffer` and typed arrays; no addresses |
| TypeScript | idiom | as JavaScript |
| Java | stdlib | fixed-width types; the Foreign Function and Memory API |
| Kotlin | stdlib | as Java on the JVM; pointers in Kotlin/Native |
| C# | native | fixed-width types; `unsafe` pointers; `sizeof`; `Span<T>` |
| Go | native | pointers; `unsafe.Sizeof`; `unsafe.Pointer` |
| Rust | native | `size_of`, `align_of`; raw pointers in `unsafe` code |
| C | native | pointers, `sizeof`, `offsetof`; memory as bytes |
| C++ | native | the memory model of C; `alignof`; `std::array` |
| Bash | n/a | no view of memory |
| PowerShell | idiom | .NET types for sizes and byte order |
| SQL | n/a | the database chooses how to compute |
| Regex | n/a | no model of execution |
| HTML | n/a | no model of execution |
| CSS | n/a | no model of execution |

Every example file builds the same stored-program machine: an array of words that holds both the program and the data, a program counter, an accumulator, and a loop that fetches, decodes and executes. Each runs the program for n = 5 and gets a sum of 15 after 48 instructions. The second line of output shows what the language tells a program about the real machine: the size of an address, the size of a struct with padding, or the widths of the integer types.

The table measures how much of the machine a language lets a program see. C, C++, Rust, Go and C# expose addresses, sizes and layout, with unsafe operations for raw memory, and C does not check indices at all. Java and Python hide addresses but give standard libraries for raw memory when a program must talk to hardware or to code in C. JavaScript gives raw bytes in buffers and nothing more. The languages that hide the most protect a program from reading memory that it does not own, and pay for it with checks and with less control over layout.

## Variations and related ideas

- **Harvard architecture.** Separate memories and buses for instructions and data, as in many microcontrollers and in the split instruction and data caches of modern processors.
- **Stack machines and register machines.** The JVM, CPython and WebAssembly use an operand stack in place of named registers; physical processors and the Lua and Dalvik virtual machines use registers. See [bytecode](/bytecode/).
- **The random-access machine.** The model of algorithm analysis: a machine that reads or writes any cell in constant time. It ignores caches, which is why two algorithms with the same complexity can differ greatly in speed.
- **Caches and locality.** Data layout that keeps related values together, such as arrays of structs or structs of arrays, matters as much as the instruction count.
- **Parallel and vector hardware.** SIMD instructions apply one operation to several words, and GPUs run thousands of threads. See [array-oriented programming](/array-oriented-programming/).
- **Turing machines.** The theoretical model of computation, with an unbounded tape in place of a finite memory of addressed cells.

## Pitfalls

- **Assuming sizes.** A `long` is 8 bytes on 64-bit Linux and macOS and 4 bytes on 64-bit Windows, and a pointer is 4 bytes on 32-bit systems. Use fixed-width types such as `int64_t` and `size_t`, and `sizeof`, instead of numbers.
- **Ignoring padding.** A struct is often larger than the sum of its fields, and writing it to a file or a network copies the padding and the byte order of the machine. Serialize field by field. The C and Rust files show the padding.
- **Reading past the end.** In C and C++ with `[]`, an index outside an array reads or writes other memory, which is undefined behavior and a common source of security flaws. The fails block of the Rust file shows a checked language stopping the read.
- **Numbers that are not words.** JavaScript numbers lose integer precision above 2 to the power 53, as the wrong block of the JavaScript file shows. Use `BigInt` for 64-bit values such as IDs from other systems.
- **Code that fights the cache.** Walking a large matrix by columns when it is stored by rows, or following long chains of pointers, makes each access a cache miss. Store data in the order that the program reads it.
- **Assuming the order of instructions.** On several threads, writes from one thread can become visible to another in a different order than the program states. Use the synchronization of the language, not the order of the source text.

## Cost

Registers are the fastest storage, and each level of the memory hierarchy is larger and slower than the one before. Drepper gives the access times that Intel lists for a Pentium M: at most 1 cycle for a register, about 3 cycles for the level 1 data cache, about 14 for the level 2 cache, and about 240 for main memory.[^4] Current processors have similar ratios, so a program that misses the cache on each access can run a hundred times slower than one that hits it, with the same number of instructions. A simple instruction such as an add takes one cycle or less, because a processor runs several in parallel, while a mispredicted branch costs 10 to 20 cycles. A software interpreter of the kind in the examples spends several machine instructions to fetch and decode each of its own, which is why bytecode interpreters run many times slower than native code.

## History

Alan Turing described a universal machine that reads its program from its tape in 1936. In 1945, John von Neumann's *First Draft of a Report on the EDVAC*, which drew on the work of J. Presper Eckert and John Mauchly, described a computer that keeps its program and its data in one memory, and Turing's design for the ACE followed later that year. The Manchester Baby ran the first stored program in June 1948, and EDSAC at Cambridge began regular work in May 1949. The Harvard Mark I, from 1944, had read its instructions from punched tape, separate from its data, which gave the Harvard architecture its name.

The IBM System/360, announced in 1964, defined an instruction set as an interface that many machines of different speeds could share, and its Model 85 of 1968 was the first commercial computer with a cache. John Backus named the von Neumann bottleneck in his Turing Award lecture of 1977, published in 1978.[^1] RISC designs from Berkeley and Stanford in the early 1980s simplified the instruction set so that the cycle could run faster, and in the 2000s, when clock speeds stopped rising, processors added more cores. Ulrich Drepper's 2007 paper *What Every Programmer Should Know About Memory* explained the memory hierarchy for programmers.[^4]

## References
