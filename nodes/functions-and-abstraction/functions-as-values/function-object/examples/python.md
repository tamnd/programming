```python run id=function-object-python
class Multiplier:
    def __init__(self, factor):
        self.factor = factor
        self.calls = 0

    def __call__(self, x):
        self.calls += 1
        return x * self.factor


triple = Multiplier(3)
print(triple(5))
print(" ".join(str(y) for y in map(triple, [1, 2, 3])))
print(triple.calls)
```

```output for=function-object-python
15
3 6 9
4
```

- Verdict: native. The method `__call__` makes an object of the class callable. `map` takes the object as it takes a function, and the field `calls` keeps the count between calls.
