---
tag: YSNP3Z
kind: concept
slug: loop
name: Loop
aliases: [loop, loop statement, iteration, iteration statement, repetition, looping construct, loop body, loop condition, pre-test loop, post-test loop, definite iteration, indefinite iteration]
area: errors-and-control/loops
level: intro
summary: >-
  A loop runs the same block of code again and again: a fixed number of times, while a condition is true, or once for each element of a collection.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1957
  people: [John Backus, Stephen C. Johnson]
  note: The first FORTRAN compiler of April 1957 had the DO loop. ALGOL 60 added the for statement with step, until and while, and the three-part for loop of B and C came from Stephen C. Johnson.
see_also: [for-loop, while-loop, do-while-loop, for-each-loop, infinite-loops, recursion, iterators, control-flow]
same_as:
  wikidata: Q8868615
  wikipedia: Loop_(statement)
sources:
- url: https://en.wikipedia.org/wiki/Loop_(statement)
  title: Loop (statement), Wikipedia
  locator: Introduction; Conditional loop; Enumeration; Recursion; Loop counter; Early exit and continuation; Infinite loop
- url: https://en.wikipedia.org/wiki/Control_flow
  title: Control flow, Wikipedia
  locator: Loops; Numeric; Condition-controlled
- url: https://docs.python.org/3/reference/compound_stmts.html
  title: Compound statements, The Python Language Reference
  locator: 8.2 The while statement; 8.3 The for statement
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 14
  locator: 14.12 The while Statement; 14.13 The do Statement; 14.14 The for Statement
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/iteration-statements
  title: Iteration statements, C# language reference
  locator: Introduction
- url: https://doc.rust-lang.org/reference/expressions/loop-expr.html
  title: Loops and other breakable expressions, The Rust Reference
  locator: Introduction; Infinite loops; Breaking and values
- url: https://en.wikipedia.org/wiki/Fortran
  title: Fortran, Wikipedia
  locator: History; FORTRAN 77
- url: https://www.masswerk.at/algol60/report.htm
  title: "J. W. Backus et al., Revised Report on the Algorithmic Language ALGOL 60, edited by P. Naur, 1963"
  locator: 4.6 For statements
- url: https://en.wikipedia.org/wiki/Structured_programming
  title: Structured programming, Wikipedia
  locator: Theoretical foundation
---

## Idea

Many jobs repeat one step: add up a list of prices, read the lines of a file, retry a request, move each object in a game by one frame. A loop writes the step once and runs it again and again. Wikipedia defines a loop as "a control flow construct that allows code to be executed repeatedly, usually with minor alterations between repetitions"[^1].

```python run id=idea
# A counted loop: the body runs once for each number in a range.
total = 0
for i in range(1, 6):
    total += i
print("sum 1..5 =", total)

# A condition loop: the body runs while the condition is true.
n, steps = 100, 0
while n > 1:
    n //= 2
    steps += 1
print("halvings of 100:", steps)

# A loop over a collection: the body runs once for each element.
for word in ["red", "green", "blue"]:
    print(word, len(word))

# continue skips to the next pass, and break leaves the loop.
for k in range(10):
    if k % 2 == 0:
        continue
    if k > 7:
        break
    print("odd", k)


# Recursion can do the same job as a loop.
def total_to(n):
    return 0 if n == 0 else n + total_to(n - 1)


print("recursive sum 1..5 =", total_to(5))
```

```output for=idea
sum 1..5 = 15
halvings of 100: 6
red 3
green 5
blue 4
odd 1
odd 3
odd 5
odd 7
recursive sum 1..5 = 15
```

The three loops show the three main kinds. The first runs a known number of times. The second runs until its condition becomes false, and the program does not know the count before it starts: 100 becomes 50, 25, 12, 6, 3 and 1 in six steps. The third runs once for each element of a list. The fourth loop leaves early with `break` and skips the even numbers with `continue`. The last function gets the same sum with recursion and no loop statement.

## How it works

**The body and the condition.** "A conditional loop is usually composed of two parts: a condition and a body"[^1]. The body is a block of statements. The condition decides if the body runs one more time. Each run of the body is an iteration, or a pass. The Control flow article puts the two main cases like this: "the body is executed once for each item of a collection (definite iteration), until a condition is met (indefinite iteration)"[^2].

**When the test runs.** A pre-test loop tests the condition before each pass, so the body can run zero times. Python's `while` statement "repeatedly tests the expression and, if it is true, executes the first suite"[^3], and in Go "The condition is evaluated before each iteration"[^4]. A post-test loop tests after each pass, so the body runs at least once: in Java, "Executing a do statement always executes the contained Statement at least once"[^5]. See [while loop](/while-loop/) and [do-while loop](/do-while-loop/).

**Counted loops and loops over collections.** A counted loop has a loop variable that takes the values of a range. The three-part `for` loop of C, Java, C# and JavaScript has an initialization, a condition and an update, and it is a pre-test loop with a counter, see [for loop](/for-loop/). A loop over a collection gets each element from the collection itself. In Python, the `for` statement "is used to iterate over the elements of a sequence (such as a string, tuple or list) or other iterable object"[^3], see [for-each loop](/for-each-loop/) and [iterators](/iterators/).

**Exits.** Most languages let the body change the normal order. `break` "terminates the current loop the program is in", and `continue` "skips to the next iteration of the current loop"[^1]. A `return` in the body also ends the loop. "In the case of nested loops, the break and continue statements apply to the inner most loop"[^1], and Java, Go, Rust and JavaScript add labels to leave an outer loop.

**One loop or many.** The languages have different numbers of loop statements. C# has four, `for`, `foreach`, `do` and `while`, which "repeatedly execute a statement or a block of statements"[^6]. Go has only one: a `for` statement "specifies repeated execution of a block"[^4], and its forms are a condition loop, a three-part loop and a `range` loop. "Rust supports four loop expressions"[^7]: `loop`, `while`, `for` and a labeled block. In Rust, a loop is an expression, and a `loop` can give a value with `break`.

**Loops and recursion.** A loop and a recursive function can compute the same things. "In most functional programming languages, recursion is used instead of traditional loops"[^1], because variables do not change, so a counter cannot step. To make deep recursion safe, these languages "implement tail call optimisation, which allows the same stack frame to be used for each iteration of the loop"[^1]. Python does not do this, so a deep recursion fails where a loop works, as the Python example shows. See [recursion](/recursion/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `while` and `for ... in` over iterables; `range`; `else` clause on loops; comprehensions |
| JavaScript | native | `for`, `while`, `do...while`, `for...of`, `for...in`; labels; `Array.prototype.forEach` |
| TypeScript | native | the loops of JavaScript; the type checker gives the type of the loop variable |
| Java | native | `for`, enhanced `for`, `while`, `do`; labeled `break` and `continue`; streams |
| Kotlin | native | `for` over ranges and iterables, `while`, `do`-`while`; labels such as `break@outer` |
| C# | native | `for`, `foreach`, `while`, `do`; `await foreach` for async streams; LINQ |
| Go | native | one statement `for`: condition, three-part and `range` forms; labels |
| Rust | native | `loop`, `while`, `while let`, `for` over iterators; loop labels; `break` with a value |
| C | native | `for`, `while`, `do`; `break`, `continue` and `goto` |
| C++ | native | the loops of C; range-based `for`; algorithms such as `std::for_each` |
| Bash | native | `for ... in`, the arithmetic `for ((...))`, `while` and `until` on exit status |
| PowerShell | native | `for`, `foreach`, `while`, `do`-`while`, `do`-`until`; `ForEach-Object` in pipelines |
| SQL | idiom | set operations on all rows replace loops; recursive common table expressions; loops in procedural extensions |
| Regex | idiom | quantifiers such as `*`, `+` and `{n,m}` repeat a pattern |
| HTML | n/a | no loops; a script or a template language repeats content |
| CSS | n/a | no loops; a rule applies to all the elements that its selector matches |

All general-purpose languages have loops, but they differ in the number of loop statements, in the scope of the loop variable, and in the exits. Python has only two loop statements and an `else` clause. Go has one. Rust treats a loop as an expression. SQL works on sets of rows, so a query does not need a loop.

The example files show these differences. Python uses `for ... else`, replaces a loop with `sum` and a comprehension, and shows that deep recursion fails where a loop does not. JavaScript has five loop statements, a labeled `break` and `forEach`. Java uses its four loop forms, a labeled `break` and a stream. C# uses `for`, `foreach`, `while`, `do` and LINQ. Go writes all its loops with `for`. Rust uses `loop` with a value, `while let` and `for` over a range. C uses `for`, `while`, `do` and a `goto` loop. C++ uses a range-based `for` and `std::accumulate`.

## Variations and related ideas

- **Nested loops.** A loop in the body of another loop visits all the pairs, for example each cell of a grid. The inner loop runs completely for each pass of the outer loop.
- **Loops as expressions.** In Rust, a `loop` gives the value of its `break`. In Python, a comprehension such as `[x * x for x in xs]` is a loop that builds a list.
- **Higher-order functions.** `map`, `filter`, `reduce` and `forEach` hide the loop inside a function call. See [higher-order functions](/higher-order-functions/).
- **Loop with an `else`.** Python runs the `else` clause of a loop when the loop ends without `break`, which is useful for a search.
- **Infinite loops.** A loop with no exit runs until the program stops, which is right for a server or an event loop and wrong anywhere else. See [infinite loops](/infinite-loops/).

## Pitfalls

- **Off by one.** `i <= n` instead of `i < n` runs one pass too many and can read past the end of an array. Half-open ranges such as `range(0, n)` help.
- **A condition that never becomes false.** If the body does not change the values that the condition tests, the loop never ends.
- **Changing a collection while looping over it.** Adding or removing elements during a for-each loop raises an error for a Java list or a Python dictionary, and it can be undefined behavior for a C++ container.
- **Capture of the loop variable.** A closure made in a loop can see the last value of the variable. In Go, "Prior to [Go 1.22], iterations share one set of variables"[^4]. A `var` variable in a JavaScript loop has the same trap.
- **Floating-point counters.** `for (x = 0.1; x != 1.0; x += 0.1)` may never stop, because 0.1 is not exact in binary. Count with integers and compute the floating-point value from the count.

## Cost

A loop compiles to a test and a jump back to the start of the body. Wikipedia notes that "When a program is compiled to machine code, looping may be achieved using jumps; but some loops can be optimized to run without jumping"[^1]. The cost of the loop itself is small. The cost of the body times the number of passes is what counts, and two nested loops over n elements do n × n passes. Compilers unroll short loops, move constant work out of the body, and use vector instructions to do several passes at once.

## History

FORTRAN had a `DO` loop from its first version. The first FORTRAN manual came out in October 1956, "with the first FORTRAN compiler delivered in April 1957"[^8]. ALGOL 60 made the loop a structured statement. Its `for` clause "causes the statement S which it precedes to be repeatedly executed zero or more times"[^9], with `step` and `until` for counting and `while` for a condition. The three-part `for` of C "came from B and was originally invented by Stephen C. Johnson"[^1]. In the late 1960s, structured programming built on the structured program theorem, which states that sequencing, selection and iteration "are sufficient to express any computable function"[^10]. Since then, the structured loop with `break` and `continue` has replaced the backward `goto` in most code.

## References
