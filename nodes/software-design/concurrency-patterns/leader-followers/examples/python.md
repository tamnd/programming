```python run id=leader-followers
import queue
import threading
import time

handle_set = queue.Queue()  # stands for a handle set that one thread may wait on
for event_id in range(100):
    handle_set.put(event_id)


class ThreadPool:
    def __init__(self):
        self.synchronizer = threading.Condition()
        self.leader = None
        self.waiting = 0  # threads inside handle_set.get(); must stay at most 1
        self.most_waiting = 0
        self.processed = []

    def join(self):
        me = threading.current_thread()
        while True:
            with self.synchronizer:  # follower role: wait for the turn to lead
                while self.leader is not None:
                    self.synchronizer.wait()
                self.leader = me
                self.waiting += 1
                self.most_waiting = max(self.most_waiting, self.waiting)
            event = handle_set.get()  # leader role: wait for an event
            self.promote_new_leader()
            if event is None:
                return
            time.sleep(0.001)  # processing role: handle the event in this thread
            with self.synchronizer:
                self.processed.append(event)

    def promote_new_leader(self):
        with self.synchronizer:
            self.waiting -= 1
            self.leader = None
            self.synchronizer.notify()


pool = ThreadPool()
threads = [threading.Thread(target=pool.join) for _ in range(4)]
for _ in threads:
    handle_set.put(None)  # one stop event per thread
for t in threads:
    t.start()
for t in threads:
    t.join()
print("events processed:", len(pool.processed))
print("sum of event ids:", sum(pool.processed))
print("most threads waiting on the handle set at once:", pool.most_waiting)
```

```output
events processed: 100
sum of event ids: 4950
most threads waiting on the handle set at once: 1
```

- Verdict: stdlib. `threading.Condition` is the thread pool synchronizer. The leader is the only thread that calls `get` on the queue, which stands for the handle set.
