```python run id=exhaustive-matching-python
from enum import Enum
from typing import assert_never


class Light(Enum):
    RED = "red"
    YELLOW = "yellow"
    GREEN = "green"


def action(light: Light) -> str:
    match light:
        case Light.RED:
            return "stop"
        case Light.YELLOW:
            return "slow down"
        case Light.GREEN:
            return "go"
        case _ as unreachable:
            assert_never(unreachable)


for light in Light:
    print(f"{light.value}: {action(light)}")
```

```output for=exhaustive-matching-python
red: stop
yellow: slow down
green: go
```

- Verdict: stdlib. The interpreter does not check that a `match` is exhaustive. If no case matches, no case runs. `typing.assert_never` asks a static type checker to confirm that the last case is unreachable, and at run time it throws an exception if the case runs.
