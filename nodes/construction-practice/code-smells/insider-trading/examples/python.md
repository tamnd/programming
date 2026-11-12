```python run id=stop-the-trade
# Before: billing reads and writes the account's internals.
class AccountBefore:
    def __init__(self):
        self._payments = []
        self._balance = 0


def pay_before(account, cents):
    account._payments.append(cents)
    account._balance -= cents


def invoice_before(account, charged):
    account._balance += charged
    return {"charged": charged, "paid": sum(account._payments), "due": account._balance}


# After: the account owns its data and offers three operations.
class Account:
    def __init__(self):
        self._payments = []
        self._charges = []

    def pay(self, cents):
        self._payments.append(cents)

    def charge(self, cents):
        self._charges.append(cents)

    @property
    def paid(self):
        return sum(self._payments)

    @property
    def due(self):
        return sum(self._charges) - self.paid


def invoice(account, charged):
    account.charge(charged)
    return {"charged": charged, "paid": account.paid, "due": account.due}


before = AccountBefore()
after = Account()
pay_before(before, 1500)
after.pay(1500)
assert invoice(after, 4000) == invoice_before(before, 4000)
assert invoice(after, 1000) == {"charged": 1000, "paid": 1500, "due": 3500}
print("invoice: tests pass")
```

```output
invoice: tests pass
```

- Python has no private fields. A leading underscore is a convention that says "internal", and the smell in the first version is code outside the class that ignores it.
- `paid` and `due` are properties, so billing reads them like fields, but the account computes them from its own data and can change how it stores that data.
