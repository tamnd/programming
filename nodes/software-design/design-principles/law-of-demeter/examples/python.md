```python run id=paying-customer
class Wallet:
    def __init__(self, cash: int) -> None:
        self._cash = cash

    def take(self, amount: int) -> None:
        if amount > self._cash:
            raise ValueError("not enough cash")
        self._cash -= amount

    @property
    def cash(self) -> int:
        return self._cash


class Customer:
    def __init__(self, cash: int) -> None:
        self._wallet = Wallet(cash)

    def pay(self, amount: int) -> None:
        self._wallet.take(amount)

    @property
    def balance(self) -> int:
        return self._wallet.cash


def charge(customer: Customer, amount: int) -> None:
    customer.pay(amount)


c = Customer(20)
charge(c, 7)
print(c.balance)
```

```output
13
```
