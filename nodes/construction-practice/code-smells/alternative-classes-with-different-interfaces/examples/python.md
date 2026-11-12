```python run id=align-the-senders
from typing import Protocol


# Before: two classes for one job, with two interfaces.
class EmailSenderBefore:
    def __init__(self):
        self.sent = []

    def send(self, to, subject, body):
        self.sent.append(f"email to {to}: {subject} | {body}")


class SmsGatewayBefore:
    def __init__(self):
        self.sent = []

    def dispatch(self, message):
        self.sent.append(f"sms to {message['phone']}: {message['text'][:40]}")


def alert_before(channel, user, text):
    if isinstance(channel, EmailSenderBefore):
        channel.send(user["email"], "Alert", text)
    else:
        channel.dispatch({"phone": user["phone"], "text": text})


# After: one interface, deliver(user, text), in both classes.
class Channel(Protocol):
    def deliver(self, user: dict, text: str) -> None: ...


class EmailSender:
    def __init__(self):
        self.sent = []

    def deliver(self, user, text):
        self.sent.append(f"email to {user['email']}: Alert | {text}")


class SmsGateway:
    def __init__(self):
        self.sent = []

    def deliver(self, user, text):
        self.sent.append(f"sms to {user['phone']}: {text[:40]}")


def alert(channel: Channel, user, text):
    channel.deliver(user, text)


ana = {"email": "ana@example.org", "phone": "+31600000000"}
msg = "Your backup finished with 2 warnings."
for old, now in [(EmailSenderBefore(), EmailSender()), (SmsGatewayBefore(), SmsGateway())]:
    alert_before(old, ana, msg)
    alert(now, ana, msg)
    assert now.sent == old.sent
print("deliver: tests pass")
```

```output
deliver: tests pass
```

- `Channel` is a `typing.Protocol`. Neither class names it, but a type checker such as mypy accepts both as a `Channel`, because both have a matching `deliver` method. At run time Python does not check it.
- `isinstance` in `alert_before` is the sign of the smell: the client must know the concrete class to call it.
