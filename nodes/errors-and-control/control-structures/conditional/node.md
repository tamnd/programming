---
tag: F62G2R
kind: concept
slug: conditional
name: Conditional
aliases: [conditional, conditional statement, if statement, if-then-else, if else, else if, elif, conditional expression, ternary operator, conditional operator, selection statement, branch, dangling else, guard clause]
area: errors-and-control/control-structures
level: core
summary: >-
  A conditional runs one piece of code when a condition is true and, optionally, another when it is false. The if statement and the conditional expression are its two main forms.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [John McCarthy]
  note: McCarthy developed the conditional expression for LISP in the late 1950s and suggested it for ALGOL, and ALGOL 60 allowed if, then and else as an expression and as a statement.
see_also: [control-flow, boolean-type, short-circuit-evaluation, pattern-matching, structured-programming]
same_as:
  wikidata: Q817862
  wikipedia: Conditional_(computer_programming)
sources:
- url: https://en.wikipedia.org/wiki/Conditional_(computer_programming)
  title: Conditional (computer programming), Wikipedia
  locator: Introduction; History and development; Arithmetic if; Conditional expression
- url: https://docs.python.org/3/reference/compound_stmts.html
  title: Compound statements, The Python Language Reference
  locator: 8.1 The if statement
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 14
  locator: 14.9 The if Statement
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: If statements
- url: https://doc.rust-lang.org/reference/expressions/if-expr.html
  title: if expressions, The Rust Reference
  locator: Introduction
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/if...else
  title: if...else, MDN Web Docs
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/selection-statements
  title: Selection statements, C# language reference
  locator: The if statement; Case guards
- url: https://en.cppreference.com/w/cpp/language/if.html
  title: if statement, C++ reference, cppreference.com
  locator: Explanation; Constexpr if
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Conditional_operator
  title: Conditional (ternary) operator, MDN Web Docs
  locator: Introduction
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 15
  locator: 15.25 Conditional Operator
- url: https://docs.python.org/3/reference/expressions.html
  title: Expressions, The Python Language Reference
  locator: 6.13 Conditional expressions
---

## Idea

A program often has to do different things in different cases: charge more for a heavy parcel, show an error for bad input, skip a step when there is nothing to do. A conditional tests a condition and runs one piece of code when it is true and, optionally, another piece when it is false. Wikipedia says that "a conditional statement directs program control flow based on the value of a condition"[^1].

```python run id=idea
def shipping(weight_kg, express):
    # The tests run in order, and only the first true branch runs.
    if weight_kg <= 0:
        raise ValueError("weight must be positive")
    elif weight_kg < 1:
        cost = 5
    elif weight_kg < 10:
        cost = 9
    else:
        cost = 20
    # An if with no else runs its body or does nothing.
    if express:
        cost *= 2
    return cost


for w, e in [(0.5, False), (3, True), (25, False)]:
    print(w, e, shipping(w, e))

# A conditional expression chooses between two values.
n = 4
print(n, "is", "even" if n % 2 == 0 else "odd")

# Only the chosen branch is evaluated, so 10 / d does not run when d is 0.
d = 0
print("undefined" if d == 0 else 10 / d)
```

```output for=idea
0.5 False 5
3 True 18
25 False 20
4 is even
undefined
```

The `if`, `elif` and `else` chain in `shipping` tests its conditions from top to bottom and runs only the first branch whose condition is true. The second `if` has no `else`, so it doubles the cost or does nothing. The conditional expression `"even" if n % 2 == 0 else "odd"` chooses between two values. In the last line, only the chosen operand is evaluated, so `10 / d` never runs and there is no division by zero.

## How it works

**The if statement.** Python says that "The if statement is used for conditional execution"[^2]. It "selects exactly one of the suites by evaluating the expressions one by one until one is found to be true"[^2], and "If all expressions are false, the suite of the else clause, if present, is executed"[^2]. The other languages state the same rule. In Java, "The if statement allows conditional execution of a statement or a conditional choice of two statements, executing one or the other but not both"[^3]. In Go, "If" statements "specify the conditional execution of two branches according to the value of a boolean expression"[^4].

**The condition.** Java, C#, Go and Rust require a Boolean condition. In Rust, "Condition operands must be either an Expression with a boolean type or a conditional let match"[^5]. Python, JavaScript, C and C++ accept other values and test their truth. MDN says that the `if...else` statement "executes a statement if a specified condition is truthy"[^6]. In C, the branch runs when the expression is not zero.

**Chains.** `else if` is an `if` statement in the `else` branch of another, so a chain of tests is a nest of `if` statements written flat. Python, Bash and others have a keyword for it, such as `elif`. A `switch` or a `match` statement also chooses a branch, but it compares one value with many cases, and C# adds a case guard, a condition "after the when keyword"[^7].

**Dangling else.** In `if (a) if (b) x(); else y();`, the `else` could belong to either `if`. C and C++ give it to the inner one: "the else is associated with the closest if that does not yet have an associated else"[^8]. Languages with required braces or with indentation, such as Go, Rust and Python, do not have this ambiguity.

**Conditional expressions.** "A conditional expression evaluates to a value without the side-effect of changing control flow"[^1]. In C, Java, JavaScript and C#, it is the operator `c ? a : b`. In JavaScript it "is the only JavaScript operator that takes three operands"[^9]. In Java, "The conditional operator ? : uses the boolean value of one expression to decide which of two other expressions should be evaluated"[^10]. Python writes `x if C else y`, which "first evaluates the condition, C rather than x"[^11]. In Rust, `if` itself is an expression, and "An if expression evaluates to the same value as the executed block"[^5]. Go has no conditional expression, so a choice of value uses an `if` statement.

**Compile-time conditionals.** Some conditions are known before the program runs. C and C++ use `#if` in the preprocessor, and C++ has `if constexpr`, in which the branch that is not taken is a "discarded statement"[^8]. Rust uses the `cfg` attribute.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `if`, `elif`, `else`; `x if c else y`; truth values of all objects; `match` with guards |
| JavaScript | native | `if`, `else`; `c ? a : b`; truthy and falsy values; `??` and `?.` |
| TypeScript | native | the conditionals of JavaScript; each branch narrows the types of variables |
| Java | native | `if`, `else`; `c ? a : b`; `instanceof` patterns; the condition must be `boolean` |
| Kotlin | native | `if` is an expression; `when` with conditions; no `c ? a : b` operator |
| C# | native | `if`, `else`; `c ? a : b`; `is` patterns with `and`, `or` and `not`; `when` guards |
| Go | native | `if` with a short statement; `switch` with no expression; no conditional operator |
| Rust | native | `if` is an expression; `if let`, `let else` and let chains |
| C | native | `if`, `else`; `c ? a : b`; any non-zero scalar is true; `#if` |
| C++ | native | the conditionals of C; `if` with an initializer; `if constexpr`; `[[likely]]` |
| Bash | native | `if`, `elif`, `else`, `fi` on the exit status of a command; `[[ ]]`; `&&` and `\|\|` |
| PowerShell | native | `if`, `elseif`, `else`; `c ? a : b` in PowerShell 7; `switch` |
| SQL | native | `CASE WHEN ... THEN ... ELSE ... END` in expressions; `WHERE` filters rows by a condition |
| Regex | idiom | alternation with `\|`; some engines have conditionals such as `(?(1)a\|b)` |
| HTML | n/a | no conditionals; the browser shows the content that the document contains |
| CSS | idiom | `@media`, `@supports` and `@container` apply rules only when a condition is true |

Every general-purpose language has an `if` statement or an `if` expression, so the differences are in the condition and the form. The languages differ in which values a condition accepts, in whether `if` gives a value, and in the forms that test and bind at the same time.

The example files show these differences. Python tests an empty list for truth, chains comparisons, binds a value with `:=` in a condition, replaces a chain of `elif` with a dictionary and uses `match` with a guard. JavaScript shows which values are truthy, that `else if` is an `if` in an `else`, a chained conditional operator, an object lookup instead of conditions and `??=`. Java binds a variable with an `instanceof` pattern, shows that `?:` converts its operands to one type and can unbox a `null`, and shows a dangling `else`. C# tests ranges with relational patterns and uses a `when` guard. Go uses an `if` with a short statement, the `err != nil` check and a `switch` with no expression. Rust uses `if` as an expression, `if let`, `let else` and a let chain. C shows that any non-zero value is true, an assignment in a condition and `#if`. C++ uses `if constexpr`, an `if` with an initializer and `[[likely]]`.

## Variations and related ideas

- **Guard clauses.** A function that tests for bad cases first and returns early keeps its main path flat. Rust `let else` and Swift `guard` make this a form of the language.
- **Lookup tables.** A map from keys to values or functions replaces a long chain of tests for fixed cases.
- **Pattern matching.** `match` and `switch` with patterns test the shape of a value and bind its parts. See [pattern matching](/pattern-matching/).
- **Short-circuit operators.** `a && b()` runs `b` only when `a` is true, so it is a small conditional. See [short-circuit evaluation](/short-circuit-evaluation/).
- **Branch-free code.** `cond * a + (1 - cond) * b` and the conditional move instruction of a processor choose a value with no jump.

## Pitfalls

- **Assignment in a condition.** In C, `if (x = 0)` assigns and tests the result, which is always false. Compilers warn, and extra parentheses mark an assignment that is on purpose.
- **Truthiness.** In Python, `if value:` is false for `0` and `""` as well as for `None`. Test `if value is None:` when only `None` means missing.
- **Dangling else.** Indentation does not decide where an `else` belongs in C, C++, Java and JavaScript. Use braces.
- **Mixed types in `?:`.** In Java, `flag ? 1 : 2.0` gives `1.0`, and an `Integer` operand that is `null` throws `NullPointerException` when the other operand is an `int`.
- **Long chains.** A chain of ten `else if` tests is hard to check for missing cases. A `match` with an exhaustiveness check, or a table, is safer.

## Cost

A conditional compiles to a comparison and a conditional jump. The processor predicts the direction of the jump, and a wrong prediction costs some cycles, so a branch whose direction is random is slower than one that is regular. For a simple choice of value, compilers often emit a conditional move with no jump.

## History

The earliest conditional statement in Fortran, Wikipedia says, "was the arithmetic if statement which jumped to one of three labels depending on whether a value (of type integer, real, or double precision) is <0, 0, or >0"[^1]. "The concept of conditional expression was first developed by John McCarthy during his research into symbolic processing and LISP in the late 1950s"[^1]. McCarthy also suggested conditional expressions for ALGOL, and "ALGOL 60 and some other members of the ALGOL family allow if–then–else as an expression"[^1]. In early dialects of BASIC, an `if` could contain only a `goto`, and the structured `if`, `then`, `else` of the ALGOL family became "one of the key elements of structured programming"[^1]. C added the operator `? :` and the rule for the dangling `else`, and most later languages copied both.

## References
