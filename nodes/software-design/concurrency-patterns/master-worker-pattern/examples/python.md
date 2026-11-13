```python run id=master-worker-pattern
import threading

LIMIT = 20000
WORKERS = 4


def count_primes(lo, hi):
    """The work of one worker: count the primes in [lo, hi)."""
    count = 0
    for n in range(max(lo, 2), hi):
        d = 2
        while d * d <= n and n % d:
            d += 1
        if d * d > n:
            count += 1
    return count


def master(limit, workers):
    # 1. Split the work into one part for each worker.
    bounds = [limit * i // workers for i in range(workers + 1)]
    parts = list(zip(bounds, bounds[1:]))
    results = [0] * workers  # each worker writes only its own slot

    def worker(i):
        results[i] = count_primes(*parts[i])

    # 2. Start the workers, then wait for all of them.
    threads = [threading.Thread(target=worker, args=(i,)) for i in range(workers)]
    for t in threads:
        t.start()
    for t in threads:
        t.join()
    # 3. Combine the partial results.
    return parts, results, sum(results)


parts, results, total = master(LIMIT, WORKERS)
print("parts:", parts)
print("primes in each part:", results)
print("primes below", LIMIT, "from the master:", total)
print("same as one loop:", total == count_primes(0, LIMIT))
```

```output
parts: [(0, 5000), (5000, 10000), (10000, 15000), (15000, 20000)]
primes in each part: [669, 560, 525, 508]
primes below 20000 from the master: 2262
same as one loop: True
```

- Verdict: stdlib. `threading.Thread` for each worker, a result slot for each part, and `join` before the master combines. Because of the GIL, the threads show the structure, not a speed-up.
