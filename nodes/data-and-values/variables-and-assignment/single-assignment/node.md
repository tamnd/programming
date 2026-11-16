---
tag: 10A3P5
kind: concept
slug: single-assignment
name: Single assignment
aliases: [single assignment, single-assignment variable, assign once, immutable variable, immutable binding, write once variable, final variable, val, rebinding, shadowing, static single assignment, SSA]
area: data-and-values/variables-and-assignment
level: advanced
summary: >-
  With single assignment, a variable gets a value once and keeps it. Erlang allows only single assignment, other languages offer it with const, final or let, and compilers use it in SSA form.
applies_to: core-languages
viz: none
formal: none
see_also: [assignment, variable-concept, constant, initialization-concept, immutability, binding]
sources:
- url: https://en.wikipedia.org/wiki/Assignment_(computer_science)
  title: Assignment (computer science), Wikipedia
  locator: Single assignment
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: Chapter 4, Types, Values, and Variables, The Java Language Specification, Java SE 25
  locator: 4.12.4 final Variables
- url: https://www.erlang.org/doc/system/expressions.html
  title: Expressions, Erlang System Documentation
  locator: Variables
- url: https://doc.rust-lang.org/book/ch03-01-variables-and-mutability.html
  title: Variables and Mutability, The Rust Programming Language
  locator: Introduction; Shadowing
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/const
  title: const, MDN Web Docs
  locator: Introduction
- url: https://en.wikipedia.org/wiki/Static_single-assignment_form
  title: Static single-assignment form, Wikipedia
  locator: Introduction; Converting to SSA
---

## Idea

In most languages, a [[variable-concept]] can get a new value at any time. With single assignment, it gets one value and keeps it. Wikipedia says that single assignment is "more commonly known as initialization"[^1], and that it "can only be done once, usually when the variable is created; no subsequent reassignment is allowed"[^1].

A loop that adds to a total assigns `total` many times. The same computation with single assignment gives each step a new name:

```python run id=idea
total = 0
for price in [3, 4, 5]:
    total = total + price
print(total)

prices = [3, 4, 5]
subtotal = sum(prices)
tax = subtotal * 0.25
grand_total = subtotal + tax
print(subtotal, tax, grand_total)
```

```output for=idea
12
12 3.0 15.0
```

In the second part, each name has one value for its whole life. A reader who sees `tax` anywhere knows its value without a search for other assignments.

## How it works

**Why it helps.** A name with one value is a fact: the reader, the compiler and other threads can rely on it. An [[assignment]] that changes a value has an effect that other code can see. "Imperative assignment can introduce side effects while destroying and making the old value unavailable while substituting it with a new one"[^1]. Java says of `final`: "Declaring a variable final can serve as useful documentation that its value will not change and can help avoid programming errors."[^2]

**Languages with only single assignment.** In Erlang, "Variables are bound to values using pattern matching. Erlang uses single assignment, that is, a variable can only be bound once."[^3] A second match of `X = 2` after `X = 1` fails. "Single assignment is the only form of assignment available in purely functional languages, such as Haskell"[^1]. A loop in these languages is a recursive function, and each call binds the names again with new values.

**Languages with both.** Most languages have a keyword for a name that is assigned once: `const` in JavaScript, `final` in Java, `val` in Kotlin and Scala, and `let` without `mut` in Rust. In Rust it is the default: "by default, variables are immutable"[^4]. In Java, "A final variable may only be assigned to once."[^2] The rule is about the name, not about the object: for JavaScript `const`, "The value of a constant can't be changed through reassignment using the assignment operator, but if a constant is an object, its properties can be added, updated, or removed."[^5] See [[immutability]] for objects that do not change.

**Rebinding is not mutation.** Some languages allow a new variable with the same name. In Rust, a second `let x` makes a new variable that hides the first, called shadowing. "Shadowing is different from marking a variable as mut because we’ll get a compile-time error if we accidentally try to reassign to this variable without using the let keyword."[^4] Each `x` is still assigned once. Code that held the old value sees the old value.

**Static single assignment in compilers.** Compilers convert code to a form where each variable gets one value. "In compiler design, static single assignment form (often abbreviated as SSA form or simply SSA) is a type of intermediate representation (IR) where each variable is assigned exactly once."[^6] The compiler gives each assignment a new version of the name, such as `x1` and `x2`. Where two paths join, "a special statement is inserted in the last block, called a Φ (Phi) function"[^6], which picks the version from the path that ran. "SSA makes numerous analyses needed for optimizations easier to perform, such as determining use-define chains, because when looking at a use of a variable there is only one place where that variable may have received a value."[^6]

## In languages

| Language | Verdict | Single assignment |
|---|---|---|
| Python | idiom | no single-assignment variables; `typing.Final` for a type checker, and tuples and frozen dataclasses for values that do not change |
| JavaScript | native | `const` |
| TypeScript | native | `const`, and `readonly` for properties |
| Java | native | `final` variables; a local variable that is never assigned again is effectively final |
| Kotlin | native | `val` |
| C# | idiom | `readonly` fields and `init` properties; no single-assignment local variables except `const` |
| Go | absent | `const` for compile-time values only; each variable can be assigned again |
| Rust | native | `let` without `mut` is the default, and shadowing makes a new variable |
| C | idiom | a local variable with `const` and an initializer |
| C++ | idiom | `const` and `constexpr` variables |
| Bash | idiom | `readonly` and `declare -r` |
| PowerShell | idiom | `Set-Variable -Option ReadOnly` or `Constant` |
| SQL | native | a query has no variables; each name in a common table expression is defined once |
| Regex | n/a | a pattern has no variables |
| HTML | n/a | HTML is markup, with no variables |
| CSS | n/a | a custom property can get a new value in each rule |

## Variations and related ideas

- **Erlang and equal values.** Wikipedia lists "Erlang (it accepts multiple assignment if the values are equal, in contrast to Haskell)"[^1]: `X = 1` after `X = 1` is a successful match.
- **Effectively final.** Java treats a local variable that the program never assigns again as final when a lambda uses it.
- **Dataflow variables.** Some languages have variables that start with no value and get one value later, and a read waits for the value.
- **Recursion instead of a loop.** With single assignment, the state of a loop moves into the parameters of a recursive function or into a fold such as `reduce`.

## Pitfalls

- **One variable for many values.** A closure that uses a loop variable sees the variable, not its value at the time. In Python, the loop assigns `i` again on each pass, so all the functions see the last value:

```python wrong id=late-binding
funcs = []
for i in range(3):
    funcs.append(lambda: i)
print([f() for f in funcs])
```

```output for=late-binding
[2, 2, 2]
```

Bind the value as a parameter, `lambda i=i: i`, so that each function gets its own name with one value. JavaScript `let` in a `for` loop makes a new variable on each pass for the same reason.

- **A constant name with an object that changes.** `final List<String> names` cannot get a new list, but `names.add(...)` changes the list.
- **Shadowing that hides a bug.** A new variable with the same name in an inner block can hide the outer one, and an assignment meant for the outer variable changes the inner one.

## Cost

Single assignment costs nothing at run time: the check is in the compiler. Code that makes a new value for each step, instead of a change in place, can cost more memory and time, for example a new list on each pass of a loop. Compilers remove much of this cost, and SSA form helps them do it.

## History

Erlang and the purely functional languages made single assignment the only form. SSA form is an intermediate representation in "most high-quality optimizing compilers for imperative languages, including LLVM, the GNU Compiler Collection, and many commercial compilers"[^6]. Newer languages make the single-assignment name the short default, such as `let` in Rust and `val` in Kotlin.

## References
