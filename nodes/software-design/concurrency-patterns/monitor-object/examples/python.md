```python run id=monitor
import threading


class MessageQueue:
    """A monitor object: one lock, two conditions on that lock."""

    def __init__(self, capacity):
        self.capacity = capacity
        self.items = []
        self.lock = threading.Lock()
        self.not_full = threading.Condition(self.lock)
        self.not_empty = threading.Condition(self.lock)
        self.puts = 0
        self.gets = 0
        self.largest = 0

    def put(self, message):
        with self.lock:
            while len(self.items) == self.capacity:
                self.not_full.wait()  # releases the lock while it waits
            self.items.append(message)
            self.puts += 1
            self.largest = max(self.largest, len(self.items))
            self.not_empty.notify()

    def get(self):
        with self.lock:
            while not self.items:
                self.not_empty.wait()
            message = self.items.pop(0)
            self.gets += 1
            self.not_full.notify()
            return message


q = MessageQueue(2)
got = [[], []]


def producer(p):
    for i in range(250):
        q.put(p * 250 + i)


def consumer(mine):
    for _ in range(500):
        mine.append(q.get())


threads = [threading.Thread(target=producer, args=(p,)) for p in range(4)]
threads += [threading.Thread(target=consumer, args=(mine,)) for mine in got]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("messages put:", q.puts)
print("messages got:", q.gets)
print("sum of messages got:", sum(got[0]) + sum(got[1]))
print("queue never held more than 2:", q.largest <= 2)
```

```output
messages put: 1000
messages got: 1000
sum of messages got: 499500
queue never held more than 2: True
```

- Verdict: stdlib. `threading.Condition` takes an existing lock, so the two conditions share the monitor lock.
