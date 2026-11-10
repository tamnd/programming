---
tag: NEW-resource-acquisition-is-initialization
kind: concept
slug: resource-acquisition-is-initialization
name: Resource acquisition is initialization
aliases: [RAII, scope-bound resource management, SBRM, scoped resource management, deterministic cleanup]
area: memory-and-resources/raii
level: core
summary: >-
  RAII ties a resource to the lifetime of an object or a scope. The language releases the resource on every exit from the scope, also on an exception.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1990
  people: [Bjarne Stroustrup]
  note: Stroustrup writes that he held back the exception handling design of the ARM and its 1990 conference paper for about half a year, until he had found "resource acquisition is initialization".
see_also: [exception-handling, mutual-exclusion, reference-counting, explicit-memory-management, memory-leaks, stack-and-heap, garbage-collection, rust/ownership]
same_as:
  wikidata: Q1714630
  wikipedia: Resource_acquisition_is_initialization
sources:
  - url: https://www.stroustrup.com/bs_faq2.html#finally
    title: Bjarne Stroustrup, C++ Style and Technique FAQ
    locator: Why doesn't C++ provide a "finally" construct?
    accessed: 2026-11-10
  - url: https://www.stroustrup.com/glossary.html#Gresource-acquisition-is-initialization
    title: Bjarne Stroustrup, C++ Glossary
    locator: resource acquisition is initialization; RAII; resource; resource leak
    accessed: 2026-11-10
  - paper: doi:10.1145/1238844.1238848
    title: Bjarne Stroustrup, Evolving a language in and for the real world, C++ 1991-2006, HOPL III, 2007
    locator: section 5.3.1 Resource management
  - url: https://timsong-cpp.github.io/cppwp/n4950/class.dtor
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[class.dtor] paragraphs 6, 13 and 14"
    accessed: 2026-11-10
  - url: https://timsong-cpp.github.io/cppwp/n4950/stmt.dcl
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[stmt.dcl] paragraph 2"
    accessed: 2026-11-10
  - url: https://timsong-cpp.github.io/cppwp/n4950/stmt.jump
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[stmt.jump] paragraph 2"
    accessed: 2026-11-10
  - url: https://timsong-cpp.github.io/cppwp/n4950/except.ctor
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[except.ctor] paragraphs 1 to 3, stack unwinding"
    accessed: 2026-11-10
  - url: https://timsong-cpp.github.io/cppwp/n4950/except.spec
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[except.spec] paragraphs 3 and 5"
    accessed: 2026-11-10
  - url: https://timsong-cpp.github.io/cppwp/n4950/except.terminate
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[except.terminate] paragraph 1, items 1.3 and 1.4"
    accessed: 2026-11-10
  - url: https://timsong-cpp.github.io/cppwp/n4950/class.temporary
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[class.temporary] paragraph 4"
    accessed: 2026-11-10
  - url: https://timsong-cpp.github.io/cppwp/n4950/thread.lock.guard
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[thread.lock.guard] paragraphs 1, 2 and 6"
    accessed: 2026-11-10
  - url: https://timsong-cpp.github.io/cppwp/n4950/unique.ptr.single.dtor
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[unique.ptr.single.dtor] paragraph 1"
    accessed: 2026-11-10
  - url: https://timsong-cpp.github.io/cppwp/n4950/filebuf.cons
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[filebuf.cons] paragraph 5, the destructor of basic_filebuf"
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/reference/destructors.html#r-destructors.scope.intro
    title: The Rust Reference, Destructors
    locator: destructors.intro, destructors.operation, destructors.scope.intro, destructors.scope.bindings.let, destructors.manually-suppressing, destructors.process-termination
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/book/ch15-03-drop.html
    title: The Rust Programming Language, 15.3 Running Code on Cleanup with the Drop Trait
    locator: Dropping a Value Early with std::mem::drop (error E0040)
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/std/mem/fn.forget.html
    title: The Rust Standard Library, function std::mem::forget
    locator: Safety
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/std/sync/struct.MutexGuard.html
    title: The Rust Standard Library, struct std::sync::MutexGuard
    locator: struct description
    accessed: 2026-11-10
  - url: https://docs.python.org/3.14/reference/compound_stmts.html#the-with-statement
    title: The Python Language Reference, Compound statements, Python 3.14
    locator: 8.5 The with statement
    accessed: 2026-11-10
  - url: https://docs.python.org/3.14/reference/datamodel.html#with-statement-context-managers
    title: The Python Language Reference, Data model, Python 3.14
    locator: 3.3.9 With Statement Context Managers, object.__enter__ and object.__exit__
    accessed: 2026-11-10
  - url: https://docs.python.org/3.14/reference/datamodel.html#object.__del__
    title: The Python Language Reference, Data model, Python 3.14
    locator: Basic customization, object.__del__
    accessed: 2026-11-10
  - url: https://docs.python.org/3.14/library/contextlib.html#contextlib.ExitStack
    title: The Python Standard Library, contextlib, Python 3.14
    locator: class contextlib.ExitStack
    accessed: 2026-11-10
  - url: https://peps.python.org/pep-0343/
    title: "PEP 343: The \"with\" Statement"
    locator: header (Created 13-May-2005, Python-Version 2.5)
    accessed: 2026-11-10
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.20.3
    title: The Java Language Specification, Java SE 25 Edition
    locator: 14.20.3 try-with-resources and 14.20.3.1 Basic try-with-resources
    accessed: 2026-11-10
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/AutoCloseable.html
    title: Java SE 25 API, java.lang.AutoCloseable
    locator: interface description and close()
    accessed: 2026-11-10
  - url: https://jcp.org/en/jsr/detail?id=334
    title: "JSR 334: Small Enhancements to the Java Programming Language"
    locator: stage timeline (Final Release 20 Jul 2011) and section 2.1 and 2.3
    accessed: 2026-11-10
  - url: https://openjdk.org/jeps/421
    title: "JEP 421: Deprecate Finalization for Removal"
    locator: Summary; Motivation, Finalization and its flaws
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/using
    title: C# language reference, using statement
    locator: the using statement and the using declaration
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/dotnet/standard/garbage-collection/implementing-dispose
    title: .NET documentation, Implement a Dispose method
    locator: the dispose pattern; Dispose should be idempotent; GC.SuppressFinalize
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/finalizers
    title: C# programming guide, Finalizers
    locator: Remarks, the notes on the Finalize queue and on application termination
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/whats-new/csharp-version-history#c-version-80
    title: C# language version history
    locator: C# version 8.0, Using declarations
    accessed: 2026-11-10
  - url: https://ecma-international.org/wp-content/uploads/ECMA-334_1st_edition_december_2001.pdf
    title: ECMA-334, C# Language Specification, 1st edition, December 2001
    locator: 15.13 The using statement
    accessed: 2026-11-10
  - url: https://go.dev/ref/spec#Defer_statements
    title: The Go Programming Language Specification
    locator: Defer statements
    accessed: 2026-11-10
  - url: https://pkg.go.dev/os#Exit
    title: Go standard library, package os
    locator: func Exit
    accessed: 2026-11-10
  - url: https://pkg.go.dev/runtime#AddCleanup
    title: Go standard library, package runtime
    locator: func AddCleanup, func SetFinalizer
    accessed: 2026-11-10
  - url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.io/use.html
    title: Kotlin standard library, kotlin.io, use
    locator: "inline fun <T : Closeable?, R> T.use(block: (T) -> R): R"
    accessed: 2026-11-10
  - url: https://github.com/tc39/proposal-explicit-resource-management/blob/38c13295dc20c2273ba0a6ed82555f1fabb37764/README.md
    title: "ECMAScript Explicit Resource Management, proposal README"
    locator: Syntax, using declarations; Semantics, the reverse order of disposal and SuppressedError; DisposableStack
    accessed: 2026-11-10
  - url: https://github.com/tc39/proposals/blob/07002d4d999558b9c7326f98e5134e0222b13808/finished-proposals.md
    title: TC39, Finished Proposals
    locator: the row Explicit Resource Management, Expected Publication Year
    accessed: 2026-11-10
  - url: https://man7.org/linux/man-pages/man1/bash.1.html#SHELL_BUILTIN_COMMANDS
    title: bash(1), the Bash manual page, from the upstream Git repository of Bash
    locator: SHELL BUILTIN COMMANDS, trap and exit
    accessed: 2026-11-10
  - url: https://gcc.gnu.org/onlinedocs/gcc/Common-Attributes.html#Common-Variable-Attributes
    title: Using the GNU Compiler Collection, Common Variable Attributes
    locator: cleanup (cleanup_function)
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_functions_advanced_methods#clean
    title: PowerShell documentation, about_Functions_Advanced_Methods
    locator: clean
    accessed: 2026-11-10
---

## Idea

A program uses many things that it must give back: heap memory, file descriptors, sockets, locks, database transactions, temporary directories. Each of these is a resource. The release must happen exactly once, and it must happen on every path out of the code that uses the resource. A function that closes its file on its last line can still leak the file. An early `return`, a `break` out of a loop or an exception skips that line. A leaked lock is worse than leaked memory, because the next thread that asks for it waits forever.

Resource acquisition is initialization (RAII) binds the resource to the lifetime of an object. The constructor acquires the resource, and the destructor releases it. The object lives in a scope, and the language ends the lifetime of the object on every exit from that scope. So the release cannot be forgotten, and the programmer writes it once per kind of resource, not once per use.[^1][^2] The term comes from C++, where the destructor of a local object runs when its block exits.[^3] Languages without destructors use a statement that marks the scope. Examples are `with` in Python, try-with-resources in Java, `using` in C# and JavaScript, and `defer` in Go.

```python run id=idea
class Connection:
    def __init__(self, name):
        self.name = name

    def __enter__(self):
        print("open", self.name)
        return self

    def __exit__(self, exc_type, exc, traceback):
        print("close", self.name)
        return False  # do not suppress the exception


def query(fail):
    with Connection("orders") as conn:
        if fail:
            raise RuntimeError("query failed")
        return "rows from " + conn.name


print(query(False))
try:
    query(True)
except RuntimeError as error:
    print("caught:", error)
```

```output for=idea
open orders
close orders
rows from orders
open orders
close orders
caught: query failed
```

The connection closes before the `return` hands its value to the caller, and it closes before the exception reaches the `except` clause. The function body has no `close` call and no `finally` block.

## How it works

A scope has one entry and many exits. The compiler knows every exit: the end of the block, each `return`, `break`, `continue` and `goto`, and each point where an exception can leave. RAII asks the compiler, not the programmer, to put the release on each of them.

C++ states the rule for each transfer of control. Control can move from a point P to a point Q. Then each automatic variable that is active at P and not at Q is destroyed, in the reverse order of construction.[^5][^6] An exception follows the same rule. On the way from the `throw` to the handler, stack unwinding destroys the automatic objects made since the `try` block began. Again the order is the reverse.[^7] A destructor runs implicitly when the block of its object exits and when a temporary dies. Destructors of thread and static objects run at thread exit and at program end.[^4]

```text
void add_order() {                      exit paths:
    lock_guard guard(m);   // 1             end of block      -> ~cache, ~log, ~guard
    Handle log("log");     // 2             return            -> ~cache, ~log, ~guard
    Handle cache("cache"); // 3             throw             -> ~cache, ~log, ~guard, then the handler
    ...                                     std::exit()       -> no destructors of locals
}
```

The order is the reverse of acquisition because a later resource can depend on an earlier one. A cache may write to the log while it shuts down, so the log must still be open. Rust uses the same order for local variables.[^14] Members are a different case. C++ destroys the bases and members of an object in the reverse order of their construction.[^4] Rust drops the fields of a struct in declaration order, after the `drop` method of the struct itself.[^14]

Construction can fail halfway. If a constructor throws, C++ destroys only the subobjects whose construction has finished, and it never runs the destructor of the object itself.[^7] So a class that acquires two resources in its constructor should hold each one in a member that is itself an RAII object. Then a failure at the second acquisition still releases the first.

The languages fall into two families.

**Destructor-bound.** C++ and Rust attach the release to the type. Every value of the type releases its resource when it dies, wherever it lives. It can be a local variable, a member of another object or an element of a container. It can also be a heap object that a `std::unique_ptr` or a `Box` owns. The destructor of `std::unique_ptr` deletes the object it owns, and the destructor of `std::lock_guard` unlocks its mutex.[^12][^11] Ownership can move, so a function can make a resource and return it, and the scope of the caller then releases it. See [[rust/ownership]].

**Statement-bound.** Python, Java, C#, JavaScript, Go, Kotlin and Bash attach the release to a block in the code that uses the resource. The type only offers a release method: `__exit__`, `close`, `Dispose`, `[Symbol.dispose]`. The language rewrites the block into a `try` and `finally`. The Python reference gives this translation for `with`, and the JLS gives one for try-with-resources.[^18][^23] The object itself can outlive the block, in a closed state.

```pseudo
SCOPED(acquire₁, …, acquireₙ, body)         ▷ what with, using and try-with-resources do
L1  done ← empty stack
L2  try
L3      for i ← 1 to n do
L4          r ← acquireᵢ()                  ▷ a failure here skips the body
L5          push r on done
L6      body()                              ▷ may return, break or throw
L7  finally
L8      while done is not empty do
L9          r ← pop done                    ▷ reverse order of acquisition
L10         release r                       ▷ an error here must not stop the other releases
```

Line 10 hides a hard question: what happens when the release itself fails while another error is already in flight. Java closes every resource and adds each later exception to the suppressed list of the first one.[^23] The JavaScript proposal wraps the two errors in a `SuppressedError`.[^36] C++ destructors are `noexcept` unless they say otherwise, so an exception that leaves a destructor calls `std::terminate`.[^8][^9] Python passes the exception to `__exit__`, which can suppress it by returning a true value.[^19]

RAII covers the exits of a scope, not the end of the process. `std::exit` ends a C++ program without destroying automatic objects.[^6] `std::process::exit` and `std::process::abort` in Rust run no destructors.[^14] `os.Exit` in Go does not run deferred functions.[^33]

## In languages

The example files run the same test in each language. They open two resources and leave once normally and once through an error. Then they print the order of the releases.

| Language | Verdict | Construct |
|---|---|---|
| Python | native | `with` and context managers (`__enter__`, `__exit__`); `contextlib.ExitStack` for a list of resources[^18][^21] |
| JavaScript | native | `using` declarations, `Symbol.dispose` and `DisposableStack` from the Explicit Resource Management proposal[^36] |
| TypeScript | native | the JavaScript `using` declaration, with the `Disposable` interface |
| Java | native | try-with-resources on `AutoCloseable`[^23][^24] |
| C | absent | no destructors; the `goto` cleanup idiom; the `cleanup` attribute of GCC and Clang is an extension[^39] |
| C++ | native | destructors of automatic objects; `std::lock_guard`, `std::unique_ptr`, `std::fstream`[^4][^11][^12][^13] |
| C# | native | the `using` statement and the `using` declaration on `IDisposable`[^27] |
| Go | native | `defer` runs a call when the surrounding function returns[^32] |
| Rust | native | the `Drop` trait; every value is dropped at the end of its scope[^14] |
| Kotlin | stdlib | the `use` extension function on `Closeable`[^35] |
| PowerShell | idiom | `try` and `finally` that call `Dispose()`; the `clean` block of an advanced function[^40] |
| Bash | idiom | `trap ... EXIT` in a subshell, so the trap runs when the subshell exits[^38] |
| SQL | n/a | the database engine releases locks and cursors; a statement holds no resource of the program |
| Regex | n/a | a pattern holds no resources |
| HTML/CSS | n/a | markup and style hold no resources |

**C++ and Rust** are the destructor-bound family. The release runs for every value of the type. A container of files closes all of them, and a struct that holds a lock guard unlocks when the struct dies. Rust adds `std::mem::drop` to end a value early, because a direct call of the `drop` method is a compile error (E0040).[^15] `MutexGuard` is the Rust lock guard: dropping it unlocks the mutex.[^17]

**Python, Java, C# and JavaScript** use a statement that names the resource at the top of a block. Python calls `__exit__` after the suite ends for any reason, once `__enter__` has returned.[^18] Java closes the resources in the reverse order of initialization, and only those with a non-null value.[^23] C# disposes the variable of a `using` declaration at the end of its scope. It disposes several resources of one statement in reverse order.[^27] JavaScript disposes `using` bindings in reverse order when the block exits.[^36] Kotlin has no statement; `use` is a library function that runs a lambda and closes the receiver whether the lambda throws or not.[^35]

**Go** binds the release to the function, not to a block. Each `defer` saves a call, and the deferred calls run in the reverse order when the function returns or panics.[^32] This is a coarser scope than a C++ block, and it causes the loop pitfall below.

**C** has no destructors and no `finally`. Programs write the releases by hand. A common form is a `goto` to a list of labels at the end of the function, in reverse order of acquisition. GCC has a `cleanup` attribute on a local variable. It runs a function when the variable goes out of scope, in reverse order of definition.[^39] It is a compiler extension, not standard C. The Clang that recorded these examples accepts it too.

**Bash** has no scopes with cleanup, but `trap 'commands' EXIT` runs the commands when the shell exits, and `exit` runs that trap first.[^38] A function whose body is a subshell `( ... )` therefore gets an EXIT trap that runs at the end of the function.

## Variations and related ideas

- **Scope guards and defer.** A scope guard is an object whose destructor runs an arbitrary function. Go `defer` and `DisposableStack.defer` in JavaScript are the same idea without an object type for each kind of resource.[^32][^36]
- **Dynamic stacks of resources.** `contextlib.ExitStack` and `DisposableStack` hold a list of release callbacks and run them in reverse order. They cover a number of resources that is known only at run time.[^21][^36]
- **Ownership transfer.** `std::unique_ptr` and a Rust value can move to another owner, and the scope of the last owner releases the resource. Statement-bound forms cannot do this: the block always releases.
- **Shared ownership.** `std::shared_ptr`, `Rc` and `Arc` release the resource when the last owner dies. See [[reference-counting]].
- **Lock guards.** `std::lock_guard` and `MutexGuard` hold a lock for exactly one scope.[^11][^17] See [[mutual-exclusion]].
- **try and finally.** The manual form. It works in every language with exceptions, but each use of a resource repeats the release code. Stroustrup gives this as the reason C++ has no `finally`.[^1] See [[exception-handling]].
- **Finalizers and cleaners.** A finalizer runs when the collector frees an object, at a time the collector decides. The C# guide says that the programmer has no control over when the finalizer runs.[^29] Java deprecated finalization for removal, in part because no finalizer is guaranteed to run.[^26] Python does not guarantee `__del__` for objects that live until the interpreter exits.[^20] Go does not guarantee that a function of `runtime.AddCleanup` runs before the program exits.[^34] So finalizers are a safety net, not RAII. The .NET dispose pattern uses both: `Dispose` releases at once, and `GC.SuppressFinalize` then cancels the finalizer.[^28] See [[garbage-collection]], which shows a finalizer that runs late.

## Pitfalls

### A resource without a scope leaks on an early return

A release at the end of a function runs only when control reaches the end. Here the loop returns from the middle of the function, and the connection stays open.

```python wrong id=early-return
class Connection:
    open_count = 0

    def __init__(self, name):
        self.name = name
        Connection.open_count += 1

    def close(self):
        Connection.open_count -= 1

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc, traceback):
        self.close()
        return False


def first_large(orders):
    conn = Connection("orders")
    for order in orders:
        if order > 100:
            return order  # conn.close() never runs
    conn.close()
    return None


first_large([5, 250, 7])
print("open connections:", Connection.open_count)
```

```output
open connections: 1
```

The same leak happens when an exception leaves the loop. The fix is to give the connection a scope, so that every exit closes it.

```python run id=early-return-fixed
class Connection:
    open_count = 0

    def __init__(self, name):
        self.name = name
        Connection.open_count += 1

    def close(self):
        Connection.open_count -= 1

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc, traceback):
        self.close()
        return False


def first_large(orders):
    with Connection("orders"):
        for order in orders:
            if order > 100:
                return order
    return None


first_large([5, 250, 7])
print("open connections:", Connection.open_count)
```

```output
open connections: 0
```

### Go defer in a loop runs at the end of the function

A deferred call runs when the surrounding function returns, not when the loop body ends.[^32] A loop that opens a file and defers its close keeps every file open until the loop is over.

```go wrong id=defer-loop
package main

import "fmt"

type file struct{ name string }

var openFiles int

func openFile(name string) *file {
	openFiles++
	return &file{name}
}

func (f *file) Close() { openFiles-- }

func processAll(names []string) {
	for _, name := range names {
		f := openFile(name)
		defer f.Close()
		fmt.Println("processing", f.name, "open files:", openFiles)
	}
}

func main() {
	processAll([]string{"a.log", "b.log", "c.log"})
	fmt.Println("after return, open files:", openFiles)
}
```

```output
processing a.log open files: 1
processing b.log open files: 2
processing c.log open files: 3
after return, open files: 0
```

With many names, the program can run out of file descriptors. Move the body into a function of its own, so that each iteration has a function scope.

```go run id=defer-loop-fixed
package main

import "fmt"

type file struct{ name string }

var openFiles int

func openFile(name string) *file {
	openFiles++
	return &file{name}
}

func (f *file) Close() { openFiles-- }

func processOne(name string) {
	f := openFile(name)
	defer f.Close()
	fmt.Println("processing", f.name, "open files:", openFiles)
}

func main() {
	for _, name := range []string{"a.log", "b.log", "c.log"} {
		processOne(name)
	}
	fmt.Println("after loop, open files:", openFiles)
}
```

```output
processing a.log open files: 1
processing b.log open files: 1
processing c.log open files: 1
after loop, open files: 0
```

### A C++ destructor that throws ends the program

A destructor without a `noexcept` specifier is non-throwing.[^8] When an exception reaches the outermost block of a non-throwing function, the program calls `std::terminate`.[^8][^9] Here the destructor calls a function that throws, and the `catch` clause never runs.

```cpp fails expect=runtime id=throwing-destructor
#include <print>
#include <stdexcept>

void flush_log(bool fail) {
    if (fail) {
        throw std::runtime_error("flush failed");
    }
}

struct Log {
    bool fail;
    ~Log() { flush_log(fail); }
};

int main() {
    try {
        Log log{true};
        std::println("writing");
    } catch (const std::exception& error) {
        std::println("caught {}", error.what());
    }
}
```

Marking the destructor `noexcept(false)` does not help much: if the destructor throws during stack unwinding, the program still terminates.[^9] Give the class a `close` method that may throw, and let the destructor release without throwing if the caller did not close. The destructor of `std::basic_filebuf` does this: it calls `close()` and catches any exception.[^13]

```cpp run id=throwing-destructor-fixed
#include <print>
#include <stdexcept>

void flush_log(bool fail) {
    if (fail) {
        throw std::runtime_error("flush failed");
    }
}

struct Log {
    bool fail;
    bool closed = false;

    void close() {
        closed = true;
        flush_log(fail);
    }

    ~Log() {
        if (!closed) {
            try {
                flush_log(fail);
            } catch (...) {
                // A destructor must not throw; the caller can call close() to see the error.
            }
        }
    }
};

int main() {
    try {
        Log log{true};
        std::println("writing");
        log.close();
    } catch (const std::exception& error) {
        std::println("caught {}", error.what());
    }
}
```

```output
writing
caught flush failed
```

### A C++ temporary dies at the end of its statement

A temporary object is destroyed as the last step of the full-expression that made it.[^10] A guard without a name is a temporary, so it acquires and releases in the same statement, and the code after it runs unprotected.

```cpp wrong id=temporary-guard
#include <print>

struct Guard {
    Guard() { std::println("lock"); }
    ~Guard() { std::println("unlock"); }
};

int main() {
    Guard{};
    std::println("update the shared counter");
}
```

```output
lock
unlock
update the shared counter
```

Give the guard a name, so that it lives to the end of the block. With the Clang and libc++ that recorded these examples, `std::lock_guard<std::mutex>{m};` does not compile under `-Werror`, because the constructor is marked `[[nodiscard]]`. `std::lock_guard<std::mutex>(m);` declares a variable named `m` instead, and fails too. A guard type of your own gets no such check.

```cpp run id=temporary-guard-fixed
#include <print>

struct Guard {
    Guard() { std::println("lock"); }
    ~Guard() { std::println("unlock"); }
};

int main() {
    Guard guard;
    std::println("update the shared counter");
}
```

```output
lock
update the shared counter
unlock
```

### Rust does not guarantee that Drop runs

`std::mem::forget` takes a value and never runs its destructor. It is safe code, because the safety rules of Rust do not include a promise that destructors run. A cycle of `Rc` values has the same effect.[^16][^14] Here a forgotten `MutexGuard` leaves the mutex locked.

```rust wrong id=forget-guard
use std::sync::Mutex;

fn main() {
    let orders = Mutex::new(Vec::new());
    let mut guard = orders.lock().unwrap();
    guard.push(1);
    std::mem::forget(guard);
    println!("lock free again: {}", orders.try_lock().is_ok());
}
```

```output
lock free again: false
```

Unsafe code must not depend on a destructor for soundness, for the same reason.[^14] To end a guard early, pass it to `drop`.

```rust run id=forget-guard-fixed
use std::sync::Mutex;

fn main() {
    let orders = Mutex::new(Vec::new());
    let mut guard = orders.lock().unwrap();
    guard.push(1);
    drop(guard);
    println!("lock free again: {}", orders.try_lock().is_ok());
}
```

```output
lock free again: true
```

### Python binds the result of `__enter__`, not the manager

The `as` target of a `with` statement gets the return value of `__enter__`.[^18][^19] A manager whose `__enter__` has no `return` gives `None`.

```python fails expect=exception:AttributeError id=enter-none
class Ledger:
    def __init__(self):
        self.entries = []

    def __enter__(self):
        print("lock ledger")

    def __exit__(self, exc_type, exc, traceback):
        print("unlock ledger")
        return False

    def add(self, amount):
        self.entries.append(amount)


with Ledger() as ledger:
    ledger.add(10)
```

Return `self` from `__enter__` when the target should be the manager. Some managers return another object on purpose: a `threading.Lock` gives the result of `acquire`.

```python run id=enter-self
import threading


class Ledger:
    def __init__(self):
        self.entries = []

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc, traceback):
        return False

    def add(self, amount):
        self.entries.append(amount)


with Ledger() as ledger:
    ledger.add(10)
print("entries:", ledger.entries)

with threading.Lock() as held:
    print("target of a lock:", held)
```

```output
entries: [10]
target of a lock: True
```

## Cost

- **Destructor calls.** A destructor is an ordinary function that the compiler calls at each exit of the scope. The standard defines when and in which order, not how.[^4][^5] The destructor of `std::unique_ptr` is one test and one call of the deleter.[^12]
- **Statement forms.** A `with`, `using` or try-with-resources block costs about what the equivalent `try` and `finally` costs. Python looks up and calls two methods per block.[^18] Java adds a null check per resource and the bookkeeping for suppressed exceptions.[^23]
- **Go defer.** Each execution of `defer` evaluates and saves the function value and its arguments.[^32] In a loop, this storage grows with the number of iterations until the function returns.
- **Finalizers.** A finalizer is not free even when it does nothing. In .NET, an object with a finalizer gets an entry in the Finalize queue, and an empty finalizer still costs performance.[^29] The dispose pattern calls `GC.SuppressFinalize` after `Dispose`, so the collector no longer needs to run the finalizer.[^28] JEP 421 lists the costs of finalization for the JDK and for every program.[^26]

## History

- **1980.** Destructors were among the first features of C with Classes. Stroustrup writes that resource management motivated them: constructors acquire resources, and destructors release them.[^3]
- **1990.** Andrew Koenig and Stroustrup presented exception handling for C++ at the USENIX C++ Conference in April 1990. Stroustrup writes that this paper and the exception design of the ARM waited about half a year. He held them back until he had found "resource acquisition is initialization".[^3]
- **2001.** The first edition of ECMA-334, the C# specification, defined the `using` statement on `System.IDisposable`.[^31]
- **2005.** PEP 343 proposed the `with` statement for Python 2.5.[^22]
- **2011.** JSR 334, the small language changes for Java SE 7, reached its final release on 20 July 2011. It included the try-with-resources statement.[^25]
- **2019.** C# 8.0 added the `using` declaration.[^30]
- **2021.** JEP 421, created in September 2021, deprecated finalization for removal and named try-with-resources and cleaners as the replacement. It was delivered in JDK 18.[^26]
- **Stage 4.** The Explicit Resource Management proposal for JavaScript is on the TC39 list of finished proposals. The list gives 2027 as its expected year of publication in ECMA-262.[^37]

## References
