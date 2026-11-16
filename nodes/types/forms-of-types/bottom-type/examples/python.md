```python run id=never-unchecked
import sys
from typing import Never, get_type_hints


def stop(code: int) -> Never:
    sys.exit(code)


# sys.exit raises SystemExit, so the call never returns normally.
try:
    stop(3)
except SystemExit as e:
    print("exit code", e.code)


def bad() -> Never:
    return None  # a type checker reports this line, but Python runs it


print(bad(), get_type_hints(stop)["return"])
```

```output for=never-unchecked
exit code 3
None typing.Never
```

- Verdict: stdlib. `typing.Never` and `typing.NoReturn` are the bottom type for type checkers. `stop` calls `sys.exit`, which raises `SystemExit`, so the call never gives a value. Python does not check the annotation when the program runs, so `bad` returns `None` even though its result type is `Never`, and only a type checker reports the error.
