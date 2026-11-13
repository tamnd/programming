```python run id=pipeline-concurrency
import queue
import threading

ITEMS = 20
WORKERS = 3
DONE = object()  # tells the next stage that no more values come

numbers = queue.Queue(maxsize=1)  # source -> square workers
squares = queue.Queue(maxsize=1)  # square workers -> sink
sink_started = threading.Event()
overlap = []


def source():
    for n in range(1, ITEMS + 1):
        numbers.put(n, timeout=5)  # blocks while the buffer is full
    # Each stage can hold only a few values, so the sink must already be running.
    overlap.append(sink_started.is_set())
    for _ in range(WORKERS):
        numbers.put(DONE, timeout=5)


def square():
    while (n := numbers.get(timeout=5)) is not DONE:
        squares.put(n * n, timeout=5)
    squares.put(DONE, timeout=5)  # this worker is done


stages = [threading.Thread(target=source)]
stages += [threading.Thread(target=square) for _ in range(WORKERS)]  # fan-out
for t in stages:
    t.start()

# The sink reads the outputs of all workers from one queue: fan-in.
received, total, finished = 0, 0, 0
while finished < WORKERS:
    value = squares.get(timeout=5)
    sink_started.set()
    if value is DONE:
        finished += 1
    else:
        received += 1
        total += value
for t in stages:
    t.join()

print("items from the source:", ITEMS)
print("square workers:", WORKERS)
print("squares that reached the sink:", received)
print("sum of squares:", total)
print("the sink got a value before the source sent its last:", overlap[0])
```

```output
items from the source: 20
square workers: 3
squares that reached the sink: 20
sum of squares: 2870
the sink got a value before the source sent its last: True
```

- Verdict: stdlib. `queue.Queue(maxsize=1)` links the stages, and a `DONE` marker per worker ends the stream.
