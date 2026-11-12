```python run id=wrappers
class Sensor:
    def __init__(self):
        self.reads = 0

    def read(self):
        self.reads += 1
        return 20

    def unit(self):
        return "C"


class FahrenheitAdapter:
    def __init__(self, sensor):
        self._sensor = sensor

    def fahrenheit(self):
        return self._sensor.read() * 9 // 5 + 32


class Calibrated:
    """A decorator: it overrides read and forwards every other attribute."""

    def __init__(self, sensor, offset):
        self._sensor = sensor
        self._offset = offset

    def read(self):
        return self._sensor.read() + self._offset

    def __getattr__(self, name):  # called only when normal lookup fails
        return getattr(self._sensor, name)


class CachingProxy:
    def __init__(self, sensor):
        self._sensor = sensor
        self._value = None

    def read(self):
        if self._value is None:
            self._value = self._sensor.read()
        return self._value

    def __getattr__(self, name):
        return getattr(self._sensor, name)


print(f"adapter: {FahrenheitAdapter(Sensor()).fahrenheit()} F")
calibrated = Calibrated(Sensor(), 1)
assert calibrated.unit() == "C"  # forwarded by __getattr__
print(f"decorator: {calibrated.read()}")
real = Sensor()
proxy = CachingProxy(real)
print(f"proxy: {proxy.read()} {proxy.read()}, real reads: {real.reads}")
```

```output
adapter: 68 F
decorator: 21
proxy: 20 20, real reads: 1
```

- Verdict: idiom. Duck typing means that a wrapper needs only the methods that clients call, and `__getattr__` forwards the rest. Special methods such as `__len__` must be defined on the wrapper class.
