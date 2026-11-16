```python run id=threads-and-the-gil
import sys
import threading
import time

print("switch interval:", sys.getswitchinterval(), "seconds")


def spin(n):
    while n:
        n -= 1


def timed(target, arg, threads):
    workers = [threading.Thread(target=target, args=(arg,)) for _ in range(threads)]
    start = time.perf_counter()
    for w in workers:
        w.start()
    for w in workers:
        w.join()
    return time.perf_counter() - start


# The best of three runs, so that a busy machine changes the result less.
one = min(timed(spin, 1_000_000, 1) for _ in range(3))
four = min(timed(spin, 1_000_000, 4) for _ in range(3))
print("4 CPU-bound threads take at least twice as long as 1:", four >= 2 * one)

# time.sleep releases the GIL, as I/O does, so the four sleeps overlap.
sleeps = timed(time.sleep, 0.2, 4)
print("4 threads that sleep 0.2 s each finish in under 0.4 s:", sleeps < 0.4)

# The GIL does not make "total += 1" one step. Shared data still needs a lock.
total = 0
lock = threading.Lock()


def add(n):
    global total
    for _ in range(n):
        with lock:
            total += 1


workers = [threading.Thread(target=add, args=(100_000,)) for _ in range(4)]
for w in workers:
    w.start()
for w in workers:
    w.join()
print("total:", total)
```

```output for=threads-and-the-gil
switch interval: 0.005 seconds
4 CPU-bound threads take at least twice as long as 1: True
4 threads that sleep 0.2 s each finish in under 0.4 s: True
total: 400000
```

- Verdict: native. CPython, the standard Python, has a GIL, so "only one thread can execute Python code at once".
- `sys.getswitchinterval()` gives the ideal length of the "timeslices" that running Python threads get, one after another. It is 5 milliseconds by default.
- Four threads that each count down from 1,000,000 take about four times as long as one thread, because they take turns. The example checks only that they take at least twice as long, so that a busy machine does not change the output.
- `time.sleep` releases the GIL, as I/O calls do, so four sleeps of 0.2 seconds overlap.
- The GIL does not make `total += 1` one step. The example uses a `Lock` for the shared total.
- The output is for the default build. On a free-threaded build, the four CPU-bound threads can run in parallel, and the second line can print `False`.
