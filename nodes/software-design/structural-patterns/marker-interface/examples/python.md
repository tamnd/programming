```python run id=redacting-logger
class Sensitive:
    """Marker: the value must not be written to a log."""


class Username:
    def __init__(self, value: str) -> None:
        self.value = value


class Password(Sensitive):
    def __init__(self, value: str) -> None:
        self.value = value


class ApiKey(Sensitive):
    def __init__(self, value: str) -> None:
        self.value = value


def log(label: str, item: Username | Password | ApiKey) -> None:
    shown = "[redacted]" if isinstance(item, Sensitive) else item.value
    print(f"{label}: {shown}")


log("user", Username("ann"))
log("password", Password("hunter2"))
log("api key", ApiKey("k-123"))
```

```output
user: ann
password: [redacted]
api key: [redacted]
```

- Verdict: idiom. An empty class works as the marker, and `isinstance` tests it at run time.
- A subclass of a marked class is marked too, and it cannot remove the marker.
