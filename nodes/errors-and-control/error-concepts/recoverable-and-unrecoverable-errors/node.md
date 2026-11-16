---
tag: BF11PP
kind: concept
slug: recoverable-and-unrecoverable-errors
name: Recoverable and unrecoverable errors
aliases: [recoverable error, unrecoverable error, fatal error, Result versus panic, Exception versus Error, errors versus bugs]
area: errors-and-control/error-concepts
level: core
summary: >-
  A recoverable error is one that a correct program expects and handles, such as a missing file. An unrecoverable error is usually a bug, and the safe action is to stop.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2010
  people: [Andrew Gerrand]
  note: Gerrand's post "Defer, Panic, and Recover" on the Go blog, on 4 August 2010, described panic as the Go mechanism that stops the ordinary flow of control, apart from the error values that Go uses for expected errors.
see_also: [error-concept, error-handling-strategies, panics, assertions, errors-as-values, exception-handling]
sources:
- url: https://doc.rust-lang.org/book/ch09-00-error-handling.html
  title: Error Handling, The Rust Programming Language
  locator: the introduction of chapter 9
- url: https://doc.rust-lang.org/book/ch09-03-to-panic-or-not-to-panic.html
  title: To panic! or Not to panic!, The Rust Programming Language
  locator: Guidelines for Error Handling
- url: https://doc.rust-lang.org/book/ch09-01-unrecoverable-errors-with-panic.html
  title: Unrecoverable Errors with panic!, The Rust Programming Language
  locator: the first part; Unwinding the Stack or Aborting in Response to a Panic
- url: https://man7.org/linux/man-pages/man3/abort.3.html
  title: abort(3), Linux manual page
  locator: DESCRIPTION
- url: https://doc.rust-lang.org/book/ch09-02-recoverable-errors-with-result.html
  title: Recoverable Errors with Result, The Rust Programming Language
  locator: the first paragraph; Shortcuts for Panic on Error
- url: https://go.dev/ref/spec#Handling_panics
  title: The Go Programming Language Specification
  locator: Run-time panics; Handling panics
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-11.html#jls-11.1.1
  title: Chapter 11. Exceptions, The Java Language Specification, Java SE 25 Edition
  locator: 11.1.1 The Kinds of Exceptions
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Error.html
  title: Class Error, Java SE 25 API
  locator: the class description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.environment.failfast
  title: Environment.FailFast Method, .NET API
  locator: the summary; Examples
- url: https://en.cppreference.com/w/cpp/error/terminate.html
  title: std::terminate, cppreference.com
  locator: the reasons for a call
- url: https://docs.python.org/3.15/library/exceptions.html
  title: Built-in Exceptions, The Python Standard Library
  locator: MemoryError; AssertionError
- url: https://go.dev/blog/defer-panic-and-recover
  title: Andrew Gerrand, Defer, Panic, and Recover, The Go Blog, 4 August 2010
  locator: Panic; Recover
- url: https://docs.python.org/3.15/reference/simple_stmts.html#the-assert-statement
  title: Simple statements, The Python Language Reference
  locator: 7.3 The assert statement
---

## Idea

Some errors are part of the normal life of a program. A file is missing, a user types a letter where a number belongs, a server is down. A correct program meets these errors, and it can do something useful: ask again, use a default, or report the error. These errors are **recoverable**.

Other errors mean that the program itself is wrong. An index is past the end of an array, or a value that must never be negative is negative. The code that would handle this error was written by the same person who made the mistake, so it has no reliable way to fix it. These errors are **unrecoverable**, and the safe action is to stop.

The Rust Book makes this split the base of the language: "Rust groups errors into two major categories: recoverable and unrecoverable errors"[^1]. "For a recoverable error, such as a file not found error, we most likely just want to report the problem to the user and retry the operation. Unrecoverable errors are always symptoms of bugs, such as trying to access a location beyond the end of an array, and so we want to immediately stop the program"[^1]. This program meets a recoverable error and handles it:

```rust run id=idea
use std::fs;

fn main() {
    // A recoverable error: the file may not exist.
    match fs::read_to_string("/no/such/file.txt") {
        Ok(text) => println!("read {} bytes", text.len()),
        Err(e) => println!("recovered: {:?}", e.kind()),
    }

    // A correct index. An index past the end would be a bug,
    // and Rust would stop the thread with a panic.
    let v = vec![1, 2, 3];
    let i = 2;
    println!("v[{i}] = {}", v[i]);
}
```

```output for=idea
recovered: NotFound
v[2] = 3
```

## How it works

- **The test.** The question is not how bad the error is, but whether a correct program can meet it. The Rust Book advises a panic when the code "could end up in a bad state"[^2], where "a bad state is when some assumption, guarantee, contract, or invariant has been broken"[^2]. It adds that the bad state is "something that is unexpected, as opposed to something that will likely happen occasionally, like a user entering data in the wrong format"[^2].
- **The caller decides.** A function often cannot know whether an error is recoverable for its caller. So the Rust Book advises: "returning Result is a good default choice when you’re defining a function that might fail"[^2]. The caller can handle the error, or turn it into a stop: "it could decide that an Err value in this case is unrecoverable"[^2].
- **Stopping.** A stop for a bug can unwind or abort. In Rust, a panic will by default "print a failure message, unwind, clean up the stack, and quit"[^3]. In C, `abort` raises `SIGABRT`, which "results in the abnormal termination of the process"[^4] unless a handler catches it.
- **Not all bugs stop.** A stop happens only where the code or the run-time system tests for the bug. In C and C++, many bugs have undefined behavior, and nothing stops the program; see [[undefined-behavior]].

## In languages

| Language | Recoverable errors | Unrecoverable errors |
|---|---|---|
| Rust | `Result<T, E>`[^5] | `panic!`, and `assert!`[^3] |
| Go | values of type `error` | `panic`, and run-time panics[^6] |
| Java | subclasses of `Exception`[^7] | subclasses of `Error`[^7][^8] |
| C# | exceptions | `Environment.FailFast`[^9] |
| C++ | exceptions and error codes | `std::abort`, and `std::terminate`[^10] |
| C | return values and `errno` | `abort`[^4] |
| Python | exceptions | the same exceptions, such as `AssertionError`[^11] |
| JavaScript, TypeScript | exceptions | the same exceptions |
| Bash | exit status | `exit` |

- **Rust.** Rust has no exceptions; "it has the type Result<T, E> for recoverable errors and the panic! macro that stops execution when the program encounters an unrecoverable error"[^1]. "Most errors aren’t serious enough to require the program to stop entirely"[^5]. The method `unwrap` turns one into the other: "If the Result is the Err variant, unwrap will call the panic! macro for us"[^5].
- **Go.** "Panic is a built-in function that stops the ordinary flow of control and begins panicking"[^12]. Panics "can also be caused by runtime errors, such as out-of-bounds array accesses"[^12]. The deferred functions run, and when the panic reaches the top of the goroutine, "the program crashes"[^12]. A deferred call to `recover` can stop a panic: "Recover is a built-in function that regains control of a panicking goroutine"[^12].
- **Java.** Java splits the classes of `Throwable`. "Exception is the superclass of all the exceptions from which ordinary programs may wish to recover"[^7]. "Error is the superclass of all the exceptions from which ordinary programs are not ordinarily expected to recover"[^7]. The API says that an `Error` "indicates serious problems that a reasonable application should not try to catch"[^8].
- **C#.** Exceptions report both kinds. For a state that is too bad to go on, `Environment.FailFast` "Immediately terminates the process before reporting an error message"[^9], and the `finally` blocks do not run[^9].
- **C++.** The runtime calls `std::terminate` "when the program cannot continue"[^10], for example when "An exception is thrown and not caught"[^10].
- **Python.** Python uses exceptions for both kinds. `AssertionError` is "Raised when an assert statement fails"[^11], and a program can catch it as any other exception. Some errors that sound fatal can be handled: `MemoryError` is "Raised when an operation runs out of memory but the situation may still be rescued (by deleting some objects)"[^11].

## Variations and related ideas

- **Panics.** [[panics]] describes the mechanism that stops a program or a thread, and the choice between unwinding and aborting.
- **Assertions.** [[assertions]] test the conditions that only a bug can break. They are the usual way to find an unrecoverable error early.
- **Strategies.** [[error-handling-strategies]] compares the ways to report a recoverable error.
- **Isolation.** A server can stop one request, one thread or one process for a bug, and keep the rest of the program alive. Go's `recover` and a top-level handler for each request work in this way.

## Pitfalls

### Recovering from a bug

A handler for all exceptions also catches a failed assertion. Here the bug, a call of `mean` with an empty list, becomes a normal result:

```python wrong id=python-hidden-assertion
def mean(xs):
    assert xs, "mean of an empty list"
    return sum(xs) / len(xs)


def report(xs):
    try:
        return f"mean = {mean(xs)}"
    except Exception:
        return "no data"


print(report([2, 4]))
print(report([]))
```

```output for=python-hidden-assertion
mean = 3.0
no data
```

If an empty list is expected, test for it and return a result. If it is a bug, let the error reach the top of the program. Also, "The current code generator emits no code for an assert statement when optimization is requested at compile time"[^13], so `python -O` removes this test.

### A stop for an expected error

`unwrap` turns a recoverable error into a panic. For input from a user, this stops the program for an error that a correct program must expect:

```rust fails expect=panic id=rust-unwrap-input
fn main() {
    let input = "x";
    let port: u32 = input.parse().unwrap();
    println!("{port}");
}
```

Use `match`, `?`, or a method such as `unwrap_or` for input from outside the program. Keep `unwrap` for values that cannot be wrong if the code is correct.

### Catching Error in Java

`catch (Throwable t)` also catches `Error`, such as `OutOfMemoryError` and `StackOverflowError`. After such an error, the state of the program may be broken. Catch `Exception`, or a narrower class.

### A recover that hides a bug

In Go, a deferred `recover` can turn each panic into an ordinary return. Use it at a border, such as the handler of one request, and report the panic there. Do not use it to make bugs silent.

## Cost

- **Recoverable errors.** Each one needs code at the call site or in a handler, and each needs a test.
- **Unrecoverable errors.** A stop is cheap to write. The cost is in availability: a program that stops for each bug needs a supervisor, a restart, or isolation of the part that failed.

## History

- **2010.** Andrew Gerrand's post "Defer, Panic, and Recover", on 4 August 2010, described the panic mechanism of Go and its use for a stop of the ordinary flow of control[^12].
- **The JLS.** The Java Language Specification splits `Throwable` into `Exception` and `Error`: the exceptions that programs may wish to recover from, and those that they are not expected to recover from[^7].
- **Rust.** The Rust Book makes the split between `Result` and `panic!` the first idea of its chapter on errors[^1].

## References
