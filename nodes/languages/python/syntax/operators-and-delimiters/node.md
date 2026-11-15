---
tag: X3K37K
kind: feature
owner: python
slug: operators-and-delimiters
name: Operators and delimiters
aliases: [Python operator tokens, Python punctuation, OP token, exact_type, Python delimiters]
area: languages/python/syntax
level: intro
summary: >-
  Python's operators and delimiters are the symbol tokens, such as +, **, :=, ->, brackets and commas. The tokenizer reads the longest token it can, so ++ is two plus signs.
grammar: "OP: assignment_operator | bitwise_operator | comparison_operator | enclosing_delimiter | other_delimiter | arithmetic_operator | '...' | other_op (Language Reference, 2.7)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/lexical_analysis.html#operators-and-delimiters", locator: "2.7 Operators and delimiters"}
see_also: [python/keywords, python/numeric-literals, python/line-structure]
sources:
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#operators-and-delimiters
    title: The Python Language Reference, Python 3.15
    locator: 2.7 Operators and delimiters
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#other-tokens
    title: The Python Language Reference, Python 3.15
    locator: 2.2 Other tokens
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#whitespace-between-tokens
    title: The Python Language Reference, Python 3.15
    locator: 2.1.9 Whitespace between tokens
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/tokenize.html#tokenize.TokenInfo
    title: tokenize, Python 3.15 documentation
    locator: tokenize.TokenInfo, exact_type
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/token.html#token.OP
    title: token, Python 3.15 documentation
    locator: token.OP and the table of operator tokens
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/whatsnew/3.0.html
    title: What's New In Python 3.0
    locator: Removed Syntax
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#IncDec_statements
    title: The Go Programming Language Specification
    locator: IncDec statements
    accessed: 2026-11-16
---

## Synopsis

```python run id=op-names
import io
import tokenize

source = "total += price * 2 ** n // 3 != limit\n"
for token in tokenize.generate_tokens(io.StringIO(source).readline):
    if token.type == tokenize.OP:
        print(f"{token.string:3} {tokenize.tok_name[token.exact_type]}")
```

```output for=op-names
+=  PLUSEQUAL
*   STAR
**  DOUBLESTAR
//  DOUBLESLASH
!=  NOTEQUAL
```

## Semantics

**r1.** The operators and delimiters are the tokens of the generic type `OP`. The grammar sorts them into groups:[^1]

| Group | Tokens |
|---|---|
| assignment | `+=` `-=` `*=` `**=` `/=` `//=` `%=` `&=` `\|=` `^=` `<<=` `>>=` `@=` `:=` |
| bitwise | `&` `\|` `^` `~` `<<` `>>` |
| comparison | `<=` `>=` `<` `>` `==` `!=` |
| enclosing delimiters | `(` `)` `[` `]` `{` `}` |
| other delimiters | `,` `:` `!` `;` `=` `->` |
| arithmetic | `+` `-` `**` `*` `//` `/` `%` |
| other | `.` `@` `...` |

**r2.** There is "no clear, formal distinction" between operators and delimiters. `*` is the multiplication operator and the delimiter of unpacking, and `@` is matrix multiplication and the start of a decorator. Some operators, such as `and`, `or` and `not in`, are keyword tokens, not `OP` tokens.[^1] Example: [`same-symbol`](#same-symbol).

**r3.** Three periods, `...`, are the `Ellipsis` literal.[^1]

**r4.** Where the text could be read in more than one way, a token is "the longest possible string that forms a legal token, when read from left to right".[^2] Whitespace is needed between two tokens only when their concatenation would be a different token.[^3] Example: [`longest-match`](#longest-match).

**r5.** `tokenize` reports all these tokens with the type `OP`, and the `exact_type` property gives the specific type.[^4] The `token` module lists the names, such as `LPAR` for `(`.[^5] Example: [`op-names`](#op-names).

**r6.** Some symbols are not tokens at all. Python 3.0 removed the operator `<>`, to be written `!=`, and the backticks, to be written as a call of `repr()`.[^6] The characters `$` and `?` are in no token of the grammar of rule r1, so outside strings and comments they are an error.[^1]

## Examples

One token, two roles (rule r2): `*` multiplies and unpacks, and `@` is a decorator here.

```python run id=same-symbol
def shout(function):
    return lambda *args: function(*args).upper()

@shout
def greet(name):
    return "hi " + name

first, *rest = [1, 2, 3]
print(greet("ana"), first * 10, rest, ...)
```

```output for=same-symbol
HI ANA 10 [2, 3] Ellipsis
```

The longest match decides the tokens (rule r4). `<<=` is one token, and `**-` is two.

```python run id=longest-match
import io
import tokenize

for source in ["x<<=1\n", "2**-1\n", "a--b\n"]:
    strings = [t.string for t in tokenize.generate_tokens(io.StringIO(source).readline) if t.type == tokenize.OP]
    print(source.strip(), strings)
```

```output for=longest-match
x<<=1 ['<<=']
2**-1 ['**', '-']
a--b ['-', '-']
```

## Pitfalls

### `++` is not an increment

There is no `++` token (rule r1), so `++count` is two unary plus signs, and the value does not change. `count++` is a `SyntaxError`. Write `count += 1`.

```python wrong id=plus-plus
count = 5
++count
print(count)
```

```output for=plus-plus
5
```

```python fails id=postfix-increment expect=exception:SyntaxError
count = 5
count++
```

### `=` where `==` is meant

`=` is a delimiter of assignment, not a comparison (rule r1), so it is an error in a condition.

```python fails id=assign-in-if expect=exception:SyntaxError
x = 3
if x = 3:
    print("three")
```

## Comparison

Go has increment and decrement statements: `x++` and `x--` "increment or decrement their operands by the untyped constant 1".[^7] Python has no such statement; an augmented assignment such as `count += 1` takes its place.
