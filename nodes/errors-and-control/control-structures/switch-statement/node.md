---
tag: 547T26
kind: concept
slug: switch-statement
name: Switch statement
aliases: [switch, switch statement, case statement, multiway branch, multi-way branch, select case, switch expression, case labels, default label, when expression]
area: errors-and-control/control-structures
level: core
summary: >-
  A switch statement compares one value with a list of case labels and jumps to the code of the label that matches, or to a default label when no label matches.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1966
  people: [Niklaus Wirth, C. A. R. Hoare]
  note: ALGOL W, described by Wirth and Hoare in 1966, had a case expression that used an integer to select one expression from a list.
see_also: [conditional, fall-through, write-a-switch-or-match, pattern-matching, control-flow]
same_as:
  wikidata: Q838092
  wikipedia: Switch_statement
sources:
- url: https://en.wikipedia.org/wiki/Switch_statement
  title: Switch statement, Wikipedia
  locator: Introduction; Fall through; Compilation; History; Case expression
- url: https://port70.net/~nsz/c/c11/n1570.html#6.8.4.2
  title: N1570, ISO/IEC 9899:201x Committee Draft (C11)
  locator: 6.8.4.2 The switch statement, paragraphs 1, 3 and 5
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.11
  title: The Java Language Specification, Java SE 25 Edition
  locator: 14.11 The switch Statement; 14.11.1 Switch Blocks; Example 14.11.3-1
- url: https://en.cppreference.com/w/cpp/language/switch.html
  title: switch statement, cppreference.com
  locator: Condition; Notes
- url: https://go.dev/ref/spec#Switch_statements
  title: The Go Programming Language Specification
  locator: Switch statements; Expression switches
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/switch
  title: switch, MDN Web Docs
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/selection-statements#the-switch-statement
  title: Selection statements, C# language reference
  locator: The switch statement
- url: https://www.gnu.org/software/bash/manual/html_node/Conditional-Constructs.html
  title: Conditional Constructs, Bash Reference Manual
  locator: case
- url: https://kotlinlang.org/docs/control-flow.html#when-expressions-and-statements
  title: Conditions and loops, Kotlin documentation
  locator: When expressions and statements
- url: https://docs.python.org/3.15/tutorial/controlflow.html#match-statements
  title: More Control Flow Tools, The Python Tutorial
  locator: 4.7 match Statements
- url: https://openjdk.org/jeps/361
  title: "JEP 361: Switch Expressions"
  locator: Summary; the header (Release 14)
- url: https://learn.microsoft.com/en-us/dotnet/csharp/whats-new/csharp-version-history
  title: The history of C#
  locator: C# version 8.0 (September 2019), switch expressions
- url: https://www.postgresql.org/docs/current/functions-conditional.html
  title: Conditional Expressions, PostgreSQL documentation
  locator: 9.18.1 CASE
- url: https://doc.rust-lang.org/book/ch06-02-match.html
  title: The match Control Flow Construct, The Rust Programming Language
  locator: Introduction; Matches Are Exhaustive
- url: https://www.typescriptlang.org/docs/handbook/2/narrowing.html
  title: Narrowing, TypeScript Handbook
  locator: Equality narrowing; Discriminated unions
- url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_switch
  title: about_Switch, PowerShell documentation
  locator: Long description; Use break and continue to control the flow
- url: https://docs.python.org/3.15/reference/compound_stmts.html#capture-patterns
  title: Compound statements, The Python Language Reference
  locator: 8.6.4.4 Capture Patterns; 8.6.4.6 Value Patterns
- url: https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html
  title: Options for Code Generation Conventions, GCC documentation
  locator: -fno-jump-tables
- url: https://openjdk.org/projects/jdk/14/
  title: JDK 14
  locator: the schedule (General Availability on 17 March 2020)
- url: https://peps.python.org/pep-0634/
  title: "PEP 634: Structural Pattern Matching: Specification"
  locator: the header (Python-Version 3.10)
- url: https://peps.python.org/pep-0619/
  title: "PEP 619: Python 3.10 Release Schedule"
  locator: Release Schedule (3.10.0 final on Monday, 2021-10-04)
---

## Idea

A program often has to choose one of many paths by the value of one thing: a menu choice, a command letter, a kind of token. A chain of `if` and `else if` can do it, but the chain repeats the value in each test, and a reader must check each test to see that they all compare the same value. A switch statement names the value once and lists the cases. Wikipedia describes it as "a selection control flow mechanism that changes execution control based on the value of an expression"[^1], and adds that "instead of branching only on true or false, it branches on any number of values"[^1].

```c run id=idea
#include <stdio.h>

int main(void) {
    const char *text = "a cat, 2 dogs";
    int vowels = 0, spaces = 0, punctuation = 0, other = 0;
    for (const char *p = text; *p != '\0'; p++) {
        switch (*p) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            vowels++;
            break;
        case ' ':
            spaces++;
            break;
        case ',':
        case '.':
            punctuation++;
            break;
        default:
            other++;
            break;
        }
    }
    printf("vowels %d, spaces %d, punctuation %d, other %d\n", vowels, spaces, punctuation, other);
    return 0;
}
```

```output for=idea
vowels 3, spaces 3, punctuation 1, other 6
```

The value `*p` is written once. Each `case` label is a constant, and five labels share the code that counts a vowel. The `default` label takes each character that no label names. Each group of statements ends with `break`, which leaves the `switch`. Without the `break`, C goes on into the code of the next label. That rule is the subject of [[fall-through]].

## How it works

**The value and the labels.** The value that a switch tests has different names: the controlling expression in C, the selector expression in Java. In C, "The controlling expression of a switch statement shall have integer type", and "The expression of each case label shall be an integer constant expression and no two of the case constant expressions in the same switch statement shall have the same value after conversion"[^2]. Other languages accept more types. In Java, "The type of the selector expression must be char , byte , short , int , or a reference type, or a compile-time error occurs"[^3], so a Java `switch` can test a `String` or an enum. In C++, the condition can have an integral type, an enumeration type or a class type that converts to one of them[^4]. In Go, the case expressions "need not be constants"[^5].

**The jump.** A C `switch` is a computed jump. "If a converted value matches that of the promoted controlling expression, control jumps to the statement following the matched case label. Otherwise, if there is a default label, control jumps to the labeled statement. If no converted case constant expression matches and there is no default label, no part of the switch body is executed"[^2]. Most languages keep this rule for the `default` label. In Go, the case expressions "are evaluated left-to-right and top-to-bottom; the first one that equals the switch expression triggers execution of the statements of the associated case; the other cases are skipped"[^5]. JavaScript compares "using the strict equality comparison"[^6].

**The end of a case.** In C, C++, Java and JavaScript, a case is only a label in a block of statements. Java states the rule for a group of statements: "all the block statements in the switch block that follow the switch label are executed, including those that appear after subsequent switch labels"[^3]. A `break` ends the `switch`. Other languages end each case by themselves. In Go, control goes to the end of the `switch` after a case, unless the last statement of the case is `fallthrough`[^5]. In C#, "Falling through from one switch section to the next generates a compiler error"[^7]. In Bash, a case that ends with `;;` stops after the first match, and `;&` and `;;&` continue[^8]. [[fall-through]] compares these rules.

**Several values for one case.** A case often stands for several values. C and Java put several labels in front of one group of statements, and in Java "Multiple switch labels are permitted for a switch labeled statement group"[^3]. Go lists the values in one case, as in `case 0, 1, 2, 3:`[^5]. Kotlin separates them with commas[^9], and Python joins them with `|`[^10].

**Switch expressions.** A switch statement runs code. A switch expression gives a value. Java 14 extended `switch` "so it can be used as either a statement or an expression"[^11], with the new labels `case ... ->` that have no fall-through. "When a switch label applies, and that switch label is for a switch rule, the switch rule expression or statement introduced by the switch label is executed, and nothing else"[^3]. C# 8.0 added switch expressions[^12]. Kotlin `when` "evaluates its argument and compares the result against each branch in order until one branch condition is satisfied", and it can be an expression or a statement[^9]. SQL has the `CASE` expression, "a generic conditional expression, similar to if/else statements in other programming languages"[^13].

**Switch and match.** Rust `match` and Python `match` look like a switch, but they compare the value with patterns, not only with constants. The Python tutorial says that `match` is "superficially similar to a switch statement in C, Java or JavaScript (and many other languages), but it’s more similar to pattern matching in languages like Rust or Haskell"[^10]. A Rust `match` must cover each possible value: "The arms’ patterns must cover all possibilities"[^14]. [[pattern-matching]] describes the patterns.

## In languages

| Language | Verdict | Form |
|---|---|---|
| Python | native | `match` with literal patterns and `\|`; the first case that matches runs[^10] |
| JavaScript | native | `switch` with `case` and `default`; strict equality; fall-through without `break`[^6] |
| TypeScript | native | the `switch` of JavaScript; a `switch` on a literal field narrows a union type[^15] |
| Java | native | `switch` statement and expression; `case ... :` falls through, `case ... ->` does not[^3] |
| Kotlin | native | `when` with a subject, branches with commas, and `else`[^9] |
| C# | native | `switch` statement with no fall-through, and a `switch` expression[^7] |
| Go | native | `switch` with any comparable value; no fall-through without `fallthrough`[^5] |
| Rust | native | `match`, which must be exhaustive[^14] |
| C | native | `switch` on an integer with constant labels; fall-through without `break`[^2] |
| C++ | native | the `switch` of C, also on an enumeration or a class that converts[^4] |
| Bash | native | `case` with glob patterns; `;;`, `;&` and `;;&`[^8] |
| PowerShell | native | `switch`, which runs the action of each condition that matches[^16] |
| SQL | native | the `CASE` expression[^13] |
| Regex | n/a | a regular expression has alternation, not a branch on a value |
| HTML/CSS | n/a | no values to branch on |

The example files sort the same kinds of input in each language. The C, C++, Java and JavaScript examples group labels and end each group with `break`. The Go example has a `switch` with no value, which works like an `if` chain. The C# and Java examples also use a switch expression. The Rust and Python examples use `match` on literal values. The Bash example uses glob patterns.

## Variations and related ideas

- **The `if` chain.** A `switch` with no value, in Go and Kotlin, is a chain of conditions. In Go, "A missing switch expression is equivalent to the boolean value true"[^5]. See [[conditional]].
- **Lookup tables.** A map from keys to values or to functions can replace a `switch` whose cases only choose a value.
- **Pattern matching.** `match` in Rust and Python, `switch` with patterns in Java and C#, and `when` in Kotlin test the shape of a value and bind its parts. See [[pattern-matching]].
- **Fall-through.** The cases of a C `switch` run into each other unless a `break` stops them. See [[fall-through]].
- **The task.** [[write-a-switch-or-match]] writes the same `switch` in ten languages.

## Pitfalls

### A missing break

In C, C++, Java and JavaScript, a case without `break` runs on into the next case. Here the case for `"tea"` has no `break`:

```javascript wrong id=js-missing-break
function price(drink) {
  let cents = 0;
  switch (drink) {
    case "tea":
      cents = 200;
    case "coffee":
      cents = 250;
      break;
    default:
      cents = -1;
  }
  return cents;
}

console.log(price("tea"), price("coffee"), price("milk"));
```

```output for=js-missing-break
250 250 -1
```

Tea costs 250, not 200, because the code of the coffee case runs after the code of the tea case. Wikipedia calls the omission of `break` "a common mistake"[^1]. The fix is a `break` at the end of each case, or a form with no fall-through, such as `case ... ->` in Java[^3]. [[fall-through]] shows the compiler warnings that find it.

### Strict comparison in JavaScript

A JavaScript `switch` compares "using the strict equality comparison"[^6], so the string `"2"` does not match `case 2`. A value read from a form or a URL is a string:

```javascript wrong id=js-strict
function size(n) {
  switch (n) {
    case 1:
      return "small";
    case 2:
      return "medium";
    default:
      return "unknown";
  }
}

console.log(size(2), size("2"));
```

```output for=js-strict
medium unknown
```

Convert the value first, for example with `Number(n)`, or write the cases as strings.

### A name that is not a constant in Python

A Python `case` with a bare name is a capture pattern, not a comparison. "A capture pattern binds the subject value to a name", and "Capture patterns always succeed"[^17]. So `case QUIT:` matches each value:

```python wrong id=python-capture
QUIT = "q"


def handle(key):
    match key:
        case "h":
            return "help"
        case QUIT:
            return "quit"


for key in ["h", "q", "x"]:
    print(key, handle(key))
```

```output for=python-capture
h help
q quit
x quit
```

The key `x` gives `quit`. A dotted name, such as `Keys.QUIT`, is a value pattern, and it "will succeed only if <subject> == NAME1.NAME2"[^17]. Use a dotted name or a literal.

### A declaration that a jump skips

A C++ `switch` jumps into its body, and "transfer of control is not permitted to enter the scope of a variable"[^4]. A variable with an initializer in one case is in scope in the cases below it, so the jump to a later label is an error:

```cpp fails expect=compile id=cpp-jump-past-init
#include <iostream>

int main() {
    int n = 2;
    switch (n) {
    case 1:
        int x = 10;
        std::cout << x << '\n';
        break;
    case 2:
        std::cout << "two\n";
        break;
    }
}
```

The fix is braces around the statements of the case, so that `x` has its own block.

## Cost

A compiler can turn a `switch` into a sequence of comparisons, a binary search, or a jump table. Wikipedia states that "An optimizing compiler such as GCC or Clang may compile a switch statement into either a branch table or a binary search"[^1]. "A branch table allows the program to determine which branch to execute with a single calculation instead of comparing values in a sequence"[^1]. GCC has an option, `-fno-jump-tables`, to "not use jump tables for switch statements even where it would be more efficient than other code generation strategies"[^18]. So a `switch` can be faster than a long `if` chain, but the compiler decides which code to emit.

## History

- **Fortran and ALGOL 60.** "The earliest Fortran compilers supported the computed goto statement for multi-way branching"[^1]. Early ALGOL compilers had a switch that a `goto` could index[^1].
- **1966.** Wirth and Hoare described ALGOL W. "The case expression is supported by languages dating at least as far back as ALGOL-W"[^1]. "Languages such as BCPL, ALGOL-W, and ALGOL-68 introduced forms of this construct which have survived through modern languages"[^1].
- **2019.** C# 8.0, released in September 2019, added switch expressions[^12].
- **2020.** JDK 14, released on 17 March 2020, made switch expressions and the `case ... ->` labels a standard part of Java (JEP 361)[^11][^19].
- **2021.** Python 3.10, released on 4 October 2021, added the `match` statement (PEP 634)[^20][^21].

## References
