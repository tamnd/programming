---
tag: PY0RER
kind: concept
slug: call-stack
name: Call stack
aliases: [execution stack, run-time stack, control stack, program stack, machine stack, the stack]
area: foundations/the-stack-and-the-heap
level: core
summary: >-
  The call stack holds one entry for each call that has not yet returned. A call pushes an entry and a return pops it, so the top is always the function that runs now.
applies_to: core-languages
viz: optional
formal: none
see_also: [stack-and-heap, stack-frame, stack-overflow-error, process-memory-layout, recursion, exception-handling, stack-data-structure, interactive-debugging]
same_as:
  wikidata: Q759899
  wikipedia: Call_stack
sources:
- url: https://en.wikipedia.org/wiki/Call_stack
  title: Call stack, Wikipedia
  locator: Introduction; Description; Unwinding; Inspection
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Execution_model
  title: JavaScript execution model, MDN Web Docs
  locator: Stack and execution contexts
- url: https://docs.oracle.com/javase/specs/jvms/se21/html/jvms-2.html
  title: The Structure of the Java Virtual Machine, Java Virtual Machine Specification, Java SE 21
  locator: 2.5.2 Java Virtual Machine Stacks; 2.6 Frames
- url: https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/full-text/book/book-Z-H-31.html
  title: 5.1 Designing Register Machines, Structure and Interpretation of Computer Programs
  locator: 5.1.4 Using a Stack to Implement Recursion
- url: https://sourceware.org/gdb/current/onlinedocs/gdb.html/Backtrace.html
  title: Backtraces, Debugging with GDB
  locator: Whole page
- url: https://docs.python.org/3.14/library/inspect.html
  title: inspect, Python 3.14 documentation
  locator: The interpreter stack
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Error/stack
  title: Error.prototype.stack, MDN Web Docs
  locator: Introduction; Description
- url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/StackWalker.html
  title: StackWalker, Java SE 21 API
  locator: Class description
- url: https://pkg.go.dev/runtime
  title: runtime, Go standard library
  locator: func Callers; func CallersFrames
- url: https://en.cppreference.com/w/cpp/utility/basic_stacktrace.html
  title: std::basic_stacktrace, C++ reference, cppreference.com
  locator: Introduction
- url: https://www.gnu.org/software/bash/manual/html_node/Bash-Variables.html
  title: Bash Variables, Bash Reference Manual
  locator: FUNCNAME
- url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.utility/get-pscallstack
  title: Get-PSCallStack, PowerShell documentation
  locator: Description
- url: https://en.wikipedia.org/wiki/Tail_call
  title: Tail call, Wikipedia
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/api/system.diagnostics.stacktrace
  title: StackTrace Class, .NET API reference
  locator: Remarks
- url: https://doc.rust-lang.org/std/backtrace/index.html
  title: Module std::backtrace, Rust standard library
  locator: Accuracy; Environment Variables
- url: https://en.wikipedia.org/wiki/Stack_(abstract_data_type)
  title: Stack (abstract data type), Wikipedia
  locator: History
---

## Idea

When `main` calls `a`, `a` calls `b` and `b` calls `c`, the program must remember three places to go back to, in the reverse order. The call stack keeps them. Each call that has started and not yet returned has one entry on the stack, and the newest entry is on top. A call pushes an entry, and a return pops it and goes back to the caller. The top entry is the function that runs now, and the entries below it are the calls that wait for it.

```python run id=idea
import inspect


def where():
    names = [f.function for f in inspect.stack()[1:]]  # innermost first
    return " <- ".join(n for n in names if n != "<module>")


def c():
    print("      stack:", where())


def b():
    print("    call c")
    c()
    print("    back in b")


def a():
    print("  call b")
    b()
    print("  back in a")


def main():
    print("call a")
    a()
    print("back in main")


main()
```

```output for=idea
call a
  call b
    call c
      stack: c <- b <- a <- main
    back in b
  back in a
back in main
```

At the deepest point, four calls are active: `c` on top, and `b`, `a` and `main` below it. Each return pops one entry, so the program goes back to `b`, then to `a`, then to `main`, in the reverse order of the calls.

## How it works

**Push on call, pop on return.** A call stack is "a stack data structure that stores information about the active subroutines" of a program[^1]. The main reason for it "is to keep track of the point to which each active subroutine should return control when it finishes executing"[^1]. The caller or the start of the called function "pushes the return address onto the stack, and the called subroutine, when it finishes, pulls or pops the return address off the call stack and transfers control to that address"[^1]. MDN describes the JavaScript stack in the same way: "It's called a stack because it's last-in-first-out"[^2].

**An entry is a frame.** Each entry holds more than the return address. It also holds the arguments and the local variables of the call. This entry is a [stack frame](/stack-frame/). In the Java Virtual Machine, "A new frame is created each time a method is invoked. A frame is destroyed when its method invocation completes, whether that completion is normal or abrupt"[^3]. Because each call has its own frame, each call of a recursive function has its own copy of its locals.

**Why recursion needs a stack.** SICP shows the need with factorial. To compute n!, the machine must first compute (n - 1)!, and after that it still needs the old value of n. "In order to be able to continue the suspended computation, the machine must save the contents of any registers that will be needed after the subproblem is solved"[^4]. A stack holds these saved values, one set for each call that waits. See [recursion](/recursion/).

**One stack for each thread.** "There is usually exactly one call stack associated with a running program (or more accurately, with each task or thread of a process)"[^1]. In Java, "Each Java Virtual Machine thread has a private Java Virtual Machine stack, created at the same time as the thread"[^3]. When the stack has no more space for a push, "an error called a stack overflow occurs"[^1]. See [stack overflow](/stack-overflow-error/).

**Unwinding.** A return pops one frame. An exception can pop many frames at once. "When an exception is thrown, the stack is unwound until a handler is found that is prepared to handle (catch) the type of the thrown exception"[^1]. See [exception handling](/exception-handling/).

**Reading the stack.** A debugger and a stack trace read the same data. In GDB, a backtrace "shows one line per frame, for many frames, starting with the currently executing frame (frame zero), followed by its caller (frame one), and on up the stack"[^5]. The trace that a program prints when an exception is not caught is the call stack at the place of the throw. A profiler also samples the stack: "if a subroutine's address appears in the call stack sampling data many times, it is likely to act as a code bottleneck"[^1].

## In languages

| Language | Verdict | How the program reads its own stack |
|---|---|---|
| Python | native | `inspect.stack()`; the `traceback` module; frames are objects |
| JavaScript | native | the `stack` string of an `Error`, which is not in the standard |
| TypeScript | native | as in JavaScript |
| Java | native | `StackWalker`; `Throwable.getStackTrace()` |
| Kotlin | native | the JVM stack; `StackWalker` and `Throwable.stackTrace` from Java |
| C# | native | `System.Diagnostics.StackTrace` |
| Go | native | one stack for each goroutine; `runtime.Callers` and `runtime.CallersFrames` |
| Rust | native | `std::backtrace::Backtrace` |
| C | native | no standard function; a debugger such as GDB reads it |
| C++ | native | `std::stacktrace` |
| Bash | native | the `FUNCNAME` array and the `caller` builtin |
| PowerShell | native | `Get-PSCallStack` |
| SQL | n/a | a query has no call stack that the code can see |
| Regex | n/a | a pattern has no calls |
| HTML | n/a | HTML has no calls |
| CSS | n/a | CSS has no calls |

Every language with functions uses a call stack, but the languages give different access to it. Python's `inspect.stack()` returns a list of frame records: "The first entry in the returned list represents the caller; the last entry represents the outermost call on the stack"[^6]. In JavaScript, the `stack` property of an error is "Non-standard", but it "is de facto implemented by all major JavaScript engines"[^7]. Java's `StackWalker` reports frames "from the top most frame that represents the execution point at which the stack was generated to the bottom most frame"[^8]. Go's `runtime.Callers` "fills the slice pc with the return program counters of function invocations on the calling goroutine's stack"[^9]. C++ has `std::basic_stacktrace`, which "represents a snapshot of the whole stacktrace or its given part"[^10]. In Bash, `FUNCNAME` is "An array variable containing the names of all shell functions currently in the execution call stack"[^11]. In PowerShell, "The Get-PSCallStack cmdlet displays the current call stack"[^12].

The example files print the same trace in each language. Python, JavaScript, TypeScript, Java, C#, Go, Rust and Bash read the stack with the tools in the table. C and C++ keep their own list of names, which each function pushes on entry and pops on return.

## Variations and related ideas

- **Frames on the heap.** The stack is a model, and a runtime can store frames in other places. The JVM specification says that "Because the Java Virtual Machine stack is never manipulated directly except to push and pop frames, frames may be heap allocated"[^3].
- **Generators and coroutines.** A frame can leave the stack and come back. MDN says that "When a frame is popped, it's not necessarily gone forever, because sometimes we need to come back to it", as for a generator function[^2].
- **Tail calls.** "Tail calls can be implemented without adding a new stack frame to the call stack"[^13]. A language with tail-call elimination can run a tail-recursive loop in a fixed amount of stack.
- **More than one stack.** A thread usually has one call stack, but "additional stacks may be created for signal handling or cooperative multitasking"[^1].

## Pitfalls

- **Missing frames in a trace.** An optimizing compiler can inline a function, so its frame is not on the stack. The .NET documentation says that "StackTrace might not report as many method calls as expected, due to code transformations that occur during optimization"[^14]. Tail-call elimination also removes frames[^13]. Read a trace from an optimized build with care.
- **A trace format is not an interface.** Do not parse a stack trace in program logic. MDN warns: "You cannot rely on the precise content of the stack string due to implementation inconsistencies"[^7]. For Rust, "no guarantees are provided about the exact accuracy of a backtrace"[^15].
- **Frames kept alive.** In Python, a frame object keeps its locals alive. "Keeping references to frame objects, as found in the first element of the frame records these functions return, can cause your program to create reference cycles"[^6].
- **Deep recursion.** Each active call uses space on the stack, so a deep recursion can use all of it. See [stack overflow](/stack-overflow-error/).

## Cost

A call and a return cost little, because they only move the top of the stack. For local data, Wikipedia says that to move the top of the stack "is very fast when compared to dynamic memory allocation, which uses the heap space"[^1]. To read the stack is slower. Rust says that "Capturing a backtrace can be a quite expensive runtime operation"[^15], and for this reason `Backtrace::capture` does nothing unless an environment variable turns it on[^15]. Capture a trace when an error happens, not in a loop that runs often.

## History

Stacks "entered the computer science literature in 1946", when Alan Turing used the words bury and unbury for the call of a subroutine and the return from it[^16]. Klaus Samelson and Friedrich L. Bauer "proposed the idea of a stack called Operationskeller" in 1955, and filed a patent in 1957[^16].
