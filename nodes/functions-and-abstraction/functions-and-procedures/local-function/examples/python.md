```python run id=local-function-python
def sum_of_scaled(values, factor):
    def scale(x):
        return x * factor

    total = 0
    for v in values:
        total += scale(v)
    return total


print(sum_of_scaled([1, 2, 3], 10))
print(sum_of_scaled([4], 2))
```

```output for=local-function-python
60
8
```

- Verdict: native. A `def` in a function body defines a nested function. It reads the parameter `factor` of the outer function.
