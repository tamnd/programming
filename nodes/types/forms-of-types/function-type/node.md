---
tag: RWHYVY
kind: concept
slug: function-type
name: Function type
aliases: [arrow type, function signature type, callable type, delegate type, functional interface, Func, std::function, fn pointer type, exponential type]
area: types/forms-of-types
level: core
summary: >-
  A function type is the type of a function value: its parameter types and its result type, as in int -> int. It lets a program pass, return and store functions.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1940
  people: [Alonzo Church]
  note: Alonzo Church introduced the simply typed lambda calculus in 1940, with one type constructor that builds function types. The arrow notation for function types came into use by the 1970s.
see_also: [first-class-function, higher-order-functions, anonymous-function, closures, callback, function-pointer-and-delegate, subtyping, composite-type]
same_as:
  wikidata: Q5508778
  wikipedia: Function_type
sources:
- url: https://en.wikipedia.org/wiki/Function_type
  title: Function type, Wikipedia
  locator: Introduction; Programming languages
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Function types; Type identity; Comparison operators; Calls
- url: https://docs.python.org/3.14/library/typing.html
  title: typing, Python 3.14 documentation
  locator: Annotating callable objects
- url: https://www.typescriptlang.org/docs/handbook/2/functions.html
  title: More on Functions, The TypeScript Handbook
  locator: Function Type Expressions; Optional Parameters in Callbacks
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/function/package-summary.html
  title: Java SE 25 API, Package java.util.function
  locator: Package description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.func-2?view=net-10.0
  title: Func<T,TResult> Delegate, .NET API reference
  locator: Definition
- url: https://doc.rust-lang.org/reference/types/function-item.html
  title: Function item types, The Rust Reference
  locator: Introduction
- url: https://doc.rust-lang.org/reference/types/closure.html
  title: Closure types, The Rust Reference
  locator: Introduction
- url: https://en.cppreference.com/w/cpp/language/lambda.html
  title: Lambda expressions, C++ reference, cppreference.com
  locator: Closure type
- url: https://doc.rust-lang.org/reference/types/function-pointer.html
  title: Function pointer types, The Rust Reference
  locator: Introduction
- url: https://en.cppreference.com/w/cpp/utility/functional/function.html
  title: std::function, C++ reference, cppreference.com
  locator: Description
- url: https://en.cppreference.com/w/c/language/pointer.html
  title: Pointer declaration, C reference, cppreference.com
  locator: Pointers to functions
- url: https://en.wikipedia.org/wiki/Covariance_and_contravariance_(computer_science)
  title: Covariance and contravariance (computer science), Wikipedia
  locator: Function types
- url: https://www.typescriptlang.org/tsconfig/strictFunctionTypes.html
  title: TSConfig Option strictFunctionTypes, TypeScript documentation
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/map
  title: Array.prototype.map(), MDN Web Docs
  locator: Using parseInt() with map()
- url: https://en.wikipedia.org/wiki/Simply_typed_lambda_calculus
  title: Simply typed lambda calculus, Wikipedia
  locator: Introduction; Syntax
---

## Idea

A function can be a value: a program passes it to another function, returns it, or keeps it in a list. A value needs a type, and the type of a function says what it takes and what it gives back. The type of a function from an integer to an integer is written `int -> int` in many texts, `Callable[[int], int]` in Python, `func(int) int` in Go and `Func<int, int>` in C#. With function types, a compiler can check that a function passed as an argument has the right parameters and result.

```python run id=idea
from collections.abc import Callable


def twice(f: Callable[[int], int], x: int) -> int:
    return f(f(x))


def inc(n: int) -> int:
    return n + 1


def compose(f: Callable[[int], int], g: Callable[[int], int]) -> Callable[[int], int]:
    return lambda x: f(g(x))


print(twice(inc, 5))
print(twice(lambda n: n * 3, 2))

h = compose(inc, lambda n: n * 10)
print(h(4), callable(h), type(h).__name__)
print(inc.__annotations__)

ops = {"+": lambda a, b: a + b, "*": lambda a, b: a * b}
print([op(6, 7) for op in ops.values()])
```

```output for=idea
7
18
41 True function
{'n': <class 'int'>, 'return': <class 'int'>}
[13, 42]
```

`twice` takes a function of type `Callable[[int], int]` and calls it two times. `compose` takes two functions and returns a third one, so its result type is a function type too. A dictionary holds two functions of two parameters as values. The annotations record the parameter type and the result type of `inc`. Python does not check them when the program runs, but a type checker such as mypy does.

## How it works

**The type of a function value.** "In computer science and mathematical logic, a function type (or arrow type or exponential) is the type of a variable or parameter to which a function has or can be assigned, or an argument or result type of a higher-order function taking or returning a function"[^1]. "A function type depends on the type of the parameters and the result type of the function"[^1]. In Go, "A function type denotes the set of all functions with the same parameter and result types"[^2].

**The notations.** The type theory notation is `A -> B`, a function from `A` to `B`. Haskell uses the same arrow. In Python, `Callable[[int], str]` "signifies a function that takes a single parameter of type int and returns a str"[^3]. TypeScript writes a function type expression such as `(a: string) => void`, and these types "are syntactically similar to arrow functions"[^4]. Go writes `func(int) string`, Rust writes `fn(i32) -> String`, and C writes a pointer to a function as `int (*)(int)`. C++ has function types such as `int(int)` and the wrapper `std::function<int(int)>`.

**Structural and nominal function types.** In Go, Rust, TypeScript and Python, the parameter and result types decide the function type. Go says that "Two function types are identical if they have the same number of parameters and result values, corresponding parameter and result types are identical, and either both functions are variadic or neither is", and "Parameter and result names are not required to match"[^2]. Java has no arrow type. It uses interfaces with one method: "Functional interfaces provide target types for lambda expressions and method references. Each functional interface has a single abstract method"[^5]. Thus `Function<Integer, Integer>` and `IntUnaryOperator` are two types for the same lambda. C# uses delegate types in the same way: `Func<T, TResult>` "Encapsulates a method that has one parameter and returns a value of the type specified by the TResult parameter"[^6].

**One type for each function.** Rust and C++ give each function or closure a type of its own, so that the compiler knows the exact function at each call. In Rust, a function item "yields a zero-sized value of its function item type", so "no indirection is needed when the function is called"[^7]. "A closure expression produces a closure value with a unique, anonymous type that cannot be written out"[^8]. In C++, a lambda has a "unique unnamed non-union non-aggregate class type, known as closure type"[^9]. To keep different functions in one variable or one list, a program uses a common type: "A function pointer type, written using the fn keyword, refers to a function whose identity is not necessarily known at compile-time"[^10]. In C++, `std::function` "is a general-purpose polymorphic function wrapper"[^11]. In C, "A pointer to function can be initialized with an address of a function"[^12].

**Subtyping of function types.** A function that accepts any animal can stand in where a function that accepts cats is expected, but not the other way. "In other words, the → type constructor is contravariant in the parameter (input) type and covariant in the return (output) type"[^13]. TypeScript checks this rule only with a flag: with `strictFunctionTypes`, "this flag causes functions parameters to be checked more correctly"[^14]. See [subtyping](/subtyping/).

**Function types as values.** A language with function types can treat functions as [first-class functions](/first-class-function/): it can pass them to [higher-order functions](/higher-order-functions/), return them, and store them. A [closure](/closures/) is a function value that also holds the variables that it captured. Its function type shows only the parameters and the result, not the captured variables.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | functions are objects; `Callable[[int], str]` in annotations, which type checkers read |
| JavaScript | native | functions are objects, and `typeof` gives `"function"`; no parameter types |
| TypeScript | native | function type expressions such as `(a: string) => void`; call signatures |
| Java | native | functional interfaces such as `Function<T, R>`; a lambda takes the type that the context expects |
| Kotlin | native | function types such as `(Int) -> String`; lambdas and function references |
| C# | native | delegate types such as `Func<T, TResult>` and `Action<T>` |
| Go | native | function types such as `func(int) int`; a function value compares only with `nil` |
| Rust | native | `fn(i32) -> i32` pointers; a unique type for each function and closure; the traits `Fn`, `FnMut` and `FnOnce` |
| C | native | function types and pointers to functions such as `int (*)(int)` |
| C++ | native | function types and pointers; a unique type for each lambda; `std::function<int(int)>` |
| Bash | idiom | a function is a command, not a value; a script passes the name of a function as a string |
| PowerShell | native | script blocks such as `{ param($x) $x + 1 }` and .NET delegates such as `[Func[int,int]]` |
| SQL | absent | functions are not values, so a query cannot pass a function to a function |
| Regex | n/a | a pattern has no functions |
| HTML | n/a | HTML has no functions; event handler attributes hold JavaScript code |
| CSS | n/a | `calc()` and `rgb()` use functional notation, but they are not function values |

The general-purpose languages all have function values, so the verdict is native for them except Bash. They differ in whether a function type is structural, as `func(int) int` in Go, or nominal, as the functional interfaces of Java and the delegates of C#, and in whether each function has a type of its own, as in Rust and C++.

The example files show these differences. Python shows that annotations are not checked when the program runs, that an object with `__call__` is callable, and that lambdas in a loop read the loop variable late. JavaScript shows that any function fits any call, which gives `[1, NaN, NaN]` from `map(parseInt)`. Java shows `Function`, `IntUnaryOperator`, `BiFunction`, `Supplier`, `compose` and a method reference. C# shows `Func` and `Action`, a method group and the type of a lambda with `var`. Go shows a named function type, `strings.Map` with a function argument, and a comparison with `nil`. Rust shows that a function item has the size 0, a closure as a generic parameter, `Box<dyn Fn>` for a list of closures, and the conversion of a closure to a `fn` pointer. C uses an array of function pointers and `qsort`. C++ shows that two equal lambdas have different types, a list of `std::function` values and the exception of an empty `std::function`.

## Variations and related ideas

- **Curried function types.** `int -> int -> int` is a function that takes an `int` and returns a function from `int` to `int`. Haskell and ML define functions of many parameters in this way. See [currying](/currying/).
- **Function pointers and delegates.** A function pointer holds only the address of a function. A delegate or a function object can also hold an object or captured variables. See [function pointer and delegate](/function-pointer-and-delegate/) and [function object](/function-object/).
- **Effects in the type.** Java lists checked exceptions in a method signature, and Rust `FnOnce`, `FnMut` and `Fn` say whether a closure can run once, can change its captures, or only reads them.
- **Generic function types.** `<T>(x: T) => T` in TypeScript and `fn<T>(T) -> T` as a generic function in Rust are types of functions that work for each `T`. See [parametric polymorphism](/parametric-polymorphism/).
- **Composite types.** A function type is a [composite type](/composite-type/): a type constructor builds it from the parameter types and the result type.

## Pitfalls

- **A callback that gets more arguments than it expects.** In JavaScript, `['1', '2', '3'].map(parseInt)` gives `[1, NaN, NaN]`, because `map` "passes 3 arguments: the element, the index, and the array"[^15], and `parseInt` uses the second one as the radix. Wrap the function: `map((s) => parseInt(s, 10))`. TypeScript does not catch this, because "Functions with fewer parameters (of the same types) can always take the place of functions with more parameters"[^4].
- **Method syntax is not checked strictly.** `strictFunctionTypes` "only applies to functions written in function syntax, not to those in method syntax"[^14]. A property declared as `f(x: string | number): void` accepts a function that takes only a `string`, and a call with a number then fails.
- **An empty function value.** In Go, "The value of an uninitialized variable of function type is nil"[^2], and "Calling a nil function value causes a run-time panic"[^2]. In C++, "Invoking the target of an empty std::function results in std::bad_function_call exception being thrown"[^11]. Check for an empty value before the call, or give the variable a default function.
- **Two function types with the same signature.** In Java, a `Function<Integer, Integer>` is not an `IntUnaryOperator`, and in C#, a `Func<int, int>` is not a delegate type of your own with the same signature. Convert with a method reference or a lambda, such as `inc::apply`.
- **A list of closures.** In Rust and C++, two closures have two types, so they cannot share a list without a common type such as `Box<dyn Fn(i32) -> i32>` or `std::function<int(int)>`.

## Cost

A call to a known function is a direct call, and the compiler can inline it. A call through a function value is an indirect call: the program loads an address and jumps to it, and the compiler usually cannot inline it. Rust and C++ give each closure its own type, so a generic function that takes the closure type gets a copy for each closure and can inline the call. A common type such as `std::function` or `Box<dyn Fn>` stores the closure behind a pointer and adds an indirect call. In Java and C#, a lambda is an object, and a call goes through the method of the interface or the delegate.

## History

Function types come from logic. "The simply typed lambda calculus was originally introduced by Alonzo Church in 1940 as an attempt to avoid paradoxical use of the untyped lambda calculus"[^16]. It has "only one type constructor" and that constructor "builds function types"[^16]. Church wrote types as subscripts, and "By the 1970s stand-alone arrow notation was in use"[^16]. The rule that function types are contravariant in the parameter type "was first stated formally by John C. Reynolds"[^13], and a paper by Luca Cardelli made it better known[^13].

## References
