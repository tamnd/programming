---
tag: ME8J17
kind: concept
slug: error-propagation
name: Error propagation
aliases: [propagating errors, passing errors up the call stack, error bubbling, rethrowing, wrapping errors]
area: errors-and-control/error-concepts
level: core
summary: >-
  Error propagation passes an error from the function that finds it up the call stack to a function that can handle it, often with more context added on the way.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2019
  people: [Damien Neil, Jonathan Amsterdam]
  note: Their post "Working with Errors in Go 1.13" on the Go blog, on 17 October 2019, described the new %w verb of fmt.Errorf and the functions errors.Is and errors.As, which let Go code add context to an error and keep the original error in a chain.
see_also: [error-concept, error-handling-strategies, exception-handling, errors-as-values, error-codes, recoverable-and-unrecoverable-errors]
sources:
- url: https://doc.rust-lang.org/book/ch09-02-recoverable-errors-with-result.html
  title: Recoverable Errors with Result, The Rust Programming Language
  locator: Propagating Errors; A Shortcut for Propagating Errors
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-11.html
  title: Chapter 11. Exceptions, The Java Language Specification, Java SE 25 Edition
  locator: 11.3 Run-Time Handling of an Exception
- url: https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/exceptions/
  title: Exceptions and Exception Handling, C# documentation
  locator: Exceptions overview
- url: https://docs.oracle.com/javase/tutorial/essential/exceptions/advantages.html
  title: Advantages of Exceptions, The Java Tutorials
  locator: Advantage 2
- url: https://go.dev/blog/go1.13-errors
  title: Working with Errors in Go 1.13, The Go Blog
  locator: Wrapping errors with %w; Whether to Wrap
- url: https://go.dev/blog/error-handling-and-go
  title: Error handling and Go, The Go Blog
  locator: The error type
- url: https://docs.python.org/3.15/tutorial/errors.html#exception-chaining
  title: Errors and Exceptions, The Python Tutorial
  locator: 8.5 Exception Chaining
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Error
  title: Error, MDN Web Docs
  locator: Instance properties (cause); Wrapping errors
- url: https://learn.microsoft.com/en-us/dotnet/standard/exceptions/best-practices-for-exceptions
  title: Best practices for exceptions, .NET documentation
  locator: Capture and rethrow exceptions properly; Wrap exceptions
- url: https://www.gnu.org/software/bash/manual/bash.html#The-Set-Builtin
  title: Bash Reference Manual
  locator: 4.3.1 The Set Builtin (-e)
- url: https://nodejs.org/api/errors.html#error-propagation-and-interception
  title: Errors, Node.js documentation
  locator: Error propagation and interception
- url: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#e17-dont-try-to-catch-every-exception-in-every-function
  title: C++ Core Guidelines
  locator: E.17
---

## Idea

The function that finds an error is often not the right place to handle it. A function that parses a number does not know if the number came from a file, a form or a network message, so it cannot choose what to tell the user. Error propagation passes the error up the call stack to a caller that knows enough to handle it.

The Rust book describes it so: "When a function’s implementation calls something that might fail, instead of handling the error within the function itself, you can return the error to the calling code so that it can decide what to do. This is known as propagating the error"[^1].

```python run id=idea
def parse_port(text):
    if not text.isdigit():
        raise ValueError(f"not a number: {text!r}")
    return int(text)


def load_config(lines):
    return {key: parse_port(value) for key, value in (line.split("=") for line in lines)}


def main():
    try:
        print(load_config(["port=8080"]))
        print(load_config(["port=x"]))
    except ValueError as e:
        print(f"bad config: {e}")


main()
```

```output for=idea
{'port': 8080}
bad config: not a number: 'x'
```

`parse_port` finds the error, and `load_config` has no code for it. The exception goes up through `load_config` to `main`, which handles it.

## How it works

Each error model moves the error up the stack in its own way.

- **Exceptions.** The run-time system moves the error. In Java, "control is transferred from the code that caused the exception to the nearest dynamically enclosing catch clause"[^2]. In C#, "the CLR will unwind the stack, looking for a method with a catch block for the specific exception type"[^3]. The functions in between need no code: the Java tutorial says that a method can "duck any exceptions thrown within it, thereby allowing a method farther up the call stack to catch it"[^4].
- **Error values.** Each function returns the error, and each caller tests it and returns it again. Rust shortens this with the operator `?`: "If the value is an Err, the Err will be returned from the whole function as if we had used the return keyword so that the error value gets propagated to the calling code"[^1]. Go writes the test out, as `if err != nil` followed by a `return`[^5].
- **Error codes.** Each function returns a code, and each caller must test it and return it. The Java tutorial shows this with error codes: each method in the chain must "propagate the error codes returned by readFile up the call stack until the error codes finally reach method1—the only method that is interested in them"[^4].

### Adding context

A low-level error message is often not enough. The Go blog says that "It is the error implementation’s responsibility to summarize the context"[^6]: the error from `os.Open` "formats as “open /etc/passwd: permission denied,” not just “permission denied.”"[^6]. Each layer that passes the error up can add what it knows: the file, the line, the request.

To add context and keep the original error, a function wraps it. The new error holds a message and a reference to the cause.

- Python: `raise NewError(...) from e`. The tutorial says that this "can be useful when you are transforming exceptions"[^7].
- JavaScript: `new Error(message, { cause: e })`. The property `cause` gives the "reason why the current error is thrown — usually another caught error"[^8].
- Java and C#: a constructor argument for the cause, which C# calls the inner exception[^9].
- Go: `fmt.Errorf` with the verb `%w`. With this verb, "the error returned by fmt.Errorf will have an Unwrap method returning the argument of %w"[^5].
- Rust: `?` passes the error through the `from` function, so "the error type received is converted into the error type defined in the return type of the current function"[^1]. `map_err` can add a message first.

## In languages

- **Python, JavaScript, TypeScript, Java, C#, C++** propagate exceptions without code in the functions in between. Java also checks at compile time that each checked exception is caught or declared, so each method that lets one pass shows it in its `throws` clause[^2].
- **Go** returns errors as values, and each caller passes an error up with `return`. `%w` wraps an error with context[^5].
- **Rust** returns errors in `Result`, and the operator `?` propagates them[^1].
- **C** passes error codes by hand, with a test after each call[^4].
- **Bash** passes exit statuses by hand. The option `-e` makes the shell exit at once when a pipeline returns a status that is not zero[^10]. This stops the whole script on the first failure, but it adds no context.

Node.js lists several mechanisms side by side, because "How these errors are reported and handled depends entirely on the type of Error and the style of the API that is called"[^11].

## Variations and related ideas

- **Where to handle.** The C++ Core Guidelines say: "Let an exception propagate until it reaches a function that can handle it"[^12]. A function that cannot do anything useful with an error passes it on.
- **Whether to wrap.** Wrapping exposes the original error to callers. The Go blog advises: "Wrap an error to expose it to callers. Do not wrap an error when doing so would expose implementation details."[^5]
- **Rethrowing.** A handler can catch an error, do some cleanup, and throw it again. In C#, a handler should "use the throw statement without specifying the exception" to keep the stack trace; with `throw e`, "the stack trace is restarted at the current method"[^9].

## Pitfalls

### Swallowing an error

A function that catches an error and returns a default stops the propagation. The caller gets a value that looks correct.

```python wrong id=python-swallowed-error
def parse_port(text):
    try:
        return int(text)
    except ValueError:
        return 0


port = parse_port("x")
print(f"listening on port {port}")
```

```output for=python-swallowed-error
listening on port 0
```

The program goes on with port 0, and no one sees the bad input. If a function cannot handle an error, it should let it propagate.

### Losing the cause when adding context

In Go, `%v` copies only the text of the error. The Go blog says that "Creating a new error with fmt.Errorf discards everything from the original error except the text"[^5].

```go wrong id=go-lost-cause
package main

import (
	"errors"
	"fmt"
	"io/fs"
	"os"
)

func readConfig(path string) error {
	_, err := os.ReadFile(path)
	if err != nil {
		return fmt.Errorf("read config: %v", err)
	}
	return nil
}

func main() {
	err := readConfig("/no/such/config")
	fmt.Println("not found:", errors.Is(err, fs.ErrNotExist))
}
```

```output for=go-lost-cause
not found: false
```

The file does not exist, but the caller cannot see it, because `%v` made a new error with only the text. With `%w`, `errors.Is` finds the original error in the chain and the program prints `true`[^5].

### Catching too early

A `try` block around each call makes the code long, and the handler often has nothing useful to do. The Core Guidelines say: "Catching an exception in a function that cannot take a meaningful recovery action leads to complexity and waste."[^12]

## Cost

With exceptions, the functions in between have no code for the error, so only the methods that care about errors have to detect them[^4]. With error values and codes, each call that can fail needs a test, and the Java tutorial notes that with codes even methods that are not interested in the error must test and return it[^4].

## History

On 17 October 2019, Damien Neil and Jonathan Amsterdam described the error wrapping of Go 1.13 on the Go blog. They wrote that "Go 1.13 introduces new features to the errors and fmt standard library packages to simplify working with errors that contain other errors"[^5], including the verb `%w` and the functions `errors.Is` and `errors.As`.

## References
