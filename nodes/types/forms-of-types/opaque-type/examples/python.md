```python run id=name-mangling
class Account:
    def __init__(self, balance: int) -> None:
        self.__balance = balance  # stored as _Account__balance

    def deposit(self, amount: int) -> None:
        if amount <= 0:
            raise ValueError("amount must be positive")
        self.__balance += amount

    @property
    def balance(self) -> int:
        return self.__balance


a = Account(10)
a.deposit(5)
print(a.balance)

try:
    print(a.__balance)
except AttributeError as e:
    print("AttributeError:", e)

print(sorted(vars(a)))
a._Account__balance = -100  # the convention does not stop this
print(a.balance)
```

```output for=name-mangling
15
AttributeError: 'Account' object has no attribute '__balance'
['_Account__balance']
-100
```

- Verdict: idiom. Python has no opaque types. A name with two leading underscores is renamed to `_Account__balance`, so `a.__balance` fails outside the class. But the renamed attribute is still there, and any code can change it. A leading underscore is a promise between programmers, not a check.
