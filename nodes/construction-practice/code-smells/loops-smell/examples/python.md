```python run id=loop-to-pipeline
orders = [
    {"email": "Ana@example.com", "country": "PT", "paid": True},
    {"email": "bo@example.com", "country": "SE", "paid": True},
    {"email": "ana@example.com", "country": "PT", "paid": True},
    {"email": "cy@example.com", "country": "PT", "paid": False},
    {"email": "Dee@example.com", "country": "PT", "paid": True},
]


# Before: one loop body chooses, changes and collects.
def emails_before(orders, country):
    result = []
    for o in orders:
        if o["country"] == country:
            if o["paid"]:
                email = o["email"].lower()
                if email not in result:
                    result.append(email)
    result.sort()
    return result


# After: a set comprehension chooses and changes, and sorted orders the result.
def emails(orders, country):
    return sorted({o["email"].lower() for o in orders if o["country"] == country and o["paid"]})


want = ["ana@example.com", "dee@example.com"]
assert emails_before(orders, "PT") == want
assert emails(orders, "PT") == want
assert emails(orders, "FR") == []
print("emails: tests pass")
```

```output
emails: tests pass
```

- The braces make a set comprehension, so the duplicates go away as the set is built. `sorted` takes any iterable and returns a list.
- The `for` and `if` inside the comprehension are the same filter as in the loop, but there is no `result` variable to follow.
