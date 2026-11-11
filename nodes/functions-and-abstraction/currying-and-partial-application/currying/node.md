---
tag: KJT04W
kind: concept
slug: currying
name: Currying and partial application
aliases: [currying, partial application, curried function, uncurrying, bind, bind_front, functools.partial]
area: functions-and-abstraction/currying-and-partial-application
level: core
summary: >-
  Currying turns a function of many arguments into a chain of functions of one argument. Partial application fixes some arguments of a function and returns a function of the rest.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1924
  people: [Moses Schönfinkel, Haskell Curry]
  note: Schönfinkel reduced functions of several arguments to functions of one argument, and Curry used the device in combinatory logic.
see_also: [closures, higher-order-functions, parameter-passing, lexical-scope, recursion]
same_as:
  wikidata: Q1144925
  wikipedia: Currying
sources:
- url: https://www.cs.cmu.edu/~crary/819-f09/Strachey67.pdf
  title: Fundamental Concepts in Programming Languages, C. Strachey, lectures of 1967, Higher-Order and Symbolic Computation 13, 2000
  locator: 3.2.4 Evaluation
- url: https://www.haskell.org/onlinereport/haskell2010/haskellch3.html
  title: Haskell 2010 Language Report, Expressions
  locator: 3.3 Curried Applications and Lambda Abstractions
- url: https://learn.microsoft.com/en-us/dotnet/fsharp/language-reference/functions/
  title: Functions, F# language reference, Microsoft Learn
  locator: Partial Application of Arguments
- url: https://docs.python.org/3.14/library/functools.html#functools.partial
  title: functools, Python 3.14 documentation
  locator: functools.partial
- url: https://docs.python.org/3.14/library/functools.html#functools.Placeholder
  title: functools, Python 3.14 documentation
  locator: functools.Placeholder
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Function/bind
  title: Function.prototype.bind(), MDN Web Docs
  locator: Partially applied functions
- url: https://en.cppreference.com/w/cpp/utility/functional/bind_front
  title: std::bind_front, std::bind_back, cppreference.com
  locator: description
---

## Idea

A function of two arguments, such as `power(base, exponent)`, can be seen as a function of one argument that returns a function of one argument. `power(2)` is then the function "two to the power of", and `power(2)(10)` is 1024. Currying is this change of form: a function of n arguments becomes a chain of n functions of one argument. Partial application is the related operation that fixes some arguments of a function and returns a function of the rest. The two ideas let a program make a specific function from a general one without writing a new definition:

```python run id=idea
from functools import partial


def power(base, exponent):
    return base**exponent


square = partial(power, exponent=2)
print([square(n) for n in range(5)])
```

```output for=idea
[0, 1, 4, 9, 16]
```

In ML, OCaml, F# and Haskell, functions are curried by default, and a call with fewer arguments is a partial application with no extra syntax. In most other languages, a closure or a library function does the same work.

## How it works

**One argument at a time.** Christopher Strachey described the device in his lectures of 1967: "There is a device originated by Schönfinkel, for reducing operators with several operands to the successive application of single operand operators." Instead of `+(2, p)`, a one-argument operator `+′` gives `+′(2)`, "a function which adds 2 to its argument". Strachey added a rule that application associates to the left, so `+′ 2 p` means `(+′ 2) p`.[^1] The Haskell report uses the same rule: "Function application is written e1 e2. Application associates to the left, so the parentheses may be omitted in (f x) y."[^2] So in Haskell, `add 1 2` is the application of `add 1` to `2`, and `add 1` alone is a function.

**Types.** The type of a curried function shows the chain. A function from two integers to an integer has the type `Int -> Int -> Int`, which means `Int -> (Int -> Int)`. Uncurrying is the reverse change, to a function that takes a pair: `(Int, Int) -> Int`. The two types hold the same information, and a program can convert between them with two small higher-order functions, usually called `curry` and `uncurry`.

**Partial application.** Partial application fixes some arguments and leaves the rest open. It does not need the target function to be curried. The F# documentation states: "If you supply fewer than the specified number of arguments, you create a new function that expects the remaining arguments. This method of handling arguments is referred to as currying".[^3] In Python, `functools.partial` returns "a new partial object which when called will behave like func called with the positional arguments args and keyword arguments keywords". Arguments in the later call are appended to the fixed ones.[^4] `functools.Placeholder` reserves a position, so that a later argument can fill a gap in the middle of the fixed arguments.[^5] In JavaScript, `bind` fixes the `this` value and leading arguments, which "are then inserted at the start of the arguments passed to the target function".[^6] In C++, `std::bind_front` and `std::bind_back` fix the first or the last arguments of a callable.[^7]

**Closures are the mechanism.** A partially applied function must keep the fixed arguments until it is called. That is a closure: a function together with the values that it captured. In a language with closures, partial application is one line: `b => f(a, b)`. In C, which has no closures, the program must store the function pointer and the fixed arguments in a struct. See [closures](/closures/).

**When the arguments are evaluated.** The fixed arguments are evaluated once, when the partial application is made. A closure that refers to a variable reads it when it runs. The difference matters in loops. A lambda made in a loop that reads the loop variable sees its last value, but `partial(f, n)` keeps the value of `n` at that point.

**Argument order matters.** Partial application of the leading arguments works best when the most general argument comes first. Libraries designed for currying put the data last, so that `map(f)` is a function that applies `f` to any list. Libraries designed for method calls put the data first, as the receiver.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `functools.partial`, closures |
| JavaScript | native | arrow functions, `bind` |
| TypeScript | native | as JavaScript, with generic types |
| Java | idiom | `Function<A, Function<B, R>>`, lambdas |
| Kotlin | idiom | lambdas that return lambdas |
| C# | idiom | `Func<A, Func<B, R>>`, lambdas |
| Go | idiom | closures, method values |
| Rust | idiom | closures with `move`, `impl Fn` |
| C | absent | a struct with a function pointer and the fixed arguments |
| C++ | stdlib | `std::bind_front`, `std::bind_back`, lambdas |
| Bash | idiom | a command prefix in an array, wrapper functions |
| PowerShell | idiom | script blocks |
| SQL | absent | functions are not values |
| Regex, HTML, CSS | n/a | no functions as values |

**Python** has `functools.partial` and `partialmethod`. Functions are not curried, and a call with too few arguments is a `TypeError`.

**JavaScript** and **TypeScript** curry with nested arrow functions, `a => b => a + b`, and partially apply with `bind`. A generic `curry` helper depends on the `length` of the function, which does not count rest parameters or parameters with default values.

**Java** and **C#** can express curried functions with nested function types and lambdas. Neither has a partial application function in the standard library.

**Go** returns closures. A method value, such as `sb.WriteString`, fixes the receiver of a method.

**Rust** returns closures with `impl Fn`. Each closure has its own anonymous type, so a collection of different closures needs `Box<dyn Fn>`.

**C** needs a struct for the fixed arguments. **C++** has `std::bind_front`, `std::bind_back` and lambdas. `std::bind` and its placeholders are older and harder to read.

**Bash** stores a command and its first arguments in an array, or writes a small function that calls another one with a fixed first argument.

## Variations and related ideas

- **Automatic currying.** In Haskell, OCaml and F#, each function takes one argument, and functions of many arguments are curried by default. In Scala, a method can have many parameter lists.
- **Point-free style.** With curried functions and composition, a function can be defined without naming its arguments: `sumSquares = sum . map (^2)`. The style is short, but it can be hard to read when it is overused.
- **Placeholders.** Scala `_`, C++ `std::placeholders` and Python `functools.Placeholder` fix arguments that are not at the start.
- **Method references and bound methods.** `obj.method` in Python and `sb.WriteString` in Go fix the receiver. See [higher-order functions](/higher-order-functions/).
- **Default arguments.** A default value fixes an argument for all callers. Partial application fixes it for one use.

## Pitfalls

- **Too many arguments in one call.** In JavaScript, `add(1, 2)` on a curried `add` ignores the second argument and returns a function.
- **Late binding in loops.** A closure made in a loop sees the last value of the loop variable. Partial application fixes the current value.
- **Copies in C++.** `std::bind_front` and `std::bind` copy their arguments. To bind a reference, use `std::ref`.
- **`length` in JavaScript.** A generic `curry` helper calls the function too early when the function has rest parameters or default values.
- **Error messages.** In a language that curries automatically, a missing argument does not cause an error at the call. The error appears later, where a function is used as if it were a value of another type.
- **Readability.** A chain of partial applications and compositions can hide what a function does. Name the intermediate functions.

## Cost

A partially applied function is a closure, so making one allocates a small object in most languages. A call goes through one more level of indirection. Compilers for languages that curry by default, such as GHC and OCaml, optimize calls with all the arguments, so a curried function costs nothing extra when it gets all its arguments at once. In Rust and C++, each closure has its own type, and the compiler can inline the call completely. In Java and C#, a lambda that captures variables allocates an object, and a call through `Function` or `Func` is an interface or delegate call.

## History

Moses Schönfinkel described the reduction of functions of several arguments to functions of one argument in his paper "Über die Bausteine der mathematischen Logik", published in 1924.[^1] Haskell Curry used the same device throughout combinatory logic, and the name currying comes from him. Christopher Strachey brought it into the theory of programming languages in his lectures of 1967.[^1] ML, in the 1970s, made curried functions the normal style, and Miranda, Haskell, OCaml and F# followed. Partial application came to other languages as a library feature: `functools.partial` in Python in 2006, `Function.prototype.bind` in ECMAScript 5 in 2009, and `std::bind_front` in C++20.

## References
