---
tag: YJE5C1
kind: concept
slug: function-call
name: Function call
aliases: [call, invocation, function invocation, subroutine call, method call]
area: functions-and-abstraction/functions-and-procedures
level: intro
summary: >-
  A function call runs the body of a function with a list of arguments. The program evaluates the arguments, binds them to the parameters, runs the body and goes back with the result.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1945
  people: [Alan Turing, David Wheeler]
  note: Turing's 1945 proposal for the ACE described a stack of return addresses. Early machines such as the EDSAC had no call instruction, and later machines added a jump to subroutine instruction that saved the return address.
see_also: [function, procedure-concept, return-value, function-signature, parameter-passing, stack-and-heap, recursion, statements-and-expressions]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Functions
  title: Functions, JavaScript Guide, MDN Web Docs
  locator: Calling functions; Function parameters
- url: https://go.dev/ref/spec#Calls
  title: The Go Programming Language Specification
  locator: Calls
- url: https://docs.python.org/3/reference/expressions.html#calls
  title: Expressions, The Python Language Reference
  locator: 6.3.4 Calls; 6.16 Evaluation order
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.12.4
  title: Expressions, The Java Language Specification, Java SE 25 Edition
  locator: 15.7.4 Argument Lists are Evaluated Left-to-Right; 15.12.4 Run-Time Evaluation of Method Invocation
- url: https://en.cppreference.com/w/cpp/language/functions.html
  title: Functions, C++ reference, cppreference.com
  locator: the introduction
- url: https://www.typescriptlang.org/docs/handbook/2/functions.html
  title: More on Functions, The TypeScript Handbook
  locator: Optional Parameters
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/methods
  title: Methods, C# Programming Guide
  locator: the introduction; Method access
- url: https://en.cppreference.com/w/c/language/operator_other.html
  title: Other operators, C reference, cppreference.com
  locator: Function call
- url: https://doc.rust-lang.org/reference/expressions/call-expr.html
  title: Call expressions, The Rust Reference
  locator: expr.call.intro
- url: https://www.gnu.org/software/bash/manual/html_node/Shell-Functions.html
  title: Shell Functions, Bash Reference Manual
  locator: 3.3 Shell Functions
- url: https://gcc.gnu.org/onlinedocs/gcc/Inline.html
  title: An Inline Function is As Fast As a Macro, Using the GNU Compiler Collection
  locator: 6.9
- url: https://en.wikipedia.org/wiki/Function_(computer_programming)
  title: Function (computer programming), Wikipedia
  locator: History
---

## Idea

A call runs a function. It names the function, gives it a list of arguments, waits for the body to finish and then takes the result. The rest of the program goes on from the place of the call.

```python run id=idea
def rectangle_area(width, height):
    print("computing", width, "by", height)
    return width * height


area = rectangle_area(3, 4)
print("area is", area)
```

```output for=idea
computing 3 by 4
area is 12
```

The definition did nothing when Python read it. The call `rectangle_area(3, 4)` ran the body, and the program printed the second line only after the body returned. MDN says: "Defining a function does not execute it. Defining it names the function and specifies what to do when the function is called. Calling the function actually performs the specified actions with the indicated parameters."[^1]

## How it works

The Go specification gives the steps of a call in one paragraph[^2], and the other languages follow the same plan.

1. **Evaluate the function and the arguments.** "In a function call, the function value and arguments are evaluated in the usual order."[^2] Python says: "All argument expressions are evaluated before the call is attempted."[^3]
2. **Make new storage for the call.** "After they are evaluated, new storage is allocated for the function's variables, which includes its parameters and results."[^2] Java calls this storage an activation frame. It contains "the target reference (if any) and the argument values (if any), as well as enough space for the local variables"[^4] and bookkeeping data such as the place to go back to. [[stack-and-heap]] describes where this storage lives.
3. **Bind the arguments to the parameters.** "Then, the arguments of the call are passed to the function, which means that they are assigned to their corresponding function parameters, and the called function begins execution."[^2] In C++, "the parameters are initialized from the arguments (either provided at the place of call or defaulted) and the statements in the function body are executed"[^5]. [[parameter-passing]] describes how a language copies or shares each argument.
4. **Run the body.** The body runs with its own local variables. It can call other functions, and each of those calls gets its own storage.
5. **Return.** "The return parameters of the function are passed back to the caller when the function returns."[^2] The storage of the call is freed, and the program goes on after the call. The value of the call expression is the result. [[return-value]] describes this step.

Each active call has its own storage, so a function can call itself, and each call has its own copy of the parameters. See [[recursion]].

## In languages

- **Python.** A call is a primary followed by an argument list in parentheses. "A call calls a callable object (e.g., a function) with a possibly empty series of arguments"[^3]. Python also has keyword arguments, such as `area(width=3, height=4)`.
- **JavaScript.** A call writes the arguments in parentheses after the function. JavaScript does not check the number of arguments. "In JavaScript, parameters of functions default to undefined."[^1] "In JavaScript, if you call a function with more arguments than there are parameters, the extra arguments are simply ignored."[^6]
- **TypeScript.** The syntax is the same as JavaScript, but the compiler checks the number and the types of the arguments. A missing argument gives an error such as "Expected 1 arguments, but got 0."[^6]
- **Java.** A call is a method invocation. A static method is called by its name or as `ClassName.method(args)`, and an instance method as `object.method(args)`. At run time, "a new activation frame is created, synchronization is performed if necessary, and control is transferred to the method code"[^4].
- **C#.** "A program causes the statements to be executed by calling the method and specifying any required method arguments."[^7] "Calling a method on an object is like accessing a field."[^7]
- **C.** A call has the form `expression ( argument-list )`, and the argument list is optional[^8]. The function "is executed, and the value it returns becomes the value of the function call expression (if the function returns void, the function call expression is a void expression)"[^8].
- **C++.** The call syntax is the same as in C. The arguments can be "either provided at the place of call or defaulted"[^5], because C++ has default arguments.
- **Go.** A call is `f(args)` or `value.Method(args)`. A Go function can return several results, and a call can pass them all to another function at once.
- **Rust.** "A call expression calls a function. The syntax of a call expression is an expression, called the function operand, followed by a parenthesized comma-separated list of expression, called the argument operands."[^9] A method call uses `value.method(args)`.
- **Bash.** A call is a command. The program writes the function name and the arguments with spaces and no parentheses: `rectangle_area 3 4`. "When a function is executed, the arguments to the function become the positional parameters during its execution"[^10], so the body reads them as `$1`, `$2` and so on.

## Variations and related ideas

- **Calls as expressions and as statements.** In most languages, a call is an expression with a value, and a call on its own line is an expression statement that throws its value away. A call of a [[procedure-concept]] is used as a statement.
- **Positional and keyword arguments.** Most calls match arguments to parameters by position. Python can also match them by name, with keyword arguments such as `width=3`[^3].
- **Method calls.** A method call such as `list.append(x)` gives the object as an extra argument. See [[method-concept]].
- **Indirect calls.** A program can call a function through a variable, a pointer or a reference. The program knows the function only at run time. See [[higher-order-functions]].
- **Order of evaluation.** "Python evaluates expressions from left to right."[^3] In Java, "Each argument expression appears to be fully evaluated before any part of any argument expression to its right."[^4] In C, the evaluations of the function and of the arguments are "unsequenced with respect to each other (but there is a sequence point before the body of the function begins executing)"[^8].

## Pitfalls

### Arguments are evaluated before the call

A call evaluates all its arguments first, even when the body does not use one of them. "All argument expressions are evaluated before the call is attempted."[^3]

```python wrong id=python-eager-arguments
def log(enabled, message):
    if enabled:
        print(message)


def build_report():
    print("building a large report")
    return "report"


log(False, build_report())
```

```output for=python-eager-arguments
building a large report
```

The program did the work of `build_report` and then threw the result away. Pass a function that builds the message, and call it only when it is needed, or test the condition before the call.

### A missing argument in JavaScript

JavaScript does not check the number of arguments. A missing argument is `undefined`[^1], and arithmetic with `undefined` gives `NaN`:

```javascript wrong id=javascript-missing-argument
function rectangleArea(width, height) {
  return width * height;
}

console.log(rectangleArea(3));
```

```output for=javascript-missing-argument
NaN
```

TypeScript rejects this call at compile time[^6]. In JavaScript, give the parameter a default value, or check it in the body.

### Too many nested calls

Each active call uses storage. If calls nest too deep, for example in a recursion with no end, that storage runs out. Java says: "If there is not sufficient memory available to create such an activation frame, a StackOverflowError is thrown."[^4]

```java wrong id=java-stack-overflow
public class Main {
    static int countDown(int n) {
        return countDown(n - 1) + 1;
    }

    public static void main(String[] args) {
        try {
            countDown(10);
        } catch (StackOverflowError e) {
            System.out.println("StackOverflowError");
        }
    }
}
```

```output for=java-stack-overflow
StackOverflowError
```

`countDown` has no base case, so each call makes one more call. Add a base case, such as `if (n == 0) return 0;`, or use a loop for very deep work.

## Cost

A call costs more than the code of its body. The program evaluates the arguments, makes the new storage, saves the place to go back to, jumps to the body and at the end jumps back. For a small body that runs in a tight loop, this extra work can be larger than the body. Compilers can remove it by inlining. GCC says that by declaring a function inline, "you can direct GCC to make calls to that function faster", and that "One way GCC can achieve this is to integrate that function’s code into the code for its callers"[^11].

A call also uses memory for its storage until it returns. A chain of nested calls uses the storage of all the active calls at once, so the depth of the calls limits how deep a recursion can go[^4].

## History

Early computers had no instruction for a call. Some of "The earliest computers, such as the Manchester Baby," had no call instruction, so the program had to store the place to go back to by itself[^12]. "Alan Turing had discussed subroutines in a paper of 1945 on design proposals for the NPL ACE, going so far as to invent the concept of a return address stack."[^12]

Later machines added instructions for calls. "Another advance was the jump to subroutine instruction, which combined the saving of the return address with the calling jump, thereby minimizing overhead significantly."[^12] Wikipedia names the DEC PDP-6 of 1964 as "one of the first accumulator-based machines to have a subroutine call instruction that saved the return address in a stack addressed by an accumulator or index register"[^12]

## References
