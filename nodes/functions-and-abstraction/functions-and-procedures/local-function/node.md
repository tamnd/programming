---
tag: TFDSQ6
kind: concept
slug: local-function
name: Local and nested functions
aliases: [nested function, local function, inner function, nested procedure]
area: functions-and-abstraction/functions-and-procedures
level: core
summary: >-
  A local or nested function is a function defined inside another function. Only the outer function can call it by name, and in most languages it can use the variables of the outer function.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [John Backus, Peter Naur]
  note: ALGOL 60 let procedures be nested with lexical scope, and Pascal and Ada kept this. C left nested functions out, and C# added local functions in version 7.0.
see_also: [function, closures, lexical-scope, block, higher-order-functions, recursion, method, stack-and-heap]
sources:
- url: https://en.wikipedia.org/wiki/Nested_function
  title: Nested function, Wikipedia
  locator: the introduction; Attributes; Languages; Implementation
- url: https://docs.python.org/3/reference/executionmodel.html#binding-of-names
  title: Execution model, The Python Language Reference
  locator: 4.2.1 Binding of names
- url: https://docs.python.org/3/reference/simple_stmts.html#the-nonlocal-statement
  title: Simple statements, The Python Language Reference
  locator: 7.13 The nonlocal statement
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Functions
  title: Functions, MDN Web Docs JavaScript Guide
  locator: Function scopes and closures
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.2
  title: Blocks, Statements, and Patterns, The Java Language Specification, Java SE 25 Edition
  locator: 14.2 Blocks; 14.3 Local Class and Interface Declarations
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/local-functions
  title: Local functions, C# Programming Guide
  locator: the introduction; Use parameters or capture local state; Choose a local function or lambda expression
- url: https://en.cppreference.com/w/c/language/functions.html
  title: Functions, C reference, cppreference.com
  locator: Explanation
- url: https://gcc.gnu.org/onlinedocs/gcc/Nested-Functions.html
  title: Nested Functions, Using the GNU Compiler Collection (GCC)
  locator: 6.4 Nested Functions
- url: https://en.cppreference.com/w/cpp/language/function.html
  title: Function declaration, C++ reference, cppreference.com
  locator: Function definition
- url: https://go.dev/ref/spec#Function_literals
  title: The Go Programming Language Specification
  locator: Source file organization; Function literals
- url: https://doc.rust-lang.org/reference/statements.html#item-declarations
  title: Statements, The Rust Reference
  locator: Item declarations
- url: https://doc.rust-lang.org/book/ch13-01-closures.html
  title: Closures, The Rust Programming Language
  locator: the introduction
- url: https://www.gnu.org/software/bash/manual/html_node/Shell-Functions.html
  title: Shell Functions, Bash Reference Manual
  locator: 3.3 Shell Functions
- url: https://en.wikipedia.org/wiki/ALGOL_60
  title: ALGOL 60, Wikipedia
  locator: the introduction
---

## Idea

A local function, also called a nested function, is a function defined inside another function. Its name is visible only inside the outer function, so other code cannot call it. In most languages that have them, the inner function can also read the variables and the parameters of the outer function.

Wikipedia defines it: "a nested function (or nested procedure or subroutine) is a named function that is defined within another (enclosing) block and is lexically scoped within the enclosing block – meaning it is only callable by name within the body of the enclosing block and can use identifiers declared in outer blocks, including outer functions."[^1]

```python run id=idea
def sum_of_scaled(values, factor):
    def scale(x):
        return x * factor

    total = 0
    for v in values:
        total += scale(v)
    return total


print(sum_of_scaled([1, 2, 3], 10))
```

```output for=idea
60
```

The function `scale` exists only inside `sum_of_scaled`. It reads `factor`, which is a parameter of the outer function, so the call `scale(v)` needs only one argument.

## How it works

1. **The scope of the name.** The inner function is a local name of the outer function. "The scope of a nested function is the block that contains it – be it a function block or block within a function body. It is not visible (cannot be called by name) outside its containing block."[^1]
2. **The outer variables.** The inner function uses the rules of [[lexical-scope]]. "A nested function can use identifiers (i.e. the name of functions, variables, types, classes) declared in any enclosing block, except when they are masked by inner declarations with the same names."[^1]
3. **When the definition runs.** In Python, a `def` in a function body binds the name of the inner function in the block of the outer function. "If a name is bound in a block, it is a local variable of that block, unless declared as nonlocal or global."[^2]
4. **The life of the inner function.** If the outer function returns the inner function or stores it, the inner function can outlive the call. Then it is a closure. See [[closures]].
5. **The implementation.** "Implementation of nested functions can be more involved than it may appear, as a reference to a nested function that references non-local variables creates a closure."[^1] A classic compiler reaches an outer variable through links between the stack frames: "Any non-local object, X, is reached via access-links in the activation frames on the machine stack."[^1]

## In languages

- **Python.** A `def` statement can appear in a function body. The inner function reads the outer variables. To assign to one, it must declare it `nonlocal`. "The nonlocal statement causes the listed identifiers to refer to names previously bound in nonlocal scopes."[^3]
- **JavaScript.** A function declaration can appear in a function body. "A function defined inside another function can also access all variables defined in its parent function, and any other variables to which the parent function has access."[^4]
- **TypeScript.** The rules are the same as in JavaScript. The inner function can have type annotations.
- **Java.** A method cannot contain a method. A block holds only these statements: "BlockStatement: LocalClassOrInterfaceDeclaration LocalVariableDeclarationStatement Statement"[^5]. So a method can declare a local class, or a local variable that holds a lambda.
- **C#.** "A local function is a named function declared inside another member, such as a method or constructor. Its name is in scope only within that member."[^6] "A non-static local function can use parameters and local variables from its containing member. This use is called capture."[^6]
- **C.** Standard C has no nested functions: "each function definition must appear at file scope"[^7]. GCC has them as an extension: "Nested functions are supported as an extension in GNU C, but are not supported by GNU C++."[^8]
- **C++.** "A non-member function definition may appear at namespace scope only (there are no nested functions)."[^9] A lambda expression in the function body does the same work.
- **Go.** A function declaration is a top-level declaration: "TopLevelDecl = Declaration | FunctionDecl | MethodDecl ."[^10] In a function body, a Go program assigns a function literal to a variable. "Function literals are closures: they may refer to variables declared in a surrounding function."[^10]
- **Rust.** A `fn` item can appear in a block, and "Declaring an item within a statement block restricts its scope to the block containing the statement."[^11] But the inner `fn` cannot use the outer variables: "There is no implicit capture of the containing function’s generic parameters, parameters, and local variables."[^11] A closure can capture them: "Unlike functions, closures can capture values from the scope in which they’re defined."[^12]
- **Bash.** A function definition is a command, and "This defines a shell function named fname."[^13] A definition inside a function body runs when the outer function runs, and the new function is not local to the outer function. Bash variables use dynamic scope: "The value of a variable that a function sees depends on its value within its caller"[^13].

## Variations and related ideas

- **Inner functions and lambdas.** A lambda can also be defined in a function body and use the outer variables. The C# guide compares them: a local function is the better choice when "The helper benefits from a name, has several statements, needs recursion, or is called from more than one place in the containing member."[^6] Wikipedia says that a lambda "has no name (is anonymous) and therefore cannot be called by name"[^1]. See [[higher-order-functions]].
- **Static local functions.** C# lets a local function declare that it uses no outer state: "A static local function can't capture local variables, parameters, or instance state from the containing member."[^6] A Rust inner `fn` is always like this[^11].
- **Recursive helpers.** "A nested function typically acts as a helper function or a recursive function."[^1] In C#, "Local functions work well for recursion because their names are in scope throughout the containing member."[^6] See [[recursion]].
- **Private functions of a module.** A language without nested functions can hide a helper in a module instead. Wikipedia says: "In C, this can be implemented by declaring functions and variables as static to hide them from code"[^1] outside the file.
- **Closures.** An inner function that the outer function returns keeps the outer variables. See [[closures]].

## Pitfalls

### Assigning to an outer variable in Python

An assignment in the inner function makes a new local variable of the inner function[^2]. Then the read in `count += 1` fails:

```python wrong id=python-missing-nonlocal
def counter():
    count = 0

    def increment():
        count += 1

    increment()
    return count


try:
    print(counter())
except UnboundLocalError as e:
    print(type(e).__name__)
```

```output for=python-missing-nonlocal
UnboundLocalError
```

Add `nonlocal count` as the first line of `increment`. The `nonlocal` statement "allows encapsulated code to rebind such nonlocal identifiers"[^3].

### An inner function in Bash is global

A Bash function definition inside another function defines the function for the whole shell when the outer function runs[^13]. It can replace a function with the same name:

```bash wrong id=bash-inner-global
helper() {
  echo "global helper"
}

outer() {
  helper() {
    echo "inner helper"
  }
  helper
}

outer
helper
```

```output for=bash-inner-global
inner helper
inner helper
```

After `outer` runs, the global `helper` is gone. Give helper functions names with a prefix, such as `outer_helper`, and do not define functions inside functions.

### A Rust inner fn cannot see the outer variables

The Rust reference gives this example[^11]:

```rust norun reason=fragment
fn outer() {
    let outer_var = true;

    fn inner() { /* outer_var is not in scope here */ }

    inner();
}
```

The reference says that "inner may not access outer_var."[^11] Use a closure, such as `let inner = || outer_var;`, or pass the value to the inner `fn` as a parameter.

## Cost

An inner function that uses no outer variables costs the same as a top-level function. An inner function that uses outer variables needs a way to reach them. A classic compiler follows access links between the frames on the stack[^1]. GCC uses another technique when a program takes the address of a nested function: "GCC implements taking the address of a nested function using a technique called trampolines."[^8] A closure that outlives its call keeps the captured variables alive. In Go, "they survive as long as they are accessible"[^10]. See [[stack-and-heap]].

## History

"ALGOL 60 function definitions could be nested within one another (a feature introduced by ALGOL 60) with lexical scope."[^14] Simula 67, Pascal and Ada also have nested procedures[^1]. Wikipedia says nested functions are not supported in C, C++ or Java "as this makes compilers more difficult to implement."[^1] C# added local functions "starting with C# 7.0"[^1], and Kotlin and Rust have them[^1].

## References
