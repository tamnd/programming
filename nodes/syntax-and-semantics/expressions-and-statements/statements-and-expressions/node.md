---
tag: 98X2DH
kind: concept
slug: statements-and-expressions
name: Statements and expressions
aliases: [expression, statement, expression statement, assignment expression, conditional expression, ternary operator, expression language, block expression, switch expression]
area: syntax-and-semantics/expressions-and-statements
level: core
summary: >-
  An expression has a value, and a statement does something. Languages differ in where they draw the line, such as whether assignment, if and blocks give values.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [John Backus, Peter Naur, John McCarthy]
  note: FORTRAN separated formulas from statements in 1957, Algol 60 defined both in a formal grammar in 1960, and Lisp had only expressions from 1958.
see_also: [order-of-evaluation, pattern-matching, functional-programming, immutability, imperative-programming, higher-order-functions, computer-program]
same_as:
  wikidata: Q778379
  wikipedia: Expression_(computer_science)
sources:
- url: https://en.cppreference.com/w/cpp/language/expressions.html
  title: Expressions, cppreference.com
  locator: Introduction
- url: https://docs.python.org/3/reference/simple_stmts.html
  title: Simple statements, The Python Language Reference
  locator: 7. Simple statements and 7.1 Expression statements
- url: https://doc.rust-lang.org/reference/statements.html
  title: Statements, The Rust Reference
  locator: Expression statements
- url: https://docs.python.org/3/reference/expressions.html
  title: Expressions, The Python Language Reference
  locator: 6.12 Assignment expressions
- url: https://doc.rust-lang.org/reference/statements-and-expressions.html
  title: Statements and expressions, The Rust Reference
  locator: Introduction
---

## Idea

An expression is a piece of code that has a value, such as `2 + 3`, `f(x)` or `a > b`. A statement is a piece of code that does something, such as an assignment, a loop or a `return`, and in many languages has no value. Programs are built from both: statements give the order of the steps, and expressions compute the values that the steps use. Languages draw the line between them in different places. In C, assignment is an expression; in Go and Python, it is a statement. In Rust, Kotlin and the functional languages, `if` and blocks are expressions with values; in C, Java and Python, `if` is a statement.

```python run id=idea
import ast

code = "x = 2 + 3\nprint(x * 2)\nif x > 4:\n    x = 0\n"
for stmt in ast.parse(code).body:  # a program is a list of statements
    inner = [type(n).__name__ for n in ast.walk(stmt) if isinstance(n, ast.expr)]
    print(f"{type(stmt).__name__:6} contains {inner}")

print(eval("2 + 3"))  # an expression has a value
env = {}
print(exec("x = 2 + 3", env), env["x"])  # a statement has an effect
try:
    eval("x = 2 + 3")
except SyntaxError:
    print("an assignment statement is not an expression")
print((y := 2 + 3) * 2, y)  # an assignment expression has both
```

```output for=idea
Assign contains ['Name', 'BinOp', 'Constant', 'Constant']
Expr   contains ['Call', 'Name', 'BinOp', 'Name', 'Constant']
If     contains ['Compare', 'Name', 'Constant', 'Name', 'Constant']
5
None 5
an assignment statement is not an expression
10 5
```

The parser of Python turns the four lines into three statements, an assignment, an expression statement and an `if`, and each statement contains expressions: names, constants, a binary operation, a call and a comparison. `eval` takes an expression and returns its value, `exec` takes statements and returns nothing but changes the namespace, and `eval` rejects an assignment because an assignment is not an expression in Python. The assignment expression `:=` is the exception that has both a value and an effect.

## How it works

**Expressions compute values.** The C++ reference defines the term: "An expression is a sequence of operators and operands, that specifies a computation. Expression evaluation may produce a result (e.g., evaluation of 2 + 2 produces the result 4) and may generate side-effects".[^1] Expressions nest: the operands of an operator and the arguments of a call are themselves expressions, so a single expression can be a tree of any depth. Every expression has a type, and the language defines the order in which its parts run, or leaves some of that order open. See [order of evaluation](/order-of-evaluation/).

**Statements run in sequence.** A statement is a unit of execution. The Python reference lists the simple statements, among them assignment, `return`, `raise`, `import` and the expression statement, and says: "A simple statement is comprised within a single logical line."[^2] Compound statements such as `if`, `while` and `def` contain other statements. A block is a list of statements that run one after another, which is how a program states the order of its effects.

**Expression statements.** Most languages let an expression stand as a statement, and the value is thrown away. The Rust reference describes one: "An expression statement is one that evaluates an expression and ignores its result. As a rule, an expression statement’s purpose is to trigger the effects of evaluating its expression."[^3] The Python reference adds that expression statements are used "(mostly interactively) to compute and write a value, or (usually) to call a procedure".[^2] A function call for its effect, such as `print(x)`, is the common case.

**Assignment: statement or expression.** In C and the languages that follow it, `x = 5` is an expression whose value is 5. This allows `a = b = 0` and `while ((n = read()) > 0)`, and also the bug `if (x = 0)`, which assigns instead of comparing. Python and Go make assignment a statement to prevent that bug. Python later added a separate operator for the useful cases: "An assignment expression (sometimes also called a “named expression” or “walrus”) assigns an expression to an identifier, while also returning the value of the expression."[^4]

**Expression languages.** Some languages make almost everything an expression. The Rust reference says: "Rust is primarily an expression language. This means that most forms of value-producing or effect-causing evaluation are directed by the uniform syntax category of expressions."[^5] In Rust, Kotlin, Scala, Ruby and the Lisp and ML families, `if`, `match` and blocks have values, so a variable can take its value from a choice without a temporary that each branch assigns. Statement languages add expression forms over time: the conditional operator `?:`, switch expressions in Java and C#, lambdas with expression bodies, and comprehensions. See [pattern matching](/pattern-matching/).

**Values without effects.** An expression with no side effects is easy to reason about: it can be replaced by its value, moved, cached or evaluated in parallel. Code that keeps effects in statements and computes in pure expressions is easier to test and to change, and functional languages build whole programs this way. See [functional programming](/functional-programming/) and [immutability](/immutability/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | statements and expressions; `x if c else y`; `:=` |
| JavaScript | native | C-style; assignment, `?:` and comma are expressions |
| TypeScript | native | as JavaScript |
| Java | native | C-style; `switch` expressions with `yield` |
| Kotlin | native | `if`, `when` and `try` are expressions |
| C# | native | C-style; `switch` expressions; `??=`; `throw` expressions |
| Go | native | assignment and `++` are statements; no `?:` |
| Rust | native | blocks, `if`, `match` and `loop` are expressions |
| C | native | assignment and comma are expressions; `?:` |
| C++ | native | as C; lvalue expressions; lambdas called at once |
| Bash | native | commands as statements; `$(( ))` and `[[ ]]` |
| PowerShell | native | statements whose output can be assigned |
| SQL | native | statements containing expressions; `CASE` |
| Regex | n/a | no statements or values |
| HTML | n/a | no statements or values |
| CSS | n/a | declarations and `calc()`, no statements |

Every language with code has both, so the table records where the line falls. Each example file chooses a label from a score and, in most files, reads numbers until a zero. Python, JavaScript, Java, C and C# use the conditional operator or a `switch` expression to give a value; Go must use a `switch` statement with a `return` in each case. Rust needs neither a ternary operator nor a `return`, because `match` and blocks have values.

The files also show the two places where languages differ most. Assignment is an expression in JavaScript, Java, C#, C and C++, so their files read and test in one loop condition, and the wrong block of the C file shows the price. Assignment and `++` are statements in Go, as the fails block shows. The fails block of the Rust file shows the other side of an expression language: a single semicolon changes the value of a block.

## Variations and related ideas

- **Side effects.** An expression can change state, by an assignment, a call or `++`, and the order of these effects inside one expression is a common source of bugs. See [order of evaluation](/order-of-evaluation/).
- **Conditional expressions.** `c ? a : b` in the C family, `a if c else b` in Python, and `if` expressions in Rust and Kotlin choose a value without a statement.
- **Switch and match expressions.** Java, C#, Rust and Kotlin give a value from the case that matches, and check that the cases cover every input. See [pattern matching](/pattern-matching/).
- **Comprehensions and pipelines.** List comprehensions and LINQ express loops as expressions that build a value. See [higher-order functions](/higher-order-functions/).
- **Lvalues and rvalues.** In C and C++, some expressions name an object and can appear on the left of `=`, while others are only values.
- **Statement expressions.** GCC's `({ ... })` extension and immediately invoked lambdas in C++ and JavaScript turn blocks of statements into expressions.
- **Everything is an expression.** In Lisp, ML and Haskell there are no statements, and a program is one large expression.

## Pitfalls

- **`=` for `==`.** In languages where assignment is an expression, a condition that assigns compiles and gives a wrong result, as the wrong block of the C file shows. Use the warnings of the compiler, and put a constant on the left only if your team agrees to.
- **A semicolon that discards a value.** In Rust, a semicolon after the last expression of a block makes its value `()`, as the fails block of the Rust file shows.
- **Effects hidden in expressions.** `a[i] = i++` in C has undefined behavior, and `f(x++, x)` depends on the order of evaluation. Keep each side effect in its own statement.
- **Nested conditional expressions.** Chains of `?:` or `if else` expressions are hard to read past two levels. Use a `switch` expression or a statement.
- **Expression statements that do nothing.** `x == 5;` compiles in C and JavaScript and does nothing. Linters and compilers warn about unused values; enable those warnings.
- **Python's comma.** `x = 1,` makes `x` a tuple, because the comma, not the parentheses, makes a tuple expression.

## Cost

The difference between expressions and statements costs nothing at run time: compilers translate both to the same instructions, and an `if` expression produces the same code as an `if` statement that assigns a temporary. The choice affects the cost of reading and changing code. Expression forms remove temporaries and make it clear that each branch gives a value, and the compiler can check that none is missing. Long expressions with side effects cost more to understand than the statements they replace.

## History

FORTRAN, in 1957, separated arithmetic expressions from statements, and its name comes from the translation of formulas. Algol 60 in 1960 defined statements and expressions in a formal grammar and allowed conditional expressions, and Lisp in 1958 had no statements at all: every form was an expression with a value. Algol 68 made almost every construct an expression. BCPL, in 1967, had `VALOF` blocks that give a value, and C, in 1972, kept assignment as an expression with the conditional operator `?:` and the comma operator.

Python, from 1991, made assignment a statement on purpose, and PEP 572 added the assignment expression `:=` in Python 3.8 in 2019, after long debate. Go, in 2009, made `++` and assignment statements and left out the conditional operator. Rust, released in 2015, and Kotlin, in 2016, followed the ML family and made blocks and `if` expressions.[^5] Java 14 in 2020 and C# 8 in 2019 added `switch` expressions.

## References
