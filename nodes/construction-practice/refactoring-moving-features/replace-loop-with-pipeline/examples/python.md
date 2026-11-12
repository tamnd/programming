```python run id=named-steps
from itertools import islice

log = [
    ("/a", 200, 40), ("/b", 503, 900), ("/c", 500, 120),
    ("/d", 502, 2500), ("/e", 500, 300), ("/f", 404, 10),
]


def slow_failures_loop(log):
    failed = []
    for r in log:
        if r[1] >= 500:
            failed.append(r)
    failed.sort(key=lambda r: r[2], reverse=True)
    return [failed[i][0] for i in range(min(3, len(failed)))]


def slow_failures(log):
    failed = [r for r in log if r[1] >= 500]            # filter
    ranked = sorted(failed, key=lambda r: r[2], reverse=True)
    return [path for path, _, _ in ranked[:3]]          # take 3, map


# A lazy form: a generator expression and islice read only what they need.
def first_failures(log, n):
    return list(islice((path for path, status, _ in log if status >= 500), n))


assert slow_failures(log) == slow_failures_loop(log) == ["/d", "/b", "/e"]
assert first_failures(log, 2) == ["/b", "/c"]
print("replace loop with pipeline: tests pass")
```

```output
replace loop with pipeline: tests pass
```
