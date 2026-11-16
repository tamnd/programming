---
tag: PWSEXB
kind: concept
slug: fall-through
name: Fall-through
aliases: [fallthrough, fall through, falling through, implicit fall-through, case fall-through, missing break]
area: errors-and-control/control-structures
level: core
summary: >-
  In a switch statement with fall-through, control runs from the end of one case into the code of the next case, unless a statement such as break ends the case.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1983
  people: [Tom Duff]
  note: In November 1983, Tom Duff used the fall-through of the C switch statement to jump into the middle of an unrolled loop. This code is now called Duff's device.
see_also: [switch-statement, write-a-switch-or-match, control-flow, conditional, pattern-matching]
sources:
- url: https://en.wikipedia.org/wiki/Switch_statement#Fall_through
  title: Switch statement, Wikipedia
  locator: Fall through
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/switch#breaking_and_fall-through
  title: switch, MDN Web Docs
  locator: Breaking and fall-through
- url: https://en.cppreference.com/w/cpp/language/switch.html
  title: switch statement, cppreference.com
  locator: Control flow transfer
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.11.3
  title: The Java Language Specification, Java SE 25 Edition
  locator: 14.11.3 Execution of a switch Statement; Example 14.11.3-1
- url: https://go.dev/ref/spec#Fallthrough_statements
  title: The Go Programming Language Specification
  locator: Expression switches; Fallthrough statements
- url: https://www.gnu.org/software/bash/manual/html_node/Conditional-Constructs.html
  title: Conditional Constructs, Bash Reference Manual
  locator: case
- url: https://en.cppreference.com/w/c/language/attributes/fallthrough.html
  title: "C attribute: fallthrough (since C23), cppreference.com"
  locator: Explanation
- url: https://en.cppreference.com/w/cpp/language/attributes/fallthrough.html
  title: "C++ attribute: fallthrough (since C++17), cppreference.com"
  locator: Explanation
- url: https://openjdk.org/jeps/361
  title: "JEP 361: Switch Expressions"
  locator: Summary; Motivation
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/selection-statements#the-switch-statement
  title: Selection statements, C# language reference
  locator: The switch statement
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/jump-statements#the-goto-statement
  title: Jump statements, C# language reference
  locator: The goto statement
- url: https://docs.python.org/3.15/tutorial/controlflow.html#match-statements
  title: More Control Flow Tools, The Python Tutorial
  locator: 4.7 match Statements
- url: https://doc.rust-lang.org/book/ch06-02-match.html
  title: The match Control Flow Construct, The Rust Programming Language
  locator: Introduction
- url: https://kotlinlang.org/docs/control-flow.html#when-expressions-and-statements
  title: Conditions and loops, Kotlin documentation
  locator: When expressions and statements
- url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_switch
  title: about_Switch, PowerShell documentation
  locator: Use break and continue to control the flow
- url: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
  title: Options to Request or Suppress Warnings, GCC documentation
  locator: -Wimplicit-fallthrough
- url: https://www.typescriptlang.org/tsconfig/noFallthroughCasesInSwitch.html
  title: noFallthroughCasesInSwitch, TSConfig Reference
  locator: the whole page
- url: https://en.wikipedia.org/wiki/Duff%27s_device
  title: Duff's device, Wikipedia
  locator: Introduction; Mechanism
---

## Idea

A [[switch-statement]] jumps to the case label that matches a value. In some languages, the case then has no end of its own. Control runs on through the code of the next case, and the next, until a `break` or the end of the switch. This is **fall-through**. Wikipedia describes it: "When control branches to one case, but continues into the subsequent branch, the control flow is called fall through, and allows branching to the same code for multiple values"[^1].

MDN states the rule for JavaScript: "If break is omitted, execution will proceed to the next case clause, even to the default clause, regardless of whether the value of that clause's expression matches"[^2]. The labels of the next cases are not tested again.

Fall-through can be useful. In this C program, each access level has the rights of the levels under it, so the case for level 3 falls through into the cases for levels 2 and 1:

```c run id=idea
#include <stdio.h>

static void rights(int level) {
    printf("level %d:", level);
    switch (level) {
    case 3:
        printf(" delete");
        /* no break: fall through to case 2 */
    case 2:
        printf(" write");
        /* no break: fall through to case 1 */
    case 1:
        printf(" read");
        break;
    default:
        printf(" none");
    }
    printf("\n");
}

int main(void) {
    for (int level = 3; level >= 0; level--) {
        rights(level);
    }
    return 0;
}
```

```output for=idea
level 3: delete write read
level 2: write read
level 1: read
level 0: none
```

But fall-through is also the cause of a common bug: a case that should end, but does not, because its `break` is missing[^1].

## How it works

- **Labels are only targets.** In C and C++, a switch jumps to a label, and the label does nothing more. cppreference states: "case and default labels in themselves do not alter the flow of control"[^3]. The JLS gives the same rule for a Java statement group: "all the block statements in the switch block that follow the switch label are executed, including those that appear after subsequent switch labels"[^4].
- **Unstructured and structured switches.** Wikipedia names two kinds of switch. "An unstructured switch, as in C (and more generally languages influenced by Fortran's computed goto), acts like goto"[^1]. In a structured switch, "as in Pascal-like languages, control jumps from the start of the switch statement to the selected case and at the end of the case, control jumps to the end of the switch statement"[^1]. Only the unstructured kind has fall-through.
- **Ending a case.** In a switch with fall-through, a case ends with a statement that leaves the switch: `break`, `return`, `throw`, or `goto`. A case with no statements, such as each label but the last in `case 4: case 6: case 9: case 11:`, falls through into the next label. This is how several values share one body.
- **Fall-through on request.** Some languages do not fall through, but they have a statement that does it on request. A Go `fallthrough` statement "transfers control to the first statement of the next case clause in an expression"[^5] switch. In Bash, "Using ‘;&’ in place of ‘;;’ causes execution to continue with the command-list associated with the next clause, if any"[^6].

## In languages

| Language | Falls through by default | Fall-through on request | How a case ends |
|---|---|---|---|
| C | yes[^3] | leave out `break`; mark it with the C23 attribute `[[fallthrough]];`[^7] | `break`, `return`, `goto` |
| C++ | yes[^3] | leave out `break`; mark it with the C++17 attribute `[[fallthrough]];`[^8] | `break`, `return`, `throw` |
| Java | yes with `case ... :` labels; no with `case ... ->` labels[^4][^9] | leave out `break` | `break`, or the end of a `->` rule |
| JavaScript, TypeScript | yes[^2] | leave out `break` | `break`, `return`, `throw` |
| C# | no; a compile error, except for a section with no statements[^10] | `goto case` and `goto default`[^11] | `break`, `goto`, `return`[^10] |
| Go | no[^5] | `fallthrough`[^5] | the end of the case |
| Bash | no with `;;`[^6] | `;&`, or `;;&` to test the next patterns[^6] | `;;` |
| Python, Rust, Kotlin | no[^12][^13][^14] | none | the end of the case |
| PowerShell | runs each condition that matches[^15] | none | `break` or `continue`[^15] |

- **C and C++.** A case without `break` falls through. The C23 and C++17 standards added the statement `[[fallthrough]];`, which marks a fall-through as intentional. cppreference states that it "Indicates that the fall through from the previous case label is intentional and should not be diagnosed by a compiler that warns on fallthrough"[^8][^7].
- **Java.** A switch with `case ... :` labels falls through, as in C. JEP 361 added `case ... ->` labels "(with no fall through)"[^9]. A `->` rule runs its own code "and nothing else"[^4].
- **C#.** "Within a switch statement, control can't fall through from one switch section to the next"[^10]. The C# reference adds: "Falling through from one switch section to the next generates a compiler error"[^10]. To get the effect of fall-through, a section uses `goto case` or `goto default`[^11].
- **Go.** A case ends by itself. A `fallthrough` statement "may appear as the last statement of all but the last clause of an expression switch"[^5]. A type switch cannot use `fallthrough`[^5].
- **Bash.** A clause ends with `;;`, `;&` or `;;&`. `;&` runs the commands of the next clause. `;;&` is different: it causes "the shell to test the patterns in the next clause, if any, and execute any associated command-list if the match succeeds"[^6].
- **Python, Rust and Kotlin.** These languages run one case only. In Python, "Only the first pattern that matches gets executed"[^12]. In Rust, "If a pattern matches the value, the code associated with that pattern is executed"[^13], and the `match` ends. Kotlin `when` compares the value "against each branch in order until one branch condition is satisfied"[^14].
- **PowerShell.** A PowerShell `switch` does not fall through, but it does not stop at the first match either: "If the value matches multiple conditions, the action for each condition is executed"[^15]. `break` stops this.

## Variations and related ideas

- **Warnings.** Wikipedia notes that "often fall through code results in a warning from a code quality tool such as lint"[^1]. GCC has `-Wimplicit-fallthrough`, which warns "when a switch case falls through"[^16]. The warning "does not warn when the last statement of a case cannot fall through, e.g. when there is a return statement"[^16]. TypeScript has the compiler option `noFallthroughCasesInSwitch`. It reports "errors for fallthrough cases in switch statements" and "Ensures that any non-empty case inside a switch statement includes either break, return, or throw"[^17].
- **Markers.** A marker tells the compiler and the reader that a fall-through is intentional. GCC accepts a comment such as `/* FALLTHRU */` before the next label, the attribute `__attribute__ ((fallthrough))`, and the standard `[[fallthrough]];`[^16].
- **Several values in one case.** Stacked labels with no statements are the most common use of fall-through. Languages without fall-through give a list of values in one case instead, such as `case 4, 6, 9, 11:` in Go or `case 4 | 6 | 9 | 11:` in Python. [[write-a-switch-or-match]] shows both forms.
- **Duff's device.** Duff's device puts the labels of a switch inside a `do` loop, and jumps into the middle of the loop. "Duff implemented this technique in C by using C's case label fall-through feature to jump into the unrolled body"[^18].

## Pitfalls

### A missing break

A case that should end, but has no `break`, runs the code of the next case. Wikipedia states: "a common mistake is to accidentally omit the keyword, causing unintentional fall through and often a bug"[^1]. The JLS gives an example with three cases and adds: "Fall through can be the cause of subtle bugs"[^4]. Here the case for 200 has no `break`:

```java wrong id=java-missing-break
public class Main {
    static String status(int code) {
        String text;
        switch (code) {
            case 200:
                text = "ok";
            case 404:
                text = "not found";
                break;
            default:
                text = "other";
        }
        return text;
    }

    public static void main(String[] args) {
        System.out.println(status(200) + ", " + status(404) + ", " + status(500));
    }
}
```

```output for=java-missing-break
not found, not found, other
```

The fix is a `break` at the end of the case for 200, or a `->` rule for each case. A `->` rule runs its own code "and nothing else"[^4].

### Fall-through into default

The default clause is a label too, so a case can fall through into it. MDN states that execution proceeds "even to the default clause"[^2]:

```javascript wrong id=js-into-default
function describe(n) {
  const parts = [];
  switch (n) {
    case 0:
      parts.push("zero");
    default:
      parts.push("not zero");
  }
  return parts.join(", ");
}

console.log(describe(0));
console.log(describe(5));
```

```output for=js-into-default
zero, not zero
not zero
```

### A Go fallthrough does not test the next case

A Go `fallthrough` goes to "the first statement of the next case clause"[^5]. It does not test the expression of that case:

```go wrong id=go-fallthrough-no-test
package main

import "fmt"

func main() {
	n := 5
	switch {
	case n > 0:
		fmt.Println("positive")
		fallthrough
	case n > 100:
		fmt.Println("more than 100")
	}
}
```

```output for=go-fallthrough-no-test
positive
more than 100
```

5 is not more than 100, but the second case runs. To test the next condition, use a second `switch` or an `if`.

### Fall-through in C# does not compile

Code that relies on fall-through does not move to C# without change. "Every switch section must end with a break, goto, or return"[^10]:

```csharp fails expect=compile id=csharp-no-fallthrough
using System;

static void Rights(int level)
{
    switch (level)
    {
        case 2:
            Console.Write(" write");
        case 1:
            Console.Write(" read");
            break;
    }
    Console.WriteLine();
}

Rights(2);
```

The fix is `goto case 1;` at the end of the section for 2[^11].

## Cost

Fall-through adds no work at run time. In an unstructured switch, which "acts like goto"[^1], control simply goes on to the next statement. The cost is in reading the code: a reader must check each case for a `break` to know where the case ends. Markers such as `[[fallthrough]];` and warnings such as `-Wimplicit-fallthrough` reduce this cost[^8][^16].

## History

- **1983.** "Its discovery is credited to Tom Duff in November 1983, when Duff was working for Lucasfilm and used it to speed up a real-time animation program"[^18]. The Jargon File called Duff's device "the most dramatic use yet seen of fall through in C"[^18]. Duff said: "This code forms some sort of argument in that debate, but I'm not sure whether it's for or against"[^18].
- **C++17 and C23.** C++17 added the `[[fallthrough]]` attribute[^8], and C23 added the same attribute to C[^7].
- **2020.** JEP 361 made `case ... ->` labels with no fall through a standard part of Java in JDK 14[^9]. The JEP names "the default control flow behavior between switch labels (fall through)" as one of the problems of the old switch[^9].

## References
