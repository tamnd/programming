---
tag: 220A67
kind: task
slug: write-a-conditional
name: Write a conditional
aliases: [if else, if elif else, else if chain, if statement, choose between branches, letter grade from a score, Conditional structures]
area: errors-and-control/control-structures
level: intro
summary: >-
  Map a score to a letter grade with one if, else if and else chain, and put the tests in the right order so that each score takes exactly one branch.
contract: contract.yaml
category: control-flow
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Conditional-structures/00-TASK.txt
  title: Conditional structures, Rosetta Code (the RosettaCodeData mirror)
  locator: Task
- url: https://docs.python.org/3.15/reference/compound_stmts.html#the-if-statement
  title: Compound statements, The Python Language Reference
  locator: 8.1 The if statement
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/if...else
  title: if...else, MDN Web Docs
  locator: Description
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.5
  title: The Java Language Specification, Java SE 25 Edition
  locator: 14.5 Statements (the dangling else problem); 14.9 The if Statement
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/selection-statements
  title: Selection statements, C# language reference
  locator: The if statement
- url: https://port70.net/~nsz/c/c11/n1570.html#6.8.4.1
  title: N1570, ISO/IEC 9899:201x Committee Draft (C11)
  locator: 6.8.4.1 The if statement, paragraphs 1 to 3
- url: https://en.cppreference.com/w/cpp/language/if.html
  title: if statement, cppreference.com
  locator: Condition
- url: https://go.dev/ref/spec#If_statements
  title: The Go Programming Language Specification
  locator: If statements
- url: https://doc.rust-lang.org/book/ch03-05-control-flow.html
  title: Control Flow, The Rust Programming Language
  locator: if Expressions; Using if in a let Statement
- url: https://www.gnu.org/software/bash/manual/html_node/Conditional-Constructs.html
  title: Conditional Constructs, Bash Reference Manual
  locator: if
- url: https://docs.python.org/3.15/tutorial/controlflow.html#if-statements
  title: More Control Flow Tools, The Python Tutorial
  locator: 4.1 if Statements
---

## Task

Choose one of several branches with a chain of `if`, `else if` and `else`. The function `grade(score)` takes a score from 0 to 100 and returns a letter grade:

- `"A"` for a score of 90 or more,
- `"B"` for a score from 80 to 89,
- `"C"` for a score from 70 to 79,
- `"D"` for a score from 60 to 69,
- `"F"` for a score under 60.

For example, `grade(85)` is `"B"` and `grade(60)` is `"D"`.

The Rosetta Code task asks for a list of the "conditional structures" of a language, and it names **if-then-else** and **switch** as the common ones[^1]. This task uses the first of the two. [[write-a-switch-or-match]] uses the second.

## Solutions

| Language | The middle branches | The last branch | The condition |
|---|---|---|---|
| Python | `elif score >= 80:` | `else:` | any value; Python tests its truth value[^2] |
| JavaScript, TypeScript | `else if (score >= 80) { ... }` | `else { ... }` | any value that is truthy or falsy[^3] |
| Java | `else if (score >= 80) { ... }` | `else { ... }` | `boolean` or `Boolean`[^4] |
| C# | `else if (score >= 80) { ... }` | `else { ... }` | a Boolean expression[^5] |
| C | `else if (score >= 80) { ... }` | `else { ... }` | a scalar value; zero is false[^6] |
| C++ | `else if (score >= 80) { ... }` | `else { ... }` | a value that converts to `bool`[^7] |
| Go | `} else if score >= 80 {` | `} else {` | a boolean expression; the braces are required[^8] |
| Rust | `} else if score >= 80 {` | `} else {` | a `bool`[^9] |
| Bash | `elif ((score >= 80)); then` | `else` ... `fi` | the exit status of a command[^10] |

- **One chain, one branch.** The Python `if` statement "selects exactly one of the suites by evaluating the expressions one by one until one is found to be true"[^2]. MDN describes the same rule for JavaScript: "The conditions are evaluated in order until one evaluates to true"[^3]. So each test can use only the lower bound. When the test `score >= 80` runs, the test `score >= 90` was false already.
- **`elif` and `else if`.** In Python, "The keyword ‘elif ’ is short for ‘else if’, and is useful to avoid excessive indentation"[^11]. JavaScript has no special keyword: "Note that there is no elseif (in one word) keyword in JavaScript"[^3]. An `else if` there is an `else` whose statement is another `if`. Java, C, C++ and C# work the same way.
- **Go.** In the grammar of the Go `if` statement, the condition is followed by a `Block`, and an `else` is followed by another `if` or by a `Block`[^8]. A block is a list of statements in braces, so the braces are required[^8]. The condition is "a boolean expression"[^8].
- **Rust.** `if` is an expression. "Because if is an expression, we can use it on the right side of a let statement to assign the outcome to a variable"[^9]. The Rust solution has no `return`: the value of the chain is the value of the function.
- **Bash.** In an `if`, "The test-commands list is executed, and if its return status is zero, the consequent-commands list is executed"[^10]. The test `((score >= 90))` is an arithmetic command, and its status is zero when the comparison is true. The function prints the grade.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `ninety`, `eighty-nine`, `sixty` and `fifty-nine`: the scores on each side of a bound. A test written as `score > 90` in place of `score >= 90` gives `"B"` for 90, and only a case on the bound finds the mistake.
- `hundred` and `zero`: the highest and the lowest score. The `else` branch takes 0 and each other score under 60.

## Pitfalls

- **A wide test before a narrow test.** The first true test wins, so the tests must go from the highest bound down. If the test `score >= 60` comes first, it is true for 95 too, and the chain never reaches the test for `"A"`[^2]:

```python run id=python-grade-order
def wrong(score):
    if score >= 60:
        return "D"
    elif score >= 70:
        return "C"
    elif score >= 80:
        return "B"
    elif score >= 90:
        return "A"
    else:
        return "F"


def right(score):
    if score >= 90:
        return "A"
    elif score >= 80:
        return "B"
    elif score >= 70:
        return "C"
    elif score >= 60:
        return "D"
    else:
        return "F"


print([wrong(s) for s in (95, 85, 65, 40)])
print([right(s) for s in (95, 85, 65, 40)])
```

```output for=python-grade-order
['D', 'D', 'D', 'F']
['A', 'B', 'D', 'F']
```

- **The dangling else.** In Java, C and C++, an `if` without braces can hold a second `if`, and then an `else` could belong to either of them. The JLS states that Java, "like C and C++ and many programming languages before them, arbitrarily decrees that an else clause belongs to the innermost if to which it might possibly belong"[^4]. The indentation does not change this:

```java run id=java-dangling-else
public class Main {
    static void report(int score) {
        if (score >= 60)
            if (score >= 90)
                System.out.println(score + ": excellent");
        else
            System.out.println(score + ": failed");
    }

    public static void main(String[] args) {
        report(95);
        report(75);
        report(40);
    }
}
```

```output for=java-dangling-else
95: excellent
75: failed
```

The `else` belongs to `if (score >= 90)`, so 75 prints `75: failed` and 40 prints nothing. C has the same rule: "An else is associated with the lexically nearest preceding if that is allowed by the syntax"[^6]. Braces on each branch remove the doubt. The branches of a Go `if` are blocks, so Go does not have this problem[^8].

- **A condition that is not a boolean.** Python and JavaScript test the truth value of any value. In JavaScript, the condition is "An expression that is considered to be either truthy or falsy"[^3]. In C, "the first substatement is executed if the expression compares unequal to 0"[^6]. Rust does not convert: "Unlike languages such as Ruby and JavaScript, Rust will not automatically try to convert non-Boolean types to a Boolean"[^9]. Java also requires a boolean: the expression "must have type boolean or Boolean , or a compile-time error occurs"[^4].

```rust fails expect=compile id=rust-int-condition
fn main() {
    let score = 75;
    if score {
        println!("the score is not zero");
    }
}
```

The fix is an explicit comparison, such as `if score != 0`.

## Related tasks

- [[write-a-switch-or-match]]: choose a branch by the value of one expression.
- [[fizzbuzz-task]]: an `if` chain inside a loop, where the order of the tests matters too.
- [[conditional]]: the concept of a conditional in each language.

## References
