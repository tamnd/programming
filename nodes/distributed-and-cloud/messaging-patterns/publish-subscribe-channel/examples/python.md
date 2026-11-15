```python run id=three-subscribers
import queue
import threading

STOP = None


class Topic:
    """A publish-subscribe channel: one queue for each subscriber."""

    def __init__(self):
        self._queues = {}

    def subscribe(self, name):
        self._queues[name] = queue.Queue()
        return self._queues[name]

    def publish(self, event):
        for q in self._queues.values():
            q.put(event)  # each subscriber gets its own copy
        return len(self._queues)


def subscriber(inbox, seen):
    while (event := inbox.get()) is not STOP:
        seen.append(event)


topic = Topic()
seen = {name: [] for name in ["billing", "email", "stock"]}
threads = [threading.Thread(target=subscriber, args=(topic.subscribe(name), seen[name])) for name in seen]
for t in threads:
    t.start()

copies = sum(topic.publish(f"order {n} placed") for n in range(1, 4))
topic.publish(STOP)
for t in threads:
    t.join()

for name, events in seen.items():
    print(f"{name}: {', '.join(events)}")
print("copies sent:", copies)
```

```output
billing: order 1 placed, order 2 placed, order 3 placed
email: order 1 placed, order 2 placed, order 3 placed
stock: order 1 placed, order 2 placed, order 3 placed
copies sent: 9
```

- Verdict: stdlib. `Topic` keeps one `queue.Queue` for each subscriber, and `publish` puts the event in each queue. Each subscriber thread reads only its own queue.
- `STOP` is published like an event, so each subscriber gets one copy of it and ends.
- With RabbitMQ and Pika, the same program declares a fanout exchange, and each subscriber binds its own queue to it. The broker makes the copies.
