---
tag: ZAHKXJ
kind: concept
slug: compile-time-and-run-time-errors
name: Compile-time and run-time errors
aliases: [compile-time error, run-time error, runtime error, static error, dynamic error, syntax error, link error]
area: errors-and-control/error-concepts
level: intro
summary: >-
  A compile-time error is found before the program runs, such as a syntax or type error. A run-time error is found only when the bad operation runs, such as an index past the end of an array.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1978
  people: [Robin Milner]
  note: Milner's paper "A theory of type polymorphism in programming" gave a compile-time type-checking algorithm for the language ML, and a theorem that states that well-typed programs cannot go wrong.
see_also: [error-concept, static-and-dynamic-typing, type-system, undefined-behavior, panics, exception-handling, recoverable-and-unrecoverable-errors]
sources:
- url: https://docs.python.org/3.15/tutorial/errors.html
  title: Errors and Exceptions, The Python Tutorial
  locator: 8.1 Syntax Errors; 8.2 Exceptions
- url: https://www.typescriptlang.org/docs/handbook/2/basic-types.html
  title: The Basics, TypeScript Handbook
  locator: Static type-checking
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/SyntaxError
  title: SyntaxError, MDN Web Docs
  locator: Description
- url: https://doc.rust-lang.org/book/ch03-02-data-types.html
  title: Data Types, The Rust Programming Language
  locator: Chapter 3.2
- url: https://en.cppreference.com/w/cpp/language/translation_phases.html
  title: Phases of translation, cppreference.com
  locator: Phase 9 Linking
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/TypeError
  title: TypeError, MDN Web Docs
  locator: Description
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-11.html
  title: Chapter 11. Exceptions, The Java Language Specification, Java SE 25 Edition
  locator: 11.2 Compile-Time Checking of Exceptions
- url: https://mypy.readthedocs.io/en/stable/getting_started.html
  title: Getting started, mypy documentation
  locator: Installing and running mypy
- url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/ArrayIndexOutOfBoundsException.html
  title: ArrayIndexOutOfBoundsException, Java SE 21 API
  locator: Class description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.indexoutofrangeexception
  title: IndexOutOfRangeException Class, .NET API
  locator: Definition
- url: https://go.dev/ref/spec#Index_expressions
  title: The Go Programming Language Specification
  locator: Index expressions; Import declarations
- url: https://doc.rust-lang.org/book/ch08-01-vectors.html
  title: Storing Lists of Values with Vectors, The Rust Programming Language
  locator: Reading Elements of Vectors
- url: https://en.cppreference.com/w/c/language/behavior.html
  title: Undefined behavior, cppreference.com (C)
  locator: Description; Access out of bounds
- url: https://en.cppreference.com/w/cpp/container/vector/at.html
  title: std::vector::at, cppreference.com
  locator: Description; Exceptions
- url: https://en.cppreference.com/w/cpp/container/vector/operator_at.html
  title: std::vector::operator[], cppreference.com
  locator: Description; Notes
- url: https://www.gnu.org/software/bash/manual/bash.html#Aliases
  title: Bash Reference Manual
  locator: 6.6 Aliases; 4.3.1 The Set Builtin (-n)
- url: https://www.typescriptlang.org/tsconfig/noUncheckedIndexedAccess.html
  title: noUncheckedIndexedAccess, TSConfig Reference
  locator: Description
- paper: doi:10.1016/0022-0000(78)90014-4
  title: A theory of type polymorphism in programming (Milner, Journal of Computer and System Sciences 17, 1978)
  locator: Abstract; Section 1 Introduction
---

## Idea

An error in a program can be found at two times. A compile-time error is found by a tool before the program runs: the compiler, the parser or a type checker. A run-time error is found only when the program runs the bad operation, and only for the input that causes it.

Python shows the two kinds in its tutorial. It calls the first kind syntax errors, "also known as parsing errors"[^1]. For the second kind it says: "Even if a statement or expression is syntactically correct, it may cause an error when an attempt is made to execute it. Errors detected during execution are called exceptions"[^1].

```python run id=idea
source = "total = (1 + 2"
try:
    compile(source, "<demo>", "exec")
except SyntaxError as e:
    print(f"found before it runs: {type(e).__name__}")

values = [10, 20, 30]
for i in [1, 5]:
    try:
        print(f"values[{i}] = {values[i]}")
    except IndexError as e:
        print(f"found when it runs: {type(e).__name__}")
```

```output for=idea
found before it runs: SyntaxError
values[1] = 20
found when it runs: IndexError
```

The call to `compile` finds the syntax error, and no line of `source` runs. The index 5 is found only on the second turn of the loop, after the first turn has run.

An error that the compiler finds is cheap to fix: the programmer sees it at once, and no user sees it. An error found at run time can stay hidden until a rare input arrives. So languages and tools try to move errors from run time to compile time. The TypeScript handbook gives this as the reason for a static type-checker[^2].

## How it works

A compiler or an interpreter reads a program in steps, and each step can find a kind of error.

1. The parser reads the text and finds syntax errors. In JavaScript, a `SyntaxError` "is thrown when the JavaScript engine encounters tokens or token order that does not conform to the syntax of the language when parsing code"[^3].
2. A type checker finds an operation on a value of the wrong type. Rust "is a statically typed language, which means that it must know the types of all variables at compile time"[^4]. The TypeScript handbook says: "Ideally, we could have a tool that helps us find these bugs before our code runs. That’s what a static type-checker like TypeScript does."[^2]
3. The linker joins the parts of the program. In C++, "Translated translation units are combined, and all external entity references are resolved"[^5], so a function that is declared but has no definition is an error at this step.
4. The program runs. Some errors depend on values that are known only now: an index past the end, a division by zero, a file that does not exist. A language that has no static types finds type errors here too: JavaScript throws a `TypeError` "when an operation could not be performed, typically (but not exclusively) when a value is not of the expected type"[^6].

Some checks are a mix of the two times. Java checks at compile time that each checked exception is caught or declared; the JLS calls this "compile-time checking for the presence of exception handlers"[^7]. The exception itself still happens at run time.

## In languages

All ten languages find syntax errors before the code that has the error runs. They differ in how many other errors they find before the program runs, and in what they do at run time.

- Python finds a syntax error when it parses the code, before that code runs[^1]. It finds type errors at run time. A separate tool, mypy, "will type check your code statically: this means that it will check for errors without ever running your code"[^8].
- JavaScript finds syntax errors when it parses a script, and type errors at run time as a `TypeError`[^3][^6]. TypeScript adds a static type checker[^2], but its types are erased from the output: "Type annotations never change the runtime behavior of your program"[^2].
- Java and C# find syntax and type errors at compile time. At run time, Java throws `ArrayIndexOutOfBoundsException` "to indicate that an array has been accessed with an illegal index"[^9], and C# throws `IndexOutOfRangeException` for "an index that is outside its bounds"[^10].
- Go finds syntax and type errors at compile time. Its spec also states that "a constant index must be in range" for an array, and that "if x is out of range at run time, a run-time panic occurs"[^11].
- Rust finds syntax, type and borrowing errors at compile time. For an index past the end, the book says that `[]` "will cause the program to panic", and that `get` "returns None without panicking"[^12].
- C and C++ find syntax and type errors at compile time, and the linker finds missing definitions[^5]. They do not test an array index at run time. In C, "memory accesses outside of array bounds" are an example of undefined behavior[^13]. In C++, `std::vector::at` throws `std::out_of_range` for a bad index[^14], but for `operator[]`, "No bounds checking is performed"[^15].
- Bash reads "at least one complete line of input, and all lines that make up a compound command, before executing any of the commands on that line or the compound command"[^16]. So a syntax error at the end of a script is found only after the commands before it have run.

## Variations and related ideas

- **Static and dynamic typing.** A statically typed language finds type errors at compile time, and a dynamically typed language finds them at run time. See static-and-dynamic-typing.
- **Stricter options.** A checker can find more errors when an option tells it to. For example, the TypeScript option `noUncheckedIndexedAccess` "will add undefined to any un-declared field in the type"[^17], so the code must test for a missing value.
- **Types that make a run-time check visible.** Rust's `get` returns an `Option`, so the compiler makes the caller handle the missing item[^12]. The run-time check stays, but the compiler checks that the code handles its result.
- **Undefined behavior.** C and C++ do not report some run-time errors at all. The result is undefined behavior, not an error[^13]. See undefined-behavior.

## Pitfalls

### A run-time error that the tests do not reach

A run-time error happens only on the path and the input that cause it. A test that does not use that input does not find it.

```python wrong id=python-untested-path
def label(count):
    if count == 1:
        return "1 item"
    return str(count) + " items"

def report(count):
    if count < 0:
        return "bad count: " + count
    return label(count)

print(report(3))
```

```output for=python-untested-path
3 items
```

The program runs and prints the correct result. But the line for a negative count adds a string and an integer, and Python finds this only when `count` is negative: it raises a `TypeError` at run time. A static type checker such as mypy reports this line without running the code[^8].

### An unchecked index in C

C does not test an index at run time, so an index past the end is not reported as an error[^13]. The program can print a wrong value, or crash, or seem to work.

For example, the loop `for (int i = 0; i <= 3; i++)` over an array of three items reads `items[3]`, which is past the end. The behavior of the program is undefined[^13]. "Compilers are not required to diagnose undefined behavior"[^13], and the program does not have to fail.

### A late syntax error in Bash

Because Bash reads and runs one line or compound command at a time[^16], the commands before a syntax error run.

```bash wrong id=bash-late-syntax-error
script='echo "step 1 done"
echo "step 2 done"
if [ 1 -eq 1 ]; then echo "step 3"'
bash -c "$script" 2>/dev/null
echo "status $?"
```

```output for=bash-late-syntax-error
step 1 done
step 2 done
status 2
```

Steps 1 and 2 run before Bash finds that the `if` has no `fi`. A script that changes files can stop halfway. The option `-n` tells Bash to "Read commands but do not execute them", and the manual says that this "may be used to check a script for syntax errors"[^16].

## Cost

A compile-time check costs time when the program is built, and nothing when it runs. Milner wrote that once the type checker has accepted a program, "code may be generated which assumes that no objects carry their types at run-time. This is widely accepted as yielding efficient object code"[^18].

A run-time check costs a test each time the operation runs. C++ gives the choice: `at` tests the index[^14], and `operator[]` does not[^15].

The other cost is when a person finds the error. A compile-time error is found by the programmer. A run-time error can be found by a user, after the program has shipped.

## History

In 1978, Robin Milner published a paper on type polymorphism in programming. The paper gives "a compile time type-checking algorithm" and a theorem that "well-type programs cannot “go wrong”"[^18]. The type checker was part of the language ML, and Milner wrote that it "has proved to be a valuable filter which traps a significant proportion of programming errors"[^18].

## References
