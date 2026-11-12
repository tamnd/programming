```python run id=keep-together
class Cart:
    def __init__(self):
        self.items: list[int] = []

    def add(self, cents: int) -> None:
        self.items.append(cents)


def checkout_ok(cart: Cart) -> tuple[int, int]:
    count = len(cart.items)  # read before the call that changes the cart
    cart.add(500)            # shipping is added as an item
    total = sum(cart.items)
    return count, total


def checkout_bad_slide(cart: Cart) -> tuple[int, int]:
    cart.add(500)
    count = len(cart.items)  # slid below a statement that changes what it reads
    total = sum(cart.items)
    return count, total


def fresh() -> Cart:
    c = Cart()
    c.add(1200)
    c.add(800)
    return c


assert checkout_ok(fresh()) == (2, 2500)
# The test catches the illegal slide: the count now includes the shipping item.
assert checkout_bad_slide(fresh()) != checkout_ok(fresh())
print("slide statements: tests pass")
```

```output
slide statements: tests pass
```
