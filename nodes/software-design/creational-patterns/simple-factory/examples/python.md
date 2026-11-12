```python run id=simple-factory
from typing import Protocol


class Notifier(Protocol):
    def send(self, user: str, text: str) -> str: ...


class EmailNotifier:
    def send(self, user: str, text: str) -> str:
        return f"mail to {user}@example.com: {text}"


class SmsNotifier:
    def send(self, user: str, text: str) -> str:
        return f"text to the phone of {user}: {text}"


class PushNotifier:
    def send(self, user: str, text: str) -> str:
        return f"push to the app of {user}: {text}"


def make_notifier(channel: str) -> Notifier:
    """The one place that knows the concrete classes."""
    if channel == "email":
        return EmailNotifier()
    if channel == "sms":
        return SmsNotifier()
    if channel == "push":
        return PushNotifier()
    raise ValueError(f"unknown channel: {channel}")


for channel in ["email", "sms", "push", "fax"]:
    try:
        notifier = make_notifier(channel)
    except ValueError as e:
        print(e)
        continue
    print(f"{type(notifier).__name__}: {notifier.send('ann', 'order shipped')}")
```

```output
EmailNotifier: mail to ann@example.com: order shipped
SmsNotifier: text to the phone of ann: order shipped
PushNotifier: push to the app of ann: order shipped
unknown channel: fax
```

- Verdict: idiom. A module-level function is the simple factory. The caller sees only the `Notifier` protocol.
- A `dict` from the parameter to the class, such as `{"email": EmailNotifier, "sms": SmsNotifier}`, replaces the `if` chain. Classes are callable objects in Python, so the factory calls the class it finds.
