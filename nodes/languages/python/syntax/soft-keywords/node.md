---
tag: 8VFM8Q
kind: feature
owner: python
slug: soft-keywords
name: Soft keywords
aliases: [contextual keywords in Python, keyword.softkwlist, match as a name, type statement keyword, lazy keyword]
area: languages/python/syntax
level: advanced
summary: >-
  A soft keyword, such as match, case, type or lazy, acts as a keyword only in one place of the grammar. Everywhere else it is an ordinary identifier.
grammar: "match_stmt: \"match\" subject_expr ':' NEWLINE INDENT case_block+ DEDENT (PEP 634; soft keywords in double quotes)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/lexical_analysis.html#soft-keywords", locator: "2.3.2 Soft Keywords"}
see_also: [python/keywords, python/identifiers-in-python, python/match-statement]
sources:
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#soft-keywords
    title: The Python Language Reference, Python 3.15
    locator: 2.3.2 Soft Keywords
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0634/#the-match-statement
    title: "PEP 634: Structural Pattern Matching: Specification"
    locator: The Match Statement
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/keyword.html#keyword.softkwlist
    title: keyword, Python 3.15 documentation
    locator: keyword.issoftkeyword(), keyword.softkwlist
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#keywords
    title: The Python Language Reference, Python 3.15
    locator: 2.3.1 Keywords
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0634/#wildcard-pattern
    title: "PEP 634: Structural Pattern Matching: Specification"
    locator: Wildcard Pattern
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0695/#generic-type-alias
    title: "PEP 695: Type Parameter Syntax"
    locator: Generic Type Alias; Grammar Changes
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/whatsnew/3.15.html#whatsnew315-lazy-imports
    title: What's New In Python 3.15
    locator: "PEP 810: Explicit lazy imports"
    accessed: 2026-11-16
---

## Synopsis

```python run id=match-as-name
import re

match = re.match(r"\d+", "42 apples")
match match:
    case None:
        print("no number")
    case _:
        print("number", match.group())
```

```output for=match-as-name
number 42
```

The first `match` is a variable. The second line is a `match` statement whose subject is that variable.

## Semantics

**r1.** Some names are "only reserved under specific contexts": `match`, `case` and `_` in the `match` statement, `type` in the `type` statement, and `lazy` before an `import` statement.[^1]

**r2.** The tokenizer makes an ordinary `NAME` token for a soft keyword. The parser decides from the context whether the name is a keyword: "this distinction is done at the parser level, not when tokenizing".[^1] Example: [`soft-tokens`](#soft-tokens).

**r3.** Outside its context, a soft keyword is an identifier. So the new statements keep "compatibility with existing code that uses these names as identifier names".[^1] For `match` and `case`, PEP 634 says that they "are allowed to be used in all other contexts as variable or argument names", and that they are not reserved "at the start of a line if there is no colon where expected".[^2] Example: [`match-as-name`](#match-as-name).

**r4.** `keyword.softkwlist` holds the soft keywords of the running interpreter, and `keyword.issoftkeyword(s)` tests a string.[^3] The hard keywords are a separate list, and a hard keyword is never an identifier.[^4] Example: [`softkwlist`](#softkwlist).

**r5.** In a `case` pattern, `_` is the wildcard. "A wildcard pattern always succeeds. It binds no name."[^5] Example: [`wildcard-binds-nothing`](#wildcard-binds-nothing).

**r6.** `type` "is interpreted as a keyword only in this part of the grammar", the `type` statement. "In all other locations, it is assumed to be an identifier name", so the built-in function `type` still works.[^6] Example: [`type-statement`](#type-statement).

**r7.** In Python 3.15, `lazy` before `import` or `from ... import` makes the import lazy: the module is loaded when the imported name is "first used".[^7]

## Examples

The soft keywords of CPython 3.12, the version that runs these examples (rule r4). `lazy` is in the list from 3.15 on.

```python run id=softkwlist
import keyword

print(keyword.softkwlist)
print(keyword.iskeyword("match"), keyword.issoftkeyword("match"))
```

```output for=softkwlist
['_', 'case', 'match', 'type']
False True
```

The tokenizer sees a `NAME`, whatever the role of the word (rule r2).

```python run id=soft-tokens
import io
import tokenize

source = "match point:\n    case _:\n        type = 1\n"
for token in tokenize.generate_tokens(io.StringIO(source).readline):
    if token.string in ("match", "case", "_", "type"):
        print(token.string, tokenize.tok_name[token.type])
```

```output for=soft-tokens
match NAME
case NAME
_ NAME
type NAME
```

A `type` statement and the built-in `type` in one program (rule r6). The statement needs Python 3.12.

```python run id=type-statement since=3.12
type Pair = tuple[int, int]
print(type(Pair).__name__, Pair.__value__)
```

```output for=type-statement
TypeAliasType tuple[int, int]
```

## Versions

> [!since v="3.10"]
> The `match` statement brought the first soft keywords: `match`, `case` and `_`.[^1]

The 3.12 release made `type` a soft keyword for the `type` statement of PEP 695, and the 3.15 release made `lazy` one, for the explicit lazy imports of PEP 810.[^1] [^7] Old code that uses these words as names still runs (rule r3).

## Pitfalls

### `case _` does not bind `_`

The wildcard matches anything and binds nothing (rule r5). To use the matched value, write a capture pattern with a real name, such as `case [verb, target]`.

```python fails id=wildcard-binds-nothing expect=exception:NameError
command = "go north"
match command.split():
    case ["go", _]:
        print("going", _)
```

### A soft keyword as a name can confuse the reader

A variable named `match`, `case` or `type` is valid (rule r3), but a reader of `match match:` must work out which word is which.
