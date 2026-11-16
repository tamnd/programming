---
tag: RZYRG5
kind: concept
slug: function
name: Function
aliases: [subprogram, callable unit, routine, user-defined function]
area: functions-and-abstraction/functions-and-procedures
level: intro
summary: >-
  A function is a named piece of code that takes inputs as parameters, runs its body when a program calls it, and gives back a result. A program defines it once and calls it many times.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1945
  people: [Alan Turing, Maurice Wilkes, David Wheeler, Stanley Gill]
  note: Turing discussed subroutines in his 1945 proposal for the ACE, and Wilkes, Wheeler and Gill are credited with the closed subroutine of the EDSAC. FORTRAN II in 1958 was one of the first languages with user-written subroutines and functions.
see_also: [procedure-concept, function-call, return-value, function-signature, method-concept, parameter-passing, higher-order-functions, closures]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Functions
  title: Functions, JavaScript Guide, MDN Web Docs
  locator: the introduction; Defining functions; Function hoisting
- url: https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/full-text/book/book-Z-H-10.html
  title: Structure and Interpretation of Computer Programs, 2nd edition
  locator: 1.1.4 Compound Procedures
- url: https://doc.rust-lang.org/reference/items/functions.html
  title: Functions, The Rust Reference
  locator: items.fn.intro
- url: https://docs.python.org/3/reference/compound_stmts.html#function-definitions
  title: Compound statements, The Python Language Reference
  locator: 8.7 Function definitions
- url: https://docs.python.org/3/tutorial/controlflow.html#defining-functions
  title: More Control Flow Tools, The Python Tutorial
  locator: 4.8 Defining Functions
- url: https://www.typescriptlang.org/docs/handbook/2/functions.html
  title: More on Functions, The TypeScript Handbook
  locator: the introduction; Function Type Expressions
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.4
  title: Classes, The Java Language Specification, Java SE 25 Edition
  locator: 8.4 Method Declarations; 8.4.3.2 static Methods
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/methods
  title: Methods, C# Programming Guide
  locator: the introduction
- url: https://en.cppreference.com/w/c/language/functions.html
  title: Functions, C reference, cppreference.com
  locator: the introduction
- url: https://en.cppreference.com/w/cpp/language/functions.html
  title: Functions, C++ reference, cppreference.com
  locator: the introduction
- url: https://go.dev/ref/spec#Function_declarations
  title: The Go Programming Language Specification
  locator: Function declarations; Function types
- url: https://doc.rust-lang.org/book/ch03-03-how-functions-work.html
  title: Functions, The Rust Programming Language
  locator: Functions with Return Values
- url: https://www.gnu.org/software/bash/manual/html_node/Shell-Functions.html
  title: Shell Functions, Bash Reference Manual
  locator: 3.3 Shell Functions
- url: https://www.gnu.org/software/bash/manual/html_node/Bourne-Shell-Builtins.html
  title: Bourne Shell Builtins, Bash Reference Manual
  locator: return
- url: https://gcc.gnu.org/onlinedocs/gcc/Inline.html
  title: An Inline Function is As Fast As a Macro, Using the GNU Compiler Collection
  locator: 6.9
- url: https://en.wikipedia.org/wiki/Function_(computer_programming)
  title: Function (computer programming), Wikipedia
  locator: History; Language support
- url: https://en.wikipedia.org/wiki/Wheeler_jump
  title: Wheeler Jump, Wikipedia
  locator: the introduction
- url: https://www.masswerk.at/algol60/report.htm
  title: Revised Report on the Algorithmic Language Algol 60
  locator: 5.4 Procedure declarations
---

## Idea

A function is a named piece of code. It takes inputs, called parameters, it runs a list of statements, called the body, and it gives back a result. The program defines the function once and then calls it by name each time it needs the work.

```python run id=idea
def rectangle_area(width, height):
    return width * height


print(rectangle_area(3, 4))
print(rectangle_area(5, 6))
```

```output for=idea
12
30
```

The definition says what the work is. Each call gives the parameters values, called arguments, and gets back one result. The two calls run the same body with different arguments.

The word comes from mathematics, but a function in code is wider. MDN says that a function in JavaScript "is similar to a procedure—a set of statements that performs a task or calculates a value, but for a procedure to qualify as a function, it should take some input and return an output where there is some obvious relationship between the input and the output"[^1]. A mathematical function always maps the same input to the same output. A function in code can also read a clock, write to a file or change a variable. A function that only computes its result from its arguments is close to the mathematical idea. [[side-effect]] explains the difference.

SICP calls the definition of a function "a much more powerful abstraction technique by which a compound operation can be given a name and then referred to as a unit"[^2]. The name hides the steps. A reader of `rectangle_area(3, 4)` does not need to know how the body computes the area.

## How it works

A function has two parts in the program text: the definition and the calls.

1. **The definition.** It gives the name, the list of parameters, often the types of the parameters and the result, and the body. The Rust Reference names these parts: "A function consists of a block (that’s the body of the function), along with a name, a set of parameters, and an output type"[^3]. [[function-signature]] describes the part without the body.
2. **The binding.** The definition binds the name to the function. In Python, a definition is a statement that runs: "A function definition is an executable statement"[^4]. It creates "a user-defined function object"[^4], but "The function definition does not execute the function body; this gets executed only when the function is called"[^4]. In C, C++, Java and Go, the compiler handles the definitions before the program runs.
3. **The call.** A call names the function and gives the arguments. The program puts the argument values into the parameters, runs the body and then goes back to the place of the call. [[function-call]] describes these steps. [[parameter-passing]] describes the ways a language can pass an argument.
4. **The result.** The body gives back a value, usually with a `return` statement, and the call expression takes that value. [[return-value]] describes the rules.

A function that gives back no value is often called a procedure or a subroutine. [[procedure-concept]] describes them.

## In languages

All the tier 1 languages have functions, but some give them another name or put them inside a class. The examples in each language define and call the same function.

- **Python.** "The keyword def introduces a function definition"[^5]. A function is an object, so a program can store it in a variable or pass it to another function. "Falling off the end of a function also returns None"[^5].
- **JavaScript.** A function declaration "consists of the function keyword, followed by" the name, the parameters and the body[^1]. JavaScript also has function expressions and arrow functions, which give a function without a name.
- **TypeScript.** "Functions are the basic building block of any application, whether they’re local functions, imported from another module, or methods on a class"[^6]. TypeScript adds types to the parameters and the result. When the result type is not written, the compiler infers it[^6].
- **Java.** Java has no functions outside classes. Each function is a method of a class: "A method declares executable code that can be invoked, passing a fixed number of values as arguments"[^7]. "A method that is declared static is called a class method"[^7]. A static method has no object, so it works as a plain function.
- **C#.** "A method is a code block that contains a series of statements"[^8]. As in Java, a method belongs to a class or a struct. C# also has local functions, which are functions inside a method.
- **C.** "A function is a C language construct that associates a compound statement (the function body) with an identifier (the function name)"[^9]. Each program starts in the function `main`.
- **C++.** "Functions are C++ entities that associate a sequence of statements"[^10] with a name and a list of parameters. C++ has free functions, as in C, and member functions of classes.
- **Go.** "A function declaration binds an identifier, the function name, to a function"[^11]. A Go function can return more than one result. "A function type denotes the set of all functions with the same parameter and result types"[^11], so functions are values with types.
- **Rust.** A function is an item with the keyword `fn`. "In Rust, the return value of the function is synonymous with the value of the final expression in the block of the body of a function"[^12], so a function can end with an expression and no `return`.
- **Bash.** "Shell functions are a way to group commands for later execution using a single name for the group"[^13]. A Bash function has no parameter list in its definition. It reads its arguments as `$1`, `$2` and so on. It gives back only an exit status. A function that must give back text prints it, and the caller captures the output with `$( )`.

## Variations and related ideas

- **Procedures and subroutines.** Some languages, such as Pascal and Ada, keep two words: a function gives back a value, and a procedure does not. C, Java and Go use one construct and give a procedure the result type `void` or no result. See [[procedure-concept]].
- **Methods.** A method is a function that belongs to a type or an object, and it gets the object as a hidden or explicit first parameter. See [[method-concept]].
- **Anonymous functions.** A lambda or an arrow function is a function with no name. A program writes it where it uses it, for example as an argument.
- **Functions as values.** In Python, JavaScript, Go and Rust, a program can store a function in a variable, pass it as an argument and return it from another function. See [[higher-order-functions]].
- **Closures.** A function that is defined inside another function can use the variables of the outer function. See [[closures]].
- **Recursion.** A function can call itself. See [[recursion]].
- **Pure functions.** A function whose result depends only on its arguments, and that changes nothing outside itself, behaves like a mathematical function. See [[side-effect]].

## Pitfalls

### A missing return

In Python, a function with no `return` statement gives back `None`. A body that computes a value but does not return it is a common mistake:

```python wrong id=python-missing-return
def rectangle_area(width, height):
    width * height


print(rectangle_area(3, 4))
```

```output for=python-missing-return
None
```

The body computes 12 and throws it away. "Falling off the end of a function also returns None"[^5]. Write `return width * height`. A typed language such as Java, C# or Rust stops this at compile time, because a function with a result type must return a value of that type.

### The name without the call

In Python and JavaScript, the name of a function without parentheses is the function object, not a call. An object is true in a test, so the test is always true:

```python wrong id=python-no-call
def is_ready():
    return False


if is_ready:
    print("ready")
```

```output for=python-no-call
ready
```

The test checks the function object, which is true. Write `if is_ready():` to call the function and test its result.

### A function expression used before its definition

In JavaScript, a function declaration is hoisted: the program can call it before the line that defines it. But "Function hoisting only works with function declarations — not with function expressions"[^1]:

```javascript wrong id=javascript-expression-hoisting
try {
  console.log(square(5));
} catch (e) {
  console.log(e.name);
}

const square = function (n) {
  return n * n;
};
```

```output for=javascript-expression-hoisting
ReferenceError
```

The name `square` exists, but it has no value until the `const` line runs. Define the function before the first call, or use a function declaration.

### A Bash function that returns a number

`return` in Bash sets the exit status, not a result value. "If n is supplied, the return value is its least significant 8 bits"[^14]:

```bash wrong id=bash-return-number
double() {
  return $(( $1 * 2 ))
}

double 150
echo "$?"
```

```output for=bash-return-number
44
```

300 does not fit in 8 bits, so the status is 300 minus 256, which is 44. Print the result with `echo` and capture it with `result=$(double 150)`.

## Cost

A call costs a few machine instructions: the program passes the arguments, saves the place to come back to, jumps to the body and jumps back. In a tight loop this cost can matter. Compilers remove it for small functions by inlining. GCC says that "By declaring a function inline, you can direct GCC to make calls to that function faster", and that "One way GCC can achieve this is to integrate that function’s code into the code for its callers"[^15].

The larger effect is on the people who read the code. A function with a good name lets a reader skip its body, and a function that is called from many places has one place to fix.

## History

Subroutines came before programming languages. Maurice Wilkes, David Wheeler and Stanley Gill "are generally credited with the formal invention of this concept, which they termed a closed sub-routine"[^16]. Wheeler developed his method of calling a subroutine "while working on the pioneering EDSAC machine"[^17]. Alan Turing "had discussed subroutines in a paper of 1945 on design proposals for the NPL ACE"[^16]. Some of "The earliest computers, such as the Manchester Baby," had no call instruction, so a program had to store the return address by itself[^16].

"One of the first programming languages to support user-written subroutines and functions was FORTRAN II. The IBM FORTRAN II compiler was released in 1958"[^16]. The ALGOL 60 report defined procedures: "A procedure declaration serves to define the procedure associated with a procedure identifier"[^18].

## References
