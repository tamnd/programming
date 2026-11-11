---
tag: W6XZEH
kind: concept
slug: panics
name: Panic
aliases: [panic, abort, fail-fast, unrecoverable error, fatal error]
area: errors-and-control/panics-and-aborts
level: core
summary: >-
  A panic is how a program reacts to a bug that it finds while it runs. It stops the current work, and either ends the process or lets a boundary throw the broken work away.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1985
  people: [Jim Gray]
  note: Gray's report on the failures of Tandem systems described fail-fast software modules, which either work correctly or detect the fault, signal failure and stop.
see_also: [exception-handling, errors-as-values, assertions, defer-and-finally, threads, unit-testing]
same_as:
  wikidata: Q15117556
  wikipedia: Abort_(computing)
sources:
  - url: https://jimgray.azurewebsites.net/papers/TandemTR85.7_WhyDoComputersStop.pdf
    title: Jim Gray, Why Do Computers Stop and What Can Be Done About It?, Tandem Technical Report 85.7, June 1985
    locator: page 1, Introduction (the time line of an outage); page 11, Fault containment through fail-fast software modules
  - url: https://go.dev/ref/spec#Run_time_panics
    title: The Go Programming Language Specification
    locator: Run-time panics
  - url: https://go.dev/ref/spec#Handling_panics
    title: The Go Programming Language Specification
    locator: Handling panics
  - url: https://doc.rust-lang.org/std/panic/fn.set_hook.html
    title: Rust standard library, function std::panic::set_hook
    locator: the description (the hook runs before the panic runtime; the default hook prints to standard error)
  - url: https://doc.rust-lang.org/book/ch09-01-unrecoverable-errors-with-panic.html#unwinding-the-stack-or-aborting-in-response-to-a-panic
    title: The Rust Programming Language, Unrecoverable Errors with panic!
    locator: Unwinding the Stack or Aborting in Response to a Panic
  - url: https://blog.rust-lang.org/2016/05/26/Rust-1.9/
    title: The Rust Core Team, Announcing Rust 1.9, Rust Blog, 26 May 2016
    locator: Controlled unwinding
  - url: https://timsong-cpp.github.io/cppwp/n4950/except.ctor
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[except.ctor], paragraphs 1 and 2"
  - url: https://doc.rust-lang.org/std/process/fn.abort.html
    title: Rust standard library, function std::process::abort
    locator: the description (no destructors run)
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: ISO/IEC 9899 working draft N3220 (C23)
    locator: 7.2.2.1 The assert macro; 7.24.4.1 The abort function; 7.24.4.4 The exit function
  - url: https://doc.rust-lang.org/std/thread/struct.JoinHandle.html#method.join
    title: Rust standard library, struct std::thread::JoinHandle
    locator: method join (Err when the thread panics)
  - url: https://doc.rust-lang.org/std/panic/fn.catch_unwind.html
    title: Rust standard library, function std::panic::catch_unwind
    locator: the description and the Notes
  - url: https://erlang.org/download/armstrong_thesis_2003.pdf
    title: Joe Armstrong, Making reliable distributed systems in the presence of software errors, PhD thesis, Royal Institute of Technology, Stockholm, December 2003
    locator: section 4.3, Error handling philosophy (page 104); section 4.3.2, Workers and supervisors (pages 106 and 107); section 4.4, Let it crash (pages 107 and 108)
  - url: https://doc.rust-lang.org/book/ch09-03-to-panic-or-not-to-panic.html#guidelines-for-error-handling
    title: The Rust Programming Language, To panic! or Not to panic!
    locator: Guidelines for Error Handling
  - url: https://go.dev/doc/faq#exceptions
    title: Go FAQ
    locator: Why does Go not have exceptions?
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Error.html
    title: Java SE 21 API, class java.lang.Error
    locator: the class description and the list of direct subclasses
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Exception.html
    title: Java SE 21 API, class java.lang.Exception
    locator: the class description
  - url: https://docs.python.org/3/library/exceptions.html#SystemExit
    title: Python standard library, Built-in Exceptions
    locator: exception SystemExit
  - url: https://timsong-cpp.github.io/cppwp/n4950/sequence.reqmts
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[sequence.reqmts], paragraphs 118 to 123 (a[n] and a.at(n))"
  - url: https://timsong-cpp.github.io/cppwp/n4950/except.terminate
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[except.terminate], paragraphs 1 and 2"
  - url: https://timsong-cpp.github.io/cppwp/n4950/terminate.handler
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[terminate.handler], paragraph 3 (the default handler calls abort)"
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Runtime.html
    title: Java SE 21 API, class java.lang.Runtime
    locator: Java Virtual Machine Termination; methods exit and halt
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.environment.failfast
    title: .NET API reference, Environment.FailFast Method
    locator: the summary of each overload and the example ("The try-finally block is not executed")
  - url: https://docs.python.org/3/library/sys.html#sys.exit
    title: Python standard library, sys
    locator: sys.exit
  - url: https://docs.python.org/3/library/os.html#os._exit
    title: Python standard library, os
    locator: os._exit and os.abort
  - url: https://nodejs.org/api/process.html#processexitcode
    title: Node.js documentation, Process
    locator: process.exit([code]); process.abort(); Exit codes (code 1)
  - url: https://www.typescriptlang.org/docs/handbook/2/functions.html#never
    title: TypeScript Handbook, More on Functions
    locator: never
  - url: https://doc.rust-lang.org/book/ch11-01-writing-tests.html
    title: The Rust Programming Language, How to Write Tests
    locator: the paragraph after the first test run ("Each test is run in a new thread")
  - url: https://go.dev/doc/go1.21#language
    title: Go 1.21 Release Notes
    locator: Changes to the language (panic with a nil value and runtime.PanicNilError)
  - url: https://go.dev/doc/devel/release#go1.21.0
    title: Go Release History
    locator: go1.21.0 (released 2023-08-08)
---

## Idea

Some failures are expected. A file is missing, or a user types a letter where a number should be. A program handles these with [[errors-as-values]] or with [[exception-handling]]. Other failures are bugs. An index is past the end of an array, or a value breaks an invariant that the code relies on. The program is now in a state that its author did not plan for. Code that goes on from that state can give wrong answers or damage data.

A panic is the reaction to such a bug. The program stops the current work at once. It does not try to repair the state. Jim Gray called this property fail-fast: a module "should either function correctly or it should detect the fault, signal failure and stop operating."[^1] The failure is then handled at a coarser level. The whole process ends, or a boundary such as a thread, a request handler or a worker throws the broken work away and goes on with the next one.

In Rust, a failed `assert!` panics. Here each call runs in its own thread, so the panic ends that thread and not the program.

```rust run id=idea
use std::panic;
use std::thread;

fn average(values: &[i32]) -> i32 {
    assert!(!values.is_empty(), "average of no values");
    values.iter().sum::<i32>() / values.len() as i32
}

fn main() {
    panic::set_hook(Box::new(|_| {}));
    for values in [vec![2, 4, 9], vec![]] {
        let worker = thread::spawn(move || average(&values));
        match worker.join() {
            Ok(avg) => println!("average {avg}"),
            Err(_) => println!("the worker panicked; main goes on"),
        }
    }
}
```

```output
average 5
the worker panicked; main goes on
```

The call `panic::set_hook` replaces the default report on standard error with nothing, so the output stays short.

## How it works

A panic has four steps.

1. **Detection.** A check fails. The check is explicit, such as `assert!` or `panic!`, or the language inserts it, such as the bounds check on an index. In Go, an index out of bounds causes a run-time panic, which is the same as a call of `panic` with a value of type `runtime.Error`.[^2] At the level of the machine, an inserted check is a comparison and a branch to a rarely used path that starts the panic.
2. **Report.** The runtime records what went wrong. Go reports the value given to `panic`.[^3] In Rust, the panic hook runs first, before the stack unwinds or the process aborts, and the default hook prints a message to standard error.[^4]
3. **Unwinding or abort.** The runtime either walks back up the stack or ends the process at once. The next section compares the two.
4. **Stop at a boundary, or end.** If some frame on the stack catches the panic, execution goes on there. If none does, the process ends.

### Unwinding and abort

Unwinding means that the runtime walks back up the stack and cleans up the data of each function that it leaves.[^5] In Go, the deferred functions of each frame run, from the function that panicked up to the top function of the goroutine.[^3] In Rust, unwinding runs destructors but no other code.[^6] In C++, stack unwinding destroys the local objects between the throw and the handler, in the reverse order of their construction ([[exception-handling]]).[^7]

Abort means that the process ends at once, without that cleanup. The operating system then frees the memory of the process.[^5] Rust can be set up to abort on a panic. The line `panic = 'abort'` in a `[profile]` section of `Cargo.toml` selects it, for example for release builds.[^5] The function `std::process::abort` ends the process in the same way, and no destructors run on any thread.[^8] C `abort` ends the program by `raise(SIGABRT)`. Whether it flushes the open streams is up to the implementation.[^9]

| | Unwinding | Abort |
|---|---|---|
| Cleanup code | runs: destructors, deferred calls, `finally` | does not run |
| Can a boundary catch it? | yes | no |
| Buffered output and files | flushed by the cleanup code | may be lost |
| Binary | larger; it holds the data that the unwinder needs | smaller[^5] |
| State after the failure | the cleanup code runs on state that may be broken | nothing runs on the broken state |

The last row is the main tradeoff. Unwinding gives the program a chance to close files and release locks. It also runs code in a process that has just shown a bug.

### Boundaries

A boundary is the place where a panic stops and becomes an ordinary failure. The Rust 1.9 announcement says that panics abort execution up to an "isolation boundary", and code on the other side can still run. "A server, for example, does not necessarily need to go down just because of an assertion failure in one of its threads."[^6]

- **A thread.** In Rust, `JoinHandle::join` returns `Err` when the thread panicked.[^10] The idea example uses this.
- **A call.** Rust `catch_unwind` runs a closure and returns `Err` if the closure panics and unwinds.[^11] Go `recover` stops a panic when a deferred function calls it directly. The function that deferred it then returns to its caller in the normal way.[^3]
- **A process.** An operating system process shares no memory with other processes. Gray's fail-fast processes share no state and talk only by messages, so a failed process can stop without harm to the others.[^1] Joe Armstrong built the error handling of Erlang on the same idea: "Let some other process do the error recovery" and "If you can't do what you want to do, die."[^12]

A boundary is safe only if the work that failed did not change shared state. A thread or a process that owns its data can stop and leave nothing broken behind. A call that changed a shared map halfway cannot. The first pitfall below shows this.

### Stop or recover

The question is whether the program can trust its own state.

- **Recover from expected failures.** A missing file, a timeout or bad input from a user is part of normal operation. The Rust book says to use `Result` for these.[^13] Go says the same, and keeps `panic` and `recover` for "truly exceptional conditions".[^14]
- **Stop on bugs.** The Rust book says to panic when the code could end up in a bad state: an assumption, a guarantee, a contract or an invariant is broken, and the code after that point relies on not being in the bad state.[^13] Java says that an `Error` indicates "serious problems that a reasonable application should not try to catch".[^15]
- **Recover at a coarse boundary only.** When a server must stay up, it catches the panic at the edge of a request or a worker, logs it, and throws away all the state of that unit. It does not catch a panic in the middle of an operation and go on.
- **Stop the whole process when the state is shared.** If the bug may have damaged state that other work uses, the process ends. A supervisor, a service manager or a person then starts it again from a clean state.

## In languages

The example files run three jobs. One job has a bug, an index out of range. A boundary catches the failure, the cleanup of that job runs, and the next job goes on. Each file then shows the hard stop of the language in a `fails` block.

| Language | Verdict | Form |
|---|---|---|
| Python | idiom | no panic; a bug raises an exception; `sys.exit` raises `SystemExit`; `os._exit` and `os.abort` stop at once |
| JavaScript | idiom | no panic; a bug throws; `process.exit` and `process.abort` in Node.js |
| TypeScript | idiom | as JavaScript; the return type `never` marks a function that does not return |
| Java | idiom | `Error` versus `Exception`; `System.exit` and `Runtime.halt` |
| C | idiom | `abort`, `exit` and `assert`; no unwinding |
| C++ | idiom | `std::terminate` and `std::abort`; `noexcept` |
| C# | idiom | no panic; a bug throws; `Environment.FailFast` |
| Go | native | `panic`, `recover` in a deferred function, run-time panics |
| Rust | native | `panic!`, unwinding or abort, `catch_unwind`, `join` |
| Kotlin | idiom | the exceptions of the JVM, with the same split of `Error` and `Exception` as Java |
| PowerShell | idiom | the keyword `exit` and terminating errors |
| Bash | idiom | the builtin `exit`, and `set -e` to stop at the first failing command |
| SQL | absent | a statement fails; the engine does not panic the host program |
| Regex | n/a | a pattern has no notion of a bug |
| HTML/CSS | n/a | markup and style rules do not run code |

Go and Rust have a separate mechanism for bugs. In the other languages, a bug and an expected failure use the same exception mechanism, and the split is a convention or a type hierarchy. Java puts it in the type hierarchy: an `Exception` is a condition that "a reasonable application might want to catch",[^16] and an `Error` is one that it should not try to catch.[^15] Python puts `SystemExit` under `BaseException`, not under `Exception`, so that code which catches `Exception` does not catch it by accident.[^17]

Each language also has a hard stop that skips the cleanup code.

- **C.** `exit` calls the functions registered with `atexit` in the reverse order of registration, and then flushes and closes the open streams. `abort` ends the program abnormally.[^9] A failed `assert` writes a message to standard error and calls `abort`.[^9]
- **C++.** `vector::at` checks the index and throws `std::out_of_range`. The standard gives `operator[]` no such clause.[^18] In some situations the language abandons exception handling and calls `std::terminate`. One is an exception that no handler catches. Another is an exception that reaches the end of a `noexcept` function.[^19] The default terminate handler calls `abort`.[^20]
- **Java.** `System.exit` starts the shutdown sequence, which runs the shutdown hooks. `Runtime.halt` ends the JVM without that sequence. When the JVM ends, threads stop running Java code at once, so their `finally` blocks do not run.[^21]
- **C#.** `Environment.FailFast` "Immediately terminates the process". On Unix systems it writes the message and a stack trace to standard error, and the `finally` blocks do not run.[^22]
- **Python.** `sys.exit` raises `SystemExit`, so `finally` blocks run and an outer handler can stop the exit.[^23] `os._exit` ends the process without the cleanup handlers and without flushing the standard I/O buffers.[^24]
- **Node.js.** `process.exit` ends the process synchronously, and an exception that nothing catches ends it with exit code 1.[^25] In TypeScript, the return type `never` says that a function throws an exception or ends the program.[^26]

## Variations and related ideas

- **Assertions.** An assertion is the most common way to start a panic on purpose. C `assert`, Rust `assert!` and Java `assert` all stop the current work when a condition is false ([[assertions]]).
- **Fail-fast and let it crash.** Gray's fail-fast modules check their inputs, intermediate results, outputs and data structures, and stop when a check fails.[^1] Armstrong goes further: in the event of an error that the programmer does not know how to handle, "the program should just crash", and another process does the recovery.[^12]
- **Panics as error values.** At a boundary, a panic often becomes an error value. The Go example sets the named result `err` in the deferred function that recovers. The Rust 1.9 announcement gives this as a reason for `catch_unwind`: a library can translate a panic into an error on the C side of an interface.[^6]
- **Workers and supervisors.** A process that stops on a bug needs another process to act on the failure. In Erlang, a worker process does the job, and a supervisor process observes it and corrects the error when the worker fails.[^12] A restart begins from a clean state, which a recovery in place cannot promise.
- **Panics in tests.** A test fails when the test function panics. The Rust test runner runs each test in a new thread and marks the test as failed when that thread dies.[^27] This is a boundary per test ([[unit-testing]]).

## Pitfalls

### Recovery that keeps broken state

A recovery is safe only if it throws away all the state that the failed work touched. Here a transfer takes money from one account, then panics before it adds the money to the other. The handler recovers, logs, and the program goes on. The total is now wrong.

```go wrong id=broken-state
package main

import "fmt"

var balance = map[string]int{"alice": 100, "bob": 50}

func fee(amount int, rates []int) int {
	return amount * rates[len(rates)] / 100
}

func transfer(from, to string, amount int) {
	defer func() {
		if r := recover(); r != nil {
			fmt.Println("recovered:", r)
		}
	}()
	balance[from] -= amount
	balance[to] += amount - fee(amount, []int{1, 2})
}

func main() {
	fmt.Println("total before:", balance["alice"]+balance["bob"])
	transfer("alice", "bob", 30)
	fmt.Println("total after:", balance["alice"]+balance["bob"])
}
```

```output
total before: 150
recovered: runtime error: index out of range [2] with length 2
total after: 120
```

The fix is to stop the process, or to recover only at a boundary that owns all the state of the work, such as a request with its own data or a transaction that rolls back.

### recover in the wrong place

Go `recover` returns `nil` when it is not called directly by a deferred function.[^3] A deferred function that calls a helper, which then calls `recover`, does not stop the panic (see the `indirect` block of the Go example). A panic in another goroutine is also out of reach: it ends the whole program, even when `main` has a deferred `recover` (see the `goroutine` block).

### Catching too much

A handler for every kind of failure also catches the bugs and the requests to stop. In Python, a bare `except:` catches `SystemExit`, so a call of `sys.exit` does not end the program (see the `bare-except` block of the Python example). In Java, a handler for `Throwable` also catches `AssertionError` and `OutOfMemoryError`.[^15] Catch the narrowest type that the code can handle.

### Hard stops lose output

`os._exit` does not flush the standard I/O buffers,[^24] and C `abort` may not flush the open streams.[^9] Text that a program wrote to a buffer just before a hard stop can vanish. A program that must report a failure before it stops writes the report to an unbuffered stream such as standard error, as the C example does.

### Code that needs unwinding under abort

`catch_unwind` does not catch a panic that aborts the process.[^11] A Rust library that relies on `catch_unwind` loses its boundary when the application builds with `panic = 'abort'`. Unwinding across a language boundary, for example from Rust into C, is undefined behaviour.[^6] Catch the panic before it reaches the foreign code.

### A nil panic value in Go

Under the old rule of Go, `panic(nil)` started a panic for which `recover` returned `nil`. A handler that tested `recover() != nil` could not see it. The rule changed in 2023 (see History). A panic with a nil value now causes a run-time panic of type `*runtime.PanicNilError`, and `recover` in a panicking goroutine never returns `nil`.[^28]

## Cost

- **Checks on the normal path.** Each inserted check, such as a bounds check, is a comparison and a branch on every access. The panic path itself is rarely used.
- **Unwinding.** The unwinder needs data about each frame, and the cleanup code takes space. The Rust book suggests `panic = 'abort'` when the binary must be as small as possible.[^5]
- **Abort.** An abort costs nothing at the time of the failure, but no cleanup runs. Temporary files stay, buffered output can be lost, and a lock held in a file or a database stays until something else releases it.
- **Restart.** When the process ends, the service is down until something starts it again. Gray's report describes outages where the restart of a large system took from minutes to an hour.[^1]

## History

- **1985.** Jim Gray's Tandem report "Why Do Computers Stop and What Can Be Done About It?" described fail-fast software modules and processes that share no state.[^1]
- **2003.** Joe Armstrong's thesis described the error handling of Erlang with the slogans "If you can't do what you want to do, die" and "Let it crash".[^12]
- **2016.** Rust 1.9 stabilized the module `std::panic` with `catch_unwind`, to make isolation boundaries inside a thread.[^6]
- **2023.** Go 1.21, released on 8 August 2023, made `panic(nil)` a run-time panic of type `*runtime.PanicNilError`.[^28][^29]

## References
