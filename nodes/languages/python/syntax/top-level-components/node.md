---
tag: MGZ8DP
kind: feature
owner: python
slug: top-level-components
name: Top-level components
aliases: [Python start symbols, file input, interactive input, expression input, complete Python program, eval input]
area: languages/python/syntax
level: advanced
summary: >-
  Python reads code as file input (programs, modules, exec), interactive input (one statement) or expression input (eval). A program runs in the namespace of __main__.
grammar: "file_input: (NEWLINE | statement)* ENDMARKER; interactive_input: [stmt_list] NEWLINE | compound_stmt NEWLINE | ENDMARKER; eval_input: expression_list NEWLINE* ENDMARKER (Language Reference, 9)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/toplevel_components.html", locator: "9. Top-level components"}
see_also: [python/the-python-grammar, python/expression-statements, python/statements-and-suites]
sources:
  - url: https://docs.python.org/3.15/reference/toplevel_components.html
    title: The Python Language Reference, Python 3.15
    locator: 9. Top-level components (9.1 to 9.4)
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/cmdline.html#interface-options
    title: Command line and environment, Python 3.15 documentation
    locator: 1.1.1. Interface options (-c, -, script)
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#compile
    title: Built-in Functions, Python 3.15 documentation
    locator: compile(), exec()
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Program_execution
    title: The Go Programming Language Specification
    locator: Program execution
    accessed: 2026-11-16
---

## Synopsis

```python run id=three-inputs
exec("a = 1\nb = a + 1\nprint(b)")
print(eval("  1 + 2"))
exec(compile("if True:\n    print('one statement')\n", "<demo>", "single"))
```

```output for=three-inputs
2
3
one statement
```

`exec()` reads file input, `eval()` reads expression input and the `'single'` mode of `compile()` reads interactive input.

## Semantics

**r1.** A complete Python program runs in "a minimally initialized environment". All built-in and standard modules are available, but only `sys`, `builtins` and `__main__` are initialized. The `__main__` module gives "the local and global namespace for execution of the complete program".[^1] Example: [`main-namespace`](#main-namespace).

**r2.** A complete program can be passed to the interpreter in three forms: with the `-c` option, as a file that is the first command line argument, or as standard input. If the file or standard input is a terminal, the interpreter enters interactive mode.[^1] The first element of `sys.argv` is `"-c"` for `-c`, `"-"` for standard input named with `-`, and the script name for a file.[^2] Example: [`three-forms`](#three-forms).

**r3.** All input read from non-interactive files has the form `file_input: (NEWLINE | statement)* ENDMARKER`. Python uses it to parse a complete program, a module and a string passed to `exec()`.[^1] The code that `exec()` runs "is expected to be valid as file input".[^3]

**r4.** Interactive mode reads and runs one statement at a time, possibly compound, in the namespace of `__main__`. Its form is `interactive_input: [stmt_list] NEWLINE | compound_stmt NEWLINE | ENDMARKER`. A top-level compound statement must be followed by a blank line in interactive mode, so the parser can find the end of the input.[^1] The `'single'` mode of `compile()` takes "a single interactive statement".[^3] Example: [`one-statement`](#one-statement).

**r5.** `eval()` uses expression input, `eval_input: expression_list NEWLINE* ENDMARKER`, and ignores leading whitespace.[^1] A statement is not an expression list, so `eval()` rejects it. Example: [`eval-forms`](#eval-forms).

## Examples

The global namespace of a program is the namespace of the `__main__` module (rule r1).

```python run id=main-namespace
import sys

print(__name__)
print(sys.modules["__main__"].__dict__ is globals())
```

```output for=main-namespace
__main__
True
```

The same program passed in the three forms of rule r2. The value of `sys.argv[0]` shows which form ran it.

```python run id=three-forms
import subprocess, sys

prog = "import sys\nprint('argv0 =', sys.argv[0])\n"
with open("prog.py", "w") as f:
    f.write(prog)

runs = [
    [sys.executable, "-c", prog],
    [sys.executable, "prog.py"],
    [sys.executable, "-"],
]
for cmd in runs:
    out = subprocess.run(cmd, input=prog, capture_output=True, text=True)
    print(out.stdout, end="")
```

```output for=three-forms
argv0 = -c
argv0 = prog.py
argv0 = -
```

Expression input accepts leading whitespace, trailing newlines and an expression list, but not a statement (rule r5).

```python run id=eval-forms
print(eval("  1 + 2"))
print(eval("1, 2\n\n"))
try:
    eval("x = 1")
except SyntaxError:
    print("SyntaxError")
```

```output for=eval-forms
3
(1, 2)
SyntaxError
```

## Pitfalls

### One interactive statement at a time

Interactive input is one statement (rule r4). Two statements on two lines are not one statement, but a simple statement list on one line is.

```python run id=one-statement
try:
    compile("a = 1\nb = 2\n", "<demo>", "single")
except SyntaxError as e:
    print("SyntaxError:", e.msg)
exec(compile("a = 1; b = 2; print(a + b)\n", "<demo>", "single"))
```

```output for=one-statement
SyntaxError: multiple statements found while compiling a single statement
3
```

### A compound statement needs its end

In interactive input, a line that starts at the left margin right after a compound statement is not part of that statement, and there is no blank line to end it (rule r4). The same text is valid file input.

```python run id=no-blank-line
src = "if True:\n    print('a')\nprint('b')\n"
exec(compile(src, "<demo>", "exec"))
try:
    compile(src, "<demo>", "single")
except SyntaxError:
    print("SyntaxError")
```

```output for=no-blank-line
a
b
SyntaxError
```

## Comparison

The Go specification says: "A complete program is created by linking a single, unimported package called the main package with all the packages it imports". Execution starts with the function `main` of package `main`.[^4] A Python program has no entry function: the interpreter runs the statements of the file input from the top, in the namespace of `__main__` (rules r1 and r3).
