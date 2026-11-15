```python run id=poll-orders
import queue
import threading
import time

orders = queue.Queue()


def poll(max_messages, timeout):
    """Wait up to timeout seconds for one message, then take up to max_messages."""
    try:
        batch = [orders.get(timeout=timeout) if timeout else orders.get_nowait()]
    except queue.Empty:
        return []
    while len(batch) < max_messages:
        try:
            batch.append(orders.get_nowait())
        except queue.Empty:
            break
    return batch


print("poll 0, no wait:", poll(2, 0) or "empty")
for n in range(1, 6):
    orders.put(f"order-{n}")
print("producer: 5 orders on the queue")

late = threading.Timer(0.05, lambda: orders.put("order-6"))
n = 0
while True:
    n += 1
    if n == 4:
        late.start()  # a producer sends one more order while the consumer waits
    batch = poll(2, 0.3)
    if not batch:
        print(f"poll {n}: nothing in 300 ms, the consumer stops")
        break
    print(f"poll {n}:", " ".join(batch), f"({orders.qsize()} left)")
    # the consumer works on the batch here, at its own pace
```

```output
poll 0, no wait: empty
producer: 5 orders on the queue
poll 1: order-1 order-2 (3 left)
poll 2: order-3 order-4 (1 left)
poll 3: order-5 (0 left)
poll 4: order-6 (0 left)
poll 5: nothing in 300 ms, the consumer stops
```

- Verdict: idiom. `queue.Queue.get(timeout=...)` is a blocking receive with a limit, and `get_nowait` is the receive with no wait; both raise `queue.Empty` when there is no message.
- The first `get` waits; the next ones do not, so a batch takes only the messages that are already there.
- `threading.Timer` plays a producer that sends while the consumer waits in poll 4.
