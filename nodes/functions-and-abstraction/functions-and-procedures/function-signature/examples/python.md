```python run id=function-signature-python
def average(a: int, b: int) -> float:
    return (a + b) / 2


print(average(3, 4))
```

```output for=function-signature-python
3.5
```

- Verdict: native. The signature has the parameter names and optional annotations. Python does not check the annotations when the program runs.
