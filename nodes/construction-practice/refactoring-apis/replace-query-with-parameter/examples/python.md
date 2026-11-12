```python run id=parameter-not-query
from types import SimpleNamespace

thermostat = SimpleNamespace(current_temperature=19)
plan = SimpleNamespace(target=21, max_step=1)


def target_temperature_before(a_plan) -> float:
    current = thermostat.current_temperature
    return min(a_plan.target, current + a_plan.max_step)


def target_temperature(a_plan, current_temperature: float) -> float:
    return min(a_plan.target, current_temperature + a_plan.max_step)


assert target_temperature(plan, thermostat.current_temperature) == target_temperature_before(plan)
assert target_temperature(plan, 20.5) == 21
assert target_temperature(plan, 15) == 16
print(target_temperature(plan, thermostat.current_temperature))
```

```output
20
```
