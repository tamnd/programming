```python run id=sensor
from typing import Callable

Observer = Callable[[float], None]


class Sensor:
    def __init__(self) -> None:
        self._observers: list[Observer] = []

    def subscribe(self, observer: Observer) -> Callable[[], None]:
        self._observers.append(observer)
        return lambda: self._observers.remove(observer)

    def set(self, value: float) -> None:
        # A copy, so an observer can unsubscribe while the loop runs.
        for observer in list(self._observers):
            observer(value)


def display(value: float) -> None:
    print(f"display: {value:.1f}")


def alarm(value: float) -> None:
    if value > 30:
        print(f"alarm: {value:.1f} is above 30")


sensor = Sensor()
remove_display = sensor.subscribe(display)
sensor.subscribe(alarm)
sensor.set(21.5)
sensor.set(30.5)
remove_display()
sensor.set(31.0)
```

```output for=sensor
display: 21.5
display: 30.5
alarm: 30.5 is above 30
alarm: 31.0 is above 30
```

- Verdict: idiom. Python has no observer construct. A list of callables is the usual form, because functions, bound methods and objects with `__call__` are all callables.
- `subscribe` returns a function that removes the observer. The caller keeps it in place of a reference to the observer.
- The loop runs over `list(self._observers)`, a copy, so an observer can remove itself during a notification (see Consequences).
- `weakref.WeakMethod` holds a bound method without keeping its object alive, for subjects that must not keep their observers alive.
