---
tag: BJQVD1
kind: concept
slug: combinator
name: Combinator
aliases: [combinator function, combinator pattern, combinator library, SKI combinators, fixed-point combinator, Y combinator]
area: functions-and-abstraction/functions-as-values
level: advanced
summary: >-
  A combinator is a function that uses only its arguments, such as identity, const and flip. A combinator library gives simple values and functions that join them into larger values of the same type.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1924
  people: [Moses Schönfinkel, Haskell Curry]
  note: Schönfinkel introduced combinators in 1920 and published his paper in 1924. Curry found them again in 1927. Later, David Turner used combinators to implement the language SASL.
see_also: [higher-order-functions, first-class-function, function-composition, anonymous-function, closures, currying, function-object, map-filter-reduce]
sources:
- url: https://wiki.haskell.org/Combinator
  title: Combinator, HaskellWiki
  locator: the whole page
- url: https://en.wikipedia.org/wiki/Combinatory_logic
  title: Combinatory logic, Wikipedia
  locator: the introduction; In mathematics; Examples of combinators; Compilation of functional languages
- url: https://hackage.haskell.org/package/base/docs/Prelude.html
  title: Prelude, the Haskell package base
  locator: Miscellaneous functions, id, const and flip
- url: https://en.wikipedia.org/wiki/Parser_combinator
  title: Parser combinator, Wikipedia
  locator: the introduction; Basic idea
- url: https://docs.python.org/3/library/functools.html#functools.partial
  title: functools, The Python Standard Library
  locator: functools.partial
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/function/Function.html
  title: Interface Function, Java SE 25 API
  locator: identity
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/function/Predicate.html
  title: Interface Predicate, Java SE 25 API
  locator: and; not
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/lambda-expressions
  title: Lambda expressions and anonymous functions, C# language reference
  locator: the introduction
- url: https://en.cppreference.com/w/cpp/utility/functional/identity.html
  title: std::identity, C++ reference, cppreference.com
  locator: the description; Notes
- url: https://en.cppreference.com/w/cpp/utility/functional/not_fn.html
  title: std::not_fn, C++ reference, cppreference.com
  locator: the description
- url: https://en.cppreference.com/w/cpp/utility/functional/bind_front.html
  title: std::bind_front, std::bind_back, C++ reference, cppreference.com
  locator: the description
- url: https://doc.rust-lang.org/std/convert/fn.identity.html
  title: Function identity, The Rust Standard Library
  locator: the description
- url: https://doc.rust-lang.org/rust-by-example/error/option_unwrap/map.html
  title: Combinators, map, Rust By Example
  locator: the whole page
- url: https://go.dev/ref/spec#Function_literals
  title: The Go Programming Language Specification
  locator: Function literals
- url: https://en.cppreference.com/w/c/language/functions.html
  title: Functions, C reference, cppreference.com
  locator: the introduction
- url: https://www.gnu.org/software/bash/manual/html_node/Shell-Functions.html
  title: Shell Functions, Bash Reference Manual
  locator: 3.3 Shell Functions
- url: https://en.wikipedia.org/wiki/SKI_combinator_calculus
  title: SKI combinator calculus, Wikipedia
  locator: the introduction
- url: https://en.wikipedia.org/wiki/Fixed-point_combinator
  title: Fixed-point combinator, Wikipedia
  locator: the introduction; Example implementations; Strict fixed-point combinator
- url: https://en.wikipedia.org/wiki/Tacit_programming
  title: Tacit programming, Wikipedia
  locator: the introduction
---

## Idea

The word combinator has two meanings in programming. The HaskellWiki gives "two distinct meanings" of the word[^1].

- **The narrow meaning.** A combinator is "A function or definition with no free variables."[^1] It uses only its arguments. Identity, const and flip are examples.
- **The wide meaning.** The combinator pattern is "a style of organizing libraries centered around the idea of combining things."[^1] A combinator takes values of a type and joins them into a new value of the same type.

```python run id=idea
def identity(x):
    return x


def const(x):
    return lambda _: x


def flip(f):
    return lambda a, b: f(b, a)


def both(p, q):
    return lambda x: p(x) and q(x)


def sub(a, b):
    return a - b


def is_even(x):
    return x % 2 == 0


def is_positive(x):
    return x > 0


print(identity(5))
print(const(7)(99))
print(flip(sub)(2, 10))
print([x for x in range(-2, 5) if both(is_even, is_positive)(x)])
```

```output for=idea
5
7
8
[2, 4]
```

`identity`, `const` and `flip` use only their arguments. `both` takes two predicates and returns a new predicate. The result is a predicate too, so a program can give it to `both` again.

## How it works

1. **Only the arguments.** A combinator in the narrow sense refers to no variable outside itself[^1]. Wikipedia gives the definition: "A combinator is a higher-order function that uses only function application and earlier defined combinators to define a result from its arguments."[^2]
2. **Functions in, functions out.** Many combinators take functions and return a new function. `flip` returns a function that calls `f` with its two arguments in the other order. Haskell says that "flip f takes its (first) two arguments in the reverse order of f."[^3]
3. **One type for the parts and the whole.** A combinator library has a type, functions that make simple values of the type, and combinators that join values into more complex values of the same type[^1]. A combinator can take the result of another combinator, so small parts build large ones.
4. **An example library.** "In computer programming, a parser combinator is a higher-order function that accepts several parsers as input and returns a new parser as its output."[^4]

## In languages

- **Python.** Functions are values, so a function can return a lambda. `functools.partial` is a combinator in the library. It returns "a new partial object which when called will behave like func called with the positional arguments args and keyword arguments keywords."[^5]
- **JavaScript.** Arrow functions make combinators short, as in `const flip = (f) => (a, b) => f(b, a)`.
- **TypeScript.** The same as JavaScript. Generic types describe combinators such as `flip<A, B, C>(f: (a: A, b: B) => C)`.
- **Java.** `Function.identity()` "Returns a function that always returns its input argument."[^6] `Predicate.and` "Returns a composed predicate that represents a short-circuiting logical AND of this predicate and another."[^7] `Predicate.not` "Returns a predicate that is the negation of the supplied predicate."[^7]
- **C#.** A combinator is a method that takes and returns delegates. A lambda "that has one parameter and returns a value" converts to a `Func<T,TResult>` delegate[^8].
- **C++.** "std::identity is a function object type whose operator() returns its argument unchanged."[^9] `std::not_fn` "Creates a forwarding call wrapper that returns the negation of the callable object it holds."[^10] `std::bind_front` binds the first arguments of a callable object[^11].
- **Rust.** `std::convert::identity` is "The identity function."[^12] Rust calls methods such as `Option::map` combinators: "Option has a built in method called map(), a combinator for the simple mapping of Some -> Some and None -> None."[^13]
- **Go.** "Function literals are closures"[^14], so a generic function can return a new function value.
- **C.** C has function pointers but no closures. "A function is a C language construct that associates a compound statement (the function body) with an identifier (the function name)."[^15] A C function cannot make a new function at run time, so a helper takes the function pointer and the arguments together.
- **Bash.** Functions are not values, but a function can get the name of another function as an argument. "When the name of a shell function is used as a simple command name, the shell executes the list of commands associated with that function name."[^16]

## Variations and related ideas

- **SKI combinators.** Combinatory logic uses a few basic combinators. "The simplest example of a combinator is I, the identity combinator, defined by (I x)=x for all terms x."[^2] "Another simple combinator is K, which manufactures constant functions"[^2]. "A third combinator is S, which is a generalized version of application"[^2]. The SKI calculus is "an extremely simple Turing complete language."[^17]
- **Haskell Prelude.** The examples of the HaskellWiki "are id, const, and flip respectively."[^1] For `id`, the Prelude says: "This function might seem useless at first glance, but it can be very useful in a higher order context."[^3]
- **Fixed-point combinators.** Wikipedia describes "a fixed-point combinator (or fixpoint combinator)"[^18] as a function that "is a higher-order function (i.e., a function that takes a function as argument) that returns some fixed point (a value that is mapped to itself) of its argument function, if one exists."[^18] "In programming languages that support anonymous functions, fixed-point combinators allow the definition and use of anonymous recursive functions"[^18]. The best known one is the Y combinator of Curry[^18]. "However, they are rarely used in practical programming."[^18]
- **Combinators in libraries.** Parser combinators[^4], the `Option` methods of Rust[^13] and the `Predicate` methods of Java[^7] are combinator libraries.
- **Point-free style.** A function written only with combinators and composition does not name its arguments. "Tacit programming, also called point-free style, is a programming paradigm in which function definitions do not identify the arguments"[^19] on which they operate. See [[function-composition]].

## Pitfalls

### The Y combinator loops in a strict language

The Y combinator needs lazy evaluation. Wikipedia says: "Languages that use strict evaluation, such as Python, C++, and other strict programming languages, can often express Y; however, any implementation is useless in practice since it loops indefinitely until terminating via a stack overflow."[^18]

```python wrong id=python-y-combinator
Y = lambda f: (lambda x: f(x(x)))(lambda x: f(x(x)))
fact_step = lambda self: lambda n: 1 if n == 0 else n * self(n - 1)

try:
    fact = Y(fact_step)
    print(fact(5))
except RecursionError as e:
    print(type(e).__name__)
```

```output for=python-y-combinator
RecursionError
```

Python computes `x(x)` before it calls `f`, and that call computes `x(x)` again. Use the Z combinator. "The Z combinator will work in strict languages"[^18]. It puts `x(x)` inside a lambda, so Python computes it only when the function gets an argument:

```python run id=python-z-combinator
Z = lambda f: (lambda x: f(lambda v: x(x)(v)))(lambda x: f(lambda v: x(x)(v)))
fact_step = lambda self: lambda n: 1 if n == 0 else n * self(n - 1)

print(Z(fact_step)(5))
```

```output for=python-z-combinator
120
```

### Too many combinators hide the meaning

A long chain of combinators names no arguments, and a reader must work out what each step gets. "The lack of argument naming gives point-free style a reputation of being needlessly obscure"[^19]. Give names to the parts, or write a small function with named parameters.

## Cost

A combinator returns a new function, so each call of the result also calls the functions inside it. A chain of combinators adds one call for each step. Some combinators save work. `Predicate.and` is short-circuiting: "if this predicate is false, then the other predicate is not evaluated."[^7]

## History

Combinatory logic "was introduced by Moses Schönfinkel"[^2] and Haskell Curry. "It is based on combinators, which were introduced by Schönfinkel in 1920"[^2]. Schönfinkel published nothing more on combinators "after his original 1924 paper."[^2] "Haskell Curry rediscovered the combinators while working as an instructor at Princeton University in late 1927."[^2] Later, "David Turner used his combinators to implement the SASL programming language."[^2]

## References
