```python run id=nest-and-copy
from dataclasses import dataclass

# A grid is a list of lists. * repeats the reference to one inner list.
bad = [[0] * 3] * 3
bad[0][0] = 1
print(bad)
good = [[0] * 3 for _ in range(3)]
good[0][0] = 1
print(good)

# Tuples compare part by part, from the left.
print((1, "a") == (1, "a"), (1, "a") < (1, "b"))


@dataclass(frozen=True)
class Point:
    x: int
    y: int


p = Point(1, 2)
print(p, p == Point(1, 2), hash(p) == hash(Point(1, 2)))
try:
    p.x = 5
except AttributeError as e:
    print(type(e).__name__)
```

```output for=nest-and-copy
[[1, 0, 0], [1, 0, 0], [1, 0, 0]]
[[1, 0, 0], [0, 0, 0], [0, 0, 0]]
True True
Point(x=1, y=2) True True
FrozenInstanceError
```

- Verdict: native. `[[0] * 3] * 3` holds three references to one inner list, so a change to one row shows in all rows. A list comprehension makes a separate list for each row. Tuples compare part by part. A frozen dataclass compares and hashes by its fields and raises `FrozenInstanceError`, a subclass of `AttributeError`, when code assigns to a field.
