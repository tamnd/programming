```python run id=sharing
def update(items, count):
    items.append(4)
    items = [0]
    count += 1
    return items, count


numbers = [1, 2, 3]
total = 1
print(update(numbers, total))
print(numbers, total)


def connect(host, /, port=5432, *, timeout):
    return f"{host}:{port} timeout={timeout}"


print(connect("db", timeout=5))
print(connect("db", 6543, timeout=1))
```

```output for=sharing
([0], 2)
[1, 2, 3, 4] 1
db:5432 timeout=5
db:6543 timeout=1
```

- Verdict: native. Python passes arguments by assignment. The parameter becomes a new name for the same object as the argument. A change to the object, such as `append`, is visible to the caller. A new assignment to the parameter only changes the local name.
- Parameters before `/` are positional-only, and parameters after `*` are keyword-only. A default value is evaluated once, when the `def` statement runs.
- `*args` collects extra positional arguments in a tuple, and `**kwargs` collects extra keyword arguments in a dictionary.

```python wrong id=swap
def swap(a, b):
    a, b = b, a


x, y = 1, 2
swap(x, y)
print(x, y)
```

```output for=swap
1 2
```

The function rebinds its own names `a` and `b`. The names `x` and `y` of the caller do not change. Return the values and assign them: `x, y = y, x`.
