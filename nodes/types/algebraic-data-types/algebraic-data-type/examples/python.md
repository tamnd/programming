```python run id=count
from dataclasses import dataclass
from enum import Enum
from itertools import product


class Light(Enum):
    RED = "red"
    AMBER = "amber"
    GREEN = "green"


# Product: a flag AND a light.
@dataclass(frozen=True)
class Pair:
    flag: bool
    light: Light


# Sum: a flag OR a light. Each case is a class.
@dataclass(frozen=True)
class Flag:
    flag: bool


@dataclass(frozen=True)
class Lamp:
    light: Light


type Choice = Flag | Lamp


def yn(b: bool) -> str:
    return "yes" if b else "no"


def show(c: Choice) -> str:
    match c:
        case Flag(flag):
            return f"flag:{yn(flag)}"
        case Lamp(light):
            return f"light:{light.value}"


flags = [False, True]
pairs = [Pair(f, light) for f, light in product(flags, Light)]
choices: list[Choice] = [Flag(f) for f in flags] + [Lamp(light) for light in Light]

print("pairs:", len(pairs))
print(" ".join(f"{yn(p.flag)}/{p.light.value}" for p in pairs))
print("choices:", len(choices))
print(" ".join(show(c) for c in choices))
```

```output for=count
pairs: 6
no/red no/amber no/green yes/red yes/amber yes/green
choices: 5
flag:no flag:yes light:red light:amber light:green
```

- Verdict: idiom. A `dataclass` is a product. A union of classes, `Flag | Lamp`, is a sum, and `match` selects the case with class patterns.
- The union exists only for a type checker. At run time, nothing stops a list of choices from holding some other object, and `show` then returns `None`.
- `itertools.product` builds the 2 × 3 combinations of the product.
