```python run id=peg-ordered-choice
import re


def lit(s):
    return lambda text, i: i + len(s) if text.startswith(s, i) else None


def seq(*parsers):
    def parse(text, i):
        for p in parsers:
            i = p(text, i)
            if i is None:
                return None
        return i
    return parse


def choice(*parsers):  # PEG: the first alternative that matches wins
    def parse(text, i):
        for p in parsers:
            j = p(text, i)
            if j is not None:
                return j
        return None
    return parse


def end(text, i):
    return i if i == len(text) else None


short_first = seq(choice(lit("<"), lit("<=")), end)
long_first = seq(choice(lit("<="), lit("<")), end)
for text in ["<", "<="]:
    print(f"{text!r:5} short first: {short_first(text, 0) is not None!s:5}"
          f"  long first: {long_first(text, 0) is not None}")
print("regex <|<= on '<=':", re.fullmatch("<|<=", "<=") is not None)
```

```output for=peg-ordered-choice
'<'   short first: True   long first: True
'<='  short first: False  long first: True
regex <|<= on '<=': True
```

```python wrong id=right-recursion-groups-to-the-right
import re


def tokens(text):
    return re.findall(r"\d+|\S", text)


def expr(ts):  # expr = number [ "-" expr ]   (right recursive)
    left = int(ts.pop(0))
    if ts and ts[0] == "-":
        ts.pop(0)
        return left - expr(ts)
    return left


print("10 - 3 - 2 =", expr(tokens("10 - 3 - 2")))
```

```output for=right-recursion-groups-to-the-right
10 - 3 - 2 = 9
```

- Verdict: library. The standard library has no parser generator, so programs use Lark, pyparsing, parsimonious or the Python target of ANTLR. CPython itself parses with a PEG grammar, and the language reference writes its syntax in a mix of EBNF and PEG notation. The run block builds the three PEG operators as functions and shows ordered choice: when `"<"` is the first alternative, it matches the start of `<=`, the choice commits, and the end of input then fails. A regular expression backtracks into the other alternative, so it accepts the same text.
- The wrong block follows the rule `expr = number [ "-" expr ]` literally. The recursion is on the right, so the parser groups `10 - 3 - 2` as `10 - (3 - 2)` and prints 9, not 5. Write `expr = number { "-" number }` and parse the repetition with a loop, as the Rust file does.
