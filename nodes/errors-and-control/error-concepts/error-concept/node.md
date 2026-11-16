---
tag: Z7860D
kind: concept
slug: error-concept
name: Error
aliases: [error, errors, fault, failure, bug, error condition, expected error]
area: errors-and-control/error-concepts
level: intro
summary: >-
  An error is a state in which a program cannot do what it was asked to do. Some errors are expected, such as bad input, and the program reports them. Others come from bugs in the code.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1975
  people: [John B. Goodenough]
  note: John B. Goodenough published "Exception Handling, Issues and a Proposed Notation" in Communications of the ACM in December 1975.
see_also: [errors-as-values, exception-handling, panics, assertions, undefined-behavior, static-and-dynamic-typing]
sources:
- url: https://pascal.computer.org/sev_display/index.action
  title: SEVOCAB, Software and Systems Engineering Vocabulary, IEEE Computer Society
  locator: the entries error, fault and failure
- url: https://en.cppreference.com/w/cpp/language/exceptions.html
  title: Exceptions, cppreference.com
  locator: Usage; Error handling
- url: https://docs.python.org/3.15/tutorial/errors.html
  title: Errors and Exceptions, The Python Tutorial
  locator: 8.1 Syntax Errors; 8.2 Exceptions
- url: https://doc.rust-lang.org/book/ch09-00-error-handling.html
  title: Error Handling, The Rust Programming Language
  locator: the introduction of chapter 9
- url: https://go.dev/ref/spec#Errors
  title: The Go Programming Language Specification
  locator: Errors; Run-time panics
- url: https://go.dev/blog/error-handling-and-go
  title: Andrew Gerrand, Error handling and Go, The Go Blog, 12 July 2011
  locator: Introduction
- url: https://docs.oracle.com/javase/tutorial/essential/exceptions/definition.html
  title: What Is an Exception?, The Java Tutorials
  locator: the whole page
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-11.html
  title: Chapter 11. Exceptions, The Java Language Specification, Java SE 25 Edition
  locator: 11.1.1 The Kinds of Exceptions; 11.2 Compile-Time Checking of Exceptions
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Error
  title: Error, MDN Web Docs
  locator: the first paragraph
- url: https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/exceptions/
  title: Exceptions and Exception Handling, C# fundamentals
  locator: the first paragraphs
- url: https://man7.org/linux/man-pages/man3/errno.3.html
  title: errno(3), Linux manual page
  locator: DESCRIPTION
- url: https://www.gnu.org/software/bash/manual/bash.html#Exit-Status
  title: Bash Reference Manual
  locator: 3.7.5 Exit Status
- paper: doi:10.1145/361227.361230
  title: "John B. Goodenough, Exception Handling: Issues and a Proposed Notation, Communications of the ACM 18(12), 1975"
  locator: the Crossref record (author, journal, December 1975)
---

## Idea

A function is called to do a job. When it cannot do the job, the program is in an error state, and some code must decide what happens next. The C++ reference names the usual cases: an error is a failure "to meet the postconditions, such as failing to produce a valid return value object"[^2], a failure to meet the preconditions of another function, or a failure to keep a class invariant[^2].

The word "error" names many things, so this page uses the terms of the IEEE vocabulary[^1]:

- An **error** is a "discrepancy between a computed, observed, or measured value or condition and the true, specified, or theoretically correct value or condition"[^1]. A wrong value in a variable is an error.
- A **fault** is a "situation that can cause errors to occur in an object"[^1]. A fault in code is a bug: the vocabulary gives "bug" as a synonym[^1].
- A **failure** is the "departure of system behavior from system requirements"[^1]. "A failure can be produced when a fault is encountered"[^1].

Programs see two kinds of error. An expected error comes from the world outside the code: bad input, a missing file, a lost network connection. A correct program meets these errors, so it must report them and let a caller handle them. A bug comes from a fault in the code. The program cannot handle a bug in a useful way, because the code that would handle it has the same fault. This Python program shows both:

```python run id=idea
def average(xs):
    return sum(xs) / len(xs)


# An expected error: the input is empty, and Python reports it.
try:
    average([])
except ZeroDivisionError as e:
    print("reported:", type(e).__name__)


# A fault: the code divides by the wrong count, and nothing reports it.
def average_with_fault(xs):
    return sum(xs) / (len(xs) + 1)


print("expected 2.0, got", average_with_fault([1, 2, 3]))
```

```output for=idea
reported: ZeroDivisionError
expected 2.0, got 1.5
```

The first error is reported, so a caller can handle it. The fault gives a wrong value with no report, and the failure is visible only to a person who knows the correct answer.

## How it works

- **Detection.** Code must first find the error. A function tests its input, a library call tests the result of a system call, and the hardware or the run-time system finds some errors itself, such as a division by zero.
- **Report.** The code that finds an error is often not the code that can decide what to do. So the language gives a way to send the error to a caller. The main ways are an exception, an error value, an error code, and a stop of the program. Python reports "Errors detected during execution"[^3] with exceptions, which "are not unconditionally fatal"[^3]: a program can handle them.
- **Handling.** A caller that receives an error can try again, use a default, report the error to a person, or pass the error on to its own caller. If no code handles an error, the program usually stops with a message.
- **When it is found.** Some errors are found before the program runs. Python names "(at least) two distinguishable kinds of errors: syntax errors and exceptions"[^3]. The parser finds a syntax error before any code runs. A type checker finds a type error before the program runs, and a test at run time finds the rest.
- **Expected errors and bugs.** Some languages give the two kinds of error different mechanisms. Rust "groups errors into two major categories: recoverable and unrecoverable errors"[^4]. "Unrecoverable errors are always symptoms of bugs, such as trying to access a location beyond the end of an array, and so we want to immediately stop the program"[^4].

## In languages

| Language | An expected error | A bug found at run time |
|---|---|---|
| Python | an exception, such as `ValueError`[^3] | an exception, such as `IndexError` |
| JavaScript, TypeScript | a thrown value, usually an `Error` object[^9] | an `Error` object, such as `TypeError`[^9] |
| Java | a checked exception, which the compiler checks[^8] | an unchecked exception or an `Error`[^8] |
| C# | an exception[^10] | an exception |
| C++ | an exception or an error code[^2] | undefined behavior, or an assertion |
| Go | a value of type `error`[^5] | a run-time panic[^5] |
| Rust | a `Result<T, E>`[^4] | a panic[^4] |
| C | a special return value and `errno`[^11] | undefined behavior |
| Bash | an exit status that is not zero[^12] | an exit status that is not zero |

- **Python.** An exception is an object. "Most exceptions are not handled by programs, however, and result in error messages"[^3], with a traceback.
- **JavaScript.** "Error objects are thrown when runtime errors occur"[^9]. A program can also throw its own `Error`, or a value of any other type.
- **Java.** An exception is "an event, which occurs during the execution of a program, that disrupts the normal flow of the program's instructions"[^7]. The JLS divides exceptions into checked and unchecked classes, and the "compile-time checking for the presence of exception handlers is designed to reduce the number of exceptions which are not properly handled"[^8].
- **C#.** Exceptions help "deal with any unexpected or exceptional situations that occur when a program is running"[^10].
- **C++.** "Throwing an exception is used to signal errors from functions"[^2]. The standard library also has functions that return an error code.
- **Go.** The predeclared type `error` "is the conventional interface for representing an error condition, with the nil value representing no error"[^5]. "Go code uses error values to indicate an abnormal state"[^6]. A bug such as an index out of range causes a run-time panic: "Execution errors such as attempting to index an array out of bounds trigger a run-time panic"[^5].
- **Rust.** "Rust doesn’t have exceptions. Instead, it has the type Result<T, E> for recoverable errors and the panic! macro that stops execution when the program encounters an unrecoverable error"[^4].
- **C.** A function returns a special value, and the variable `errno` is "set by system calls and some library functions in the event of an error to indicate what went wrong"[^11]. Its value "is significant only when the return value of the call indicated an error"[^11].
- **Bash.** Each command has an exit status. "So while an exit status of zero indicates success, a non-zero exit status indicates failure"[^12].

## Variations and related ideas

- **Error values.** [[errors-as-values]] describes errors that a function returns as ordinary values, as in Go and Rust.
- **Exceptions.** [[exception-handling]] describes errors that jump to a handler up the call stack.
- **Panics.** [[panics]] describe a stop of the program, or of one thread, for a bug.
- **Assertions.** [[assertions]] find bugs: an assertion tests a condition that must be true if the code has no fault.
- **Undefined behavior.** In C and C++, some bugs have no defined result. [[undefined-behavior]] explains why such a bug can give no report at all.
- **Static checks.** [[static-and-dynamic-typing]] explains how a type checker finds some errors before the program runs.

## Pitfalls

### An ignored error

An error that the program does not test gives a wrong value later, far from its cause. In Go, the blank identifier discards the error, and the value is 0:

```go wrong id=go-ignored-error
package main

import (
	"fmt"
	"strconv"
)

func main() {
	n, _ := strconv.Atoi("x42")
	fmt.Println("n =", n)
}
```

```output for=go-ignored-error
n = 0
```

Test each error, or say in a comment why the program can ignore it.

### A handler that hides a bug

A handler for all errors also catches errors that come from bugs. Here a misspelled name gives a `NameError`, and the handler turns the bug into a wrong total:

```python wrong id=python-hidden-bug
def total(prices, shipping):
    try:
        return sum(prices) + shiping
    except Exception:
        return 0


print(total([10, 20], 5))
```

```output for=python-hidden-bug
0
```

Catch only the errors that the code expects, such as `ValueError` or `OSError`. Let a bug stop the program, or catch it in one place at the top level and report it.

### A failed command that does not stop the script

A shell script goes on after a command fails. The exit status is the only report:

```bash wrong id=bash-goes-on
false
echo "status $?"
echo "the script goes on"
```

```output for=bash-goes-on
status 1
the script goes on
```

Test the exit status of each command that can fail, or turn on `set -e`, which has its own limits.

### Errors for bugs and bugs for errors

A program that stops on an expected error is fragile: bad input from a user should give a message, not a crash. A program that tries to recover from a bug goes on in a state that is not correct. Use the mechanism for expected errors for the first kind, and stop on the second kind.

## Cost

- **Code.** Each place that can fail needs a test or a handler. In languages with error values, the tests are in the code; with exceptions, they are in fewer places, but the paths that the errors take are harder to see.
- **Run time.** A test of a return value costs a compare and a branch. The cost of an exception depends on the implementation; [[exception-handling]] gives the details.

## History

- **1975.** John B. Goodenough published a paper on exception handling in Communications of the ACM[^13].
- **2011.** Andrew Gerrand's post "Error handling and Go", on 12 July 2011, described the Go view: "Go code uses error values to indicate an abnormal state"[^6].
- **2025.** The IEEE vocabulary of software engineering separates error, fault and failure. Its definitions of error and fault come from ISO/IEC/IEEE 15026-1:2025, and one of its definitions of failure comes from IEEE 982:2024[^1].

## References
