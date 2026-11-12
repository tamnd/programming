```python run id=one-place
class Section:
    def __init__(self, name: str, price_cents: int):
        self.name = name
        self.price_cents = price_cents


class Seat:
    def __init__(self, row: str, number: int, section: Section):
        self.row = row
        self.number = number
        self._section = section

    @property
    def price_cents(self) -> int:
        return self._section.price_cents  # the field now lives in Section


stalls = Section("stalls", 4500)
seats = [Seat("A", 1, stalls), Seat("A", 2, stalls), Seat("B", 7, stalls)]
assert [s.price_cents for s in seats] == [4500] * 3
stalls.price_cents = 4900
assert [s.price_cents for s in seats] == [4900] * 3
print("move field: tests pass")
```

```output
move field: tests pass
```
