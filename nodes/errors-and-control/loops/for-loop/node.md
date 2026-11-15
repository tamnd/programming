---
tag: AB58R6
kind: concept
slug: for-loop
name: For loop
aliases: [for loop, for statement, three-part for loop, C-style for loop, counted loop, counting loop, numeric for loop, range loop, loop counter, loop variable, for range]
area: errors-and-control/loops
level: intro
summary: >-
  A for loop runs its body once for each value of a loop variable. The three-part form of C has an initialization, a condition and an update, and many languages loop over a range of numbers.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [Stephen C. Johnson]
  note: ALGOL 60 had a for statement with step and until. The three-part for loop of B, and then of C, came from Stephen C. Johnson.
see_also: [loop, while-loop, for-each-loop, iterators, block]
same_as:
  wikidata: Q838119
  wikipedia: For_loop
sources:
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 14
  locator: 14.14.1 The basic for Statement
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements with for clause; For statements with range clause
- url: https://en.cppreference.com/w/c/language/for.html
  title: for loop, C reference, cppreference.com
  locator: Explanation
- url: https://en.cppreference.com/w/cpp/language/for.html
  title: for loop, C++ reference, cppreference.com
  locator: Explanation; Notes
- url: https://en.wikipedia.org/wiki/Control_flow
  title: Control flow, Wikipedia
  locator: Loops; Numeric; Condition-controlled
- url: https://docs.python.org/3/reference/compound_stmts.html
  title: Compound statements, The Python Language Reference
  locator: 8.3 The for statement
- url: https://doc.rust-lang.org/reference/expressions/loop-expr.html
  title: Loops and other breakable expressions, The Rust Reference
  locator: Iterator loops
- url: https://www.gnu.org/software/bash/manual/html_node/Looping-Constructs.html
  title: Looping Constructs, Bash Reference Manual
  locator: for (( expr1 ; expr2 ; expr3 ))
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/for
  title: for, MDN Web Docs
  locator: Introduction; Lexical declarations in the initialization block
- url: https://www.masswerk.at/algol60/report.htm
  title: "J. W. Backus et al., Revised Report on the Algorithmic Language ALGOL 60, edited by P. Naur, 1963"
  locator: 4.6 For statements
- url: https://en.wikipedia.org/wiki/Loop_(statement)
  title: Loop (statement), Wikipedia
  locator: Three-part for loop; Enumeration; Loop counter
---

## Idea

A for loop is the loop for counting. It gives a loop variable a first value, runs the body, steps the variable, and stops when the variable passes a limit. The loop header holds all the control, so a reader sees at once how many times the body runs.

```python run id=idea
squares = []
for i in range(1, 6):
    squares.append(i * i)
print(squares)

# range(start, stop, step): the stop value is not included.
print(list(range(0, 10, 3)))
print(list(range(5, 0, -1)))

# The C loop for (i = 0; i < 5; i++) is a while loop with a counter.
i = 0
while i < 5:
    i += 1
print("after the loop, i =", i)

# In Python, the loop variable stays after the loop with its last value.
for j in range(3):
    pass
print("j =", j)

# A change to the loop variable in the body does not change the next value.
for k in range(3):
    print("k is", k)
    k = 100
```

```output for=idea
[1, 4, 9, 16, 25]
[0, 3, 6, 9]
[5, 4, 3, 2, 1]
after the loop, i = 5
j = 2
k is 0
k is 1
k is 2
```

`range(1, 6)` gives the numbers 1 to 5, so the first loop makes five squares. A third argument sets the step, and a negative step counts down. The `while` loop shows what the three-part loop of C does: set a counter, test it before each pass, and step it after each pass. The last two loops show two rules of Python: the variable `j` keeps its last value after the loop, and the assignment `k = 100` has no effect on the next pass.

## How it works

**The three parts.** The for loop of C, C++, Java, C#, JavaScript and Go has a header with three parts: an initialization, a condition and an update. In Java, "The basic for statement executes some initialization code, then executes an Expression, a Statement, and some update code repeatedly until the value of the Expression is false"[^1]. The Go specification gives the order: "the init statement is executed once before evaluating the condition for the first iteration; the post statement is executed after each execution of the block (and only if the block was executed)"[^2]. So a three-part for loop is a while loop with two more slots. cppreference says that the C for loop is "Used as a shorter equivalent of while loop"[^3]. The C++ page writes the loop as a block with the initialization and a `while` loop, with one difference: "Executing a continue statement in statement will evaluate expression"[^4].

**Empty parts.** Any of the three parts can be empty. In Go, "Any element of the ForClause may be empty but the semicolons are required unless there is only a condition"[^2]. In C++, an "Empty condition is equivalent to true"[^4], so `for (;;)` loops until a `break`. Wikipedia notes that "Although using the for keyword, the three-part C-style loop is a condition-based construct, not a numeric-based one"[^5]: the three parts can hold any code, not only a counter.

**Ranges.** Python, Rust, Kotlin and Bash write a counted loop as a loop over a range of numbers. Python has no three-part loop, and its `for` statement takes the values of an iterable: "iterating range(3) successively yields 0, 1, and then 2"[^6]. The Rust Reference adds the numbers 1 to 10 with `for n in 1..11`[^7]. The range `1..11` does not include 11, and `1..=10` is the same range written with its last value. Go can loop over an integer: `for i := range n` gives the "integer values from zero to an upper limit"[^2]. Bash has both forms: `for name in words` and an arithmetic `for (( expr1 ; expr2 ; expr3 ))`, which repeats "until it evaluates to zero"[^8].

**The scope of the loop variable.** The rules differ by language and by version.

- In C, a variable declared in the initialization "is in scope in the entire loop body, including the remainder of init-clause, the entire cond-expression, the entire iteration-expression and the entire loop-statement"[^3], and it does not exist after the loop. C++, Java, C# and JavaScript with `let` have the same rule.
- In Python, "Names in the target list are not deleted when the loop is finished"[^6], so the variable is still there after the loop.
- In JavaScript, a `var` in the header belongs to the whole function. MDN shows that closures made in a loop then all see the last value, "because variables declared with var are only function-scoped, but not lexically scoped"[^9].
- In Go, "Each iteration has its own separate declared variable (or variables) [Go 1.22]"[^2]. "Prior to [Go 1.22], iterations share one set of variables instead of having their own separate variables"[^2].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | no three-part loop; `for i in range(start, stop, step)` over a range object |
| JavaScript | native | `for (init; condition; update)`; `let` makes a new binding for each pass |
| TypeScript | native | the `for` loop of JavaScript; the counter gets the type `number` |
| Java | native | the basic `for` with lists of initializers and updates; the variable lives in the loop |
| Kotlin | idiom | no three-part loop; `for (i in 1..10)`, `until`, `downTo` and `step` |
| C# | native | `for (init; condition; iterator)` with lists of expressions in the first and last parts |
| Go | native | `for init; cond; post`; `for i := range n`; a new variable for each pass in Go 1.22 |
| Rust | idiom | no three-part loop; `for i in 0..n`, `(0..n).rev()` and `step_by` |
| C | native | `for (init; cond; update)`; the first part can declare the counter |
| C++ | native | the loop of C; a range-based `for` for containers and ranges |
| Bash | native | `for (( i = 0; i < n; i++ ))` and `for i in {1..10}` |
| PowerShell | native | `for ($i = 0; $i -lt 10; $i++)` and `foreach ($i in 1..10)` |
| SQL | idiom | no loops in queries; `generate_series` or a recursive common table expression makes a range of rows |
| Regex | n/a | no counters; the quantifier `{n}` repeats a pattern n times |
| HTML | n/a | no loops; HTML is a markup language |
| CSS | n/a | no loops; `:nth-child(2n+1)` selects every second element |

Two families exist. C, C++, Java, C#, JavaScript, Go and Bash have the three-part loop. Python, Rust and Kotlin dropped it and loop over a range object, so the bounds and the step are values. The second family cannot forget the update or test the wrong variable, which are common bugs of the first.

The example files show these differences. Python shows that adding 0.1 ten times does not give 1.0, computes each value from an integer counter, and counts down with a negative step. JavaScript compares closures over `let` and `var` counters, uses two counters in one header and leaves all three parts empty. Java steps two variables and counts over a range of characters. C# counts down and doubles its counter. Go uses the three-part loop, `range` over an integer and a closure for each pass. Rust uses half-open and inclusive ranges, `rev` and `step_by`. C counts down with an unsigned counter in the safe order. C++ loops with an index, with an empty condition and with a reverse iterator.

## Variations and related ideas

- **Loops over collections.** A for-each loop takes the elements of a collection, not numbers, and has no counter. See [for-each loop](/for-each-loop/).
- **Several variables.** C, C++, Java, C# and JavaScript allow lists in the first and last parts, for example `for (i = 0, j = n - 1; i < j; i++, j--)`, which walks from both ends.
- **Downward and stepped loops.** A loop can count down or count by a step: `for (i = 10; i > 0; i--)` in C, `range(10, 0, -1)` in Python and `(1..=10).rev()` in Rust.
- **Loops with an index.** A loop that needs the position and the element uses `enumerate` in Python, `entries()` in JavaScript or the index and value of `range` in Go.
- **Vectorized code.** NumPy, SQL and array languages apply one operation to all elements and need no explicit loop.

## Pitfalls

- **Off-by-one bounds.** `for (i = 0; i <= n; i++)` runs n + 1 times and reads `a[n]`, one past the end. Use the half-open form `i < n`.
- **Unsigned counters that count down.** In C and C++, `for (size_t i = n - 1; i >= 0; i--)` never ends, because an unsigned value is always 0 or more. Test before the decrement: `for (size_t i = n; i-- > 0; )`.
- **Floating-point counters.** The Control flow article warns that a loop from 0.1 to 1.0 by 0.1 "might iterate 9 or 10 times, depending on various factors such as rounding error, hardware, compiler"[^5]. Count with an integer and compute the value from it.
- **A changed bound.** In C and Java, the condition runs again before each pass, so a loop that adds elements to a list while it tests `i < list.size()` can run forever.
- **Closures over the counter.** A closure made in the body can see a later value of the counter. This depends on the language and the version, as described in the section on scope.

## Cost

A three-part loop compiles to the same code as the equivalent while loop: one test and one jump for each pass. Compilers know the pattern of a counter that steps by a constant, so they unroll such loops, remove bounds checks that the condition already proves, and use vector instructions. In Python, `for i in range(n)` is slower than a vectorized operation, because each pass runs bytecode, but `range` does not build a list, so it uses the same memory for any n.

## History

ALGOL 60 had a general for statement. A for clause "causes the statement S which it precedes to be repeatedly executed zero or more times"[^10], and a for list could hold values, a `step`-`until` element and a `while` element. The report also said that after a normal exit, "the value of the controlled variable is undefined after the exit"[^10]. BASIC and Pascal kept the counted form: the Control flow article shows `FOR I = 1 TO N` in BASIC and `for I := 1 to N do` in Pascal[^5]. Wikipedia says that the syntax of the three-part loop "came from B and was originally invented by Stephen C. Johnson"[^11]. C++, Java, JavaScript, C# and Go copied it, and Python, Rust and Kotlin replaced it with a loop over a range.

## References
