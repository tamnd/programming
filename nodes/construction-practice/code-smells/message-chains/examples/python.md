```python run id=hide-the-path
from dataclasses import dataclass


@dataclass
class Address:
    country: str


@dataclass
class Customer:
    address: Address


@dataclass
class Order:
    customer: Customer
    delivery: Address | None = None

    # After: the order answers the question, and knows the path.
    @property
    def ships_to(self):
        return (self.delivery or self.customer.address).country


# Before: each client walks the chain.
def carrier_before(order):
    return "ups" if order.customer.address.country == "US" else "dhl"


# After: each client asks the order.
def carrier(order):
    return "ups" if order.ships_to == "US" else "dhl"


home = Order(Customer(Address("DE")))
assert carrier_before(home) == carrier(home) == "dhl"

gift = Order(Customer(Address("DE")), delivery=Address("US"))
assert carrier_before(gift) == "dhl"  # the chain still reads the customer's address
assert carrier(gift) == "ups"
print("ships_to: tests pass")
```

```output
ships_to: tests pass
```

- `@property` lets clients write `order.ships_to` with no parentheses, so the call reads like a field, but the path is still hidden in one method.
- `Address | None` in the annotation says that an order may have no delivery address. The `or` picks the customer's address in that case.
