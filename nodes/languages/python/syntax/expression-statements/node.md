---
tag: N7CT24
kind: feature
owner: python
slug: expression-statements
name: Expression statements
aliases: [Python expression statement, procedure call statement, bare expression, statement context]
area: languages/python/syntax
level: intro
summary: >-
  An expression statement is an expression on a line of its own, such as a function call. Python evaluates it and discards the value; in interactive mode a value other than None is printed.
grammar: "expression_stmt: starred_expression (Language Reference, 7.1)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#expression-statements", locator: "7.1 Expression statements"}
see_also: [python/expression-lists, python/statements-and-suites, python/evaluation-order-in-python]
sources:
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#expression-statements
    title: The Python Language Reference, Python 3.15
    locator: 7.1 Expression statements
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#compile
    title: Built-in Functions, Python 3.15 documentation
    locator: compile()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/glossary.html#term-docstring
    title: Glossary, Python 3.15 documentation
    locator: docstring
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Expression_statements
    title: The Go Programming Language Specification
    locator: Expression statements
    accessed: 2026-11-16
---

## Synopsis

```python run id=expr-stmts
def greet(name):
    print("hi", name)

greet("ana")
len("ignored")
[1, 2, 3].append(4)
"just a string"
...
```

```output for=expr-stmts
hi ana
```

Five expression statements; only the first one prints, because it calls `print`.

## Semantics

**r1.** Expression statements are used "(mostly interactively) to compute and write a value, or (usually) to call a procedure", a function that returns no meaningful result; Python procedures return `None`. Other uses are allowed.[^1] Example: [`expr-stmts`](#expr-stmts).

**r2.** An expression statement evaluates the expression list, which may be a single expression, and discards the result.[^1] Any expression is allowed, so a bare name or comparison is legal and does nothing visible. Example: [`compare-not-assign`](#compare-not-assign).

**r3.** In interactive mode, a value that is not `None` is converted to a string with `repr()` and written to standard output on a line of its own; `None` is not written, so procedure calls print nothing extra.[^1] The `'single'` mode of `compile()` gives the same behavior: there, expression statements whose value is not `None` are printed.[^2] Example: [`single-mode`](#single-mode).

**r4.** A string literal that is the first expression in a class, function or module is a docstring. It is ignored when the suite runs, but the compiler puts it in the `__doc__` attribute.[^3] Example: [`docstring-expr`](#docstring-expr).

## Examples

The same expression statement prints its value in `'single'` mode and nothing in `'exec'` mode (rule r3).

```python run id=single-mode
for mode in ["single", "exec"]:
    print("mode", mode)
    exec(compile("40 + 2\n", "<demo>", mode))
    exec(compile("None\n", "<demo>", mode))
```

```output for=single-mode
mode single
42
mode exec
```

A docstring is an expression statement too (rule r4).

```python run id=docstring-expr
def area(r):
    "Return the area of a circle of radius r."
    return 3.14159 * r * r

print(area.__doc__)
```

```output for=docstring-expr
Return the area of a circle of radius r.
```

## Pitfalls

### Forgetting the parentheses

A function name on its own is a valid expression statement (rule r2): it looks the function up and does not call it.

```python wrong id=missing-call
def save():
    print("saved")

save
print("done")
```

```output for=missing-call
done
```

### `==` where `=` was meant

`count == 1` compares and throws the result away (rule r2), so `count` keeps its old value.

```python wrong id=compare-not-assign
count = 0
count == 1
print(count)
```

```output for=compare-not-assign
0
```

## Comparison

Go allows only function and method calls and receive operations "in statement context", and even some built-in functions such as `len` are not permitted there.[^4] So the Go compiler rejects the two pitfalls above, while Python runs them (rule r2).
