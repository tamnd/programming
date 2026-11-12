```python run id=split-the-checkout
from dataclasses import dataclass


# Before: one class, two jobs, and fields with prefixes.
class CheckoutBefore:
    def __init__(self):
        self.cart_items = []
        self.cart_coupon = None
        self.ship_name = self.ship_street = self.ship_city = self.ship_country = ""

    def add_item(self, name, cents):
        self.cart_items.append((name, cents))

    def apply_coupon(self, percent):
        self.cart_coupon = percent

    def cart_total(self):
        s = sum(c for _, c in self.cart_items)
        return s if self.cart_coupon is None else s - round(s * self.cart_coupon / 100)

    def set_address(self, name, street, city, country):
        self.ship_name, self.ship_street, self.ship_city, self.ship_country = name, street, city, country

    def shipping_label(self):
        return "\n".join([self.ship_name, self.ship_street, f"{self.ship_city}, {self.ship_country}"])

    def is_domestic(self):
        return self.ship_country == "NL"


# After: each group of fields has a class, and the checkout uses both.
class Cart:
    def __init__(self):
        self._items = []
        self._coupon = None

    def add(self, name, cents):
        self._items.append((name, cents))

    def apply_coupon(self, percent):
        self._coupon = percent

    @property
    def total(self):
        s = sum(c for _, c in self._items)
        return s if self._coupon is None else s - round(s * self._coupon / 100)


@dataclass(frozen=True)
class Address:
    name: str
    street: str
    city: str
    country: str

    @property
    def label(self):
        return "\n".join([self.name, self.street, f"{self.city}, {self.country}"])


@dataclass
class Checkout:
    cart: Cart
    address: Address

    def is_domestic(self):
        return self.address.country == "NL"


before = CheckoutBefore()
for name, cents in [("lamp", 3999), ("bulb", 499)]:
    before.add_item(name, cents)
before.apply_coupon(10)
before.set_address("Ada", "Keizersgracht 1", "Amsterdam", "NL")

cart = Cart()
for name, cents in [("lamp", 3999), ("bulb", 499)]:
    cart.add(name, cents)
cart.apply_coupon(10)
after = Checkout(cart, Address("Ada", "Keizersgracht 1", "Amsterdam", "NL"))

assert after.cart.total == before.cart_total() == 4048
assert after.address.label == before.shipping_label()
assert after.is_domestic() == before.is_domestic()
print("checkout: tests pass")
```

```output
checkout: tests pass
```

- `Address` is a frozen dataclass. Its four fields are set together in the constructor, so there is no state where the street is set and the city is not.
- `Cart` keeps its list behind an underscore and offers `add`, `apply_coupon` and `total`. It has state that changes, so it is a plain class.
