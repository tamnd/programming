```python run id=final
from typing import Final

MAX_SIZE: Final = 10
LIMITS: Final = [1, 2]

MAX_SIZE = 11
LIMITS.append(3)
print(MAX_SIZE, LIMITS)
```

```output for=final
11 [1, 2, 3]
```

- Verdict: idiom. Python has no constants. A name in capitals is a constant by convention, and `typing.Final` marks it for a type checker such as mypy, which reports the line `MAX_SIZE = 11` as an error. The interpreter runs the line with no error, and a list behind a `Final` name can still change.
