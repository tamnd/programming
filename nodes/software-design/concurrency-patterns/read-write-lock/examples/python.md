```python run id=read-write-lock
import threading
import time


class ReadWriteLock:
    """Many readers or one writer. A waiting writer stops new readers."""

    def __init__(self):
        self._cond = threading.Condition()
        self._readers = 0  # threads that hold the read lock
        self._writer = False  # True while a thread holds the write lock
        self._waiting_writers = 0

    def acquire_read(self):
        with self._cond:
            while self._writer or self._waiting_writers:
                self._cond.wait()
            self._readers += 1

    def release_read(self):
        with self._cond:
            self._readers -= 1
            if self._readers == 0:
                self._cond.notify_all()

    def acquire_write(self):
        with self._cond:
            self._waiting_writers += 1
            while self._writer or self._readers:
                self._cond.wait()
            self._waiting_writers -= 1
            self._writer = True

    def release_write(self):
        with self._cond:
            self._writer = False
            self._cond.notify_all()


lock = ReadWriteLock()
pair = [0, 0]  # the writer keeps the two values equal
counts = {"reads": 0, "mixed": 0, "writes": 0, "overlap": 0, "inside": 0}
counts_lock = threading.Lock()

# Part 1: two readers hold the read lock at the same time.
meeting = threading.Barrier(2, timeout=5)
met = []


def meet():
    lock.acquire_read()
    try:
        meeting.wait()  # returns only when both readers are inside
        met.append(True)
    except threading.BrokenBarrierError:
        met.append(False)
    finally:
        lock.release_read()


# Part 2: readers and a writer share the pair.
def reader():
    for _ in range(1000):
        lock.acquire_read()
        with counts_lock:
            counts["inside"] += 1
        x = pair[0]
        time.sleep(0)  # give the writer a chance to run here
        y = pair[1]
        with counts_lock:
            counts["inside"] -= 1
            counts["reads"] += 1
            if x != y:
                counts["mixed"] += 1
        lock.release_read()


def writer():
    for i in range(1, 101):
        lock.acquire_write()
        with counts_lock:
            if counts["inside"]:
                counts["overlap"] += 1
        pair[0] = i
        time.sleep(0)
        pair[1] = i
        counts["writes"] += 1
        lock.release_write()


def run_all(targets):
    threads = [threading.Thread(target=t) for t in targets]
    for t in threads:
        t.start()
    for t in threads:
        t.join()


run_all([meet, meet])
run_all([reader, reader, reader, reader, writer])
print("two readers inside at once:", all(met))
print("reads:", counts["reads"])
print("reads that saw a mixed pair:", counts["mixed"])
print("writes:", counts["writes"])
print("writes that ran with a reader inside:", counts["overlap"])
```

```output
two readers inside at once: True
reads: 4000
reads that saw a mixed pair: 0
writes: 100
writes that ran with a reader inside: 0
```

- Verdict: idiom. The `threading` module gives `Condition`, and this `ReadWriteLock` class is built from one `Condition` with a count of readers and a count of waiting writers.
