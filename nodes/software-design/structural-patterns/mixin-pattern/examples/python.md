```python run id=comparable-mixin
class ComparableMixin:
    """Requires compare(other) -> int. Provides less_than and between."""

    def less_than(self, other):
        return self.compare(other) < 0

    def between(self, lo, hi):
        return lo.compare(self) <= 0 and self.compare(hi) <= 0


class Version(ComparableMixin):
    def __init__(self, major, minor):
        self.major, self.minor = major, minor

    def compare(self, other):
        a, b = (self.major, self.minor), (other.major, other.minor)
        return (a > b) - (a < b)

    def __str__(self):
        return f"{self.major}.{self.minor}"


def yes(flag):
    return "yes" if flag else "no"


v12, v110, v20 = Version(1, 2), Version(1, 10), Version(2, 0)
print(f"{v12} < {v110}: {yes(v12.less_than(v110))}")
print(f"{v110} between {v12} and {v20}: {yes(v110.between(v12, v20))}")
print(f"{v20} between {v12} and {v110}: {yes(v20.between(v12, v110))}")
```

```output
1.2 < 1.10: yes
1.10 between 1.2 and 2.0: yes
2.0 between 1.2 and 1.10: no
```

- Verdict: native. A mixin is a class in the list of bases, and the method resolution order decides which method runs. The standard library names such classes with `MixIn`, as in `socketserver.ThreadingMixIn`.
- For the comparison operators themselves, `functools.total_ordering` fills in the missing ones from `__eq__` and one ordering method.
