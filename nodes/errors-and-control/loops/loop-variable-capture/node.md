---
tag: 6TYQCC
kind: concept
slug: loop-variable-capture
name: Loop variable capture
aliases: [loop variable capture, closure over the loop variable, closing over the loop variable, loop variable closure, late binding closures, per-iteration loop variable, loopvar, captured loop variable, x := x]
area: errors-and-control/loops
level: advanced
summary: >-
  A closure made in a loop captures the loop variable, not its value. If the loop has one variable for all passes, every closure sees the last value. Languages differ, and Go and C# changed their rules.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2009
  people: [Eric Lippert, David Chase, Russ Cox]
  note: In 2009 Eric Lippert called the capture of the foreach variable the most common incorrect bug report about C#, and C# 5 then gave each pass of foreach its own variable. Go 1.22 made the same change for all its for loops.
see_also: [closures, lexical-scope, for-loop, for-each-loop, loop, binding]
sources:
- url: https://ericlippert.com/2009/11/12/closing-over-the-loop-variable-considered-harmful-part-one/
  title: "Eric Lippert, Closing over the loop variable considered harmful, part one, 2009"
  locator: the update at the top; the whole post
- url: https://docs.python.org/3/faq/programming.html
  title: Programming FAQ, Python documentation
  locator: Why do lambdas defined in a loop with different values all return the same result?
- url: https://go.dev/blog/loopvar-preview
  title: "David Chase and Russ Cox, Fixing For Loops in Go 1.22, The Go Blog, 2023"
  locator: The Problem; The Fix
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements with for clause
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/for
  title: for, MDN Web Docs
  locator: Lexical declarations in the initialization block
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Closures
  title: Closures, JavaScript Guide, MDN Web Docs
  locator: Creating closures in loops, A common mistake
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/statements
  title: Statements, C# language specification
  locator: 13.9.5 The foreach statement
- url: https://go.dev/wiki/LoopvarExperiment
  title: "Go Wiki: LoopvarExperiment"
  locator: How do I try the change?; What is the solution?
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 15
  locator: 15.27.2 Lambda Body
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 14
  locator: 14.14.2 The enhanced for statement
- url: https://doc.rust-lang.org/reference/expressions/loop-expr.html
  title: Loops and other breakable expressions, The Rust Reference
  locator: Iterator loops
---

## Idea

A loop makes three functions, one in each pass, and each function should print the number of its pass. In many languages they all print the last number. The functions are closures, and a closure keeps a reference to a variable, not a copy of its value. If the loop has one variable for all passes, all three closures share it, and they see the value that it has when they run, after the loop has ended.

```python run id=idea
# Each lambda refers to the variable i, not to its value in that pass.
funcs = []
for i in range(3):
    funcs.append(lambda: i)
print([f() for f in funcs])

# The variable still exists, so a later change shows in all closures.
i = 10
print([f() for f in funcs])

# A default argument is evaluated when the lambda is made.
funcs = []
for i in range(3):
    funcs.append(lambda n=i: n)
print([f() for f in funcs])


# A factory function gives each closure its own variable.
def make(n):
    return lambda: n


print([make(i)() for i in range(3)])
```

```output for=idea
[2, 2, 2]
[10, 10, 10]
[0, 1, 2]
[0, 1, 2]
```

The first loop makes three lambdas that all return 2: they read `i` when they run, and the loop has left 2 in it. The second line changes `i`, and all three lambdas see the new value. The default argument `n=i` copies the value at the time the lambda is made, so each lambda keeps its own number. The factory `make` gets a new parameter `n` in each call, so each closure has a variable of its own.

## How it works

**Closures capture variables.** Eric Lippert explains the rule for C#: the lambda `()=>v` returns the current value of the variable `v`, not the value that `v` had when the lambda was made. In his words, "Closures close over variables, not over values"[^1]. The Python FAQ gives the same reason for its loop of lambdas that all return 16: "x is not local to the lambdas, but is defined in the outer scope, and it is accessed when the lambda is called — not when it is defined"[^2]. See [closures](/closures/).

**One variable for the loop, or one for each pass.** The result depends on where the language declares the loop variable.

- If the loop has one variable for all passes, the closures share it. The Go blog describes the old rule of Go: three goroutines "are all printing the same variable v"[^3], so they usually print `c` three times, not `a`, `b` and `c`.
- If each pass has its own variable, each closure keeps the value of its pass. In Go, "Each iteration has its own separate declared variable (or variables)"[^4].

**JavaScript: `var` and `let`.** A `var` variable belongs to the whole function, so the closures made in a loop share it. MDN shows that a loop with `var` logs `3, 3, 3`, "because variables declared with var are only function-scoped, but not lexically scoped"[^5]. A `let` in the head of a `for` loop is special: "A new lexical scope is created with new let -declared variables. The binding values from the last iteration are used to re-initialize the new variables"[^5]. The MDN guide on closures describes the old trap of a loop with `var`: each closure "shares the same single lexical environment, which has a variable with changing values"[^6].

**C#: `foreach` changed, `for` did not.** C# 5 moved the variable of `foreach` into the loop body. Lippert's post starts with the update: "In C# 5, the loop variable of a foreach will be logically inside the loop, and therefore closures will close over a fresh copy of the variable each time. The for loop will not be changed"[^1]. The C# specification now says that "The placement of v inside the while loop is important for how it is captured", and it adds "(Note that earlier versions of C# declared v outside of the while loop.)"[^7].

**Go: the change in Go 1.22.** Go made the same change for all its `for` loops, the three-part loop too. The Go team wanted "to change for loops to make these variables have per-iteration scope instead of per-loop scope"[^3]. In a three-part loop, "The variable used by each subsequent iteration is declared implicitly before executing the post statement and initialized to the value of the previous iteration's variable at that moment"[^4]. The rule depends on the module: "If the language version is go1.22 or later, the module will use the new loop variable semantics"[^8].

**Java: only effectively final variables.** Java does not let a lambda capture a variable that changes. "Any local variable, formal parameter, or exception parameter used but not declared in a lambda expression must either be final or effectively final"[^9]. The counter of a basic `for` loop changes, so a lambda cannot capture it, and the compiler reports an error. The enhanced `for` loop declares its variable inside the body of the equivalent basic loop, as `T VarDeclId = (TargetType) #i.next();`[^10], so each pass has a new variable, and a lambda can capture it.

**Rust: a new binding in each pass.** A Rust `for` loop matches each value against its pattern inside the loop: the Reference writes the equivalent code with `let PATTERN = next;`[^11] in the body of a `loop`. Each pass binds a new variable.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | one variable for the whole function; a default argument `lambda n=i: n` or a factory function copies the value |
| JavaScript | native | `let` and `const` in the loop head give each pass its own binding; `var` shares one variable |
| TypeScript | native | the rules of JavaScript; `let` and `const` give each pass its own binding |
| Java | native | lambdas capture only effectively final variables; the enhanced `for` has a new variable in each pass |
| Kotlin | native | the variable of `for (x in xs)` is read-only, and a lambda in the body sees the value of its pass |
| C# | native | `foreach` has a new variable in each pass in C# 5 and later; `for` has one variable for the loop |
| Go | native | a new variable in each pass of every `for` loop in modules with `go 1.22` or later; one shared variable before |
| Rust | native | the pattern of `for` binds a new variable in each pass; `move` closures copy or move the value |
| C | absent | no closures; a function pointer with a context argument gets the value as an argument |
| C++ | idiom | a lambda chooses its captures: `[i]` copies the value, and `[&i]` refers to the variable |
| Bash | n/a | no closures |
| PowerShell | idiom | a script block reads variables when it runs; `GetNewClosure()` copies them when it is made |
| SQL | n/a | no closures |
| Regex | n/a | no closures |
| HTML | n/a | no closures |
| CSS | n/a | no closures |

The languages fall into three groups. JavaScript with `let`, Go, Rust, Kotlin and the `foreach` of C# make a new variable in each pass, so the closures work as a reader expects. Python, JavaScript with `var` and the `for` of C# share one variable, so the program must copy the value. Java and C++ make the programmer choose: Java allows only variables that do not change, and C++ states for each capture if it copies or refers.

The example files show these differences. Python fixes late binding with a default argument and a factory. JavaScript compares `var`, `let` and a `let` outside the loop. Java captures the variable of an enhanced `for` and a copy of the counter of a basic `for`. C# compares `foreach` with `for` and fixes `for` with a copy in the body. Go shows that each pass has its own variable and its own address. Rust moves the values into closures and threads. C passes the value to a function with a context argument. C++ compares a capture by value with a capture by reference.

## Variations and related ideas

- **Goroutines and threads.** A closure that runs later, in a goroutine, a thread or a callback, sees the value of the variable when it runs. The Go blog notes that "Although concurrency is often involved, it need not be"[^3].
- **The copy in the body.** In old Go code, `v := v` at the start of the body made a new variable for each pass. The Go wiki describes the new rule in these terms: "the effect is as if each loop body starts with k := k and v := v for each range variable"[^8].
- **Pointers to the loop variable.** Taking the address of a loop variable, as in `&v` in Go, has the same problem as a closure. The Let's Encrypt bug in the Go blog came from a pointer, not a closure[^3].
- **Tests that pass for the wrong reason.** The Go blog shows a test that ran its subtests after the loop, so it checked only the last value, and it passed with an odd number in the list[^3].

## Pitfalls

- **Old versions of the language.** In Go, "the new semantics will only apply in packages contained in modules that declare go 1.22 or later in their go.mod files"[^3], so a module with `go 1.21` keeps the old rule. C# before version 5 shares the `foreach` variable[^7]. The same code can behave in two ways.
- **The `for` loop of C#.** C# changed only `foreach`. A lambda that captures the counter of a `for` loop still sees its last value.
- **`var` in old JavaScript.** Code that uses `var` in the loop head, or declares the variable before the loop, shares one variable between all closures, as MDN shows[^5].
- **Python comprehensions.** A list of lambdas made in a comprehension, such as `[lambda: i for i in range(3)]`, has the same problem, because all the lambdas share the variable of the comprehension.
- **Unnecessary copies.** Tools that warn about the capture can be wrong. The Go team found many `x := x` lines that were not needed, "suggesting instead that popular checkers have significant false positive rates"[^3].

## Cost

A new variable in each pass costs nothing in most loops. The Go wiki says that "A loop only compiles differently if the loop variable has its address taken (&i ) or is captured by a closure"[^8], and even then the compiler may keep the variable on the stack. "However, in some cases, an extra allocation will be added"[^8]: a closure that must keep three values needs three variables, not one. The Go team ran the new rule in all the builds at Google for months and reported "zero reports of any problems in production code"[^3].

## History

Lippert wrote in 2009 that the capture of the `foreach` variable was "the single most common incorrect bug report we get"[^1]. He listed the reasons against a change, such as that "obviously this would be a breaking change"[^1], but C# 5 made it. In Go, "Prior to [Go 1.22], iterations share one set of variables instead of having their own separate variables"[^4]. In 2023 David Chase and Russ Cox wrote that the bug "has caused production problems at many companies"[^3], and Go 1.22 gave each pass its own variable in modules that declare the new version.

## References
