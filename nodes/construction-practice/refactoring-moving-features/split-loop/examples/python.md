```python run id=one-job
orders = [(1, 1200), (2, 5400), (3, 800)]


def summary_before(orders):
    revenue, largest = 0, None
    for oid, cents in orders:
        revenue += cents
        if largest is None or cents > largest[1]:
            largest = (oid, cents)
    return revenue, largest[0] if largest else None


# After the split, each loop has one job, and each job is a built-in.
def revenue_of(orders):
    return sum(cents for _, cents in orders)


def largest_id(orders):
    top = max(orders, key=lambda o: o[1], default=None)
    return top[0] if top else None


for case in (orders, []):
    assert (revenue_of(case), largest_id(case)) == summary_before(case)
print("split loop: tests pass")
```

```output
split loop: tests pass
```
