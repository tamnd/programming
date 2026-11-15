```python run id=notifier-interface
from typing import Protocol

# ---- package domain: it defines the interface that it needs, and imports nothing below.


class Notifier(Protocol):
    def send(self, customer: str, text: str) -> None: ...


class OrderService:
    def __init__(self, notifier: Notifier):
        self.notifier = notifier

    def confirm(self, number: int, customer: str) -> None:
        self.notifier.send(customer, f"order {number} is confirmed")


# ---- package infrastructure: it imports the domain and implements its interface.


class EmailNotifier:
    def send(self, customer: str, text: str) -> None:
        print(f"email to {customer}: {text}")


# ---- package tests: another implementation of the same interface.


class RecordingNotifier:
    def __init__(self):
        self.sent = []

    def send(self, customer: str, text: str) -> None:
        self.sent.append(f"{customer}: {text}")


# ---- main: the only code that knows both the domain and the implementations.

OrderService(EmailNotifier()).confirm(7, "ann")

recorder = RecordingNotifier()
OrderService(recorder).confirm(8, "bob")
print("the test notifier recorded:", "; ".join(recorder.sent))
```

```output
email to ann: order 7 is confirmed
the test notifier recorded: bob: order 8 is confirmed
```

- Verdict: stdlib. `typing.Protocol` defines the interface in the domain code. A class satisfies it by its methods, with no import of the domain, and a type checker such as mypy checks it. At run time, Python only calls the method.
- In a real program, each `----` section is its own package. Only `main` imports both the domain and the implementations.
