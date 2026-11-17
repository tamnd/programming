```python run id=duck-protocol
from typing import Protocol


class Speaker(Protocol):
    def speak(self) -> str: ...


# Neither class names Speaker, but both have speak, so both fit.
class Dog:
    def speak(self) -> str:
        return "woof"


class Robot:
    def speak(self) -> str:
        return "beep"


def talk(s: Speaker) -> str:
    return s.speak().upper()


print([talk(x) for x in (Dog(), Robot())])

# Duck typing: the call fails only when the program runs.
try:
    talk(42)  # a static checker reports this line
except AttributeError as e:
    print("AttributeError:", e)
```

```output for=duck-protocol
['WOOF', 'BEEP']
AttributeError: 'int' object has no attribute 'speak'
```

- Verdict: native. `talk` calls `speak` on any object, and the class of the object picks the method. `Speaker` is a `Protocol`, so a class fits it when it has the method, with no base class. Python does not check the type when the program runs, so `talk(42)` fails only at the call. A static checker such as mypy reports that `int` is not a `Speaker` before the run.
