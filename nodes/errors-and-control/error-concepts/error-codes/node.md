---
tag: A63VKT
kind: concept
slug: error-codes
name: Error codes
aliases: [error code, return code, status code, errno, HRESULT, exit code]
area: errors-and-control/error-concepts
level: core
summary: >-
  An error code is a number that a function returns, or stores in a known place, to tell the caller which error happened. The caller must test it after each call.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1990
  people: []
  note: The first ISO C standard, ISO/IEC 9899:1990, defined errno in the header errno.h, in section 4.1.3. The Seventh Edition Unix manual had already described system calls that return -1 and put an error number in errno.
see_also: [error-concept, error-handling-strategies, errors-as-values, exception-handling, recoverable-and-unrecoverable-errors]
sources:
- url: https://en.cppreference.com/w/c/error/errno.html
  title: errno, cppreference.com (C)
  locator: Description; References
- url: https://man7.org/linux/man-pages/man3/errno.3.html
  title: errno(3), Linux manual page
  locator: DESCRIPTION
- url: https://docs.python.org/3.15/library/errno.html
  title: errno, Python Standard Library
  locator: errno.errorcode
- url: http://man.cat-v.org/unix_7th/2/intro
  title: intro(2), Unix Seventh Edition manual
  locator: DESCRIPTION
- url: https://learn.microsoft.com/en-us/windows/win32/com/structure-of-com-error-codes
  title: Structure of COM Error Codes, Win32 documentation
  locator: Whole page
- url: https://learn.microsoft.com/en-us/dotnet/api/system.exception.hresult
  title: Exception.HResult Property, .NET API
  locator: Definition; Remarks
- url: https://www.gnu.org/software/bash/manual/bash.html#Exit-Status
  title: Bash Reference Manual
  locator: 3.2.3 Pipelines; 3.2.4 Lists of Commands; 3.7.5 Exit Status
- url: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#e28-avoid-error-handling-based-on-global-state-eg-errno
  title: C++ Core Guidelines
  locator: E.28
- url: https://en.cppreference.com/w/cpp/utility/from_chars.html
  title: std::from_chars, cppreference.com
  locator: Return value; Exceptions
- url: https://en.cppreference.com/w/cpp/language/attributes/nodiscard.html
  title: C++ attribute nodiscard, cppreference.com
  locator: Explanation
- url: https://nodejs.org/api/errors.html#errorcode
  title: Errors, Node.js documentation
  locator: error.code
- url: https://pkg.go.dev/syscall#Errno
  title: Package syscall, Go standard library
  locator: type Errno
- url: https://doc.rust-lang.org/std/io/struct.Error.html
  title: std::io::Error, Rust standard library
  locator: last_os_error; raw_os_error
- url: https://docs.oracle.com/javase/tutorial/essential/exceptions/advantages.html
  title: Advantages of Exceptions, The Java Tutorials
  locator: Advantage 1; Advantage 2
---

## Idea

An error code is a number that tells the caller which error happened. The function returns it, or stores it in a known place, and the caller tests it after the call. A code of zero often means success, and each other value names one error.

C uses error codes in its standard library. Several functions "indicate errors by writing positive integers to errno"[^1], and the man page says that the value "is significant only when the return value of the call indicated an error"[^2]. So the caller tests the return value first, and reads `errno` only after a failure.

```c run id=idea
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    FILE *f = fopen("/no/such/file", "r");
    if (f == NULL) {
        int code = errno;
        printf("fopen failed: errno = %d (%s)\n", code, strerror(code));
        return 0;
    }
    fclose(f);
    return 0;
}
```

```output for=idea
fopen failed: errno = 2 (No such file or directory)
```

`fopen` returns `NULL`, the program copies `errno` at once, and `strerror` turns the number into a message[^2]. The number 2 is the code for `ENOENT` on Linux, but "The error numbers that correspond to each symbolic name vary across UNIX systems"[^2], so a program tests the name, not the number.

## How it works

A design with error codes has three parts.

1. **A set of codes.** Each code is a named constant. C names its codes with macros that begin with the letter `E`[^1], and Python's module `errno` maps each number to its name, so that `errno.errorcode[errno.EPERM]` "maps to 'EPERM'"[^3].
2. **A channel for the code.** The function returns the code, or it returns a special value and puts the code in a variable. The Seventh Edition Unix manual describes the second form: "An error condition is indicated by an otherwise impossible returned value. Almost always this is -1"[^4], and the error number is in `errno`.
3. **A test at the caller.** The caller tests the code after each call and decides what to do. A code says nothing until a person or a program reads it.

Some codes carry more than one fact. A Windows `HRESULT` is 32 bits: "The high-order bit in the HRESULT or SCODE indicates whether the return value represents success or failure"[^5], and other fields hold a facility, which names the part of the system, and the code itself[^6].

A shell uses the same idea for whole programs. In Bash, `command1 && command2` runs the second command "if, and only if, command1 returns an exit status of zero (success)"[^7]. A status that is not zero is an error code.

## In languages

- **C** has no exceptions, so return codes and `errno` are its usual mechanism[^1]. The C++ Core Guidelines advise against the `errno` style: "Global state is hard to manage and it is easy to forget to check it."[^8]
- **C++** usually throws exceptions, but some functions return a code. `std::from_chars` "Throws nothing" and reports a failure in its result, with `ec` equal to `std::errc::result_out_of_range` when the number is too large[^9]. The attribute `[[nodiscard]]` asks the compiler for a warning when a caller ignores a result: "the compiler is encouraged to issue a warning"[^10].
- **Bash** uses the exit status of each command and function as its error code[^7].
- **Java, C#, Python and JavaScript** report errors with exceptions. Codes still appear inside them: an exception in .NET has an `HResult`, "a coded numerical value that is assigned to a specific exception"[^6], and a Node.js error has `error.code`, "a string label that identifies the kind of error"[^11].
- **Go and Rust** return errors as values, not as bare integers. They keep the operating system code inside the value. In Go, "An Errno is an unsigned number describing an error condition. It implements the error interface."[^12] In Rust, `raw_os_error` "Returns the OS error that this error represents (if any)"[^13].

## Variations and related ideas

- **Codes and exceptions.** The Java tutorial shows a file reader written with error codes, and lists as the first advantage of exceptions "Separating Error-Handling Code from "Regular" Code"[^14]. With codes, each function between the error and the handler must also return the code up the call stack[^14]. See exception-handling.
- **Codes inside error values.** A Go `error` or a Rust `Result` can hold a code and also a message and a cause[^13][^12]. See errors-as-values.
- **Status codes of a process.** A program returns an exit status to the shell that ran it, and the shell tests it with `&&`, `||` and `if`[^7].
- **Codes in an exception.** A .NET exception and a Node.js error carry a code, so a handler can test the code instead of the message[^6][^11]. Node.js says that `error.code` "is the most stable way to identify an error"[^11].

## Pitfalls

### Testing the code the wrong way round

A function that returns 0 for success makes the test look inverted. In C, `if (f())` is true when `f` fails.

```c wrong id=c-inverted-test
#include <stdio.h>
#include <string.h>

/* Returns 0 for success and 1 for an error. */
static int parse_digit(const char *text, int *out) {
    if (strlen(text) != 1 || text[0] < '0' || text[0] > '9') return 1;
    *out = text[0] - '0';
    return 0;
}

int main(void) {
    int value = -1;
    if (parse_digit("x", &value)) {
        printf("parsed %d\n", value);
    }
    return 0;
}
```

```output for=c-inverted-test
parsed -1
```

The test reads like "if the parse worked", but it is true for the error code 1. The program then uses a value that `parse_digit` never set. Write the test as `if (parse_digit("x", &value) != 0)` for the error case.

### A stale errno

`errno` keeps its old value after a call that succeeds. The man page says that "a function that succeeds is allowed to change errno" and that "The value of errno is never set to zero by any system call or library function"[^2].

```c wrong id=c-stale-errno
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *f = fopen("/no/such/file", "r");
    if (f != NULL) fclose(f);
    long n = strtol("42", NULL, 10);
    if (errno != 0) {
        printf("strtol failed? errno = %d, n = %ld\n", errno, n);
    }
    return 0;
}
```

```output for=c-stale-errno
strtol failed? errno = 2, n = 42
```

`strtol` worked, but `errno` still holds the code from `fopen`. The man page gives the fix for functions where the return value cannot show an error: set `errno` to zero before the call, and test it after[^2].

### A lost exit status in a pipeline

In Bash, "The exit status of a pipeline is the exit status of the last command in the pipeline, unless the pipefail option is enabled"[^7]. So an error in the first command is lost.

```bash wrong id=bash-pipeline-status
grep "x" /no/such/file 2>/dev/null | sort
echo "status $?"
```

```output for=bash-pipeline-status
status 0
```

`grep` fails, but `sort` succeeds, so the status is 0. With `set -o pipefail`, the status is that of "the last (rightmost) command to exit with a non-zero status"[^7], here the status of `grep`.

## Cost

Returning a code needs no allocation and no exception. For example, `std::from_chars` is "locale-independent, non-allocating, and non-throwing"[^9].

The cost is in the code that calls it. The Java tutorial's file reader with error codes needs a nested `if` for each step[^14], and each caller up the stack must test and return the code again[^14]. The Core Guidelines add that "it is easy to forget to check it"[^8].

## History

The Seventh Edition Unix manual describes the convention that C still uses. A system call returns an impossible value, "Almost always this is -1", and "An error number is also made available in the external variable errno"[^4]. It also warns that "Errno is not cleared on successful calls, so it should be tested only after an error has occurred"[^4].

The first ISO C standard, ISO/IEC 9899:1990, defined `errno` in section 4.1.3, "Errors <errno.h>"[^1]. Each later C standard keeps it[^1].

## References
