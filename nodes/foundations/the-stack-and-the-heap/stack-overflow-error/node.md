---
tag: 1J4BHR
kind: concept
slug: stack-overflow-error
name: Stack overflow
aliases: [stack overflow error, StackOverflowError, RecursionError, maximum call stack size exceeded, too much recursion, maximum recursion depth]
area: foundations/the-stack-and-the-heap
level: core
summary: >-
  A stack overflow happens when the calls that have not returned need more space than the call stack has. Deep or endless recursion is the usual cause.
applies_to: core-languages
viz: optional
formal: none
see_also: [call-stack, stack-frame, recursion, stack-and-heap, process-memory-layout, exception-handling]
same_as:
  wikidata: Q13218767
  wikipedia: Stack_overflow
sources:
- url: https://en.wikipedia.org/wiki/Stack_overflow
  title: Stack overflow, Wikipedia
  locator: Introduction; Causes; Constrained environment
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Errors/Too_much_recursion
  title: InternalError, too much recursion, MDN Web Docs
  locator: Message; What went wrong?; Examples
- url: https://docs.oracle.com/javase/specs/jvms/se21/html/jvms-2.html
  title: The Structure of the Java Virtual Machine, Java Virtual Machine Specification, Java SE 21
  locator: 2.5.2 Java Virtual Machine Stacks
- url: https://docs.python.org/3.14/library/exceptions.html
  title: Built-in Exceptions, Python 3.14 documentation
  locator: RecursionError
- url: https://learn.microsoft.com/en-us/dotnet/api/system.stackoverflowexception
  title: StackOverflowException Class, .NET API documentation
  locator: Remarks
- url: https://pkg.go.dev/runtime/debug
  title: runtime/debug, Go standard library
  locator: func SetMaxStack
- url: https://docs.python.org/3.14/library/sys.html
  title: sys, Python 3.14 documentation
  locator: sys.getrecursionlimit; sys.setrecursionlimit
- url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Thread.html
  title: Thread, Java SE 21 API
  locator: Thread(ThreadGroup, Runnable, String, long)
- url: https://doc.rust-lang.org/std/thread/index.html
  title: Module std::thread, Rust standard library
  locator: Stack size
- url: https://www.gnu.org/software/bash/manual/html_node/Bash-Builtins.html
  title: Bash Builtin Commands, Bash Reference Manual
  locator: ulimit
- url: https://www.gnu.org/software/bash/manual/html_node/Bash-Variables.html
  title: Bash Variables, Bash Reference Manual
  locator: FUNCNEST
- url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/StackOverflowError.html
  title: StackOverflowError, Java SE 21 API
  locator: Class StackOverflowError
---

## Idea

The [call stack](/call-stack/) has a fixed size. Each call that has not returned holds a [stack frame](/stack-frame/) on it, so a chain of calls that is too deep fills it. A stack overflow "occurs if the call stack pointer exceeds the stack bound"[^1]. In some languages the program gets an error that it can catch. In others, the program crashes.

```python run id=idea
import sys


def depth(n):
    if n == 0:
        return 0
    return 1 + depth(n - 1)          # one frame for each step


def loop(n):
    count = 0
    while n > 0:                     # one frame for the whole loop
        n -= 1
        count += 1
    return count


print("depth(100) =", depth(100))
try:
    depth(1_000_000)
except RecursionError:
    print("depth(1000000): RecursionError, the limit is", sys.getrecursionlimit())
print("loop(1000000) =", loop(1_000_000))
```

```output for=idea
depth(100) = 100
depth(1000000): RecursionError, the limit is 1000
loop(1000000) = 1000000
```

`depth(100)` needs 100 frames and works. `depth(1000000)` would need a million frames, and Python stops it with an error. The loop computes the same count in one frame.

## How it works

**The cause.** "The most common cause of stack overflow is excessively deep or infinite recursion, in which a function calls itself so many times that the space needed to store the variables and information associated with each call is more than can fit on the stack"[^1]. A function with no base case never stops. MDN describes the JavaScript error as one that "occurs when there are too many function calls, or a function is missing a base case"[^2]. A function with a base case can still overflow if the input is large, as `depth(1000000)` does above.

**Large locals.** "The other major cause of a stack overflow results from an attempt to allocate more memory on the stack than will fit, for example by creating local array variables that are too large"[^1]. A C function with a local array of 1048576 doubles "consumes 8 megabytes of data"[^1], which can be more than the whole stack.

**Threads.** Each thread has its own stack, and the size can differ. "most programs with threads have less stack space per thread than a program with no threading support"[^1]. A program that works on its main thread can overflow on another thread.

**What happens.** In C and C++, the program tries "to access memory beyond the call stack's bounds", and this is "typically resulting in a program crash"[^1]. For a C function that calls itself with no end, Wikipedia says the stack overflows, "resulting in a segmentation fault"[^1]. A virtual machine or an interpreter checks the depth and reports an error:

- The JVM: "If the computation in a thread requires a larger Java Virtual Machine stack than is permitted, the Java Virtual Machine throws a StackOverflowError"[^3].
- Python: `RecursionError` "is raised when the interpreter detects that the maximum recursion depth (see sys.getrecursionlimit() ) is exceeded"[^4].
- JavaScript: the engines throw `RangeError: Maximum call stack size exceeded` in Chrome and Safari, and `InternalError: too much recursion` in Firefox[^2].
- .NET: "You can't catch a StackOverflowException object with a try /catch block, and the corresponding process is terminated by default"[^5].
- Go: a goroutine stack grows as needed, up to a limit. "If any goroutine exceeds this limit while growing its stack, the program crashes"[^6].

## In languages

| Language | Verdict | What happens and how to change the limit |
|---|---|---|
| Python | native | `RecursionError`, which the program can catch; `sys.setrecursionlimit` sets the limit |
| JavaScript | native | `RangeError` or `InternalError`, which the program can catch |
| TypeScript | native | as in JavaScript |
| Java | native | `StackOverflowError`, which the program can catch; a `Thread` constructor takes a stack size |
| Kotlin | native | as in Java |
| C# | native | `StackOverflowException`, which ends the process; a `Thread` constructor takes a stack size |
| Go | native | the goroutine stack grows; past the limit of `debug.SetMaxStack`, the program crashes |
| Rust | native | the program aborts; `thread::Builder::stack_size` sets the size for a thread |
| C | native | a crash, often a segmentation fault; `ulimit -s` sets the limit of the shell |
| C++ | native | as in C |
| Bash | native | the `FUNCNEST` variable sets a limit on the nesting of functions |
| PowerShell | native | functions can call themselves, and each call uses stack space |
| SQL | n/a | a plain query has no recursion of functions |
| Regex | n/a | a pattern has no calls |
| HTML | n/a | HTML has no calls |
| CSS | n/a | CSS has no calls |

The example files run the same three steps. Python, JavaScript, TypeScript and Java catch the error. C#, C and C++ cannot catch it, so they count the depth and stop at a limit before the stack runs out. Go grows the stack and finishes. Rust runs the deep call on a thread with a large stack. Bash sets `FUNCNEST`.

## Variations and related ideas

- **The limit is a setting.** Python sets a depth, not a size: `sys.setrecursionlimit` will "Set the maximum depth of the Python interpreter stack to limit"[^7]. "This limit prevents infinite recursion from causing an overflow of the C stack and crashing Python"[^7]. In Java, a thread can ask for a stack size, but "The effect of the stackSize parameter, if any, is highly platform dependent"[^8]. In Rust, "The default stack size is platform-dependent and subject to change", and "the stack size of the main thread is not determined by Rust"[^9]. In Bash, the `ulimit` option `-s` sets "The maximum stack size"[^10], and `FUNCNEST` "defines a maximum function nesting level"[^11].
- **Tail calls.** "some compilers implement tail-call optimization", so that endless tail recursion can run "without stack overflow"[^1]. Scheme and some other languages "require all implementations to include tail-recursion as part of the language standard"[^1]. In a language without that rule, a tail call may still use a frame. See [recursion](/recursion/).
- **An accumulator.** A function that passes its partial result to the next call, such as `pow_accum(base, exp - 1, accum * base)`, is a tail call, so a tail-call optimizer can "drop" the prior stack frames[^1].

## Pitfalls

- **A higher limit can crash.** To raise the Python limit "should be done with care, because a too-high limit can lead to a crash"[^7]. The limit counts frames, and the real C stack may run out first.
- **Recursion on input data.** A function that recurses once for each item of a list, or once for each level of a tree, overflows on large input. Test with large input, or use a loop.
- **Catching the error.** In .NET the error cannot be caught, so "you should write your code to detect and prevent a stack overflow"[^5]. In Java, `StackOverflowError` is an `Error`, not an `Exception`[^12].
- **A setter that calls itself.** In JavaScript, a setter that assigns to its own property calls itself again: MDN shows that `this.name = name` inside `set name` is infinitely recursive[^2].

## Cost

A recursion that is too deep "can be fixed by transforming the recursion into a loop and storing the function arguments in an explicit stack (rather than the implicit use of the call stack)"[^1]. The explicit stack is on the heap, which is much larger than the call stack. To raise the stack size instead costs address space for each thread: "specifying a lower value may allow a greater number of threads to exist concurrently"[^8].

## History

The languages added their own ways to report and limit the error. `StackOverflowError` was in Java from version 1.0[^12]. Go added `debug.SetMaxStack` in Go 1.2, and its initial setting "is 1 GB on 64-bit systems, 250 MB on 32-bit systems"[^6]. Python added `RecursionError` in version 3.5. Before it, "a plain RuntimeError was raised"[^4].
