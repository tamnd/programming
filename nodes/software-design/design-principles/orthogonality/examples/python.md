```python run id=orthogonal-total
from dataclasses import dataclass


@dataclass
class Line:
    qty: int
    price: int


def invoice_total(lines: list[Line]) -> int:
    return sum(line.qty * line.price for line in lines)


assert invoice_total([Line(3, 5)]) == 15
print(invoice_total([Line(2, 3), Line(1, 4)]))
```

```output
10
```
