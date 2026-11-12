```python run id=car
class Starter:
    def crank(self):
        return "crank the engine"


class NewStarter:
    def crank(self):
        return "crank the engine with the new starter"


class Engine:
    def fire(self):
        return "fire the cylinders"


class Wheels:
    def turn(self):
        return "turn the wheels"


class Car:
    """The whole: it creates its parts and keeps them private."""

    def __init__(self):
        self._starter = Starter()
        self._engine = Engine()
        self._wheels = Wheels()

    def drive(self):
        return ", ".join([self._starter.crank(), self._engine.fire(), self._wheels.turn()])

    def replace_starter(self, starter):
        self._starter = starter


car = Car()
print(f"drive: {car.drive()}")
car.replace_starter(NewStarter())
print(f"drive: {car.drive()}")
```

```output
drive: crank the engine, fire the cylinders, turn the wheels
drive: crank the engine with the new starter, fire the cylinders, turn the wheels
```

- Verdict: idiom. A leading underscore marks the parts as private by convention; the language does not stop access to them.
