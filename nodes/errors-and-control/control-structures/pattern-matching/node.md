---
tag: M6NX0X
kind: concept
slug: pattern-matching
name: Pattern matching
aliases: [structural pattern matching, match expression, destructuring, case analysis, switch patterns]
area: errors-and-control/control-structures
level: core
summary: >-
  A pattern describes the shape of a value. A match tests a value against a list of patterns, takes the first one that fits, and binds the parts of the value to names in one step.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1980
  people: [R. M. Burstall, D. B. MacQueen, D. T. Sannella]
  note: HOPE declared a data type as a set of constructors, and its designers asked that a compiler check whether a case analysis over the constructors is exhaustive.
see_also: [sum-types, python/match-statement, visitor-pattern, null-references, errors-as-values, subtyping, tree-walking-interpreter]
same_as:
  wikidata: Q1503724
  wikipedia: Pattern_matching
sources:
  - url: https://doc.rust-lang.org/reference/patterns.html#refutability
    title: The Rust Reference, Patterns
    locator: Refutability, and the kinds of patterns (literal, identifier, wildcard, rest, range, struct, tuple struct, tuple, slice, path and or-patterns)
  - url: https://doc.rust-lang.org/reference/expressions/match-expr.html
    title: The Rust Reference, Match expressions
    locator: rules expr.match.scrutinee-value (the first arm with a matching pattern is chosen), expr.match.pattern-vars and expr.match.guard.next
  - url: https://doc.rust-lang.org/reference/expressions/match-expr.html#match-guards
    title: The Rust Reference, Match expressions
    locator: rule expr.match.guard.behavior, and the note on a guard that runs once for each alternative of an or-pattern
  - url: https://doc.rust-lang.org/reference/patterns.html#identifier-patterns
    title: The Rust Reference, Patterns
    locator: rules patterns.ident.precedent (path patterns take precedence over identifier patterns) and patterns.ident.scrutinized (variable @ subpattern)
  - url: https://doc.rust-lang.org/book/ch19-02-refutability.html
    title: The Rust Programming Language, 19.2 Refutability
    locator: Refutability, Whether a Pattern Might Fail to Match (let statements, function parameters and for loops take only irrefutable patterns)
  - url: https://doc.rust-lang.org/book/ch19-03-pattern-syntax.html#adding-conditionals-with-match-guards
    title: The Rust Programming Language, 19.3 Pattern Syntax
    locator: Adding Conditionals with Match Guards (the compiler does not check exhaustiveness when match guards are involved)
  - url: https://doc.rust-lang.org/book/ch19-03-pattern-syntax.html#matching-named-variables
    title: The Rust Programming Language, 19.3 Pattern Syntax
    locator: Matching Named Variables, Listing 19-11 (a variable in a pattern shadows an outer variable of the same name)
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#the-match-statement
    title: The Python Language Reference, Python 3.15
    locator: 8.6.1 Overview (the case blocks are tried in order, the guard runs after a successful match, and bindings outlive the block)
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#capture-patterns
    title: The Python Language Reference, Python 3.15
    locator: 8.6.3 Irrefutable Case Blocks, and 8.6.4.4 Capture Patterns
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#mapping-patterns
    title: The Python Language Reference, Python 3.15
    locator: 8.6.4.9 Mapping Patterns
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.30.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 14.30.1 Kinds of Patterns (type patterns and record patterns, and the match-all pattern _ inside a record pattern)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.11.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 14.11.1 Switch Blocks (a dominated switch label is a compile-time error; guarded and unguarded case labels)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.11.1.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 14.11.1.1 Exhaustive Switch Blocks (only unguarded case labels count)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/patterns
    title: C# language reference, Patterns
    locator: the list of patterns, Positional pattern (the generated code calls the Deconstruct method), and List patterns
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/patterns#closed-hierarchy-patterns
    title: C# language reference, Patterns
    locator: Closed hierarchy patterns (error CS8510 for an arm that an earlier arm already handles, as in any class hierarchy)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/switch-expression#nonexhaustive-switch-expressions
    title: C# language reference, the switch expression
    locator: Non-exhaustive switch expressions (a compiler warning, and SwitchExpressionException at run time)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/whats-new/csharp-version-history
    title: The history of C#
    locator: C# version 7.0 (March 2017), 8.0 (September 2019), 9 (November 2020) and 11 (November 2022), the pattern matching items
  - url: https://kotlinlang.org/docs/control-flow.html#when-expressions-and-statements
    title: Kotlin documentation, Conditions and loops
    locator: When expressions and statements (a when expression must cover all cases; is checks with smart casts; guard conditions)
  - url: https://kotlinlang.org/docs/whatsnew22.html#stable-features-guard-conditions-non-local-break-and-continue-and-multi-dollar-interpolation
    title: What's new in Kotlin 2.2.0
    locator: "Stable features: Guard conditions, non-local break and continue, and multi-dollar interpolation"
  - url: https://github.com/tc39/proposal-pattern-matching
    title: TC39 proposal, Pattern Matching
    locator: the Stage line (Stage 1), the match expression and the is operator
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Destructuring#default_value
    title: MDN Web Docs, Destructuring
    locator: Default value, and Destructuring primitive values (destructuring null or undefined throws a TypeError)
  - url: https://go.dev/ref/spec#Expression_switches
    title: The Go Programming Language Specification
    locator: Expression switches (case expressions are evaluated left to right and top to bottom; a missing switch expression means true)
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.8.4.2
    title: N1570, ISO/IEC 9899:201x Committee Draft (C11)
    locator: 6.8.4.2 paragraphs 1 and 3 (the controlling expression has integer type, and each case label is an integer constant expression)
  - url: https://timsong-cpp.github.io/cppwp/n4950/dcl.struct.bind
    title: N4950, Working Draft, Programming Languages C++ (C++23)
    locator: "[dcl.struct.bind] paragraphs 3, 4 and 5 (the number of names must equal the number of elements)"
  - url: https://www.gnu.org/software/bash/manual/html_node/Conditional-Constructs.html
    title: Bash Reference Manual, Conditional Constructs
    locator: case (patterns use the rules of filename expansion, and no match is tried after the first one)
  - paper: doi:10.1017/S0956796807006223
    title: Luc Maranget, Warnings for pattern matching, Journal of Functional Programming 17(3), 2007, pp. 387-421
    locator: Abstract (useless clauses and non-exhaustive matches, and an algorithm in the Objective Caml compiler)
  - paper: doi:10.1145/1411304.1411311
    title: Luc Maranget, Compiling pattern matching to good decision trees, ML 2008, pp. 35-46
    locator: 1 Introduction (decision trees and backtracking automata, and Augustsson 1985)
  - paper: doi:10.1145/800087.802799
    title: R. M. Burstall, D. B. MacQueen and D. T. Sannella, HOPE, An experimental applicative language, LFP 1980
    locator: 2 Desirable programming language features, Exhaustive case analysis (pp. 136 and 137)
  - url: https://peps.python.org/pep-0634/
    title: "PEP 634: Structural Pattern Matching: Specification"
    locator: the header (Python-Version 3.10)
  - url: https://peps.python.org/pep-0619/
    title: "PEP 619: Python 3.10 Release Schedule"
    locator: Release Schedule (3.10.0 final on Monday, 2021-10-04)
  - url: https://openjdk.org/jeps/440
    title: "JEP 440: Record Patterns"
    locator: the header (Release 21)
  - url: https://openjdk.org/jeps/441
    title: "JEP 441: Pattern Matching for switch"
    locator: the header (Release 21)
  - url: https://openjdk.org/projects/jdk/21/
    title: JDK 21
    locator: the schedule (General Availability on 19 September 2023)
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_switch
    title: PowerShell documentation, about_Switch
    locator: Syntax (the Wildcard and Regex parameters), and Use break and continue to control the flow (the action of each matching condition runs)
---

## Idea

Code often asks two questions about a value at the same time. What shape does it have? And what is inside it? A command is a list of words. If the first word is `go` and there is one more word, that word is the direction. Without patterns, the program first tests the length, then tests the first word, then reads the second word by its index. The test and the read are separate steps, and a mistake in one of them gives a wrong read.

A pattern puts the two steps together. It looks like the value that it matches, with names in the places of the parts. A `match` tries its patterns from top to bottom. The first pattern that fits the value wins, and its names are bound to the parts of the value.

```rust run id=idea
fn run(command: &[&str]) -> String {
    match command {
        [] => "nothing to do".to_string(),
        ["go", direction] => format!("you go {direction}"),
        ["take", item] => format!("you take the {item}"),
        ["take", first, rest @ ..] => format!("you take the {first} and {} more", rest.len()),
        [verb, ..] => format!("unknown command: {verb}"),
    }
}

fn main() {
    let commands: [&[&str]; 5] = [
        &["go", "north"],
        &["take", "lamp"],
        &["take", "lamp", "key", "map"],
        &["dance"],
        &[],
    ];
    for command in commands {
        println!("{}", run(command));
    }
}
```

```output for=idea
you go north
you take the lamp
you take the lamp and 2 more
unknown command: dance
nothing to do
```

`["go", direction]` matches a slice of exactly two elements whose first element is `"go"`. It binds the second element to `direction`. `rest @ ..` binds the remaining elements to `rest`.[^4] The arm `["take", item]` comes before `["take", first, rest @ ..]`, so a command with one item takes the first of the two arms. The last arm matches each slice that is not empty, and the first arm matches the empty slice. The compiler checks that the arms cover every slice.

## How it works

**Two jobs in one step.** A pattern tests a value and binds names. The test can look at the type of the value, at its length, at constants inside it, and at the parts of its parts. The names are bound only if the whole test succeeds. In Rust, the names are visible in the guard and in the body of the arm, and not after the `match`.[^2] In Python, the names stay bound after the `match` statement, and a failed match can leave some names bound.[^8]

**The kinds of patterns.** Most languages with patterns have the same small set. Each kind of pattern can hold other patterns, so a pattern can describe a value at any depth.

| Kind | Matches | Rust | Python | C# | Java |
|---|---|---|---|---|---|
| literal | a value equal to a constant | `0`, `"go"` | `0`, `"go"` | `0`, `"go"` | none |
| wildcard | any value, binds nothing | `_` | `_` | `_` | `_` in a record pattern |
| variable | any value, and binds it | `x` | `x` | `var x` | `int x` |
| type | a value of a run-time type | none | `int()` | `string s` | `String s` |
| record or class | a value of a type, and its fields | `Point { x, y }` | `Point(x=0)` | `Point(0, var y)` | `Point(int x, int y)` |
| tuple and sequence | a fixed number of elements, or a rest | `(a, b)`, `[a, ..]` | `(a, b)`, `[a, *rest]` | `(a, b)`, `[a, ..]` | none |
| mapping | a dictionary that has some keys | none | `{"type": t}` | none | none |
| range or relation | a value in a range | `1..=9` | none | `>= 1 and <= 9` | none |
| or | a value that one of the alternatives matches | `A \| B` | `A \| B` | `A or B` | none |
| binding | the value of a sub-pattern, and binds it | `x @ 1..=9` | `[1, 2] as p` | `Point(> 0, _) p` | none |

The sources for each column are the Rust Reference, the Python Language Reference, the C# reference and the JLS.[^1][^8][^14][^11] A Java pattern is a type pattern or a record pattern, and a record pattern holds only patterns of these kinds and `_`.[^11] So Java has no constant inside a record pattern, and a test for zero goes in a guard (see the Java example).

**The order of the arms.** The arms are tried from top to bottom, and the first arm that matches is chosen.[^2][^8] So the order of the arms is part of the meaning. A specific arm must come before a general arm that also matches its values. A Go `switch` has the same rule for its cases.[^22]

**Guards.** A guard is a boolean condition after a pattern, written `if` in Rust and Python and `when` in C# and Java. It runs only after the pattern matches, and it can use the names that the pattern bound. If the guard is false, the match goes on to the next arm.[^3][^8] A guard can test what a pattern cannot, such as `x == y` for two parts of the same value.

**Refutable and irrefutable patterns.** A pattern that can fail is refutable, such as `Some(x)` or `(0, y)`. A pattern that matches every value of its type is irrefutable, such as `x` or `(a, b)` for a pair.[^1] Rust lets a `let` statement, a function parameter and a `for` loop take only irrefutable patterns, because the program has nothing to do when the value does not match.[^5] So `let (x, y) = point;` takes a pair apart. `if let` and `let else` take refutable patterns and give code for the failure. In Python, a case block whose pattern cannot fail must be the last one.[^9]

**Destructuring without a test.** Some languages have the binding half of patterns but not the testing half. JavaScript destructuring binds names to the properties of an object, but a missing property gives `undefined` and no error. Destructuring `null` or `undefined` throws a `TypeError`.[^21] A C++ structured binding takes apart a pair, a tuple, an array or a struct, and the number of names must equal the number of elements. A wrong count is a compile error.[^24] Neither form can choose between several shapes.

**What the compiler checks.** A compiler that knows the patterns can find two mistakes. A match is non-exhaustive when some value matches no arm. An arm is useless when the arms above it already match each value that it matches.[^26]

| Language | Non-exhaustive match | Useless arm |
|---|---|---|
| Rust | error E0004 | warning `unreachable pattern` |
| Java, `switch` with patterns | compile error | compile error, the label is dominated[^12] |
| C#, `switch` expression | warning, then `SwitchExpressionException` at run time[^16] | error CS8510[^15] |
| Python | nothing; the statement does nothing | `SyntaxError` only after an irrefutable pattern[^9] |
| Go, C, JavaScript | no patterns | no patterns |

The checks do not look inside guards. The compiler treats a guarded arm as an arm that may fail. The Rust book states that the compiler does not check exhaustiveness when match guards are involved.[^6] In Java, only unguarded case labels count when the compiler checks that a `switch` covers its type.[^13] The exhaustiveness check over the cases of a type is described in [[sum-types]].

**Compiling a match.** The compiler turns the patterns into a sequence of simple tests. Luc Maranget describes two kinds of result. A decision tree never tests a part of the value more than once, but its code can grow very large. A backtracking automaton, first described by Lennart Augustsson in 1985, has code whose size grows linearly with the patterns, but it can test a part of the value again after it backtracks.[^27]

## In languages

The example files classify the same five points in each language: the origin, a point on an axis, a point on the diagonal, and any other point. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | native | `match` with literal, capture, sequence, mapping, class, OR and AS patterns, and guards[^8] |
| JavaScript | idiom | destructuring and a chain of `if` tests; a `match` proposal is at Stage 1[^20][^21] |
| TypeScript | idiom | as JavaScript, and a `switch` on a literal field that narrows a union |
| Java | native | type and record patterns in `switch` and `instanceof`, and guards after `when`[^11] |
| C | absent | `switch` on integer constants and a chain of `if` tests[^23] |
| C++ | absent | structured bindings and a chain of `if` tests; `std::visit` for a variant[^24] |
| C# | native | positional, property, relational, logical and list patterns in `is` and `switch`[^14] |
| Go | idiom | a `switch` with conditions; a type switch for interfaces[^22] |
| Rust | native | `match`, `if let`, `let else`, and patterns in `let` and parameters[^1][^5] |
| Kotlin | idiom | `when` with `is` checks, smart casts and guard conditions[^18] |
| PowerShell | absent | `switch` with `-Wildcard` or `-Regex` on strings[^34] |
| Bash | absent | `case` with glob patterns on a string[^25] |
| SQL | absent | a `CASE` expression with conditions |
| Regex | n/a | a regular expression matches strings, not structured values |
| HTML/CSS | n/a | no values to take apart |

**Python.** The `match` statement has all the kinds of patterns in the table. A mapping pattern matches a dictionary that has the listed keys, and it ignores other keys.[^10] So it suits data from `json.loads` (see the Python example). [[python/match-statement]] describes each pattern in detail.

**Java.** A record pattern takes a record apart by its components, and its components can be record patterns again. There is no constant pattern inside a record pattern, so the Java example tests the zeros in guards.[^11]

**C#.** A positional pattern calls the `Deconstruct` method of the type, and a tuple matches by position.[^14] Relational patterns such as `>= 3 and < 6` test a range, and list patterns, from C# 11, take apart arrays and lists (see the C# example).[^14][^17]

**Kotlin.** A `when` with a subject can test types with `is`, and the subject is then smart-cast in that branch. A guard condition, written `is Animal.Cat if !animal.mouseHunter`, adds a test to a branch. A `when` used as an expression must cover all cases.[^18] Guard conditions are stable from Kotlin 2.2.0.[^19] `when` cannot take a value apart into nested parts. Kotlin is not installed on the machine that recorded the examples, so it has no example file.

**Go, C and C++.** These languages have no patterns. A Go `switch` with no expression tests its `case` conditions from top to bottom, and the first true case runs.[^22] A C `switch` compares an integer with integer constants only.[^23] In all three, a chain of `if` tests does the work of the arms, and the programmer must keep the tests and the reads in step.

**PowerShell.** A `switch` statement converts its values to strings, and it can match them as wildcard patterns or as regular expressions. Unlike a `match`, it runs the action of each condition that matches, not only the first one. A `break` in an action stops the `switch`.[^34]

**Bash.** A `case` statement matches one string against glob patterns. The patterns use the rules of filename expansion, and no match is tried after the first one.[^25] The Bash example joins the two numbers into one string, such as `3,0`, so that `*,0` can match it.

## Variations and related ideas

- **Sum types.** Patterns are the usual way to use a sum type, because a constructor pattern tests the case and binds its fields in one step. [[sum-types]] covers the exhaustiveness check over the cases of a type.
- **Visitors.** In a language with no patterns, the Visitor pattern gives one method for each class of node. A `match` with one arm for each case does the same job in one function. [[visitor-pattern]] compares the two.
- **Option and Result.** `if let Some(x)` and `match` on `Ok` and `Err` are the normal way to read an optional value or a result in Rust. [[null-references]] and [[errors-as-values]] describe these types.
- **String patterns.** Glob patterns, such as `*.txt`, and regular expressions match strings, not structured values. They share the name but not the idea. A `case` in Bash and `switch -Wildcard` in PowerShell use glob patterns.[^25][^34]
- **Interpreters.** A tree-walking interpreter is the classic use of nested patterns, with one arm for each kind of node. [[tree-walking-interpreter]] shows one.

## Pitfalls

### A name that binds instead of compares

In a pattern, a plain name is a variable that matches anything. It is a comparison only if the name refers to a constant. Rust gives path patterns, such as a constant, precedence over identifier patterns.[^4] So a misspelled constant becomes a new variable that matches each value.

```rust wrong id=rust-typo
const ORIGIN: (i32, i32) = (0, 0);

fn describe(point: (i32, i32)) -> &'static str {
    match point {
        ORIGN => "origin",
        _ => "elsewhere",
    }
}

fn main() {
    println!("{}", describe((0, 0)));
    println!("{}", describe((3, 4)));
}
```

```output for=rust-typo
origin
origin
```

`(3, 4)` is not the origin. `rustc` warns that the `_` arm is unreachable and that the variable `ORIGN` is unused, but the program builds. The fix is the correct name, `ORIGIN`. Python has the same trap in a stronger form, because a bare name in a pattern always binds, even when a constant has that name. [[python/match-statement]] shows it. The same rule makes an inner name shadow an outer variable. The Rust book shows `Some(y)` binding a new `y` in place of a comparison with the outer `y`.[^7] Use a guard, such as `Some(n) if n == y`, to compare with a variable.

### The order of the arms

The first arm that matches wins. Here the diagonal arm comes before the origin arm. The origin is also on the diagonal, so it takes the diagonal arm.

```rust wrong id=rust-order
fn describe(point: (i32, i32)) -> String {
    match point {
        (x, y) if x == y => format!("on the diagonal at {x}"),
        (0, 0) => "origin".to_string(),
        (x, y) => format!("at {x}, {y}"),
    }
}

fn main() {
    println!("{}", describe((0, 0)));
    println!("{}", describe((2, 2)));
}
```

```output for=rust-order
on the diagonal at 0
on the diagonal at 2
```

The compiler does not warn, because the first arm has a guard, and the compiler does not know what a guard does. The fix is to put the specific arm, `(0, 0)`, first. When no guard is involved, the compiler can see the mistake. In Java, a case label that an earlier label covers is a compile error.[^12]

```java fails expect=compile id=java-dominated
public class Main {
    static String describe(Object o) {
        return switch (o) {
            case CharSequence cs -> "text of length " + cs.length();
            case String s -> "a string";
            default -> "something else";
        };
    }

    public static void main(String[] args) {
        System.out.println(describe("hello"));
    }
}
```

`javac` reports `this case label is dominated by a preceding case label`. A `String` is a `CharSequence`, so the second case can never run. C# reports error CS8510 for the same mistake, and Rust gives the warning `unreachable pattern`.[^15]

### A guard that the exhaustiveness check cannot see

These three arms cover every integer. But the compiler does not reason about guards, so it must assume that both guarded arms can fail.[^6]

```rust fails expect=compile id=rust-guards
fn sign(n: i32) -> &'static str {
    match n {
        n if n < 0 => "negative",
        0 => "zero",
        n if n > 0 => "positive",
    }
}

fn main() {
    println!("{}", sign(-5));
}
```

`rustc` stops with ``error[E0004]: non-exhaustive patterns: `i32::MIN..=-1_i32` and `1_i32..=i32::MAX` not covered``. One fix is range patterns, which the compiler can check: `i32::MIN..=-1`, `0` and `1..=i32::MAX`. Another fix is a last arm without a guard, `_ => "positive"`. A guard is a test the compiler cannot read, so prefer a pattern where a pattern can say the same thing. Java has the same rule: a guarded case label does not count when the compiler checks that the `switch` is exhaustive.[^13]

### Destructuring that does not test

JavaScript destructuring binds names, but it does not check that the properties exist. A missing property gives `undefined`, and the error appears later, far from its cause.[^21]

```javascript wrong id=js-missing
function area({ width, height }) {
  return width * height;
}

console.log(area({ width: 2, height: 3 }));
console.log(area({ width: 2, heigth: 3 }));
```

```output for=js-missing
6
NaN
```

The misspelled `heigth` gives `undefined` for `height`, and the product is `NaN`. A value of `null` fails at once, because destructuring `null` throws a `TypeError`.[^21]

```javascript fails expect=exception:TypeError id=js-null
function area({ width, height }) {
  return width * height;
}

console.log(area(null));
```

The fix is an explicit test of the shape before the use, or TypeScript, which reports the misspelled property at compile time. A real `match` would choose a branch only when the shape fits.

## Cost

- **The tests.** A match does the tests that a hand-written chain of `if` statements would do. A compiler can order and share the tests. A decision tree tests each part of the value at most once, but its code can become large. A backtracking automaton keeps the code small, but it can test a part again.[^27]
- **Guards run code.** A guard is an ordinary expression, and it runs each time its pattern matches. In Rust, a guard on an arm with an or-pattern can run once for each alternative.[^3] Keep guards cheap and free of side effects.
- **Calls for some patterns.** A C# positional pattern calls the `Deconstruct` method of the type.[^14] So the cost of such a pattern includes the cost of that method.
- **The checks are free at run time.** The exhaustiveness and usefulness checks run in the compiler.[^26]

## History

- **1980.** R. M. Burstall, D. B. MacQueen and D. T. Sannella described HOPE. A `data` declaration listed the constructors of a type, and the designers asked that a compiler check whether a case analysis over the constructors is exhaustive.[^28]
- **1985.** Lennart Augustsson described the compilation of pattern matching to backtracking automata.[^27]
- **2007.** Luc Maranget defined two anomalies of ML pattern matching, useless clauses and non-exhaustive matches, and gave an algorithm that finds them. He integrated it in the Objective Caml compiler, and he showed that it also works for a non-strict language such as Haskell.[^26]
- **2008.** Maranget showed new heuristics for the compilation of patterns to decision trees, and he compared the result with optimized backtracking automata.[^27]
- **2017 to 2022.** C# 7.0, released in March 2017, added pattern matching. C# 8.0, in September 2019, added switch expressions and property, tuple and positional patterns. C# 9, in November 2020, added relational and logical patterns, and C# 11, in November 2022, added list patterns.[^17]
- **2021.** Python 3.10, released on 4 October 2021, added the `match` statement (PEP 634).[^29][^30]
- **2023.** JDK 21, released on 19 September 2023, added record patterns (JEP 440) and pattern matching for `switch` (JEP 441).[^31][^32][^33]
- **2025.** Kotlin 2.2.0 made guard conditions in `when` stable.[^19] The TC39 proposal to add pattern matching to JavaScript is at Stage 1.[^20]

## References
