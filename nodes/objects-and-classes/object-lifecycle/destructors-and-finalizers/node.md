---
tag: D6D0AW
kind: concept
slug: destructors-and-finalizers
name: Destructors and finalizers
aliases: [destructor, finalizer, finalization, drop, cleaner, cleanup callback, object lifetime, object resurrection]
area: objects-and-classes/object-lifecycle
level: core
summary: >-
  A destructor runs when an object's lifetime ends, at a point the program decides. A finalizer runs some time after a garbage collector finds the object unreachable, or never.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1980
  people: [Bjarne Stroustrup]
  note: C with Classes ran a function when an object was destroyed, C++ named it the destructor, and Java and C# gave garbage collected objects finalizers.
see_also: [resource-acquisition-is-initialization, garbage-collection, reference-counting, constructors, ownership, memory-leaks, use-after-free, defer-and-finally]
same_as:
  wikidata: Q735151
  wikipedia: Destructor_(computer_programming)
sources:
- url: https://en.cppreference.com/cpp/language/destructor
  title: Destructors, cppreference.com
  locator: Explanation
- url: https://doc.rust-lang.org/std/ops/trait.Drop.html
  title: Trait Drop, The Rust Standard Library
  locator: Drop; Drop order
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/finalizers
  title: Finalizers, C# programming guide, Microsoft Learn
  locator: Remarks
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/FinalizationRegistry
  title: FinalizationRegistry, MDN Web Docs
  locator: Notes on cleanup callbacks
- url: https://docs.python.org/3/reference/datamodel.html#object.__del__
  title: Data model, The Python Language Reference
  locator: 3.3.1 Basic customization, object.__del__
- url: https://pkg.go.dev/runtime#AddCleanup
  title: Package runtime, Go standard library
  locator: func AddCleanup
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/ref/Cleaner.html
  title: Cleaner, Java SE 25 API documentation
  locator: Class Cleaner
---

## Idea

A destructor is code that runs when an object's lifetime ends, and it releases what the object holds: memory, a file, a lock or a connection. A finalizer is code that a garbage collector runs some time after it finds that an object is unreachable. The two differ in timing. A destructor runs at a point that the program text decides, such as the end of a scope or a call to `delete`, so the program can rely on it. A finalizer runs when the collector decides, on a thread the program does not control, and perhaps never. C++ and Rust have destructors. Java, C#, Go and JavaScript have finalizers or cleanup callbacks, and use explicit `close` or `Dispose` calls for cleanup that must happen on time. CPython has both behaviors in one mechanism.

```python run id=idea
import gc


class Resource:
    def __init__(self, name):
        self.name = name
        print("open", name)

    def __del__(self):  # runs when CPython frees the object
        print("cleanup", self.name)


def work():
    r = Resource("a")
    print("using", r.name)
    # the last reference goes when the function returns


work()
print("after work")

b = Resource("b")
b.me = b  # a cycle: the count of b never reaches zero
del b
print("after del b")
gc.collect()  # the cycle collector finds b and runs its cleanup
print("after collect")
```

```output for=idea
open a
using a
cleanup a
after work
open b
after del b
cleanup b
after collect
```

`__del__` acts as a destructor for `a`: CPython counts references, the count reaches zero when `work` returns, and the cleanup runs before the next line. For `b`, the same method acts as a finalizer. `b` refers to itself, so its count never reaches zero, and the cleanup waits until the cycle collector runs. Here the program calls `gc.collect`, but a real program would get the cleanup at a time it cannot predict. Code that needs a file closed or a lock released at a known point cannot rely on the second case.

## How it works

**Destructors end a lifetime.** cppreference defines the C++ form: "A destructor is a special member function that is called when the lifetime of an object ends. The purpose of the destructor is to free the resources that the object may have acquired during its lifetime."[^1] The compiler knows where each lifetime ends, so it inserts the call: at the end of a scope for a local variable, at the destruction of the owner for a member or an element of a container, and at `delete` for an object on the heap. Rust does the same through the `Drop` trait: "When a value is no longer needed, Rust will run a “destructor” on that value."[^2] Because the call point is known, a destructor can release a resource that other code waits for, which is the basis of [RAII](/resource-acquisition-is-initialization/).

**The order of destruction.** An object can depend on objects that were created before it, so destruction runs in the reverse order of construction. Locals in a C++ or Rust scope go from last to first. Inside a C++ object, the body of the destructor runs first, while all members still exist, then the members go in the reverse order of their declaration, then the base classes. Rust drops the fields of a struct in the order of their declaration, after the struct's own `drop`. A destructor that runs through a pointer to a base class must be virtual in C++, or the destructors of the derived class do not run.

**Finalizers come after collection.** A tracing collector finds unreachable objects in batches, at times that depend on memory pressure. A finalizer runs after that, often on a separate thread. The C# documentation states it plainly: "The programmer has no control over when the finalizer is called; the garbage collector decides when to call it."[^3] A finalizer may also never run. MDN describes JavaScript: "A conforming JavaScript implementation, even one that does garbage collection, is not required to call cleanup callbacks."[^4] Python gives no guarantee for objects that still exist when the interpreter exits.[^5] So a finalizer must never be the only way to release a resource that matters: a program that opens files faster than the collector runs runs out of file descriptors long before it runs out of memory. See [garbage collection](/garbage-collection/).

**Finalizers can see a broken world.** When a group of objects becomes unreachable at once, the collector finalizes them in no particular order, so a finalizer cannot use the other objects of the group. The Go documentation of cleanups warns: "There is no specified order in which cleanups will run."[^6] A finalizer that receives the object can also store a reference to it somewhere reachable, which brings the object back to life. Python calls this resurrection: "It is possible (though not recommended!) for the __del__() method to postpone destruction of the instance by creating a new reference to it."[^5] Collectors must then check reachability again before they free the memory, which is one reason why objects with finalizers cost more to collect.

**Cleanup without the object.** Newer APIs avoid resurrection by never handing the object to the cleanup code. Java's `Cleaner`, Go's `runtime.AddCleanup`, JavaScript's `FinalizationRegistry` and Python's `weakref.finalize` register a separate action with the state that it needs, such as a file descriptor. The action must not refer to the object, or the object stays reachable forever. The Java documentation says: "the cleaning action must not refer to the object being registered. If so, the object will not become phantom reachable and the cleaning action will not be invoked automatically."[^7] Each of these APIs also lets the program run or cancel the action early, so the usual design is an explicit `close` that releases the resource and cancels the action, with the collector as a safety net for a forgotten `close`.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `__del__`; `weakref.finalize`; prompt in CPython for objects outside cycles |
| JavaScript | stdlib | `FinalizationRegistry`; `using` and `Symbol.dispose` |
| TypeScript | stdlib | as JavaScript |
| Java | stdlib | `java.lang.ref.Cleaner`; `finalize` deprecated |
| Kotlin | stdlib | `Cleaner` on the JVM; `use` for `Closeable` |
| C# | native | finalizers `~T()`; `IDisposable` and `using` |
| Go | stdlib | `runtime.AddCleanup`, `runtime.SetFinalizer`; `defer` |
| Rust | native | `Drop`; `std::mem::forget` |
| C | idiom | destroy functions; `atexit` |
| C++ | native | destructors `~T()` |
| Bash | idiom | `trap` on `EXIT` |
| PowerShell | idiom | .NET finalizers and `Dispose` |
| SQL | n/a | no objects to destroy |
| Regex | n/a | no objects |
| HTML | idiom | `disconnectedCallback` of custom elements |
| CSS | n/a | no objects |

C++ and Rust are the languages with true destructors: the compiler inserts each call, and the order is defined. C# and Python have syntax for a cleanup method that the collector calls, and C# calls it a finalizer for this reason, although its syntax looks like a C++ destructor. Java, Go, JavaScript and Kotlin offer library APIs that register a cleanup action. In all of these languages, the cleanup that must happen on time is an explicit call: `close`, `Dispose` or a function deferred to the end of a scope. See [defer and finally](/defer-and-finally/).

Each example file creates connections, closes them explicitly, and shows that a second close does nothing. The files for the collected languages also register a safety net and cancel it when the program closes the connection. The Rust and C++ files show the order of destruction, and the C file shows that without destructors, the caller must call each destroy function once and in the right order.

## Variations and related ideas

- **Dispose and close.** `IDisposable`, `AutoCloseable`, `io.Closer` and the context manager protocol give a method for explicit cleanup, which `using`, try-with-resources, `defer` and `with` call at the end of a block.
- **Reference counting.** CPython, Swift and `std::shared_ptr` run the cleanup when the last reference goes, which makes the time predictable except for cycles. See [reference counting](/reference-counting/).
- **Weak and phantom references.** A weak reference lets a program notice that an object was collected. Java's phantom references and reference queues are the machinery under `Cleaner`.
- **Ownership.** Rust moves ownership so that each value has exactly one owner, and the destructor runs when that owner goes away. See [ownership](/ownership/).
- **Linear and affine types.** A type that must be used exactly once can force a program to call a specific function to destroy it, which some research languages explore.
- **Construction.** Destruction mirrors construction: the object is built bases first and members in order, and taken apart in reverse. See [constructors](/constructors/).
- **Process exit.** The operating system reclaims memory, files and sockets when a process ends, so some programs skip their destructors at exit to save time. Handlers registered with `atexit`, and destructors of static objects in C++, run at a normal exit.

## Pitfalls

- **Relying on a finalizer for a scarce resource.** Files, sockets and locks can run out before the collector runs. Close them explicitly, and keep the finalizer as a safety net only.
- **A cleanup that refers to its object.** A lambda or inner class that captures `this` keeps the object reachable, so the cleanup never runs. The Java, Go and JavaScript files pass only the state that the cleanup needs.
- **Resurrection.** A finalizer that stores its object makes it reachable again, and the finalizer does not run a second time. The wrong block in the Python file shows this.
- **Exceptions from destructors.** A C++ destructor that throws during stack unwinding ends the program with `std::terminate`, and exceptions from Java cleaning actions and Python `__del__` are ignored. Report errors from cleanup in another way, or give the class an explicit `close` that can fail.
- **Missing virtual destructor.** In C++, `delete` through a pointer to a base class without a virtual destructor is undefined behavior. Declare the destructor of a polymorphic base class `virtual`.
- **Use after destruction.** A pointer or reference that outlives its object leads to [use after free](/use-after-free/) in C and C++. Rust's borrow checker rejects such code.
- **Double destruction.** Calling a destroy function twice in C, or calling a destructor explicitly in C++ and then letting the scope end, destroys the object twice. Make `close` safe to call twice, as every example file does.
- **Leaks through forgotten cleanup.** A forgotten destroy call in C, a `std::mem::forget` in Rust, or a reference cycle leaks the resource. See [memory leaks](/memory-leaks/).

## Cost

A destructor costs a call and the work it does, and compilers inline small ones; a type without a destructor costs nothing at the end of its lifetime. A virtual destructor adds an indirect call. Finalizers cost more: the collector must track each object that has one, keep it alive for one more cycle so that the finalizer can run, and run the finalizers on a separate thread. Objects with finalizers therefore survive longer and slow down collection. Cleanup actions in the style of `Cleaner` cost a registration and a small extra object for each registered object, and cancelling an action after an explicit close removes most of the collector's extra work.

## History

C with Classes, from 1980, had a function that ran when an object was destroyed, and C++ named it the destructor and gave it the syntax `~T()`. Java 1.0, in 1996, gave every object a `finalize` method, and C#, from 2002, borrowed the syntax of C++ destructors for its finalizers, which its documentation later renamed to reflect their behavior.[^3] Experience showed the problems of finalizers: unpredictable timing, resurrection and slow collection. Java 9 deprecated `finalize` in 2017 and added `Cleaner`, and Java 18 deprecated finalization for removal in 2022.[^7]

Python 3.4, in 2014, made the cycle collector able to free cycles of objects with `__del__` methods, and added `weakref.finalize`. Rust, from 2015, gave types destructors through `Drop`; shortly before its first stable release, its designers made `std::mem::forget` safe, after they found that reference cycles could leak values without it. ECMAScript 2021 added `WeakRef` and `FinalizationRegistry` to JavaScript.[^4] Go 1.24, in 2025, added `runtime.AddCleanup` as a safer alternative to `runtime.SetFinalizer`.[^6]

## References
