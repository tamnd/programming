```python run id=fold-it-back
from dataclasses import dataclass
from urllib.parse import quote


# Step 2 of the mechanics: the target delegates to the source for a time.
class Delivery:
    def __init__(self, tracking_id: str):
        self.tracking_id = tracking_id

    def tracking_url(self) -> str:
        return "https://track.example/" + quote(self.tracking_id, safe="")


class OrderMidway:
    def __init__(self, order_id: int, tracking_id: str):
        self.order_id = order_id
        self._delivery = Delivery(tracking_id)

    def tracking_url(self) -> str:
        return self._delivery.tracking_url()


# Steps 4 and 5: the field and the method move in, and Delivery is gone.
@dataclass
class Order:
    order_id: int
    tracking_id: str

    def tracking_url(self) -> str:
        return "https://track.example/" + quote(self.tracking_id, safe="")


for t in ["AB12", "x y/z"]:
    assert Order(1, t).tracking_url() == OrderMidway(1, t).tracking_url()
assert Order(1, "x y/z").tracking_url() == "https://track.example/x%20y%2Fz"
print("inline class: tests pass")
```

```output
inline class: tests pass
```
