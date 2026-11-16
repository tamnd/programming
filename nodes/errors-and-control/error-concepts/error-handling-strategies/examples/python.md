```python run id=strategy-python
def divide(a, b):
    if b == 0:
        raise ValueError("division by zero")
    return a // b


for a, b in [(10, 2), (10, 0)]:
    try:
        print(f"{a} / {b} = {divide(a, b)}")
    except ValueError as e:
        print(f"{a} / {b}: error: {e}")
```

```output for=strategy-python
10 / 2 = 5
10 / 0: error: division by zero
```

- Verdict: native. Strategy: exceptions. Python raises an exception for an error, and the style called EAFP tries an operation and catches the exception. A function that returns `None` for a failure, such as `dict.get`, is the second strategy, for a case that is not an error.
