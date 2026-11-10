```python run id=thermometer
from typing import Protocol


class Thermometer(Protocol):
    def celsius(self) -> float: ...


def report(name: str, sensor: Thermometer) -> None:
    print(f"{name}: {sensor.celsius():.1f} C")


class RoomSensor:
    def __init__(self, value: float) -> None:
        self.value = value

    def celsius(self) -> float:
        return self.value


# The vendor class. The program cannot change it.
class LegacyThermometer:
    def __init__(self, tenths: int) -> None:
        self.tenths = tenths

    def fahrenheit_tenths(self) -> int:
        return self.tenths


class LegacyAdapter:
    def __init__(self, legacy: LegacyThermometer) -> None:
        self.legacy = legacy

    def celsius(self) -> float:
        return (self.legacy.fahrenheit_tenths() / 10 - 32) * 5 / 9


report("lab", RoomSensor(21.5))
report("patient", LegacyAdapter(LegacyThermometer(986)))
report("hall", LegacyAdapter(LegacyThermometer(704)))
```

```output
lab: 21.5 C
patient: 37.0 C
hall: 21.3 C
```

- Verdict: idiom. The adapter is a class with the method that the client calls. It holds the legacy object and converts its result.
- `typing.Protocol` states the target interface for a type checker. Neither class inherits from it.
- For one method, a `lambda` or a small function can be enough. `contextlib.closing` and `io.TextIOWrapper` are adapters in the standard library.
