---
tag: ZQPDKQ
kind: concept
slug: exhaustive-matching
name: Exhaustive matching
aliases: [exhaustiveness checking, exhaustiveness check, exhaustive switch, exhaustive pattern matching, non-exhaustive patterns, complete case analysis, totality check]
area: errors-and-control/control-structures
level: core
summary: >-
  A match is exhaustive when each possible value matches one of its arms. Some compilers check this, so a missing case, such as a new variant of an enum, becomes a compile-time error.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2007
  people: [Luc Maranget]
  note: Luc Maranget defined two anomalies of ML pattern matching, non-exhaustive matches and useless clauses, and gave an algorithm that finds them. He integrated it in the Objective Caml compiler.
see_also: [pattern-matching, match-guards, switch-statement, pattern-match-on-a-value, write-a-switch-or-match]
sources:
- url: https://doc.rust-lang.org/book/ch06-02-match.html#matches-are-exhaustive
  title: The match Control Flow Construct, The Rust Programming Language
  locator: Matches Are Exhaustive
- paper: doi:10.1017/S0956796807006223
  title: Luc Maranget, Warnings for pattern matching, Journal of Functional Programming 17(3), 2007, pp. 387-421
  locator: Abstract; 1 Introduction
- url: https://openjdk.org/jeps/441
  title: "JEP 441: Pattern Matching for switch"
  locator: Goals; Exhaustiveness of switch expressions and statements; the header (Release 21)
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/switch-expression#nonexhaustive-switch-expressions
  title: switch expression, C# language reference
  locator: Nonexhaustive switch expressions; the example with the Direction enumeration
- url: https://kotlinlang.org/docs/control-flow.html#when-expressions-and-statements
  title: Conditions and loops, Kotlin documentation
  locator: When expressions and statements
- url: https://kotlinlang.org/docs/sealed-classes.html#use-sealed-classes-with-when-expression
  title: Sealed classes and interfaces, Kotlin documentation
  locator: Use sealed classes with when expression
- url: https://openjdk.org/jeps/409
  title: "JEP 409: Sealed Classes"
  locator: Goals; the header (Release 17)
- url: https://www.typescriptlang.org/docs/handbook/2/narrowing.html#exhaustiveness-checking
  title: Narrowing, TypeScript Handbook
  locator: The never type; Exhaustiveness checking
- url: https://docs.python.org/3.15/tutorial/controlflow.html#match-statements
  title: More Control Flow Tools, The Python Tutorial
  locator: 4.7 match Statements
- url: https://docs.python.org/3.15/library/typing.html#typing.assert_never
  title: typing, The Python Standard Library
  locator: assert_never
- url: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
  title: Options to Request or Suppress Warnings, GCC documentation
  locator: -Wswitch; -Wswitch-enum
- url: https://go.dev/ref/spec#Expression_switches
  title: The Go Programming Language Specification
  locator: Expression switches
- url: https://www.gnu.org/software/bash/manual/html_node/Conditional-Constructs.html
  title: Conditional Constructs, Bash Reference Manual
  locator: case
- url: https://doc.rust-lang.org/reference/attributes/type_system.html#the-non_exhaustive-attribute
  title: The Rust Reference, Type system attributes
  locator: The non_exhaustive attribute
- url: https://doc.rust-lang.org/book/ch19-03-pattern-syntax.html#adding-conditionals-with-match-guards
  title: Pattern Syntax, The Rust Programming Language
  locator: Adding Conditionals with Match Guards
---

## Idea

A match is **exhaustive** when each value that can reach it matches at least one arm. A match that is not exhaustive has a gap: some value finds no arm. Rust makes the check a rule of the language: "The arms’ patterns must cover all possibilities"[^1]. "Matches in Rust are exhaustive: We must exhaust every last possibility in order for the code to be valid"[^1].

The check is most useful for a type with a fixed list of cases, such as an enum. This match names each variant, so it compiles:

```rust run id=idea
enum Shape {
    Circle(f64),
    Square(f64),
    Triangle(f64, f64),
}

fn area(shape: &Shape) -> f64 {
    match shape {
        Shape::Circle(r) => 3.0 * r * r,
        Shape::Square(s) => s * s,
        Shape::Triangle(b, h) => b * h / 2.0,
    }
}

fn main() {
    let shapes = [Shape::Circle(1.0), Shape::Square(2.0), Shape::Triangle(3.0, 4.0)];
    for shape in &shapes {
        println!("{}", area(shape));
    }
}
```

```output for=idea
3
4
6
```

If someone adds a variant `Hexagon` to `Shape`, this match no longer compiles, and the error names the missing pattern, because the compiler "even knows which pattern we forgot"[^1]. So the compiler finds each place in the program that must handle the new case.

## How it works

- **The cases of a type.** To check a match, the compiler must know each case of the type. An enum, a `bool`, a sealed type and a tuple of these have a finite list of cases. An integer or a string has too many cases to list, so a match on it needs a catch-all arm. The Rust Book shows a match on a `u8` that "compiles, even though we haven’t listed all the possible values a u8 can have, because the last pattern will match all values not specifically listed"[^1].
- **Two anomalies.** Maranget names the two checks that a compiler does on a match. "This style of reasoning calls for two basic checks: Are all cases considered? And, is any case subsumed by some others?"[^2] The first finds a non-exhaustive match. The second finds a useless arm, which no value can reach. His paper gives "a simple algorithm to detect them"[^2], which the Objective Caml compiler uses[^2].
- **The error names the gap.** The Rust compiler reports the patterns that no arm covers. For a match on an `Option<i32>` without a `None` arm, the error is "non-exhaustive patterns: `None` not covered"[^1].
- **Guards do not count.** The check looks at patterns only. A guard is an arbitrary expression, so the compiler treats a guarded arm as an arm that can fail. The Rust Book states that "the compiler doesn’t try to check for exhaustiveness when match guard expressions are involved"[^15]. [[match-guards]] shows the error that this gives.
- **Statements and expressions.** A match that gives a value must give a value for each input. JEP 441 states: "A switch expression requires that all possible values of the selector expression be handled in the switch block; in other words, it must be exhaustive"[^3]. Kotlin makes the same split: a `when` expression must be exhaustive, but "If you use when as a statement, you don't need to cover all possible cases"[^5].

## In languages

| Language | What checks the match | A gap gives |
|---|---|---|
| Rust | the compiler, for each `match`[^1] | a compile-time error[^1] |
| Java | the compiler, for each switch expression and each pattern switch[^3] | a compile-time error |
| Kotlin | the compiler, for each `when` expression[^5][^6] | a compile-time error |
| C# | the compiler, for each switch expression[^4] | a warning, and an exception at run time[^4] |
| TypeScript | the type checker, with an assignment to `never`[^8] | a compile-time error from that assignment |
| Python | a static type checker, with `assert_never`[^10] | no error from the interpreter; no case runs[^9] |
| C, C++ | GCC with `-Wswitch`, for a switch on an enum[^11] | a warning |
| Go | nothing | no case runs[^12] |
| JavaScript, Bash | nothing | no case runs[^13] |

- **Rust.** Each `match` must be exhaustive. The Rust Book shows the error for a missing `None`, and adds: "Rust knows that we didn’t cover every possible case"[^1].
- **Java.** A switch expression must be exhaustive[^3]. JEP 441 extends the rule to switch statements with patterns, and one of its goals is "requiring that pattern switch statements cover all possible input values"[^3]. Sealed classes give the compiler the list of subclasses: JEP 409 has the goal of "providing a foundation for the exhaustive analysis of patterns"[^7].
- **Kotlin.** "Covering all possible cases is called being exhaustive"[^5], and "If you use when as an expression, you must cover all possible cases"[^5]. With a sealed class, "The when expression, used with a sealed class, allows the Kotlin compiler to check exhaustively that all possible cases are covered"[^6].
- **C#.** "In most cases, the compiler generates a warning if a switch expression doesn't handle all possible input values"[^4]. At run time, "If none of a switch expression's patterns matches an input value, the runtime throws an exception"[^4].
- **TypeScript.** The language does not check a switch. But "The never type is assignable to every type; however, no type is assignable to never (except never itself)"[^8]. So in the `default` clause of a switch on a union, an assignment to a `never` variable compiles only when each member of the union has a case. The handbook calls this "exhaustive checking in a switch statement"[^8].
- **Python.** "If no case matches, none of the branches is executed"[^9]. `typing.assert_never` asks "a static type checker to confirm that a line of code is unreachable"[^10]. The check is done by the type checker, not by the interpreter.
- **C and C++.** The languages have no check. GCC has `-Wswitch`, which warns "whenever a switch statement has an index of enumerated type and lacks a case for one or more of the named codes of that enumeration"[^11]. "This warning is enabled by -Wall"[^11].
- **Go and Bash.** Nothing checks the cases. In Go, "If no case matches and there is a "default" case, its statements are executed"[^12]; with no `default`, the switch does nothing. In Bash, "The return status is zero if no pattern matches"[^13].

## Variations and related ideas

- **Sealed types and closed hierarchies.** An enum gives a list of values. A sealed type gives a list of subclasses, so a match on the type can be exhaustive without a wildcard. Java and Kotlin both use sealed types for this[^7][^6]. [[pattern-matching]] shows matches on such types.
- **Useless arms.** The second check of Maranget finds an arm that no value can reach[^2]. Java makes some of these an error: a label that is dominated by an earlier label does not compile[^3].
- **Types that can grow.** A library can mark an enum as open to new variants. In Rust, "The non_exhaustive attribute indicates that a type or variant may have more fields or variants added in the future"[^14]. Outside the crate that defines it, "When pattern matching on a non-exhaustive enum, matching on a variant does not contribute towards the exhaustiveness of the arms"[^14], so each match needs a wildcard arm.
- **Values outside the named cases.** In C, C++ and C#, an enum variable can hold an integer that is not a named member. The C# reference shows a discard arm "to handle any integer value that doesn't have the corresponding member of the Direction enumeration (for example, (Direction)10)"[^4].

## Pitfalls

### A wildcard arm that hides a new case

A wildcard arm makes each match exhaustive, so the compiler can no longer find a missing case. Here `Blue` was added to the enum after the match was written. The program compiles, and blue gets the color of the wildcard arm:

```rust wrong id=rust-wildcard-hides-variant
enum Color {
    Red,
    Green,
    Blue,
}

fn hex(c: &Color) -> &'static str {
    match c {
        Color::Red => "#ff0000",
        Color::Green => "#00ff00",
        _ => "#000000",
    }
}

fn main() {
    for c in [Color::Red, Color::Green, Color::Blue] {
        print!("{} ", hex(&c));
    }
    println!();
}
```

```output for=rust-wildcard-hides-variant
#ff0000 #00ff00 #000000
```

JEP 441 states the problem: "If we have an explicit match-all clause then we will only discover the new constant value if it shows up at run time"[^3], and "A match-all clause risks sweeping exhaustiveness errors under the rug"[^3]. Name each variant, and use a wildcard only for a type with too many values to list. In C, the same problem affects `-Wswitch`: "The presence of a default label prevents this warning"[^11]. `-Wswitch-enum` warns "about an omitted enumeration code even if there is a default label"[^11].

### A match that is not exhaustive

Without the wildcard, the match must name each variant. A match that misses one does not compile:

```rust fails expect=compile id=rust-missing-variant
enum Color {
    Red,
    Green,
    Blue,
}

fn hex(c: &Color) -> &'static str {
    match c {
        Color::Red => "#ff0000",
        Color::Green => "#00ff00",
    }
}

fn main() {
    println!("{}", hex(&Color::Blue));
}
```

The fix is the arm `Color::Blue => "#0000ff"`. This error is the point of the check: it comes at compile time, in place of a wrong color at run time.

### A gap that nothing reports

In a language without the check, a missing case does nothing. This Python function returns `None` for a value that it does not handle, and no error appears:

```python wrong id=python-silent-gap
from enum import Enum


class Color(Enum):
    RED = 1
    GREEN = 2
    BLUE = 3


def hex_code(c):
    match c:
        case Color.RED:
            return "#ff0000"
        case Color.GREEN:
            return "#00ff00"


print([hex_code(c) for c in Color])
```

```output for=python-silent-gap
['#ff0000', '#00ff00', None]
```

Add a last case that fails loudly, such as `case _ as unreachable: assert_never(unreachable)`. A static type checker then reports the gap, and the interpreter raises an exception if the case runs[^10].

### Exhaustive at compile time, not at run time

A check at compile time sees the cases that exist at compile time. In Java, a new enum constant can reach a switch in a class that was not compiled again. So "if an exhaustive enum switch does not have a match-all clause then the compiler will synthesize a default clause that throws an exception"[^3]. In C#, the runtime throws an exception when no arm matches[^4].

## Cost

- **The check runs in the compiler.** An exhaustiveness check costs nothing at run time. In the compiler, the check can be slow: Maranget notes that the related useful clause problem "is NP-complete", and that his algorithm "can exhibit exponential time behavior" in the size of the match[^2]. He adds safeguards, because the input to a compiler "consists of programs written by human beings attempting to solve specific problems, not to break the compiler"[^2].
- **A synthesized default.** Java adds a default clause that throws to an exhaustive enum switch without a match-all clause[^3], so a switch has one more branch. C# throws an exception when no arm matches[^4].
- **Edits in many places.** Adding a case to a type makes each match on the type fail to compile until it handles the case. This is the purpose of the check, but it is work for each new case.

## History

- **2007.** Luc Maranget, in "Warnings for pattern matching", examined "the ML pattern-matching anomalies of useless clauses and non-exhaustive matches"[^2]. He gave an algorithm for both, integrated it "in the Objective Caml compiler", and showed that "the same algorithm is also usable in a non-strict language such as Haskell"[^2].
- **Java 17.** JEP 409 made sealed classes standard, with the goal of exhaustive analysis of patterns[^7].
- **Python 3.11.** Python added `typing.assert_never`[^10].
- **Java 21.** JEP 441 required pattern switch statements to be exhaustive[^3].

## References
