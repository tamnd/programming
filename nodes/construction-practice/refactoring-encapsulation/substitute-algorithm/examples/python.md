```python run id=same-answers
import calendar


def is_leap_old(y: int) -> bool:
    # Old body: the rule written out by hand.
    if y % 400 == 0:
        return True
    if y % 100 == 0:
        return False
    return y % 4 == 0


def is_leap(y: int) -> bool:
    # Replacement: the standard library has the rule.
    return calendar.isleap(y)


# The old body is the oracle until the results agree.
diff = [y for y in range(1000, 3001) if is_leap(y) != is_leap_old(y)]
assert diff == [], diff[:5]
assert not is_leap(1900) and is_leap(2000)
print("substitute algorithm: tests pass")
```

```output
substitute algorithm: tests pass
```
