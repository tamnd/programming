```python run id=talk-to-the-worker
class Ledger:
    def __init__(self):
        self._entries: list[tuple[int, int]] = []

    def add(self, day: int, cents: int) -> None:
        self._entries.append((day, cents))

    def balance(self) -> int:
        return sum(c for _, c in self._entries)

    def since(self, day: int) -> list[tuple[int, int]]:
        return [e for e in self._entries if e[0] >= day]


class Account:
    """After: the account gives out its ledger, and the forwarding methods are gone."""

    def __init__(self, owner: str):
        self.owner = owner
        self._ledger = Ledger()

    @property
    def ledger(self) -> Ledger:
        return self._ledger


a = Account("Ana")
for day, cents in [(1, 500), (3, -120), (9, 40)]:
    a.ledger.add(day, cents)  # before: a.add(day, cents)
assert a.ledger.balance() == 420  # before: a.balance()
assert a.ledger.since(3) == [(3, -120), (9, 40)]
assert not hasattr(a, "balance")
print("remove middle man: tests pass")
```

```output
remove middle man: tests pass
```
