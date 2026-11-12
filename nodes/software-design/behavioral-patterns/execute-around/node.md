---
tag: A6GFP9
kind: pattern
slug: execute-around
name: Execute Around
aliases: [execute around method, execute-around method, execute around pattern]
area: software-design/behavioral-patterns
level: core
summary: >-
  An execute-around function does the setup, calls a block that its caller gives, and then always does the cleanup, so that callers cannot forget the cleanup.
family: behavioral-design
polarity: positive
intent: When a pair of actions must be taken together, such as open and close, wrap the pair around a function that the caller passes in.
also_known_as: [execute around method]
see_also: [resource-acquisition-is-initialization, higher-order-functions, closures, callback-pattern, template-method-pattern, strategy-pattern]
sources:
  - url: https://wiki.c2.com/?ExecuteAroundMethod
    title: Execute Around Method, WikiWikiWeb
    locator: whole page
  - url: https://github.com/python/cpython/blob/v3.14.0/Lib/contextlib.py#L276-L306
    title: CPython v3.14.0 source, Lib/contextlib.py
    locator: contextmanager and its docstring, lines 276 to 306
  - url: https://docs.ruby-lang.org/en/3.4/IO.html
    title: class IO, Ruby 3.4 documentation
    locator: IO.open
  - url: https://docs.python.org/3/reference/compound_stmts.html#the-with-statement
    title: The Python Language Reference, 8.5 The with statement
    locator: The with statement
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/using
    title: using statement, C# language reference, Microsoft Learn
    locator: first paragraphs
  - url: https://docs.oracle.com/javase/tutorial/essential/exceptions/tryResourceClose.html
    title: The try-with-resources Statement, The Java Tutorials
    locator: first paragraphs
  - url: https://github.com/ruby/ruby/blob/v3_4_0/io.c#L8158-L8168
    title: Ruby v3_4_0 source, io.c
    locator: rb_io_s_open, lines 8158 to 8168
---

## Intent

When a pair of actions have to be taken together, such as file open and close, you can use a higher-order function that wraps the actions around the function that is passed in.[^1]

## Problem

Many resources need two actions: one before the work and one after it. A file must be opened and then closed. A lock must be taken and then released. A transaction must be started and then committed.

If each caller writes both actions, some caller will forget the second one, or an error will skip it. One comment on the WikiWikiWeb page says that you should not have to balance `open()` and `close()` calls throughout an application.[^1]

## Solution

Write one function that owns the pair of actions. The caller gives it the work as a block or a function. The function does the setup, calls the block, and then does the cleanup.[^1] The cleanup must also run when the block fails, so the function puts it in a `finally` or `ensure` clause.[^1][^2]

```text
   caller                         execute-around function
     |  open_during(block)              |
     |--------------------------------->|  setup (open)
     |                                  |  try:
     |            block(resource)       |      call the block
     |<---------------------------------|
     |--------------------------------->|  finally:
     |                                  |      cleanup (close)
     |<-------- value of the block -----|
```

The WikiWikiWeb page shows the Smalltalk form. Instead of `aFile open`, the work, and `aFile close`, the caller writes `aFile openDuring: [...]`, and the method `openDuring:` opens the file, evaluates the block and closes the file. The page refers to Smalltalk Best Practice Patterns for more.[^1]

## Example

The example files show one program in each language. The function `with_connection` opens a connection, calls the block of the caller with the connection, and closes the connection in a `finally` clause. It returns the value of the block. The program prints:

```text
open db
query: 3 rows
close db
result: 3
```

## Consequences

The benefits:

- The setup and the cleanup are in one place. Callers cannot forget the cleanup.
- The cleanup also runs when the block raises an error, if the function uses `finally` or an equivalent.[^2]
- The function can return the value of the block. Ruby's `IO.open` with a block calls the block with the IO object and returns its value.[^3]

The costs:

- The caller must put its work in a block or a function. In a language without closures this needs an object, for example an interface with `setUp`, `thunk` and `tearDown` methods.[^1]
- The resource is available only inside the block. Code that must keep the resource open across calls cannot use this form.

If the function does not protect the cleanup, an error in the block skips it. The simple `openDuring:` method on the WikiWikiWeb page has this form.[^1] Here the file is never closed:

```python wrong id=no-finally
def open_during(block):
    print("open file")
    block()
    print("close file")  # skipped when block raises


try:
    open_during(lambda: 1 / 0)
except ZeroDivisionError as e:
    print("error:", e)
```

```output for=no-finally
open file
error: division by zero
```

## When not to use it

- The resource must stay open longer than one call, for example a connection that a pool keeps.
- The language has a statement for the same work, such as `with` in Python, `using` in C# or try-with-resources in Java.[^4][^5][^6] Then use the statement and write a context manager or a disposable type.

## Variants and related patterns

- **Language statements.** Python's `with` statement wraps the execution of a block with methods defined by a context manager. This allows common try, except and finally usage patterns to be encapsulated for convenient reuse.[^4] Java's try-with-resources statement ensures that each resource is closed at the end of the statement.[^6] C#'s `using` statement disposes the `IDisposable` instance when control leaves the block, even if an exception occurs.[^5]
- **Generator-based context managers.** Python's `contextlib.contextmanager` turns a generator with setup, `yield` and a `finally` cleanup into a context manager.[^2]

```python run id=contextmanager
from contextlib import contextmanager


@contextmanager
def connection(name):
    print("open", name)
    try:
        yield name
    finally:
        print("close", name)


with connection("db") as c:
    print("using", c)
```

```output for=contextmanager
open db
using db
close db
```

- **RAII.** In C++, the destructor of an object runs when the object goes out of scope. The WikiWikiWeb page notes that the pattern has also been discussed under Resource Acquisition Is Initialization.[^1]
- **Lisp `with-open-file`.** It expands into `unwind-protect` around the body, with `close` as the cleanup form.[^1]
- **Template Method.** A template method also fixes the steps before and after a variable step, but it uses a subclass for the variable step, not a block.

## Known uses

- **Ruby `IO.open`.** The C function `rb_io_s_open` creates the IO object. When a block is given, it returns `rb_ensure(rb_yield, io, io_close, io)`, so `io_close` runs even if the block raises.[^7]
- **Python `contextlib`.** The docstring of `contextmanager` shows that `with some_generator(<arguments>) as <variable>` is equivalent to setup, then `try`, the body, and `finally` with the cleanup.[^2]

## References
