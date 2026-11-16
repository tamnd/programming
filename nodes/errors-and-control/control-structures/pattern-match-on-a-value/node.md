---
tag: 5BRCEX
kind: task
slug: pattern-match-on-a-value
name: Pattern match on a value
aliases: [match on a tuple, match with guards, match on an enum, classify a point, tuple patterns, case guards]
area: errors-and-control/control-structures
level: core
summary: >-
  Name the place of a point in the plane with one match on the pair (x, y): tuple patterns for the axes, a guard for the diagonal, and a wildcard for the rest.
contract: contract.yaml
category: control-flow
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3.15/tutorial/controlflow.html#match-statements
  title: More Control Flow Tools, The Python Tutorial
  locator: 4.7 match Statements
- url: https://doc.rust-lang.org/book/ch19-03-pattern-syntax.html
  title: Pattern Syntax, The Rust Programming Language
  locator: Destructuring Enums; Adding Conditionals with Match Guards
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/patterns#positional-pattern
  title: Pattern matching, C# language reference
  locator: Positional pattern
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/switch-expression#case-guards
  title: switch expression, C# language reference
  locator: Case guards
- url: https://openjdk.org/jeps/440
  title: "JEP 440: Record Patterns"
  locator: Summary; the header (Release 21)
- url: https://openjdk.org/jeps/441
  title: "JEP 441: Pattern Matching for switch"
  locator: Case refinement; the header (Release 21)
- url: https://go.dev/ref/spec#Expression_switches
  title: The Go Programming Language Specification
  locator: Expression switches
- url: https://www.gnu.org/software/bash/manual/html_node/Conditional-Constructs.html
  title: Conditional Constructs, Bash Reference Manual
  locator: case
- url: https://doc.rust-lang.org/reference/expressions/match-expr.html#match-guards
  title: The Rust Reference, Match expressions
  locator: Match guards
- url: https://github.com/tc39/proposal-pattern-matching
  title: ECMAScript Pattern Matching, TC39 proposal
  locator: README, Status
- url: https://docs.python.org/3.15/reference/compound_stmts.html#overview
  title: Compound statements, The Python Language Reference
  locator: 8.6.1 Overview
---

## Task

Match one value against a list of patterns, and use a guard where a pattern alone cannot say what you need. The function `classify(x, y)` names the place of the point (x, y):

- `"origin"` when x and y are both 0,
- `"x-axis"` when y is 0, and x is not,
- `"y-axis"` when x is 0, and y is not,
- `"diagonal"` when x equals y, and the point is not the origin,
- `"other"` for each other point.

For example, `classify(5, 0)` is `"x-axis"` and `classify(4, 4)` is `"diagonal"`.

The value to match is the pair (x, y). The first three cases are patterns with literals and wildcards. The fourth case needs a guard, because a pattern cannot compare x with y. [[pattern-matching]] explains patterns, guards and exhaustiveness. This task applies them to a tuple. The last part of the solutions also matches on an enum.

## Solutions

| Language | The match | The pattern for the x-axis | The guard |
|---|---|---|---|
| Python | `match (x, y):` | `case (_, 0):` | `case (a, b) if a == b:`[^1] |
| Rust | `match (x, y)` | `(_, 0) =>` | `(a, b) if a == b =>`[^2] |
| C# | `(x, y) switch` | `(_, 0) =>`[^3] | `var (a, b) when a == b =>`[^4] |
| Java | `switch (new Point(x, y))` | `case Point(int a, int b) when b == 0 ->`[^5] | `case Point(int a, int b) when a == b ->`[^6] |
| Go | `switch {` | `case y == 0:` | `case x == y:`[^7] |
| Bash | `case "$1,$2" in` | `*,0)`[^8] | an `if` in the last clause |
| JavaScript, TypeScript, C, C++ | none; an `if` chain | `if (y === 0)` or `if (y == 0)` | `if (x === y)` or `if (x == y)` |

- **Python.** A tuple pattern looks like the tuple it matches. The tutorial shows the same idea for a point: "Patterns can look like unpacking assignments, and can be used to bind variables"[^1]. In `case (_, 0):`, the `_` matches any x, and the `0` matches only a y of 0. For the diagonal, "We can add an if clause to a pattern, known as a “guard”. If the guard is false, match goes on to try the next case block"[^1].
- **Rust.** The solution matches the tuple `(x, y)`. "A match guard is an additional if condition, specified after the pattern in a match arm, that must also match for that arm to be chosen"[^2]. The Rust Reference states that a guard comes after the pattern, "following the if keyword"[^9]. The last arm is `_`, because a match must cover each value.
- **C#.** A switch expression can match a tuple with a positional pattern: "You can also match expressions of tuple types against positional patterns"[^3]. "A case guard is another condition that must be satisfied together with a matched pattern"[^4], and it comes after the `when` keyword[^4].
- **Java.** Java has no tuples, so the solution puts x and y in a record. A record pattern such as `Point(int a, int b)` takes the record apart. JEP 440 adds "record patterns to deconstruct record values"[^5]. JEP 441 allows "when clauses in switch blocks to specify guards to pattern case labels"[^6]. Both are standard in Java 21[^5][^6]. In this solution, each case except the last has a guard.
- **Go.** Go has no pattern matching. A switch with no expression tests each case as a condition, because "A missing switch expression is equivalent to the boolean value true"[^7].
- **JavaScript and TypeScript.** JavaScript has no `match` statement. A TC39 proposal for pattern matching exists, but it is at stage 1[^10]. The solution tests the conditions in order with an `if` chain.
- **C and C++.** These languages have no pattern matching on values, so the solution is an `if` chain.
- **Bash.** A `case` command matches a string against glob patterns, "according to the rules described below in Pattern Matching"[^8]. The solution makes the string `x,y`, so `*,0` matches each point on the x-axis. A glob cannot compare two parts of the string, so the last clause uses an `if` in place of a guard.

### A match on an enum

A match can also take apart an enum with data, and a guard can refine each variant. The Rust Book states that "the pattern to destructure an enum corresponds to the way the data stored within the enum is defined"[^2]. Here a guard separates a square from the other rectangles:

```rust run id=rust-enum-guard
enum Shape {
    Circle(f64),
    Rect(f64, f64),
}

fn describe(shape: &Shape) -> String {
    match shape {
        Shape::Circle(r) if *r == 0.0 => "a point".to_string(),
        Shape::Circle(r) => format!("a circle of radius {r}"),
        Shape::Rect(w, h) if w == h => format!("a square of side {w}"),
        Shape::Rect(w, h) => format!("a rectangle of {w} by {h}"),
    }
}

fn main() {
    let shapes = [
        Shape::Circle(0.0),
        Shape::Circle(1.5),
        Shape::Rect(2.0, 2.0),
        Shape::Rect(2.0, 3.0),
    ];
    for shape in &shapes {
        println!("{}", describe(shape));
    }
}
```

```output for=rust-enum-guard
a point
a circle of radius 1.5
a square of side 2
a rectangle of 2 by 3
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `origin`: the point (0, 0) is on both axes and on the diagonal. The case for the origin must come first, or a later case takes the point.
- `negative-x-axis` and `negative-diagonal`: negative coordinates. The patterns test only for 0, so the sign does not change the result.
- `anti-diagonal`: (3, -3) has the same size of x and y, but not the same value, so it is `"other"`.
- `one-one`: the point nearest to the origin on the diagonal.

## Pitfalls

- **A guard before a literal pattern.** The cases are tried in order, so a wide case before a narrow case takes its values. Here the diagonal case comes first, and the origin is on the diagonal:

```python wrong id=python-guard-first
def classify(x, y):
    match (x, y):
        case (a, b) if a == b:
            return "diagonal"
        case (0, 0):
            return "origin"
        case (_, 0):
            return "x-axis"
        case (0, _):
            return "y-axis"
        case _:
            return "other"


print(classify(0, 0), classify(4, 4), classify(5, 0))
```

```output for=python-guard-first
diagonal diagonal x-axis
```

Put the narrow case `(0, 0)` first.

- **A guard and exhaustiveness.** A compiler that checks exhaustiveness does not look inside a guard. The Rust Book states that "the compiler doesn’t try to check for exhaustiveness when match guard expressions are involved"[^2]. These two guards cover each pair, but the match does not compile:

```rust fails expect=compile id=rust-guards-not-exhaustive
fn classify(x: i32, y: i32) -> &'static str {
    match (x, y) {
        (a, b) if a == b => "diagonal",
        (a, b) if a != b => "not diagonal",
    }
}

fn main() {
    println!("{}", classify(1, 2));
}
```

The fix is an arm without a guard as the last arm, such as `_ => "not diagonal"`.

- **A name that binds.** In a pattern, a name such as `a` does not compare. It binds the value, and it matches each value. In the Python tutorial example, "the variable binds a value from the subject (point)"[^1]. So `case (a, 0):` matches each point on the x-axis, and does not test that x equals some earlier variable `a`. To compare with a variable, use a guard, such as `case (a, 0) if a == limit:`.
- **The binding happens before the guard.** In Python, "value capture happens before the guard is evaluated"[^1]. So a case that fails its guard can still change a name. The language reference warns: "Do not rely on bindings being made for a failed match. Conversely, do not rely on variables remaining unchanged after a failed match"[^11]. Use a name only in the case that bound it.

## Related tasks

- [[write-a-switch-or-match]]: a match on one value with several values in one case.
- [[write-a-conditional]]: the same order rule in an `if` chain.
- [[destructure-a-value]]: take a tuple or a record apart without a test.

## References
