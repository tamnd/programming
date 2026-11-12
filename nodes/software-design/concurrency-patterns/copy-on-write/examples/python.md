```python run id=copy-on-write
import threading


class CopyOnWriteList:
    def __init__(self, items=()):
        self._items = tuple(items)  # never changed in place
        self._write_lock = threading.Lock()  # writers only

    def add(self, item):
        with self._write_lock:
            self._items = self._items + (item,)  # copy, then publish

    def snapshot(self):
        return self._items  # readers take no lock


listeners = CopyOnWriteList(["listener 0"])
first = listeners.snapshot()
iterations = 0
errors = 0
count_lock = threading.Lock()  # only for the counters of the demo


def reader():
    global iterations, errors
    for _ in range(1000):
        try:
            for name in listeners.snapshot():
                pass  # call each listener
        except RuntimeError:
            with count_lock:
                errors += 1
        with count_lock:
            iterations += 1


def writer():
    for n in range(1, 101):
        listeners.add(f"listener {n}")


threads = [threading.Thread(target=reader) for _ in range(4)] + [threading.Thread(target=writer)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("iterations:", iterations)
print("iterations that failed:", errors)
print("listeners at the end:", len(listeners.snapshot()))
print("length of the first snapshot:", len(first))
```

```output
iterations: 4000
iterations that failed: 0
listeners at the end: 101
length of the first snapshot: 1
```

- Verdict: idiom. `CopyOnWriteList` is a small class: the items are a tuple that never changes, and `add` builds a new tuple under a lock that only writers take.
