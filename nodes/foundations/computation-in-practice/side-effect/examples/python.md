```python run id=sort-and-sorted
scores = [58, 12, 90, 31]
result = scores.sort()  # sorts in place and returns None
print("sort returned", result, "and changed scores to", scores)

names = ["Grace", "Ada", "Alan"]
ordered = sorted(names)  # returns a new list
print("sorted:", ordered, "names unchanged:", names)

frozen = (1, 2, 3)
try:
    frozen.append(4)
except AttributeError as err:
    print("tuple:", err)
```

```output for=sort-and-sorted
sort returned None and changed scores to [12, 31, 58, 90]
sorted: ['Ada', 'Alan', 'Grace'] names unchanged: ['Grace', 'Ada', 'Alan']
tuple: 'tuple' object has no attribute 'append'
```

```python wrong id=shared-default-list
def add_tag(tag, tags=[]):  # the default list is made once, when def runs
    tags.append(tag)
    return tags


print(add_tag("a"))
print(add_tag("b"))  # expected ['b']
```

```output for=shared-default-list
['a']
['a', 'b']
```

- Verdict: native. Any Python function can change globals, its arguments and the outside world, and nothing in the signature says so. The library marks the difference by name and result: `list.sort` changes the list and returns `None`, and `sorted` returns a new list. Tuples, frozensets and frozen dataclasses cannot change, so passing them is safe.
- The wrong block uses a list as a default value. The list is made once, when `def` runs, so each call that uses the default appends to the same list. The usual fix is `tags=None` and `if tags is None: tags = []` in the body.
