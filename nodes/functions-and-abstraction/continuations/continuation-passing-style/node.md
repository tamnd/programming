---
tag: 6X37N0
kind: concept
slug: continuation-passing-style
name: Continuation-passing style
aliases: [CPS, continuation, continuations, CPS transform, callback style, call/cc]
area: functions-and-abstraction/continuations
level: core
summary: >-
  In continuation-passing style a function does not return; it passes its result to an extra argument, the continuation, which is a function that does the rest of the computation.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1964
  people: [Adriaan van Wijngaarden, Christopher Strachey, Christopher Wadsworth, Gerald Jay Sussman, Guy L. Steele Jr.]
  note: Van Wijngaarden described a CPS transform for Algol 60 in 1964, and Sussman and Steele named continuation-passing style in their 1975 report on Scheme.
see_also: [closures, higher-order-functions, recursion, async-await, futures, exception-handling, iterators, order-of-evaluation]
same_as:
  wikidata: Q749893
  wikipedia: Continuation-passing_style
sources:
- url: https://small.r7rs.org/attachment/r7rs.pdf
  title: Revised7 Report on the Algorithmic Language Scheme, 2013
  locator: section 6.10, Control features, call-with-current-continuation
- url: https://www.cs.ru.nl/~freek/courses/tt-2011/papers/cps/histcont.pdf
  title: John C. Reynolds, The Discoveries of Continuations, LISP and Symbolic Computation 6, 233-247, 1993
  locator: abstract; sections 2 and 8
- url: https://nodejs.org/en/learn/asynchronous-work/javascript-asynchronous-programming-and-callbacks
  title: 'Node.js documentation: JavaScript Asynchronous Programming and Callbacks'
  locator: Callbacks; Handling errors in callbacks
---

## Idea

In continuation-passing style (CPS), a function does not return its result. It takes an extra argument, the continuation, which is a function that stands for the rest of the computation, and it calls the continuation with the result. A direct-style factorial returns `n * fact(n - 1)`; a CPS factorial calls `fact_k(n - 1, k2)`, where the new continuation `k2` multiplies by `n` and passes the product to the old continuation `k`. Every call in CPS is a tail call, and the order of every step is explicit. Compilers use CPS as an intermediate form, interpreters use it to implement exceptions, generators and `call/cc`, and asynchronous code uses it as callbacks.

```python run id=idea
def fact(n):  # direct style: the function returns its result
    return 1 if n == 0 else n * fact(n - 1)


def fact_k(n, k):  # CPS: the function passes its result to k
    if n == 0:
        return k(1)
    return fact_k(n - 1, lambda r: k(n * r))


print(fact(5), fact_k(5, lambda r: r))
fact_k(5, lambda r: print("the continuation got", r))

done = []


def product_k(xs, k, abort):
    if not xs:
        return k(1)
    if xs[0] == 0:
        return abort(0)  # skip every pending multiplication

    def rest(r):
        done.append(xs[0])
        return k(xs[0] * r)

    return product_k(xs[1:], rest, abort)


identity = lambda r: r
print(product_k([2, 3, 4], identity, identity), done)
done.clear()
print(product_k([2, 3, 0, 4], identity, identity), done)


def safe_div(a, b, ok, fail):  # two continuations: one for each outcome
    return fail("division by zero") if b == 0 else ok(a / b)


safe_div(1, 4, lambda v: print("ok", v), lambda e: print("error:", e))
safe_div(1, 0, lambda v: print("ok", v), lambda e: print("error:", e))
```

```output for=idea
120 120
the continuation got 120
24 [4, 3, 2]
0 []
ok 0.25
error: division by zero
```

`fact` waits for the recursive call to return and then multiplies. `fact_k` multiplies inside the continuation, so each call hands over all the remaining work and never needs to come back. The caller chooses what happens to the result: the identity function returns it, and the second continuation prints it. `product_k` takes two continuations: `k` for the normal result and `abort` for a zero. When it meets a zero, it calls `abort`, and the multiplications that wait in the chain of continuations never run, so `done` stays empty. `safe_div` uses two continuations in place of a return value and an exception.

## How it works

**The continuation is the rest of the program.** At each point of a computation, something waits for the value that the current expression computes. The Scheme report puts it this way: "Whenever a Scheme expression is evaluated there is a continuation wanting the result of the expression. The continuation represents an entire (default) future for the computation."[^1] In direct style the continuation is implicit: it is the return address and the stack frames below it. CPS makes it a value that the program can pass, store, call once, call twice or never call.

**The CPS transform.** A mechanical translation turns any direct-style program into CPS. Each function gets an extra parameter `k`. A function that returns `e` becomes a function that calls `k(e)`. A call whose result is used later, as in `n * f(x)`, becomes a call to `f_k(x, lambda r: ...)`, and the code that used the result moves into the new continuation. After the transform, every call is a tail call, every intermediate value has a name, and the order of evaluation is fixed by the nesting of the continuations. Reynolds lists the uses: continuations "underlie a method of program transformation (into continuation-passing style), a style of definitional interpreter (defining one language by an interpreter written in another language), and a style of denotational semantics".[^2] See [order of evaluation](/order-of-evaluation/).

**Control flow becomes function calls.** Because the rest of the program is a value, any jump is a call to the right continuation. An early exit calls an outer continuation and skips the inner ones, as `product_k` does. An exception handler is a second continuation that every function passes down, and `raise` calls it. A generator keeps the continuation of its loop and calls it when the consumer asks for the next item. Backtracking keeps a continuation for each choice that it has not tried yet. Scheme's `call-with-current-continuation` gives direct-style code access to its own continuation, and the report notes that it "is useful for implementing a wide variety of advanced control structures".[^1] See [exception handling](/exception-handling/) and [iterators](/iterators/).

**The stack in CPS.** CPS does not remove the stack; it moves it into the chain of continuation closures on the heap. In a language that eliminates tail calls, such as Scheme, a CPS program runs in constant stack space. In Python, Java, C# and JavaScript, each call still pushes a frame, so a deep CPS recursion overflows the stack sooner than the direct version, because it nests the calls to the continuations as well. A trampoline fixes this: each step returns the next step as a function, and a loop runs the steps. The Python example file shows a trampoline.

**Callbacks are continuations.** An asynchronous operation cannot return its result, because the result does not exist yet when the call returns. So the operation takes a callback and calls it later. The Node.js documentation describes it: "A callback is a simple function that's passed as a value to another function, and will only be executed when the event happens."[^3] Node.js passes errors as the first argument of the callback, a convention it calls "error-first callbacks".[^3] Long chains of such callbacks nest deeply and are hard to read, so promises and `async`/`await` came to hide them. An `async` function treats the code after each `await` as a continuation, so the programmer writes direct style and the program runs in CPS. See [async/await](/async-await/) and [futures](/futures/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | functions as arguments; a trampoline for depth |
| JavaScript | idiom | callbacks; error-first callbacks in Node.js; `await` |
| TypeScript | idiom | as JavaScript, with typed callbacks |
| Java | idiom | `Function` and `Consumer`; `CompletableFuture.thenApply` |
| Kotlin | idiom | lambdas; the compiler turns `suspend` functions into CPS |
| C# | idiom | `Func` and `Action` delegates; `Task.ContinueWith` |
| Go | idiom | function values; growing goroutine stacks |
| Rust | idiom | closures; `Box<dyn FnOnce>` for recursive CPS |
| C | idiom | a function pointer and a `void *` for its data |
| C++ | idiom | lambdas as template parameters; `std::function` |
| Bash | idiom | the name of a function passed as an argument |
| PowerShell | idiom | script blocks passed as arguments |
| SQL | n/a | no functions as values |
| Regex | n/a | no functions or control flow |
| HTML | n/a | no functions |
| CSS | n/a | no functions as values |

Every core language with first-class functions can write CPS by hand, so the verdict is idiom for all of them. None of them offers a first-class continuation of the current point of the program, as `call/cc` in Scheme does. The languages differ in two ways. The first is depth: Go grows its stacks, so a deep CPS recursion runs, while Python, Java, C# and JavaScript overflow and need a trampoline. The second is types: Rust and C++ give each closure its own type, so a recursive CPS function must take a boxed `dyn FnOnce` or a `std::function`, and the Rust example file shows the compile error of the generic version.

CPS is also the hidden form of the asynchronous features of these languages. The compilers of C#, Rust and Kotlin turn an `async` or `suspend` function into a state machine whose states are the continuations after each `await`. JavaScript and Python engines keep the suspended frame of the function and resume it when the awaited value is ready. In both designs, the code after each `await` is the continuation. Kotlin makes the transform visible: each `suspend` function gets an extra `Continuation` parameter in the compiled code.

## Variations and related ideas

- **First-class continuations.** Scheme's `call/cc` and the `callCC` of Haskell's `Cont` monad capture the current continuation of direct-style code as a value. A program can store it and call it later to return to that point.
- **Delimited continuations.** `shift` and `reset` and similar operators capture the continuation only up to a marked point, so the captured part is an ordinary function that returns. Effect handlers in OCaml 5 and other languages build on them.
- **CPS as an intermediate language.** Compilers for Scheme, ML and other functional languages convert the program to CPS, because every value has a name and every jump is explicit. Static single assignment form, used by most compilers today, is closely related. See [bytecode](/bytecode/).
- **Defunctionalization.** The closures of a CPS program can become a data type with one case for each kind of continuation, which turns a recursive interpreter into a loop with an explicit stack. See [tree-walking interpreter](/tree-walking-interpreter/).
- **Callbacks and promises.** Asynchronous APIs pass continuations as callbacks. A promise holds a list of continuations and calls them when it settles. See [futures](/futures/).
- **Coroutines and generators.** A suspended coroutine is a continuation that the runtime stores, and resuming the coroutine calls it.
- **Monads.** The bind operation of a monad takes the rest of the computation as a function, which is CPS in a typed form. See [higher-order functions](/higher-order-functions/).
- **Tail calls.** CPS needs proper tail calls to run in constant stack space. See [recursion](/recursion/).

## Pitfalls

- **A path that forgets the continuation.** If one branch returns a value instead of calling `k`, the rest of the program never runs on that branch, or runs with the wrong value. The wrong block in the Python file shows this.
- **A continuation called twice.** A callback that falls through after an error calls the continuation a second time with a wrong value. The wrong block in the JavaScript file shows this. Return at the point where the continuation is called.
- **Stack overflow.** Without tail call elimination, CPS uses more stack than the direct version. Use a trampoline, a loop, or a language that eliminates tail calls.
- **Callback nesting.** Each step of an asynchronous sequence adds a level of indentation, and error handling repeats at every level. Use promises, `async`/`await` or named functions.
- **Lost errors.** An exception thrown inside a callback does not reach the `try` of the code that started the operation, because that code has already returned. Pass errors to a continuation, or use promises that carry them.
- **Captures that outlive their data.** A continuation that runs later must not hold pointers or references to stack data that is gone. The C and C++ example files use the stack only because every continuation runs before the function returns.

## Cost

A call in CPS costs the same as a call in direct style, plus the creation of a closure for each new continuation. In most languages that is a heap allocation for each step, which makes a hand-written CPS loop several times slower than the direct loop, and it adds work for the garbage collector. A trampoline adds a return and an indirect call for each step. Compilers that use CPS internally do not pay these costs, because they allocate most continuations on the stack or turn them into jumps. The memory of a CPS computation is the chain of continuations, which grows as the stack of the direct version would grow.

## History

Continuations were found many times. Reynolds wrote: "In the early history of continuations, basic concepts were independently discovered an extraordinary number of times."[^2] Adriaan van Wijngaarden described the first CPS transform in September 1964, at an IFIP working conference in Baden bei Wien: to translate Algol 60 into a smaller language, he gave each procedure an extra parameter, a label, and ended its body with a jump to that label. Antoni Mazurkiewicz, Lockwood Morris, Christopher Wadsworth, James Morris, Michael Fischer and Kamal Abdali found related ideas between 1969 and 1973. Christopher Strachey and Wadsworth used continuations to give a denotational semantics for jumps, in a report of 1974.[^2]

Gerald Jay Sussman and Guy L. Steele Jr. described Scheme in 1975 and used the name continuation-passing style. Steele's Rabbit compiler for Scheme, in 1978, converted programs to CPS as its intermediate form, and Andrew Appel's book *Compiling with Continuations*, in 1992, described the CPS compiler of Standard ML of New Jersey. Scheme added `call-with-current-continuation` to its standard reports. Node.js, from 2009, built its asynchronous API on error-first callbacks.[^3] Promises in ES2015 and `async` functions in ES2017, C# 5 and Python 3.5 then let programmers write direct-style code that compilers turn into continuations.

## References
