---
tag: WA755V
kind: feature
owner: python
slug: statements-and-suites
name: Statements and suites
aliases: [Python statements, simple statements, compound statements, suite, clause, block, semicolon]
area: languages/python/syntax
level: intro
summary: >-
  A Python statement is simple (one logical line) or compound (clauses whose headers end in a colon). A suite is an indented block, or simple statements on the header line.
grammar: "suite: stmt_list NEWLINE | NEWLINE INDENT statement+ DEDENT; statement: stmt_list NEWLINE | compound_stmt; stmt_list: simple_stmt (';' simple_stmt)* [';'] (Language Reference, 8)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html", locator: "8 Compound statements"}
see_also: [python/indentation, python/line-structure, python/expression-statements]
sources:
  - url: https://docs.python.org/3.15/reference/simple_stmts.html
    title: The Python Language Reference, Python 3.15
    locator: 7 Simple statements
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html
    title: The Python Language Reference, Python 3.15
    locator: 8 Compound statements
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#the-pass-statement
    title: The Python Language Reference, Python 3.15
    locator: 7.4 The pass statement
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0008/#other-recommendations
    title: "PEP 8: Style Guide for Python Code"
    locator: Other Recommendations
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Blocks
    title: The Go Programming Language Specification
    locator: Blocks
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#If_statements
    title: The Go Programming Language Specification
    locator: If statements
    accessed: 2026-11-16
---

## Synopsis

```python run id=suites
for n in range(3):
    if n % 2 == 0:
        print(n, "even")
    else:
        print(n, "odd")
total = 0; total += 5; print(total)
```

```output for=suites
0 even
1 odd
2 even
5
```

## Semantics

**r1.** "A simple statement is comprised within a single logical line." Several simple statements may share a line, separated by semicolons.[^1] Example: [`suites`](#suites).

**r2.** "Compound statements contain (groups of) other statements". They include `if`, `while`, `for`, `try`, `with`, `match`, and function and class definitions.[^2] Example: [`suites`](#suites).

**r3.** A compound statement consists of one or more clauses. A clause has a header and a suite; the headers of one statement are all at the same indentation level, and each begins with a keyword and ends with a colon.[^2] Example: [`suites`](#suites).

**r4.** A suite is either semicolon-separated simple statements on the header's line, after the colon, or indented statements on the following lines. "Only the latter form of a suite can contain nested compound statements".[^2] Example: [`nested-one-line`](#nested-one-line).

**r5.** The reference notes that "the semicolon binds tighter than the colon in this context", so in `if x < y < z: print(x); print(y); print(z)` either all or none of the calls run.[^2] Example: [`semicolon-binds`](#semicolon-binds).

**r6.** Optional continuation clauses such as `else` begin with a keyword that cannot start a statement, and nested `if` statements must be indented, which removes the dangling `else` problem.[^2] Example: [`suites`](#suites).

**r7.** A suite cannot be empty. `pass` "is a null operation" for places where a statement is required syntactically but no code needs to run.[^3] Example: [`pass-placeholder`](#pass-placeholder).

**r8.** PEP 8 says that "Compound statements (multiple statements on the same line) are generally discouraged".[^4] Example: [`one-line-suite`](#one-line-suite).

## Examples

One-line suites and a trailing semicolon (rules r4 and r5). The grammar allows a final `;` in a statement list.[^2]

```python run id=one-line-suite
x, y, z = 1, 2, 3
if x < y < z: print(x); print(y); print(z)
print("end");
```

```output for=one-line-suite
1
2
3
end
```

Placeholders for suites with nothing to do yet (rule r7).

```python run id=pass-placeholder
class Empty:
    pass

def todo():
    ...

for _ in range(3):
    pass
print(Empty.__name__, todo())
```

```output for=pass-placeholder
Empty None
```

`...` works too: it is an expression statement that evaluates the `Ellipsis` constant.

## Pitfalls

### A one-line suite takes the whole line

Everything after the colon belongs to the suite (rule r5), so the second `print` is skipped too.

```python wrong id=semicolon-binds
ready = False
if ready: print("start"); print("this is skipped too")
print("after")
```

```output for=semicolon-binds
after
```

### No nested compound statement on one line

A one-line suite holds only simple statements (rule r4).

```python fails id=nested-one-line expect=exception:SyntaxError
x = 1
if x: if x > 0: print("positive")
```

### An empty suite

A header must be followed by at least one statement (rule r7). Without an indented line the parser raises `IndentationError`.

```python fails id=empty-suite expect=exception:IndentationError
for n in range(3):
print(n)
```

## Comparison

In Go, "A block is a possibly empty sequence of declarations and statements within matching brace brackets."[^5] An `if` statement always takes a block, and `else` is followed by another `if` or a block.[^6] Go marks a suite with braces and can leave it empty; Python marks it with the colon and indentation, and needs `pass` for an empty one (rule r7).
