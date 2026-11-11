---
tag: E7DS4A
kind: concept
slug: exception-handling
name: Exception
aliases: [exception handling, raise and catch, throw and catch]
area: errors-and-control/exceptions
level: core
summary: >-
  An exception is an object that a program raises when an operation cannot complete. Control goes up the call stack to the nearest matching handler, and the cleanup code of each frame runs on the way.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1975
  people: [Barbara Liskov]
  note: Liskov's group made the main decisions about the exception mechanism of CLU by June 1975. CLU used the termination model, in which a handler takes the place of the rest of the operation that raised.
see_also: [async-await, iterators, recursion, static-and-dynamic-typing, garbage-collection]
same_as:
  wikidata: Q121191894
  wikipedia: Exception_handling_(programming)
sources:
  - url: https://www.stroustrup.com/except89.pdf
    title: Andrew Koenig and Bjarne Stroustrup, Exception Handling for C++ (revised), Proceedings of the USENIX C++ Conference, 1990
    locator: 1 Introduction; 5.2 Throwing Exceptions in Destructors; 7 Termination vs. Resumption; 16.5 Outline of an Efficient Implementation
  - url: https://docs.python.org/3.14/reference/compound_stmts.html#the-try-statement
    title: The Python Language Reference, Compound statements, Python 3.14
    locator: "8.4 The try statement: 8.4.1 except clause, 8.4.4 finally clause"
  - url: https://timsong-cpp.github.io/cppwp/n4950/except.handle
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[except.handle], paragraphs 6 and 9"
  - url: https://timsong-cpp.github.io/cppwp/n4950/except.ctor
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[except.ctor], paragraphs 1 and 2"
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/exception-handling-statements
    title: C# language reference, Exception-handling statements
    locator: The throw statement; The try-catch statement; A when exception filter
  - url: https://pmg.csail.mit.edu/papers/clu-history.pdf
    title: Barbara Liskov, A History of CLU, HOPL-II, 1993
    locator: 3.9 Exception Handling
  - url: https://www.lispworks.com/documentation/HyperSpec/Body/09_ad.htm
    title: Common Lisp HyperSpec
    locator: 9.1.4 Signaling and Handling Conditions
  - url: https://www.lispworks.com/documentation/HyperSpec/Body/09_adb.htm
    title: Common Lisp HyperSpec
    locator: 9.1.4.2 Restarts
  - url: https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html#base-framework
    title: "Itanium C++ ABI: Exception Handling"
    locator: 1.1 Exception Handler Framework; 1.6 Personality Routine
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-11.html#jls-11.2.3
    title: The Java Language Specification, Java SE 25 Edition
    locator: 11.1.1 The Kinds of Exceptions; 11.2.3 Exception Checking
  - url: https://kotlinlang.org/docs/exceptions.html#exception-interoperability-with-java-swift-and-objective-c
    title: Kotlin documentation, Exceptions
    locator: the introduction; Exception interoperability with Java, Swift, and Objective-C
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.20.2
    title: The Java Language Specification, Java SE 25 Edition
    locator: 14.20.2 Execution of try-finally and try-catch-finally; 14.20.3.1 Basic try-with-resources
  - url: https://docs.python.org/3.14/library/exceptions.html#exception-context
    title: The Python Standard Library, Built-in Exceptions, Python 3.14
    locator: Exception context; Base classes, BaseException; Concrete exceptions, StopIteration; Exception hierarchy
  - url: https://tc39.es/ecma262/2026/#sec-installerrorcause
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 20.5.8.1 InstallErrorCause
  - url: https://go.dev/ref/spec#Handling_panics
    title: The Go Programming Language Specification
    locator: Handling panics; Run-time panics
  - url: https://go.dev/doc/faq#exceptions
    title: Go FAQ
    locator: Why does Go not have exceptions?
  - url: https://go.dev/blog/defer-panic-and-recover
    title: Andrew Gerrand, Defer, Panic, and Recover, The Go Blog, 2010
    locator: the text on recover, and the encoding/json example
  - url: https://doc.rust-lang.org/book/ch09-00-error-handling.html
    title: The Rust Programming Language, Chapter 9, Error Handling
    locator: the introduction of chapter 9
  - url: https://doc.rust-lang.org/std/panic/fn.catch_unwind.html
    title: The Rust Standard Library, Function catch_unwind
    locator: the description and Notes
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, ISO/IEC 9899:2024 working draft (C23)
    locator: 7.5 Errors <errno.h>, paragraph 3
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, ISO/IEC 9899:2024 working draft (C23)
    locator: 7.13.1.1 The setjmp macro, paragraph 4; 7.13.2.1 The longjmp function, paragraphs 2 and 3
  - url: https://www.gnu.org/software/bash/manual/bash.html#The-Set-Builtin
    title: Bash Reference Manual
    locator: 4.3.1 The Set Builtin, the -e option
  - url: https://www.gnu.org/software/bash/manual/bash.html#index-trap
    title: Bash Reference Manual
    locator: 4.1 Bourne Shell Builtins, trap
  - url: https://www.postgresql.org/docs/current/plpgsql-control-structures.html#PLPGSQL-ERROR-TRAPPING
    title: PostgreSQL documentation, PL/pgSQL Control Structures
    locator: Trapping Errors
  - url: https://learn.microsoft.com/en-us/sql/t-sql/language-elements/try-catch-transact-sql
    title: Transact-SQL reference, TRY...CATCH
    locator: the description
  - url: https://peps.python.org/pep-0654/#specification
    title: "PEP 654: Exception Groups and except*"
    locator: Specification
  - url: https://tc39.es/ecma262/2026/#sec-promise.any
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 27.2.4.3 Promise.any
  - url: https://tc39.es/ecma262/2026/#sec-asyncblockstart
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 27.7.5.2 AsyncBlockStart
  - url: https://peps.python.org/pep-0765/#rationale
    title: "PEP 765: Disallow return/break/continue that exit a finally block"
    locator: Rationale
  - url: https://docs.python.org/3.14/whatsnew/3.14.html#pep-765-control-flow-in-finally-blocks
    title: What's new in Python 3.14
    locator: "PEP 765: Control flow in finally blocks"
  - url: https://learn.microsoft.com/en-us/dotnet/standard/exceptions/best-practices-for-exceptions#call-try-methods-to-avoid-exceptions
    title: .NET documentation, Best practices for exceptions
    locator: Call Try* methods to avoid exceptions; Capture and rethrow exceptions properly
  - url: https://timsong-cpp.github.io/cppwp/n4950/except.terminate
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[except.terminate], paragraph 1"
  - url: https://timsong-cpp.github.io/cppwp/n4950/except.spec
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[except.spec], paragraphs 5 and 8"
  - url: https://nodejs.org/api/cli.html#--unhandled-rejectionsmode
    title: Node.js documentation, Command-line API
    locator: --unhandled-rejections=mode
  - url: https://nodejs.org/api/process.html#event-uncaughtexception
    title: Node.js documentation, Process
    locator: "Event: 'uncaughtException'"
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_error_handling#non-terminating-errors
    title: PowerShell documentation, about_Error_Handling
    locator: Types of errors, Non-terminating errors; Script-terminating errors
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_try_catch_finally#catching-errors
    title: PowerShell documentation, about_Try_Catch_Finally
    locator: Long description; Catching errors
  - url: https://docs.python.org/3.14/whatsnew/3.11.html#misc
    title: What's New In Python 3.11
    locator: Faster CPython, Misc
  - paper: doi:10.1145/361227.361230
    title: "John B. Goodenough, Exception Handling: Issues and a Proposed Notation, Communications of the ACM 18(12), 1975"
    locator: pages 683 to 696
  - paper: doi:10.1109/TSE.1979.230191
    title: Barbara Liskov and Alan Snyder, Exception Handling in CLU, IEEE Transactions on Software Engineering SE-5(6), 1979
    locator: pages 546 to 558
---

## Idea

An operation sometimes cannot do its work. The text is not a number, the file is not there, or the connection closes. The code that finds the problem often does not know what to do about it, and the code that knows is often several calls away.[^1] An exception connects the two. The code that finds the problem raises (or throws) an exception, an object that describes the problem. Control leaves the operation at once and goes to the nearest handler for that kind of exception, in the same function or in one of its callers. The functions between the two end early, and they do not have to contain any code for the error.

In Python, `parse_port` raises `ValueError` for a port out of range, and `int` raises `ValueError` for text that is not a number. `read_config` has no handler, so the exception goes through it, and its `print` does not run. The handler in the loop catches both.

```python run id=idea
def parse_port(text):
    port = int(text)
    if not 1 <= port <= 65535:
        raise ValueError(f"port out of range: {port}")
    return port


def read_config(lines):
    ports = [parse_port(line) for line in lines]
    print("read", len(ports), "ports")
    return ports


for lines in (["80", "443"], ["80", "70000"], ["80", "abc"]):
    try:
        read_config(lines)
    except ValueError as e:
        print("handled:", e)
```

```output
read 2 ports
handled: port out of range: 70000
handled: invalid literal for int() with base 10: 'abc'
```

## How it works

An exception mechanism has four steps.

1. **Raise.** A `raise` or `throw` statement, or a failed check of the run-time system, makes an exception object and starts a search.
2. **Search.** The run-time system looks for a handler that matches the exception, usually by its class. It looks first in the innermost `try` statement around the point of the raise, then in the `try` statements around that one, and then in the caller at the point of the call, up the call stack.[^2][^3]
3. **Unwind.** The frames between the raise and the handler are removed. The cleanup code of each frame runs on the way: `finally` blocks, context managers and destructors.[^4]
4. **Handle.** The handler runs. Then execution continues after the `try` statement that contains the handler, not at the point of the raise.

This run shows the order. Each `finally` block runs while the exception passes through its frame, from the inside out, and then the handler runs.

```python run id=order
def inner():
    try:
        raise KeyError("port")
    finally:
        print("finally in inner")


def middle():
    try:
        inner()
        print("not reached")
    finally:
        print("finally in middle")


try:
    middle()
except KeyError as e:
    print("handler:", repr(e))
```

```output
finally in inner
finally in middle
handler: KeyError('port')
```

When the search finds no handler, the program reports the exception and stops. C++ calls `std::terminate`.[^3] The .NET runtime ends the thread.[^5] Python prints a traceback and exits with a non-zero status.

### Termination and resumption

In the termination model, the handler takes the place of the rest of the operation that raised. The operation cannot continue. The designers of CLU chose this model and rejected the resumption model of PL/I and Mesa, because resumption was complex and because termination was what most programs needed.[^6] Koenig and Stroustrup made the same choice for C++. They gave this reason: if a handler can resume, the code after `if (something_wrong) throw zxc();` cannot assume that `something_wrong` is false.[^1] Python, Java, JavaScript, C# and the other languages on this page use termination.

The Common Lisp condition system separates the parts. A handler runs before the stack unwinds, in the dynamic context of the code that signalled. It can decline, and then the next handler runs.[^7] It can also transfer control to a restart, a named way to continue that the lower code set up.[^8] This gives the effect of resumption under the control of the code that signalled.

### Two-phase unwinding

An implementation can search before it unwinds. The Itanium C++ ABI defines a two-phase process. In the search phase, the unwinder walks up the stack and asks the personality routine of each frame whether the frame has a handler. It does not remove any frame. If no frame has a handler, it calls `terminate()`. In the cleanup phase, it walks up the stack again and runs the cleanup code of each frame, up to the frame of the handler.[^9] A C# exception filter (`catch (E e) when (condition)`) also runs before the stack unwinds, so a filter that returns false leaves the stack and its trace as they were.[^5]

### Checked exceptions

In Java, the compiler checks the exceptions that a method can throw. A checked exception must be caught in the method or named in its `throws` clause. `RuntimeException`, `Error` and their subclasses are unchecked and need neither.[^10] The `throws` clause makes the exception part of the static type of the method (see [[static-and-dynamic-typing]]). CLU had an earlier form: a procedure listed its exceptions in its specification. An exception that the caller did not handle did not go on up the stack. It became the special exception `failure`.[^6] Kotlin has no checked exceptions,[^11] and C# has no `throws` clause.

### Cleanup and chaining

Cleanup must run on both paths, the normal one and the exceptional one. A `finally` block runs when control leaves the `try` block for any reason.[^2][^12] Other forms tie the cleanup to a resource: `with` in Python, try-with-resources in Java, `using` in C#, and destructors in C++, which run during stack unwinding.[^4] In Java, when `close()` throws after the body has thrown, the second exception is added to the first as a suppressed exception.[^12]

A handler often raises a second exception that fits its own level, such as a configuration error in place of a parse error. Chaining keeps the first exception as the cause of the second. Python sets `__cause__` with `raise ... from e`, and it sets `__context__` when any exception is raised inside an `except` clause.[^13] Java and Kotlin pass the cause to the constructor, C# sets `InnerException`, and JavaScript has the `cause` option of `Error`.[^14]

## In languages

The example files parse three port numbers with the same functions in each language. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | native | `raise`; `try` with `except`, `else` and `finally`; `with` |
| JavaScript | native | `throw` of any value; `try`, `catch` and `finally`; the rejection of a promise |
| TypeScript | native | as JavaScript; a `catch` variable has the type `unknown` or `any` |
| Java | native | `throw`; `try`, `catch` and `finally`; try-with-resources; checked exceptions |
| C | idiom | status codes and `errno`; `setjmp` and `longjmp` for a non-local jump |
| C++ | native | `throw`; `try` and `catch`; destructors for cleanup; `noexcept` |
| C# | native | `throw`; `try`, `catch` with `when`, and `finally`; `using` |
| Go | native | `panic`, and `recover` in a deferred function; error values for ordinary errors |
| Rust | idiom | `Result<T, E>` and the `?` operator; `panic!` for bugs |
| Kotlin | native | the exceptions of Java with no checked exceptions; `try` is an expression |
| PowerShell | native | `throw`; `try`, `catch` and `finally`; `trap`; non-terminating errors |
| Bash | idiom | the exit status; `set -e`, `trap ... ERR` and a subshell as the boundary |
| SQL | absent | a statement fails with an error that goes to the host program |
| Regex | n/a | a pattern does not raise or handle errors |
| HTML/CSS | n/a | markup and style rules do not raise or handle errors |

Go has `panic` and `recover`. A panic stops the function, runs its deferred calls and those of each caller, and ends the program unless a deferred function calls `recover`.[^15] The Go FAQ explains why Go has no `try` statement: the designers think that it makes code convoluted and that it makes programmers treat ordinary errors as exceptional. Ordinary errors are values that a function returns.[^16] A package can panic inside and recover at its boundary, but its API returns error values.[^17]

Rust has no exceptions. A function that can fail returns `Result<T, E>`, and `panic!` is for errors that the program cannot recover from.[^18] A panic unwinds the stack by default. `std::panic::catch_unwind` can stop it, but its documentation says that it is not for a general try and catch, and it does not catch a panic that aborts.[^19]

C has no exceptions. A function returns a status code, and some library functions also set `errno`. No library function sets `errno` to zero, so a program sets it to zero before the call that it checks.[^20] `setjmp` and `longjmp` give a jump to an earlier point in the call stack, but no cleanup runs, and a local variable that changed after `setjmp` has an indeterminate value unless it is `volatile`.[^21] Koenig and Stroustrup described a portable implementation of C++ exceptions that uses `setjmp` and `longjmp`.[^1]

In Bash, each command has an exit status. `set -e` makes the shell exit when a command fails, except in a few contexts,[^22] and `trap ... ERR` runs a command when a command fails.[^23] A subshell gives a boundary: an exit inside it ends only the subshell, and the outer script tests its status.

SQL statements have no handler. The procedural extensions of some systems add one: an `EXCEPTION` clause in a PL/pgSQL block,[^24] and `BEGIN TRY ... END TRY BEGIN CATCH ... END CATCH` in Transact-SQL.[^25]

## Variations and related ideas

- **Error values and result types.** Go, Rust and C return the error as a value. The caller must test it at each call, and the type or the signature shows that the call can fail. Rust's `?` operator returns the error to the caller in one character, which gives explicit propagation with little text.
- **Panics.** Go and Rust keep a mechanism that unwinds the stack for bugs and states that the program cannot recover from, and they use error values for the rest.[^16][^18]
- **Exception groups.** Concurrent code can fail in several places at once. Python has `ExceptionGroup` and `except*`, which handles the matching parts of a group and raises the rest again.[^26] JavaScript has `AggregateError`, and `Promise.any` rejects with one when all its promises are rejected.[^27]
- **Exceptions in asynchronous code.** An exception in an `async` function goes into the promise or task that the function returns. It reaches a handler only when the caller waits for the result (see [[async-await]] and Pitfalls).[^28][^5]
- **Exceptions as control flow.** A Python iterator signals that it has no more items by raising `StopIteration` (see [[iterators]]).[^13] CLU programs also used exceptions to pass ordinary information to the caller.[^6]
- **Exceptions and recursion.** One raise can leave many frames of a deep recursion at once, so a recursive search can stop when it finds an answer (see [[recursion]]).
- **Memory and other resources.** In a language with [[garbage-collection]], the frames that the unwinder removes leave their objects for the collector. A file or a lock that those frames held needs `finally`, `with` or `using`, because the collector can run much later.

## Pitfalls

### A broad handler hides a bug

A handler for `Exception` catches every error, also the ones that come from bugs. Here the key `"prot"` has a typing error. The handler was written for text that is not a number, but it catches the `KeyError` and returns the default.

```python wrong id=broad
config = {"port": "8080"}


def port_of(config):
    try:
        return int(config["prot"])
    except Exception:
        return 80


print(port_of(config))
```

```output
80
```

The fix is to catch only the exception that the handler can deal with, and to keep the `try` block small. Now the typing error stops the program with `KeyError` at the line that has it.

```python fails expect=exception:KeyError id=narrow
config = {"port": "8080"}


def port_of(config):
    text = config["prot"]
    try:
        return int(text)
    except ValueError:
        return 80


print(port_of(config))
```

An `except` clause with no expression is worse. It matches every exception,[^2] also `KeyboardInterrupt` and `SystemExit`, which derive from `BaseException` and not from `Exception`.[^13]

### A return in finally discards the exception

When a `finally` block ends with `return`, `break` or `continue`, the exception that was passing through is lost.[^2] In Java the rule is the same: when the `finally` block completes abruptly, the `try` statement completes for the same reason, and the exception is discarded.[^12]

```python wrong id=finally-return
def save():
    try:
        raise OSError("disk full")
    finally:
        return "saved"


print(save())
```

```output
saved
```

The caller sees `saved`, and the error is gone. PEP 765 found that most of these statements in real code were bugs.[^29] The same PEP made the compiler warn about them with a `SyntaxWarning`.[^30] The fix is to put the `return` after the `try` statement, so that `finally` only cleans up.

### A second exception without the first one

When a handler throws a second exception and does not pass the first one as the cause, the reason for the failure is lost. Java does not chain the exceptions by itself.

```java wrong id=no-cause
public class Main {
    static int parsePort(String text) {
        try {
            return Integer.parseInt(text);
        } catch (NumberFormatException e) {
            throw new IllegalArgumentException("bad port");
        }
    }

    public static void main(String[] args) {
        try {
            parsePort("80a");
        } catch (IllegalArgumentException e) {
            System.out.println(e.getMessage() + ", cause: " + e.getCause());
        }
    }
}
```

```output
bad port, cause: null
```

The fix is `new IllegalArgumentException("bad port", e)`, as in the Java example. In C#, `throw e;` in a handler has a related effect: it starts the stack trace again at that point. `throw;` keeps the original trace.[^5][^31]

### A destructor that throws during unwinding

C++ runs destructors during stack unwinding. If a destructor throws while another exception is passing through, the program calls `std::terminate`.[^1][^32] A destructor with no `noexcept` specifier is `noexcept` unless the destructor of a member or a base can throw, and an exception that leaves a `noexcept` function also calls `std::terminate`.[^33] This destructor says `noexcept(false)`, so it compiles, and it throws during unwinding.

```cpp fails expect=runtime id=destructor
#include <print>
#include <stdexcept>

struct Connection {
    ~Connection() noexcept(false) { throw std::runtime_error("close failed"); }
};

void send() {
    Connection connection;
    throw std::runtime_error("send failed");
}

int main() {
    try {
        send();
    } catch (const std::exception& e) {
        std::println("caught: {}", e.what());
    }
}
```

The program aborts, and the handler in `main` does not run. The fix is a destructor that does not throw. A failure on close can be reported through a separate `close()` function that the caller calls on the normal path.

### An async call that nobody waits for

An exception in an `async` function rejects the promise that the function returns.[^28] The call does not throw, so a `try` statement around it does not see the exception. Node.js treats a rejection with no handler as an uncaught exception by default,[^34] and an uncaught exception ends the process with status 1.[^35]

```javascript fails expect=exit:1 id=no-await
async function load() {
  throw new Error("disk full");
}

try {
  load();
} catch (e) {
  console.log("caught", e.message);
}
console.log("after the call");
```

The fix is `await`, which turns the rejection back into an exception at the point of the call.

```javascript run id=await
async function load() {
  throw new Error("disk full");
}

async function main() {
  try {
    await load();
  } catch (e) {
    console.log("caught", e.message);
  }
}

main();
```

```output
caught disk full
```

### A PowerShell error that does not reach catch

Most cmdlet errors are non-terminating. The cmdlet writes the error and goes on, and a non-terminating error does not trigger `catch` or `trap`.[^36]

```powershell wrong id=non-terminating
try {
    Get-Item -Path ./no-such-file
    'after Get-Item'
} catch {
    'caught'
}
```

```output
after Get-Item
```

The fix is `-ErrorAction Stop`, which makes the error terminating, so the `catch` block runs.[^36][^37]

```powershell run id=stop
try {
    Get-Item -Path ./no-such-file -ErrorAction Stop
    'after Get-Item'
} catch {
    'caught'
}
```

```output
caught
```

### set -e has no effect inside a tested function

Bash ignores `set -e` in the test of an `if`, in a `&&` or `||` list, and in a function that runs in such a context, with all the commands of that function.[^22] So the error in `deploy` does not stop it.

```bash wrong id=errexit
set -e

deploy() {
  false
  echo "deploy went on after the failure"
}

if deploy; then
  echo "deploy succeeded"
fi
```

```output
deploy went on after the failure
deploy succeeded
```

The fix is to test each command that can fail and return its status, as in `false || return`.

```bash run id=errexit-fix
deploy() {
  false || return
  echo "not reached"
}

if deploy; then
  echo "deploy succeeded"
else
  echo "deploy failed with status $?"
fi
```

```output
deploy failed with status 1
```

## Cost

- **The normal path.** A table-based implementation adds no instructions where no exception is thrown. The compiler writes tables that map code addresses to cleanup code and handlers. The tables use space, and the unwinder reads them only when an exception is thrown.[^1][^9]
- **The throw.** A throw searches the tables, unwinds each frame and runs its cleanup code, so it costs much more than a return. Koenig and Stroustrup saw this as an advantage, because it discourages exceptions where other control structures will do.[^1] .NET adds `Try` methods such as `Int32.TryParse`, which return a Boolean in place of an exception, for code where the cost of exceptions is too high.[^31]
- **Python.** CPython 3.11 made `try` statements "zero-cost": a `try` costs nothing when no exception is raised.[^38]
- **Error values.** A `Result` or an error value is a value that the caller tests after each call. The test is a branch on the normal path, and the value can make the return type larger.

## History

- **1975.** John B. Goodenough published "Exception Handling: Issues and a Proposed Notation" in Communications of the ACM.[^39]
- **1975 to 1979.** Barbara Liskov's group designed the exception mechanism of CLU. They made the main decisions by June 1975 and completed the design in 1977. CLU used the termination model and turned unhandled exceptions into `failure`.[^6] Liskov and Alan Snyder described the mechanism in 1979.[^40]
- **1989 and 1990.** Andrew Koenig and Bjarne Stroustrup presented the design of exception handling for C++: termination, handlers that match by type, destructors during unwinding, and an implementation with no run-time cost until a throw.[^1]
- **2022.** Python 3.11 added exception groups and `except*`.[^26][^38]

## References
