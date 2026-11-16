---
tag: 6ZS3BB
kind: feature
owner: python
slug: syntax-errors
name: Syntax Errors
aliases: [SyntaxError, parsing error, IndentationError, TabError, invalid syntax, parser error message]
area: languages/python/errors
level: intro
summary: >-
  A SyntaxError comes from the parser, before the code runs. The message shows the line and marks the place where the parser stopped. IndentationError and TabError are subclasses for bad indentation.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/exceptions.html#SyntaxError", locator: "Built-in Exceptions, SyntaxError, IndentationError, TabError"}
see_also: [python/errors-and-exceptions, python/exceptions, python/the-exception-hierarchy]
sources:
  - url: https://docs.python.org/3.15/tutorial/errors.html#syntax-errors
    title: The Python Tutorial, 8. Errors and Exceptions
    locator: 8.1. Syntax Errors
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/exceptions.html#SyntaxError
    title: Built-in Exceptions
    locator: SyntaxError; IndentationError; TabError
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#indentation
    title: The Python Language Reference, 2. Lexical analysis
    locator: 2.1.8. Indentation
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#semicolons
    title: Effective Go
    locator: Semicolons
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
source = "if count > 3\n    print('many')\n"
try:
    compile(source, "demo.py", "exec")
except SyntaxError as err:
    print(err.msg)
    print(err.filename, err.lineno, err.offset)
    print(repr(err.text))
```

```output for=basics
expected ':'
demo.py 1 13
'if count > 3\n'
```

`compile` parses the text without running it. The colon is missing at column 13 of line 1, and the exception gives the message and the place as attributes.

## Semantics

**r1.** "Syntax errors, also known as parsing errors, are perhaps the most common kind of complaint you get while you are still learning Python".[^1] SyntaxError is "Raised when the parser encounters a syntax error. This may occur in an import statement, in a call to the built-in functions compile(), exec(), or eval(), or when reading the initial script or standard input (also interactively)."[^2]

**r2.** "The parser repeats the offending line and displays little arrows pointing at the place where the error was detected. Note that this is not always the place that needs to be fixed."[^1] Example: [`place`](#place).

**r3.** "The str() of the exception instance returns only the error message."[^2] The attributes are `filename`, `lineno`, `offset`, `text`, `end_lineno` and `end_offset`. `lineno` "is 1-indexed: the first line in the file has a lineno of 1", and so is `offset`.[^2]

**r4.** IndentationError is the "Base class for syntax errors related to incorrect indentation. This is a subclass of SyntaxError."[^2] TabError is "Raised when indentation contains an inconsistent use of tabs and spaces. This is a subclass of IndentationError."[^2] Example: [`indent`](#indent).

**r5.** "Leading whitespace (spaces and tabs) at the beginning of a logical line is used to compute the indentation level of the line, which in turn is used to determine the grouping of statements."[^3] "Indentation is rejected as inconsistent if a source file mixes tabs and spaces in a way that makes the meaning dependent on the worth of a tab in spaces; a TabError is raised in that case."[^3]

## Examples

```python run id=messages
sources = [
    "print('a'",
    "x = [1 2]",
    "if x = 1:\n    pass",
    "print 'hello'",
]
for s in sources:
    try:
        compile(s, "<demo>", "exec")
    except SyntaxError as err:
        print(err.msg)
```

```output for=messages
'(' was never closed
invalid syntax. Perhaps you forgot a comma?
invalid syntax. Maybe you meant '==' or ':=' instead of '='?
Missing parentheses in call to 'print'. Did you mean print(...)?
```

The parser gives a specific message for many common mistakes.

```python run id=indent
for s in ["def f():\nreturn 1", "if True:\n\tx = 1\n        y = 2\n"]:
    try:
        compile(s, "<demo>", "exec")
    except SyntaxError as err:
        print(type(err).__name__, isinstance(err, SyntaxError), err.lineno)
        print(err.msg)
```

```output for=indent
IndentationError True 2
expected an indented block after function definition on line 1
TabError True 3
inconsistent use of tabs and spaces in indentation
```

Both are SyntaxError subclasses, so `except SyntaxError` catches them (rule r4). The second source indents one line with a tab and the next with eight spaces (rule r5).

```python run id=eval
expr = "2 +* 3"
try:
    print(eval(expr))
except SyntaxError as err:
    print("cannot parse", repr(expr), "at column", err.offset)
```

```output for=eval
cannot parse '2 +* 3' at column 4
```

Text given to `eval` is parsed while the program runs, so the program can catch the SyntaxError (rule r1).

## Pitfalls

A try statement cannot catch a syntax error in its own file:

```python fails id=same-file expect=exception:SyntaxError
try:
    total = (1 + 2
except SyntaxError:
    print("caught")
```

The error happens "when reading the initial script",[^2] so no line of the file runs. Fix the code; the try statement helps only for text given to `compile`, `exec` or `eval` (rule r1).

The marked place can be after the real mistake:

```python wrong id=place
try:
    compile("while True print('hi')", "<demo>", "exec")
except SyntaxError as err:
    print(err.offset, err.text[err.offset - 1:].strip())
```

```output for=place
12 print('hi')
```

The marker points at `print`, but the fix is the colon after `True`. The tutorial says the error "is detected at the function print(), since a colon (':') is missing just before it."[^1] Look before the marked place too (rule r2).

## Comparison

Go is also parsed before it runs, and its grammar has its own traps. Effective Go says: "you cannot put the opening brace of a control structure (if, for, switch, or select) on the next line. If you do, a semicolon will be inserted before the brace, which could cause unwanted effects."[^4] In Go the braces group statements. In Python indentation groups them (rule r5), so a bad indent is a syntax error.
