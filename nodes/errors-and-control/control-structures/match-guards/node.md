---
tag: ZZRHD3
kind: concept
slug: match-guards
name: Match guards
aliases: [guard, guards, case guard, pattern guard, guard condition, guard clause in a match, when clause, at binding, as pattern]
area: errors-and-control/control-structures
level: core
summary: >-
  A match guard is a boolean condition on a match arm. The arm is chosen only when its pattern matches and its guard is true; otherwise the match tries the next arm.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1997
  people: [Simon Peyton Jones]
  note: In April 1997, Simon Peyton Jones proposed pattern guards for Haskell, in which a guard can itself match a pattern.
see_also: [pattern-matching, exhaustive-matching, pattern-match-on-a-value, switch-statement, write-a-switch-or-match, conditional]
sources:
- url: https://docs.python.org/3.15/tutorial/controlflow.html#match-statements
  title: More Control Flow Tools, The Python Tutorial
  locator: 4.7 match Statements
- url: https://doc.rust-lang.org/book/ch19-03-pattern-syntax.html#adding-conditionals-with-match-guards
  title: Pattern Syntax, The Rust Programming Language
  locator: Adding Conditionals with Match Guards; Matching Named Variables; Using @ Bindings
- url: https://doc.rust-lang.org/reference/expressions/match-expr.html#match-guards
  title: The Rust Reference, Match expressions
  locator: Match guards
- url: https://peps.python.org/pep-0634/
  title: "PEP 634: Structural Pattern Matching: Specification"
  locator: Guards; the header (Python-Version 3.10)
- url: https://kotlinlang.org/docs/control-flow.html#guard-conditions-in-when-expressions
  title: Conditions and loops, Kotlin documentation
  locator: Guard conditions in when expressions
- url: https://docs.python.org/3.15/reference/compound_stmts.html#guards
  title: Compound statements, The Python Language Reference
  locator: 8.6.1 Overview; 8.6.2 Guards; 8.6.4.2 AS Patterns
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/switch-expression#case-guards
  title: switch expression, C# language reference
  locator: Case guards
- url: https://openjdk.org/jeps/441
  title: "JEP 441: Pattern Matching for switch"
  locator: Case refinement; Dominance of case labels; the header (Release 21)
- url: https://go.dev/ref/spec#Expression_switches
  title: The Go Programming Language Specification
  locator: Expression switches
- url: https://github.com/tc39/proposal-pattern-matching
  title: ECMAScript Pattern Matching, TC39 proposal
  locator: README, Status
- url: https://www.gnu.org/software/bash/manual/html_node/Conditional-Constructs.html
  title: Conditional Constructs, Bash Reference Manual
  locator: case
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/patterns#relational-patterns
  title: Pattern matching, C# language reference
  locator: Relational patterns
- url: https://kotlinlang.org/docs/whatsnew22.html
  title: What's new in Kotlin 2.2.0
  locator: the header (Released June 23, 2025); Language
- url: https://en.wikipedia.org/wiki/Guard_(computer_science)
  title: Guard (computer science), Wikipedia
  locator: Terminology; Multiple guards; Evolution; Pattern guard
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.11.1
  title: The Java Language Specification, Java SE 25 Edition
  locator: 14.11.1 Switch Blocks
---

## Idea

A pattern can test the shape of a value and constants inside it. It cannot compare two parts of the value, or call a function. A **match guard** adds such a test. It is a boolean condition after the pattern of a match arm, and the arm is chosen only when the pattern matches and the condition is true. The Python tutorial states: "We can add an if clause to a pattern, known as a “guard”. If the guard is false, match goes on to try the next case block"[^1].

In this Python function, the second case needs a guard, because no pattern can test that `a` divides by `b`:

```python run id=idea
def divide(cmd):
    match cmd:
        case ("div", _, 0):
            return "error: divide by zero"
        case ("div", a, b) if a % b == 0:
            return a // b
        case ("div", a, b):
            return a / b
        case ("add", a, b):
            return a + b


for cmd in [("div", 6, 0), ("div", 6, 3), ("div", 7, 2), ("add", 1, 2)]:
    print(divide(cmd))
```

```output for=idea
error: divide by zero
2
3.5
3
```

The command `("div", 7, 2)` matches the pattern of the second case, but its guard is false, so the third case takes it.

## How it works

- **First the pattern, then the guard.** The guard runs only after its pattern matches, and it can use the names that the pattern bound. The Rust Book defines a guard as "an additional if condition, specified after the pattern in a match arm, that must also match for that arm to be chosen"[^2]. The Rust Reference states: "When the pattern matches successfully, the pattern guard is executed"[^3]. In Python, "value capture happens before the guard is evaluated"[^1].
- **A false guard goes on to the next arm.** A false guard does not end the match. The arm is skipped, and the arms after it are tried in order. PEP 634 states that "guard evaluation must happen in order", and "Guard evaluation must stop once a case block is selected"[^4].
- **One guard for each arm.** An arm has at most one guard, but the guard can be any boolean expression. To combine conditions, use `and`, `&&` or `||` in the guard.
- **A guard on an or-pattern.** When an arm has several patterns joined with `|`, the guard applies to each of them. The Rust Book states that "the match guard condition will apply to all the patterns"[^2].
- **A guard is not a pattern.** A name in a pattern binds a new variable. A name in a guard reads a variable. The Rust Book explains this for the guard `if n == y`: "The match guard if n == y is not a pattern and therefore doesn’t introduce new variables"[^2]. So a guard is the way to compare a part of the value with a variable from outside the match.
- **Guards and exhaustiveness.** A compiler that checks that a match covers each value does not look inside guards. The Rust Book states that "the compiler doesn’t try to check for exhaustiveness when match guard expressions are involved"[^2]. Kotlin gives the same rule: "If you use guard conditions in when expressions without an else branch, the compiler requires you to handle every possible case to avoid runtime errors"[^5]. So the last arm of a match is usually an arm without a guard.

## In languages

| Language | Guard | Example |
|---|---|---|
| Python | `if` after the pattern of a `case`[^6] | `case (a, b) if a == b:` |
| Rust | `if` after the pattern of an arm[^3] | `(a, b) if a == b =>` |
| C# | `when` after the pattern of a switch expression arm[^7] | `var (a, b) when a == b =>` |
| Java | `when` after the pattern of a case label[^8] | `case Point(int a, int b) when a == b ->` |
| Kotlin | `if` after the condition of a `when` branch with a subject[^5] | `is Animal.Cat if !animal.mouseHunter ->` |
| Go | none; a switch with no expression tests conditions[^9] | `case x == y:` |
| JavaScript, TypeScript | none; no `match` statement[^10] | an `if` chain |
| C, C++ | none; a case takes only a constant | an `if` chain |
| Bash | none; a `case` pattern is a glob[^11] | an `if` in the clause |

- **Python.** A guard takes "the form: if followed by an expression"[^6], after the patterns of a `case`. "If the pattern succeeds, the corresponding guard (if present) is evaluated"[^6]. If the guard is true, the case block is selected.
- **Rust.** "Pattern guards appear after the pattern following the if keyword and consist of an Expression with a boolean type or a conditional let match"[^3]. So a guard can also match a second pattern with `let`.
- **C#.** "A case guard is another condition that must be satisfied together with a matched pattern"[^7]. C# also has relational patterns, which "compare an expression result with a specified constant"[^12]. So a pattern such as `< 0` does a test that needs a guard in Python or Java.
- **Java.** JEP 441 allows "when clauses in switch blocks to specify guards to pattern case labels"[^8]. "Only pattern labels can have guards"[^8], so `case 0 when ...` is not valid. JEP 441 is standard in Java 21[^8].
- **Kotlin.** "Guard conditions allow you to include more than one condition to the branches of a when expression or statement"[^5]. "The code in a branch with a guard condition runs only if both the primary condition and the guard condition evaluate to true"[^5]. Kotlin 2.2.0 made guard conditions stable[^13].
- **Go, JavaScript, TypeScript, C, C++ and Bash.** These languages have no guards. Go has a switch with no expression, which is "equivalent to the boolean value true"[^9], so each case is a condition. A JavaScript proposal for pattern matching exists, but it is at stage 1[^10].

## Variations and related ideas

- **Guards outside a match.** Wikipedia gives a wider meaning: "a guard is a Boolean expression that must evaluate to true if the execution of the program is to continue in the branch in question"[^14]. The guard clause at the start of a function, which returns early for a bad argument, uses the same word[^14].
- **Several guards in order.** In Haskell, a function equation can have a list of guards. "If there are several parallel guards, they are normally tried in a top-to-bottom order, and the branch of the first to pass is chosen"[^14]. This is the same rule as for the arms of a match.
- **Pattern guards.** A pattern guard is a guard that is itself a pattern match. "In effect, a match of the pattern is taken to mean pass"[^14]. A Rust guard can be "a conditional let match"[^3], which is a guard of this kind.
- **Binding a tested value.** A guard is not the only way to test a value and keep it. With a range pattern, Rust can test and bind in the pattern: "The at operator @ lets us create a variable that holds a value at the same time we’re testing that value for a pattern match"[^2]. So `id @ 3..=7` replaces `id if id >= 3 && id <= 7`. Python has the AS pattern: "An AS pattern matches an OR pattern on the left of the as keyword against a subject"[^6], so `case 1 | 2 | 3 as n:` binds the value that matched. A pattern of this kind counts in the exhaustiveness check, but a guard does not[^2].
- **Dominance.** Java checks the order of the case labels. "It is a compile-time error if any switch label in a switch block is dominated"[^15]. "An unguarded pattern case label dominates a guarded pattern case label that has the same pattern"[^8]. So a label with a guard must come before the same label without a guard.

## Pitfalls

### A name that shadows in place of a guard

A name in a pattern binds a new variable, even if a variable with that name exists outside the match. The Rust Book shows this with the pattern `Some(y)`: "Because each of these kinds of expressions starts a new scope, variables declared as part of a pattern inside these expressions will shadow those with the same name outside the constructs"[^2]. Here the second arm was meant to match only 10:

```rust wrong id=rust-shadow
fn main() {
    let y = 10;
    for x in [Some(5), Some(10)] {
        match x {
            Some(50) => println!("got 50"),
            Some(y) => println!("matched, y = {y}"),
            None => println!("none"),
        }
    }
}
```

```output for=rust-shadow
matched, y = 5
matched, y = 10
```

The arm `Some(y)` matches each `Some`, and the outer `y` is not used. The fix is a new name and a guard: `Some(n) if n == y`. Then only `Some(10)` takes the arm, because "This y is the outer y rather than a new y shadowing it"[^2].

### A guard that the exhaustiveness check cannot see

The compiler treats a guarded arm as an arm that can fail. These two guards cover each `Integer`, but the switch does not compile, because the compiler does not look inside the guards:

```java fails expect=compile id=java-guards-not-exhaustive
public class Main {
    static String sign(Integer n) {
        return switch (n) {
            case Integer k when k < 0 -> "negative";
            case Integer k when k >= 0 -> "not negative";
        };
    }

    public static void main(String[] args) {
        System.out.println(sign(-1));
    }
}
```

The fix is a last label without a guard: `case Integer k` with no `when`. Rust gives the same error for a match with guards on each arm[^2].

### A guarded label after the same label without a guard

The arms are tried in order, so an arm without a guard takes each value that a later arm with the same pattern would take. In Python and Rust, the later arm never runs. In Java, the order is a compile-time error, because the first label dominates the second[^15][^8]:

```java fails expect=compile id=java-dominated-guard
public class Main {
    static String size(String s) {
        return switch (s) {
            case String t -> "text";
            case String t when t.isEmpty() -> "empty";
        };
    }

    public static void main(String[] args) {
        System.out.println(size(""));
    }
}
```

Put the label with the guard first.

### A guard with side effects

A guard is an ordinary expression, so it can change state. Python allows this: "Guards are allowed to have side effects as they are expressions"[^6]. But a guard can run more often than it seems. In Rust, an arm with an or-pattern runs its guard once for each alternative that matches: "Multiple matches using the | operator can cause the pattern guard and the side effects it has to execute multiple times"[^3]. In this program, the value 1 matches both alternatives, so the guard runs twice:

```rust run id=rust-guard-twice
use std::cell::Cell;

fn main() {
    let calls = Cell::new(0);
    match 1 {
        1 | _ if {
            calls.set(calls.get() + 1);
            false
        } => println!("first arm"),
        _ => println!("second arm"),
    }
    println!("the guard ran {} times", calls.get());
}
```

```output for=rust-guard-twice
second arm
the guard ran 2 times
```

Keep guards free of side effects.

## Cost

- **A guard is a test at run time.** A guard runs each time its pattern matches, and it costs what its expression costs. A guard that calls a slow function makes each match slow.
- **Guards limit the compiler.** A compiler can merge and reorder the tests of patterns, but it must run guards in order[^4], because a guard can have side effects[^6]. So a match with many guards is close to an `if` chain.
- **Repeated runs.** In Rust, a guard on an or-pattern can run once for each alternative[^3].

## History

- **1963 to 1966.** "A simple conditional expression, already present in CPL in 1963, has a guard on first sub-expression"[^14]. In 1966, ISWIM had a conditional expression with no required last case, "thus separating guard from the concept of choosing either-or"[^14].
- **KRC.** Wikipedia names KRC, a small version of SASL, as "one of the first programming languages to use the term" guard[^14]. A function definition had several clauses, and a guard after each clause chose the clause to apply[^14].
- **1997.** The idea of a pattern guard "was introduced in a proposal for Haskell by Simon Peyton Jones titled A new view of guards in April 1997"[^14].
- **2020.** PEP 634, created on 12 September 2020, specified the Python `match` statement with guards for Python 3.10[^4].
- **2023.** JEP 441 added guards with `when` to Java 21[^8].
- **2025.** Kotlin 2.2.0, released on 23 June 2025, made guard conditions in `when` stable[^13].

## References
