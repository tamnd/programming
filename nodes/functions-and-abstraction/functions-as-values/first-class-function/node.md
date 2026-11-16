---
tag: JW0TZE
kind: concept
slug: first-class-function
name: First-class function
aliases: [first-class functions, functions as values, function value, first-class procedure]
area: functions-and-abstraction/functions-as-values
level: intro
summary: >-
  A language has first-class functions when a function is a value. A program can store a function in a variable, pass it as an argument, return it from a function and put it in a data structure.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1967
  people: [Christopher Strachey]
  note: Strachey compared first-class and second-class objects in his 1967 lectures on programming languages. In ALGOL, a procedure was second class because no expression could give a procedure as its result.
see_also: [higher-order-functions, closures, function, currying, callback-pattern, map-over-a-list, function-signature, local-function]
sources:
- url: https://en.wikipedia.org/wiki/First-class_function
  title: First-class function, Wikipedia
  locator: the introduction; Concepts; Equality of functions
- url: https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/full-text/book/book-Z-H-12.html
  title: Structure and Interpretation of Computer Programs, second edition, Abelson and Sussman
  locator: 1.3.4 Procedures as Returned Values, footnotes 64 and 66
- url: https://developer.mozilla.org/en-US/docs/Glossary/First-class_Function
  title: First-class function, MDN Web Docs Glossary
  locator: the introduction
- url: https://docs.python.org/3/tutorial/controlflow.html#defining-functions
  title: More Control Flow Tools, The Python Tutorial
  locator: 4.8 Defining Functions
- url: https://docs.python.org/3/reference/expressions.html#calls
  title: Expressions, The Python Language Reference
  locator: 6.3.4 Calls
- url: https://www.typescriptlang.org/docs/handbook/2/functions.html
  title: More on Functions, The TypeScript Handbook
  locator: Function Type Expressions
- url: https://docs.oracle.com/javase/tutorial/java/javaOO/lambdaexpressions.html
  title: Lambda Expressions, The Java Tutorials
  locator: the introduction
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/function/package-summary.html
  title: Package java.util.function, Java SE 25 API
  locator: the package description
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/delegates/
  title: Delegates, C# Programming Guide
  locator: the introduction
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/lambda-expressions
  title: Lambda expressions and anonymous functions, C# language reference
  locator: the introduction
- url: https://en.cppreference.com/w/c/language/pointer.html
  title: Pointer declaration, C reference, cppreference.com
  locator: Pointers to functions
- url: https://en.cppreference.com/w/c/language/functions.html
  title: Functions, C reference, cppreference.com
  locator: Explanation
- url: https://en.cppreference.com/w/cpp/language/lambda.html
  title: Lambda expressions, C++ reference, cppreference.com
  locator: the introduction
- url: https://en.cppreference.com/w/cpp/utility/functional/function.html
  title: std::function, C++ reference, cppreference.com
  locator: the introduction
- url: https://go.dev/ref/spec#Function_types
  title: The Go Programming Language Specification
  locator: Function types; Function literals; Comparison operators
- url: https://doc.rust-lang.org/book/ch20-04-advanced-functions-and-closures.html
  title: Advanced Functions and Closures, The Rust Programming Language
  locator: Function Pointers; Returning Closures
- url: https://doc.rust-lang.org/book/ch13-01-closures.html
  title: Closures, The Rust Programming Language
  locator: Capturing References or Moving Ownership
- url: https://www.gnu.org/software/bash/manual/html_node/Shell-Functions.html
  title: Shell Functions, Bash Reference Manual
  locator: 3.3 Shell Functions
- url: https://www.gnu.org/software/bash/manual/html_node/Simple-Command-Expansion.html
  title: Simple Command Expansion, Bash Reference Manual
  locator: 3.7.1 Simple Command Expansion
- url: https://en.wikipedia.org/wiki/First-class_citizen
  title: First-class citizen, Wikipedia
  locator: History
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Strict_equality
  title: Strict equality (===), MDN Web Docs
  locator: Description
---

## Idea

A language has first-class functions when a function is a value, as a number or a string is a value. Wikipedia gives the definition: "In computer science, a programming language is said to have first-class functions if it treats functions as first-class citizens."[^1]

Structure and Interpretation of Computer Programs lists four rights of a first-class element[^2]:

1. "They may be named by variables."
2. "They may be passed as arguments to procedures."
3. "They may be returned as the results of procedures."
4. "They may be included in data structures."

MDN says the same for JavaScript: "a function can be passed as an argument to other functions, can be returned by another function and can be assigned as a value to a variable."[^3]

```python run id=idea
def double(x):
    return x * 2


def square(x):
    return x * x


for f in [double, square, abs]:
    print(f.__name__, f(-3))
```

```output for=idea
double -6
square 9
abs 3
```

The list holds three functions, two defined in the program and one built in. The loop takes each function from the list, puts it in the variable `f` and calls it.

## How it works

1. **A function is an object.** The definition of a function makes a value and gives it a name. In Python, "A function definition associates the function name with the function object in the current symbol table."[^4]
2. **Store it.** An assignment copies the value to a second name. Python says that "Other names can also point to that same function object and can also be used to access the function"[^4].
3. **Call it through any name.** A call needs a value that can be called, not a fixed name. In Python, "The primary must evaluate to a callable object"[^5]. So `f(5)`, `ops["square"](4)` and `double(5)` are all calls.
4. **Pass it and return it.** A function that takes or returns a function is a [[higher-order-functions|higher-order function]].
5. **Keep the free variables.** A function made inside another function can use the local variables of the outer function. These variables must stay alive after the outer function returns. This makes the function value a [[closures|closure]].

## In languages

- **Python.** Functions are objects. `def` makes a function object and gives it a name[^4]. "Small anonymous functions can be created with the lambda keyword."[^4]
- **JavaScript.** Functions are values. MDN says that functions "are treated like any other variable"[^3].
- **TypeScript.** The handbook says that functions are "also values"[^6]. A function type expression such as `(a: string) => void` gives the type of a function value[^6].
- **Java.** A method is not a value, but a lambda expression or a method reference is. Lambda expressions "treat functionality as method argument, or code as data"[^7]. The type of the value is a functional interface: "Functional interfaces provide target types for lambda expressions and method references."[^8]
- **C#.** A delegate holds a function: "A delegate is a type that represents references to methods with a particular parameter list and return type."[^9] "Delegates allow methods to be passed as parameters."[^9] A lambda converts to a delegate type such as `Func<int, int>`[^10].
- **C.** A function is not a value, but a pointer to a function is: "Unlike functions, pointers to functions are objects and thus can be stored in arrays, copied, assigned, passed to other functions as arguments, etc."[^11] C cannot make a new function at run time. "There are no nested functions"[^12], so a C program keeps extra data in a struct and passes it with the pointer.
- **C++.** C++ has function pointers from C, and lambdas. A lambda "Constructs a closure (an unnamed function object capable of capturing variables in scope)."[^13] `std::function` can hold any of them: "Class template std::function is a general-purpose polymorphic function wrapper."[^14]
- **Go.** Functions are values with a function type. "A function type denotes the set of all functions with the same parameter and result types."[^15] "Function literals are closures"[^15].
- **Rust.** A named function coerces to a function pointer: "The fn type is called a function pointer."[^16] Closures "will automatically implement one, two, or all three of these Fn traits"[^17], and "each closure is also its own distinct type"[^16]. So a function that returns a closure uses "the impl Trait syntax"[^16], for example `impl Fn(i32) -> i32`.
- **Bash.** Functions are not values. "Shell functions are a way to group commands for later execution using a single name for the group."[^18] A variable can hold the name of a function, and a command that starts with the variable calls that function, because after expansion "the first word is taken to be the name of the command and the remaining words are the arguments"[^19].

## Variations and related ideas

- **Second-class functions.** In ALGOL, a procedure could be an argument, but no expression could give a procedure as its result. Strachey wrote that "in a sense procedures in ALGOL are second class citizens"[^20].
- **Function pointers and delegates.** "Languages where functions are not first-class often still allow one to write higher-order functions through the use of features such as function pointers or delegates."[^1] A C function pointer has no captured data. A C# delegate and a C++ `std::function` can hold a closure[^10][^14].
- **Higher-order functions.** First-class functions make [[higher-order-functions]] possible: [[map-over-a-list|map]], filter, sort with a key, and callbacks.
- **Closures.** A function value that keeps the variables from the place where it was made is a [[closures|closure]]. Wikipedia says that lexically scoped first-class functions require "handling references to functions as closures instead of bare function pointers"[^1].
- **Currying.** A function that returns a function can take its arguments one at a time. See [[currying]].

## Pitfalls

### Calling the function instead of passing it

`f` is the function and `f(3)` is its result. A program that passes `f(3)` passes a number:

```python wrong id=python-call-not-pass
def double(x):
    return x * 2


def apply_twice(g, x):
    return g(g(x))


try:
    print(apply_twice(double(3), 3))
except TypeError as e:
    print(e)
```

```output for=python-call-not-pass
'int' object is not callable
```

`double(3)` is 6, so `apply_twice` gets the number 6 as `g`. A call needs "a callable object"[^5], and 6 is not one. Write `apply_twice(double, 3)`, which gives 12.

### Two equal functions are not equal

Two functions with the same code are two different values. Wikipedia says: "Two separately defined, but otherwise identical function definitions will be considered unequal."[^1] In JavaScript, `===` on objects returns true "only if they refer to the same object"[^21]:

```javascript run id=javascript-function-equality
const f = (x) => x * 2;
const g = (x) => x * 2;
console.log(f === g);
console.log(f === f);
```

```output for=javascript-function-equality
false
true
```

A program that removes a function from a list, or an event handler from an event, must keep the same function value that it added. A second function with the same code does not match. Go does not compare function values at all: "Slice, map, and function types are not comparable."[^15] A Go function value can only be compared to `nil`[^15].

## Cost

A function value that is a closure must keep its free variables. SICP says: "The major implementation cost of first-class procedures is that allowing procedures to be returned as values requires reserving storage for a procedure's free variables even while the procedure is not executing."[^2]

## History

Christopher Strachey introduced the idea of first-class and second-class objects. "The term was coined by Christopher Strachey in the context of "functions as first-class citizens" in the mid-1960s."[^1] SICP says that "The notion of first-class status of programming-language elements is due to the British computer scientist Christopher Strachey (1916-1975)."[^2]

Lisp gave procedures first-class status early. SICP says: "Lisp, unlike other common programming languages, awards procedures full first-class status."[^2] Early Lisp used dynamic scope. "Proper support for lexically scoped first-class functions was introduced in Scheme"[^1].

## References
