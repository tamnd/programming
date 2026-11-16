```python run id=stop-a-thread
import threading
import time

stop = threading.Event()
rounds = 0


def worker():
    global rounds
    while not stop.is_set():
        rounds += 1
        # Event.wait sleeps, but it returns at once when the event is set.
        stop.wait(10)
    print("worker: stop requested, cleaning up")


t = threading.Thread(target=worker)
t.start()
time.sleep(0.1)
start = time.perf_counter()
stop.set()
t.join()
print("worker stopped in under 1 s:", time.perf_counter() - start < 1)
print("worker alive after join:", t.is_alive())
```

```output for=stop-a-thread
worker: stop requested, cleaning up
worker stopped in under 1 s: True
worker alive after join: False
```

- Verdict: absent. Python threads "cannot be destroyed, stopped, suspended, resumed, or interrupted". The worker must stop by itself.
- The Python documentation says: "If you want your threads to stop gracefully, make them non-daemonic and use a suitable signalling mechanism such as an Event". The example uses an `Event` as the stop flag.
- `stop.wait(10)` sleeps like `time.sleep(10)`, but it returns at once when the main thread calls `stop.set()`. So the worker stops in well under 1 second.
- A daemon thread does not stop when it is asked to. It is "abruptly stopped at shutdown", and its resources "may not be released properly".
