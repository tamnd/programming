```python run id=enum-and-match
import re
from enum import Enum, auto


class S(Enum):
    START = auto()
    SIGN = auto()
    INT = auto()
    DOT = auto()
    FRAC = auto()
    DEAD = auto()


def step(s, ch):
    digit = ch.isdigit()
    match s:
        case S.START if ch in "+-":
            return S.SIGN
        case S.START | S.SIGN | S.INT if digit:
            return S.INT
        case S.INT if ch == ".":
            return S.DOT
        case S.DOT | S.FRAC if digit:
            return S.FRAC
        case _:
            return S.DEAD


def accepts(text):
    s = S.START
    for ch in text:
        s = step(s, ch)
    return s in (S.INT, S.FRAC)  # accept only in a final state


inputs = ["42", "-3.5", "1.", "+", "007", ".5", "4.2.1"]
for text in inputs:
    print(text, accepts(text))
pattern = re.compile(r"[+-]?[0-9]+(\.[0-9]+)?")
print("regex agrees:", all(bool(pattern.fullmatch(t)) == accepts(t) for t in inputs))
```

```output for=enum-and-match
42 True
-3.5 True
1. False
+ False
007 True
.5 False
4.2.1 False
regex agrees: True
```

```python wrong id=accept-in-any-live-state
def step(s, ch):
    if s == "start" and ch in "+-":
        return "sign"
    if s in ("start", "sign", "int") and ch.isdigit():
        return "int"
    if s == "int" and ch == ".":
        return "dot"
    if s in ("dot", "frac") and ch.isdigit():
        return "frac"
    return "dead"


def accepts(text):
    s = "start"
    for ch in text:
        s = step(s, ch)
    return s != "dead"  # wrong: "sign" and "dot" are not final states


for text in ["42", "1.", "+", ""]:
    print(repr(text), accepts(text))
```

```output for=accept-in-any-live-state
'42' True
'1.' True
'+' True
'' True
```

- Verdict: idiom. Python has no state machine construct, but an `Enum` names the states and a `match` statement with guards gives one case for each transition. The `re` module compiles a regular expression for the same language, and the two agree on every input.
- The wrong block accepts the input when the machine has not died, instead of when it stops in a final state. The input `1.` ends in the state for a dot and `+` in the state for a sign, which are live but not final, and the empty string never leaves the start state, so all three are wrongly accepted.
