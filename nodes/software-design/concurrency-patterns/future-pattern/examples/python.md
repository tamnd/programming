```python run id=future-pattern
import threading
from concurrent.futures import ThreadPoolExecutor

PRICES = {"apple": 12, "bread": 7, "cheese": 30}
# The lookups wait here with the caller, so the output does not depend on timing.
meeting = threading.Barrier(4, timeout=5)


def lookup(item):
    meeting.wait()  # returns when all 3 lookups and the caller are here
    return PRICES[item]


with ThreadPoolExecutor(max_workers=3) as pool:
    # Each call returns a future at once. The lookup runs on a pool thread.
    futures = [pool.submit(lookup, item) for item in PRICES]
    print("lookups started:", len(futures))
    print("done right after the calls:", any(f.done() for f in futures))
    meeting.wait()  # the caller is free to do other work here
    print("all three lookups ran at once:", not meeting.broken)
    prices = [f.result() for f in futures]  # wait for each result

print("prices:", prices)
print("total:", sum(prices))
```

```output
lookups started: 3
done right after the calls: False
all three lookups ran at once: True
prices: [12, 7, 30]
total: 49
```

- Verdict: stdlib. `Executor.submit` returns a `concurrent.futures.Future` at once; `done()` checks it and `result()` waits for the value.
