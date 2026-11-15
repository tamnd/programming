---
tag: E2YAB0
kind: concept
slug: while-loop
name: While loop
aliases: [while loop, while statement, pre-test loop, condition-controlled loop, indefinite loop, entry-controlled loop, while let, until loop, sentinel loop]
area: errors-and-control/loops
level: intro
summary: >-
  A while loop tests a condition before each pass and runs its body while the condition is true. It fits loops whose number of passes is not known at the start.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [Peter Naur]
  note: The ALGOL 60 report, edited by Peter Naur, had a while element in its for statement. Later languages such as Pascal and C have a separate while statement, and the descendants of C kept it.
see_also: [loop, do-while-loop, for-loop, infinite-loops, loop-and-a-half, conditional, control-flow]
same_as:
  wikidata: Q838101
  wikipedia: While_loop
sources:
- url: https://en.wikipedia.org/wiki/Loop_(statement)
  title: Loop (statement), Wikipedia
  locator: Conditional loop; Pre-test loop
- url: https://docs.python.org/3/reference/compound_stmts.html
  title: Compound statements, The Python Language Reference
  locator: 8.2 The while statement
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/while
  title: while, MDN Web Docs
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/iteration-statements
  title: Iteration statements, C# language reference
  locator: The while statement
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements with single condition
- url: https://doc.rust-lang.org/reference/expressions/loop-expr.html
  title: Loops and other breakable expressions, The Rust Reference
  locator: Predicate loops; while let patterns
- url: https://doc.rust-lang.org/book/ch03-05-control-flow.html
  title: Control Flow, The Rust Programming Language
  locator: Conditional Loops with while
- url: https://www.gnu.org/software/bash/manual/html_node/Looping-Constructs.html
  title: Looping Constructs, Bash Reference Manual
  locator: until; while
- url: https://www.masswerk.at/algol60/report.htm
  title: "J. W. Backus et al., Revised Report on the Algorithmic Language ALGOL 60, edited by P. Naur, 1963"
  locator: 4.6.4.3 While-element
---

## Idea

Some loops cannot know their count in advance: read input until it ends, retry until a request works, halve a number until it reaches 1. A while loop states only the condition to go on. It tests the condition, runs the body if the condition is true, and tests again.

```python run id=idea
# The number of passes is not known before the loop starts.
n, steps = 27, 0
while n != 1:
    n = n // 2 if n % 2 == 0 else 3 * n + 1
    steps += 1
print("27 reaches 1 after", steps, "steps")

# The condition is tested first, so the body can run zero times.
count = 0
while count > 0:
    count -= 1
print("no pass:", count)

# A while loop that empties a list from the end.
stack = [1, 2, 3]
while stack:
    print("pop", stack.pop())

# while ... else: the else part runs when the condition becomes false.
k = 3
while k > 0:
    k -= 1
else:
    print("done, k =", k)
```

```output for=idea
27 reaches 1 after 111 steps
no pass: 0
pop 3
pop 2
pop 1
done, k = 0
```

The first loop follows the Collatz rule from 27, and nobody can tell from the code how many passes it needs: the answer is 111. The second loop shows the main property of a while loop: the condition is false at the start, so the body never runs. The third loop runs while the list is not empty, and each pass makes the list shorter. The last loop uses the `else` clause of Python, which runs when the condition becomes false.

## How it works

**Test, then run.** Wikipedia calls this form a pre-test loop: "the condition is checked and if it holds the body is executed. Afterwards, the condition is checked again, and if it holds the body is executed again"[^1]. Python's `while` statement "repeatedly tests the expression and, if it is true, executes the first suite"[^2]. MDN says the same for JavaScript: "The condition is evaluated before executing the statement"[^3]. Because the test comes first, C# notes that "a while loop executes zero or more times"[^4].

**The condition is tested once for each pass.** The loop does not watch its condition all the time. Wikipedia names a "common misconception", namely "that the execution of the body terminates as soon as the condition does not hold anymore"[^1]. If the body makes the condition false in its first line, the rest of the body still runs, and the loop ends only at the next test.

**The condition must change.** A while loop ends only when something in the body, or outside the program, makes the condition false. A loop over input reads one more item in each pass, a search moves its position, and a numeric method improves its estimate. If nothing changes, the loop is an [infinite loop](/infinite-loops/).

**Forms in the languages.**

- C, C++, Java, C# and JavaScript write `while (condition) body`. Java and C# require a Boolean condition, and C and C++ accept any scalar, where 0 is false.
- Go has no `while` keyword. A `for` with only a condition is the while loop: it runs "as long as a boolean condition evaluates to true. The condition is evaluated before each iteration"[^5].
- Rust has `while` and `while let`. "A while loop expression allows repeating the evaluation of a block while a set of conditions remain true"[^6], and `while let Some(y) = x.pop()` runs while the pattern matches. The Rust book explains why `while` exists although `loop`, `if` and `break` can do the same: "this pattern is so common that Rust has a built-in language construct for it, called a while loop"[^7].
- Bash runs a command as the condition. `while` executes the body "as long as test-commands has an exit status of zero", and `until` executes it "as long as test-commands has an exit status which is not zero"[^8].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `while condition:` with an optional `else`; `:=` reads and tests in one condition |
| JavaScript | native | `while (condition)` on a truthy or falsy value |
| TypeScript | native | the `while` loop of JavaScript; the condition narrows types in the body |
| Java | native | `while (condition)` with a `boolean` or `Boolean` condition |
| Kotlin | native | `while (condition)` with a `Boolean` condition |
| C# | native | `while (condition)` with a `bool` condition |
| Go | native | `for condition { }` with no `while` keyword |
| Rust | native | `while condition { }` and `while let pattern = expr { }` |
| C | native | `while (expression)`; any non-zero scalar is true |
| C++ | native | `while (condition)`; the condition can declare a variable |
| Bash | native | `while` and `until` on the exit status of a command; `while read -r line` |
| PowerShell | native | `while (condition)` |
| SQL | idiom | no loops in queries; `WHILE` in procedural extensions such as T-SQL and PL/pgSQL |
| Regex | n/a | no conditions on state; a quantifier repeats as long as the pattern matches |
| HTML | n/a | no loops |
| CSS | n/a | no loops |

The while loop is almost the same in all general-purpose languages. The differences are in the condition: Java, C#, Kotlin, Go and Rust require a Boolean, Python, JavaScript, C and C++ test the truth of any value, and Bash tests the exit status of a command. Go spells the loop with `for`, and Rust and Python can bind a value in the condition.

The example files show these differences. Python reads lines with `:=` in the condition and stops Newton's method at a tolerance. JavaScript loops over the matches of a regular expression with `exec`. Java loops with an `Iterator` and `hasNext`. C# reads lines from a `StringReader` until `ReadLine` returns `null`. Go writes a while loop with `for` and a `Scanner`. Rust uses `while` and `while let`. C walks a string with a pointer until the null character. C++ reads words from a stream in the condition.

## Variations and related ideas

- **Do-while loop.** A post-test loop runs the body before the first test, so the body runs at least once. See [do-while loop](/do-while-loop/).
- **Until loop.** The `until` loop of Bash runs while a command fails[^8], so it is a while loop with the condition reversed. The `repeat ... until` loop of Pascal also stops when its condition becomes true, but it tests after the body, so it is a do-while loop.
- **Read loops.** A loop that reads, tests for the end and then processes is a [loop and a half](/loop-and-a-half/). An assignment in the condition, such as `while ((line = reader.ReadLine()) != null)` or `while (line := f.readline())`, writes it as one while loop.
- **Sentinel values.** A special value, such as `-1`, `null` or an empty line, marks the end of the input, and the loop condition tests for it.
- **Loops with a counter.** A while loop with a counter is a [for loop](/for-loop/), and the for loop is the clearer form when the count is known.

## Pitfalls

- **A body that does not change the condition.** `while (i < n) { sum += a[i]; }` with no `i++` loops forever.
- **A semicolon after the condition.** In C, C++, Java and JavaScript, `while (x > 0);` has an empty body, so the loop runs forever or not at all, and the block below it runs once.
- **Floating-point tests.** `while (x != 1.0)` can miss 1.0 by a rounding error. Test with `<` or with a tolerance.
- **The test runs only between passes.** A body that makes the condition false still runs to its end. Use `break` when the body must stop at once.
- **Assignment instead of comparison.** `while (x = 0)` in C assigns 0 and never runs the body. `while (x = 1)` runs forever.

## Cost

A while loop compiles to a test and a conditional jump for each pass, and compilers often move the test to the end of the body and add one jump before the loop, so each pass has only one jump. The total cost is the number of passes times the cost of the body and the test, so a slow condition, such as a function call that scans a list, can cost more than the body.

## History

ALGOL 60 had no separate while statement, but its for statement had a while element: `for V := E while F do S`[^9] assigns E to V, tests F, and runs S again and again while F is true. Later languages made `while` a statement of its own. C has `while (expression) statement`, and C++, Java, C# and JavaScript copied the C form. Go merged `while` into `for`, and Rust gave `while` a pattern form with `while let`.

## References
