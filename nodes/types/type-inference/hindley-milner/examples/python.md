```python run id=rebind
count = 1
print(type(count).__name__)
count = "one"
print(type(count).__name__)

items = []
items.append(3)
print(items, type(items[0]).__name__)


def double(n):
    return n * 2


print(double(4), double("ab"))
```

```output for=rebind
int
str
[3] int
8 abab
```

- Verdict: library. CPython does not infer or check static types. The program runs, and each name holds a value of any type.
- A type checker such as mypy infers the type `int` for `count` from its first value.
- mypy cannot infer the element type of `items = []` and asks for an annotation, such as `items: list[int] = []`.
- A function with no annotations, such as `double`, accepts any argument at run time.
