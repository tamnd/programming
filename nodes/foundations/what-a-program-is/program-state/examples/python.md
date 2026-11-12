```python run id=object-state-and-storage
import json
import os
import tempfile


class Account:
    def __init__(self, balance=0):
        self.balance = balance  # the state of the object
        self.history = []

    def withdraw(self, amount):  # the method keeps the state valid
        if amount > self.balance:
            raise ValueError(f"balance {self.balance} is less than {amount}")
        self.balance -= amount
        self.history.append(-amount)


acc = Account(100)
acc.withdraw(30)
try:
    acc.withdraw(100)
except ValueError as err:
    print("refused:", err)
print("state:", vars(acc))

path = os.path.join(tempfile.mkdtemp(), "account.json")
with open(path, "w") as f:  # state that must outlive the process goes to storage
    json.dump(vars(acc), f)
del acc
with open(path) as f:
    restored = Account()
    restored.__dict__.update(json.load(f))
print("restored:", vars(restored))
```

```output for=object-state-and-storage
refused: balance 70 is less than 100
state: {'balance': 70, 'history': [-30]}
restored: {'balance': 70, 'history': [-30]}
```

- Verdict: native. The state of an object is in its attributes, which `vars` shows as a dictionary. Methods such as `withdraw` check a change before they make it, so the state stays valid, but Python does not stop other code from writing `acc.balance` directly; a leading underscore or a property marks the field as internal. The state in memory ends with the process, so the file saves it and a new object loads it.
