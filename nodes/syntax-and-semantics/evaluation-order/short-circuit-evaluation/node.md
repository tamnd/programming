---
tag: D7SGB5
kind: concept
slug: short-circuit-evaluation
name: Short-circuit evaluation
aliases: [short-circuit evaluation, short circuit, minimal evaluation, McCarthy evaluation, conditional and, conditional or, lazy boolean operators, cand, cor, and then, or else, AndAlso, OrElse, null coalescing, optional chaining, guard expression]
area: syntax-and-semantics/evaluation-order
level: core
summary: >-
  Short-circuit evaluation computes the right operand of a Boolean and or or only when the left operand does not decide the result, so a test can guard the code that follows it.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [John McCarthy, Dennis Ritchie, Alan Snyder, Edsger W. Dijkstra]
  note: McCarthy's LISP paper of 1960 defined and and or so that the second operand is not computed when the first decides the result. C added the operators && and || in the early 1970s to make this rule explicit.
see_also: [order-of-evaluation, side-effect, null-references, null-pointer-dereference, statements-and-expressions, undefined-behavior]
same_as:
  wikidata: Q605499
  wikipedia: Short-circuit_evaluation
sources:
- url: https://docs.python.org/3/reference/expressions.html
  title: Expressions, The Python Language Reference
  locator: 6.11 Boolean operations
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Logical_AND
  title: Logical AND (&&), MDN Web Docs
  locator: Short-circuit evaluation
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Nullish_coalescing
  title: Nullish coalescing operator (??), MDN Web Docs
  locator: Description
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 15
  locator: 15.23 Conditional-And Operator &&; 15.24 Conditional-Or Operator ||
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/boolean-logical-operators
  title: Boolean logical operators, C# language reference
  locator: Conditional logical AND operator &&
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Logical operators
- url: https://doc.rust-lang.org/reference/expressions/operator-expr.html
  title: Operator expressions, The Rust Reference
  locator: Lazy boolean operators
- url: https://en.cppreference.com/w/c/language/operator_logical
  title: Logical operators, C reference, cppreference.com
  locator: Logical AND
- url: https://en.cppreference.com/w/cpp/language/operator_logical
  title: Logical operators, C++ reference, cppreference.com
  locator: Description
- url: https://www.postgresql.org/docs/current/sql-expressions.html
  title: Value Expressions, PostgreSQL documentation
  locator: 4.2.14 Expression Evaluation Rules
- url: http://www-formal.stanford.edu/jmc/recursive.pdf
  title: Recursive Functions of Symbolic Expressions and Their Computation by Machine, Part I, John McCarthy, 1960
  locator: Section 2, Conditional expressions
- url: https://www.nokia.com/bell-labs/about/dennis-m-ritchie/chist.html
  title: The Development of the C Language, Dennis M. Ritchie
  locator: Embryonic C
- url: https://en.wikipedia.org/wiki/Short-circuit_evaluation
  title: Short-circuit evaluation, Wikipedia
  locator: Introduction
---

## Idea

In `a and b`, if `a` is false the result is false whatever `b` is, and in `a or b`, if `a` is true the result is true. Short-circuit evaluation uses this: the language evaluates the left operand first and evaluates the right operand only when it still matters. This is a rule about which code runs, not only about the result, so a program can write `x != 0 and 12 / x > 2` and the division never runs with a zero.

```python run id=idea
calls = []


def check(name, value):
    """Record each call, so the output shows which operands ran."""
    calls.append(name)
    return value


def show(label, result):
    print(f"{label:<28} -> {result!r:<6} evaluated: {', '.join(calls) or 'none'}")
    calls.clear()


show("check(a, False) and check(b)", check("a", False) and check("b", True))
show("check(a, True) and check(b)", check("a", True) and check("b", True))
show("check(a, True) or check(b)", check("a", True) or check("b", False))
show("check(a, False) or check(b)", check("a", False) or check("b", False))
show("check(a, False) & check(b)", check("a", False) & check("b", True))

# The guard pattern: the right side runs only when it is safe.
for x in (0, 4):
    print(f"x={x}:", x != 0 and 12 / x > 2)

# Python returns the operand that decided the result, not always a bool.
print(repr("" or "default"), repr("name" or "default"), repr(0 and 1 / 0), repr([] and None))
```

```output for=idea
check(a, False) and check(b) -> False  evaluated: a
check(a, True) and check(b)  -> True   evaluated: a, b
check(a, True) or check(b)   -> True   evaluated: a
check(a, False) or check(b)  -> False  evaluated: a, b
check(a, False) & check(b)   -> False  evaluated: a, b
x=0: False
x=4: True
'default' 'name' 0 []
```

`check` records each operand that runs. With `and`, a false left operand stops the evaluation, and with `or`, a true one does. The operator `&` is not short-circuit, so it runs both operands. The last line shows that Python returns the operand that decided the result, which is not always a `bool`.

## How it works

**The rule.** The left operand is evaluated first. For *and*, the right operand is evaluated only if the left one is true; for *or*, only if it is false. Python states it as "The expression x and y first evaluates x; if x is false, its value is returned; otherwise, y is evaluated and the resulting value is returned"[^1]. JavaScript's `&&` and `||` also return an operand value, and MDN calls `&&` "a short-circuit operator"[^2]. JavaScript's `??` returns "its right-hand side operand when its left-hand side operand is null or undefined", and it also skips the right operand when it does not need it[^3].

**Typed languages.** Java's `&&` "evaluates its right-hand operand only if the value of its left-hand operand is true", and `||` does so only if the left operand is false. Java's `&` and `|` on `boolean` values evaluate both operands[^4]. C# calls `&&` the "short-circuiting" logical AND operator, and its `&` and `|` on `bool` also evaluate both[^5]. The Go specification defines `p && q` as "if p then q else false" and `p || q` as "if p then true else q"[^6]. Rust calls `&&` and `||` lazy boolean operators: "the right-hand operand is only evaluated when the left-hand operand does not already determine the result of the expression"[^7].

**C and C++.** In C, there is a sequence point after the left operand of `&&` and `||`, and if the left operand of `&&` is zero, "rhs is not evaluated at all"[^8]. The side effects of the left operand are complete before the right operand starts, so `p != NULL && p->qty > 0` is safe. C++ has the same rule for the built-in operators, but "overloaded operators behave like regular function calls and always evaluate both operands"[^9].

**A guard.** The common use is a test that protects the next one: a null check before a field access, a bounds check before an index, a zero check before a division, or a map lookup before the use of the value. The order of the operands matters, and a guard must come first.

**Other forms.** The conditional expression `c ? a : b` evaluates only one of `a` and `b`. Null-safe operators such as `?.`, `??` and `??=` in C# and JavaScript and `?:` in Kotlin stop at a null value. Library functions such as Python's `any` and `all` and Rust's `Iterator::any` stop at the first value that decides the result.

**Where the order is not fixed.** SQL does not promise short-circuit evaluation. The PostgreSQL documentation says that the inputs of an operator "are not necessarily evaluated left-to-right or in any other fixed order", so `WHERE x > 0 AND y/x > 1.5` can still divide by zero. It recommends a `CASE` construct "when it is essential to force evaluation order"[^10].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `and`, `or`; `any`, `all`; conditional expression |
| JavaScript | native | `&&`, `\|\|`, `??`, `?.`; `&&=`, `\|\|=`, `??=` |
| TypeScript | native | as JavaScript |
| Java | native | `&&`, `\|\|`, `?:`; `&` and `\|` evaluate both |
| Kotlin | native | `&&`, `\|\|`, `?.`, `?:` |
| C# | native | `&&`, `\|\|`, `??`, `?.`, `??=`; `&` and `\|` evaluate both |
| Go | native | `&&`, `\|\|` |
| Rust | native | `&&`, `\|\|`; `bool::then`, `Option::unwrap_or_else` |
| C | native | `&&`, `\|\|`, `?:`, with a sequence point after the left operand |
| C++ | native | as C; an overloaded `&&` or `\|\|` evaluates both operands |
| Bash | native | `&&` and `\|\|` lists of commands; `&&` and `\|\|` in `[[ ]]` |
| PowerShell | native | `-and`, `-or`; `&&` and `\|\|` between pipelines |
| SQL | idiom | `CASE WHEN`; `AND` and `OR` have no fixed order |
| Regex | n/a | no Boolean operators |
| HTML | n/a | no expressions |
| CSS | n/a | no expressions with side effects |

The general-purpose languages in the table all have short-circuit `and` and `or`, so the verdict is native. They differ in what the operators return and which other operators stop early. Python and JavaScript return an operand, so `or` and `||` give a default value. Java, C#, Go, Rust, C and C++ return a Boolean. C# and JavaScript add null-safe operators, and C++ loses the rule when a program overloads `&&`. SQL is the exception: its `AND` and `OR` have no fixed order of evaluation, and `CASE` is the way to force one.

The example files show the rule and the common guards. Python shows that `any` and `all` stop early with a generator but not with a list. JavaScript shows `??`, `?.` and `??=`. Java and C# compare `&&` with `&`. Go shows a nil guard and a map lookup with `ok`. Rust shows that `unwrap_or` computes its argument first and `unwrap_or_else` does not. C shows a null guard, and C++ shows that an overloaded `&&` evaluates both operands while a fold over `&&` does not.

## Variations and related ideas

- **Eager operators.** Java and C# have `&` and `|` for `bool`, Ada has `and` and `or` beside `and then` and `or else`, and Visual Basic has `And` and `Or` beside `AndAlso` and `OrElse`. The eager forms run both operands, which some code needs for their side effects.
- **Null coalescing and optional chaining.** `??`, `?.` and `??=` in C# and JavaScript, `?:` and `?.` in Kotlin, and `??` and `?.` in Swift test for null and skip the rest. See [null references](/null-references/).
- **Lazy arguments in libraries.** `Option::unwrap_or_else` in Rust, `Optional.orElseGet` in Java and the factory of Python's `collections.defaultdict` compute a default only when it is needed. Their eager forms, `unwrap_or`, `orElse` and `dict.get` with a default, compute it first.
- **User-defined short-circuit operators.** Lisp `and` and `or` are special forms or macros, Swift has `@autoclosure` parameters, Scala has by-name parameters, and C# runs a user-defined `&&` through the operators `&`, `true` and `false`. In Haskell, all evaluation is lazy, so `&&` is an ordinary function.
- **Order of evaluation.** Short-circuit operators fix an order in languages that otherwise leave it open, such as C. See [order of evaluation](/order-of-evaluation/).

## Pitfalls

- **Side effects in the right operand.** In `if (ok && count++ > 0)`, `count` changes only when `ok` is true. Do not put an action that must always run in the right operand. See [side effect](/side-effect/).
- **Defaults that replace valid values.** `x or 5` in Python and `x || 5` in JavaScript replace `0`, `""` and `False` as well as a missing value. Use `??` in JavaScript and C#, or `x if x is not None else 5` in Python.
- **Arguments that look lazy.** `opt.unwrap_or(expensive())`, `optional.orElse(load())` and `d.get(k, compute())` run the default every time. Use the closure form when the default is expensive or has side effects.
- **`&` for `&&`.** A single `&` evaluates both operands, so a guard such as `p != null & p.x > 0` does not protect the access. In C, `&` also has an unspecified order and a different precedence.
- **Overloaded operators in C++.** A class that overloads `&&` or `||` loses short-circuit evaluation. Overload conversion to `bool` instead.
- **Bash `a && b || c`.** This is not if-then-else: `c` runs when `a` fails and also when `b` fails. Use `if a; then b; else c; fi`.
- **SQL guards.** `WHERE x > 0 AND y / x > 1` can still fail, because the database chooses the order. Use `CASE`.

## Cost

Short-circuit operators compile to conditional branches, so `a && b` can cost a branch per operand. When both operands are cheap and have no side effects, a compiler can evaluate both and combine them with no branch, and a program can write `&` to ask for this. Put the operand that is cheap and that most often decides the result first: a fast test that usually fails saves the work of the slow test. For `any` and `all` over a sequence, short-circuit evaluation can stop after the first element instead of reading all *n*.

## History

John McCarthy's 1960 paper on LISP defined the propositional connectives with conditional expressions, so that "p ∧ q is computed by first computing p, and if p is false q is not computed"[^11]. In BCPL and B, `&` and `|` were bitwise in ordinary expressions but short-circuit inside the condition of an `if`. Dennis Ritchie wrote that "At the suggestion of Alan Snyder, I introduced the && and || operators to make the mechanism more explicit" in C, and that their late introduction explains why `&` has a lower precedence than `==`[^12].

Not everyone liked the rule. Edsger W. Dijkstra called the conditional connectives "cand" and "cor" and wrote that they "complicate the formal reasoning about programs"[^13]. Most languages kept them, and the term McCarthy evaluation names the idea after him[^13]. Later languages added null-safe operators that use the same rule.

## References
