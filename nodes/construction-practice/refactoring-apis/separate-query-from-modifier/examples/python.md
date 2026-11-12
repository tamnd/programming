```python run id=query-and-modifier
from dataclasses import dataclass, field


@dataclass
class Customer:
    name: str
    invoices: list[int] = field(default_factory=list)


sent: list[str] = []


def charge_and_total_before(c: Customer) -> int:
    total = sum(c.invoices)
    sent.append(f"bill to {c.name}: {total}")
    return total


def total_outstanding(c: Customer) -> int:
    return sum(c.invoices)


def send_bill(c: Customer) -> None:
    sent.append(f"bill to {c.name}: {total_outstanding(c)}")


ana = Customer("Ana", [40, 2])
assert charge_and_total_before(ana) == 42
assert len(sent) == 1

assert total_outstanding(ana) == 42
assert total_outstanding(ana) == 42
assert len(sent) == 1

total = total_outstanding(ana)
send_bill(ana)
assert total == 42
assert sent == ["bill to Ana: 42", "bill to Ana: 42"]
print("separate query from modifier: tests pass")
```

```output
separate query from modifier: tests pass
```
