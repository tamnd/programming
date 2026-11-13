```python run id=key-and-cmp-to-key
from functools import cmp_to_key

people = [("ann", 31), ("bob", 25), ("cy", 31), ("di", 25)]

# A tuple key sorts by several fields: age first, then name.
print(sorted(people, key=lambda p: (p[1], p[0])))

# reverse=True turns the whole order around.
print(sorted(people, key=lambda p: p[1], reverse=True))

# A minus sign reverses one numeric field and keeps the other ascending.
print(sorted(people, key=lambda p: (-p[1], p[0])))

# A comparison that returns a bool is wrong: False is 0, so "less" never happens.
nums = [3, 1, 2, 5, 4]
print(sorted(nums, key=cmp_to_key(lambda a, b: a > b)))

# The correct three-way form returns -1, 0 or 1.
print(sorted(nums, key=cmp_to_key(lambda a, b: (a > b) - (a < b))))

# Python 3 has no cmp argument.
try:
    sorted(nums, cmp=lambda a, b: a - b)
except TypeError as e:
    print(type(e).__name__)
```

```output
[('bob', 25), ('di', 25), ('ann', 31), ('cy', 31)]
[('ann', 31), ('cy', 31), ('bob', 25), ('di', 25)]
[('ann', 31), ('cy', 31), ('bob', 25), ('di', 25)]
[3, 1, 2, 5, 4]
[1, 2, 3, 4, 5]
TypeError
```

- Verdict: native. A tuple key sorts by several fields, and a minus sign reverses one numeric field. `reverse=True` reverses the whole order and keeps equal items in their old order. A comparison function that returns `a > b` never gives a negative number, so `cmp_to_key` with it leaves the list in a wrong order. The three-way form `(a > b) - (a < b)` sorts the list. `sorted` has no `cmp` argument, so the call raises `TypeError`.
