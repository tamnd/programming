```python run id=counting-set
from collections.abc import MutableSet


class CountingSet(MutableSet):
    """A set that counts how many elements callers try to add."""

    def __init__(self, inner=None):
        self._inner = set() if inner is None else inner  # has-a set
        self.added = 0

    def add(self, x):
        self.added += 1
        self._inner.add(x)

    def discard(self, x):
        self._inner.discard(x)

    def __contains__(self, x):
        return x in self._inner

    def __iter__(self):
        return iter(self._inner)

    def __len__(self):
        return len(self._inner)


s = CountingSet()
s.add("a")
s |= {"b", "c"}  # MutableSet builds |= from add, so each element counts once
s.add("a")
print(s.added, len(s), sorted(s))
print("b" in s, s <= {"a", "b", "c", "d"})  # the other set methods come from MutableSet
```

```output for=counting-set
4 3 ['a', 'b', 'c']
True True
```

```python wrong id=subclass-dict
class CountingDict(dict):
    """Counts writes by overriding one method of dict."""

    def __init__(self):
        super().__init__()
        self.writes = 0

    def __setitem__(self, key, value):
        self.writes += 1
        super().__setitem__(key, value)


d = CountingDict()
d["a"] = 1
d.update(b=2, c=3)  # dict.update does not call __setitem__
d.setdefault("e", 5)  # nor does setdefault
print(d.writes, len(d))
```

```output for=subclass-dict
1 4
```

- Verdict: idiom. `CountingSet` holds a `set` in a field and forwards to it, so the class decides which operations it offers. It also inherits from `collections.abc.MutableSet`, an abstract base class built for this use: the class writes five methods, and the base class builds `|=`, `<=` and the other set operations from them, so `|=` calls `add` for each element. Duck typing lets any object with the right methods be the part, with no common base class.
- The wrong block counts writes by overriding `__setitem__` in a subclass of `dict`. The other methods of `dict` that write, such as `update` and `setdefault`, do not call `__setitem__`, so the count misses three of the four writes. `collections.UserDict` wraps a dictionary in a field and routes its writes through `__setitem__`, which makes it the safer base for such a class.
