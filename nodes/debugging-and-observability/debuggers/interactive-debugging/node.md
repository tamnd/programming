---
tag: NEW-interactive-debugging
kind: concept
slug: interactive-debugging
name: Interactive debugging
aliases: [debugger, breakpoint, watchpoint, stepping, backtrace, gdb, lldb, pdb, Delve, jdb]
area: debugging-and-observability/debuggers
level: core
summary: >-
  A debugger stops a running program at a breakpoint, runs it one step at a time, and shows its stack and variables. It controls the process through the operating system and reads debug information.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1988
  people: [Brian Russell]
  note: Brian Russell developed DWARF at Bell Labs for the C compiler and the sdb debugger of Unix System V Release 4. The PLSIG documented it as DWARF Version 1 in 1992.
see_also: [cpu-profiling, structured-logging, delta-debugging, address-sanitizer, exception-handling, recursion, unit-testing, threads]
same_as:
  wikidata: Q193231
  wikipedia: Debugger
sources:
  - url: https://sourceware.org/gdb/current/onlinedocs/gdb.html/Set-Breaks.html
    title: Debugging with GDB, Setting Breakpoints
    locator: section 5.1.1 (break, break … if cond)
  - url: https://sourceware.org/gdb/current/onlinedocs/gdb.html/Continuing-and-Stepping.html
    title: Debugging with GDB, Continuing and Stepping
    locator: section 5.2 (step, next, finish, until, stepi)
  - url: https://sourceware.org/gdb/current/onlinedocs/gdb.html/Set-Watchpoints.html
    title: Debugging with GDB, Setting Watchpoints
    locator: section 5.1.2 (software and hardware watchpoints, rwatch, awatch)
  - url: https://sourceware.org/gdb/current/onlinedocs/gdb.html/Backtrace.html
    title: Debugging with GDB, Backtraces
    locator: section 8.2 (the first paragraph)
  - url: https://sourceware.org/gdb/wiki/Internals/Breakpoint%20Handling
    title: GDB wiki, Internals, Breakpoint Handling
    locator: the first section (hardware and software breakpoints)
  - url: https://man7.org/linux/man-pages/man2/ptrace.2.html
    title: Linux manual page, ptrace(2)
    locator: DESCRIPTION; PTRACE_TRACEME; PTRACE_PEEKTEXT; PTRACE_POKETEXT; PTRACE_GETREGS; PTRACE_SINGLESTEP
  - url: https://man.cat-v.org/unix-6th/2/ptrace
    title: Unix Programmer's Manual, Sixth Edition, ptrace(II)
    locator: DESCRIPTION; requests 0 to 8; SEE ALSO; BUGS
  - url: https://dwarfstd.org/doc/Debugging-using-DWARF-2012.pdf
    title: Michael J. Eager, Introduction to the DWARF Debugging Format, April 2012
    locator: Using a Debugger; DWARF 1, Unix SVR4 sdb and PLSIG; Debugging Information Entry; Location Expressions; Line Number Table; Call Frame Information
  - url: https://sourceware.org/gdb/current/onlinedocs/gdb.html/Optimized-Code.html
    title: Debugging with GDB, Debugging Optimized Code
    locator: section 11 (the first paragraphs)
  - url: https://lldb.llvm.org/use/map.html
    title: LLDB, GDB to LLDB command map
    locator: Breakpoint Commands; Watchpoint Commands; Execution Commands; Examining Variables; Examining Thread State
  - url: https://docs.python.org/3.14/library/pdb.html
    title: Python documentation, pdb, The Python Debugger
    locator: the introduction; Command-line interface; Debugger commands (break, step, next, return, until, display, up, where, !)
  - url: https://docs.python.org/3.14/library/sys.html
    title: Python documentation, sys, System-specific parameters and functions
    locator: sys.breakpointhook
  - url: https://docs.python.org/3.14/library/faulthandler.html
    title: Python documentation, faulthandler, Dump the Python traceback
    locator: the introduction; Dumping the traceback; Fault handler state
  - url: https://go.dev/doc/gdb
    title: Go documentation, Debugging Go Code with GDB
    locator: the note at the top; Introduction
  - url: https://github.com/go-delve/delve/blob/master/Documentation/cli/README.md
    title: Delve documentation, Commands
    locator: break; condition; step; next; stepout; stack; goroutines; watch; rev
  - url: https://pkg.go.dev/runtime
    title: Go standard library, package runtime
    locator: Breakpoint; Callers; CallersFrames; FuncForPC
  - url: https://pkg.go.dev/runtime/debug
    title: Go standard library, package runtime/debug
    locator: Stack; PrintStack
  - url: https://docs.oracle.com/en/java/javase/25/docs/specs/man/jdb.html
    title: Java SE 25 tool specifications, The jdb Command
    locator: Description; Start a JDB Session; Breakpoints; Stepping
  - url: https://docs.oracle.com/en/java/javase/25/docs/specs/jpda/architecture.html
    title: Java SE 25, Java Platform Debugger Architecture, Structure Overview
    locator: the diagram and the list of the three layers
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/StackWalker.html
    title: Java SE 25 API, java.lang.StackWalker
    locator: the class description; StackWalker.StackFrame.getDeclaringClass
  - url: https://microsoft.github.io/debug-adapter-protocol/overview
    title: Debug Adapter Protocol, Overview
    locator: What is the Debug Adapter Protocol?
  - url: https://nodejs.org/api/debugger.html
    title: Node.js documentation, Debugger
    locator: Interactive mode; Watchers; Command reference; V8 inspector integration for Node.js
  - url: https://tc39.es/ecma262/multipage/ecmascript-language-statements-and-declarations.html
    title: ECMAScript Language Specification, The debugger Statement
    locator: section 14.16.1, Runtime Semantics, Evaluation
  - url: https://v8.dev/docs/stack-trace-api
    title: V8 documentation, Stack trace API
    locator: Basic stack traces; Error.stackTraceLimit
  - url: https://doc.rust-lang.org/std/backtrace/struct.Backtrace.html
    title: Rust standard library, std::backtrace::Backtrace
    locator: capture; force_capture; disabled
  - url: https://doc.rust-lang.org/rustc/codegen-options/index.html
    title: The rustc book, Codegen Options
    locator: debuginfo
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.diagnostics.debugger.break
    title: .NET API reference, Debugger.Break Method
    locator: the summary and the Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.diagnostics.debugger.isattached
    title: .NET API reference, Debugger.IsAttached Property
    locator: the summary
  - url: https://en.cppreference.com/w/cpp/utility/source_location/current
    title: cppreference, std::source_location::current
    locator: Notes (the call in a default argument)
  - url: https://sourceware.org/gdb/current/onlinedocs/gdb.html/Contributors.html
    title: Debugging with GDB, Contributors to GDB
    locator: the first paragraph
---

## Idea

A program gives a wrong result, and the code looks correct. The developer must see what the program does, not what the code seems to say. A debugger runs the program under its control. It stops the program at a chosen place, shows the values of the variables and the chain of calls, and lets the program continue one step at a time.

The DWARF introduction lists the common operations. The programmer sets a breakpoint at a line or a function. When the program reaches the breakpoint, the programmer displays variables and the call stack, which shows how the program arrived there. Then the programmer continues the program, or steps through it line by line, into called functions or over them.[^8]

Interactive debugging has a cost: a person must be at the terminal. A breakpoint in a server stops all requests, and a session leaves no record. The other tools of this area answer different questions. [[structured-logging]] records what happened in a run that nobody watched. [[cpu-profiling]] measures where the time goes. [[delta-debugging]] reduces a failing input before anybody opens a debugger. A debugger is for the moment when the developer has a failing case and needs to see the state at one point in it.

## How it works

### Breakpoints

A breakpoint is a place in the program where the user wants to regain control. GDB names two ways to make one.[^5]

- A hardware breakpoint uses a dedicated register of the processor. The debugger stores the address in the register. When the program counter matches it, the processor raises an exception. The registers are few, so GDB uses software breakpoints when the user asks for more.[^5]
- A software breakpoint changes the program. GDB replaces an instruction with a trap or another instruction that causes an exception. When the program reaches it, GDB takes the exception and stops the program. To continue, GDB puts back the original instruction, executes one instruction, puts the trap in again and continues.[^5]

A software breakpoint writes into the code, so the code must be in writable memory. The technique does not work for a program in ROM.[^5] The trap instruction must also be short. A long trap could cover the start of the next instruction, and a jump to that instruction would land in the middle of the trap.[^5]

A conditional breakpoint stops only when an expression is true. In GDB, `break … if cond` evaluates the condition each time the program reaches the breakpoint and stops if the value is nonzero.[^1] The program still stops at the trap each time. The debugger evaluates the condition and continues the program when the condition is false.

### Stepping

A stop at a breakpoint is the start. From there, the user moves through the code in steps of different sizes. GDB has these commands:[^2]

| Command | What it runs before it stops |
|---|---|
| `step` (`s`) | until control reaches a different source line, also inside a called function |
| `next` (`n`) | to the next line in the current frame; calls on the line run without a stop |
| `finish` | until the function of the selected frame returns; prints the returned value |
| `until` | like `next`, but at a jump it continues until it passes the jump, so it runs a loop to its end |
| `stepi` (`si`) | one machine instruction |

A step at the level of a source line is many machine instructions. The debugger needs a map from addresses to lines to know where a line ends. That map is the line number table of the debug information (see below).

### Watchpoints

A watchpoint stops the program when a value changes, wherever the change happens. It answers the question "who wrote this variable?" GDB makes a software watchpoint when it has no other choice. It single-steps the program and tests the value after each step, which is hundreds of times slower than normal execution. Most x86 and PowerPC targets have hardware watchpoints, which do not slow down the program.[^3] `rwatch` stops when the program reads a value, and `awatch` stops on a read or a write. They need hardware support.[^3]

### Inspecting the stack

A backtrace is a summary of how the program got where it is. It shows one line for each frame. Frame zero is the frame that runs now, frame one is its caller, and so on up the stack.[^4] The user selects a frame with `up` and `down` and prints the local variables of that frame.[^15] In the pdb example, `up` moves from `total` to `checkout`, and `p cart[1]["name"]` reads a variable that only `checkout` has.

### A session

The commands are similar across debuggers. This table lists the forms that the manuals give.

| Action | gdb | lldb | pdb | Delve | node inspect |
|---|---|---|---|---|---|
| set a breakpoint | `break total` [^1] | `b main` [^10] | `break total` [^11] | `break main.total` [^15] | `sb('main.js', 16)` [^22] |
| with a condition | `break 30 if price > 100` [^1] | `breakpoint set --name foo --condition '…'` [^10] | `break 30, price > 100` [^11] | `break 30 if price > 100` [^15] | |
| step into | `step` | `s` | `step` | `step` | `step` |
| step over | `next` | `n` | `next` | `next` | `next` |
| step out | `finish` | `finish` | `return` | `stepout` | `out` |
| show the stack | `backtrace` | `bt` | `where` | `stack` | `backtrace` |
| stop on a write | `watch total` [^3] | `watchpoint set variable total` [^10] | | `watch -w total` [^15] | |

The manuals of pdb and of the Node debugger list no watchpoint command. pdb's `display` prints an expression each time the program stops in the frame and the value has changed.[^11] The Node debugger evaluates its watch expressions at each breakpoint and shows them.[^22]

### How a debugger works

#### Control of the process: ptrace

A debugger is a separate process. It needs a way to stop another process, read its memory and registers, and change them. On Unix, the system call for this is `ptrace`. The Linux manual says that ptrace lets the tracer observe and control the execution of the tracee, and examine and change the tracee's memory and registers. It is primarily used to implement breakpoint debugging and system call tracing.[^6]

The steps are these:[^6]

1. The debugger forks. The child calls `PTRACE_TRACEME` and then `execve` with the program. Or the debugger attaches to a running process with `PTRACE_ATTACH` or `PTRACE_SEIZE`.
2. The tracee stops each time a signal is delivered. The tracer learns this from its next call to `waitpid`.
3. While the tracee is stopped, the tracer reads and writes its memory with `PTRACE_PEEKTEXT` and `PTRACE_POKETEXT`, and reads its registers with `PTRACE_GETREGS`.
4. The tracer restarts the tracee with `PTRACE_CONT`, or with `PTRACE_SINGLESTEP` for one instruction. After the single step, the tracee appears stopped by `SIGTRAP`.

A software breakpoint is step 3 and step 4 together. `PTRACE_POKETEXT` writes the trap. The trap raises a signal, and the signal stops the tracee. Attachment is per thread on Linux. In a process with many threads, each thread is attached on its own.[^6]

The design is old. The `ptrace` page of the Sixth Edition of Unix gives the same model: a parent process controls a child, examines and changes its core image, and its primary use is breakpoint debugging. The traced child stops at a signal, and the parent learns of it through `wait`. That version had nine requests and no single step. Its BUGS section asks for a way to stop the child at each system call.[^7]

#### Meaning of the bytes: debug information

ptrace gives addresses and bytes. The user thinks in files, lines, functions and variables. The compiler writes debug information that maps one to the other, and DWARF is a common format for it. The DWARF introduction says that the debugger must reverse much of the transformation that the compiler made.[^8] Three parts of DWARF do most of the work:[^8]

- Debugging information entries (DIEs) describe the program. Each DIE has a tag, such as a subprogram or a variable, and attributes such as the name, the type and the location. A location expression gives the place of a variable. A stack machine evaluates it, so a variable can be in a register, or at an offset from the frame base.
- The line number table maps each address of code to a file, a line and a column. To set a breakpoint at a line, the debugger looks up the address in the table and stores the breakpoint instruction there. After a fault, the debugger looks up the address of the fault to find the line.
- Call frame information (CFI) tells the debugger how each function was called. With it, the debugger finds the frame of the caller and the arguments. The debugger uses it to unwind the stack and print the backtrace.

A program compiled without debug information has none of this. GDB can still stop the process, but it cannot map an address to a line, and `step` goes on until it reaches a function that has debug information.[^2] Compilers write the information with `-g`.[^9] In Rust, `-g` is the same as `-C debuginfo=2`, and without a flag rustc writes no debug information.[^26] The Go linker writes DWARF by default, and the flag `-ldflags=-w` leaves it out.[^14]

#### Debuggers of a virtual machine

A program on the JVM, in Python or in V8 is not a process that the debugger reads byte by byte. The virtual machine has a debugging interface of its own. The Java Platform Debugger Architecture has three layers. JVM TI defines the debugging services of the VM. JDWP defines the protocol between the debuggee and the debugger processes. JDI is a Java interface for the authors of debuggers.[^19] A debugger can connect at any layer.[^19] jdb, the command-line debugger of the JDK, demonstrates this architecture.[^18] pdb runs inside the Python process that it debugs. Node starts the V8 inspector, and a client connects to it on port 9229 of `127.0.0.1`.[^22]

## In languages

| Language | Verdict | Form |
|---|---|---|
| Python | stdlib | `pdb`, `breakpoint()` and `sys.breakpointhook`; `faulthandler` and `traceback` print the stack |
| Java | stdlib | `jdb` and JPDA in the JDK; `StackWalker` in the code |
| Kotlin | stdlib | the JVM debuggers, as in Java |
| JavaScript | native | the `debugger` statement; `node inspect` and the V8 inspector |
| TypeScript | native | the `debugger` statement of JavaScript |
| Go | library | Delve outside the toolchain; `runtime.Breakpoint`, `runtime.CallersFrames` and `runtime/debug.Stack` in the code |
| C | library | gdb or lldb; `__func__` and `__LINE__` for manual traces |
| C++ | library | gdb or lldb; `std::source_location` |
| Rust | library | gdb or lldb; `std::backtrace` |
| C# | library | an external debugger; `System.Diagnostics.Debugger` and `StackTrace` in the code |
| PowerShell | stdlib | `Set-PSBreakpoint` |
| Bash | idiom | `set -x` traces each command |
| SQL, Regex, HTML, CSS | n/a | no statements to stop at |

The examples on this page cannot type commands into a debugger. Each example shows instead the hook that a debugger uses, or the part of the work that a program can do itself: print its own stack.

### Python

pdb supports conditional breakpoints, single steps at the level of source lines, inspection of stack frames and post-mortem debugging.[^11] `python -m pdb script.py` runs a script under the debugger. When the script exits with an error, pdb enters post-mortem debugging.[^11] `breakpoint()` in the code calls `sys.breakpointhook`. By default, the hook calls `pdb.set_trace()`. If the environment variable `PYTHONBREAKPOINT` is `0`, the hook returns at once. Another value names a function to call instead.[^12] The Python example replaces the hook with a function that prints the stack and the local variables and returns.

pdb reads its commands from any file object. The example gives it the commands in a string and records a full session. A command name that is also a variable name is a trap: `n` is `next`, not the variable `n`. The `!` prefix runs a line as a Python statement, and it is necessary when the first word resembles a command.[^11]

`faulthandler` dumps the Python stack on a fault, after a timeout, on a user signal, or on request. It is written in C and uses only signal-safe functions, so it works after a crash or in a deadlock. It prints only the file, the function and the line, and at most 100 frames.[^13]

### Java and Kotlin

A JVM starts with the JDWP agent, for example `java -agentlib:jdwp=transport=dt_socket,server=y,suspend=n MyClass`. Then `jdb -attach 8000` connects to it. `stop at MyClass:22` and `stop in java.lang.String.length` set breakpoints. `step` goes to the next line, also in a called method, and `next` stays in the current frame.[^18] A debugger, such as the one in an IDE, can hook into JPDA at any of the three layers.[^19] In the code, `StackWalker` walks the frames of the current thread. By default, a frame has the names but not the `Class` object, and `getDeclaringClass` throws `UnsupportedOperationException`.[^20]

### JavaScript and TypeScript

JavaScript has a statement for this. The specification says that evaluating `debugger` may cause a breakpoint when the program runs under a debugger, and that it has no observable effect when no debugger is present or active.[^23] `node inspect script.js` starts the command-line client of Node. `node --inspect-brk` stops before the first line and waits for a client such as Chrome DevTools.[^22] TypeScript has the same statement, and the TypeScript example runs under Node with it. In V8, `error.stack` holds the topmost 10 frames by default. `Error.stackTraceLimit` changes the number, and `Infinity` collects all frames.[^24]

### Go

The Go documentation calls Delve a better alternative to GDB for programs that the standard toolchain builds. GDB does not understand Go programs well. The stack management, the threads and the runtime of Go differ from the model that GDB expects.[^14] Delve has commands for goroutines, conditional breakpoints, watchpoints and reverse execution with `rev`.[^15] The compiler inlines functions and keeps variables in registers. `go build -gcflags=all="-N -l"` turns these optimizations off for a debugging session.[^14] In the code, `runtime.Breakpoint` executes a breakpoint trap, `runtime.CallersFrames` reads the stack with inlined calls, and `debug.Stack` formats the stack of the calling goroutine.[^16][^17]

### C, C++ and Rust

These languages compile to machine code, and gdb and lldb debug them through ptrace and DWARF as described above. The lldb project keeps a map of each gdb command to its lldb form.[^10] The standard libraries of C and C++ do not walk the stack. A macro with `__func__` and `__LINE__` records where it was used. In C++, `std::source_location::current()` as a default argument gives the place of the call.[^29] Rust has `std::backtrace`. `Backtrace::capture` records a trace only when `RUST_BACKTRACE` or `RUST_LIB_BACKTRACE` is set. `Backtrace::force_capture` records one in all cases.[^25]

### C#

The .NET debugger is outside the program. `Debugger.IsAttached` tells if a debugger is attached to the process.[^28] `Debugger.Break` signals a breakpoint to an attached debugger. If no debugger is attached, the user is asked to attach one. The documentation also says that the runtime reports an error to Windows Error Reporting, and that its settings change what happens.[^27] The example checks `IsAttached` before it calls `Break`.

### IDE debuggers

An IDE shows breakpoints, values and the stack in the editor. Without a common protocol, each development tool must build this interface again for each debugger.[^21] The Debug Adapter Protocol defines one protocol between a development tool and a debugger. A debug adapter translates between the protocol and the API of a debugger or runtime.[^21] The JVM gives IDEs the layers of JPDA.[^19]

## Variations and related ideas

### Post-mortem debugging

A debugger can also examine a program that has already failed. `python -m pdb` enters post-mortem debugging when the program exits with an error, and the user inspects the frames of the exception.[^11] GDB can also inspect a core dump of a program that crashed.[^14]

### Printing the stack from the code

A program can print its own stack without a debugger: `traceback` in Python, `debug.Stack` in Go, `StackWalker` in Java, `StackTrace` in .NET, `std::backtrace` in Rust, `Error().stack` in JavaScript. The examples show each one. A printed stack is a record of one moment, so it fits in a log. See [[structured-logging]] and [[exception-handling]].

### Reverse execution

Delve's `rev` runs `next`, `step`, `step-instruction` and `stepout` backward, and `rewind` runs backward until a breakpoint or the start of the recorded history.[^15] The user goes to the failure and then back to the cause.

## Pitfalls

### Optimized code

The optimizer rearranges the code, and the debugger shows what is really there. The execution path may not match the source. A variable that the program defines but never uses does not exist in the compiled code.[^9] GDB suggests a build with `-g` alone when a problem with `-g -O` is in doubt.[^9] Go gives the flags `-N -l` for the same reason.[^14] The Rust example shows the effect on a backtrace: the closure in `total` has its own frame only when the compiler did not inline it.

### Inlined calls in a stack walk

The Go documentation advises against a loop over the program counters that `runtime.Callers` returns, and against `FuncForPC` on them. Such code cannot account for inlining. For an inlined call, `FuncForPC` returns the innermost function but with the entry of the outermost function.[^16] The Go `wrong` example loses `main.lookup` from the chain. `runtime.CallersFrames` gives the correct frames.

### A command that has the name of a variable

pdb runs a line as a command if the first word is a command. `n`, `s`, `c`, `p` and others are commands.[^11] The Python `wrong` example types `n` and `s` to see two variables. pdb runs `next` and `step` instead, and the program ends. `p n` or `!n` prints the variable.

### A stack trace that is too short

V8 keeps 10 frames by default.[^24] In a deep recursion, the trace loses the frames where the chain started. The JavaScript `wrong` example has 16 frames of `parse` and the trace keeps 10. See [[recursion]].

### A trace that was never captured

`Backtrace::capture` in Rust does nothing when the environment variables are not set.[^25] The Rust `wrong` example prints a disabled trace. The code looks correct, and the problem shows only when somebody needs the trace.

### A breakpoint call in production code

A `breakpoint()` call or a `debugger;` statement left in the code stops the program under a debugger, or calls whatever function `PYTHONBREAKPOINT` names.[^12] `Debugger.Break` in .NET without an attached debugger asks the user to attach one.[^27] Guard such calls, or remove them before a release.

### A watchpoint in software

A software watchpoint makes the program hundreds of times slower.[^3] In a program with many threads, a software watchpoint watches the expression in one thread only. GDB may not notice when another thread changes the value. A hardware watchpoint watches all threads.[^3] See [[threads]] and [[data-races]].

## Cost

Until the program reaches a breakpoint, the breakpoint costs nothing: the trap or the register waits for the program counter.[^5] Hardware watchpoints do not slow down the program.[^3] Each stop at a software breakpoint costs a signal, a switch to the debugger and back, and a single step to pass the original instruction.[^5][^6] A conditional breakpoint pays this cost at each pass, also when the condition is false. A software watchpoint single-steps the whole program.[^3]

In Rust, the levels range from none to line tables only to full information.[^26] Go puts DWARF in its binaries unless the linker gets `-w`.[^14] Some things do not work as well with `-g -O` as with `-g` alone, and GDB suggests a build without optimization when in doubt.[^9] That build is a different program from the one that runs in production, and a bug that depends on the optimizer may not appear in it.

A captured stack costs time too. The Rust documentation warns that a capture can be expensive on some platforms.[^25] V8 keeps 10 frames by default so that the collection has no noticeable cost.[^24]

## History

The `ptrace` system call is in the Sixth Edition of the Unix Programmer's Manual. The page names breakpoint debugging as its primary use and lists the command `cdb` under SEE ALSO.[^7] The model of the call, with a parent that controls a stopped child, is still the model of Linux.[^6]

Brian Russell developed DWARF at Bell Labs in 1988 for the C compiler and the sdb debugger of Unix System V Release 4. The Programming Languages Special Interest Group of Unix International documented it as DWARF Version 1 in 1992.[^8] Richard Stallman was the original author of GDB.[^30]

## References
