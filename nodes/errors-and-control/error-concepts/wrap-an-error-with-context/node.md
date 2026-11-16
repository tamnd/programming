---
tag: ZAGNTN
kind: task
slug: wrap-an-error-with-context
name: Wrap an error with context
aliases: [exception chaining, chained exceptions, error wrapping, add context to an error, inner exception, nested exception, raise from]
area: errors-and-control/error-concepts
level: core
summary: >-
  Catch the error of a parser, add the place of the bad setting, and throw or return a new error that keeps the first one as its cause, so a caller can still find it.
contract: contract.yaml
category: errors
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3.15/tutorial/errors.html#exception-chaining
  title: Errors and Exceptions, The Python Tutorial
  locator: 8.5 Exception Chaining
- url: https://docs.python.org/3.15/library/exceptions.html#exception-context
  title: Built-in Exceptions, The Python Standard Library
  locator: Exception context
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Error/cause
  title: "Error: cause, MDN Web Docs"
  locator: the introduction
- url: https://docs.oracle.com/javase/tutorial/essential/exceptions/chained.html
  title: Chained Exceptions, The Java Tutorials
  locator: the first paragraphs
- url: https://learn.microsoft.com/en-us/dotnet/api/system.exception.innerexception
  title: Exception.InnerException Property, .NET API
  locator: Remarks
- url: https://en.cppreference.com/w/cpp/error/throw_with_nested.html
  title: std::throw_with_nested, cppreference.com
  locator: the description
- url: https://en.cppreference.com/w/cpp/error/rethrow_if_nested.html
  title: std::rethrow_if_nested, cppreference.com
  locator: the description
- url: https://go.dev/blog/go1.13-errors
  title: Damien Neil and Jonathan Amsterdam, Working with Errors in Go 1.13, The Go Blog, 17 October 2019
  locator: Wrapping errors with %w; Whether to Wrap
- url: https://doc.rust-lang.org/std/error/trait.Error.html
  title: std::error::Error, Rust standard library
  locator: Error source
- url: https://docs.rs/anyhow/latest/anyhow/
  title: anyhow, docs.rs
  locator: the crate description, Context
- url: https://www.gnu.org/software/bash/manual/html_node/Command-Execution-Environment.html
  title: Command Execution Environment, Bash Reference Manual
  locator: the subshell paragraph
---

## Task

Catch an error from a lower level, add what the current level knows, and keep the first error. The function `parse_port(text)` reads a TCP port number. It fails with the message `"not a number: 'TEXT'"` or `"out of range: TEXT"`. It does not know where the text came from. The function `read_port(where, text)` calls it. On an error, it makes a new error with the message `"WHERE: read port"` and keeps the error of `parse_port` as the cause.

The function `check_setting(where, text)` calls `read_port`. It returns `"port N"` on success. On an error, it returns `"error: "` and the messages of the error and its causes, joined with `": "`. For example:

- `check_setting("app.conf:3", "8080")` is `"port 8080"`,
- `check_setting("app.conf:3", "x")` is `"error: app.conf:3: read port: not a number: 'x'"`.

The message alone could carry the context. The point of a wrap is that the cause stays an object, so a program can still test its type or its value. [[error-propagation]] explains why errors go up the call stack. This task is one step of that path. [[define-a-custom-error-type]] makes the type of the outer error.

## Solutions

| Language | The wrap | The cause | The next step in the chain |
|---|---|---|---|
| Python | `raise ConfigError(...) from e`[^1] | `e.__cause__`[^2] | `e.__cause__` |
| JavaScript, TypeScript | `new Error(msg, { cause: e })`[^3] | `e.cause`[^3] | `e.cause` |
| Java | `new ConfigException(msg, e)`[^4] | `e.getCause()`[^4] | `getCause()` |
| C# | `new ConfigException(msg, e)` | `e.InnerException`[^5] | `InnerException` |
| C++ | `std::throw_with_nested(...)`[^6] | `std::rethrow_if_nested(e)`[^7] | a `catch` around `rethrow_if_nested` |
| Go | `fmt.Errorf("...: %w", err)`[^8] | `errors.Is`, `errors.As`[^8] | in the message |
| Rust | a struct with a `source()` method[^9] | `e.source()`[^9] | `source()` |
| C | a new message with the context in front | the code | none |
| Bash | a new message with the context in front | the exit status | none |

- **Python.** "To indicate that an exception is a direct consequence of another, the raise statement allows an optional from clause"[^1]. The expression after `from` "will be set as __cause__ on the raised exception"[^2]. The traceback then shows both exceptions. The solution follows `__cause__` to build the message.
- **JavaScript and TypeScript.** "The cause data property of an Error instance indicates the specific original cause of the error. It is used when catching and re-throwing an error with a more-specific or useful error message in order to still have access to the original error"[^3]. The solution passes `{ cause: e }` to the constructor and follows `cause` while it is an `Error`, because "The value of cause can be of any type"[^3].
- **Java.** "An application often responds to an exception by throwing another exception. In effect, the first exception causes the second exception"[^4]. `Throwable` has the constructor `Throwable(String, Throwable)`, and "getCause returns the exception that caused the current exception"[^4]. `ConfigException` passes both arguments to `super`.
- **C#.** "When an exception X is thrown as a direct result of a previous exception Y, the InnerException property of X should contain a reference to Y"[^5]. `ConfigException` passes the inner exception to the constructor of `Exception`, and the solution follows `InnerException`.
- **C++.** `std::throw_with_nested(t)` throws an exception that is derived from both `std::nested_exception` and the type of `t`. "The default constructor of the nested_exception base class calls std::current_exception, capturing the currently handled exception object"[^6]. So the call must be inside the `catch` block. `std::rethrow_if_nested(e)` throws the nested exception again, if `e` has one[^7]. The solution catches it and calls itself to walk the chain.
- **Go.** "In Go 1.13, the fmt.Errorf function supports a new %w verb. When this verb is present, the error returned by fmt.Errorf will have an Unwrap method returning the argument of %w"[^8]. "Wrapping an error with %w makes it available to errors.Is and errors.As"[^8]. The message of the new error already has the message of the old error, so the solution prints `err.Error()`. The test finds `ErrNotANumber` with `errors.Is`.
- **Rust.** "Errors may provide cause information"[^9], with `Error::source()`. `ConfigError` keeps the `ParseError` in a field and returns it from `source`. Its `Display` shows only the context, and a loop over `source()` builds the full message. The anyhow crate does the same work with a method: `context` on a `Result` adds a message, and its report prints the cause after "Caused by:"[^10].
- **C.** C has no exception objects and no chain. `read_port` writes `"WHERE: read port: "` in front of the message of `parse_port` and returns the same code. A caller can still test the code, but the cause is now only text.
- **Bash.** The same idea as C. `read_port` puts the context in front of `ERROR_MESSAGE` and returns the status of `parse_port`. Call it without `$( )`, because a command substitution runs in a subshell, and a command there "cannot affect the shell’s execution environment"[^11].

### The cause at work

In Go, the wrapped error of `strconv.Atoi` is still there under the context. `errors.As` gets the value with its fields, and `errors.Is` finds the sentinel error inside it:

```go run id=go-cause-at-work
package main

import (
	"errors"
	"fmt"
	"strconv"
)

func readPort(where, text string) (int, error) {
	port, err := strconv.Atoi(text)
	if err != nil {
		return 0, fmt.Errorf("%s: read port: %w", where, err)
	}
	return port, nil
}

func main() {
	_, err := readPort("app.conf:3", "x")
	fmt.Println(err)
	var numErr *strconv.NumError
	if errors.As(err, &numErr) {
		fmt.Println("cause:", numErr.Func, numErr.Num, numErr.Err)
	}
	fmt.Println("syntax error:", errors.Is(err, strconv.ErrSyntax))
}
```

```output for=go-cause-at-work
app.conf:3: read port: strconv.Atoi: parsing "x": invalid syntax
cause: Atoi x invalid syntax
syntax error: true
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: the empty text is `"not a number: ''"`. The quotes show that the text is empty.
- `zero` and `largest`: 0 is out of range, and 65535 is the largest port.
- `leading-space` and `negative`: a space or a sign is not a digit.
- `overflow`: twenty digits are more than a 64-bit integer can hold. The solutions stop when the value goes above 65535, so the cause is `"out of range"`.

## Pitfalls

- **A wrap that drops the cause.** In Go, `%w` and `%v` give the same message: "In all other ways, %w is identical to %v"[^8]. But only `%w` keeps the cause:

```go wrong id=go-wrap-with-v
package main

import (
	"errors"
	"fmt"
)

var ErrNotANumber = errors.New("not a number")

func readPort(where string) error {
	err := fmt.Errorf("%w: 'x'", ErrNotANumber)
	return fmt.Errorf("%s: read port: %v", where, err)
}

func main() {
	err := readPort("app.conf:3")
	fmt.Println(err)
	fmt.Println("not a number:", errors.Is(err, ErrNotANumber))
}
```

```output for=go-wrap-with-v
app.conf:3: read port: not a number: 'x'
not a number: false
```

The same mistake in Java is `new ConfigException(where + ": " + e.getMessage())`. The message looks right, but `getCause()` returns `null`, and the stack trace of the first exception is lost.

- **The cause two times.** The Rust documentation says that "the underlying error should be either returned by the outer error’s Error::source(), or rendered by the outer error’s Display implementation, but not both"[^9]. Here `Display` shows the cause, and `source` returns it too, so a loop over the chain prints it two times:

```rust wrong id=rust-cause-twice
use std::error::Error;
use std::fmt;

#[derive(Debug)]
struct ParseError(String);

impl fmt::Display for ParseError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "not a number: '{}'", self.0)
    }
}

impl Error for ParseError {}

#[derive(Debug)]
struct ConfigError {
    place: String,
    cause: ParseError,
}

impl fmt::Display for ConfigError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}: read port: {}", self.place, self.cause)
    }
}

impl Error for ConfigError {
    fn source(&self) -> Option<&(dyn Error + 'static)> {
        Some(&self.cause)
    }
}

fn main() {
    let e = ConfigError { place: "app.conf:3".to_string(), cause: ParseError("x".to_string()) };
    let mut message = e.to_string();
    let mut cur = e.source();
    while let Some(s) = cur {
        message = format!("{message}: {s}");
        cur = s.source();
    }
    println!("{message}");
}
```

```output for=rust-cause-twice
app.conf:3: read port: not a number: 'x': not a number: 'x'
```

Pick one: put the cause in `Display`, or return it from `source`.

- **A hidden cause.** In Python, `raise ConfigError(...) from None` "effectively replaces the old exception with the new one for display purposes"[^2]. The traceback then shows only the new exception. Use `from None` only when the cause does not help the reader.
- **A cause that leaks the inside.** A wrapped cause becomes part of what callers can test. The Go blog says: "Wrap an error to expose it to callers. Do not wrap an error when doing so would expose implementation details"[^8]. When the cause is an internal detail, put its text in the message and do not wrap it.

## Related tasks

- [[define-a-custom-error-type]]: the type of the outer error, with fields.
- [[log-an-error]]: write the whole chain to the log.

## References
