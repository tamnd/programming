```python run id=thread-pool
import threading
import time
from concurrent.futures import ThreadPoolExecutor

lock = threading.Lock()
running = 0
most_running = 0
workers = set()


def task(n):
    global running, most_running
    with lock:
        running += 1
        most_running = max(most_running, running)
        workers.add(threading.get_ident())
    time.sleep(0.001)
    with lock:
        running -= 1
    return n * n


with ThreadPoolExecutor(max_workers=4) as pool:
    futures = [pool.submit(task, n) for n in range(100)]
    results = [f.result() for f in futures]

print("tasks run:", len(results))
print("sum of results:", sum(results))
print("workers used, at most 4:", len(workers) <= 4)
print("most tasks running at once, at most 4:", most_running <= 4)
```

```output
tasks run: 100
sum of results: 328350
workers used, at most 4: True
most tasks running at once, at most 4: True
```

- Verdict: stdlib. `concurrent.futures.ThreadPoolExecutor` is the pool, and `submit` returns a `Future` for each task.
