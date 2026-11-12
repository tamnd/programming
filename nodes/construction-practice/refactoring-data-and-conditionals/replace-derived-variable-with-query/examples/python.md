```python run id=no-stale-copy
from dataclasses import dataclass, field
from functools import cached_property


@dataclass
class Cart:
    items: list[tuple[str, int]] = field(default_factory=list)

    def add(self, name: str, cents: int) -> None:
        self.items.append((name, cents))

    def remove(self, name: str) -> None:
        self.items = [i for i in self.items if i[0] != name]

    @property
    def total_cents(self) -> int:  # computed, never stored
        return sum(c for _, c in self.items)


cart = Cart()
cart.add("tea", 350)
cart.add("cake", 400)
cart.remove("cake")
assert cart.total_cents == 350


# When the source data cannot change, computing once and keeping it is safe.
@dataclass(frozen=True)
class Receipt:
    lines: tuple[int, ...]

    @cached_property
    def total_cents(self) -> int:
        return sum(self.lines)


r = Receipt((350, 400))
assert r.total_cents == 750 and r.total_cents == 750
print("replace derived variable with query: tests pass")
```

```output
replace derived variable with query: tests pass
```
