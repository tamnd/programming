```python run id=typed-containers
from collections import defaultdict


def group_by_length(words: list[str]) -> dict[int, list[str]]:
    groups: defaultdict[int, list[str]] = defaultdict(list)
    for w in words:
        groups[len(w)].append(w)
    return dict(groups)


print(group_by_length(["ox", "cat", "dog", "emu", "be"]))
print(list[int], dict[str, int])
```

```output for=typed-containers
{2: ['ox', 'be'], 3: ['cat', 'dog', 'emu']}
list[int] dict[str, int]
```

- Verdict: native. `list`, `dict` and `defaultdict` take type arguments in square brackets. `list[int]` is a generic alias type, and a type checker uses it to check the items. Python does not check the items when the program runs.
