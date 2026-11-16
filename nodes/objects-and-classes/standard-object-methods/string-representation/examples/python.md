```python run id=str-and-repr
class Book:
    def __init__(self, title, year):
        self.title, self.year = title, year

    def __repr__(self):
        return f"Book(title={self.title!r}, year={self.year!r})"

    def __str__(self):
        return f"{self.title} ({self.year})"


class Plain:
    pass


b = Book("Dune", 1965)
print(b)
print(repr(b))
print([b])
print(f"{b} / {b!r}")
print(repr(Plain()).startswith("<__main__.Plain object at 0x"))
```

```output for=str-and-repr
Dune (1965)
Book(title='Dune', year=1965)
[Book(title='Dune', year=1965)]
Dune (1965) / Book(title='Dune', year=1965)
True
```

- Verdict: native. Python has two forms: `__str__` for `print` and `str`, and `__repr__` for `repr`, the interactive prompt and the items of a container.
- `{self.title!r}` uses the `repr` of the field, so the string shows its quotes, and the debug form looks like the call that makes the object.
- The default `__repr__` of `object` gives the class name and the address, in angle brackets. A list prints the `repr` of its items, not their `str`.
