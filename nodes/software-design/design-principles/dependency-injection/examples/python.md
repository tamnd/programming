```python run id=invoices
from dataclasses import dataclass
from typing import Protocol


class Clock(Protocol):
    def today(self) -> int: ...


class Mailer(Protocol):
    def send(self, to: str, text: str) -> None: ...


@dataclass(frozen=True)
class Invoice:
    number: int
    customer: str
    due: int  # day number


class ReminderService:
    """Depends on a clock and a mailer. It does not know which ones."""

    def __init__(self, clock: Clock, mailer: Mailer) -> None:
        self._clock = clock
        self._mailer = mailer

    def remind(self, invoice: Invoice) -> bool:
        late = self._clock.today() - invoice.due
        if late <= 0:
            return False
        self._mailer.send(invoice.customer, f"invoice {invoice.number} is {late} days overdue")
        return True


# Implementations of the two roles.
class FixedClock:
    def __init__(self, day: int) -> None:
        self.day = day

    def today(self) -> int:
        return self.day


class ConsoleMailer:
    def send(self, to: str, text: str) -> None:
        print(f"to {to}: {text}")


class RecordingMailer:
    def __init__(self) -> None:
        self.sent: list[str] = []

    def send(self, to: str, text: str) -> None:
        self.sent.append(f"to {to}: {text}")


def main() -> None:
    # The composition root: the one place that chooses the implementations.
    service = ReminderService(FixedClock(40), ConsoleMailer())
    invoices = [Invoice(17, "ana@example.com", 31), Invoice(18, "bo@example.com", 45)]
    sent = sum(service.remind(i) for i in invoices)
    print(f"reminders sent: {sent}")

    # A test wires the same class to other implementations.
    recorder = RecordingMailer()
    ReminderService(FixedClock(50), recorder).remind(invoices[1])
    print(f"recorded: {recorder.sent[0]}")


main()
```

```output
to ana@example.com: invoice 17 is 9 days overdue
reminders sent: 1
recorded: to bo@example.com: invoice 18 is 5 days overdue
```

- Verdict: idiom. Python has no construct for injection. A constructor that takes its collaborators as parameters is the whole technique.
- `Clock` and `Mailer` are `typing.Protocol` classes, so `FixedClock` and `ConsoleMailer` match them by their methods, with no base class. A type checker verifies the match. At run time, any object with the right methods works.
- `main` is the composition root. It is the only function that names the concrete classes.
- `RecordingMailer` is a test double. The service runs in the test with no mail server and no real date.
