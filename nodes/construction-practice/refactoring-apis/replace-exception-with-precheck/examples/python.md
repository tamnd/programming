```python run id=precheck
values = [12.5, 8, 3.25]


def get_value_for_period_eafp(period: int) -> float:
    try:
        return values[period]
    except IndexError:
        return 0


def get_value_for_period(period: int) -> float:
    return 0 if period >= len(values) else values[period]


for p in [0, 2, 3, 10]:
    assert get_value_for_period(p) == get_value_for_period_eafp(p)

# A negative index is valid in Python: values[-1] is the last value.
assert get_value_for_period(-1) == 3.25
print([get_value_for_period(p) for p in range(4)])
```

```output
[12.5, 8, 3.25, 0]
```
