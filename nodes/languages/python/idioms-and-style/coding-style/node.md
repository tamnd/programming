---
tag: X3B0MM
kind: feature
owner: python
slug: coding-style
name: "Intermezzo: Coding Style"
aliases: [PEP 8, style guide, code layout, line length, 79 characters, four-space indentation, pycodestyle]
area: languages/python/idioms-and-style
level: intro
summary: >-
  PEP 8 is the style guide that most Python projects follow. It asks for 4-space indentation, lines of at most 79 characters, blank lines between definitions, and spaces around operators.
grammar: null
spec: {standard: null, url: "https://peps.python.org/pep-0008/", locator: "PEP 8, Style Guide for Python Code"}
proposals: [pep-8, pep-20]
see_also: [python/naming-conventions, python/documentation-strings, python/import-conventions, python/pythonic-code, python/indentation, python/line-structure, python/comments-in-python, python/source-code-encoding]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#intermezzo-coding-style
    title: The Python Tutorial, Python 3.15 documentation
    locator: 4.10 Intermezzo, Coding Style
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0008/
    title: PEP 8, Style Guide for Python Code
    locator: A Foolish Consistency is the Hobgoblin of Little Minds; Code Lay-out; Whitespace in Expressions and Statements; Comments
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0020/
    title: PEP 20, The Zen of Python
    locator: The Zen of Python
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html
    title: The Python Language Reference, Python 3.15 documentation
    locator: 2.1.5 Explicit line joining; 2.1.8 Indentation
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/
    title: Rules, Ruff documentation
    locator: pycodestyle (E, W), E101, E501, W505
    accessed: 2026-11-18
  - url: https://google.github.io/styleguide/pyguide.html
    title: Google Python Style Guide
    locator: 3.2 Line length; 3.4 Indentation
    accessed: 2026-11-18
---

## Synopsis

```python run id=styled
MAX_WIDTH = 79


class Report:
    """Collect lines and check their width."""

    def __init__(self, title):
        self.title = title
        self.lines = []

    def add(self, text):
        # Keep each line short enough to read side by side.
        self.lines.append(text)
        return len(text) <= MAX_WIDTH


report = Report("style")
print(report.add("a = f(1, 2) + g(3, 4)"))
print(report.add("x" * 80))
```

```output for=styled
True
False
```

## Semantics

**r1.** "For Python, PEP 8 has emerged as the style guide that most projects adhere to; it promotes a very readable and eye-pleasing coding style."[^1] The tutorial section lists the most important points of PEP 8. The interpreter does not check style. Style is a convention, and tools such as linters check it.

**r2.** The reason for a style guide is that "code is read much more often than it is written."[^2] PEP 20 says it in two words: "Readability counts."[^3]

**r3.** Consistency has levels: "Consistency with this style guide is important. Consistency within a project is more important. Consistency within one module or function is the most important."[^2] A project guide wins over PEP 8: "In the event of any conflicts, such project-specific guides take precedence for that project."[^2]

**r4.** Indentation: "Use 4-space indentation, and no tabs."[^1] "Tabs introduce confusion, and are best left out."[^1] The language itself rejects some mixes of tabs and spaces: "Indentation is rejected as inconsistent if a source file mixes tabs and spaces in a way that makes the meaning dependent on the worth of a tab in spaces; a TabError is raised in that case."[^4] Example: [`mixed-tabs`](#mixed-tabs).

**r5.** Line length: "Limit all lines to a maximum of 79 characters."[^2] For docstrings and comments, "the line length should be limited to 72 characters."[^2] A team can agree on more: "it is okay to increase the line length limit up to 99 characters, provided that comments and docstrings are still wrapped at 72 characters."[^2] Example: [`width-check`](#width-check).

**r6.** Long lines wrap inside brackets. "The preferred way of wrapping long lines is by using Python’s implied line continuation inside parentheses, brackets and braces."[^2] These "should be used in preference to using a backslash for line continuation."[^2] For new code, PEP 8 suggests that the line breaks before a binary operator, not after it. Example: [`wrap`](#wrap).

**r7.** Blank lines: "Surround top-level function and class definitions with two blank lines." "Method definitions inside a class are surrounded by a single blank line."[^2] Inside a function, use blank lines "sparingly, to indicate logical sections."[^2]

**r8.** Spaces: "Use spaces around operators and after commas, but not directly inside bracketing constructs: a = f(1, 2) + g(3, 4)."[^1] There is no space around the `=` of a keyword argument or of a default value, but an annotated parameter with a default has spaces around `=`.[^2] Example: [`whitespace`](#whitespace).

**r9.** Comments: "When possible, put comments on a line of their own."[^1] "Comments that contradict the code are worse than no comments."[^2] An inline comment is "separated by at least two spaces from the statement" and starts "with a # and a single space."[^2]

**r10.** Names and encodings: "the convention is to use UpperCamelCase for classes and lowercase_with_underscores for functions and methods. Always use self as the name for the first method argument".[^1] For the source text, "Python’s default, UTF-8, or even plain ASCII work best in any case."[^1] For the full naming rules, see [[python/naming-conventions]].

**r11.** PEP 8 also says when to break a rule, for example "When applying the guideline would make the code less readable, even for someone who is used to reading code that follows this PEP."[^2] It warns: "do not break backwards compatibility just to comply with this PEP!"[^2]

## Examples

```python run id=width-check
source = (
    "def area(width, height):\n"
    "\treturn width * height\n"
    "total = area(3, 4)  # inline comment\n"
    + "x = " + "1 + " * 20 + "1\n"
)
for number, line in enumerate(source.splitlines(), start=1):
    problems = []
    if len(line) > 79:
        problems.append(f"line too long ({len(line)} > 79)")
    if line.startswith("\t"):
        problems.append("tab indentation")
    if problems:
        print(number, ", ".join(problems))
```

```output for=width-check
2 tab indentation
4 line too long (85 > 79)
```

This small checker does two of the checks that a linter does. Ruff has the rules E101 mixed-spaces-and-tabs and E501 line-too-long from pycodestyle, and W505 doc-line-too-long for docstrings and comments.[^5] The parentheses around the string join its parts over four lines with no backslash (rule r6).

```python run id=wrap
gross_wages = 52_000
taxable_interest = 1_200
dividends = 3_000
qualified_dividends = 1_000
ira_deduction = 6_000
student_loan_interest = 2_500

income = (gross_wages
          + taxable_interest
          + (dividends - qualified_dividends)
          - ira_deduction
          - student_loan_interest)
print(income)
```

```output for=wrap
46700
```

Each operator is next to its operand, so the eye can see which items are added and which are subtracted. PEP 8 permits a break "before or after a binary operator, as long as the convention is consistent locally. For new code Knuth’s style is suggested."[^2]

```python run id=whitespace
def scale(value, factor=2.0):
    return value * factor


def label(text: str, sep: str = ": ") -> str:
    return "note" + sep + text


print(scale(3, factor=1.5), label("ok"))
```

```output for=whitespace
4.5 note: ok
```

`factor=2.0` and `factor=1.5` have no spaces around `=`. The annotated parameter `sep` has them, and so does the `->` arrow (rule r8).

```python run id=long-with
import io

with (
    io.StringIO("data") as src,
    io.StringIO() as dst,
):
    dst.write(src.read().upper())
    print(dst.getvalue())
```

```output for=long-with
DATA
```

A long `with` statement can put its context managers in parentheses and wrap them over lines, so it needs no backslash.

## Versions

> [!since v="3.10"]
> A `with` statement can put its context managers in parentheses. PEP 8 notes that long `with` statements "could not use implicit continuation before Python 3.10, so backslashes were acceptable for that case".[^2]

## Pitfalls

```python fails id=mixed-tabs expect=exception:TabError
source = "if True:\n\tx = 1\n        y = 2\n"
compile(source, "mixed.py", "exec")
```

The first line of the block starts with a tab, and the second starts with eight spaces. The meaning depends on the width of a tab, so the compiler raises `TabError` (rule r4). Use four spaces at each level, and set the editor to insert spaces.

```python fails id=space-after-backslash expect=exception:SyntaxError
source = "total = 1 + \\ \n    2\n"
compile(source, "trailing.py", "exec")
```

The backslash has a space after it, so it is not at the end of the line, and it does not join the lines. PEP 8 warns about trailing whitespace: "a backslash followed by a space and a newline does not count as a line continuation marker."[^2] Wrap the expression in parentheses instead.

```python wrong id=comment-drift
def discount(price):
    # Take 10 percent off.
    return price * 0.8


print(discount(100))
```

```output for=comment-drift
80.0
```

The code takes 20 percent off, and the comment says 10 percent. A reader cannot know which one is the intent (rule r9). Change the comment when you change the code.

## Comparison

Other guides follow PEP 8 with small changes. The Google Python Style Guide says "Maximum line length is 80 characters." and "Indent your code blocks with 4 spaces."[^6] A team picks one guide and a linter that checks it, such as the pycodestyle rules in Ruff.[^5] For the rules of the language itself, see [[python/indentation]] and [[python/line-structure]]. For comments, see [[python/comments-in-python]]. For names, docstrings and imports, see [[python/naming-conventions]], [[python/documentation-strings]] and [[python/import-conventions]].
