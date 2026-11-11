```python run id=account
class Account:
    def __init__(self, balance):
        self.balance = balance
        self._invariant()

    def _invariant(self):
        assert self.balance >= 0, f"invariant: balance is {self.balance}"

    def withdraw(self, amount):
        assert 0 < amount <= self.balance, f"precondition: withdraw {amount} from {self.balance}"
        old = self.balance
        self.balance -= amount
        assert self.balance == old - amount, "postcondition"
        self._invariant()


account = Account(100)
account.withdraw(30)
print("balance", account.balance)
try:
    account.withdraw(500)
except AssertionError as exc:
    print("AssertionError:", exc)
print("balance", account.balance)
print("__debug__ is", __debug__)
```

```output
balance 70
AssertionError: precondition: withdraw 500 from 70
balance 70
__debug__ is True
```

- Verdict: native. `assert expression, message` raises `AssertionError` with the message when the expression is false.
- The statement is the same as `if __debug__: if not expression: raise AssertionError(message)`. With the option `-O`, `__debug__` is `False` and the compiler emits no code for `assert`.
- Python has no syntax for preconditions, postconditions or invariants. The example writes them as `assert` statements at the start and the end of each method.

With `-O`, the same program has no checks. The withdrawal of 500 goes through, and the invariant is broken without a report.

```python wrong id=optimized flags="-O"
class Account:
    def __init__(self, balance):
        self.balance = balance

    def withdraw(self, amount):
        assert 0 < amount <= self.balance, "precondition"
        self.balance -= amount


account = Account(100)
account.withdraw(500)
print("balance", account.balance)
print("__debug__ is", __debug__)
```

```output
balance -400
__debug__ is False
```

An assertion with parentheses around the condition and the message tests a tuple. A tuple that is not empty is true, so the assertion never fails. The compiler warns about it on standard error.

```python wrong id=tuple
balance = -5
assert (balance >= 0, "balance must not be negative")
print("passed with balance", balance)
```

```output
passed with balance -5
```
