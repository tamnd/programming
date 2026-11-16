```python run id=match-guards-python
def describe(n):
    match n:
        case 0:
            return "zero"
        case k if k < 0:
            return "negative"
        case k if k < 10 and k % 2 == 0:
            return "small even"
        case k if k < 10:
            return "small odd"
        case _:
            return "large"


print(", ".join(describe(n) for n in [-5, 0, 4, 7, 12]))
```

```output for=match-guards-python
negative, zero, small even, small odd, large
```

- Verdict: native. A `case` can have an `if` clause after its pattern. The capture pattern `k` matches each value and binds it, and then the guard decides. If the guard is false, the `match` tries the next case.
