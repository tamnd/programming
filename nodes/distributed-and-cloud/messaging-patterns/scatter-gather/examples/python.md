```python run id=best-quote
import time
from concurrent.futures import ThreadPoolExecutor, wait

# Each supplier replies after some time with a price in cents.
suppliers = {"alpha": (0.02, 1250), "beta": (0.08, 1190), "gamma": (1.0, 1100)}


def ask(name):
    delay, price = suppliers[name]
    time.sleep(delay)  # a stand-in for a request on the channel of the supplier
    return price


print(f"asked {len(suppliers)} suppliers, wait at most 400 ms")
pool = ThreadPoolExecutor(max_workers=len(suppliers))
futures = {pool.submit(ask, name): name for name in suppliers}  # scatter
done, _ = wait(futures, timeout=0.4)                              # gather, with a timeout
quotes = {futures[f]: f.result() for f in done}
pool.shutdown(wait=False, cancel_futures=True)

for name in sorted(suppliers):
    if name in quotes:
        print(f"reply from {name}: {quotes[name]}")
    else:
        print(f"no reply from {name} in time")
best = min(quotes, key=quotes.get)
print(f"best: {best} at {quotes[best]}")
```

```output
asked 3 suppliers, wait at most 400 ms
reply from alpha: 1250
reply from beta: 1190
no reply from gamma in time
best: beta at 1190
```

- Verdict: idiom. `ThreadPoolExecutor.submit` scatters the requests, and `concurrent.futures.wait` with a `timeout` gathers the futures that are done in time.
- `shutdown(wait=False, cancel_futures=True)` does not wait for gamma, but its thread still runs to the end before the interpreter exits.
- With `asyncio`, the same form is `asyncio.wait` on the tasks with a `timeout`.
