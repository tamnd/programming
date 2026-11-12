```python run id=counter
class Counter:
    def __init__(self) -> None:
        self._n = 0

    def increment(self) -> None:  # command: returns nothing
        self._n += 1

    @property
    def value(self) -> int:  # query: changes nothing
        return self._n


c = Counter()
c.increment()
assert c.value == c.value
print(c.value, c.value)
```

```output
1 1
```
