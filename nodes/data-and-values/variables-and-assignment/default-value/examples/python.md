```python run id=defaults
from collections import defaultdict
from dataclasses import dataclass, field


@dataclass
class Options:
    width: int = 80
    tags: list = field(default_factory=list)


print(Options())
print(Options(width=100))
counts = defaultdict(int)
counts["a"] += 1
print(counts["a"], counts["b"])
```

```output for=defaults
Options(width=80, tags=[])
Options(width=100, tags=[])
1 0
```

- Verdict: idiom. A Python variable has no default value: a name exists only after a binding. Defaults come from the program: a dataclass field with a default, `field(default_factory=list)` for a new list on each object, and `defaultdict(int)`, which calls `int()` to make 0 for a missing key.
