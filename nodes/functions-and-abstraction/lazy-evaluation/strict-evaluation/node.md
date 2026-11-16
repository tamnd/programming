---
tag: 2JK5K7
kind: concept
slug: strict-evaluation
name: Strict evaluation
aliases: [eager evaluation, applicative-order evaluation, call by value evaluation, greedy evaluation, strictness]
area: functions-and-abstraction/lazy-evaluation
level: intro
summary: >-
  Strict evaluation computes every argument of a function before the function body runs. It is the default in most languages, and the opposite of lazy evaluation.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [Peter Naur]
  note: The Algol 60 report, edited by Naur after the January 1960 conference in Paris, gave each procedure parameter one of two modes, call by value, which evaluates the argument before the body runs, and call by name.
see_also: [laziness, thunk, parameter-passing, order-of-evaluation, short-circuit-evaluation]
sources:
- url: https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/full-text/book/book-Z-H-10.html
  title: Structure and Interpretation of Computer Programs, H. Abelson and G. J. Sussman, second edition, MIT Press
  locator: 1.1.5 The Substitution Model for Procedure Application, Applicative order versus normal order
- url: https://en.wikipedia.org/wiki/Evaluation_strategy
  title: Evaluation strategy, Wikipedia
  locator: Strict evaluation; Non-strict evaluation
- url: https://www.haskell.org/onlinereport/haskell2010/haskellch3.html
  title: Haskell 2010 Language Report, Expressions
  locator: chapter 3 introduction, Errors
- url: https://www.haskell.org/onlinereport/haskell2010/haskellch6.html
  title: Haskell 2010 Language Report, Predefined Types and Classes
  locator: 6.2 Strict Evaluation
- url: https://docs.python.org/3.14/reference/expressions.html#calls
  title: Expressions, The Python Language Reference, Python 3.14
  locator: 6.3.4 Calls
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html
  title: Chapter 15. Expressions, The Java Language Specification, Java SE 25 Edition
  locator: 15.7.4 Argument Lists are Evaluated Left-to-Right
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Calls; Order of evaluation
- url: https://doc.rust-lang.org/reference/expressions.html
  title: Expressions, The Rust Reference
  locator: Evaluation order of operands
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/expressions
  title: Expressions, C# language specification
  locator: 12.6.2.3 Run-time evaluation of argument lists
- url: https://tc39.es/ecma262/multipage/ecmascript-language-expressions.html
  title: ECMAScript Language Specification, ECMAScript Language Expressions
  locator: 13.3.6.2 EvaluateCall
- url: https://en.cppreference.com/w/c/language/eval_order
  title: Order of evaluation (C), cppreference.com
  locator: Rules, 1
- url: https://en.cppreference.com/w/cpp/language/eval_order
  title: Order of evaluation (C++), cppreference.com
  locator: Rules, 3 and 13
- url: https://www.gnu.org/software/bash/manual/bash.html#Simple-Command-Expansion
  title: Bash Reference Manual
  locator: 3.7.1 Simple Command Expansion
- url: https://www.masswerk.at/algol60/report.htm
  title: Revised Report on the Algorithmic Language Algol 60, P. Naur, editor
  locator: 4.7.3 Semantics (procedure statements); introduction
---

## Idea

A function call has arguments, and each argument is an expression. Strict evaluation computes the value of every argument first, and only then runs the body of the function. The body receives values, not expressions. This is the rule in Python, Java, C, Go, Rust and almost every other language that programmers use every day:

```python run id=idea
def trace(label, value):
    print("evaluating", label)
    return value


def first(a, b):
    print("inside first")
    return a


print(first(trace("a", 1), trace("b", 2)))
```

```output for=idea
evaluating a
evaluating b
inside first
1
```

The function `first` never uses `b`, but the program computes it anyway, before `first` starts. The opposite rule is [lazy evaluation](/laziness/), which computes an argument only when the body needs its value.

## How it works

**Evaluate, then apply.** SICP describes two ways to evaluate a call. Normal order substitutes the argument expressions into the body and reduces them later. The other is the "evaluate the arguments and then apply" method that the interpreter actually uses, "which is called applicative-order evaluation."[^1] Applicative order is the operational name of strict evaluation: the caller reduces each argument to a value, and the call passes the values.

**Strict as a property of a function.** In semantics, the word strict describes a function, not an evaluator. Let the symbol ⊥, called bottom, stand for a computation that fails or never ends. A function `f` is strict if `f(⊥) = ⊥`, that is, if the call fails whenever the argument fails. Wikipedia notes that applicative order "has the effect of making the function strict, i.e. the function's result is undefined if any of the arguments are undefined, so applicative order evaluation is more commonly called strict evaluation."[^2] In a strict language every function is strict in every argument, because the call never starts when an argument fails.

**Strict and eager.** The two words often mean the same thing, but they come from two views. Strict is a statement about results: an undefined argument gives an undefined call. Eager is a statement about time: the work happens as soon as the program reaches the expression. The same article states that a call "is performed as soon as it is encountered in a procedure, so it is also called eager evaluation or greedy evaluation."[^2] A lazy language can still evaluate some arguments eagerly when the compiler can prove that the function is strict in them, and the result does not change.

**The view from a lazy language.** Haskell is non-strict by default. Its report states that errors, written ⊥, "are indistinguishable by a Haskell program from non-termination. Since Haskell is a non-strict language, all Haskell types include ⊥."[^3] When a Haskell program needs strict evaluation, it asks for it with `seq` or with the operator `$!`, and the report states: "The operator $! is strict (call-by-value) application, and is defined in terms of seq."[^4] So strict application is the default in most languages, and a library operator in Haskell.

**Order inside the strict rule.** Strict evaluation says that all arguments are ready before the body runs. It does not say in which order the arguments are computed. Some languages fix the order and others leave it open, so two side effects in two arguments can happen in either order. See [order of evaluation](/order-of-evaluation/).

## In languages

| Language | Verdict | Arguments | Order of arguments |
|---|---|---|---|
| Python | native | strict | left to right |
| JavaScript | native | strict | left to right |
| TypeScript | native | strict | left to right, as JavaScript |
| Java | native | strict | left to right |
| Kotlin | native | strict | not covered here |
| C# | native | strict | left to right |
| Go | native | strict | calls in left to right order |
| Rust | native | strict | left to right |
| C | native | strict | unspecified |
| C++ | native | strict | indeterminately sequenced |
| Bash | native | words are expanded before the command runs | left to right |
| PowerShell | native | strict | not covered here |
| SQL | n/a | the optimizer decides | none |

In all of these languages, the operators `&&` and `||` and the conditional expression are the exceptions: they evaluate an operand only if the result needs it. See [short-circuit evaluation](/short-circuit-evaluation/).

**Python.** The reference states: "All argument expressions are evaluated before the call is attempted."[^5]

**Java.** The specification states that each argument expression "appears to be fully evaluated before any part of any argument expression to its right."[^6]

**Go.** Arguments "are evaluated before the function is called", and in an expression "all function calls, method calls, receive operations, and binary logical operations are evaluated in lexical left-to-right order."[^7]

**Rust.** The operands of a call "are evaluated prior to applying the effects of the expression. Expressions taking multiple operands are evaluated left to right as written in the source code."[^8]

**C#.** "The expressions of an argument list are always evaluated in textual order."[^9]

**JavaScript and TypeScript.** The specification step for a call computes the argument list before it calls the function: "Let argList be ? ArgumentListEvaluation of argumentListNode."[^10] TypeScript compiles to JavaScript and keeps this rule.

**C and C++.** C has "a sequence point after the evaluation of all function arguments and of the function designator, and before the actual function call."[^11] C++ sequences "every argument expression and the postfix expression designating func" before "every expression or statement in the body of func".[^12] Neither language fixes the order of the arguments among themselves.

**Bash.** Before the shell runs a simple command, the words "are expanded", and the expansions run "from left to right".[^13] A command substitution in an argument runs to the end before the command starts.

## Variations and related ideas

**Call by value and call by reference.** These are two ways to pass an argument in a strict language. Call by value copies the value, and call by reference passes the location of a variable. In both, the argument is ready before the body runs. See [parameter passing](/parameter-passing/).

**Call by name and call by need.** Algol 60 offered call by name next to call by value. A parameter that is not in the value list "is replaced, throughout the procedure body, by the corresponding actual parameter".[^14] The argument runs each time the body uses the parameter, and not before the call. Call by need is call by name with a stored result. Both are non-strict.

**Thunks in a strict language.** A strict language can simulate a non-strict argument: the caller wraps the expression in a function with no parameters, a [thunk](/thunk/), and the body calls it only if it needs the value. A Python `lambda: x / y`, a Java `Supplier` and a Kotlin `() -> T` are thunks of this kind.

**Lazy values in a strict language.** Most strict languages offer lazy sequences and lazy initialization as library types. See [lazy evaluation](/laziness/) and [lazy sequence and stream](/lazy-sequence/).

**Strictness analysis.** A compiler for a lazy language looks for functions that always use an argument. For such a function, strict evaluation of that argument gives the same result and costs less, so the compiler uses it.

## Pitfalls

**A user-defined conditional evaluates both branches.** A function cannot replace `if` in a strict language. Both branch arguments run before the body can choose one:

```python wrong id=both-branches
def choose(test, when_true, when_false):
    return when_true if test else when_false


count = 0
try:
    print(choose(count != 0, 100 / count, 0))
except ZeroDivisionError as error:
    print("error:", type(error).__name__)
```

```output for=both-branches
error: ZeroDivisionError
```

The guard `count != 0` is false, but `100 / count` runs first and fails. Pass thunks and call only the chosen one:

```python run id=thunk-branches
def choose(test, when_true, when_false):
    return when_true() if test else when_false()


count = 0
print(choose(count != 0, lambda: 100 / count, lambda: 0))
```

```output for=thunk-branches
0
```

**Costly default work.** A logging call such as `log.debug(build_report())` builds the report even when the debug level is off, because the argument runs before the logger checks the level. Pass a thunk, or check the level first.

**Side effects in arguments.** A call such as `f(next(it), next(it))` depends on the order of argument evaluation. It is defined in Python, Java and C#. In C and C++ the order is not specified, and cppreference lists `f(++i, ++i)` as undefined behavior in C.[^11] Put each effect in its own statement.

**Infinite or failing arguments.** In a strict language, an argument that never ends stops the whole call, even if the function would ignore it. Code that builds an infinite structure must make it lazy on purpose, for example with a generator.

## Cost

Strict evaluation is cheap and predictable. The machine computes each argument once, on the call stack or in registers, and passes a value. There is no heap object for a suspended expression and no check of a ready flag on each use. Memory use follows the structure of the code, which makes it easy to reason about. SICP gives "the additional efficiency obtained from avoiding multiple evaluations of expressions" as one reason why Lisp uses applicative order.[^1]

The cost is wasted work. A strict call computes arguments that the body does not use, and a strict pipeline builds each full intermediate collection before the next stage starts. A strict program cannot describe an infinite structure directly.

## History

The Algol 60 report came from a conference in Paris in January 1960, and the revised report states the two modes of a procedure parameter in its section 4.7.3: value assignment "(call by value)" and name replacement "(call by name)".[^14] Call by value is strict, and call by name is not. Lisp used applicative order for its interpreter, and SICP still explains this choice with the efficiency of evaluating each argument once.[^1] Most later languages, from C to Java, Python and Rust, use strict evaluation for function arguments. Research languages such as SASL, Miranda and Haskell took the other road and made non-strict evaluation the default, and Haskell offers strict application as the operator `$!`.[^4]

## References
