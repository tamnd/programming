---
tag: NEW-defer-and-finally
kind: concept
slug: defer-and-finally
name: Defer and finally
aliases: [finally, try-finally, defer, cleanup, with statement, using statement, try-with-resources, scope exit]
area: errors-and-control/cleanup
level: core
summary: >-
  Cleanup constructs run code when control leaves a scope, on every path out: the normal end, a return, a break or an exception. Programs use them to close files, release locks and undo partial work.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2005
  people: [Guido van Rossum, Alyssa Coghlan]
  note: PEP 343 added the with statement to Python, so that a context manager could hold the try and finally code that programs wrote again and again around a resource.
see_also: [exception-handling, panics, errors-as-values, closures, higher-order-functions]
sources:
  - url: https://go.dev/blog/defer-panic-and-recover
    title: Andrew Gerrand, "Defer, Panic, and Recover", The Go Blog, 4 August 2010
    locator: the CopyFile example and the three rules of defer
  - url: https://docs.python.org/3.14/reference/compound_stmts.html#finally-clause
    title: The Python Language Reference (3.14), 8. Compound statements
    locator: 8.4.4 finally clause
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/try...catch
    title: MDN Web Docs, try...catch
    locator: the description of the finally block and the example Returning from a finally block
  - url: https://docs.python.org/3.14/reference/compound_stmts.html#the-with-statement
    title: The Python Language Reference (3.14), 8. Compound statements
    locator: 8.5 The with statement, the expansion into try, except and finally
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/using
    title: C# language reference, The using statement
    locator: the sections on using declarations and on several instances
  - url: https://timsong-cpp.github.io/cppwp/n4950/stmt.jump.general
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[stmt.jump.general] 8.7.1, paragraph 2 (Note 1)"
  - url: https://timsong-cpp.github.io/cppwp/n4950/except.ctor
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[except.ctor] 14.3 Constructors and destructors, paragraphs 1 and 2"
  - url: https://go.dev/ref/spec#Defer_statements
    title: The Go Programming Language Specification
    locator: Defer statements
  - url: https://doc.rust-lang.org/reference/destructors.html
    title: The Rust Reference, Destructors
    locator: destructors.intro, the drop scopes, destructors.scope.temporary and destructors.forget
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.20.3
    title: The Java Language Specification, Java SE 25 Edition
    locator: 14.20.3 try-with-resources and 14.20.3.1 Basic try-with-resources
  - url: https://docs.python.org/3.14/library/contextlib.html#contextlib.ExitStack
    title: Python 3.14 documentation, contextlib
    locator: class contextlib.ExitStack
  - url: https://www.typescriptlang.org/docs/handbook/release-notes/typescript-5-2.html
    title: TypeScript 5.2 release notes
    locator: using Declarations and Explicit Resource Management
  - url: https://peps.python.org/pep-0343/
    title: Guido van Rossum and Alyssa Coghlan, PEP 343, The "with" Statement
    locator: the header (Created 13-May-2005, Python-Version 2.5) and the abstract
  - url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
    title: TC39, Finished Proposals
    locator: the row Explicit Resource Management (Ron Buckton, expected publication year 2027)
  - url: https://kotlinlang.org/docs/exceptions.html
    title: Kotlin documentation, Exceptions
    locator: The finally block; the use() function
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_try_catch_finally
    title: PowerShell documentation, about_Try_Catch_Finally
    locator: Long description
  - url: https://www.kernel.org/doc/html/latest/process/coding-style.html#centralized-exiting-of-functions
    title: Linux kernel coding style
    locator: 7) Centralized exiting of functions
  - url: https://man7.org/linux/man-pages/man1/bash.1.html
    title: bash(1), Linux manual page
    locator: SHELL BUILTIN COMMANDS, trap ("If a sigspec is EXIT (0), action is executed on exit from the shell")
  - url: https://docs.python.org/3.14/reference/compound_stmts.html#the-async-with-statement
    title: The Python Language Reference (3.14), 8. Compound statements
    locator: 8.9.3 The async with statement
  - url: https://docs.oracle.com/javase/tutorial/essential/exceptions/tryResourceClose.html
    title: The Java Tutorials, The try-with-resources Statement
    locator: the example readFirstLineFromFileWithFinallyBlock and Suppressed Exceptions
  - url: https://peps.python.org/pep-0765/
    title: Irit Katriel and Alyssa Coghlan, PEP 765, Disallow return/break/continue that exit a finally block
    locator: the header (Created 15-Nov-2024, Python-Version 3.14) and Rationale
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.20.2
    title: The Java Language Specification, Java SE 25 Edition
    locator: 14.20.2 Execution of try-finally and try-catch-finally
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/compiler-messages/jump-statement-errors
    title: C# reference, Resolve errors and warnings related to jump statements
    locator: "CS0157: Control cannot leave the body of a finally clause"
  - url: https://doc.rust-lang.org/reference/patterns.html#wildcard-pattern
    title: The Rust Reference, Patterns
    locator: patterns.wildcard.no-binding
  - url: https://timsong-cpp.github.io/cppwp/n4950/class.temporary
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[class.temporary] paragraph 4"
  - url: https://docs.python.org/3.14/library/os.html#os._exit
    title: Python 3.14 documentation, os
    locator: os._exit
  - url: https://go.dev/doc/go1.14#runtime
    title: Go 1.14 Release Notes
    locator: Runtime
---

## Idea

A function that opens a file must close it. If the function has one exit, one call to `close` at the end is enough. Most functions have more exits than that. An error check returns early, a loop breaks, and an exception leaves from the middle of the body. Each exit must release what the function holds at that point.

The Go blog shows the failure with a function that copies one file to another. The first version opens the source, then creates the destination, and closes both at the end. "If the call to os.Create fails, the function will return without closing the source file."[^1] The fix with `defer` puts the close next to the open, and the blog says that the files are then closed "regardless of the number of return statements in the function".[^1]

Every cleanup construct does the same thing. It ties a piece of code to the exit of a scope, so that one statement covers all the paths out. In Python, the code in `finally` runs on each of the three exits here.

```python run id=idea
def process(job):
    print("open", job)
    try:
        if job == "empty":
            return "skipped"
        if job == "bad":
            raise ValueError("bad job")
        return "done"
    finally:
        print("close", job)


for job in ["a", "empty", "bad"]:
    try:
        print("->", process(job))
    except ValueError as exc:
        print("-> error:", exc)
```

```output
open a
close a
-> done
open empty
close empty
-> skipped
open bad
close bad
-> error: bad job
```

## How it works

Languages put the cleanup code in one of three places.

| Shape | Where the cleanup code is | Examples |
|---|---|---|
| A block on the exit of a region | in a `finally` block after the body | `try` and `finally` in Python, JavaScript, Java, C#, Kotlin and PowerShell |
| A call that the program schedules | next to the acquisition, as a function call | `defer` in Go; `ExitStack` in Python; `DisposableStack` in JavaScript |
| A method of the resource | in the type of the resource | destructors in C++; `Drop` in Rust; `with`, `using` and try-with-resources, which call a method of the object |

The three shapes differ in what the programmer must remember. With `finally`, each function that uses a resource must write the release itself. With a scheduled call, the release is next to the acquisition, but each use must still schedule it. With a destructor, the type writes the release once, and each use gets it without a statement of its own ([[resource-acquisition-is-initialization]]).

### The two paths out

Cleanup must run on two kinds of path. On a normal path, control reaches the end of the body or a `return`, `break` or `continue`. On an exceptional path, an exception or a panic passes through the frame ([[exception-handling]], [[panics]]).

Python describes both paths for `finally`. If an exception occurs and nothing handles it, the exception "is temporarily saved". The `finally` clause runs, and the saved exception is raised again at its end. If the `finally` clause raises another exception, the saved exception becomes the context of the new one.[^2] A `return`, `break` or `continue` in the `try` block also runs the `finally` clause "on the way out".[^2] JavaScript has the same rule: the effect of a `return`, `throw`, `break` or `continue` in the `try` block waits until the `finally` block ends.[^3]

The statement forms are written in terms of `finally`. The Python reference gives the `with` statement as an expansion into `try`, `except` and `finally` that calls the method `__exit__` of the context manager. On an exceptional exit, `__exit__` receives the exception. If it returns a true value, the exception stops there.[^4] The C# compiler rewrites a `using` block into `try` and `finally`, so `Dispose` runs even when the block returns.[^5] C++ and Rust need no statement at all. The language destroys the objects of a scope when control leaves it. In C++, the objects of a scope are destroyed on exit from the scope "however accomplished".[^6] During stack unwinding, the objects that a `try` block constructed are destroyed too.[^7]

### Order

When a scope holds several resources, the cleanup runs in reverse order. Each language states this rule.

- Go runs deferred calls "in the reverse order they were deferred".[^8]
- Rust drops the variables of a scope "in reverse order of declaration".[^9]
- C++ destroys the objects of a scope "in the reverse order of their construction".[^6]
- Java closes the resources of a try-with-resources statement "in the reverse order from that in which they were initialized".[^10]
- C# disposes several instances of one `using` statement "in reverse order of declaration".[^5]
- Python's `ExitStack` calls its callbacks in reverse order of registration.[^11]

A resource that the program acquires later can depend on an earlier one. A buffered writer depends on the file below it, and a transaction depends on its connection. Reverse order releases each resource while the ones it depends on are still alive. The TypeScript notes give this reason for `DisposableStack`, which disposes "in a first-in-last-out order".[^12]

### Go defer in detail

A Go `defer` statement has three rules, and the blog lists them.[^1] The specification states them in full.[^8]

1. The function value and the arguments of a deferred call are evaluated when the `defer` statement runs. The call itself waits.
2. The deferred calls run immediately before the surrounding function returns, in reverse order. They also run when the goroutine panics.
3. A deferred call runs after a `return` statement has set the result parameters. A deferred function literal can read and change named results. In the specification, `defer func() { result *= 7 }()` with `return 6` makes the function return 42.

Any results of the deferred function itself are discarded.[^8] This rule is why `defer f.Close()` loses the error that `Close` returns. The Go example uses a deferred function literal and the named result `err` to keep that error.

### The value of a return before finally

A `return` in the `try` block computes its value before `finally` runs. MDN describes this for JavaScript: "the return value expression ... is evaluated before entering the finally block".[^3] The examples for Python, Java, C# and JavaScript show the same result. Each one assigns a new value to the returned variable in `finally`, and each function still returns the old value. Go is the exception. Its deferred calls can change named results after `return` has set them.[^8]

## In languages

| Language | Verdict | Form |
|---|---|---|
| Python | native | `try` and `finally`; `with` and context managers; `contextlib.ExitStack` |
| JavaScript | native | `try` and `finally`; `using` and `DisposableStack` from Explicit Resource Management |
| TypeScript | native | as JavaScript; the types `Disposable` and `DisposableStack` |
| Java | native | `try` and `finally`; try-with-resources with `AutoCloseable` |
| C | absent | no cleanup construct; `goto` to cleanup labels at the end of the function |
| C++ | native | destructors; no `finally` |
| C# | native | `try` and `finally`; the `using` statement and `using` declarations with `IDisposable` |
| Go | native | `defer` |
| Rust | native | the trait `Drop`; no `finally` and no `defer` |
| Kotlin | native | `try` and `finally`; the function `use` for `AutoCloseable` |
| PowerShell | native | `try`, `catch` and `finally` |
| Bash | idiom | `trap` with the signal `EXIT` |
| SQL | absent | a statement has no clause that runs on exit |
| Regex | n/a | a pattern holds no resources |
| HTML/CSS | n/a | markup and style rules hold no resources |

Four families stand out.

- **`finally` plus a statement form.** Python, Java, C#, JavaScript and TypeScript have `try` and `finally`, and each adds a statement that calls a cleanup method of an object. Python's `with` came from PEP 343, which says that it makes it possible "to factor out standard uses of try/finally statements".[^13] Java's try-with-resources closes each resource and keeps the first exception, with the later ones added as suppressed exceptions.[^10] C# has the `using` statement and the `using` declaration, which disposes its object at the end of the enclosing scope.[^5] JavaScript's `using` comes from the proposal Explicit Resource Management, which TC39 lists as finished, with 2027 as its expected year of publication.[^14] TypeScript 5.2 added `using` declarations and the types for it.[^12] Kotlin has `finally` and the function `use`, which closes an `AutoCloseable` resource when its block completes, whether or not the block throws.[^15] PowerShell has `try`, `catch` and `finally`, and its documentation names the release of resources as a use of `finally`.[^16]
- **Destructors.** C++ and Rust have no `finally`. The type of the resource releases it in a destructor, which runs on every exit from the scope.[^6][^9] A small guard type with a function in its destructor gives the effect of `defer` (see the C++ example).
- **A scheduled call.** Go has no `finally` and no destructors. A `defer` statement schedules a call for the end of the function.[^8] The scope of a deferred call is the function, not the block.
- **Nothing.** C has none of these constructs. The Linux kernel coding style recommends a `goto` to a label at the end of the function when a function "exits from multiple locations and some common work such as cleanup has to be done".[^17] Bash runs a command on exit from the shell with `trap ... EXIT`.[^18]

## Variations and related ideas

- **Cleanup lists.** Some resources are known only at run time, such as one file for each name in a list. `ExitStack` in Python and `DisposableStack` in JavaScript keep a stack of cleanup actions and run all of them when the stack is closed.[^11][^12] The TypeScript notes compare the method `defer` of `DisposableStack` with the keyword `defer` in Go.[^12]
- **Asynchronous cleanup.** A cleanup that must wait for I/O needs an asynchronous form. Python has `async with`, which can be used only in a coroutine function.[^19] JavaScript has `await using` declarations and `AsyncDisposableStack`.[^12]
- **Cleanup that fails.** Closing a file can fail, and the failure can collide with an earlier one. With a plain `finally` in Java, an exception from `close` wins, and "the exception thrown from the try block is suppressed".[^20] Java's try-with-resources keeps the first exception and adds the others as suppressed exceptions.[^10] Python sets the saved exception as the context of the new one.[^2] Go discards the results of a deferred call unless the code passes them on by hand.[^8]
- **Scope guards.** A guard object that runs a function in its destructor turns the destructor shape into a scheduled call. The C++ example builds one in a few lines. A function that takes a callback for the body and closes the resource after it is another form ([[higher-order-functions]], [[closures]]).

## Pitfalls

### A return in finally

A `return` in `finally` replaces whatever the `try` block was doing. MDN says that a control flow statement at the end of `finally` "will override the effect of the previous one".[^3] It replaces a `return`, and it also replaces a `throw`, so the error disappears.

```javascript wrong id=finally-return
function save(record) {
  try {
    if (!record) throw new Error("empty record");
    return "saved";
  } finally {
    return "done";
  }
}

console.log(save({ id: 1 }));
console.log(save(null));
```

```output
done
done
```

Python behaves the same way.[^2] PEP 765 measured the pattern in real code. It is rare, about 2 uses per million lines in the top 8,000 PyPI packages, and "most of the usages are incorrect".[^21] Python 3.14 emits a `SyntaxWarning` for it.[^21] Java compiles it, and the abrupt end of `finally` discards the exception.[^22] C# rejects it with the compile error CS0157, "Control cannot leave the body of a finally clause".[^23] [[exception-handling]] shows the effect on the exception in more detail. The fix is to keep `finally` for cleanup and to put the `return` after the `try` statement.

### A change in finally that does not change the result

A function that assigns to its returned variable in `finally` still returns the old value, because the `return` computed its value first.[^3] The examples for Python, Java, C# and JavaScript show it. To change the result of a function on its way out, Go uses a named result and a deferred function literal.[^8] Other languages need the change before the `return`.

### Deferred arguments are evaluated at once

Go evaluates the arguments of a deferred call when the `defer` statement runs.[^8] `defer fmt.Println("processed", count)` prints the value of `count` at that point, not at the end (see the `arguments` block of the Go example). To read the value at the end, defer a function literal that reads the variable.

### Defer in a loop

A deferred call runs when the function returns, not at the end of the loop body.[^8] A loop that opens a file and defers its close keeps every file open until the function ends. The `loop` block of the Go example opens 1000 files and holds all of them at once. The fix is to move the body of the loop into a function of its own, so that each call closes its file.

### A guard without a name

In Rust, the pattern `_` does not bind the value that it matches.[^24] `let _ = lock()` creates a temporary, and Rust drops the temporaries of a statement at the end of that statement.[^9] The lock ends before the next line. In C++, `Lock{"accounts"};` creates a temporary, and a temporary is destroyed "as the last step in evaluating the full-expression" that contains it.[^25] The `underscore` block of the Rust example and the `temporary` block of the C++ example show both. Give the guard a name, such as `_held` or `held`.

### Cleanup that does not run

Cleanup constructs run when control leaves a scope. A process that ends at once does not leave its scopes. Python's `os._exit` exits "without calling cleanup handlers".[^26] The Python example of [[panics]] shows a `finally` block that does not run after `os._exit`. Rust also lets safe code skip a destructor: `std::mem::forget` is a safe function, and the reference says that types "may not safely rely on a destructor being run for soundness".[^9]

### The wrong label in C

With `goto` cleanup, each label must release only what is already acquired on every path that jumps to it. The kernel style guide calls the error "one err bugs": a single label `err` frees `foo->bar` and then `foo`, but on some paths `foo` is NULL. The fix is one label for each resource, in reverse order.[^17] A `return` in the middle of the function skips all the labels (see the `early-return` block of the C example).

## Cost

- **The cleanup code.** A cleanup construct runs the same release code that a programmer would write on each path. The source has one copy of that code in place of one copy for each path.
- **Go defer.** The Go 1.14 release notes say that most uses of `defer` "incur almost zero overhead compared to calling the deferred function directly".[^27]
- **Cleanup lists.** `ExitStack` and `DisposableStack` keep one entry for each registered action, and they call the actions one at a time when they close.[^11][^12]
- **Resources held too long.** The resource stays held until the scope ends. A C# `using` declaration holds its object to the end of the enclosing scope, and a `using` statement holds it only for its block.[^5] Rust's `drop(value)` and a short `with` block in Python release a resource earlier. A `defer` in a loop holds every resource until the function returns.

## History

- **2005.** PEP 343 by Guido van Rossum and Alyssa Coghlan proposed the `with` statement for Python 2.5. Its stated aim was to factor out standard uses of `try` and `finally`.[^13]
- **2010.** Andrew Gerrand's post "Defer, Panic, and Recover" on the Go blog explained `defer` with the file copy that leaks a file, and gave the three rules of `defer`.[^1]
- **2024.** PEP 765 by Irit Katriel and Alyssa Coghlan proposed a warning for `return`, `break` and `continue` that leave a `finally` block. Python 3.14 emits it as a `SyntaxWarning`.[^21]

## References
