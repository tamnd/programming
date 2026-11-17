```python run id=to-the-interface
from collections.abc import Collection, Iterable, Iterator, Sized


def total(xs: Iterable[int]) -> int:
    return sum(xs)


def describe(c: Collection[int]) -> str:
    return f"size {len(c)}, total {total(c)}"


class Countdown:
    """A collection class: three methods give it the Collection interface."""

    def __init__(self, n: int) -> None:
        self._n = n

    def __len__(self) -> int:
        return self._n

    def __iter__(self) -> Iterator[int]:
        return iter(range(self._n, 0, -1))

    def __contains__(self, x: object) -> bool:
        return isinstance(x, int) and 0 < x <= self._n


print("list:", describe([1, 2, 3]))
print("set:", describe({4, 5}))
print("map values:", describe({"tea": 3, "cake": 4}.values()))
print(isinstance(Countdown(3), Collection), describe(Countdown(3)))
print(isinstance(iter([1]), Sized))
```

```output for=to-the-interface
list: size 3, total 6
set: size 2, total 9
map values: size 2, total 7
True size 3, total 6
False
```

- Verdict: stdlib. `collections.abc` has `Iterable`, `Collection`, `Sequence`, `Set`, `Mapping` and their mutable forms.
- `Countdown` does not inherit from `Collection`, but `isinstance` says yes, because the class has `__len__`, `__iter__` and `__contains__`.
- An iterator is `Iterable` but not `Sized`, so `describe` would not accept it.
