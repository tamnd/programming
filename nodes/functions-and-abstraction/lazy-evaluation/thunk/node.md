---
tag: 82JJDC
kind: concept
slug: thunk
name: Thunk
aliases: [suspended computation, delayed computation, promise (Scheme), delay and force, suspension]
area: functions-and-abstraction/lazy-evaluation
level: advanced
summary: >-
  A thunk is a suspended computation: code with no parameters that computes a value when a program forces it. Thunks implement call by name, call by need and lazy values.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1961
  people: [Peter Ingerman]
  note: Ingerman's paper "Thunks", in Communications of the ACM in January 1961, named the helper routines that an Algol 60 compiler made to compute call-by-name arguments.
see_also: [laziness, strict-evaluation, closures, parameter-passing, lazy-sequence]
same_as:
  wikidata: Q2429957
  wikipedia: Thunk
sources:
- url: https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/full-text/book/book-Z-H-24.html
  title: Structure and Interpretation of Computer Programs, H. Abelson and G. J. Sussman, second edition, MIT Press
  locator: 3.5.1 Streams Are Delayed Lists, Implementing delay and force; footnote 58
- url: https://en.wikipedia.org/wiki/Thunk
  title: Thunk, Wikipedia
  locator: introduction; Background; Applications, Functional programming; references 1 and 2
- url: https://www.masswerk.at/algol60/report.htm
  title: Revised Report on the Algorithmic Language Algol 60, P. Naur, editor
  locator: 4.7.3.2 Name replacement (call by name)
- url: https://standards.scheme.org/official/r7rs.pdf
  title: Revised7 Report on the Algorithmic Language Scheme
  locator: 4.2.5 Delayed evaluation
- url: https://www.haskell.org/onlinereport/haskell2010/haskellch6.html
  title: Haskell 2010 Language Report, Predefined Types and Classes
  locator: 6.2 Strict Evaluation
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/function/Supplier.html
  title: Interface Supplier, Java SE 25 API documentation
  locator: description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.lazy-1
  title: Lazy<T> Class, .NET API documentation
  locator: Remarks, Exception caching
- url: https://pkg.go.dev/sync
  title: sync package, Go standard library
  locator: func OnceValue
- url: https://doc.rust-lang.org/std/cell/struct.LazyCell.html
  title: LazyCell, Rust standard library
  locator: description
---

## Idea

A thunk is a computation that has not run yet. It is a small piece of code with no parameters, together with the variables that the code needs. A program creates the thunk at one point and runs it later, when it needs the value. Running a thunk is called forcing it. If the thunk stores its result after the first force, later forces return the stored value:

```python run id=idea
class Thunk:
    def __init__(self, compute):
        self.compute = compute
        self.done = False
        self.value = None

    def force(self):
        if not self.done:
            self.value = self.compute()
            self.done = True
            self.compute = None
        return self.value


def slow_square(n):
    print("computing", n, "squared")
    return n * n


answer = Thunk(lambda: slow_square(12))
print("thunk created")
print(answer.force())
print(answer.force())
```

```output for=idea
thunk created
computing 12 squared
144
144
```

The lambda is the suspended computation. `Thunk` adds the stored result, so `slow_square` runs one time. Thunks are the machinery under [lazy evaluation](/laziness/), call by name and lazy values.

## How it works

**Delay packages an expression.** SICP builds its streams from two operations, `delay` and `force`. It states that `delay` "must package an expression so that it can be evaluated later on demand, and we can accomplish this simply by treating the expression as the body of a procedure."[^1] So `(delay <exp>)` can be the same as `(lambda () <exp>)`, and `force` calls that procedure. In any language with [closures](/closures/), a function with no parameters is a thunk: the closure keeps the variables of the expression alive until the force.

**Force with or without memory.** A plain thunk runs its code at each force. SICP adds memoization: "the first time they are forced, they store the value that is computed. Subsequent forcings will simply return the stored value without repeating the computation."[^1] The book connects the two forms to two parameter modes: "The Algol community would refer to our original delayed objects as call-by-name thunks and to the optimized versions as call-by-need thunks."[^1]

**The state of a memoized thunk.** A memoized thunk has two states. Before the force, it holds code and an environment. After the force, it holds a value, and it can drop the code and the environment, so that the garbage collector can free the objects that only the computation needed. The example above sets `compute` to `None` for this reason. An implementation of a lazy language often overwrites the thunk object in place with its value, so every reference to the thunk sees the value. Wikipedia notes that compilers for lazy languages, "such as the Glasgow Haskell Compiler, have relied heavily on thunks, with the added feature that the thunks save their initial result so that they can avoid recalculating it".[^2]

**Thunks for parameters.** Call by name passes an argument without evaluating it. The Algol 60 report defines it as text replacement: the formal parameter "is replaced, throughout the procedure body, by the corresponding actual parameter".[^3] A compiler cannot copy the text of the argument into the procedure for every call, so it passes a thunk instead: a routine that computes the argument in the environment of the caller. Wikipedia describes the step: "the compiler can generate a helper subroutine, called a thunk, that calculates the value of the argument."[^2]

**Thunks in the language.** Scheme has thunks as a built-in feature. The R7RS report states that `delay` "is used together with the procedure force to implement lazy evaluation or call by need", and that the value of a promise "must be cached" so that "if it is forced a second time, the previously computed value is returned."[^4] In Haskell, "a function argument is evaluated only when required", so an implementation passes thunks, and `seq` forces a value.[^5]

## In languages

| Language | Verdict | Plain thunk | Memoized thunk |
|---|---|---|---|
| Python | idiom | `lambda: expr` | a class, or `functools.cached_property` on an attribute |
| JavaScript | idiom | `() => expr` | a closure with a flag |
| TypeScript | idiom | `() => T` | a closure with a flag |
| Java | stdlib | `Supplier<T>` | a `Supplier` wrapper, or a holder class |
| Kotlin | stdlib | `() -> T` | `lazy { }` |
| C# | stdlib | `Func<T>` | `Lazy<T>` |
| Go | stdlib | `func() T` | `sync.OnceValue` |
| Rust | stdlib | a closure, `impl Fn() -> T` | `LazyCell`, `LazyLock` |
| C | idiom | a function pointer and a context pointer | a struct with a flag |
| C++ | stdlib | a lambda, `std::function<T()>` | a lambda with `std::optional` |
| Bash | idiom | a function name or a string for `eval` | a variable that a function fills once |
| Haskell | native | every unevaluated expression | every unevaluated expression |
| Scheme | native | `(lambda () expr)` | `delay` and `force` |

**Java.** `Supplier<T>` "Represents a supplier of results", and the documentation states that there is "no requirement" that each call returns a distinct result.[^6] So a `Supplier` is a plain thunk, and memoization is the job of the program.

**C#.** `Func<T>` is a plain thunk, and `Lazy<T>` is a memoized one. `Lazy<T>` also stores an error: if the factory "throws an exception the first time a thread tries to access the Value property of the Lazy<T> object, the same exception is thrown on every subsequent attempt."[^7]

**Go.** `sync.OnceValue` "returns a function that invokes f only once and returns the value returned by f. The returned function may be called concurrently."[^8]

**Rust.** A closure with no parameters is a plain thunk. `LazyCell` is "A value which is initialized on the first access."[^9] `LazyLock` is the form that threads can share.

## Variations and related ideas

**Call-by-name thunk and call-by-need thunk.** The first runs at each force, so its side effects happen at each force. The second runs at most one time. Most library types for laziness are call-by-need thunks.

**Promises in Scheme.** A Scheme promise is a memoized thunk. R7RS adds `delay-force` for lazy loops: it lets "iterative lazy algorithms that might result in a long series of chains of delay and force" run without "consuming unbounded space during evaluation."[^4]

**Lazy initialization.** A memoized thunk stored in a field or a global is lazy initialization: the value is created on the first access. See [lazy evaluation](/laziness/).

**Streams.** A stream is a pair of a value and a thunk for the rest. See [lazy sequence and stream](/lazy-sequence/).

**Other uses of the word.** Compilers and linkers also use the word thunk for small generated routines that adjust arguments before they call another routine. Wikipedia lists thunks for multiple inheritance in C++, which adjust the object address, and thunks that connect code with different calling conventions.[^2] These thunks do not delay anything, but they share the idea of a small helper routine that a tool makes for a call.

## Pitfalls

**Late binding of captured variables.** A thunk captures variables, not values. If the variable changes before the force, the thunk sees the last value:

```python wrong id=late-binding
thunks = [lambda: n * 10 for n in range(3)]
print([thunk() for thunk in thunks])
```

```output for=late-binding
[20, 20, 20]
```

All three lambdas share the one variable `n`, and the loop has ended at 2 when they run. Bind the value when you create the thunk:

```python run id=early-binding
thunks = [lambda n=n: n * 10 for n in range(3)]
print([thunk() for thunk in thunks])
```

```output for=early-binding
[0, 10, 20]
```

**Repeated work in a plain thunk.** A thunk without memoization runs its code at each force. If the code is slow or has side effects, they happen again and again. Use a memoized form, or force the thunk one time and keep the value.

**Errors at force time.** A thunk fails when a program forces it, not when the program creates it. The error can appear far from the code that made the thunk. R7RS states that a delayed expression "is evaluated using the parameter values and exception handler of the call to force which first requested its value".[^4] Some memoized thunks store the error too, as `Lazy<T>` and `sync.OnceValue` do, so a failed thunk fails at each force.

**Thunks that hold memory.** An unforced thunk keeps its environment alive. A long chain of unforced thunks can hold much more memory than the value that it describes. This is a space leak.

**Threads.** Two threads can force the same memoized thunk at the same time. A simple flag check can then run the code twice. Use a thread-safe form, such as `LazyLock` or `sync.OnceValue`. In .NET, "By default, all public and protected members of the Lazy<T> class are thread safe".[^7]

## Cost

A thunk costs a heap allocation for the code pointer and the captured variables. Each force costs a call through a pointer and, for a memoized thunk, a check of the ready flag. A thread-safe memoized thunk also needs an atomic operation or a lock on the first force. These costs are small for a single lazy value and large when a program makes a thunk for each element of a large structure. A lazy language tries to avoid most thunks: when the compiler can prove that a function always needs an argument, it evaluates the argument before the call and makes no thunk.

## History

The word comes from the first Algol 60 compilers. Wikipedia states that "The term originated as a whimsical irregular form of the verb think", and that it refers to the analysis that the compilers needed "to determine what type of routine to generate."[^2] Peter Ingerman described the method in "Thunks: a way of compiling procedure statements with some comments on procedure declarations", in Communications of the ACM in January 1961.[^2] Later, SICP noted that "Delayed evaluation, which is the key to making streams practical, was inherent in Algol 60's call-by-name parameter-passing method."[^1] Scheme made `delay` and `force` part of the language, and compilers for lazy languages such as Haskell came to rely on thunks for every unevaluated expression.[^2] Eager languages give thunks as library types, such as `Supplier<T>` in Java and `Lazy<T>` in .NET.[^6][^7]

## References
