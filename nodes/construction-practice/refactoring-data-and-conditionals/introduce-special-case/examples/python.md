```python run id=one-special-object
from dataclasses import dataclass
from typing import Protocol

DATA = {"c1": ("Ines Moreau", "pro", 10)}


class Billable(Protocol):
    @property
    def name(self) -> str: ...
    @property
    def discount_percent(self) -> int: ...
    @property
    def is_guest(self) -> bool: ...


@dataclass(frozen=True)
class Customer:
    name: str
    plan: str
    discount_percent: int
    is_guest: bool = False


@dataclass(frozen=True)
class GuestCustomer:
    name: str = "guest"
    plan: str = "free"
    discount_percent: int = 0
    is_guest: bool = True


GUEST = GuestCustomer()


def customer(cid: str) -> Billable:
    row = DATA.get(cid)
    return GUEST if row is None else Customer(*row)


def label_before(cid: str) -> str:
    row = DATA.get(cid)
    return "guest" if row is None else row[0]


for cid in ("c1", "c9"):
    assert customer(cid).name == label_before(cid)
assert customer("c9").discount_percent == 0 and customer("c9").is_guest
print("introduce special case: tests pass")
```

```output
introduce special case: tests pass
```
