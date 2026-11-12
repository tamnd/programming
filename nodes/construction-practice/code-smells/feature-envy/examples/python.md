```python run id=move-to-the-data
from dataclasses import dataclass


@dataclass
class Account:
    balance: int
    overdraft_limit: int
    frozen: bool = False

    # After: the rule lives with the data it reads.
    def can_withdraw(self, amount):
        return not self.frozen and self.balance + self.overdraft_limit >= amount


# Before: the cash machine module asks the account for its data.
def can_withdraw_before(account, amount):
    if account.frozen:
        return False
    return account.balance + account.overdraft_limit >= amount


cases = [
    (Account(100, 50), 150, True),
    (Account(100, 50), 151, False),
    (Account(100, 0, frozen=True), 10, False),
]
for account, amount, expected in cases:
    assert can_withdraw_before(account, amount) == expected
    assert account.can_withdraw(amount) == expected
print("can_withdraw: tests pass")
```

```output
can_withdraw: tests pass
```

- The `@dataclass` decorator writes the constructor from the field list. The method sits in the class body next to the fields that it reads.
- Python has no private fields. A leading underscore, such as `_overdraft_limit`, tells other modules that the field is not part of the interface, once the rule no longer needs to be outside the class.
