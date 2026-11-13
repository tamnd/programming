```python run id=three-ways
import queue
import threading

THREADS = 4
STEPS = 1000


def run_threads(target):
    threads = [threading.Thread(target=target, args=(i,)) for i in range(THREADS)]
    for t in threads:
        t.start()
    for t in threads:
        t.join()


# 1. Synchronization: a lock guards the shared counter.
count = 0
lock = threading.Lock()


def add_with_lock(_):
    global count
    for _ in range(STEPS):
        with lock:
            count += 1


run_threads(add_with_lock)
print("lock around a shared counter:", count)

# 2. Confinement: one owner thread keeps the counter; the others send it messages.
inbox = queue.Queue()
owned = []


def owner():
    total = 0
    for _ in range(THREADS * STEPS):
        total += inbox.get(timeout=5)
    owned.append(total)


keeper = threading.Thread(target=owner)
keeper.start()
run_threads(lambda _: [inbox.put(1) for _ in range(STEPS)])
keeper.join()
print("one owner thread, the others send messages:", owned[0])

# 3. No sharing: each thread makes its own result, and the results are combined after the join.
parts = [0] * THREADS


def add_own(i):
    for _ in range(STEPS):
        parts[i] += 1  # only thread i writes slot i


run_threads(add_own)
print("own result for each thread, combined after the join:", sum(parts))
```

```output
lock around a shared counter: 4000
one owner thread, the others send messages: 4000
own result for each thread, combined after the join: 4000
```

- Verdict: idiom. `threading.Lock` for the shared counter, `queue.Queue` for the messages to the owner thread, and a list slot for each thread. In CPython the GIL lets only one thread run Python code at a time, but a read and a later write can still lose an update.
