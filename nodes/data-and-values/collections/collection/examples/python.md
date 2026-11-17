```python run id=three-kinds
from collections import Counter
from collections.abc import Collection

words = ["red", "blue", "red", "green"]
unique = set(words)
counts = Counter(words)

for c in (words, unique, counts):
    print(type(c).__name__, len(c), "red" in c, isinstance(c, Collection))

print(words[1], sorted(unique), counts["red"])
```

```output for=three-kinds
list 4 True True
set 3 True True
Counter 3 True True
blue ['blue', 'green', 'red'] 2
```

- Verdict: native. `list`, `set` and `dict` are built-in types with a literal syntax. The `collections` module adds more, such as `Counter`, `deque` and `defaultdict`.
- A `Counter` is a `dict` subclass, so `len` counts its keys and `in` tests a key.
- Each of the three is a `collections.abc.Collection`: it has `len`, iteration and the `in` test.
