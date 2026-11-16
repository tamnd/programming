---
tag: 7Q1CXC
kind: task
slug: write-a-switch-or-match
name: Write a switch or match
aliases: [switch on a value, match on a value, case statement, when expression, days in a month, multi-way branch]
area: errors-and-control/control-structures
level: intro
summary: >-
  Map a month number to its number of days with one switch or match, put several values in one case, and give a default case for the values that are not months.
contract: contract.yaml
category: control-flow
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3.15/tutorial/controlflow.html#match-statements
  title: More Control Flow Tools, The Python Tutorial
  locator: 4.7 match Statements
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/switch
  title: switch, MDN Web Docs
  locator: Description
- url: https://dev.java/learn/language/constructs/basics/switch-expression/
  title: Branching with Switch Expressions, Dev.java
  locator: Modifying the Switch Syntax; Producing a Value; Adding a Default Clause
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/patterns#logical-patterns
  title: Pattern matching, C# language reference
  locator: Logical patterns
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/switch-expression
  title: switch expression, C# language reference
  locator: Nonexhaustive switch expressions
- url: https://port70.net/~nsz/c/c11/n1570.html#6.8.4.2
  title: N1570, ISO/IEC 9899:201x Committee Draft (C11)
  locator: 6.8.4.2 The switch statement, paragraph 5
- url: https://en.cppreference.com/w/cpp/language/switch.html
  title: switch statement, cppreference.com
  locator: Control flow transfer
- url: https://go.dev/ref/spec#Switch_statements
  title: The Go Programming Language Specification
  locator: Switch statements; Expression switches
- url: https://doc.rust-lang.org/book/ch19-03-pattern-syntax.html
  title: Pattern Syntax, The Rust Programming Language
  locator: Matching Multiple Patterns
- url: https://doc.rust-lang.org/book/ch06-02-match.html
  title: The match Control Flow Construct, The Rust Programming Language
  locator: Matches Are Exhaustive
- url: https://www.gnu.org/software/bash/manual/html_node/Conditional-Constructs.html
  title: Conditional Constructs, Bash Reference Manual
  locator: case
- url: https://kotlinlang.org/docs/control-flow.html#when-expressions-and-statements
  title: Conditions and loops, Kotlin documentation
  locator: When expressions and statements
- url: https://dev.java/learn/language/constructs/basics/switch-statement/
  title: Branching with Switch Statements, Dev.java
  locator: Using Switch Statements to Control the Flow of Your Program
- url: https://docs.python.org/3.15/reference/compound_stmts.html#capture-patterns
  title: Compound statements, The Python Language Reference
  locator: 8.6.4.3 Literal Patterns; 8.6.4.4 Capture Patterns; 8.6.4.6 Value Patterns
---

## Task

Choose a branch by the value of one expression with a `switch`, a `match` or a `case`. The function `days_in_month(month)` takes a month number and returns the number of days in that month of a common year, that is, a year that is not a leap year:

- `28` for month 2,
- `30` for months 4, 6, 9 and 11,
- `31` for months 1, 3, 5, 7, 8, 10 and 12,
- `0` for any other int, such as 0, 13 or -1.

For example, `days_in_month(4)` is `30` and `days_in_month(13)` is `0`.

Eleven of the twelve months share a result with other months. So the solution must put several values in one case, and it needs a default case for the values that are not months. [[write-a-conditional]] chooses a branch with a chain of tests. [[switch-statement]] explains the construct itself.

## Solutions

| Language | Several values in one case | The default case | If no case matches and there is no default |
|---|---|---|---|
| Python | `case 4 \| 6 \| 9 \| 11:`[^1] | `case _:`[^1] | no branch runs[^1] |
| JavaScript, TypeScript | `case 4:` `case 6:` and so on, one label on each line | `default:`[^2] | the program continues after the `switch`[^2] |
| Java | `case 4, 6, 9, 11 ->`[^3] | `default ->` | a compile error, because a switch expression must be exhaustive[^3] |
| C# | `4 or 6 or 9 or 11 =>`[^4] | `_ =>` | a compiler warning, and an exception at run time[^5] |
| C, C++ | `case 4:` `case 6:` and so on, one label on each line | `default:` | no part of the switch body runs[^6][^7] |
| Go | `case 4, 6, 9, 11:`[^8] | `default:`[^8] | no case runs[^8] |
| Rust | `4 \| 6 \| 9 \| 11 =>`[^9] | `_ =>` | a compile error, because a match must be exhaustive[^10] |
| Bash | `4 \| 6 \| 9 \| 11)`[^11] | `*)`[^11] | no command list runs[^11] |
| Kotlin | `4, 6, 9, 11 ->`[^12] | `else ->`[^12] | a compile error when `when` is an expression[^12] |

- **Python.** A `match` statement tries the cases from the top, and "Only the first pattern that matches gets executed"[^1]. The tutorial also states that "You can combine several literals in a single pattern using | (“or”)"[^1]. The last case uses `_`, which "acts as a wildcard and never fails to match"[^1].
- **JavaScript and TypeScript.** A `switch` "looks for the first case clause whose expression evaluates to the same value as the result of the input expression (using the strict equality comparison)"[^2]. An empty `case` label does nothing, so control goes on to the next label. This is how several labels share one body. Each body ends with `return`, so no body runs on into the next one.
- **Java.** The solution uses a switch expression with arrow labels. The arrow syntax "supports multiple constants per case, separated by commas"[^3], and each arm runs only its own code. Without the `default` arm, the code does not compile, because "The cases of a switch expression must be exhaustive"[^3].
- **C#.** The solution uses a switch expression. The `or` pattern "matches an expression when either pattern matches the expression"[^4]. The `_` arm catches the other values.
- **C and C++.** These languages have no list of values in one label, so the solution stacks the labels. Each group ends with `return`. If no label matches and there is no `default`, "no part of the switch body is executed"[^6]. cppreference states the same rule for C++: "none of the statements in the switch statement will be executed"[^7].
- **Go.** A case holds a list of expressions, such as `case 4, 6, 9, 11:`[^8]. A Go case does not run on into the next case. At the end of a case, control goes to the end of the `switch`, unless the case ends with a `fallthrough` statement[^8].
- **Rust.** In a `match`, "you can match multiple patterns using the | syntax, which is the pattern or operator"[^9]. The `_` arm is necessary, because the arms must cover each value of `i32`[^10].
- **Bash.** In a `case` command, "The ‘|’ is used to separate multiple patterns in a pattern list"[^11]. The last pattern is `*`: "It’s a common idiom to use ‘*’ as the final pattern to define the default case, since that pattern will always match"[^11].
- **Kotlin.** This task has no Kotlin solution, but Kotlin `when` does the same work. `when` "evaluates its argument and compares the result against each branch in order until one branch condition is satisfied"[^12]. Commas put several conditions in one branch[^12]. When `when` is an expression, "an else branch is required"[^12] in the general case.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `december`: the last month. A month list that stops at 11 gives `0` for it.
- `zero`, `thirteen` and `minus-one`: values that are not months. Only the default case gives `0` for them.

## Pitfalls

- **A missing break.** In C, C++, JavaScript, and Java statements with colon labels, a body without `break` or `return` runs on into the next body. In C++, "case and default labels in themselves do not alter the flow of control"[^7]. A JavaScript `switch` runs the statements after the matching clause "until a break statement is encountered"[^2]. The Java tutorial states: "The break statements are necessary because without them, statements in switch blocks fall through"[^13]. Here the body for February sets `days` to 28, and then the body for April sets it to 30:

```c wrong id=c-missing-break
#include <stdio.h>

static int days_in_month(int month) {
    int days = 0;
    switch (month) {
    case 2:
        days = 28;
    case 4:
    case 6:
    case 9:
    case 11:
        days = 30;
        break;
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        days = 31;
        break;
    }
    return days;
}

int main(void) {
    printf("%d %d %d\n", days_in_month(2), days_in_month(4), days_in_month(1));
    return 0;
}
```

```output for=c-missing-break
30 30 31
```

A `break` after `days = 28;` gives `28` for February. [[fall-through]] explains this behavior and the languages that do not have it.

- **A default that hides bad input.** A default case that returns a real result makes the code shorter, but then a bad value gets that result too. Here `default` stands for the months with 31 days, so month 13 gets 31 days:

```go wrong id=go-default-31
package main

import "fmt"

func daysInMonth(month int) int {
	switch month {
	case 2:
		return 28
	case 4, 6, 9, 11:
		return 30
	default:
		return 31
	}
}

func main() {
	fmt.Println(daysInMonth(1), daysInMonth(13), daysInMonth(-1))
}
```

```output for=go-default-31
31 31 31
```

Give each valid value its own case, and keep the default case for the values that are not valid.

- **A switch with no default.** A switch statement without a default case does nothing for a value that no case matches. The Java tutorial warns: "This may be a place for bugs to hide in your application, something you want to avoid"[^3]. Rust does not compile a `match` that misses a value: "Matches in Rust are exhaustive: We must exhaust every last possibility in order for the code to be valid"[^10]. This version has no `_` arm:

```rust fails expect=compile id=rust-no-wildcard
fn days_in_month(month: i32) -> i32 {
    match month {
        2 => 28,
        4 | 6 | 9 | 11 => 30,
        1 | 3 | 5 | 7 | 8 | 10 | 12 => 31,
    }
}

fn main() {
    println!("{}", days_in_month(2));
}
```

The compiler reports error E0004, non-exhaustive patterns[^10]. A `_ => 0` arm fixes it.

- **A name in a Python case.** In Python, `case QUIT:` does not compare the value with the variable `QUIT`. It is a capture pattern: "A capture pattern binds the subject value to a name", and "Capture patterns always succeed"[^14]. So the case matches each value. [[switch-statement]] shows this mistake. A dotted name such as `case Color.RED:` is a value pattern, and it "succeeds if the value found compares equal to the subject value"[^14]. Literals such as `case 2:` also compare the value[^14].

## Related tasks

- [[write-a-conditional]]: choose a branch with a chain of tests.
- [[switch-statement]]: the concept of a switch statement in each language.
- [[fall-through]]: the concept of a case that runs on into the next case.
- [[pattern-matching]]: a match that also takes a value apart.

## References
