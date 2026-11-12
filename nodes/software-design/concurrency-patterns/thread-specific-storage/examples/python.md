```python run id=tss
import threading
import time

last_error = threading.local()  # one access point, one value per thread


def fake_call(code):
    last_error.code = code  # the call sets its thread's error
    time.sleep(0)  # let another thread run between the set and the test
    return -1


wrong_reads = 0
seen = [None] * 4
count_lock = threading.Lock()


def worker(index):
    global wrong_reads
    code = index + 1
    for _ in range(1000):
        if fake_call(code) == -1 and last_error.code != code:
            with count_lock:
                wrong_reads += 1
    seen[index] = last_error.code


threads = [threading.Thread(target=worker, args=(i,)) for i in range(4)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("threads:", len(threads))
print("calls per thread:", 1000)
print("reads that saw another thread's error:", wrong_reads)
print("error code seen by each thread:", seen)
```

```output
threads: 4
calls per thread: 1000
reads that saw another thread's error: 0
error code seen by each thread: [1, 2, 3, 4]
```

- Verdict: stdlib. `threading.local` is the access point, and each thread sees its own `code` attribute.
