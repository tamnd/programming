```python run id=dunder-methods-of-object
class Money:
    def __init__(self, cents, currency):
        self.cents, self.currency = cents, currency

    def __eq__(self, other):
        if not isinstance(other, Money):
            return NotImplemented
        return (self.cents, self.currency) == (other.cents, other.currency)

    def __repr__(self):
        return f"Money({self.cents!r}, {self.currency!r})"

    def __str__(self):
        return f"{self.cents / 100:.2f} {self.currency}"


m = Money(1250, "EUR")
print(m == Money(1250, "EUR"), m != Money(99, "EUR"))
print(str(m), repr(m))
print(Money.__hash__)
try:
    {m}
except TypeError as e:
    print(e)
```

```output for=dunder-methods-of-object
True True
12.50 EUR Money(1250, 'EUR')
None
unhashable type: 'Money'
```

- Verdict: native. Every class inherits `__eq__`, `__ne__`, `__hash__`, `__repr__` and `__str__` from `object`, and a class overrides the ones it needs.
- `__ne__` needs no code: by default it inverts the result of `__eq__`. `print` and `str()` call `__str__`, and the interactive prompt and the debugger call `__repr__`.
- A class that defines `__eq__` and not `__hash__` gets `__hash__ = None`, so its objects cannot go in a set or be dict keys. Add `__hash__` only if the fields that `__eq__` uses do not change.
