---
tag: A057YY
kind: feature
owner: python
slug: indentation
name: Indentation
aliases: [significant whitespace, off-side rule, INDENT and DEDENT, IndentationError, TabError]
area: languages/python/syntax
level: intro
summary: >-
  Python groups statements by indentation, not by braces. The tokenizer turns changes of indentation into INDENT and DEDENT tokens; bad indentation is an IndentationError or a TabError.
grammar: "block: NEWLINE INDENT statements DEDENT | simple_stmts (Language Reference, 10 Full Grammar specification)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/lexical_analysis.html#indentation", locator: "2.1.8 Indentation"}
see_also: [python/line-structure, python/comments-in-python]
sources:
  - url: https://docs.python.org/3.15/tutorial/introduction.html#first-steps-towards-programming
    title: The Python Tutorial, Python 3.15
    locator: 3.2 First Steps Towards Programming
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#indentation
    title: The Python Language Reference, Python 3.15
    locator: 2.1.8 Indentation
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/exceptions.html#IndentationError
    title: Built-in Exceptions, Python 3.15 documentation
    locator: IndentationError, TabError
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/token.html#token.INDENT
    title: token, Python 3.15 documentation
    locator: token.INDENT, token.DEDENT
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/grammar.html
    title: The Python Language Reference, Python 3.15
    locator: 10 Full Grammar specification, block
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0008/#indentation
    title: "PEP 8: Style Guide for Python Code"
    locator: Indentation; Tabs or Spaces?
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Blocks
    title: The Go Programming Language Specification
    locator: Blocks
    accessed: 2026-11-16
---

## Synopsis

```python run id=blocks
for n in [1, 2, 3]:
    if n % 2:
        print(n, "is odd")
    else:
        print(n, "is even")
print("done")
```

```output for=blocks
1 is odd
2 is even
3 is odd
done
```

The body of a compound statement is the lines after the colon that are indented more than its header. The first line with less indentation ends the body.[^1]

## Semantics

**r1.** The spaces and tabs at the start of a logical line give its indentation level, and the levels group the statements.[^2]

**r2.** A tab moves to the next column that is a multiple of eight. The indentation of a line is the number of columns before its first character that is not blank.[^2]

**r3.** A source file that mixes tabs and spaces so that the meaning depends on the width of a tab is rejected with a `TabError`.[^2] `TabError` is a subclass of `IndentationError`, which is a subclass of `SyntaxError`.[^3] Example: [`tab-error`](#tab-error).

**r4.** The tokenizer keeps a stack of levels, with 0 at the bottom. At the start of each logical line, a level greater than the top is pushed and gives one `INDENT` token. A smaller level must be on the stack: each greater level is popped and gives one `DEDENT` token. At the end of the file, each level above 0 gives a `DEDENT`.[^2] [^4] Example: [`indent-tokens`](#indent-tokens).

**r5.** The levels need not be multiples of anything. Each body can use its own width, as long as the lines of one body agree. A dedent to a level that is not on the stack is an error.[^2] [^1] Example: [`errors`](#errors).

**r6.** Only the start of a logical line counts. The continuation lines inside brackets have free indentation, and a backslash cannot split the indentation over two lines.[^2]

**r7.** In the grammar, a block is `NEWLINE INDENT statements DEDENT`, or simple statements on the line of the header.[^5]

## Examples

The tokens that the indentation of a small program makes (rule r4).

```python run id=indent-tokens
import io
import tokenize

source = "if x:\n    if y:\n        a()\n    b()\nc()\n"
names = []
for token in tokenize.generate_tokens(io.StringIO(source).readline):
    if token.type in (tokenize.INDENT, tokenize.DEDENT):
        names.append(f"{tokenize.tok_name[token.type]}@{token.start[0]}")
print(" ".join(names))
```

```output for=indent-tokens
INDENT@2 INDENT@3 DEDENT@4 DEDENT@5
```

The errors of rules r3 and r5, from `compile()` on CPython 3.12.

```python run id=errors
programs = {
    "unexpected indent": "x = 1\n    y = 2\n",
    "no body": "if True:\nprint('hi')\n",
    "bad dedent": "if True:\n        a = 1\n    b = 2\n",
    "tabs and spaces": "if True:\n        a = 1\n\tb = 2\n",
}
for label, source in programs.items():
    try:
        compile(source, "demo.py", "exec")
    except SyntaxError as error:
        print(f"{label:18} {type(error).__name__}: {error.msg}")
```

```output for=errors
unexpected indent  IndentationError: unexpected indent
no body            IndentationError: expected an indented block after 'if' statement on line 1
bad dedent         IndentationError: unindent does not match any outer indentation level
tabs and spaces    TabError: inconsistent use of tabs and spaces in indentation
```

## Pitfalls

### A line at the wrong level changes the program

A wrong indentation that is still consistent gives no error, and the program does something else. Here the addition is outside the loop, so it runs once, with the last square.

```python wrong id=dedent-mistake
total = 0
for n in [1, 2, 3]:
    square = n * n
total += square
print(total)
```

```output for=dedent-mistake
9
```

Indented one level, `total += square` runs three times and the total is 14.

### Tabs and spaces

A tab and eight spaces are the same column for the rule r2, but the tokenizer rejects a file in which the two forms decide the structure (rule r3). PEP 8 says: "Use 4 spaces per indentation level", and "Spaces are the preferred indentation method".[^6]

```python fails id=tab-error expect=exception:TabError
compile("if True:\n        a = 1\n\tb = 2\n", "demo.py", "exec")
```

## Comparison

In Go, a block is "a possibly empty sequence of declarations and statements within matching brace brackets", and the indentation has no meaning.[^7] In Python the indentation is the block (rule r7), so a program cannot look grouped one way and run another way.
