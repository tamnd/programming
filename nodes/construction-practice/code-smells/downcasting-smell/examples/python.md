```python run id=give-the-base-the-method
from abc import ABC, abstractmethod


# Before: the client tests the class and reaches into each subclass.
class NoticeBefore:
    pass


class EmailNoticeBefore(NoticeBefore):
    def __init__(self, address):
        self.address = address


class SmsNoticeBefore(NoticeBefore):
    def __init__(self, phone):
        self.phone = phone


class PushNoticeBefore(NoticeBefore):
    def __init__(self, device):
        self.device = device


def log_line_before(notice):
    if isinstance(notice, EmailNoticeBefore):
        return f"email to {notice.address}"
    if isinstance(notice, SmsNoticeBefore):
        return f"sms to {notice.phone}"
    raise TypeError(f"unknown notice {type(notice).__name__}")


# After: an abstract method, so each subclass must say how it is described.
class Notice(ABC):
    @abstractmethod
    def describe(self): ...


class EmailNotice(Notice):
    def __init__(self, address):
        self.address = address

    def describe(self):
        return f"email to {self.address}"


class SmsNotice(Notice):
    def __init__(self, phone):
        self.phone = phone

    def describe(self):
        return f"sms to {self.phone}"


class PushNotice(Notice):
    def __init__(self, device):
        self.device = device

    def describe(self):
        return f"push to {self.device}"


def log_line(notice):
    return notice.describe()


assert log_line(EmailNotice("ada@example.com")) == log_line_before(EmailNoticeBefore("ada@example.com"))
assert log_line(SmsNotice("+15550100")) == "sms to +15550100"
assert log_line(PushNotice("tablet-7")) == "push to tablet-7"
try:
    log_line_before(PushNoticeBefore("tablet-7"))
    raise AssertionError("the chain should not know push notices")
except TypeError:
    pass


# A subclass that forgets the method cannot be made at all.
class FaxNotice(Notice):
    pass


try:
    FaxNotice()
    raise AssertionError("an abstract method was not implemented")
except TypeError:
    pass
print("notices: tests pass")
```

```output
notices: tests pass
```

With `abc.ABC` and `@abstractmethod`, Python refuses to make an instance of a subclass that does not implement `describe`, so the mistake shows when the object is made, not when the log line is written.
