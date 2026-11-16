```python run id=special-methods
class Money:
    def __init__(self, cents):
        self.cents = cents

    def __add__(self, other):
        if not isinstance(other, Money):
            return NotImplemented
        return Money(self.cents + other.cents)

    def __eq__(self, other):
        if not isinstance(other, Money):
            return NotImplemented
        return self.cents == other.cents

    def __hash__(self):
        return hash(self.cents)

    def __lt__(self, other):
        if not isinstance(other, Money):
            return NotImplemented
        return self.cents < other.cents

    def __repr__(self):
        return f"Money({self.cents})"


a, b = Money(150), Money(250)
print(a + b)
print(a + b == Money(400), a < b)
print(sorted([b, a]))
print(sum([a, b], Money(0)))
```

```output for=special-methods
Money(400)
True True
[Money(150), Money(250)]
Money(400)
```

- Verdict: native. Each operator calls a special method: `+` calls `__add__`, `==` calls `__eq__` and `<` calls `__lt__`.
- A method returns `NotImplemented` for an operand that it does not know, so Python can try the other operand and then raise `TypeError`.
- A class that defines `__eq__` must also define `__hash__` to stay usable in sets and as a dictionary key. `sorted` uses `__lt__`, and `sum` uses `__add__` from the start value.
