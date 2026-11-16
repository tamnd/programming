---
tag: DWPRJ5
kind: task
slug: declare-a-variable
name: Declare a variable
aliases: [variable declaration, declare and initialize a variable, define a variable, create a variable, let, var, local variable, type inference, Variables]
area: data-and-values/variables-and-assignment
level: intro
summary: >-
  Declare a variable, give it a first value and assign it new values, with a named or an inferred type. Know which languages need a declaration and which make a variable on the first assignment.
contract: contract.yaml
category: basics
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Variables/00-TASK.txt
  title: Variables, Rosetta Code (the RosettaCodeData mirror)
  locator: Task description
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Variable declarations; Short variable declarations
- url: https://docs.python.org/3/reference/simple_stmts.html
  title: Simple statements, The Python Language Reference
  locator: 7.2.2 Annotated assignment statements
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/let
  title: let, MDN Web Docs
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/declarations
  title: Declaration statements, C# language reference
  locator: Implicitly typed local variables
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: Chapter 14, Blocks, Statements, and Patterns, The Java Language Specification, Java SE 25
  locator: 14.4 Local Variable Declarations
- url: https://doc.rust-lang.org/book/ch03-01-variables-and-mutability.html
  title: Variables and Mutability, The Rust Programming Language
  locator: Variables and Mutability
- url: https://www.gnu.org/software/bash/manual/html_node/Bash-Builtins.html
  title: Bash Builtin Commands, GNU Bash Reference Manual
  locator: local; declare
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/var
  title: var, MDN Web Docs
  locator: Hoisting
---

## Task

The Rosetta Code task asks for the methods of a language for variable declaration, initialization, assignment, datatypes and scope[^1]. A declaration introduces a name. In Go, "A variable declaration creates one or more variables, binds corresponding identifiers to them, and gives each a type and an initial value."[^2] An initialization gives the first value, and an assignment gives a new value later.

The function `total(values)` shows all three. It declares a variable `result`, initializes it to 0, and assigns it a new value once for each value in the list. It returns the sum, and 0 for the empty list. The values and the partial sums fit in a 64-bit signed integer.

## Solutions

Each solution declares `result` inside the function, so the variable is local:

| Language | Declaration | Type |
|---|---|---|
| Python | `result: int = 0` | an optional annotation; the first assignment makes the variable |
| JavaScript | `let result = 0` | no static type |
| TypeScript | `let result: number = 0` | named, or inferred from `0` |
| Java | `long result = 0` | named; `var` infers it |
| C# | `long result = 0` | named; `var` infers it |
| Go | `var result int64` | named, with the zero value; `:=` infers it |
| Rust | `let mut result: i64 = 0` | named, or inferred; `mut` allows new values |
| C | `long long result = 0` | named |
| C++ | `long long result = 0` | named; `auto` infers it |
| Bash | `local -i result=0` | no type; `-i` makes it an integer |

- **Python.** Python has no declaration statement. The first assignment to a name in a function makes a local variable. An annotation adds a type for tools to check: "Annotation assignment is the combination, in a single statement, of a variable or attribute annotation and an optional assignment statement"[^3]. Python does not check the annotation when the program runs.
- **JavaScript and TypeScript.** "The let declaration declares re-assignable, block-scoped local variables, optionally initializing each to a value."[^4] The loop variable uses `const`, because it does not change in one pass. TypeScript adds the type `number`, which it can also infer from `0`.
- **Java and C#.** The type comes before the name. Both languages also have `var`: "you can let the compiler infer the type of the variable from the initialization expression"[^5]. The variable still has one static type. In Java, a `var` declaration without an initializer is an error, for example when "The VariableDeclarator lacks an initializer."[^6]
- **Go.** `var result int64` declares the variable and gives it the zero value 0. The short form `result := int64(0)` "is shorthand for a regular variable declaration with initializer expressions but no types"[^2].
- **Rust.** "by default, variables are immutable"[^7]. The solution writes `let mut`, because `result` gets a new value on each pass. Without `mut`, the compiler gives the error "cannot assign twice to immutable variable"[^7].
- **C and C++.** The type comes before the name. A local variable without an initializer has no defined value in C and C++, so the solution writes `= 0`. C++ also has `auto` for an inferred type.
- **Bash.** `local` creates a variable "and assign it value"[^8] that is visible only in the function and the functions it calls. `-i` makes the variable an integer, so "arithmetic evaluation (see Shell Arithmetic) is performed when the variable is assigned a value"[^8]. In Bash there are no spaces around `=`.

In Python, a declaration, an annotation and an assignment look like this:

```python run id=python-declare
result = 0
for value in [1, 2, 3]:
    result = result + value
print(result)

width: int = 80
height: int
print(width, "height" in globals())
```

```output for=python-declare
6
80 False
```

The annotation `height: int` without a value makes no variable, so the name `height` is not in the globals.

## Edge cases

- **The empty list.** The loop does not run, so the result is the first value, 0. This is why the first value matters.
- **One value.** The result is that value.
- **Negative values and zeros.** `[-1, -2, -3]` gives -6, and `[7, -7]` gives 0.
- **Sums past 32 bits.** `[4000000000, 5000000000]` gives 9000000000. A 32-bit `int` overflows here, so the solutions use 64-bit types: `long`, `int64`, `i64` and `long long`.

## Pitfalls

- **A typo makes a new variable.** In Python, an assignment to a misspelled name makes a second variable, and the program gives a wrong answer with no error:

```python wrong id=python-typo
result = 0
for value in [1, 2, 3]:
    resutl = result + value
print(result)
```

```output for=python-typo
0
```

A language with declarations rejects the typo. In JavaScript modules and strict mode, an assignment to an undeclared name throws `ReferenceError`. Go, Rust, Java, C# and C reject it at compile time.

- **`var` in JavaScript.** A `var` variable belongs to the function, not to the block. Also, "var declarations, wherever they occur in a script, are processed before any code within the script is executed."[^9] Use `let` and `const`.
- **No first value in C and C++.** `long long result;` followed by `result = result + value` reads a value that the program never set. The result can be any number.
- **A global variable in Bash.** Without `local`, an assignment in a function changes a global variable that other functions also see.
- **An unused variable in Go.** "A compiler may make it illegal to declare a variable inside a function body if the variable is never used."[^2] The Go compiler does this, so a declared and unused local variable is an error.

## Related tasks

- [[variable-concept]]: what a variable is, as a name and as a storage place.
- [[initialization-concept]]: the first value, and what happens without one.
- [[assignment]]: the assignment that gives the variable new values.
- [[declare-a-constant]]: a name whose value does not change.

## References
