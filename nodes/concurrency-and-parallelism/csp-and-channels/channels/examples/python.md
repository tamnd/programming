```python run id=queue
import queue
import threading

DONE = object()  # the sentinel: no real job is this object


def produce(ident, jobs):
    for i in range(1, 4):
        jobs.put(ident * 10 * i)  # blocks while the queue is full
    jobs.put(DONE)


def consume(jobs, results):
    finished = 0
    while finished < 2:
        item = jobs.get()
        if item is DONE:
            finished += 1
        else:
            results.append(item)
        jobs.task_done()


jobs = queue.Queue(maxsize=2)
results = []
threads = [threading.Thread(target=produce, args=(n, jobs)) for n in (1, 2)]
threads.append(threading.Thread(target=consume, args=(jobs, results)))
for t in threads:
    t.start()
jobs.join()  # waits until task_done was called for every item
for t in threads:
    t.join()
print("total:", sum(results))

try:
    jobs.get(timeout=0.05)
except queue.Empty:
    print("timeout")
```

```output for=queue
total: 180
timeout
```

- Verdict: stdlib. `queue.Queue` is a multi-producer, multi-consumer FIFO queue with its own locks. `maxsize` bounds it; a size of 0 or less means no bound.
- `task_done` and `join` let a producer wait until the consumers have handled every item.
- The usual end signal is a sentinel object. Programs with coroutines use `asyncio.Queue` instead (see [[async-await]]).
