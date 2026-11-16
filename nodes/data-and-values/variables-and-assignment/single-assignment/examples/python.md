```python run id=rebind-or-mutate
point = (1, 2)
alias = point
point = (point[0] + 1, point[1])
print(alias, point)

items = [1, 2]
same = items
items.append(3)
print(same)
```

```output for=rebind-or-mutate
(1, 2) (2, 2)
[1, 2, 3]
```

- Verdict: idiom. Python has no single-assignment variables, but a program can keep to them. The assignment to `point` binds the name to a new tuple, and `alias` still has the old tuple. `items.append(3)` changes the list in place, so `same` sees the change. Tuples, strings and frozen dataclasses cannot change, so a new value always means a new binding.
