---
tag: GD94GQ
kind: concept
slug: return-value
name: Return value
aliases: [return statement, result, function result, early return, implicit return]
area: functions-and-abstraction/functions-and-procedures
level: core
summary: >-
  The return value is the result of a call. A return statement ends the function and gives the value to the caller, and some languages also return the value of the last expression of the body.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [John Backus, Peter Naur]
  note: In ALGOL 60 a procedure gave its value by an assignment to the procedure identifier, and Pascal kept this rule. C and later languages use a return statement with a value.
see_also: [function, function-call, procedure-concept, function-signature, statements-and-expressions, block, side-effect, use-a-tuple]
sources:
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.17
  title: Blocks, Statements, and Patterns, The Java Language Specification, Java SE 25 Edition
  locator: 14.17 The return Statement
- url: https://doc.rust-lang.org/reference/expressions/return-expr.html
  title: Return expressions, The Rust Reference
  locator: expr.return.intro; expr.return.behavior
- url: https://docs.python.org/3/reference/simple_stmts.html#the-return-statement
  title: Simple statements, The Python Language Reference
  locator: 7.6 The return statement
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/return
  title: return, JavaScript reference, MDN Web Docs
  locator: Syntax; Description; Automatic semicolon insertion
- url: https://doc.rust-lang.org/book/ch03-03-how-functions-work.html
  title: Functions, The Rust Programming Language
  locator: Functions with Return Values
- url: https://www.typescriptlang.org/docs/handbook/2/functions.html
  title: More on Functions, The TypeScript Handbook
  locator: Generic Functions, Inference
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.4.7
  title: Classes, The Java Language Specification, Java SE 25 Edition
  locator: 8.4.7 Method Body
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/methods
  title: Methods, C# Programming Guide
  locator: Return values
- url: https://en.cppreference.com/w/c/language/return.html
  title: return statement, C reference, cppreference.com
  locator: Explanation
- url: https://en.cppreference.com/w/cpp/language/return.html
  title: return statement, C++ reference, cppreference.com
  locator: Notes
- url: https://go.dev/ref/spec#Return_statements
  title: The Go Programming Language Specification
  locator: Return statements
- url: https://www.gnu.org/software/bash/manual/html_node/Bourne-Shell-Builtins.html
  title: Bourne Shell Builtins, Bash Reference Manual
  locator: return
- url: https://www.masswerk.at/algol60/report.htm
  title: Revised Report on the Algorithmic Language Algol 60
  locator: 5.4.4 Values of function designators
- url: https://standardpascaline.org/iso7185.html
  title: ISO 7185 Pascal
  locator: 6.6.2 Function-declarations; 6.7.3 Function-designators
---

## Idea

A call gives back one value to the place of the call. This value is the return value, or the result, of the call. The function chooses it, usually with a `return` statement, and the caller uses it as the value of the call expression.

```python run id=idea
def absolute(n):
    if n < 0:
        return -n
    return n


print(absolute(-7))
print(absolute(5) + 1)
```

```output for=idea
7
6
```

The first call takes the first `return`, and the second call takes the second one. In each case the call expression `absolute(...)` has the value that the `return` statement gave.

## How it works

1. **The return statement.** The body runs until it reaches a `return` statement. Java says that "A return statement returns control to the invoker of a method"[^1], and that "In the case of a return statement with value Expression, the value of the Expression becomes the value of the invocation"[^1].
2. **The value.** The function evaluates the expression after `return`, then puts the value where the caller can get it. Rust describes this: "Evaluating a return expression moves its argument into the designated output location for the current function call, destroys the current function activation frame, and transfers control to the caller frame."[^2]
3. **The end of the body.** A body can also end without a `return`. Then the language decides the result. Python substitutes `None`[^3], JavaScript gives `undefined`[^4], and Rust takes the value of the last expression[^5]. In C, C++, Java and C#, a function with a result type must return a value. [[procedure-concept]] describes functions with no result.
4. **The caller.** The call expression gets the value. The caller can store it, pass it to another call or ignore it.

A `return` statement also stops the body. MDN says: "When a return statement is used in a function body, the execution of the function is stopped."[^4] This lets a function return early, for example when an argument is not valid.

## In languages

- **Python.** "If an expression list is present, it is evaluated, else None is substituted. return leaves the current function call with the expression list (or None) as return value."[^3] `return a, b` returns one tuple.
- **JavaScript.** `return expression;` gives back the value. For the expression, MDN says: "If omitted, undefined is returned."[^4]
- **TypeScript.** The return type is written after the parameter list. If the code does not write it, the compiler infers it: "The type was inferred - chosen automatically - by TypeScript."[^6]
- **Java.** A method with a result type must return a value on each path. "If a method is declared to have a return type" and its body can end without a `return`, "a compile-time error occurs"[^7].
- **C#.** "If the return type (the type listed before the method name) is not void, the method can return the value by using the return statement."[^8] "The return keyword also stops the execution of the method."[^8]
- **C.** `return expression;` "Terminates the current function"[^9] and gives the value. The language does not force a return on each path. "Reaching the end of any other value-returning function is undefined behavior if the result of the function is used in an expression"[^9].
- **C++.** C++ has the same rule as C, but stricter: "Flowing off the end of a value-returning function, except the main function and specific coroutines" and without a return statement "is undefined behavior"[^10].
- **Go.** The Go specification says that a `return` statement in a function F "terminates the execution of F, and optionally provides one or more result values"[^11]. A Go function can return several values, and the result parameters can have names.
- **Rust.** Rust is based on expressions. "You can return early from a function by using the return keyword and specifying a value, but most functions return the last expression implicitly."[^5] "If you add a semicolon to the end of an expression, you turn it into a statement, and it will then not return a value."[^5]
- **Bash.** `return n` gives back only an exit status. It means "Stop executing a shell function or sourced file and return the value n to its caller."[^12] A function gives back text by printing it, and the caller captures the text with `$( )`.

## Variations and related ideas

- **Implicit returns.** In Rust, the last expression of a block is its value, so a function often has no `return` statement[^5]. Arrow functions in JavaScript with an expression body return that expression.
- **Early returns and guard clauses.** A function can test for a bad case at the top and return at once. The rest of the body then handles only the normal case.
- **Several results.** Go returns several values at once[^11]. Python returns a tuple. Other languages return a record or an object. See [[use-a-tuple]].
- **No result.** A function that gives back nothing is a procedure. Its result type is `void` in C, Java and C#, or the unit type in Rust. See [[procedure-concept]].
- **Results through parameters.** C and C# can give results through pointer, `ref` or `out` parameters. See [[parameter-passing]].
- **Errors instead of results.** A function that cannot compute a result can throw an exception or return an error value, such as an `error` in Go or a `Result` in Rust.

## Pitfalls

### A line break after return in JavaScript

JavaScript inserts a semicolon at the end of a line after `return`. MDN gives the case `return a + b` with a line break after `return`, and says: "This makes the function return undefined and the a + b expression is never evaluated."[^4]

```javascript wrong id=javascript-return-newline
function total(a, b) {
  return
    a + b;
}

console.log(total(2, 3));
```

```output for=javascript-return-newline
undefined
```

Put the start of the expression on the same line as `return`. To split a long expression, put it in parentheses that start on the `return` line.

### A return inside a loop that ends too early

A `return` ends the whole function, not only the loop. A `return` in both branches of a test in a loop ends the function in the first pass:

```python wrong id=python-return-in-loop
def has_negative(numbers):
    for n in numbers:
        if n < 0:
            return True
        else:
            return False


print(has_negative([4, -2, 7]))
```

```output for=python-return-in-loop
False
```

`return` "leaves the current function call"[^3], so the function looked only at 4. Return `True` in the loop, and return `False` after the loop, when all the numbers are checked.

### A return inside a callback

A `return` statement stops the function that contains it[^4]. Inside a callback, such as the arrow function that `forEach` calls, `return` ends only the callback, not the outer function:

```javascript wrong id=javascript-return-in-callback
function firstNegative(numbers) {
  numbers.forEach((n) => {
    if (n < 0) {
      return n;
    }
  });
  return null;
}

console.log(firstNegative([4, -2, 7]));
```

```output for=javascript-return-in-callback
null
```

The `return n` gave -2 back to `forEach`, which ignored it, and then the outer function returned `null`. Use a `for...of` loop, so that `return` belongs to the outer function, or use `numbers.find((n) => n < 0)`.

## Cost

A return costs little: the function puts the value in a register or in a place that the caller gave, and jumps back. A large value, such as a big structure, can cost a copy. Rust moves the value into "the designated output location for the current function call"[^2].

## History

ALGOL 60 had no return statement. A procedure that gave a value assigned the value to its own name: "there must, within the procedure declaration body, occur one or more explicit assignment statements with the procedure identifier in a left part"[^13]. Pascal kept this rule. "The function-block shall contain at least one assignment-statement such that the function-identifier of the assignment-statement is associated with the block"[^14], and a call "shall yield the value of the result of the activation upon completion of the algorithm of the activation"[^14].

C used a `return` statement with an expression, which ends the function and gives the value at the same time[^9]. Java, C#, JavaScript, Python and Go use the same statement. Rust keeps `return` but makes the last expression of the body the result[^5].

## References
