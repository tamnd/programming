---
tag: PHJT60
kind: task
slug: define-a-custom-error-type
name: Define a custom error type
aliases: [custom exception, user-defined exception, custom error class, error struct, error enum, exception subclass]
area: errors-and-control/error-concepts
level: core
summary: >-
  Report a bad port number with an error type of your own that keeps the text and the reason as fields, and let the caller pick out that type from the other errors.
contract: contract.yaml
category: errors
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3.15/tutorial/errors.html#user-defined-exceptions
  title: Errors and Exceptions, The Python Tutorial
  locator: 8.6 User-defined Exceptions
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Error
  title: Error, MDN Web Docs
  locator: Custom error types; Error.prototype.name
- url: https://docs.oracle.com/javase/tutorial/essential/exceptions/creating.html
  title: Creating Exception Classes, The Java Tutorials
  locator: Choosing a Superclass
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Exception.html
  title: Class Exception, Java SE 25 API
  locator: the class description
- url: https://learn.microsoft.com/en-us/dotnet/standard/exceptions/best-practices-for-exceptions
  title: Best practices for exceptions, .NET documentation
  locator: Custom exception types
- url: https://learn.microsoft.com/en-us/dotnet/standard/exceptions/how-to-create-user-defined-exceptions
  title: How to create user-defined exceptions, .NET documentation
  locator: the introduction
- url: https://en.cppreference.com/w/cpp/error/runtime_error.html
  title: std::runtime_error, cppreference.com
  locator: the description; Member functions
- url: https://go.dev/blog/error-handling-and-go
  title: Andrew Gerrand, Error handling and Go, The Go Blog, 12 July 2011
  locator: The error type
- url: https://pkg.go.dev/errors#As
  title: Package errors, Go standard library
  locator: func As; func AsType
- url: https://doc.rust-lang.org/std/error/trait.Error.html
  title: std::error::Error, Rust standard library
  locator: the trait description
- url: https://docs.rs/thiserror/latest/thiserror/
  title: thiserror, docs.rs
  locator: the crate description; Details
- url: https://www.gnu.org/software/bash/manual/html_node/Exit-Status.html
  title: Exit Status, Bash Reference Manual
  locator: the first paragraph
- url: https://www.gnu.org/software/bash/manual/html_node/Command-Execution-Environment.html
  title: Command Execution Environment, Bash Reference Manual
  locator: the subshell paragraph
- url: https://docs.python.org/3.15/library/exceptions.html
  title: Built-in Exceptions, The Python Standard Library
  locator: the introduction
- url: https://go.dev/doc/faq#nil_error
  title: Frequently Asked Questions (FAQ), The Go Programming Language
  locator: Why is my nil error value not equal to nil?
---

## Task

Define an error type for one kind of failure, give it fields that describe the failure, and let the caller pick out that type. The function `parse_port(text)` reads a TCP port number. When the text is bad, it reports a `PortError` with two fields:

- `text`: the text that failed,
- `reason`: `"empty"`, `"not a number"` or `"out of range"`.

The text is good when it has only the ASCII digits 0 to 9, and its value is from 1 to 65535. The function `check_port(text)` calls `parse_port`. It returns `"port N"` on success. On a `PortError`, it returns the message of the error, `"invalid port 'TEXT': REASON"`. For example, `check_port("8080")` is `"port 8080"` and `check_port("x80")` is `"invalid port 'x80': not a number"`.

A general error, such as a string or a plain `Exception`, has only a message. A program that must act on the reason must then read the message. A custom type gives the reason to the program as a field, and its name tells the handler which errors to take. [[error-concept]] explains what an error is. [[exception-handling]] and [[errors-as-values]] are the two ways to report one. The type in this task works with both.

## Solutions

| Language | The error type | How the caller picks out the type |
|---|---|---|
| Python | `class PortError(Exception)` | `except PortError as e:` |
| JavaScript, TypeScript | `class PortError extends Error` | `e instanceof PortError` |
| Java | `class PortException extends Exception` | `catch (PortException e)` |
| C# | `class PortException : Exception` | `catch (PortException e)` |
| C++ | `class PortError : public std::runtime_error` | `catch (const PortError &e)` |
| Go | `type PortError struct` with an `Error` method | `errors.As(err, &pe)` |
| Rust | `struct PortError` that implements `Error` | `match` on the `Result` |
| C | `struct port_error` and `enum port_reason` | the return value `-1` |
| Bash | two global variables and the exit status | `if parse_port "$1"` |

- **Python.** "Exceptions should typically be derived from the Exception class, either directly or indirectly"[^1]. The tutorial adds that such classes "are usually kept simple, often only offering a number of attributes that allow information about the error to be extracted by handlers for the exception"[^1]. `PortError` passes the message to `Exception.__init__` and keeps `text` and `reason` as attributes. Most exception names end in "Error"[^1], so the solution uses `PortError`.
- **JavaScript and TypeScript.** MDN gives the reason for a subclass: you can "define your own error types deriving from Error to be able to throw new MyError() and use instanceof MyError to check the kind of error in the exception handler"[^2]. The `catch` clause of the solution tests the type with `instanceof` and throws other errors again. The constructor also sets `this.name`, as the MDN example does[^2]. The TypeScript solution is the same code with types on the fields.
- **Java.** The tutorial names the parent: "the parent class of LinkedListException should be Exception"[^3], and it says that "it's good practice to append the string Exception to the names of all classes that inherit (directly or indirectly) from the Exception class"[^3]. So the type is `PortException`. A subclass of `Exception` that is not a subclass of `RuntimeException` is a checked exception, and "Checked exceptions need to be declared in a method or constructor's throws clause"[^4]. So `parsePort` has `throws PortException`.
- **C#.** The .NET guide says: "When a custom exception is necessary, name it appropriately and derive it from the Exception class"[^5]. It asks for more properties "only when there's a programmatic scenario where the additional information is useful"[^5]. Here the caller can act on `Reason`, so the type has the properties `Text` and `Reason`. The guide also tells you to "implement the three common constructors"[^6]. This short solution has only the constructor that it uses.
- **C++.** The solution derives from `std::runtime_error`, which "reports errors that are due to events beyond the scope of the program and cannot be easily predicted"[^7]. Bad input from a user is such an event. The constructor of `runtime_error` "Constructs the exception object with what_arg as explanatory string"[^7], so `what()` gives the message. The handler catches by reference to `const`.
- **Go.** "The error type is an interface type. An error variable represents any value that can describe itself as a string"[^8]. So a struct with an `Error() string` method is an error. The Go blog shows the same pattern with `json.SyntaxError`, whose field `Offset` "isn’t even shown in the default formatting of the error, but callers can use it"[^8]. The caller gets the struct back with `errors.As`, which "finds the first error in err's tree that matches target, and if one is found, sets target to that error value and returns true"[^9]. The documentation says "For most uses, prefer AsType"[^9]. "As is equivalent to AsType but sets its target argument rather than returning the matching error"[^9], so either one works here.
- **Rust.** "Errors must describe themselves through the Display and Debug traits"[^10]. The solution derives `Debug`, writes `Display` by hand, and adds an empty `impl Error`. The reason is an enum, so a `match` on it must cover each reason. The standard library also says that error messages "are typically concise lowercase sentences without trailing punctuation"[^10], and the messages of the solution follow that rule. The crate thiserror "provides a convenient derive macro for the standard library’s std::error::Error trait"[^11], so a larger program can put an `error` attribute with the message on each variant in place of the `Display` code. "Thiserror deliberately does not appear in your public API"[^11].
- **C.** The solution has no exceptions. `parse_port` returns `-1` and fills a `struct port_error` that the caller passes in. The reason is an `enum`, and `port_reason_name` turns it into text. [[error-codes]] explains this style.
- **Bash.** A function can report only an exit status, and "a non-zero exit status indicates failure"[^12]. So `parse_port` returns 1 and puts the fields in the global variables `PORT_ERROR_TEXT` and `PORT_ERROR_REASON`. The caller must not call it in `$( )`. Command substitution runs in a subshell, and a command in a subshell "cannot affect the shell’s execution environment"[^13]. The variables would be lost.

### The fields at work

The handler can act on the reason without reading the message. Here only an `"out of range"` error gets a hint:

```python run id=python-reason-field
class PortError(Exception):
    def __init__(self, text, reason):
        super().__init__(f"invalid port '{text}': {reason}")
        self.text = text
        self.reason = reason


def parse_port(text):
    if text == "":
        raise PortError(text, "empty")
    if any(c not in "0123456789" for c in text):
        raise PortError(text, "not a number")
    port = int(text)
    if not 1 <= port <= 65535:
        raise PortError(text, "out of range")
    return port


for text in ["8080", "x80", "70000"]:
    try:
        print("port", parse_port(text))
    except PortError as e:
        print(e)
        if e.reason == "out of range":
            print("  hint: use a port from 1 to 65535")
```

```output for=python-reason-field
port 8080
invalid port 'x80': not a number
invalid port '70000': out of range
  hint: use a port from 1 to 65535
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: the empty text has its own reason, `"empty"`, so that the message does not say that nothing is "not a number".
- `zero` and `one`: 0 has only digits, but the task takes ports from 1 to 65535, so the reason is `"out of range"`. 1 is the smallest port.
- `largest` and `too-large`: 65535 is the largest port, and 65536 is out of range.
- `overflow`: twenty digits do not fit in a 64-bit integer. The solutions stop when the value goes above 65535, so the reason is `"out of range"`, not an overflow.
- `leading-zeros`: `"0080"` has only digits, and its value is 80.
- `negative` and `leading-space`: a sign or a space is not a digit, so the reason is `"not a number"`. The solutions test each character before they compute the value.

## Pitfalls

- **The wrong base class.** In Python, `KeyboardInterrupt` "inherits from BaseException so as to not be accidentally caught by code that catches Exception"[^14]. The same rule applies to each subclass of `BaseException`. The documentation tells programmers "to derive new exceptions from the Exception class or one of its subclasses, and not from BaseException"[^14]. A handler for `Exception` does not catch an error that derives from `BaseException`:

```python wrong id=python-base-exception
class PortError(BaseException):
    pass


try:
    try:
        raise PortError("invalid port 'x80': not a number")
    except Exception:
        print("handled")
except BaseException as e:
    print("not handled by except Exception:", type(e).__name__)
```

```output for=python-base-exception
not handled by except Exception: PortError
```

- **No name in JavaScript.** For `Error.prototype.name`, "the initial value is "Error""[^2]. A subclass that does not set `name` prints as a plain `Error`:

```javascript wrong id=javascript-no-name
class PortError extends Error {}

const e = new PortError("invalid port 'x80': not a number");
console.log(String(e));
console.log(e instanceof PortError);
```

```output for=javascript-no-name
Error: invalid port 'x80': not a number
true
```

The type is right, but a log shows only `Error`. Set `this.name = "PortError"` in the constructor.

- **A nil pointer in an error.** In Go, an interface value holds a type and a value. The FAQ says that "if any concrete value has been stored in the interface, the interface will not be nil"[^15]. A function that returns a `*PortError` variable as an `error` returns a non-nil error even when the pointer is nil:

```go wrong id=go-typed-nil
package main

import "fmt"

type PortError struct{ Text, Reason string }

func (e *PortError) Error() string {
	return "invalid port '" + e.Text + "': " + e.Reason
}

func parsePort(text string) (int, error) {
	var perr *PortError // stays nil when the text is good
	if text == "" {
		perr = &PortError{text, "empty"}
	}
	return 80, perr
}

func main() {
	_, err := parsePort("80")
	fmt.Println("err != nil:", err != nil)
}
```

```output for=go-typed-nil
err != nil: true
```

The FAQ gives the fix: "To return a proper nil error to the caller, the function must return an explicit nil"[^15]. The Go solution returns `nil` on success, and the result type of `ParsePort` is `error`, not `*PortError`.

- **A test on the message.** A handler that compares the message, such as `if "out of range" in str(e)`, breaks when someone changes the words of the message. Test the type and the fields. Keep the message for people.

## Related tasks

- [[wrap-an-error-with-context]]: add context to an error, and keep the error as its cause.
- [[log-an-error]]: write an error to the log with its fields.
- [[pattern-match-on-a-value]]: a `match` on an enum with data, such as the reason in the Rust solution.

## References
