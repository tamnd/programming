```python run id=match-as-switch
def days_in(month):
    match month:
        case 2:
            return 28
        case 4 | 6 | 9 | 11:
            return 30
        case _:
            return 31


def http_text(status):
    match status:
        case 200:
            return "OK"
        case 401 | 403:
            return "not allowed"
        case 404:
            return "not found"
        case _:
            return "other"


print(days_in(2), days_in(4), days_in(7))
print([http_text(s) for s in (200, 403, 404, 500)])
```

```output for=match-as-switch
28 30 31
['OK', 'not allowed', 'not found', 'other']
```

- Verdict: native. Python has no `switch`. A `match` with literal patterns does the same job, `|` joins several values, and `case _:` takes each other value. Only the first case that matches runs. A bare name in a `case` is a capture pattern that matches each value, so use literals or dotted names.
