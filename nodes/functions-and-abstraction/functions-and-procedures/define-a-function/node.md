---
tag: 36C32X
kind: task
slug: define-a-function
name: Define a function
aliases: [function definition, declare a function, write a function, def, func, fn]
area: functions-and-abstraction/functions-and-procedures
level: intro
summary: >-
  Define a function clamp(value, low, high) that has three parameters and one result, and call it. The result is value limited to the range from low to high.
contract: contract.yaml
category: functions
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/tutorial/controlflow.html#defining-functions
  title: More Control Flow Tools, The Python Tutorial
  locator: 4.8 Defining Functions
- url: https://docs.python.org/3/library/typing.html
  title: typing, Support for type hints, The Python Standard Library
  locator: the introduction
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/function
  title: function, MDN Web Docs JavaScript Reference
  locator: the introduction
- url: https://www.typescriptlang.org/docs/handbook/2/everyday-types.html#functions
  title: Everyday Types, The TypeScript Handbook
  locator: Parameter Type Annotations; Return Type Annotations
- url: https://docs.oracle.com/javase/tutorial/java/javaOO/methods.html
  title: Defining Methods, The Java Tutorials
  locator: the whole page
- url: https://docs.oracle.com/javase/tutorial/java/javaOO/classvars.html
  title: Understanding Class Members, The Java Tutorials
  locator: Class Methods
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/methods
  title: Methods, C# Programming Guide
  locator: Method signatures
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/static-classes-and-static-class-members
  title: Static classes and static class members, C# Programming Guide
  locator: the introduction
- url: https://en.cppreference.com/w/c/language/functions.html
  title: Functions, C reference, cppreference.com
  locator: the introduction
- url: https://en.cppreference.com/w/cpp/language/function.html
  title: Function declaration, C++ reference, cppreference.com
  locator: Function definition
- url: https://go.dev/ref/spec#Function_declarations
  title: The Go Programming Language Specification
  locator: Function types; Function declarations; Exported identifiers
- url: https://doc.rust-lang.org/book/ch03-03-how-functions-work.html
  title: Functions, The Rust Programming Language
  locator: Parameters; Functions with Return Values
- url: https://www.gnu.org/software/bash/manual/html_node/Shell-Functions.html
  title: Shell Functions, Bash Reference Manual
  locator: 3.3 Shell Functions
- url: https://www.gnu.org/software/bash/manual/html_node/Bourne-Shell-Builtins.html
  title: Bourne Shell Builtins, Bash Reference Manual
  locator: return
- url: https://www.gnu.org/software/bash/manual/html_node/Exit-Status.html
  title: Exit Status, Bash Reference Manual
  locator: the first paragraph
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/Default_parameters
  title: Default parameters, MDN Web Docs JavaScript Reference
  locator: Description
- url: https://www.typescriptlang.org/docs/handbook/2/functions.html#optional-parameters
  title: More on Functions, The TypeScript Handbook
  locator: Optional Parameters
---

## Task

Define a function `clamp(value, low, high)` and call it. The function returns `value` limited to the range from `low` to `high`:

- `low` when `value` is less than `low`,
- `high` when `value` is more than `high`,
- `value` in all other cases.

For example, `clamp(42, 0, 10)` is 10, `clamp(-3, 0, 10)` is 0 and `clamp(5, 1, 10)` is 5. The three parameters and the result are 32-bit signed integers, and `low` is not more than `high`.

A function definition gives a name to a block of code and lists its parameters. In a language with static types, it also gives the type of each parameter and the type of the result. The tests call the function with arguments and compare the result. [[function]] explains what a function is, [[function-signature]] describes the first line of the definition, and [[return-value]] describes the result.

## Solutions

| Language | The definition | The call |
|---|---|---|
| Python | `def clamp(value, low, high):` | `clamp(42, 0, 10)` |
| JavaScript | `function clamp(value, low, high)` | `clamp(42, 0, 10)` |
| TypeScript | `function clamp(value: number, low: number, high: number): number` | `clamp(42, 0, 10)` |
| Java | `public static int clamp(int value, int low, int high)` in the class `Limits` | `Limits.clamp(42, 0, 10)` |
| C# | `public static int Clamp(int value, int low, int high)` in the class `Limits` | `Limits.Clamp(42, 0, 10)` |
| C | `int clamp(int value, int low, int high)`, declared in `clamp.h` | `clamp(42, 0, 10)` |
| C++ | `int clamp(int value, int low, int high)` in the namespace `limits` | `limits::clamp(42, 0, 10)` |
| Go | `func Clamp(value, low, high int32) int32` | `Clamp(42, 0, 10)` |
| Rust | `pub fn clamp(value: i32, low: i32, high: i32) -> i32` | `clamp(42, 0, 10)` |
| Bash | `clamp() { ... }`, which prints the result | `$(clamp 42 0 10)` |

- **Python.** "The keyword def introduces a function definition. It must be followed by the function name and the parenthesized list of formal parameters. The statements that form the body of the function start at the next line, and must be indented."[^1] "The return statement returns with a value from a function."[^1] The solution adds type hints to the parameters and the result. "The Python runtime does not enforce function and variable type annotations."[^2]
- **JavaScript.** "The function declaration creates a binding of a new function to a given name."[^3]
- **TypeScript.** "When you declare a function, you can add type annotations after each parameter to declare what types of parameters the function accepts."[^4] "Return type annotations appear after the parameter list"[^4]. The return type is optional: "Much like variable type annotations, you usually don’t need a return type annotation because TypeScript will infer the function’s return type based on its return statements."[^4] The solution writes it.
- **Java.** The solution defines `clamp` as a method of the class `Limits`. "The only required elements of a method declaration are the method's return type, name, a pair of parentheses, (), and a body between braces, {}."[^5] The method is `static`. Static methods "should be invoked with the class name, without the need for creating an instance of the class"[^6].
- **C#.** "Methods are declared in a class, struct, or interface by specifying the access level such as public or private, optional modifiers such as abstract or sealed, the return value, the name of the method, and any method parameters."[^7] `Clamp` is a method of the `static` class `Limits`. "A static class is basically the same as a non-static class, but there's one difference: a static class can't be instantiated."[^8] The caller uses the class name.
- **C.** "A function is a C language construct that associates a compound statement (the function body) with an identifier (the function name)."[^9] "A function is introduced by a function declaration or a function definition."[^9] The header `clamp.h` has the declaration, so the test file can call the function. `clamp.c` has the definition.
- **C++.** The definition looks like the C one. "A non-member function definition may appear at namespace scope only"[^10], and the solution puts it in the namespace `limits`.
- **Go.** "A function declaration binds an identifier, the function name, to a function."[^11] The type comes after the names, and one type can follow several names, as in the example `func(a, b int, z float32) (bool)` in the specification[^11]. The name `Clamp` starts with a capital letter, because an identifier is exported only if "the first character of the identifier's name is a Unicode uppercase letter"[^11].
- **Rust.** "In function signatures, you must declare the type of each parameter."[^12] For the result, "We don’t name return values, but we must declare their type after an arrow (->)."[^12] The body has no `return`: "In Rust, the return value of the function is synonymous with the value of the final expression in the block of the body of a function."[^12] Here that expression is the `if`.
- **Bash.** A Bash function has no parameter list: "fname () compound-command [ redirections ]"[^13]. "When a function is executed, the arguments to the function become the positional parameters during its execution"[^13], so the solution reads `$1`, `$2` and `$3`. The `return` builtin can "return the value n to its caller"[^14], but that value is an exit status, and "Exit statuses fall between 0 and 255"[^15]. So `clamp` prints its result, and the caller reads the output with `$( )`.

### A call with named arguments

Python can also pass the arguments by name. The order of the names does not matter:

```python run id=python-keyword-call
def clamp(value: int, low: int, high: int) -> int:
    if value < low:
        return low
    if value > high:
        return high
    return value


print(clamp(42, 0, 10))
print(clamp(high=10, low=0, value=-3))
```

```output for=python-keyword-call
10
0
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `at-low` and `at-high`: a value that is equal to a limit is in the range, so the function returns the value.
- `equal-bounds`: when `low` and `high` are equal, every value gives that one number.
- `negative-range`: the range from -20 to -10 has only negative numbers. The value -50 is below it, so the result is -20.
- `zero`: 0 is inside the range from -5 to 5.
- `int-max`, `int-min` and `full-range`: the largest and the smallest 32-bit integers. The solutions only compare numbers and do no arithmetic, so no value can overflow.

## Pitfalls

### A missing return in Python

A Python function that ends without a `return` returns `None`. "Falling off the end of a function also returns None."[^1]

```python wrong id=python-missing-return
def clamp(value, low, high):
    if value < low:
        return low
    if value > high:
        return high


print(clamp(42, 0, 10))
print(clamp(5, 1, 10))
```

```output for=python-missing-return
10
None
```

Add `return value` as the last line.

### A missing argument in JavaScript

JavaScript does not check the number of arguments. "In JavaScript, function parameters default to undefined."[^16]

```javascript wrong id=javascript-missing-argument
function clamp(value, low, high) {
  if (value < low) {
    return low;
  }
  if (value > high) {
    return high;
  }
  return value;
}

console.log(clamp(42, 0));
```

```output for=javascript-missing-argument
42
```

`high` is `undefined`, the comparison `42 > undefined` is false, and the function returns 42. TypeScript requires the argument unless the definition says that it is optional: "We can model this in TypeScript by marking the parameter as optional with ?"[^17].

### The return status in Bash

A Bash function that puts its result in the return status loses every value above 255[^15]:

```bash wrong id=bash-return-status
square() {
  return $(( $1 * $1 ))
}

square 20
echo $?
```

```output for=bash-return-status
144
```

400 does not fit, and the caller gets 144. Print the result with `echo`, and read it with `$(square 20)`.

## Related tasks

- [[return-multiple-values]]: return two results from one function.
- [[define-a-method-on-a-type]]: define a function that belongs to a type.
- [[define-a-custom-error-type]]: report a failure from a function with an error type.
- [[sum-and-multiply-a-list]]: a function that takes a list.

## References
