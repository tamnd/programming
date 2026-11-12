```python run id=proactor
import queue
import threading

files = {"a.txt": "hello", "b.txt": "abc"}
operations = queue.Queue()   # requests to the processor
completions = queue.Queue()  # the completion queue


def processor():
    # the Asynchronous Operation Processor: one worker thread
    while (op := operations.get()) is not None:
        name, handler = op
        completions.put((handler, name, len(files[name])))


def start_read(name, handler):  # the Proactive Initiator
    print("start read", name)
    operations.put((name, handler))


def on_read(name, size):  # the Completion Handler
    print(f"{name} done: {size} bytes")


worker = threading.Thread(target=processor)
worker.start()
start_read("a.txt", on_read)
start_read("b.txt", on_read)
for _ in range(2):  # the Completion Dispatcher in the main thread
    handler, name, size = completions.get()
    handler(name, size)
operations.put(None)
worker.join()
```

```output
start read a.txt
start read b.txt
a.txt done: 5 bytes
b.txt done: 3 bytes
```

- Verdict: stdlib. `queue.Queue` is a thread-safe queue, so it can join the worker thread and the dispatcher. `asyncio` gives the same shape with an event loop.
