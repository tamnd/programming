---
tag: XJRWGX
kind: concept
slug: procedure-concept
name: Procedure and subroutine
aliases: [procedure, subroutine, void function, command]
area: functions-and-abstraction/functions-and-procedures
level: core
summary: >-
  A procedure, or subroutine, is a named piece of code that a program calls for an effect, such as output or a change of state, not for a result. Many languages write it as a function with no result.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1950
  people: [David Wheeler, Maurice Wilkes, Stanley Gill, Bertrand Meyer]
  note: David Wheeler developed a way to call subroutines on the EDSAC in the 1950s. ALGOL 60 used the word procedure for all subprograms, and Pascal and Ada keep procedures and functions apart. Bertrand Meyer named the rule of command query separation.
see_also: [function, function-call, return-value, side-effect, method-concept, parameter-passing, statements-and-expressions, block]
sources:
- url: https://en.wikipedia.org/wiki/Function_(computer_programming)
  title: Function (computer programming), Wikipedia
  locator: the introduction; Terminology
- url: http://www.ada-auth.org/standards/22rm/html/RM-6.html
  title: Ada Reference Manual (Ada 2022), 6 Subprograms
  locator: paragraph 1
- url: https://docs.python.org/3/reference/simple_stmts.html#expression-statements
  title: Simple statements, The Python Language Reference
  locator: 7.1 Expression statements
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/return
  title: return, JavaScript reference, MDN Web Docs
  locator: Syntax
- url: https://www.typescriptlang.org/docs/handbook/2/functions.html
  title: More on Functions, The TypeScript Handbook
  locator: Other Types to Know About, void; Return type void
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.4.5
  title: Classes, The Java Language Specification, Java SE 25 Edition
  locator: 8.4.5 Method Result
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.17
  title: Blocks, Statements, and Patterns, The Java Language Specification, Java SE 25 Edition
  locator: 14.17 The return Statement
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/void
  title: void, C# reference
  locator: the introduction
- url: https://en.cppreference.com/w/c/language/function_declaration.html
  title: Function declarations, C reference, cppreference.com
  locator: Explanation
- url: https://en.cppreference.com/w/cpp/language/return.html
  title: return statement, C++ reference, cppreference.com
  locator: Explanation
- url: https://go.dev/ref/spec#Return_statements
  title: The Go Programming Language Specification
  locator: Return statements
- url: https://doc.rust-lang.org/reference/items/functions.html
  title: Functions, The Rust Reference
  locator: items.fn.implicit-return
- url: https://doc.rust-lang.org/std/primitive.unit.html
  title: Primitive Type unit, The Rust Standard Library
  locator: the introduction
- url: https://www.gnu.org/software/bash/manual/html_node/Shell-Functions.html
  title: Shell Functions, Bash Reference Manual
  locator: 3.3 Shell Functions
- url: https://standardpascaline.org/iso7185.html
  title: ISO 7185 Pascal
  locator: 6.6.1 Procedure-declarations; 6.6.2 Function-declarations
- url: https://martinfowler.com/bliki/CommandQuerySeparation.html
  title: Command Query Separation, Martin Fowler
  locator: the whole page
- url: https://en.wikipedia.org/wiki/Wheeler_jump
  title: Wheeler Jump, Wikipedia
  locator: the introduction
- url: https://www.masswerk.at/algol60/report.htm
  title: Revised Report on the Algorithmic Language Algol 60
  locator: 5.4 Procedure declarations
---

## Idea

A procedure is a named piece of code that a program calls to do something, not to compute a value. It can print a line, write a file, add an item to a list or change a variable. The call is a step of the program, and the program uses no result from it.

```python run id=idea
def add_item(cart, item):
    cart.append(item)
    print("added", item)


cart = []
add_item(cart, "pen")
add_item(cart, "book")
print(cart)
```

```output for=idea
added pen
added book
['pen', 'book']
```

`add_item` has no `return` statement. The program calls it for its two effects: it changes the list, and it prints a line.

The words differ between languages. Wikipedia lists "function (also procedure, method, subroutine, routine, or subprogram)" as names for one idea[^1]. Some languages use two words. Ada says: "There are two forms of subprogram: procedures and functions. A procedure call is a statement; a function call is an expression and returns a value."[^2] Other languages use one construct. Python calls a procedure "a function that returns no meaningful result; in Python, procedures return the value None"[^3].

## How it works

1. **The definition.** The definition of a procedure has a name, parameters and a body, as a [[function]] has. It has no result type, or it has a special result type that means "no value", such as `void` or the unit type.
2. **The call.** The program writes the call as a statement on its own line. In Ada, "A procedure call is a statement"[^2]. In C, Java or Go, a call is an expression, and an expression statement throws its value away.
3. **The effect.** The body runs. The procedure does its work through effects: it changes data that the caller can see, such as a list that the caller passed, or it does input and output. [[side-effect]] describes these effects.
4. **The return.** The body ends, or it runs a `return` statement with no value. Control goes back to the statement after the call. [[return-value]] describes the return of a value.

## In languages

- **Python.** Python has only functions. A function with no `return` statement, or with a bare `return`, gives back `None`, so a procedure is a function that returns `None`[^3]. The interactive interpreter shows no output for `None`, "so that procedure calls do not cause any output"[^3].
- **JavaScript.** A function with no `return` statement, or a `return` with no expression, gives back `undefined`. MDN says of the expression of `return`: "If omitted, undefined is returned."[^4]
- **TypeScript.** The result type `void` marks a function that gives back no value. The TypeScript Handbook says that "void represents the return value of functions which don’t return a value"[^5].
- **Java.** A method that gives back no value has the result type `void`. The JLS says that the result of a method declaration "uses the keyword void to indicate that the method does not return a value"[^6]. A `return` with no value is allowed only in such a method: "It is a compile-time error if the return target of a return statement with no value is a method, and that method is not declared void."[^7]
- **C#.** "Use void as the return type of a method or a local function to specify that the method doesn't return a value."[^8]
- **C.** The return type of a function "must be a non-array object type or the type void"[^9]. A `void` function can end with `return;` or by reaching the end of its body.
- **C++.** C++ also uses `void`. In a `return` statement, "The expression is optional in functions whose return type is (possibly cv-qualified) void"[^10].
- **Go.** A Go function can have no result list. In a function without a result type, a `return` statement "must not specify any result values"[^11].
- **Rust.** A Rust function always gives back a value. "If the output type is not explicitly stated, it is the unit type"[^12]. The unit type `()` "has exactly one value (), and is used when there is no other meaningful value that could be returned"[^13].
- **Bash.** A Bash function gives back only an exit status. "If return is supplied a numeric argument, that is the function’s return status; otherwise the function’s return status is the exit status of the last command executed before the return."[^14] A Bash function that only prints text or changes variables is a procedure in practice.

## Variations and related ideas

- **Procedures and functions as two kinds.** Pascal has separate procedure declarations and function declarations[^15]. Ada has `procedure` and `function`[^2]. Wikipedia says that C, C++ and Rust "only use the term" function "irrespective of whether they return a value or not", and that ALGOL 60 and PL/I "only use the word procedure"[^1].
- **Void and unit.** A language with types marks a procedure with a special type. `void` in C, C++, Java and C# means that there is no value. The unit type in Rust is a real type with one value, so a procedure is a function that returns that value.
- **Command query separation.** Bertrand Meyer gave a design rule for methods. Martin Fowler describes it: "Queries: Return a result and do not change the observable state of the system (are free of side effects)." and "Commands: Change the state of a system but do not return a value."[^16] A command is a procedure.
- **Methods.** A procedure that belongs to a class is a method with no result, such as a `void` method in Java. See [[method-concept]].
- **Output parameters.** A procedure can give data back through its parameters, for example through a pointer in C or a `ref` or `out` parameter in C#. See [[parameter-passing]].

## Pitfalls

### Using the result of a procedure

A procedure in Python returns `None`. If a program stores the result of a procedure call in a variable, it loses the data that the variable held:

```python wrong id=python-procedure-result
def add_item(cart, item):
    cart.append(item)


cart = ["pen"]
cart = add_item(cart, "book")
print(cart)
```

```output for=python-procedure-result
None
```

`add_item` changes the list and returns `None`[^3]. The assignment then puts `None` in `cart`. Call the procedure as a statement: `add_item(cart, "book")`.

### A void function type in TypeScript can return a value

In TypeScript, `void` as the result of a function type does not stop a function from returning a value. The Handbook says: "Contextual typing with a return type of void does not force functions to not return something."[^5]

```typescript wrong id=typescript-void-callback
type Logger = () => void;

const log: Logger = () => true;

const result = log();
console.log(result);
```

```output for=typescript-void-callback
true
```

The type checker accepts the arrow function, and at run time the call gives back `true`. Code must not use the result of a `void` function, because the type says nothing about it.

### The status of the last command in Bash

A Bash function returns the exit status of its last command[^14]. A procedure that ends with a test returns failure when the test is false, even when the procedure did its job:

```bash wrong id=bash-procedure-status
DEBUG=0

log_debug() {
  [ "$DEBUG" = 1 ] && echo "debug: $1"
}

log_debug "start"
echo "status $?"
```

```output for=bash-procedure-status
status 1
```

The test is false, so the `&&` list fails, and the function returns 1. With `set -e`, or in an `if`, the caller treats this as an error. End the procedure with `return 0`, or use an `if` statement.

## Cost

A procedure call costs the same as a function call: the arguments, the jump to the body and the jump back. No result value goes back.

The larger cost is in reasoning. A procedure does its work through effects, so its effects decide the order in which a program can call it. Fowler says of queries: "This is because you can use queries in many situations with much more confidence, introducing them anywhere, changing their order. You have to be more careful with modifiers."[^16]

## History

Subroutines came before the words function and procedure. On the EDSAC, David Wheeler developed a way to call a subroutine, the Wheeler Jump, "while working on the pioneering EDSAC machine in the 1950s"[^17]. The EDSAC "lacked a suitable processor register or a hardware stack that might allow the return address to be easily stored"[^17], so the subroutine wrote the return address into its own last instruction.

The ALGOL 60 report used the word procedure for all subprograms: "A procedure declaration serves to define the procedure associated with a procedure identifier"[^18]. Pascal and Ada split the idea into procedures and functions[^15][^2]. C used only functions and gave a procedure the return type `void`[^9], and Java, C# and C++ do the same.

Martin Fowler says that the term command query separation "was coined by Bertrand Meyer" in his book Object Oriented Software Construction[^16].

## References
