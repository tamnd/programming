```python run id=types
for value in [3, 3.0, "3", [3], None]:
    print(type(value).__name__, isinstance(value, (int, float)))


def double(n: int) -> int:
    return n * 2


print(double.__annotations__)
print(double("ab"))
```

```output
int True
float True
str False
list False
NoneType False
{'n': <class 'int'>, 'return': <class 'int'>}
abab
```

- Verdict: native. Python is dynamically typed. A name has no type, and each object carries its type for its whole life.
- `type()` gives the class of an object, and `isinstance()` also accepts subclasses. An operation that has no implementation for its operand types raises `TypeError`.
- Annotations are stored in `__annotations__` as plain data. The runtime does not check them, so `double("ab")` returns `"abab"`. A separate tool such as mypy checks them before the run.
- Python does not convert a `str` to a number for `+` or `*`. It repeats or concatenates sequences, or it raises `TypeError`.
