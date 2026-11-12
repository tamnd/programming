```python run id=refinements-in-annotated
from dataclasses import dataclass
from typing import Annotated, get_type_hints


@dataclass(frozen=True)
class Pred:  # metadata that states a refinement
    text: str
    test: object


Percent = Annotated[int, Pred("0 <= v <= 100", lambda v: 0 <= v <= 100)]
NonEmpty = Annotated[str, Pred("len(v) > 0", lambda v: len(v) > 0)]


def refined(fn):  # check each refinement when the function is called
    hints = get_type_hints(fn, include_extras=True)

    def wrapper(*args):
        for name, value in zip(fn.__code__.co_varnames, args):
            for meta in getattr(hints.get(name), "__metadata__", ()):
                if not meta.test(value):
                    raise ValueError(f"{name} = {value!r} breaks {meta.text}")
        return fn(*args)
    return wrapper


@refined
def progress(task: NonEmpty, done: Percent) -> str:
    return f"{task}: {'#' * (done // 10):<10} {done}%"


print(Percent.__metadata__[0].text)
print(progress("build", 70))
for args in [("build", 140), ("", 50)]:
    try:
        progress(*args)
    except ValueError as err:
        print("ValueError:", err)
```

```output for=refinements-in-annotated
0 <= v <= 100
build: #######    70%
ValueError: done = 140 breaks 0 <= v <= 100
ValueError: task = '' breaks len(v) > 0
```

- Verdict: idiom. Python has no refinement or dependent types. `typing.Annotated` attaches metadata to a type, and the example stores a predicate there and checks it in a decorator when the function is called. Type checkers such as mypy ignore metadata that they do not know, so the check happens only at run time. Libraries such as Pydantic and annotated-types use the same form for constraints such as `Gt(0)` and `MaxLen(10)`.
