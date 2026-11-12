```python run id=rename-in-place
def calc(l, x):
    n = 0
    for e in l:
        if e["minutes"] > x:
            n += e["minutes"]
    return n / 60


def billable_hours(calls, free_minutes):
    billed_minutes = 0
    for call in calls:
        if call["minutes"] > free_minutes:
            billed_minutes += call["minutes"]
    return billed_minutes / 60


calls = [{"minutes": 5}, {"minutes": 30}, {"minutes": 45}]
for f in (calc, billable_hours):
    assert f(calls, 10) == 1.25
    assert f([], 10) == 0
    print(f"{f.__name__}: tests pass")
```

```output
calc: tests pass
billable_hours: tests pass
```

- The parameter `l` breaks a rule of PEP 8 as well as being mysterious: PEP 8 forbids `l`, `O` and `I` as one-letter names, because some fonts show them like `1` and `0`.
- A rename tool changes the uses of a name that it can resolve. A key such as `"minutes"` is a string, so a rename of the variable does not change it. A change of the key is Rename Field, and the tests must cover each place that reads it.
- Pylint's `disallowed-name` check reports names from a list, by default `foo`, `bar`, `baz`, `toto`, `tutu` and `tata`. It does not report `calc` or `l`.
