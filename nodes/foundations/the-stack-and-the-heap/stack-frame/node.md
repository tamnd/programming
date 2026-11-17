---
tag: SJGB98
kind: concept
slug: stack-frame
name: Stack frame
aliases: [activation record, activation frame, call frame, frame pointer, base pointer]
area: foundations/the-stack-and-the-heap
level: core
summary: >-
  A stack frame is the entry on the call stack for one call: its arguments, its local variables, the return address and saved registers. A call makes one, and the return frees it.
applies_to: core-languages
viz: optional
formal: none
see_also: [call-stack, stack-and-heap, stack-overflow-error, process-memory-layout, recursion, closures, memory-address, value-types-and-reference-types]
sources:
- url: https://en.wikipedia.org/wiki/Call_stack
  title: Call stack, Wikipedia
  locator: Structure; Stack and frame pointers; Storing the address to the caller's frame
- url: https://sourceware.org/gdb/current/onlinedocs/gdb.html/Frames.html
  title: Stack Frames, Debugging with GDB
  locator: 8.1 Stack Frames
- url: https://en.wikipedia.org/wiki/Function_prologue_and_epilogue
  title: Function prologue and epilogue, Wikipedia
  locator: Prologue; Epilogue
- url: https://go.googlesource.com/go/+/refs/heads/master/src/cmd/compile/abi-internal.md
  title: Go internal ABI specification
  locator: amd64 architecture, Stack layout
- url: https://learn.microsoft.com/en-us/cpp/build/stack-usage
  title: x64 stack usage, Microsoft C++ documentation
  locator: Stack allocation
- url: https://docs.oracle.com/javase/specs/jvms/se21/html/jvms-2.html
  title: The Structure of the Java Virtual Machine, Java Virtual Machine Specification, Java SE 21
  locator: 2.6 Frames; 2.6.1 Local Variables; 2.6.2 Operand Stacks
- url: https://docs.python.org/3.14/reference/datamodel.html
  title: Data model, Python 3.14 documentation
  locator: 3.2.13.2 Frame objects
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Execution_model
  title: JavaScript execution model, MDN Web Docs
  locator: Stack and execution contexts; Generators and reentry
- url: https://www.gnu.org/software/bash/manual/html_node/Bash-Builtins.html
  title: Bash Builtin Commands, Bash Reference Manual
  locator: local
- url: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
  title: Options That Control Optimization, GCC documentation
  locator: -fomit-frame-pointer
- url: https://en.wikipedia.org/wiki/Tail_call
  title: Tail call, Wikipedia
  locator: Introduction
- url: https://go.dev/doc/faq
  title: Frequently Asked Questions, The Go Programming Language
  locator: How do I know whether a variable is allocated on the heap or the stack?
- url: https://learn.microsoft.com/en-us/archive/blogs/ericlippert/the-stack-is-an-implementation-detail-part-one
  title: Eric Lippert, The Stack Is An Implementation Detail, Part One, Fabulous Adventures In Coding, 2009
  locator: Whole post
- url: https://gcc.gnu.org/onlinedocs/gcc/Developer-Options.html
  title: GCC Developer Options, GCC documentation
  locator: -fstack-usage
- url: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
  title: Options to Request or Suppress Warnings, GCC documentation
  locator: -Wframe-larger-than
---

## Idea

Each call of a function needs its own place for its arguments and its local variables, and it must remember where to go back to. A stack frame is that place. A call puts a frame on top of the [call stack](/call-stack/), and the return removes it. When a function calls itself, each call has its own frame, so each call has its own copy of the locals.

```python run id=idea
import sys


def count(n):
    here = n * 10                    # a local: one copy in each frame
    print(f"enter n={n} here={here}")
    if n > 1:
        count(n - 1)
    else:
        frames = []
        f = sys._getframe()          # the frame of this call
        while f.f_code is count.__code__:
            frames.append(f"n={f.f_locals['n']}")
            f = f.f_back             # the frame of the caller
        print("frames:", " <- ".join(frames))
    print(f"leave n={n} here={here}")


count(3)
```

```output for=idea
enter n=3 here=30
enter n=2 here=20
enter n=1 here=10
frames: n=1 <- n=2 <- n=3
leave n=1 here=10
leave n=2 here=20
leave n=3 here=30
```

At the deepest point, three frames of `count` are on the stack, and each frame holds its own `n` and `here`. When the inner calls return, the outer frames still have their values, so `leave n=3` prints `here=30`.

## How it works

**What a frame holds.** A call stack "is composed of stack frames (also called activation records or activation frames)"[^1]. "Each stack frame corresponds to an invocation of a subroutine that has not yet completed with a return"[^1]. A frame typically holds "the arguments (parameter values) passed to the routine on the call stack (if any)", the return address to the caller, and "space for the local variables of the routine (if any)"[^1]. GDB says the same: "The frame contains the arguments given to the function, the function's local variables, and the address at which the function is executing"[^2]. Not every argument is in the frame: "if there are only a few small parameters, processor registers will be used to pass the values"[^1].

**Made on call, freed on return.** "Each time a function is called, a new frame is made. Each time a function returns, the frame for that function invocation is eliminated. If a function is recursive, there can be many frames for the same function"[^2]. GDB numbers the frames from the top: the level "is zero for the innermost frame, one for the frame that called it, and so on upward"[^2].

**Prologue and epilogue.** The compiler adds code at the start and at the end of each function to make and free the frame. The prologue prepares "the stack and registers for use within the function", and the epilogue "restores the stack and registers to the state they were in before the function was called"[^3]. On x86, a typical prologue saves the old frame pointer, copies the stack pointer into the frame pointer, and moves the stack pointer to make room: "the stack pointer is decreased to make room for the function's local variables"[^3].

**Stack pointer and frame pointer.** "A frame pointer of a given invocation of a function is a copy of the stack pointer as it was before the function was invoked"[^1]. The fields of the frame can then be found "as negative offsets of the stack pointer" or "as positive offsets of the frame pointer"[^1]. "In most systems a stack frame has a field to contain the previous value of the frame pointer register"[^1]. These saved values link each frame to the frame of its caller, and this link "enables code to access each frame successively underneath" the frame of the running routine[^1]. A debugger walks the stack in this way to print a backtrace.

**A real layout.** The Go ABI for amd64 gives a frame with, from the top down, the return PC, the saved RBP, the locals and the outgoing arguments[^4]. "The stack pointer, RSP, grows down and is always aligned to 8 bytes"[^4]. "On entry, a function subtracts from RSP to open its stack frame and saves the value of RBP directly below the return PC"[^4]. In the Microsoft x64 convention, "A function's prolog is responsible for allocating stack space for local variables, saved registers, stack parameters, and register parameters"[^5].

**Frames in a virtual machine.** A virtual machine has frames too, but not the same fields. In the JVM, "Each frame has its own array of local variables (§2.6.1), its own operand stack (§2.6.2), and a reference to the run-time constant pool"[^6]. "The sizes of the local variable array and the operand stack are determined at compile-time"[^6]. In CPython, a frame is an object that the program can read. Its `f_back` attribute "Points to the previous stack frame (towards the caller)", and `f_locals` is "The mapping used by the frame to look up local variables"[^7]. In JavaScript, MDN calls the frame an execution context: "An execution context, also known generally as a stack frame, is the smallest unit of execution"[^8].

## In languages

| Language | Verdict | What the frame is and what the program sees |
|---|---|---|
| Python | native | a frame object with `f_locals`, `f_back` and `f_code`; `sys._getframe()` and `inspect.currentframe()` |
| JavaScript | native | an execution context for each call; the program cannot read it |
| TypeScript | native | as in JavaScript |
| Java | native | a JVM frame with a local variable array and an operand stack; the program cannot read the locals |
| Kotlin | native | JVM frames, as in Java |
| C# | native | the runtime chooses where locals go; a local that a lambda captures moves to an object on the heap[^13] |
| Go | native | the compiler chooses which locals stay in the frame and which move to the heap |
| Rust | native | locals live in the frame of the call; a `Box` puts its value on the heap |
| C | native | the platform ABI sets the layout; GCC reports frame sizes with `-fstack-usage` |
| C++ | native | as in C; destructors of locals run when the frame is freed |
| Bash | native | `local` gives a variable for one call of a function |
| PowerShell | native | each call of a function gets its own scope for its variables |
| SQL | n/a | a query has no frames that the code can see |
| Regex | n/a | a pattern has no calls |
| HTML | n/a | HTML has no calls |
| CSS | n/a | CSS has no calls |

The example files run the same recursion in each language, and each call prints its own copy of `here` after the inner calls return. Python also walks the frames with `f_back`. C, C++ and Rust also record the address of `here` in each call and show that the three frames have three different addresses. In Bash, `local` makes the variable "have a visible scope restricted to that function and its children"[^9], so each call has its own `here`.

## Variations and related ideas

- **No frame pointer.** A compiler can leave out the frame pointer and use only the stack pointer. GCC's `-fomit-frame-pointer` will "Omit the frame pointer in functions that don't need one", and "on many targets it also makes an extra register available"[^10]. A debugger then needs other data to walk the stack. GDB says that "Some compilers provide a way to compile functions so that they operate without stack frames"[^2].
- **A frame that outlives its call.** A frame can stay after its function stops for a time. MDN says that "When a frame is popped, it's not necessarily gone forever", as for a generator function[^8]. A closure keeps the variables that it uses alive after the call returns. See [closures](/closures/).
- **A frame that is reused.** With tail-call elimination, "Most of the frame of the current procedure is no longer needed, and can be replaced by the frame of the tail call"[^11].
- **A local that is not in the frame.** In Go, "if the compiler cannot prove that the variable is not referenced after the function returns, then the compiler must allocate the variable on the garbage-collected heap to avoid dangling pointer errors"[^12].

## Pitfalls

- **A pointer into a dead frame.** In C and C++, a function that returns the address of a local returns a pointer to a frame that no longer exists. The memory is reused by the next call. See [stack and heap](/stack-and-heap/).
- **Value types and the stack.** It is often said that a C# value type lives on the stack. Eric Lippert, who worked on the C# compiler, calls this a "memory-model implementation detail I often see presented as a fact"[^13]. He notes that an integer field of a class is part of the memory of the object, and that object is on the heap[^13]. See [value types and reference types](/value-types-and-reference-types/).
- **Large locals.** A large array as a local makes a large frame, and deep calls with large frames use up the stack sooner. See [stack overflow](/stack-overflow-error/).
- **Missing frames.** An inlined function has no frame of its own, so it is not in a stack trace. See [call stack](/call-stack/).

## Cost

To make a frame costs a few instructions: the prologue moves the stack pointer by the size of the frame, and the epilogue moves it back[^3]. The size is known when the code is compiled. GCC can report it: `-fstack-usage` "Makes the compiler output stack usage information for the program, on a per-function basis"[^14], and `-Wframe-larger-than=` will "Warn if the size of a function frame exceeds byte-size"[^15]. The warning is approximate, and "any space allocated via alloca, variable-length arrays, or related constructs is not included"[^15].

## History

Languages with nested procedures, such as ALGOL 60, need a frame to find the frames of the procedures around it. One way is an array of pointers "known as a display which is indexed to locate a desired frame"[^1]. Some machines did this in hardware: "the Burroughs B6500 implemented such a display in hardware which supported up to 32 levels of static nesting"[^1]. In ALGOL 60 and PL/I, a block inside a procedure "may have its own local variables, allocated on block entry and freed on block exit"[^1]. In 1977, Guy L. Steele argued that a call in tail position "can be best treated as a direct transfer of control to the called procedure, typically eliminating unnecessary stack manipulation operations"[^11].
