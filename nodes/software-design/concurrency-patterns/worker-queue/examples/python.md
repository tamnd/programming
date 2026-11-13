```python run id=worker-queue
import threading
from collections import deque

TASKS = 10
WORKERS = 3
FAILS_ON = 4  # the first worker that takes this task fails before it does the work


class WorkQueue:
    """Hands out tasks and keeps each one until a worker acknowledges it."""

    def __init__(self):
        self.cond = threading.Condition()
        self.ready = deque()
        self.unacked = {}  # delivery tag -> task
        self.pending = 0  # tasks put and not yet acknowledged
        self.tag = 0
        self.deliveries = 0
        self.put_back = 0

    def put(self, task):
        with self.cond:
            self.ready.append(task)
            self.pending += 1
            self.cond.notify_all()

    def get(self):
        """Returns (tag, task), or None when every task is acknowledged."""
        with self.cond:
            while not self.ready and self.pending > 0:
                self.cond.wait(timeout=5)
            if not self.ready:
                return None
            self.tag += 1
            self.deliveries += 1
            self.unacked[self.tag] = self.ready.popleft()
            return self.tag, self.unacked[self.tag]

    def ack(self, tag):
        with self.cond:
            del self.unacked[tag]
            self.pending -= 1
            self.cond.notify_all()

    def worker_lost(self, tag):
        """The worker went away without an ack, so the task goes back on the queue."""
        with self.cond:
            self.ready.append(self.unacked.pop(tag))
            self.put_back += 1
            self.cond.notify_all()


work = WorkQueue()
results = {}
results_lock = threading.Lock()
failed = []


def worker():
    while (delivery := work.get()) is not None:
        tag, task = delivery
        with results_lock:
            fail = task == FAILS_ON and not failed
            if fail:
                failed.append(task)
        if fail:
            work.worker_lost(tag)  # this worker stops here
            return
        with results_lock:
            results[task] = task * task  # the work
        work.ack(tag)  # only after the work is done


for task in range(1, TASKS + 1):
    work.put(task)
threads = [threading.Thread(target=worker) for _ in range(WORKERS)]
for t in threads:
    t.start()
for t in threads:
    t.join()

print("tasks put on the queue:", TASKS)
print("workers:", WORKERS)
print("deliveries:", work.deliveries)
print("tasks put back after a worker failed:", work.put_back)
print("tasks still not acknowledged:", work.pending)
print("every task has a result:", sorted(results) == list(range(1, TASKS + 1)))
```

```output
tasks put on the queue: 10
workers: 3
deliveries: 11
tasks put back after a worker failed: 1
tasks still not acknowledged: 0
every task has a result: True
```

- Verdict: idiom. The standard library has no queue with acknowledgments, so the example has a small `WorkQueue` class on `threading.Condition` that keeps each task until its ack.
