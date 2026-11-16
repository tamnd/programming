---
tag: AX3AFA
kind: task
slug: write-a-higher-order-function
name: Write a higher-order function
aliases: [function that takes a function, function that returns a function, function factory, apply a function n times, repeat a function]
area: functions-and-abstraction/functions-as-values
level: core
summary: >-
  Write repeat(f, times), which takes a function and returns a new function that applies it times times, and use it to run the step of the Collatz problem a given number of times.
contract: contract.yaml
category: functions
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://en.wikipedia.org/wiki/Collatz_conjecture
  title: Collatz conjecture, Wikipedia
  locator: Statement of the problem; Examples
- url: https://en.wikipedia.org/wiki/Higher-order_function
  title: Higher-order function, Wikipedia
  locator: the introduction
- url: https://docs.python.org/3/faq/programming.html#how-do-you-make-a-higher-order-function-in-python
  title: Programming FAQ, Python documentation
  locator: How do you make a higher order function in Python?
- url: https://docs.python.org/3/tutorial/controlflow.html#lambda-expressions
  title: More Control Flow Tools, The Python Tutorial
  locator: 4.9.7 Lambda Expressions
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Closures
  title: Closures, MDN Web Docs JavaScript Guide
  locator: Closure; Creating closures in loops, A common mistake
- url: https://www.typescriptlang.org/docs/handbook/2/functions.html
  title: More on Functions, The TypeScript Handbook
  locator: Function Type Expressions
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/function/Function.html
  title: Interface Function, Java SE 25 API
  locator: the introduction
- url: https://docs.oracle.com/javase/tutorial/java/javaOO/lambdaexpressions.html
  title: Lambda Expressions, The Java Tutorials
  locator: the introduction; Accessing Local Variables of the Enclosing Scope
- url: https://learn.microsoft.com/en-us/dotnet/api/system.func-2
  title: Func<T,TResult> Delegate, .NET API
  locator: the introduction
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/lambda-expressions
  title: Lambda expressions and anonymous functions, C# language reference
  locator: the introduction
- url: https://en.cppreference.com/w/c/language/pointer.html
  title: Pointer declaration, C reference, cppreference.com
  locator: Pointers to functions
- url: https://en.cppreference.com/w/c/language/functions.html
  title: Functions, C reference, cppreference.com
  locator: the introduction
- url: https://en.cppreference.com/w/cpp/utility/functional/function.html
  title: std::function, C++ reference, cppreference.com
  locator: the introduction
- url: https://en.cppreference.com/w/cpp/language/lambda.html
  title: Lambda expressions, C++ reference, cppreference.com
  locator: the introduction; Lambda capture
- url: https://go.dev/ref/spec#Function_literals
  title: The Go Programming Language Specification
  locator: Function types; Function literals
- url: https://doc.rust-lang.org/book/ch20-04-advanced-functions-and-closures.html
  title: Advanced Functions and Closures, The Rust Programming Language
  locator: Returning Closures
- url: https://doc.rust-lang.org/book/ch13-01-closures.html
  title: Closures, The Rust Programming Language
  locator: Capturing References or Moving Ownership
- url: https://www.gnu.org/software/bash/manual/html_node/Shell-Functions.html
  title: Shell Functions, Bash Reference Manual
  locator: 3.3 Shell Functions
---

## Task

Write a higher-order function `repeat(f, times)`. It takes a function `f` of one integer and a count `times`, and it returns a new function. The new function applies `f` to its argument `times` times. So `repeat(f, 3)(x)` is `f(f(f(x)))`, and `repeat(f, 0)(x)` is `x`.

Then use `repeat` with the step of the Collatz problem: "If the number is even, divide it by two."[^1] "If the number is odd, triple it and add one."[^1] The entry function `collatz_after(start, times)` returns `repeat(collatz_step, times)(start)`. For example, `collatz_after(6, 3)` is 5, because the steps go from 6 to 3, 10 and 5.

`start` is from 1 to 10000 and `times` is from 0 to 1000. With these limits each value on the way fits in a 32-bit signed integer. The case `peak-9663` has the largest value, 27114424.

A higher-order function is "a function that does at least one of the following: takes one or more functions as arguments (i.e. a procedural parameter, which is a parameter of a procedure that is itself a procedure), returns a function as its result."[^2] `repeat` does both. The function that it returns uses `f` and `times` after `repeat` has returned, so in most languages it is a closure. [[higher-order-functions]] explains the idea, [[first-class-function]] describes the values that make it possible, and [[closures]] explains how the returned function keeps `f` and `times`.

## Solutions

| Language | The type of `f` | What `repeat` returns |
|---|---|---|
| Python | `Callable[[int], int]` | a nested function |
| JavaScript | a function value | an arrow function |
| TypeScript | `(x: number) => number` | an arrow function |
| Java | `Function<Integer, Integer>` | a lambda expression |
| C# | `Func<int, int>` | a lambda expression |
| C | `int (*)(int)`, a pointer to a function | a struct with the pointer and the count |
| C++ | `std::function<int(int)>` | a lambda expression in a `std::function` |
| Go | `func(int) int` | a function literal |
| Rust | `impl Fn(i32) -> i32` | a `move` closure, as `impl Fn(i32) -> i32` |
| Bash | the name of a function, as a string | no function; `repeat` applies the function at once |

- **Python.** The Python FAQ gives two ways to make a higher-order function: "You have two choices: you can use nested scopes or you can use callable objects."[^3] The solution uses a nested function. `collatz_step` is passed by its name, with no parentheses. A lambda expression also gives a function value, and the tutorial shows a function that returns one: "The above example uses a lambda expression to return a function."[^4]
- **JavaScript.** The returned arrow function keeps `f` and `times`. MDN defines the mechanism: "A closure is the combination of a function bundled together (enclosed) with references to its surrounding state (the lexical environment)."[^5] MDN calls a function that makes new functions a function factory: "In essence, makeAdder is a function factory."[^5]
- **TypeScript.** The parameter and the result have a function type: "The simplest way to describe a function is with a function type expression."[^6] The solution names the type `Step = (x: number) => number`.
- **Java.** A lambda expression has the type of a functional interface. The solution uses `Function<Integer, Integer>`, which "Represents a function that accepts one argument and produces a result."[^7] Lambda expressions "treat functionality as method argument, or code as data."[^8] The returned lambda reads `f` and `times`. That is allowed, because "a lambda expression can only access local variables and parameters of the enclosing block that are final or effectively final."[^8] The two parameters do not change.
- **C#.** `Func<int, int>` "Encapsulates a method that has one parameter and returns a value of the type specified by the TResult parameter."[^9] A lambda expression converts to it: "Convert a lambda expression that has one parameter and returns a value to a Func<T,TResult> delegate."[^10]
- **C.** C can pass a function: "A pointer to function can be used on the left-hand side of the function call operator; this invokes the pointed-to function"[^11]. C cannot make a new function at run time. "There are no nested functions (except where allowed through non-standard compiler extensions): each function definition must appear at file scope, and functions have no access to the local variables from the caller"[^12]. So `repeat` returns a `struct repeated` with the pointer and the count, and `call_repeated` applies it. The struct does the work of a closure by hand.
- **C++.** "Class template std::function is a general-purpose polymorphic function wrapper."[^13] It can hold a function pointer or a lambda. A lambda expression "Constructs a closure (an unnamed function object capable of capturing variables in scope)."[^14] The returned lambda captures `f` and `times` by copy, so it is safe to call after `repeat` returns.
- **Go.** "A function type denotes the set of all functions with the same parameter and result types."[^15] `Repeat` takes and returns a `func(int) int`. "Function literals are closures: they may refer to variables declared in a surrounding function."[^15]
- **Rust.** A closure has a type that the program cannot write. "Closures are represented by traits, which means you can’t return closures directly."[^16] The solution returns `impl Fn(i32) -> i32`. The closure must own `f` and `times`, because they go out of scope when `repeat` returns: "If you want to force the closure to take ownership of the values it uses in the environment even though the body of the closure doesn’t strictly need ownership, you can use the move keyword before the parameter list."[^17]
- **Bash.** A Bash function is not a value. "Shell functions are a way to group commands for later execution using a single name for the group."[^18] The solution passes that name as a string, and `"$f" "$x"` calls it, because the shell runs a function in the same way as a simple command[^18]. Bash cannot return a new function, so `repeat` takes the start value too and prints the result.

### A lambda and a named function

`repeat` takes any function of one integer. Here it gets a lambda expression and the named function `collatz_step`:

```python run id=python-lambda-and-named
from collections.abc import Callable


def repeat(f: Callable[[int], int], times: int) -> Callable[[int], int]:
    def repeated(x: int) -> int:
        for _ in range(times):
            x = f(x)
        return x

    return repeated


def collatz_step(n: int) -> int:
    return n // 2 if n % 2 == 0 else 3 * n + 1


double_ten_times = repeat(lambda x: x * 2, 10)
print(double_ten_times(1))
print(repeat(collatz_step, 77)(27))
print(repeat(collatz_step, 0)(27))
```

```output for=python-lambda-and-named
1024
9232
27
```

From 27, the Collatz sequence climbs "as high as 9232 before descending to 1."[^1]

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `zero-times`: `repeat(f, 0)` is the identity function, so the result is the start.
- `start-one`: from 1, one step gives 4.
- `cycle` and `past-one`: from 1 the steps go to 4, 2 and 1 again. A start of 16 reaches 1 after 4 steps, and after 10 steps it is at 1 again.
- `peak-27`: 77 steps from 27 give 9232, the highest value on that path[^1].
- `peak-9663`: 48 steps from 9663 give 27114424, the largest value for any start up to 10000. It fits in 32 bits.
- `limit`: 1000 steps from 10000, the largest start and the largest count.

## Pitfalls

### The result of a call in place of the function

`collatz_step` is the function. `collatz_step(6)` is a call, and its result is the number 3. When `repeat` gets the number, the error comes only when the returned function runs:

```python wrong id=python-call-not-function
from collections.abc import Callable


def repeat(f: Callable[[int], int], times: int) -> Callable[[int], int]:
    def repeated(x: int) -> int:
        for _ in range(times):
            x = f(x)
        return x

    return repeated


def collatz_step(n: int) -> int:
    return n // 2 if n % 2 == 0 else 3 * n + 1


three_steps = repeat(collatz_step(6), 3)
try:
    print(three_steps(6))
except TypeError as error:
    print(error)
```

```output for=python-call-not-function
'int' object is not callable
```

Pass `collatz_step` with no parentheses.

### Functions made in a loop with var

Each function that a loop makes keeps a reference to its variables, not a copy of their values. "Prior to the introduction of the let keyword, a common problem with closures occurred when you created them inside a loop."[^5]

```javascript wrong id=javascript-var-in-loop
const adders = [];
for (var i = 1; i <= 3; i++) {
  adders.push((x) => x + i);
}
console.log(adders.map((add) => add(10)));
```

```output for=javascript-var-in-loop
[ 14, 14, 14 ]
```

The three functions share one `i`, and the loop leaves it at 4. With `let i`, each pass of the loop has its own `i`, and the result is `[ 11, 12, 13 ]`. The same problem is in Python, where the FAQ gives the fix `lambda n=x: n**2`[^3].

## Related tasks

- [[compose-functions]]: a higher-order function that takes several functions and returns their composition.
- [[map-over-a-list]]: pass a function to `map`.
- [[reduce-a-list-to-a-value]]: pass a function to `reduce`.
- [[define-a-function]]: define a first-order function.

## References
