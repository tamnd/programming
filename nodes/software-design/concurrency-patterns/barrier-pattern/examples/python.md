```python run id=barrier-pattern
import threading

N = 8
values = list(range(1, N + 1))
distances = [1, 2, 4]  # each step adds the value that is d places to the left
print("input:", values)
trips = 0


def count_trip():
    global trips
    trips += 1  # runs once per trip, before the threads are released


barrier = threading.Barrier(N, action=count_trip, timeout=5)


def worker(i):
    for d in distances:
        # Phase 1: read a value that the last step wrote.
        add = values[i - d] if i >= d else 0
        barrier.wait()  # nobody writes until every worker has read
        # Phase 2: write the new value.
        values[i] += add
        barrier.wait()  # nobody reads until every worker has written


threads = [threading.Thread(target=worker, args=(i,)) for i in range(N)]
for t in threads:
    t.start()
for t in threads:
    t.join()

print("steps:", len(distances))
print("barrier trips:", trips)
print("prefix sums:", values)
print("matches a running total:", values == [sum(range(1, i + 2)) for i in range(N)])
```

```output
input: [1, 2, 3, 4, 5, 6, 7, 8]
steps: 3
barrier trips: 6
prefix sums: [1, 3, 6, 10, 15, 21, 28, 36]
matches a running total: True
```

- Verdict: stdlib. `threading.Barrier(parties, action=...)` with `wait()`; the action runs once per trip.
