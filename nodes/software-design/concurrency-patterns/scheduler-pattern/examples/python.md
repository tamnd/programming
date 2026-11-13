```python run id=scheduler-pattern
import threading


class Scheduler:
    """Decides which waiting thread uses the printer next."""

    def __init__(self):
        self._cond = threading.Condition()
        self._busy = False
        self._waiting = []  # entries (priority, arrival, name)
        self._arrivals = 0
        self._chosen = None

    def enter(self, name, priority):
        with self._cond:
            if not self._busy and not self._waiting:
                self._busy = True
                return
            self._arrivals += 1
            entry = (priority, self._arrivals, name)
            self._waiting.append(entry)
            self._cond.notify_all()
            # Sleep until the scheduler chooses this thread.
            self._cond.wait_for(lambda: self._chosen == entry)
            self._chosen = None

    def done(self):
        with self._cond:
            if not self._waiting:
                self._busy = False
                return
            # The policy: the lowest priority number first, then the first to arrive.
            entry = min(self._waiting)
            self._waiting.remove(entry)
            self._chosen = entry  # the printer stays busy and passes to that thread
            self._cond.notify_all()

    def wait_until_waiting(self, n):
        with self._cond:
            self._cond.wait_for(lambda: len(self._waiting) >= n, timeout=5)
            return len(self._waiting)


scheduler = Scheduler()
lock = threading.Lock()
order = []
on_printer = 0
most_on_printer = 0


def use_printer(name):
    global on_printer, most_on_printer
    with lock:
        on_printer += 1
        most_on_printer = max(most_on_printer, on_printer)
        order.append(name)
    with lock:
        on_printer -= 1


def print_job(name, priority):
    scheduler.enter(name, priority)
    use_printer(name)
    scheduler.done()


scheduler.enter("first", 0)  # the first job takes the free printer
use_printer("first")
jobs = [("report", 2), ("invoice", 1), ("photo", 3), ("label", 1)]
threads = []
for n, (name, priority) in enumerate(jobs, start=1):
    t = threading.Thread(target=print_job, args=(name, priority))
    t.start()
    threads.append(t)
    scheduler.wait_until_waiting(n)  # start the jobs in a known order
print("jobs waiting when the first job ends:", scheduler.wait_until_waiting(len(jobs)))
scheduler.done()
for t in threads:
    t.join()

print("print order:", ", ".join(order))
print("jobs on the printer at once, at most:", most_on_printer)
```

```output
jobs waiting when the first job ends: 4
print order: first, invoice, label, report, photo
jobs on the printer at once, at most: 1
```

- Verdict: idiom. A class with a `threading.Condition`; `enter` waits with `wait_for` until `done` chooses its entry with `min`.
