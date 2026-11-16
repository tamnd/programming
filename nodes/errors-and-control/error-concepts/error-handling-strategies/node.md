---
tag: 3GV3G6
kind: concept
slug: error-handling-strategies
name: Error handling strategies
aliases: [error reporting strategies, error handling models, exceptions versus error codes, exceptions versus error values, error-first callbacks, EAFP, LBYL]
area: errors-and-control/error-concepts
level: core
summary: >-
  A program can report an error with an exception, an error value, an error code, a special value, a callback or a stop of the program. Each language prefers one strategy, and each has a cost.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2015
  people: [Rob Pike]
  note: Pike's post "Errors are values" on the Go blog, on 12 January 2015, argued that error values are ordinary values that a program can store and compute with, which is the base of the Go strategy.
see_also: [error-concept, errors-as-values, exception-handling, panics, assertions]
sources:
- url: https://docs.python.org/3.15/library/stdtypes.html#str.find
  title: Built-in Types, The Python Standard Library
  locator: str.find; str.index
- url: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#e2-throw-an-exception-to-signal-that-a-function-cant-perform-its-assigned-task
  title: C++ Core Guidelines
  locator: E.2 Throw an exception to signal that a function can't perform its assigned task
- url: https://go.dev/doc/faq#exceptions
  title: Frequently Asked Questions (FAQ), The Go Programming Language
  locator: Why does Go not have exceptions?
- url: https://man7.org/linux/man-pages/man3/errno.3.html
  title: errno(3), Linux manual page
  locator: DESCRIPTION
- url: https://nodejs.org/api/errors.html#error-first-callbacks
  title: Errors, Node.js documentation
  locator: Error propagation and interception; Error-first callbacks
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Promise
  title: Promise, MDN Web Docs
  locator: Description
- url: https://docs.python.org/3.15/glossary.html#term-EAFP
  title: Glossary, Python documentation
  locator: EAFP
- url: https://docs.oracle.com/javase/tutorial/essential/exceptions/runtime.html
  title: Unchecked Exceptions, The Controversy, The Java Tutorials
  locator: the last paragraph
- url: https://learn.microsoft.com/en-us/dotnet/standard/exceptions/best-practices-for-exceptions
  title: Best practices for exceptions, .NET documentation
  locator: Handle common conditions to avoid exceptions; Design classes so that exceptions can be avoided
- url: https://doc.rust-lang.org/book/ch09-03-to-panic-or-not-to-panic.html
  title: To panic! or Not to panic!, The Rust Programming Language
  locator: Guidelines for Error Handling
- url: https://www.gnu.org/software/bash/manual/bash.html#The-Set-Builtin
  title: Bash Reference Manual
  locator: 3.7.5 Exit Status; 4.3.1 The Set Builtin (-e)
- url: https://go.dev/blog/error-handling-and-go
  title: Andrew Gerrand, Error handling and Go, The Go Blog, 12 July 2011
  locator: Introduction
- url: https://go.dev/blog/errors-are-values
  title: Rob Pike, Errors are values, The Go Blog, 12 January 2015
  locator: the first paragraphs
---

## Idea

When a function cannot do its job, it must tell its caller. There are a few ways to do it, and each language makes one of them the normal way. Python itself uses two ways for the same question. `str.find` returns a special value: "Return -1 if sub is not found"[^1]. `str.index` raises an exception: it is "Like find(), but raise ValueError when the substring is not found"[^1]. A third way returns the error as a value next to the result:

```python run id=idea
# 1. A special value: the result -1 means "not found".
print("abc".find("z"))

# 2. An exception: the error jumps to a handler.
try:
    "abc".index("z")
except ValueError as e:
    print(type(e).__name__)


# 3. An error value: the function returns a result and an error.
def index_of(s, sub):
    i = s.find(sub)
    if i < 0:
        return None, f"{sub!r} not found"
    return i, None


i, err = index_of("abc", "z")
print(i, err)
```

```output for=idea
-1
ValueError
None 'z' not found
```

The three ways give the same information. They differ in what the caller must write, in what happens when the caller forgets the error, and in what the type of the function says.

## How it works

The main strategies are these:

- **Exceptions.** The function throws an object, and the run-time system unwinds the call stack to the nearest handler for that type. The code between the throw and the handler does nothing. The C++ Core Guidelines give the rule: "Throw an exception to signal that a function can’t perform its assigned task"[^2].
- **Error values.** The function returns the error as an ordinary value, next to the result or in place of it, and the caller tests it. Go uses a second result of type `error`; Rust uses a `Result<T, E>`. The Go FAQ explains the choice: "Go’s multi-value returns make it easy to report an error without overloading the return value"[^3].
- **Error codes.** The function returns an integer code, and 0 means success. A shell command uses its exit status in this way.
- **Special values.** The function returns a value that is not a valid result, such as -1, `NULL` or `None`. In C, the variable `errno` then says which error occurred; its value "is significant only when the return value of the call indicated an error"[^4].
- **Callbacks and promises.** An asynchronous operation cannot return an error to its caller, because the caller has gone on. In Node.js, "Most asynchronous methods that accept a callback function will accept an Error object passed as the first argument to that function"[^5]. A promise has its own state for an error: "rejected: meaning that the operation failed"[^6].
- **Stopping.** For a bug, the program stops, or panics, and does not report the error to a caller. See [[panics]].

Two questions decide between them. First, is the error expected, or is it a bug? Second, does the caller usually handle it, or does it usually pass the error on?

## In languages

| Language | Main strategy | Other strategies |
|---|---|---|
| Python | exceptions, in the EAFP style[^7] | special values such as -1 and `None`[^1] |
| JavaScript, TypeScript | exceptions | rejected promises[^6]; error-first callbacks in Node.js[^5] |
| Java | exceptions, checked and unchecked[^8] | special values such as -1 and `null` |
| C# | exceptions | the Try pattern, such as `int.TryParse`[^9] |
| C++ | exceptions[^2] | error codes |
| Go | error values[^3] | panics for bugs[^3] |
| Rust | error values of type `Result`[^10] | panics for bugs[^10] |
| C | special values and `errno`[^4] | error codes |
| Bash | exit status[^11] | `set -e` to stop on a failed command[^11] |

- **Python.** Python prefers EAFP, "Easier to ask for forgiveness than permission"[^7]. This style "assumes the existence of valid keys or attributes and catches exceptions if the assumption proves false"[^7], and it "contrasts with the LBYL style common to many other languages such as C"[^7]. LBYL means "Look before you leap"[^7]: test first, then act.
- **Java.** The Java tutorial gives the rule for the two kinds of exception: "If a client can reasonably be expected to recover from an exception, make it a checked exception. If a client cannot do anything to recover from the exception, make it an unchecked exception"[^8].
- **C#.** .NET uses exceptions, but it advises against them for common conditions: "For conditions that are likely to occur but might trigger an exception, consider handling them in a way that avoids the exception"[^9]. "A class can provide methods or properties that enable you to avoid making a call that would trigger an exception"[^9].
- **Go.** The Go FAQ gives the reason for error values: "coupling exceptions to a control structure, as in the try-catch-finally idiom, results in convoluted code"[^3], and exceptions encourage "programmers to label too many ordinary errors, such as failing to open a file, as exceptional"[^3]. Go keeps panic and recover for "truly exceptional conditions"[^3].
- **Rust.** The Rust Book advises: "returning Result is a good default choice when you’re defining a function that might fail"[^10]. A panic is right when the code "could end up in a bad state"[^10], where "a bad state is when some assumption, guarantee, contract, or invariant has been broken"[^10]. "However, when failure is expected, it’s more appropriate to return a Result than to make a panic! call"[^10].
- **Bash.** With `set -e`, the shell exits when a pipeline "returns a non-zero status"[^11]. But the shell does not exit for a command that is "part of the test in an if statement"[^11] and in some other places, so `set -e` is not a full strategy.

## Variations and related ideas

- **Error values.** [[errors-as-values]] describes the strategy of Go and Rust in detail.
- **Exceptions.** [[exception-handling]] describes throw, catch and unwinding.
- **Panics.** [[panics]] describe the stop of a program for a bug.
- **Assertions.** [[assertions]] find bugs, and are not a way to report expected errors.
- **Errors and bugs.** [[error-concept]] explains the difference between an expected error and a bug, which decides the strategy.

## Pitfalls

### A special value that is also a valid value

A special value works only when the caller tests it. In Python, -1 is also a valid index: it names the last item. So a result of `str.find` that is not tested gives a wrong value and no error:

```python wrong id=python-find-minus-one
s = "hello"
i = s.find("z")
print(s[i])
```

```output for=python-find-minus-one
o
```

Use `str.index`, which raises an exception, or test the result for -1.

### An error-first callback that ignores the error

In Node.js, the callback receives the error as its first argument. Code that reads only the data goes on with `undefined`:

```javascript wrong id=js-ignored-callback-error
const fs = require("node:fs");

fs.readFile("/no/such/file", (err, data) => {
  console.log(String(data));
});
```

```output for=js-ignored-callback-error
undefined
```

Test the first argument before the data: "If that first argument is not null and is an instance of Error, then an error occurred that should be handled"[^5].

### Exceptions for ordinary results

An exception for a result that occurs often, such as a key that is not in a map, makes the normal path slow and hard to read. The Go FAQ names this risk[^3], and .NET advises a test or a Try method for "conditions that are likely to occur"[^9].

### Many strategies in one program

When one library throws, a second returns codes and a third returns `null`, each caller must know which strategy each function uses. Choose one strategy for each layer of a program, and translate errors at the border between layers.

## Cost

- **Exceptions.** The normal path has no tests, and the paths that errors take are not visible at the call site. The cost of a throw depends on the implementation; [[exception-handling]] gives the details.
- **Error values and codes.** Each call that can fail needs a test, so the code is longer. The cost at run time is a compare and a branch on each call, and the path of each error is visible.
- **Special values.** They cost the least, but nothing makes the caller test them.

## History

- **2011.** Andrew Gerrand's post "Error handling and Go", on 12 July 2011, described the strategy of Go: "Go code uses error values to indicate an abnormal state"[^12].
- **2015.** Rob Pike published "Errors are values" on the Go blog, on 12 January 2015[^13]. He wrote that the conversation about error handling in Go "often turns into a lament at the number of times the sequence"[^13] of tests for errors appears.

## References
