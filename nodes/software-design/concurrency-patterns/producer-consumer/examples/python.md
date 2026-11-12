```python run id=producer-consumer
import queue
import threading

DONE = object()  # the end-of-stream object
buffer = queue.Queue(maxsize=4)  # put blocks when 4 items wait
consumed = []
lock = threading.Lock()


def producer(start):
    for item in range(start, start + 500):
        buffer.put(item)


def consumer():
    while True:
        item = buffer.get()  # get blocks when the buffer is empty
        if item is DONE:
            return
        with lock:
            consumed.append(item)


producers = [threading.Thread(target=producer, args=(s,)) for s in (0, 500)]
consumers = [threading.Thread(target=consumer) for _ in range(2)]
for t in producers + consumers:
    t.start()
for t in producers:
    t.join()
for _ in consumers:
    buffer.put(DONE)  # one end-of-stream object for each consumer
for t in consumers:
    t.join()

print("items consumed:", len(consumed))
print("sum of items:", sum(consumed))
print("each item consumed once:", sorted(consumed) == list(range(1000)))
```

```output
items consumed: 1000
sum of items: 499500
each item consumed once: True
```

- Verdict: stdlib. `queue.Queue(maxsize=4)` is the bounded buffer: `put` waits when it is full and `get` waits when it is empty.
