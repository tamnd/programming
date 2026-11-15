```python run id=competing-workers
import queue
import threading
import time

MAX_DELIVERIES = 3
WORKERS = 3
jobs = queue.Queue()
lock = threading.Lock()
results, events = {}, []
pending = 8
deliveries = 0


def handle(job, attempt):
    time.sleep(0.04)
    if job == 4 and attempt == 1:
        raise RuntimeError("timeout of the database")
    if job == 7:
        raise ValueError("bad payload")
    return job * job


def worker():
    global pending, deliveries
    while True:
        item = jobs.get()
        if item is None:
            return
        job, attempt = item
        with lock:
            deliveries += 1
        try:
            result = handle(job, attempt)
        except Exception as e:
            with lock:
                if attempt < MAX_DELIVERIES:
                    events.append((job, attempt, f"failed on delivery {attempt} ({e}), back on the queue"))
                    jobs.put((job, attempt + 1))
                    continue
                events.append((job, attempt, f"failed on delivery {attempt} ({e}), to the dead letter channel"))
                result = None
        with lock:
            if result is not None:
                results[job] = result
            pending -= 1
            if pending == 0:
                for _ in range(WORKERS):
                    jobs.put(None)


for job in range(1, 9):
    jobs.put((job, 1))
threads = [threading.Thread(target=worker) for _ in range(WORKERS)]
for t in threads:
    t.start()
for t in threads:
    t.join()

print(f"{WORKERS} workers, 8 jobs on one queue")
for job, attempt, text in sorted(events):
    print(f"job {job}: {text}")
print(f"done: {len(results)} jobs, results", " ".join(str(results[j]) for j in sorted(results)))
print(f"deliveries: {deliveries}")
```

```output
3 workers, 8 jobs on one queue
job 4: failed on delivery 1 (timeout of the database), back on the queue
job 7: failed on delivery 1 (bad payload), back on the queue
job 7: failed on delivery 2 (bad payload), back on the queue
job 7: failed on delivery 3 (bad payload), to the dead letter channel
done: 7 jobs, results 1 4 9 16 25 36 64
deliveries: 11
```

- Verdict: idiom. `queue.Queue` is safe for many threads, and three threads that call `get` on the same queue are competing consumers.
- A failed job is put back on the queue with its delivery count plus one; at the limit, it goes to the dead letter channel and counts as finished.
- When the count of open jobs reaches 0, the program puts one `None` for each worker, so that each thread ends.
