```python run id=trace
def f(name, value):
    print("evaluate", name)
    return value


print(f("a", 1) + f("b", 2) * f("c", 3))

d = {}
d[f("key", "k")] = f("value", "v")
print(d)
```

```output for=trace
evaluate a
evaluate b
evaluate c
7
evaluate value
evaluate key
{'k': 'v'}
```

- Verdict: native. The language reference states that Python evaluates expressions from left to right. Precedence makes `*` bind tighter than `+`, but `a` is still evaluated first.
- In an assignment, the right-hand side is evaluated before the left-hand side. The value is computed before the key of the target.
- A dictionary display evaluates each key before its value. Since Python 3.8, a dictionary comprehension does the same. Before 3.8, CPython evaluated the value first in a comprehension.

```python wrong id=default-once
def add(item, items=[]):
    items.append(item)
    return items


print(add(1))
print(add(2))
```

```output for=default-once
[1]
[1, 2]
```

A default value is evaluated once, when the `def` statement runs, and not at each call. All the calls share the same list. Use `items=None` and make a new list in the body.
