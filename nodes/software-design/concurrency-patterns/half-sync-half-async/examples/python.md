```python run id=hsha
import asyncio
import queue
import threading
import time

requests = queue.Queue()  # the queueing layer
gate = threading.Event()  # lets the demo hold the sync layer
results = []
results_lock = threading.Lock()


def worker():
    """Sync layer: blocks on the queue, then runs a blocking call."""
    gate.wait()
    while True:
        request = requests.get()
        if request is None:
            return
        time.sleep(0.001)  # a blocking call, such as a database query
        with results_lock:
            results.append(request)


async def event_source(on_event):
    """Async layer: one event loop thread calls a short handler per event."""
    for event_id in range(100):
        on_event(event_id)
        await asyncio.sleep(0)


received = 0


def on_event(event_id):
    global received
    received += 1
    requests.put_nowait(event_id)  # never blocks the event loop


workers = [threading.Thread(target=worker) for _ in range(4)]
for w in workers:
    w.start()
asyncio.run(event_source(on_event))
print("events received by the async layer:", received)
print("requests in the queueing layer before the sync layer starts:", requests.qsize())
gate.set()
for _ in workers:
    requests.put(None)
for w in workers:
    w.join()
print("requests done by the sync layer:", len(results))
print("sum of request ids:", sum(results))
```

```output
events received by the async layer: 100
requests in the queueing layer before the sync layer starts: 100
requests done by the sync layer: 100
sum of request ids: 4950
```

- Verdict: stdlib. `asyncio` gives the event loop of the async layer, `queue.Queue` is the queueing layer, and `threading` gives the sync layer.
