---
tag: NEW-errors-as-values
kind: concept
slug: errors-as-values
name: Error value
aliases: [errors as values, error code, return code, result type]
area: errors-and-control/error-values
level: core
summary: >-
  An error value is an ordinary value that a function returns to say that it failed. The caller tests the value and decides what to do next, so the path of each failure is visible in the code.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2015
  people: [Rob Pike]
  note: Pike's essay "Errors are values" stated the Go view that an error is a value like any other, so a program can store it, pass it and compute with it.
see_also: [exception-handling, sum-types, null-references, higher-order-functions, panics, assertions]
same_as:
  wikidata: Q25345885
  wikipedia: Result_type
sources:
  - url: https://go.dev/blog/errors-are-values
    title: Rob Pike, Errors are values, The Go Blog, 12 January 2015
    locator: the errWriter example and the sentence "Values can be programmed, and since errors are values, errors can be programmed."
  - url: https://go.dev/ref/spec#Errors
    title: The Go Programming Language Specification
    locator: Errors
  - url: https://pkg.go.dev/errors
    title: Go standard library, package errors
    locator: Overview (wrapping and the error tree), func Is, func As, func Join
  - url: https://pkg.go.dev/fmt#Errorf
    title: Go standard library, package fmt
    locator: func Errorf (the verb %w)
  - url: https://go.dev/doc/go1.13#error_wrapping
    title: Go 1.13 Release Notes
    locator: Error wrapping
  - url: https://doc.rust-lang.org/reference/expressions/operator-expr.html#the-question-mark-operator
    title: The Rust Reference, Operator expressions
    locator: The question mark operator (r-expr.try.intro, r-expr.try.restricted-types)
  - url: https://doc.rust-lang.org/book/ch09-02-recoverable-errors-with-result.html#a-shortcut-for-propagating-errors-the--operator
    title: The Rust Programming Language, Recoverable Errors with Result
    locator: A Shortcut for Propagating Errors, the ? operator (the call of the from function)
  - url: https://doc.rust-lang.org/std/result/index.html#results-must-be-used
    title: Rust standard library, module std::result
    locator: Results must be used
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: ISO/IEC 9899 working draft N3220 (C23)
    locator: 7.5 Errors <errno.h>, paragraphs 2 and 3, footnote 237
  - url: https://hackage.haskell.org/package/base/docs/Data-Either.html#t:Either
    title: Haskell base library, Data.Either
    locator: data Either a b (the convention that Left holds an error and Right a correct value)
  - url: https://timsong-cpp.github.io/cppwp/n4950/expected.general
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[expected.general], paragraph 1"
  - url: https://go.dev/doc/faq#exceptions
    title: Go FAQ
    locator: Why does Go not have exceptions?
  - url: https://go.dev/doc/faq#nil_error
    title: Go FAQ
    locator: Why is my nil error value not equal to nil?
  - url: https://blog.rust-lang.org/2016/11/10/Rust-1.13/
    title: Announcing Rust 1.13, The Rust Blog, 10 November 2016
    locator: The ? operator
  - url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin/-result/
    title: Kotlin standard library, class Result
    locator: the class description
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.int32.tryparse
    title: .NET API reference, Int32.TryParse Method
    locator: the summary of each overload
  - url: https://doc.rust-lang.org/book/ch09-03-to-panic-or-not-to-panic.html#guidelines-for-error-handling
    title: The Rust Programming Language, To panic! or Not to panic!
    locator: Guidelines for Error Handling
  - url: https://timsong-cpp.github.io/cppwp/n4950/expected.object.obs
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[expected.object.obs], paragraphs 3 and 9 (the precondition of operator* and the exception of value)"
  - url: https://go.dev/ref/spec#Expression_statements
    title: The Go Programming Language Specification
    locator: Expression statements
  - url: https://man7.org/linux/man-pages/man1/bash.1.html#EXIT_STATUS
    title: bash(1), Linux manual page
    locator: EXIT STATUS; Special Parameters, ?
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_automatic_variables
    title: PowerShell documentation, about_Automatic_Variables
    locator: $? and $Error
  - url: https://go.dev/doc/devel/release#go1.13
    title: Go Release History
    locator: go1.13 (released 2019-09-03)
  - url: https://tc39.es/ecma262/2026/#sec-installerrorcause
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 20.5.8.1 InstallErrorCause
  - url: https://tc39.es/ecma262/2026/#sec-promise.allsettled
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 27.2.4.2 Promise.allSettled
---

## Idea

A function can fail. A file is missing, a text is not a number, a server does not answer. The function must tell its caller. One way is to return the failure as a value, next to the result or in place of it. The caller receives the value, tests it, and decides what to do: try something else, add context and pass the error up, or report it.

This is the error value approach. It treats a failure as data, not as a jump in control. Rob Pike stated the idea for Go: "Values can be programmed, and since errors are values, errors can be programmed."[^1] The other approach is the exception, where the failing function throws and control jumps to a handler further up the call stack ([[exception-handling]]).

In Go, `strconv.Atoi` returns two results: the number and an error. The caller tests the error before it uses the number.

```go run id=idea
package main

import (
	"fmt"
	"strconv"
)

func main() {
	for _, text := range []string{"8080", "x"} {
		n, err := strconv.Atoi(text)
		if err != nil {
			fmt.Println("error:", err)
			continue
		}
		fmt.Println("port", n)
	}
}
```

```output
port 8080
error: strconv.Atoi: parsing "x": invalid syntax
```

The `if err != nil` test is part of the normal code. A reader sees each place where the function can fail and what happens there.

## How it works

At the level of the machine, an error value is a return value like any other. The function puts it in a register or in memory, and the caller compares it and branches. No hidden mechanism moves control. To pass an error up, each function on the path returns it to its own caller. This is the main difference from an exception, where the runtime unwinds many frames in one step.

Languages represent the error in four main ways.

| Form | How the failure is reported | Examples |
|---|---|---|
| a special value of the result | a value that cannot be a valid result, such as `-1` or `NULL`; the reason is often in a separate variable | C `fopen` and `errno`, Python `str.find` |
| a status code and an out parameter | the return value says success or failure; the result goes through a pointer or `out` parameter | C status codes, C# `int.TryParse`[^16] |
| a second result | the function returns the result and an error together | Go `(T, error)`[^2] |
| a sum type | one value that is either a success with a result or a failure with an error | Rust `Result<T, E>`, C++ `std::expected<T, E>`,[^11] Haskell `Either`[^10] |

The special value is the oldest form, and it has two weaknesses. The caller can use the special value as if it were a result, and the reason for the failure must go elsewhere. C puts the reason in `errno`. A library function sets `errno` to a positive number when it fails, and no library function sets it to zero. So a program that wants to test `errno` sets it to zero before the call.[^9]

A sum type removes both weaknesses. The value is either `Ok(value)` or `Err(error)`, never both. The result is not reachable without a test of which case is present, and the compiler can check that the code handles both cases ([[sum-types]]). Haskell's `Either a b` follows a convention: `Left` holds the error and `Right` holds the correct value.[^10] C++ `std::expected<T, E>` holds a value of type `T` or an `unexpected<E>`.[^11]

### Passing an error up

When a function cannot handle an error, it passes the error to its caller. With error values, the code must say so at each level. Go writes an `if` statement that returns the error. Rust has the operator `?`. On `Ok(v)` the expression gives `v`. On `Err(e)` the function returns at once with `Err(From::from(e))`.[^6] The call of `From::from` converts the error to the error type of the enclosing function, so a low-level error such as `ParseIntError` becomes the error type of the module.[^7] The operator `?` also works on `Option`.[^6]

```rust run id=question
use std::num::ParseIntError;

fn sum(a: &str, b: &str) -> Result<i32, ParseIntError> {
    let x: i32 = a.parse()?;
    let y: i32 = b.parse()?;
    Ok(x + y)
}

fn main() {
    println!("{:?}", sum("2", "40"));
    println!("{:?}", sum("2", "forty"));
}
```

```output
Ok(42)
Err(ParseIntError { kind: InvalidDigit })
```

### Wrapping and inspection

An error that goes up through several layers needs context. "invalid syntax" says little. "load config: port setting: invalid syntax" says where it happened. Wrapping makes a new error that holds the old one. In Go, `fmt.Errorf` with the verb `%w` returns an error with an `Unwrap` method that gives the wrapped error.[^4] The errors then form a chain, or a tree when one error wraps several.[^3]

A caller that needs to know the kind of error must look through the chain. `errors.Is` reports whether any error in the tree matches a target value, and `errors.As` finds the first error in the tree that has a given type.[^3] A comparison with `==` sees only the outer error, so it fails after wrapping (see the Go example). JavaScript has the same idea in the option `cause` of the `Error` constructor, which sets a property `cause` on the new error.[^23]

### Error values and exceptions

| Question | Error values | Exceptions |
|---|---|---|
| Is the failure visible in the signature? | yes, in the return type | only with checked exceptions (Java) |
| Is the failure visible at the call site? | yes: a test, or `?` | no; any call can throw |
| How does the error go up? | each level returns it | the runtime unwinds the frames |
| What happens when the caller forgets? | the error is lost, or the compiler warns | the program stops with an unhandled exception |
| What does the normal path cost? | a test and a branch after each call | nothing in a table-based implementation |

The Go designers chose error values on purpose. The FAQ says that coupling exceptions to a control structure "results in convoluted code" and that it leads programmers to label too many ordinary errors as exceptional.[^12] The Rust book gives a rule for the choice inside one language: return a `Result` when failure is expected, such as a parser that gets malformed data, and panic when the program reaches a bad state that the code should never reach.[^17] The second case is the subject of [[panics]].

## In languages

The example files read a port number from a configuration in each language. The key can be missing, and the text can be invalid. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | idiom | a union of two dataclasses tested with `match`; the language uses exceptions |
| JavaScript | idiom | an object with an `ok` field; `Promise.allSettled` gives outcomes as values |
| TypeScript | idiom | a discriminated union type that the checker narrows |
| Java | idiom | a sealed interface with two records; `Optional` for absence |
| C | idiom | a status code, a special return value and `errno` |
| C++ | stdlib | `std::expected<T, E>` and `std::error_code` |
| C# | idiom | the Try pattern, `bool TryParse(string, out int)`; a record hierarchy |
| Go | native | the interface `error` as the last result; wrapping with `%w` |
| Rust | stdlib | `Result<T, E>` and the operator `?` |
| Kotlin | stdlib | the class `Result<T>` |
| PowerShell | idiom | the variable `$?` and the error records in `$Error`[^21] |
| Bash | idiom | the exit status of each command, `0` for success, in `$?`[^20] |
| SQL | absent | the engine reports a failure to the host program as a status |
| Regex | n/a | a pattern does not return errors |
| HTML/CSS | n/a | markup and style rules do not return errors |

Kotlin has the class `Result`, a discriminated union of a success with a value and a failure with a `Throwable`.[^15] The failure case holds a `Throwable`, so `Result` wraps the exceptions of the JVM and does not replace them.

## Variations and related ideas

- **Absence without a reason.** `Option` in Rust, `Optional` in Java and a nullable type say only that a value is missing. They suit a lookup that may find nothing, not a failure with a cause ([[null-references]]).
- **Sticky errors.** Pike's `errWriter` stores the first error and turns each later write into a no-op. The caller tests the error once at the end, in place of after each write.[^1]
- **Joined errors.** `errors.Join` in Go makes one error from several and drops the `nil` ones.[^3] `Promise.allSettled` in JavaScript gives a snapshot of the state of each promise after all of them have settled.[^24] Both suit a batch, where one failure should not hide the others.
- **Chaining with functions.** `and_then` on a C++ `std::expected` and on a Rust `Result` calls the next step only on success, and it passes an error on unchanged. This is error handling with [[higher-order-functions]].
- **The Try pattern.** .NET methods such as `Int32.TryParse` return a Boolean that says whether the conversion succeeded and give the result through an `out` parameter.[^16]
- **Exceptions at the boundary.** Code that uses error values often calls a library that throws. The Java and Python examples catch the exception at the boundary and turn it into a value.

## Pitfalls

### An ignored error

An error value is lost when the caller does not test it. In Go, the blank identifier `_` discards the error, and a call statement may ignore all the results of the call.[^19] Here the text is not a number. `strconv.Atoi` returns `0` and an error, the error goes to `_`, and the program uses `0` as the port.

```go wrong id=ignored
package main

import (
	"fmt"
	"strconv"
)

func main() {
	port, _ := strconv.Atoi("80a")
	fmt.Println("listening on port", port)
}
```

```output
listening on port 0
```

The fix is to test the error before the value is used. Rust marks `Result` with `#[must_use]`, so the compiler warns when a caller drops a `Result` without a look at it.[^8] A Rust caller cannot reach the number without a `match`, `?`, or a method such as `unwrap` that panics on an error (see the Rust example).

### A stale errno

`errno` is not cleared by a call that succeeds. A program that tests `errno` without first setting it to zero can see the error of an earlier call.[^9] Here `fopen` fails and sets `errno`. Then `strtol` succeeds, but `errno` still holds the old value, and the program reports a failure that did not happen.

```c wrong id=stale-errno
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *file = fopen("/no/such/dir/settings.conf", "r");
    if (file != NULL) {
        fclose(file);
    }
    long n = strtol("42", NULL, 10);
    if (errno != 0) {
        printf("strtol failed, errno is ENOENT: %d\n", errno == ENOENT);
    } else {
        printf("n = %ld\n", n);
    }
}
```

```output
strtol failed, errno is ENOENT: 1
```

The fix is `errno = 0;` right before the call, and a test of `errno` right after it, before any other library call. Test `errno` only when the function's return value says that it may have failed.

### A typed nil is not a nil error

A Go interface value holds a type and a value. It is `nil` only when both are unset.[^13] A function that returns a `nil` pointer of a concrete error type, such as `*PortError`, through the result type `error` returns an interface that holds the type `*PortError`. The caller's test `err != nil` is then true. The Go example shows the program and its output. The fix is to return the literal `nil` on success.

### Wrapping with the wrong verb

In Go, `fmt.Errorf` wraps only with `%w`. With `%v` it formats the old error as text, so `errors.Is` and `errors.As` cannot find it.[^4]

```go wrong id=verb
package main

import (
	"errors"
	"fmt"
	"io/fs"
)

func main() {
	wrapped := fmt.Errorf("load config: %w", fs.ErrNotExist)
	formatted := fmt.Errorf("load config: %v", fs.ErrNotExist)
	fmt.Println(errors.Is(wrapped, fs.ErrNotExist))
	fmt.Println(errors.Is(formatted, fs.ErrNotExist))
}
```

```output
true
false
```

Both errors print the same message, so the bug shows only when a caller tests the kind of the error.

### A value read without a test

`std::expected` lets the caller read the value with `*` without a test. When the object holds an error, this breaks the precondition `has_value()`, and the behaviour is undefined. The member function `value()` throws `bad_expected_access` in that case.[^18] Rust's `unwrap` panics in the same situation. Both are a way back from an error value to a crash, so use them only where the code has already proved that the value is present.

## Cost

- **The normal path.** Each call that can fail is followed by a test and a branch. The test runs on success too. An exception has no such test, and it is expensive only when it is thrown ([[exception-handling]]).
- **Size.** A sum type carries a tag and room for the larger of its two cases. A Go function returns its result and an interface value, which holds a type and a pointer to the value.[^13]
- **Allocation.** In Go, `fmt.Errorf` makes a new error at each call, so wrapping adds one more error for each layer that adds context.[^4] A sentinel error in a package variable is made once.
- **Code size.** Each level that passes an error up adds a branch and a return. The Rust operator `?` hides the branch in the source, but the compiled code still has it.[^6]

## History

- **2015.** Rob Pike published "Errors are values" on the Go blog. It showed that error handling in Go can be written as ordinary code over values, such as a writer that keeps the first error.[^1]
- **2016.** Rust 1.13 added the operator `?`. It replaced the macro `try!`, which did the same thing.[^14]
- **2019.** Go 1.13, released on 3 September 2019, added error wrapping: the verb `%w` of `fmt.Errorf`, and the functions `errors.Unwrap`, `errors.Is` and `errors.As`.[^5][^22]

## References
