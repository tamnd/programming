```python run id=reentry
import threading


def try_twice(lock):
    with lock:
        again = lock.acquire(timeout=0.2)
        if again:
            lock.release()
        return again


print("Lock, second acquire:", try_twice(threading.Lock()))
print("RLock, second acquire:", try_twice(threading.RLock()))
```

```output for=reentry
Lock, second acquire: False
RLock, second acquire: True
```

- Verdict: stdlib. The `threading` module gives `Lock`, `RLock`, `Condition`, `Semaphore` and `Barrier`. Any of them can be part of a cycle of waits.
- A `Lock` has no owner. A second `acquire` by the same thread waits like any other `acquire`, so the thread waits for itself. The timeout here ends the wait with `False`.
- An `RLock` knows its owner and counts the acquisitions. The owner can acquire it again, and it must release it as many times.
- `acquire(timeout=...)` and `acquire(blocking=False)` give the try-lock of the back off method. `Thread.join(timeout=...)` returns after the timeout, and `is_alive()` then tells if the thread is still running.
