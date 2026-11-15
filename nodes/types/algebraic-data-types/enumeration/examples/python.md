```python run id=light
from enum import Enum


class Light(Enum):
    RED = 0
    AMBER = 1
    GREEN = 2


def after(light: Light) -> Light:
    match light:
        case Light.RED:
            return Light.GREEN
        case Light.GREEN:
            return Light.AMBER
        case Light.AMBER:
            return Light.RED


for light in Light:
    print(light.name.lower(), light.value, "then", after(light).name.lower())
print("by name:", Light["AMBER"].value, "by value:", Light(2).name.lower())
```

```output for=light
red 0 then green
amber 1 then red
green 2 then amber
by name: 1 by value: green
```

- Verdict: native. A subclass of `Enum` lists its members as class attributes. Each member is an object with a `name` and a `value`.
- A loop over the class gives the members in definition order.
- `Light["AMBER"]` looks a member up by name, and `Light(2)` looks it up by value.
