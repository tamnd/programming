---
tag: M0DRR8
kind: concept
slug: anonymous-function
name: Anonymous function
aliases: [lambda, lambda expression, lambda function, function literal, arrow function, function expression]
area: functions-and-abstraction/functions-as-values
level: intro
summary: >-
  An anonymous function is a function with no name, written as an expression where the program needs it. Python lambda, JavaScript arrow functions, C# lambdas and Go function literals are examples.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1936
  people: [Alonzo Church]
  note: In the lambda calculus of Church all functions are anonymous. Lisp had anonymous functions in 1958, and the keyword lambda comes from the lambda calculus.
see_also: [first-class-function, closures, higher-order-functions, function, local-function, currying, map-over-a-list, callback-pattern]
sources:
- url: https://en.wikipedia.org/wiki/Anonymous_function
  title: Anonymous function, Wikipedia
  locator: the introduction; Names
- url: https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/full-text/book/book-Z-H-12.html
  title: Structure and Interpretation of Computer Programs, second edition, Abelson and Sussman
  locator: 1.3.2 Constructing Procedures Using Lambda, footnote 53
- url: https://docs.python.org/3/reference/expressions.html#lambda
  title: Expressions, The Python Language Reference
  locator: 6.14 Lambdas
- url: https://docs.oracle.com/javase/tutorial/java/javaOO/lambdaexpressions.html
  title: Lambda Expressions, The Java Tutorials
  locator: the introduction; Syntax of Lambda Expressions
- url: https://www.typescriptlang.org/docs/handbook/2/everyday-types.html#anonymous-functions
  title: Everyday Types, The TypeScript Handbook
  locator: Anonymous Functions
- url: https://go.dev/ref/spec#Function_literals
  title: The Go Programming Language Specification
  locator: Function literals
- url: https://docs.python.org/3/tutorial/controlflow.html#lambda-expressions
  title: More Control Flow Tools, The Python Tutorial
  locator: 4.9.6 Lambda Expressions
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/Arrow_functions
  title: Arrow function expressions, MDN Web Docs
  locator: the introduction; Function body
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/lambda-expressions
  title: Lambda expressions and anonymous functions, C# language reference
  locator: the introduction; Capture of outer variables and variable scope in lambda expressions
- url: https://en.cppreference.com/w/c/language/functions.html
  title: Functions, C reference, cppreference.com
  locator: the introduction
- url: https://en.cppreference.com/w/cpp/language/lambda.html
  title: Lambda expressions (since C++11), C++ reference, cppreference.com
  locator: the introduction
- url: https://doc.rust-lang.org/book/ch13-01-closures.html
  title: Closures, The Rust Programming Language
  locator: the introduction; Inferring and Annotating Closure Types
- url: https://www.gnu.org/software/bash/manual/html_node/Shell-Functions.html
  title: Shell Functions, Bash Reference Manual
  locator: 3.3 Shell Functions
---

## Idea

An anonymous function is a function that has no name. The program writes it as an expression, often as the argument of another function. Wikipedia gives the definition: "In computer programming, an anonymous function (function literal, lambda function, or block) is a function definition that is not bound to an identifier."[^1]

```python run id=idea
words = ["house", "car", "bike"]
print(sorted(words, key=lambda w: len(w)))


def length(w):
    return len(w)


print(sorted(words, key=length))
```

```output for=idea
['car', 'bike', 'house']
['car', 'bike', 'house']
```

The two calls do the same work. The first call gives `sorted` an anonymous function. The second call gives it the named function `length`. SICP says that a procedure made with `lambda` "is just as much a procedure as one that is created using define. The only difference is that it has not been associated with any name in the environment."[^2]

## How it works

1. **An expression makes a function.** The anonymous function is an expression, so it can be in any place where a value can be: an argument, a variable or a return value. In Python, "The expression lambda parameters: expression yields a function object."[^3]
2. **The body.** Some languages allow only one expression in the body. Others allow a block of statements. In Java, the body "consists of a single expression or a statement block"[^4].
3. **The types.** Many statically typed languages get the types of the parameters from the place where the function is used. In TypeScript, "When a function appears in a place where TypeScript can determine how it’s going to be called, the parameters of that function are automatically given types."[^5]
4. **The captured variables.** An anonymous function can use the variables around it. Then it is a [[closures|closure]]. In Go, "Function literals are closures: they may refer to variables declared in a surrounding function."[^6]

## In languages

- **Python.** The `lambda` expression. "Lambda expressions (sometimes called lambda forms) are used to create anonymous functions."[^3] The body is one expression: lambda functions "are syntactically restricted to a single expression"[^7].
- **JavaScript.** Function expressions such as `function (x) { return x * x; }` and arrow functions such as `(x) => x * x`. "An arrow function expression is a compact alternative to a traditional function expression, with some semantic differences and deliberate limitations in usage"[^8].
- **TypeScript.** The same forms as JavaScript, with optional type annotations. The parameters get their types from the context[^5].
- **Java.** Lambda expressions such as `x -> x * x`. "Lambda expressions let you express instances of single-method classes more compactly."[^4]
- **C#.** Lambda expressions such as `x => x * x`. "Use a lambda expression to create an anonymous function."[^9] A `static` lambda "can't capture local variables or instance state from enclosing scopes"[^9].
- **C.** C has no anonymous functions. "A function is a C language construct that associates a compound statement (the function body) with an identifier (the function name)."[^10] A C program passes a pointer to a named function.
- **C++.** Lambda expressions such as `[](int x) { return x * x; }`. A lambda "Constructs a closure (an unnamed function object capable of capturing variables in scope)."[^11]
- **Go.** Function literals such as `func(x int) int { return x * x }`. "A function literal represents an anonymous function."[^6]
- **Rust.** Closures such as `|x| x * x`. "Rust’s closures are anonymous functions you can save in a variable or pass as arguments to other functions."[^12] "Closures don’t usually require you to annotate the types of the parameters or the return value like fn functions do."[^12]
- **Bash.** Bash has no anonymous functions. The syntax `fname () compound-command` "defines a shell function named fname"[^13], so every Bash function has a name.

## Variations and related ideas

- **Names.** Wikipedia lists the names "function literal, lambda function, or block"[^1]. "In several programming languages, anonymous functions are introduced using the keyword lambda, and anonymous functions are often referred to as lambdas or lambda abstractions."[^1]
- **Expression body or block body.** Python allows only an expression[^7]. JavaScript, Java and C# allow an expression body or a block body[^8][^4][^9]. C# calls the two forms an expression lambda and a statement lambda[^9].
- **Closures.** An anonymous function that uses outer variables is a closure. Go function literals[^6], C++ lambdas[^11] and Rust closures[^12] can capture variables. See [[closures]].
- **First-class functions.** An anonymous function is only useful when functions are values. See [[first-class-function]] and [[higher-order-functions]].
- **Local functions.** A named function inside another function is the other way to make a small helper. See [[local-function]].

## Pitfalls

### An object literal in an arrow function body

In JavaScript, a brace after `=>` starts a block, not an object. MDN says that `(params) => { object: literal }` "does not work as expected"[^8]:

```javascript wrong id=javascript-object-body
const wrap = (value) => { value: value };
console.log(wrap(5));
```

```output for=javascript-object-body
undefined
```

The body is a block with the label `value` and the expression `value`. The block has no `return`, so the call gives `undefined`. MDN gives the fix: "wrap the object literal in parentheses"[^8]. Write `(value) => ({ value: value })`.

### A Python lambda holds one expression

A Python lambda cannot hold an assignment, a loop or a `return`. "Note that functions created with lambda expressions cannot contain statements or annotations."[^3] When the code needs statements, write a named function with `def`.

### An arrow function is not a method

Arrow functions "don't have their own bindings to this, arguments, or super, and should not be used as methods"[^8]. An arrow function in an object literal cannot read the object through `this`. Use the method syntax or a `function` expression for methods.

## Cost

An anonymous function costs the same as a named function with the same body. The Python tutorial says that lambda functions "are just syntactic sugar for a normal function definition"[^7]. An anonymous function that captures variables costs the same as any other [[closures|closure]].

## History

"Anonymous functions originate in the work of Alonzo Church in his invention of the lambda calculus, in which all functions are anonymous, in 1936, before electronic computers."[^1] "Anonymous functions have been a feature of programming languages since Lisp in 1958"[^1]. Lisp took the keyword `lambda` from the notation of the lambda calculus[^2]. SICP says: "It would be clearer and less intimidating to people learning Lisp if a name more obvious than lambda, such as make-procedure, were used. But the convention is firmly entrenched."[^2]

## References
