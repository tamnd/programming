```python run id=sort-a-copy
# Before: list.sort() changes the caller's list.
def median_before(values):
    values.sort()
    return values[len(values) // 2]


# After: sorted() returns a sorted copy.
def median(values):
    ordered = sorted(values)
    return ordered[len(ordered) // 2]


readings = [30, 10, 20]
assert median_before(readings) == 20
assert readings == [10, 20, 30]  # the first reading is lost
print("median_before: the caller's list was changed")

readings = [30, 10, 20]
assert median(readings) == 20
assert readings == [30, 10, 20]
print("median: tests pass")


# The same smell in a default argument: one list is shared by all calls.
def add_tag_before(tag, tags=[]):
    tags.append(tag)
    return tags


def add_tag(tag, tags=None):
    tags = [] if tags is None else list(tags)
    tags.append(tag)
    return tags


assert add_tag_before("a") == ["a"]
assert add_tag_before("b") == ["a", "b"]  # the list from the first call
assert add_tag("a") == ["a"]
assert add_tag("b") == ["b"]
print("add_tag: tests pass")
```

```output
median_before: the caller's list was changed
median: tests pass
add_tag: tests pass
```

- `list.sort()` returns `None`, so code such as `ordered = values.sort()` fails at once. The Python sorting guide says that this is "to avoid confusion".
- The default value `[]` is made once, when the `def` statement runs, and each call that uses the default gets the same list. The fix uses `None` as the default and makes the list inside the function.
- `list(tags)` also copies a list that the caller passes, so the function does not change the caller's list either.
