```python run id=literal-enum-final
from enum import Enum
from typing import Final, Literal, get_args


class Status(Enum):
    OK = 0
    FAIL = 1


Done = Literal[Status.OK]  # a type with one value
Bit = Literal[0, 1]
MAX: Final = 3  # a checker gives MAX the type Literal[3]


def label(s: Status) -> str:
    if s is Status.OK:
        return "ok"
    # Here a checker narrows s to Literal[Status.FAIL].
    return "fail"


print(get_args(Done), get_args(Bit))
print(label(Status.OK), label(Status.FAIL))
print(Literal[True] == Literal[1], MAX)
x: Bit = 2  # a checker rejects this, but Python runs it
print(x)
```

```output for=literal-enum-final
(<Status.OK: 0>,) (0, 1)
ok fail
False 3
2
```

- Verdict: stdlib. `typing.Literal` takes strings, numbers, booleans, `None` and enum members. A checker treats `Status` as the union `Literal[Status.OK, Status.FAIL]`, so after the test for `OK` only `FAIL` remains. `Literal[True]` and `Literal[1]` are different types even though `True == 1`. Python does not check the annotation of `x` when it runs.
