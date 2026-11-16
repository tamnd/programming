---
tag: FAS41G
kind: task
slug: compose-functions
name: Compose functions
aliases: [function composition, compose helper, combine functions, chain functions, compose many functions]
area: functions-and-abstraction/functions-as-values
level: core
summary: >-
  Write a generic compose helper that takes any number of functions and returns one function, where the last function runs first, and use it to compose functions that are chosen by name.
contract: contract.yaml
category: functions
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://en.wikipedia.org/wiki/Function_composition_(computer_science)
  title: Function composition (computer science), Wikipedia
  locator: the introduction; First-class composition
- url: https://docs.python.org/3/library/functools.html#functools.reduce
  title: functools, The Python Standard Library
  locator: functools.reduce
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/reduceRight
  title: Array.prototype.reduceRight(), MDN Web Docs JavaScript Reference
  locator: the introduction
- url: https://www.typescriptlang.org/docs/handbook/2/functions.html#generic-functions
  title: More on Functions, The TypeScript Handbook
  locator: Generic Functions
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/function/Function.html
  title: Interface Function, Java SE 25 API
  locator: andThen; compose; identity
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.aggregate
  title: Enumerable.Aggregate Method, .NET API
  locator: Overloads
- url: https://en.cppreference.com/w/c/language/pointer.html
  title: Pointer declaration, C reference, cppreference.com
  locator: Pointers to functions
- url: https://en.cppreference.com/w/c/language/functions.html
  title: Functions, C reference, cppreference.com
  locator: the introduction
- url: https://en.cppreference.com/w/cpp/utility/functional/function.html
  title: std::function, C++ reference, cppreference.com
  locator: the introduction
- url: https://go.dev/ref/spec#Type_parameter_declarations
  title: The Go Programming Language Specification
  locator: Type parameter declarations; Function literals
- url: https://doc.rust-lang.org/book/ch20-04-advanced-functions-and-closures.html
  title: Advanced Functions and Closures, The Rust Programming Language
  locator: Returning Closures, Listing 20-34
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html
  title: Trait Iterator, The Rust Standard Library
  locator: rev; fold
- url: https://www.gnu.org/software/bash/manual/html_node/Shell-Functions.html
  title: Shell Functions, Bash Reference Manual
  locator: 3.3 Shell Functions
- url: https://docs.python.org/3/faq/programming.html#why-do-lambdas-defined-in-a-loop-with-different-values-all-return-the-same-result
  title: Programming FAQ, Python documentation
  locator: Why do lambdas defined in a loop with different values all return the same result?
---

## Task

Write a generic helper `compose(f1, f2, ..., fn)`. It takes any number of functions from a type to the same type, and it returns one function. The returned function applies the functions from the last to the first, as in mathematics: `compose(f, g)(x)` is `f(g(x))`. With no functions, `compose()` is the identity function.

Then use the helper with four functions on integers, chosen by name:

| Name | The function |
|---|---|
| `inc` | `x + 1` |
| `dec` | `x - 1` |
| `double` | `2 * x` |
| `negate` | `-x` |

The entry function `compose_all(ops, x)` composes the functions that `ops` names, in the order of `ops`, and applies the result to `x`. So `compose_all(["inc", "double"], 3)` is `inc(double(3))`, which is 7, and `compose_all(["double", "inc"], 3)` is 8. `ops` has 0 to 20 names and `x` is from -1000 to 1000, so each result fits in a 32-bit signed integer.

"In computer science, function composition is an act or mechanism to combine simple functions to build more complex ones."[^1] The two-function form is the subject of [[function-composition]]. A helper that takes a list of functions is a fold over the list: it starts from the identity function and composes one function at a time. [[map-filter-reduce]] explains the fold, and [[write-a-higher-order-function]] is the task for one function that takes and returns a function.

## Solutions

| Language | The compose helper | How it combines the functions |
|---|---|---|
| Python | `compose(*functions)` | `functools.reduce` with the identity function as the start |
| JavaScript | `compose(...fs)` | `reduceRight` over the functions |
| TypeScript | `compose<T>(...fs: Array<(x: T) => T>)` | `reduceRight` over the functions |
| Java | `<T> Function<T, T> compose(List<Function<T, T>> fs)` | `Stream.reduce` from `Function.identity()` with `Function.compose` |
| C# | `Func<T, T> Compose<T>(params Func<T, T>[] fs)` | `Aggregate` from the identity lambda |
| C | `apply_composed(fs, n, x)` | a loop from the last pointer to the first |
| C++ | `template <typename T> std::function<T(T)> compose(std::vector<std::function<T(T)>> fs)` | a lambda with a reverse loop |
| Go | `func Compose[T any](fs ...func(T) T) func(T) T` | a function literal with a reverse loop |
| Rust | `fn compose<T>(fs: Vec<Box<dyn Fn(T) -> T>>) -> impl Fn(T) -> T` | `iter().rev().fold(...)` in a `move` closure |
| Bash | `compose_all X NAME...` | a loop from the last name to the first |

- **Python.** `functools.reduce` does the fold. It applies a "function of two arguments cumulatively to the items of iterable, from left to right, so as to reduce the iterable to a single value."[^2] The function of two arguments is `lambda f, g: lambda x: f(g(x))`, the same as in the Wikipedia example[^1]. The solution gives the identity function as the third argument, because "If the optional initial is present, it is placed before the items of the iterable in the calculation, and serves as a default when the iterable is empty."[^2]
- **JavaScript.** "The reduceRight() method of Array instances applies a function against an accumulator and each value of the array (from right-to-left) to reduce it to a single value."[^3] The accumulator is the value, not a function, so the helper starts with `x` and applies the last function first. Wikipedia gives the same one-line helper[^1].
- **TypeScript.** The helper has a type parameter `T`, because "It’s common to write a function where the types of the input relate to the type of the output"[^4]. Each function takes a `T` and gives a `T`.
- **Java.** `Function` has two methods that compose. `compose` "Returns a composed function that first applies the before function to its input, and then applies this function to the result."[^5] `andThen` "Returns a composed function that first applies this function to its input, and then applies the after function to the result."[^5] The helper uses `compose`, and it starts from `Function.identity()`, which "Returns a function that always returns its input argument."[^5]
- **C#.** `Aggregate` "Applies an accumulator function over a sequence. The specified seed value is used as the initial accumulator value."[^6] The seed is the identity lambda `x => x`, and the accumulator function is `(f, g) => x => f(g(x))`.
- **C.** "A pointer to function can be used on the left-hand side of the function call operator; this invokes the pointed-to function"[^7], so an array of pointers can hold the functions. C cannot make a new function at run time, because "There are no nested functions"[^8]. So the helper `apply_composed` takes the array and the value, and it calls the functions from the last to the first.
- **C++.** "Class template std::function is a general-purpose polymorphic function wrapper."[^9] The helper is a template that takes a vector of `std::function<T(T)>` and returns a lambda that owns the vector.
- **Go.** "A type parameter list declares the type parameters of a generic function, method, or type declaration."[^10] `Compose[T any]` takes a variadic list of `func(T) T`. "Function literals are closures"[^10], so the returned function keeps the list.
- **Rust.** Each closure has its own type, so a list of different closures needs a trait object. The Rust book uses "a Vec<T> of closures defined by functions that return Box<dyn Fn> so that they have the same type"[^11]. The returned closure calls `rev`, which "Reverses an iterator’s direction."[^12], and `fold`, which "Folds every element into an accumulator by applying an operation, returning the final result."[^12]
- **Bash.** A function is a name for a group of commands[^13], not a value. `compose_all` takes the names, puts them in an array, and calls them from the last to the first. Each call prints its result, and the loop reads it with `$( )`.

### Compose and andThen

In Java, `f.compose(g)` runs `g` first, and `f.andThen(g)` runs `f` first[^5]:

```java run id=java-compose-and-then
import java.util.function.Function;

public class Main {
    public static void main(String[] args) {
        Function<Integer, Integer> inc = x -> x + 1;
        Function<Integer, Integer> twice = x -> 2 * x;
        System.out.println(inc.compose(twice).apply(3));
        System.out.println(inc.andThen(twice).apply(3));
    }
}
```

```output for=java-compose-and-then
7
8
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: no names give the identity function, so the result is `x`.
- `single-zero`: one function. `negate` of 0 is 0.
- `inc-after-double` and `double-after-inc`: the same two names in the two orders give 7 and 8.
- `negative` and `low`: negative values. `dec` then `negate` on -5 gives 4.
- `duplicates`: the same name three times applies the function three times.
- `limit-high` and `limit-low`: 20 times `double` on 1000 and on -1000 give 1048576000 and -1048576000, which fit in 32 bits.
- `mixed-20` and `mixed-20-swapped`: 20 names that alternate, in the two orders.

## Pitfalls

### reduce with no start value

Wikipedia's Python example calls `reduce` with no initial value[^1]. Then `compose()` with no functions fails, because there is no item to start from:

```python wrong id=python-reduce-no-initial
from functools import reduce


def compose(*functions):
    return reduce(lambda f, g: lambda x: f(g(x)), functions)


def inc(x):
    return x + 1


def double(x):
    return 2 * x


print(compose(inc, double)(3))
try:
    print(compose()(3))
except TypeError as error:
    print(error)
```

```output for=python-reduce-no-initial
7
reduce() of empty iterable with no initial value
```

Give the identity function as the initial value, as the solution does. Then the empty case is the identity[^2].

### A loop that reads the variable at call time

A loop can build the composition one function at a time. But a lambda reads a variable from the outer scope when it is called, not when it is made. The Python FAQ says that the variable "is accessed when the lambda is called — not when it is defined."[^14] Here `g` names the new lambda when it is called, so the lambda calls itself:

```python wrong id=python-loop-late-binding
def compose(*functions):
    g = lambda x: x
    for f in functions:
        g = lambda x: g(f(x))
    return g


def inc(x):
    return x + 1


def double(x):
    return 2 * x


try:
    print(compose(inc, double)(3))
except RecursionError as error:
    print(type(error).__name__, error)
```

```output for=python-loop-late-binding
RecursionError maximum recursion depth exceeded
```

Bind the current values as default arguments, `lambda x, g=g, f=f: g(f(x))`, as the FAQ shows for its loop[^14], or use `reduce`.

### reduce in place of reduceRight

`reduceRight` goes from right to left[^3]. `reduce` goes from left to right, so with `reduce` the first function runs first:

```javascript wrong id=javascript-reduce-order
const compose = (...fs) => (x) => fs.reduce((acc, f) => f(acc), x);

const inc = (x) => x + 1;
const double = (x) => 2 * x;

console.log(compose(inc, double)(3));
```

```output for=javascript-reduce-order
8
```

`inc(double(3))` is 7. Use `reduceRight`, or call the helper `pipe` and say in its name that the first function runs first.

## Related tasks

- [[write-a-higher-order-function]]: one function that takes a function and returns a new function.
- [[reduce-a-list-to-a-value]]: the fold that the helper uses.
- [[map-over-a-list]]: pass one function to `map`.

## References
