```python wrong id=hold-head
class Stream:
    alive = 0
    peak = 0

    def __init__(self, value, make_rest):
        self.value = value
        self.make_rest = make_rest
        self.rest_cell = None
        Stream.alive += 1
        Stream.peak = max(Stream.peak, Stream.alive)

    def __del__(self):
        Stream.alive -= 1

    def rest(self):
        if self.make_rest is not None:
            self.rest_cell = self.make_rest()
            self.make_rest = None
        return self.rest_cell


def naturals(n=0):
    return Stream(n, lambda: naturals(n + 1))


head = naturals()
cell = head
for _ in range(10_000):
    cell = cell.rest()
print("value", cell.value, "peak cells", Stream.peak)
```

```output for=hold-head
value 10000 peak cells 10001
```

```python run id=drop-head
class Stream:
    alive = 0
    peak = 0

    def __init__(self, value, make_rest):
        self.value = value
        self.make_rest = make_rest
        self.rest_cell = None
        Stream.alive += 1
        Stream.peak = max(Stream.peak, Stream.alive)

    def __del__(self):
        Stream.alive -= 1

    def rest(self):
        if self.make_rest is not None:
            self.rest_cell = self.make_rest()
            self.make_rest = None
        return self.rest_cell


def naturals(n=0):
    return Stream(n, lambda: naturals(n + 1))


def nth(steps):
    cell = naturals()
    for _ in range(steps):
        cell = cell.rest()
    return cell.value


print("value", nth(10_000), "peak cells", Stream.peak)
```

```output for=drop-head
value 10000 peak cells 2
```

- `Stream` is a memoized lazy list: `rest` computes the next cell once and keeps it. The counter shows how many cells are alive at the same time.
- In the first program, the variable `head` refers to the first cell, and each cell refers to the next, so all 10,001 cells stay alive. This is the leak that the Clojure reference calls holding onto the head.
- In the second program, no variable keeps the first cell, so CPython frees each cell as soon as the loop moves past it, and only two cells are alive at any time.
