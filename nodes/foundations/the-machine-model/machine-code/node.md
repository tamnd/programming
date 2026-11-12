---
tag: F8ZJ8N
kind: concept
slug: machine-code
name: Machine code
aliases: [machine code, machine language, native code, object code, binary code, instruction set, instruction set architecture, ISA, opcode, instruction encoding, assembly language]
area: foundations/the-machine-model
level: core
summary: >-
  Machine code is a program as the processor runs it: instructions encoded as bytes in memory. Each family of processors has its own instruction set, so machine code for one does not run on another.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1949
  people: [David Wheeler, Maurice Wilkes, Kathleen Booth, Gene Amdahl, Sophie Wilson, Steve Furber, Krste Asanović]
  note: The stored-program computers of 1948 and 1949 ran instructions from memory, and assemblers wrote them from text. IBM's System/360 of 1964 kept one instruction set across a family of machines, and x86 from 1978, ARM from 1985 and RISC-V from 2010 are the main instruction sets today.
see_also: [processor-and-memory, memory-address, source-code, compilers-and-interpreters, bytecode, just-in-time-compilation, bits-and-bytes, computer-program]
same_as:
  wikidata: Q55813
  wikipedia: Machine_code
sources:
- url: https://webassembly.github.io/spec/core/intro/introduction.html
  title: Introduction, WebAssembly Specification
  locator: Introduction
- url: https://webassembly.github.io/spec/core/binary/modules.html
  title: Modules, Binary Format, WebAssembly Specification
  locator: Binary Module
- url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-4.html
  title: Chapter 4, The class File Format, The Java Virtual Machine Specification, Java SE 25
  locator: 4.1 The ClassFile Structure
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/classfile/package-summary.html
  title: Package java.lang.classfile, Java SE 25 API documentation
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.reflection.emit.dynamicmethod
  title: DynamicMethod Class, .NET API documentation
  locator: Definition
- url: https://go.dev/doc/asm
  title: A Quick Guide to Go's Assembler, The Go Programming Language
  locator: Introduction
- url: https://doc.rust-lang.org/reference/inline-assembly.html
  title: Inline assembly, The Rust Reference
  locator: asm.intro
- url: https://gcc.gnu.org/onlinedocs/gcc/Basic-Asm.html
  title: Basic Asm, Using the GNU Compiler Collection
  locator: 6.11.1
---

## Idea

A processor does not read source code. It reads instructions from memory, each one a pattern of bits that says what to do: add two registers, load a word from an address, jump to another instruction. A program in this form is machine code. The set of instructions and their encoding is the instruction set architecture of the processor, and it differs between families: machine code for ARM64 means nothing to an x86-64 processor. Compilers write machine code from source code, and assemblers write it from a text form, assembly language, in which each line names one instruction.

```python run id=idea
# A tiny assembler and simulator for three ARM64 instructions.
def movz(rd, imm16):
    return 0xD2800000 | imm16 << 5 | rd


def mul(rd, rn, rm):
    return 0x9B007C00 | rm << 16 | rn << 5 | rd


RET = 0xD65F03C0

program = [movz(0, 6), movz(1, 7), mul(0, 0, 1), RET]
code = b"".join(w.to_bytes(4, "little") for w in program)
print("words:", " ".join(f"{w:08x}" for w in program))
print("bytes:", code.hex(" "))


def run(code):
    x = [0] * 32
    pc = 0
    while True:
        w = int.from_bytes(code[pc:pc + 4], "little")  # fetch
        pc += 4
        rd, rn, rm = w & 31, w >> 5 & 31, w >> 16 & 31  # decode the register fields
        if w & 0xFF800000 == 0xD2800000:  # execute
            x[rd] = w >> 5 & 0xFFFF
        elif w & 0xFFE0FC00 == 0x9B007C00:
            x[rd] = x[rn] * x[rm] % 2**64
        elif w == RET:
            return x[0]
        else:
            raise ValueError(f"unknown instruction {w:08x}")


print("x0 =", run(code))
```

```output for=idea
words: d28000c0 d28000e1 9b017c00 d65f03c0
bytes: c0 00 80 d2 e1 00 80 d2 00 7c 01 9b c0 03 5f d6
x0 = 42
```

The program is an assembler and a processor in a few lines. Each ARM64 instruction is one 32-bit word with fields at fixed places: the low 5 bits name the destination register, and other bits hold a constant or two more register numbers. The words are stored low byte first, so `movz x0, #6` is the word `d28000c0` and the bytes `c0 00 80 d2`. The simulator repeats the cycle of every processor: fetch the next word, decode its fields, execute it. The words are the same ones that the system assembler gives for the same four lines.

## How it works

**Instructions and their encoding.** An instruction set defines the registers, the instructions and the bits of each one. An instruction has an opcode, which says which operation, and operands: register numbers, a constant, or a way to find a memory address. ARM64 and RISC-V use fixed-length words of 4 bytes, so the decoder knows where each instruction starts; RISC-V adds an optional 2-byte form to make code smaller. x86-64 instructions take from 1 to 15 bytes, with optional prefixes, an opcode, a byte that names registers and modes, and constants, so a decoder must read each instruction to find the next one.

**Running instructions.** The processor keeps the address of the next instruction in the program counter. It fetches the instruction there, decodes it and executes it, and the program counter moves to the next instruction unless the instruction is a jump, a branch or a call. Real processors overlap these steps for many instructions at once and run them out of order, but the result must be the same as one at a time. See [processor and memory](/processor-and-memory/).

**Conventions around the code.** Machine code alone does not say how functions call each other. An application binary interface, or ABI, sets the rules: which registers hold the arguments and the result, which registers a function must keep, and how the stack is laid out. On ARM64 a function returns its result in `x0`, and on x86-64 Linux and macOS in `rax`, which is why the C example puts 42 in those registers. Object files and executables, in the formats ELF, Mach-O and PE, hold the machine code with the names, addresses and relocations that the linker and the loader need.

**From source to machine code.** A compiler such as GCC, Clang, `rustc` or the Go compiler translates source code to machine code before the program runs. Most compilers go through an intermediate form, and the Go toolchain's assembler "is not a direct representation of the underlying machine": it works on "a kind of semi-abstract instruction set", and the final instructions are chosen later.[^6] C, C++ and Rust allow assembly inside a function: GCC has `asm` statements,[^8] and in Rust, inline assembly "can be used to embed handwritten assembly in the assembly output generated by the compiler".[^7]

**Virtual machine code.** Many languages compile to the instructions of a machine that does not exist as hardware. A Java class file holds bytecode for the JVM, and the specification says of its first four bytes, the magic number, that "it has the value 0xCAFEBABE".[^3] The package `java.lang.classfile` "Provides classfile parsing, generation, and transformation library", so a program can write bytecode and load it.[^4] .NET assemblies hold the Common Intermediate Language, and a `DynamicMethod` lets a program write its own IL: it "Defines and represents a dynamic method that can be compiled, executed, and discarded".[^5] WebAssembly is "a safe, portable, low-level code format designed for efficient execution and compact representation".[^1] Its binary module "starts with a preamble containing a 4-byte magic number", the bytes `00 61 73 6d`, and a version.[^2] A just-in-time compiler turns these instructions into real machine code while the program runs. See [bytecode](/bytecode/) and [just-in-time compilation](/just-in-time-compilation/).

**Code is data in memory.** Machine code is bytes in memory like any other data, so a program can write instructions and then run them. Just-in-time compilers do this all the time. Operating systems protect against attacks that inject code by a rule called W^X: a page of memory is writable or executable, not both at the same time. A JIT asks for a special mapping and switches the page between the two states, as the C example does with `MAP_JIT` on macOS.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | absent | CPython runs bytecode; machine code only through C extensions or `ctypes` |
| JavaScript | absent | the engine compiles to machine code; WebAssembly bytes from the program |
| TypeScript | absent | as JavaScript |
| Java | absent | bytecode in class files; the JIT makes machine code; `java.lang.classfile` |
| Kotlin | absent | JVM bytecode; Kotlin/Native compiles to machine code |
| C# | absent | CIL in assemblies; the JIT or Native AOT makes machine code; `Reflection.Emit` |
| Go | native | the compiler writes machine code; assembly files in the Plan 9 syntax |
| Rust | native | the compiler writes machine code; `asm!`, `global_asm!` |
| C | native | the compiler writes machine code; `asm` in GCC and Clang |
| C++ | native | as C |
| Bash | absent | the shell interprets commands and runs other programs |
| PowerShell | absent | runs on .NET, which makes the machine code |
| SQL | n/a | the database decides how to run a query |
| Regex | n/a | some engines compile patterns to machine code, but the pattern cannot |
| HTML | n/a | no instructions |
| CSS | n/a | no instructions |

Go, Rust, C and C++ compile to machine code before the program runs, and Rust, C and C++ let the programmer write instructions inside a function, so the verdict for them is native. Java, C# and JavaScript run on virtual machines that make machine code with a just-in-time compiler, and the program sees only the virtual instructions: JVM bytecode, CIL or WebAssembly. CPython interprets its own bytecode. For these languages the verdict is absent, because the program cannot write or see the machine code in the normal way.

The example files show machine code at the level that each language can reach. Python decodes x86-64 instructions of different lengths. JavaScript writes a WebAssembly module byte by byte and runs it. Java writes and runs JVM bytecode with the class file API, and C# writes CIL with `Reflection.Emit`. Go runs RISC-V machine code in a small emulator. Rust uses inline assembly, C copies the bytes of a function into executable memory and calls them, and C++ checks a small ARM64 assembler at compile time.

## Variations and related ideas

- **Assembly language.** A text form of machine code with one line for each instruction, names for registers and labels for addresses. An assembler turns it into machine code, and a disassembler turns machine code back into it.
- **Microcode.** Many processors run complex instructions as sequences of simpler internal operations, which the vendor can update.
- **RISC and CISC.** Reduced instruction set designs, such as ARM and RISC-V, use simple instructions of one length that load and store memory separately. Complex instruction set designs, such as x86, have instructions of many lengths that can work on memory directly.
- **Position-independent code.** Code that uses addresses relative to the program counter can run at any address, which shared libraries and address space layout randomization need.
- **Fat binaries and emulation.** A universal binary holds machine code for two instruction sets, and translators such as Rosetta 2 turn x86-64 code into ARM64 code.
- **Bytecode.** [Bytecode](/bytecode/) is machine code for a virtual machine, designed to be easy to interpret or compile.

## Pitfalls

- **Machine code is not portable.** A binary built for x86-64 Linux does not run on ARM64 macOS, because the instruction set, the ABI and the file format all differ. Build for each target, or use a portable form such as bytecode or WebAssembly.
- **Writing code into memory.** A program that writes and runs machine code must follow the W^X rules of the system and flush the instruction cache on ARM, or the processor runs old bytes.
- **Inline assembly that lies to the compiler.** The compiler trusts the list of registers and memory that an `asm` block says it uses. A block that changes other registers corrupts the code around it.
- **Code injection.** An attacker who can write bytes and make the program jump to them controls the program. W^X, stack canaries, address space layout randomization and control-flow integrity make this harder.
- **Undefined instructions and features.** An instruction that the processor does not support stops the program. On x86, `lzcnt` on an older processor runs as `bsr` and gives a different result, so code must check the processor features first.

## Cost

Machine code is the fastest form of a program, because the processor runs it with no interpreter. An instruction such as an add takes about one cycle, and a modern processor can start several each cycle. The cost of a program is mostly in branches that the processor predicts wrong, which cost about 15 to 20 cycles each, and in loads that miss the caches, which can cost hundreds. Interpreting bytecode is often several times to tens of times slower than running compiled machine code, which is why virtual machines use just-in-time compilers. The size of the code matters too: code that does not fit in the instruction cache runs slower.

## History

The stored-program computers of 1948 and 1949, such as the Manchester Baby and the EDSAC, ran instructions that were stored in memory as numbers. Programmers first wrote the bits by hand. Kathleen Booth described an assembly notation in 1947, and David Wheeler's initial orders for the EDSAC, in 1949, read instructions as letters and decimal numbers and stored them as binary, an early assembler in daily use. Maurice Wilkes described microprogramming in 1951.

IBM's System/360, from 1964, with Gene Amdahl as one of its architects, separated the instruction set from the hardware: one set of machine code ran on a family of machines of different sizes. Intel's 8086 of 1978 started the x86 family, which AMD extended to 64 bits in 2003. Sophie Wilson and Steve Furber designed the first ARM processor in 1985, and ARM64 followed in 2011. RISC-V, started at Berkeley in 2010 by Krste Asanović and others, is an open instruction set. Java's virtual machine of 1995 and WebAssembly, which shipped in the major browsers in 2017, made portable instruction sets for code that runs on any processor.

## References
