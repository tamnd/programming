```python run id=duration
class Duration:
    """A length of time in whole seconds. Create it with a factory method."""

    _zero = None

    def __init__(self, seconds: int):
        self.seconds = seconds

    @classmethod
    def of_seconds(cls, seconds: int) -> "Duration":
        if seconds == 0:
            if cls._zero is None:
                cls._zero = cls(0)
            return cls._zero  # the same instance every time
        return cls(seconds)

    @classmethod
    def of_minutes(cls, minutes: int) -> "Duration":
        return cls.of_seconds(minutes * 60)

    @classmethod
    def parse(cls, text: str) -> "Duration | None":
        units = {"s": 1, "m": 60}
        number, unit = text[:-1], text[-1:]
        if unit not in units or not number.isdigit():
            return None  # a constructor cannot return "no object"
        return cls.of_seconds(int(number) * units[unit])

    def __str__(self) -> str:
        return f"{self.seconds} s"


print("of_minutes(2) =", Duration.of_minutes(2))
print("of_seconds(90) =", Duration.of_seconds(90))
print("zero is cached:", Duration.of_seconds(0) is Duration.of_minutes(0))
print('parse("2m") =', Duration.parse("2m"))
print('parse("2h") =', Duration.parse("2h"))
```

```output
of_minutes(2) = 120 s
of_seconds(90) = 90 s
zero is cached: True
parse("2m") = 120 s
parse("2h") = None
```

- Verdict: idiom. Python has one `__init__` for each class, so a second way to build an object is a `classmethod`. The standard library uses the form in `dict.fromkeys` and `datetime.date.fromtimestamp`.
- A class method gets the class as its first argument, so `cls(...)` builds an instance of the subclass when a subclass calls it. A `staticmethod` that names `Duration` would not.
- Python cannot make `__init__` private. The factory method is the documented way, and the constructor stays public.
