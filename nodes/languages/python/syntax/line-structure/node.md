---
tag: XA9RR3
kind: feature
owner: python
slug: line-structure
name: Line structure
aliases: [logical line, physical line, line continuation, backslash continuation, implicit line joining]
area: languages/python/syntax
level: intro
summary: >-
  A Python statement ends at the end of a logical line. A logical line is one or more physical lines, joined by a final backslash or by open brackets.
grammar: "newline: <ASCII LF> | <ASCII CR> <ASCII LF> | <ASCII CR> (Language Reference, 2.1.2 Physical lines)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/lexical_analysis.html#line-structure", locator: "2.1 Line structure"}
see_also: [python/indentation, python/comments-in-python, python/string-literals]
sources:
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#line-structure
    title: The Python Language Reference, Python 3.15
    locator: 2.1 Line structure; 2.1.1 Logical lines; 2.1.2 Physical lines
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#explicit-line-joining
    title: The Python Language Reference, Python 3.15
    locator: 2.1.5 Explicit line joining
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#implicit-line-joining
    title: The Python Language Reference, Python 3.15
    locator: 2.1.6 Implicit line joining
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#blank-lines
    title: The Python Language Reference, Python 3.15
    locator: 2.1.7 Blank lines; 2.1.10 End marker
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/token.html#token.NL
    title: token, Python 3.15 documentation
    locator: token.NEWLINE, token.NL, token.ENDMARKER
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0008/#maximum-line-length
    title: "PEP 8: Style Guide for Python Code"
    locator: Maximum Line Length
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#string-literal-concatenation
    title: The Python Language Reference, Python 3.15
    locator: 6.2.3.2 String literal concatenation
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Semicolons
    title: The Go Programming Language Specification
    locator: Semicolons
    accessed: 2026-11-16
---

## Synopsis

```python run id=joining
total = 1 + 2 + \
    3 + 4                      # explicit joining: the line ends in a backslash

months = ["Jan", "Feb",        # implicit joining: inside brackets
          "Mar", "Apr"]

print(total, len(months))
```

```output for=joining
10 4
```

## Semantics

**r1.** A program is a sequence of logical lines. The token `NEWLINE` ends a logical line, and a statement cannot cross the end of a logical line, except where the syntax allows a `NEWLINE`, as between the statements of a compound statement.[^1]

**r2.** A physical line ends with LF, CR LF or CR. Each of the three becomes one LF, "even inside string literals". One file can mix them, and the end of the input ends the last line.[^1] Example: [`line-endings`](#line-endings).

**r3.** When a physical line ends in a backslash that is not in a string literal or a comment, the next line joins it. The backslash and the end of line are deleted. A line that ends in a backslash cannot have a comment, and a backslash anywhere else outside a string literal is an error.[^2] Example: [`joining`](#joining), [`comment-after-backslash`](#comment-after-backslash).

**r4.** An expression in parentheses, square brackets or braces can continue over several physical lines with no backslash. These lines can have comments, their indentation does not matter, and they can be blank. There is no `NEWLINE` token between them.[^3] Example: [`line-tokens`](#line-tokens).

**r5.** A logical line with only whitespace and maybe a comment is ignored: it makes no `NEWLINE` token. In the standard interactive interpreter, a completely empty line ends a multi-line statement.[^4]

**r6.** At the end of the input, the lexical analyzer makes an `ENDMARKER` token.[^4] The tokenizer marks a newline that does not end a logical line with the token `NL`, which the parser ignores.[^5] Example: [`line-tokens`](#line-tokens).

## Examples

The tokens at the line ends of a small program (rules r4 to r6). The first two physical lines make one logical line, so only the second one ends with `NEWLINE`. The comment line and the blank line give `NL`.

```python run id=line-tokens
import io
import tokenize

source = "total = (1 +\n         2)\n# a comment\n\nprint(total)\n"
for token in tokenize.generate_tokens(io.StringIO(source).readline):
    if token.type in (tokenize.NEWLINE, tokenize.NL, tokenize.ENDMARKER):
        print(token.start[0], tokenize.tok_name[token.type])
```

```output for=line-tokens
1 NL
2 NEWLINE
3 NL
4 NL
5 NEWLINE
6 ENDMARKER
```

The three line ends become LF, also inside a string literal (rule r2).

```python run id=line-endings
source = b"text = '''one\r\ntwo\rthree\nfour'''\nprint(repr(text))\n"
exec(compile(source, "demo.py", "exec"))
```

```output for=line-endings
'one\ntwo\nthree\nfour'
```

## Pitfalls

### A comment or a space after the backslash

The backslash must be the last character of the line (rule r3). A comment after it, or a space that the editor does not show, is a `SyntaxError`.

```python fails id=comment-after-backslash expect=exception:SyntaxError
total = 1 + 2 + \  # add the rest
    3 + 4
```

PEP 8 prefers the brackets of rule r4 to the backslash: "Long lines can be broken over multiple lines by wrapping expressions in parentheses."[^6]

### A missing comma inside brackets

Inside brackets the line ends do not count (rule r4). Two string literals with only a line end between them join into one string, so a missing comma loses an item with no error.[^3] [^7]

```python wrong id=missing-comma
colours = [
    "red",
    "green"
    "blue",
]
print(len(colours), colours)
```

```output for=missing-comma
2 ['red', 'greenblue']
```

## Comparison

Go ends statements with semicolons, and its lexer inserts a semicolon after the last token of a line when that token can end a statement, such as an identifier, a literal or a closing bracket.[^8] Python has no such insertion step: the end of a logical line is the `NEWLINE` token, and open brackets join the lines (rule r4).
