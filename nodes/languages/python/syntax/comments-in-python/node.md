---
tag: D572D9
kind: feature
owner: python
slug: comments-in-python
name: Comments in Python
aliases: [hash comment, Python comment, inline comment, block comment in Python]
area: languages/python/syntax
level: intro
summary: >-
  A Python comment starts with # outside a string literal and ends at the end of the physical line. Python has no block comment, and a docstring is a string, not a comment.
grammar: "comment: '#' up to the end of the physical line (Language Reference, 2.1.3 Comments)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/lexical_analysis.html#comments", locator: "2.1.3 Comments"}
see_also: [python/line-structure, python/source-code-encoding, python/string-literals]
sources:
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#comments
    title: The Python Language Reference, Python 3.15
    locator: 2.1.3 Comments
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/introduction.html#an-informal-introduction-to-python
    title: The Python Tutorial, Python 3.15
    locator: 3 An Informal Introduction to Python
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0008/#comments
    title: "PEP 8: Style Guide for Python Code"
    locator: Comments; Block Comments; Inline Comments
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/glossary.html#term-docstring
    title: Glossary, Python 3.15 documentation
    locator: docstring
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/cmdline.html#cmdoption-OO
    title: Python Setup and Usage, Python 3.15
    locator: 1.1.3 Miscellaneous options, -OO
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#encoding-declarations
    title: The Python Language Reference, Python 3.15
    locator: 2.1.4 Encoding declarations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/appendix.html#executable-python-scripts
    title: The Python Tutorial, Python 3.15
    locator: 16.1.2 Executable Python Scripts
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/token.html#token.COMMENT
    title: token, Python 3.15 documentation
    locator: token.COMMENT
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Comments
    title: The Go Programming Language Specification
    locator: Comments
    accessed: 2026-11-16
---

## Synopsis

```python run id=comments
# A block comment: one or more lines, each one starting with "#".
rate = 0.2  # an inline comment, after the code
text = "# not a comment, because it is inside a string literal"
print(rate, text)
```

```output for=comments
0.2 # not a comment, because it is inside a string literal
```

## Semantics

**r1.** A comment starts with a `#` that is not part of a string literal, and it ends at the end of the physical line. The syntax ignores comments.[^1] A `#` inside a string literal is just a character.[^2] Example: [`comments`](#comments).

**r2.** A comment ends the logical line, unless the line is inside brackets, where the implicit joining continues the line.[^1]

**r3.** There is no block comment. A comment of several lines is several comments, one on each line.[^3]

**r4.** A string literal that is the first expression of a module, a class or a function is its docstring. The suite ignores it when it runs, but the compiler stores it in the attribute `__doc__`.[^4] So a docstring is data at run time, unlike a comment. The option `-OO` discards docstrings.[^5] Example: [`docstring`](#docstring).

**r5.** Two kinds of comment on the first lines of a file have a meaning outside the syntax. A comment that matches `coding[=:]` on line 1 or 2 declares the encoding of the file.[^6] A first line that starts with `#!` is the shebang line that Unix uses to run the file as a command.[^7]

**r6.** The tokenizer keeps comments as `COMMENT` tokens, and the parser ignores them.[^8] So a tool that reads the code can see the comments, but the program cannot. Example: [`comment-tokens`](#comment-tokens).

## Examples

A comment is a token for the tokenizer, but it is not in the syntax tree (rules r1 and r6). `ast.unparse` rebuilds the code from the tree, so the comment is gone.

```python run id=comment-tokens
import ast
import io
import tokenize

source = "x = 1  # set x\n"
for token in tokenize.generate_tokens(io.StringIO(source).readline):
    if token.type == tokenize.COMMENT:
        print("token:", repr(token.string))
print("tree: ", ast.unparse(ast.parse(source)))
```

```output for=comment-tokens
token: '# set x'
tree:  x = 1
```

A docstring stays in the program, and `-OO` removes it (rule r4).

```python run id=docstring
import subprocess
import sys

program = '''
def area(width, height):
    """Return the area of a rectangle."""
    # The comment is not stored anywhere.
    return width * height

print(area.__doc__)
'''
for options in ([], ["-OO"]):
    result = subprocess.run([sys.executable, *options, "-c", program], capture_output=True, text=True)
    print(options or "default", "->", result.stdout.strip())
```

```output for=docstring
default -> Return the area of a rectangle.
['-OO'] -> None
```

## Pitfalls

### A triple-quoted string is not a block comment

A triple-quoted string can hide code, but it is still a string literal. Inside brackets it becomes a value, and next to another string literal it joins it.

```python wrong id=string-as-comment
options = [
    "fast",
    """
    "slow",
    """
    "safe",
]
print(len(options))
print(repr(options[1]))
```

```output for=string-as-comment
2
'\n    "slow",\n    safe'
```

To turn off several lines, put `#` at the start of each line (rule r3).

### Comments that repeat the code

PEP 8 says that inline comments "are unnecessary and in fact distracting if they state the obvious", and that they should be separated from the code by at least two spaces.[^3]

## Comparison

Go has two forms: line comments that start with `//`, and general comments between `/*` and `*/`.[^9] Python has only the line form (rule r3). Its docstrings take the place of the documentation comments of other languages, and they stay in the program as data (rule r4).
