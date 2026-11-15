---
tag: EWSEVV
kind: concept
slug: do-while-loop
name: Do-while loop
aliases: [do-while loop, do while, do statement, post-test loop, exit-controlled loop, repeat until, repeat-until loop, do-until loop, do while 0]
area: errors-and-control/loops
level: intro
summary: >-
  A do-while loop runs its body first and tests its condition after each pass, so the body always runs at least once. Pascal and Lua call it repeat-until.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1970
  people: [Niklaus Wirth]
  note: Pascal, which Niklaus Wirth released in 1970, has repeat-until, a post-test loop. C has do-while, and FORTRAN 77 added zero trip counts to the DO loop.
see_also: [while-loop, loop, loop-and-a-half, for-loop, infinite-loops, control-flow]
same_as:
  wikidata: Q3242594
  wikipedia: Do_while_loop
sources:
- url: https://en.wikipedia.org/wiki/Loop_(statement)
  title: Loop (statement), Wikipedia
  locator: Post-test loop; Equivalent constructs
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 14
  locator: 14.13 The do Statement
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/iteration-statements
  title: Iteration statements, C# language reference
  locator: The do statement
- url: https://kotlinlang.org/docs/control-flow.html
  title: Conditions and loops, Kotlin documentation
  locator: While loops
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/do...while
  title: do...while, MDN Web Docs
  locator: Introduction; Description
- url: https://en.cppreference.com/w/cpp/language/do.html
  title: do-while loop, C++ reference, cppreference.com
  locator: Explanation
- url: https://www.lua.org/pil/4.3.3.html
  title: "Roberto Ierusalimschy, Programming in Lua, first edition, 4.3.3 Repeat"
  locator: 4.3.3
- url: https://www.freepascal.org/docs-html/ref/refsu60.html
  title: The Repeat..until statement, Free Pascal Reference Guide
  locator: the whole page
- url: https://en.wikipedia.org/wiki/Pascal_(programming_language)
  title: Pascal (programming language), Wikipedia
  locator: History; Control structures
- url: https://peps.python.org/pep-0315/
  title: "PEP 315, Enhanced While Loop"
  locator: Notice
- url: https://en.wikipedia.org/wiki/Fortran
  title: Fortran, Wikipedia
  locator: FORTRAN 77
---

## Idea

Some loops must run their body before they can test anything. A program that asks for a password must ask once before it can check the answer, and a function that prints the digits of a number must print at least one digit, even for 0. A do-while loop runs the body, then tests the condition, and runs the body again while the condition is true.

```python run id=idea
# Python has no do-while loop. The usual form is while True with a test at the end.
answers = iter(["", "abc", "42"])
tries = 0
while True:
    text = next(answers)
    tries += 1
    if text.isdigit():
        break
print("got", text, "after", tries, "tries")

# A post-test loop runs once even when the condition is false at the start.
n = 10
while True:
    print("pass with n =", n)
    n += 1
    if not n < 5:
        break

# The same loop as a pre-test loop does not run at all.
n = 10
while n < 5:
    print("never printed")
    n += 1
print("done")
```

```output for=idea
got 42 after 3 tries
pass with n = 10
done
```

The first loop reads answers until one is a number, and it must read an answer before it can test it. The second loop has the condition `n < 5`, which is false at the start, but the body runs once because the test comes after it. The third loop has the same condition at the top, so its body never runs.

## How it works

**Run, then test.** Wikipedia calls this form a post-test loop: "the body is executed and afterwards the condition is checked. If it holds the body is run again and then the condition is checked"[^1]. In Java, "The do statement executes a Statement and an Expression repeatedly until the value of the Expression is false"[^2], and "Executing a do statement always executes the contained Statement at least once"[^2]. C# puts the difference from `while` in one line: "a do loop executes one or more times. The do loop differs from the while loop, which executes zero or more times"[^3]. The Kotlin documentation says it plainly: "the body of do-while runs at least once, regardless of the condition"[^4].

**The syntax.** C, C++, Java, C# and JavaScript write `do body while (condition);`. The semicolon at the end is required. MDN notes that it "requires a semicolon at the end, but the automatic semicolon insertion process may insert one for you"[^5] in JavaScript. In C++, "When control reaches a do statement, its statement will be executed unconditionally"[^6].

**`continue` goes to the test.** In a do-while loop, `continue` does not jump to the top of the body. It jumps to the condition. MDN says that `continue` "stops statement execution and re-evaluates condition"[^5]. If the condition is false, the loop ends.

**Repeat-until.** Pascal and Lua turn the condition around. In Lua, "a repeat-until statement repeats its body until its condition is true. The test is done after the body, so the body is always executed at least once"[^7]. Free Pascal says that `repeat` executes its statements "up to the moment when Expression evaluates to True"[^8]. Pascal writes `repeat a := a + 1 until a = 10;`[^9].

**Languages without it.** Python, Go and Rust have no do-while loop. They write an endless loop with the test at the end of the body: `while True:` and `if not cond: break` in Python, `for { ... if !cond { break } }` in Go and `loop { ... if !cond { break } }` in Rust. Python rejected a `do` clause in PEP 315. Later attempts "did not meet with success because no syntax emerged that could compete with the following form", the `while True` loop with an `if`-`break`, and "Users of the language are advised to use the while-True form with an inner if-break when a do-while loop would have been appropriate"[^10].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | no do-while; `while True:` with `if not cond: break` at the end of the body |
| JavaScript | native | `do { } while (condition);` |
| TypeScript | native | the `do...while` loop of JavaScript |
| Java | native | `do { } while (condition);` with a `boolean` condition |
| Kotlin | native | `do { } while (condition)` |
| C# | native | `do { } while (condition);` |
| Go | idiom | no do-while; `for { ...; if !cond { break } }` |
| Rust | idiom | no do-while; `loop { ...; if !cond { break } }` |
| C | native | `do statement while (expression);`; `do { } while (0)` in macros |
| C++ | native | `do statement while (expression);` |
| Bash | idiom | no do-while; `while body; condition; do :; done` puts the body in the condition list |
| PowerShell | native | `do { } while (condition)` and `do { } until (condition)` |
| SQL | idiom | no loops in queries; `LOOP` with `EXIT WHEN` at the end in PL/pgSQL |
| Regex | n/a | no loops with conditions |
| HTML | n/a | no loops |
| CSS | n/a | no loops |

The do-while loop is the least used of the loops, and three of the newer languages, Go, Rust and Python, leave it out. They write it as an endless loop with an exit at the end. The languages of the C family keep `do ... while`, and Pascal and Lua have the `until` form.

The example files show these differences. Python uses `while True` with a `break` at the end. JavaScript shows that `continue` jumps to the test and retries an action until it works. Java counts digits and gets 1 for 0, where a while loop gets 0. C# reads commands until `quit` and follows the Collatz rule from 1. Go and Rust write the post-test loop with `for` and `loop` and a `break` at the end. Go also sets a Boolean in the post statement of a three-part `for`, and Rust puts the body in the condition of a `while`. C uses `do { } while (0)` in a macro. C++ converts numbers to a base, so 0 gives the string `"0"`.

## Variations and related ideas

- **Loop and a half.** Many loops need the test in the middle of the body, not at the end: read, test, process. See [loop and a half](/loop-and-a-half/).
- **Until loops.** `repeat ... until c` is `do ... while (!c)`. Bash also has an `until` loop, but it tests first, so it is a while loop.
- **`do { } while (0)` in C macros.** A macro with several statements, wrapped in `do { ... } while (0)`, acts as one statement, so `if (x) MACRO(); else ...` compiles and works. The loop runs exactly once.
- **Retry loops.** A loop that tries an action and repeats on failure tries at least once, so it is a natural post-test loop. Add a limit on the number of tries.

## Pitfalls

- **The body runs for bad input.** A do-while loop runs its body before it checks anything, so the body must handle the case that a while loop would skip, such as an empty list.
- **A missing semicolon.** In C, C++, Java and C#, `do { } while (x)` without `;` is a syntax error, and in JavaScript automatic semicolon insertion hides the mistake.
- **`continue` skips to the test.** A `continue` that skips the update of a counter at the end of the body can make the loop run forever.
- **Scope of variables.** In C, Java and C#, a variable declared in the body is not visible in the condition, so it must be declared before the loop.
- **Reversed conditions.** When code moves from `repeat ... until c` to `do ... while`, the condition must become `!c`.

## Cost

A do-while loop is the cheapest loop for the machine: the body falls through to one test and one conditional jump back. Compilers often turn a while loop into a test before the loop and a do-while loop after it, for this reason. The difference is one jump for each loop, so the choice between the forms is about meaning, not speed.

## History

FORTRAN 77 brought "DO loop extensions, including parameter expressions, negative increments, and zero trip counts"[^11], so a `DO` loop with an empty range runs zero times. Niklaus Wirth improved ALGOL W, "releasing this as Pascal in 1970"[^9]. Pascal has `repeat ... until`, a post-test loop with the condition to stop, and C has `do ... while` with the condition to go on. Wikipedia notes a trap in the names: the term do-while loop "can be confusing since Fortran and PL/I use the syntax "DO WHILE" for pre-test loops"[^1].

## References
