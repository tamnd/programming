```python run id=one-copy
class Box[T]:
    def __init__(self, item: T) -> None:
        self.item = item


a = Box[int](1)
b = Box[str]("x")
print(type(a) is type(b), type(a).__name__)

c = Box[int]("not an int")  # a type checker reports this line
print(c.item)
```

```output for=one-copy
True Box
not an int
```

- Verdict: native. `Box[int]` and `Box[str]` are one class when the program runs, and the type argument is not checked. Only a static type checker uses it.
