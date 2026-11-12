```python run id=fork-join
import threading

THRESHOLD = 10_000  # a range this small is summed directly
leaves = []  # one entry for each range summed directly
leaves_lock = threading.Lock()


def solve(lo, hi):
    """Return the sum of lo, lo + 1, ..., hi - 1."""
    if hi - lo <= THRESHOLD:
        with leaves_lock:
            leaves.append(hi - lo)
        return sum(range(lo, hi))
    mid = (lo + hi) // 2
    left = {}
    # Fork: a new thread solves the left half.
    t = threading.Thread(target=lambda: left.update(result=solve(lo, mid)))
    t.start()
    right = solve(mid, hi)  # this thread solves the right half
    t.join()  # join: wait for the left half
    return left["result"] + right


n = 1_000_000
total = solve(1, n + 1)
print("sum:", total)
print("leaf tasks:", len(leaves))
print("largest leaf, at most 10000:", max(leaves) <= THRESHOLD)
print("matches n(n+1)/2:", total == n * (n + 1) // 2)
```

```output
sum: 500000500000
leaf tasks: 128
largest leaf, at most 10000: True
matches n(n+1)/2: True
```

- Verdict: idiom. `threading.Thread` forks the left half and `join` waits for it.
