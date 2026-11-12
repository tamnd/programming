```python run id=surprise
# The surprise: the default list is made once and shared.
def add_bad(item, items=[]):
    items.append(item)
    return items


print(add_bad(1), add_bad(2))


# No surprise: a new list for each call.
def add(item, items=None):
    if items is None:
        items = []
    items.append(item)
    return items


print(add(1), add(2))
```

```output
[1, 2] [1, 2]
[1] [2]
```
