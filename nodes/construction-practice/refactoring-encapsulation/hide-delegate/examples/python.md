```python run id=ask-the-server
from dataclasses import dataclass


@dataclass(frozen=True)
class Warehouse:
    code: str
    time_zone: str


class Shipment:
    def __init__(self, shipment_id: int, warehouse: Warehouse):
        self.shipment_id = shipment_id
        self._warehouse = warehouse  # the delegate, not part of the interface

    @property
    def time_zone(self) -> str:
        return self._warehouse.time_zone


s = Shipment(7, Warehouse("AMS1", "Europe/Amsterdam"))
# Before, a client wrote: s.warehouse.time_zone
assert s.time_zone == "Europe/Amsterdam"
assert not hasattr(s, "warehouse")
print("hide delegate: tests pass")
```

```output
hide delegate: tests pass
```
