```python run id=named-channels
import queue
import threading


class Channels:
    """Named channels. Senders and receivers share only the names."""

    def __init__(self, *names):
        self._channels = {name: queue.Queue() for name in names}

    def send(self, name, message):
        if name not in self._channels:
            raise LookupError(f"no channel named {name}")
        self._channels[name].put(message)

    def receive(self, name, timeout=0.1):
        try:
            return self._channels[name].get(timeout=timeout)
        except queue.Empty:
            return None

    def depth(self, name):
        return self._channels[name].qsize()


def drain(channels, name, who):
    while (message := channels.receive(name)) is not None:
        print(f"{who}: {message}")


channels = Channels("orders", "payments")
channels.send("orders", "order 1: 2 x A-7")
channels.send("orders", "order 2: 1 x B-1")
channels.send("payments", "payment for order 1: 30.00")
print(f"waiting: orders={channels.depth('orders')} payments={channels.depth('payments')}")

for name, who in [("orders", "warehouse"), ("payments", "accounting")]:
    receiver = threading.Thread(target=drain, args=(channels, name, who))
    receiver.start()
    receiver.join()

try:
    channels.send("order", "order 3: 1 x C-2")
except LookupError as e:
    print(f"error: {e}")
```

```output
waiting: orders=2 payments=1
warehouse: order 1: 2 x A-7
warehouse: order 2: 1 x B-1
accounting: payment for order 1: 30.00
error: no channel named order
```

- Verdict: stdlib. The `queue` module gives a thread-safe FIFO queue for each channel, and `Channels` maps the names to the queues.
- `receive` waits 0.1 seconds and then returns `None`, so a receiver can stop when its channel is empty. A broker client, such as Pika for RabbitMQ, gives the same two calls: publish to a name, and consume from a name.
- The queues are in memory. When the process stops, the messages are gone, and a broker keeps them in its own process or on disk.
