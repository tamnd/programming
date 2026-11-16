---
tag: RXZVPN
kind: concept
slug: function-composition
name: Function composition
aliases: [compose, composition of functions, function chaining, compose operator, andThen]
area: functions-and-abstraction/functions-as-values
level: core
summary: >-
  Function composition joins two functions into one: the output of the first is the input of the second. In mathematics (f ∘ g)(x) = f(g(x)). Haskell, F# and Java have it in the language or library.
applies_to: core-languages
viz: optional
formal: none
see_also: [first-class-function, higher-order-functions, anonymous-function, closures, currying, collection-pipeline, function, composition-over-inheritance]
sources:
- url: https://en.wikipedia.org/wiki/Function_composition_(computer_science)
  title: Function composition (computer science), Wikipedia
  locator: the introduction; Composing function calls; Naming the composition of functions; First-class composition
- url: https://en.wikipedia.org/wiki/Function_composition
  title: Function composition, Wikipedia
  locator: the introduction; Properties; Alternative notations
- url: https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/full-text/book/book-Z-H-12.html
  title: Structure and Interpretation of Computer Programs, second edition, Abelson and Sussman
  locator: 1.3.4 Procedures as Returned Values, Exercise 1.42
- url: https://www.haskell.org/onlinereport/haskell2010/haskellch9.html
  title: Haskell 2010 Language Report, Chapter 9, Standard Prelude
  locator: 9.1 The Prelude PreludeList, the definitions of id and (.)
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/function/Function.html
  title: Interface Function, Java SE 25 API
  locator: compose; andThen; identity
- url: https://docs.python.org/3/library/functools.html#functools.reduce
  title: functools, The Python Standard Library
  locator: functools.reduce
- url: https://github.com/tc39/proposal-pipeline-operator
  title: Pipe Operator for JavaScript, TC39 proposal
  locator: the README, Why a pipe operator
- url: https://www.typescriptlang.org/docs/handbook/2/functions.html#generic-functions
  title: More on Functions, The TypeScript Handbook
  locator: Generic Functions
- url: https://en.cppreference.com/w/cpp/ranges.html
  title: Ranges library, C++ reference, cppreference.com
  locator: the introduction; Example
- url: https://go.dev/ref/spec#Function_literals
  title: The Go Programming Language Specification
  locator: Function literals
- url: https://doc.rust-lang.org/book/ch20-04-advanced-functions-and-closures.html
  title: Advanced Functions and Closures, The Rust Programming Language
  locator: Returning Closures
- url: https://www.gnu.org/software/bash/manual/html_node/Pipelines.html
  title: Pipelines, Bash Reference Manual
  locator: 3.2.3 Pipelines
- url: https://learn.microsoft.com/en-us/dotnet/fsharp/language-reference/functions/
  title: Functions, F# Language Reference
  locator: Function composition; Pipelines
---

## Idea

Function composition makes one function from two functions. The new function gives its argument to the first function and gives that result to the second function. Wikipedia gives the definition: "function composition is an act or mechanism to combine simple functions to build more complex ones."[^1] In mathematics, "When the composite function f∘g (pronounced "f of g") is evaluated at an input x, the result is (f∘g)(x)=f(g(x))."[^2]

```python run id=idea
def compose(f, g):
    return lambda x: f(g(x))


def inc(x):
    return x + 1


def square(x):
    return x * x


print(compose(square, inc)(6))
print(compose(inc, square)(6))
```

```output for=idea
49
37
```

`compose(square, inc)` adds 1 and then squares: 7 times 7 is 49. This is the example of SICP Exercise 1.42, where `((compose square inc) 6)` gives 49[^3]. The other order squares and then adds 1, so the result is 37.

## How it works

1. **The definition.** SICP defines "The composition f after g" as the function that maps x to f(g(x))[^3]. The function on the right runs first: "the function f is applied after applying g to x."[^2]
2. **The types.** The result of the first function must be a valid argument of the second function. In the Haskell type `(.) :: (b -> c) -> (a -> b) -> a -> c`, "only the relation between f, g matters (f must accept what g returns)."[^1]
3. **A new function value.** `compose` returns a function, so the language must let a program return functions. Wikipedia says that such a function "can always be defined but languages with first-class functions make it easier."[^1] Haskell defines the operator in its Prelude as `f . g = \ x -> f (g x)`[^4].
4. **Associativity.** "The composition of functions is always associative"[^2]: f ∘ (g ∘ h) = (f ∘ g) ∘ h[^2]. So a chain of functions needs no parentheses. The identity function does nothing in a chain. Java `Function.identity` "Returns a function that always returns its input argument."[^5]

## In languages

- **Python.** No compose function in the library. Wikipedia shows a `compose` built with `functools.reduce`[^1]. `reduce` "Apply function of two arguments cumulatively to the items of iterable, from left to right, so as to reduce the iterable to a single value."[^6]
- **JavaScript.** No compose function. "In JavaScript we can define it as a function which takes two functions f and g, and produces a function"[^1]. A proposal adds a pipe operator `|>`, and the README gives its stage as 2[^7].
- **TypeScript.** The same as JavaScript. Type parameters give the types of a generic `compose`: "In TypeScript, generics are used when we want to describe a correspondence between two values."[^8]
- **Java.** `java.util.function.Function` has `compose` and `andThen`. `compose` "Returns a composed function that first applies the before function to its input, and then applies this function to the result."[^5] `andThen` "Returns a composed function that first applies this function to its input, and then applies the after function to the result."[^5]
- **C#.** No compose operator. "In C# we can define it as an Extension method which takes Funcs f and g, and produces a new Func"[^1].
- **C.** "In languages such as C, the only way to create a new function is to define it in the program source, which means that functions can't be composed at run time."[^1] A program can still apply an array of function pointers in order[^1].
- **C++.** No compose function for plain functions. A lambda can capture two functions and call them. The ranges library composes views: "Adaptors can be composed into pipelines, so that their actions take place as the view is iterated."[^9]
- **Go.** A function that returns a function literal. "Function literals are closures: they may refer to variables declared in a surrounding function."[^10]
- **Rust.** A function that returns a closure. The return type uses `impl Fn`, as in `fn returns_closure() -> impl Fn(i32) -> i32`[^11].
- **Bash.** Bash functions are not values, but a pipeline joins commands: "The output of each command in the pipeline is connected via a pipe to the input of the next command."[^12]
- **Haskell and F#.** Haskell has the operator `.`[^4]. F# has `>>` and `<<`: "The composition operator >> takes two functions and returns a function; by contrast, the pipeline operator |> takes a value and a function and returns a value."[^13]

## Variations and related ideas

- **Right to left or left to right.** `f ∘ g` and Haskell `f . g` run `g` first[^2][^4]. F# `>>` and Java `andThen` run the left function first[^13][^5]. In F#, `addOne << timesTwo` applied to 2 gives 5, and `addOne >> timesTwo` gives 6[^13].
- **Pipelines.** A pipeline passes a value through a chain of functions. Wikipedia says that functions on a stream "are known as filters, and are instead connected in a pipeline, which is analogous to function composition and can execute concurrently."[^1] See [[collection-pipeline]].
- **Stack languages.** "In a stack-based language, functional composition is even more natural: it is performed by concatenation, and is usually the primary method of program design."[^1]
- **Currying.** Composition joins functions of one argument. Currying turns a function of several arguments into a chain of one-argument functions. See [[currying]].
- **Object composition.** Composition of objects is a different idea. See [[composition-over-inheritance]].

## Pitfalls

### The wrong order

`compose(f, g)` runs `g` first. A program that reads the arguments from left to right gets the steps in the wrong order:

```python wrong id=python-compose-order
def compose(f, g):
    return lambda x: f(g(x))


def inc(x):
    return x + 1


def double(x):
    return x * 2


inc_then_double = compose(inc, double)
print(inc_then_double(5))
```

```output for=python-compose-order
11
```

The name means to add 1 and then double, which gives 12. The function doubles and then adds 1, so it gives 11. Write `compose(double, inc)`, or use a left-to-right form such as Java `andThen` or F# `>>`[^5][^13]. Composition is "not commutative"[^2], so the order always matters.

### Too much nesting

A long chain of nested calls is hard to read. Wikipedia warns that "A nesting of too many functions may have the opposite effect, making the code less maintainable."[^1] Give names to the steps, or use a pipeline form that reads from left to right.

### Functions with more than one argument

`compose` gives one value to the next function. A function that needs two arguments does not fit in the chain. Fix one argument with a lambda, or curry the function. See [[currying]].

## Cost

A composed function of n functions makes n calls and one call to the wrapper for each composition. A composition at run time also makes a new function value, often a closure that holds the two functions[^10]. Haskell defines `.` with one lambda[^4], so the operator adds only that call.

## References
