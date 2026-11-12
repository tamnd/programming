```python run id=each-case-its-code
import math
from dataclasses import dataclass


# The class form: each channel carries its own code.
class Message:
    def __init__(self, text: str, urgent: bool) -> None:
        self.text, self.urgent = text, urgent

    def cost(self) -> int:
        raise NotImplementedError

    def delay_seconds(self) -> int:
        return 0


class Email(Message):
    def cost(self) -> int:
        return 0

    def delay_seconds(self) -> int:
        return 60


class Sms(Message):
    def cost(self) -> int:
        return math.ceil(len(self.text) / 160) * 7

    def delay_seconds(self) -> int:
        return 0 if self.urgent else 30


class Push(Message):
    def cost(self) -> int:
        return 1 if self.urgent else 0


# The match form: the cases are plain data, and each operation is one function.
@dataclass(frozen=True)
class EmailMsg:
    text: str


@dataclass(frozen=True)
class SmsMsg:
    text: str
    urgent: bool


@dataclass(frozen=True)
class PushMsg:
    urgent: bool


def cost(m: EmailMsg | SmsMsg | PushMsg) -> int:
    match m:
        case EmailMsg():
            return 0
        case SmsMsg(text=text):
            return math.ceil(len(text) / 160) * 7
        case PushMsg(urgent=urgent):
            return 1 if urgent else 0
    raise TypeError(m)


for urgent in (False, True):
    for text in ("hi", "x" * 161):
        assert Email(text, urgent).cost() == cost(EmailMsg(text))
        assert Sms(text, urgent).cost() == cost(SmsMsg(text, urgent))
        assert Push(text, urgent).cost() == cost(PushMsg(urgent))
assert Sms("x" * 161, False).cost() == 14 and Push("", True).delay_seconds() == 0
print("replace conditional with polymorphism: tests pass")
```

```output
replace conditional with polymorphism: tests pass
```

When no case matches, a `match` statement does nothing, and the function would return `None`. The `raise` at the end makes a missed case fail at once.
