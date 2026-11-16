```python run id=none-results
from typing import get_type_hints

# A method that changes the list in place returns None.
items = [3, 1, 2]
result = items.sort()
print(result, items)
print(sorted([3, 1, 2]))

# None is falsy, but it is not equal to 0 or False.
print(bool(None), None == 0, None == False)

# get gives None for a missing key and for a key whose value is None.
d = {"a": None}
print(d.get("a"), d.get("b"), "a" in d, "b" in d)


def f() -> None:
    pass


print(get_type_hints(f))
```

```output for=none-results
None [1, 2, 3]
[1, 2, 3]
False False False
None None True False
{'return': <class 'NoneType'>}
```

- Verdict: native. `None` is the one value of `NoneType`, and a function without a result returns it. `list.sort` changes the list and returns `None`, so `result` holds `None`, and `sorted` is the function that gives a sorted copy. `None` is falsy but not equal to `0` or `False`. Because `None` also marks a missing value, `d.get("a")` and `d.get("b")` both give `None`, and only `in` tells the two cases apart. In an annotation, `None` stands for `NoneType`.
