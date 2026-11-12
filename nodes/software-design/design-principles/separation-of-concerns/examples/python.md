```python run id=three-concerns
from dataclasses import dataclass

TEXT = "apples,3,0.5\npears,2,0.75"


@dataclass
class Item:
    name: str
    qty: int
    price: float


def parse(text: str) -> list[Item]:
    items = []
    for line in text.split("\n"):
        name, qty, price = line.split(",")
        items.append(Item(name, int(qty), float(price)))
    return items


def priced(items: list[Item]) -> list[tuple[str, float]]:
    return [(i.name, i.qty * i.price) for i in items]


def format_report(rows: list[tuple[str, float]]) -> str:
    lines = [f"{name}: {cost:.2f}" for name, cost in rows]
    lines.append(f"total: {sum(cost for _, cost in rows):.2f}")
    return "\n".join(lines)


assert priced([Item("x", 2, 1.5)]) == [("x", 3.0)]
print(format_report(priced(parse(TEXT))))
```

```output
apples: 1.50
pears: 1.50
total: 3.00
```
