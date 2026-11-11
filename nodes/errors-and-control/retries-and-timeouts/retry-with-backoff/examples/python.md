```python run id=backoff
import random


class FakeClock:
    def __init__(self):
        self.now = 0  # milliseconds

    def sleep(self, ms):
        self.now += ms


class TransientError(Exception):
    pass


class PermanentError(Exception):
    pass


def retry(op, clock, rng, attempts=5, base=100, cap=1000, deadline=2000):
    for attempt in range(attempts):
        try:
            return op()
        except TransientError as exc:
            if attempt == attempts - 1:
                raise
            delay = rng.randint(0, min(cap, base * 2**attempt))
            if clock.now + delay > deadline:
                raise TimeoutError(f"the deadline of {deadline} ms would pass") from exc
            print(f"  t={clock.now:4} ms: attempt {attempt + 1} failed ({exc}); wait {delay} ms")
            clock.sleep(delay)


def service(failures, error=TransientError("503 Service Unavailable")):
    calls = 0

    def call():
        nonlocal calls
        calls += 1
        if calls <= failures:
            raise error
        return f"200 OK after {calls} calls"

    return call


rng = random.Random(42)
for name, op in [
    ("flaky", service(3)),
    ("bad request", service(1, PermanentError("400 Bad Request"))),
    ("down", service(100)),
]:
    print(name)
    try:
        print("  result:", retry(op, FakeClock(), rng))
    except (PermanentError, TransientError, TimeoutError) as exc:
        print("  gave up:", type(exc).__name__, exc)
```

```output
flaky
  t=   0 ms: attempt 1 failed (503 Service Unavailable); wait 81 ms
  t=  81 ms: attempt 2 failed (503 Service Unavailable); wait 28 ms
  t= 109 ms: attempt 3 failed (503 Service Unavailable); wait 12 ms
  result: 200 OK after 4 calls
bad request
  gave up: PermanentError 400 Bad Request
down
  t=   0 ms: attempt 1 failed (503 Service Unavailable); wait 94 ms
  t=  94 ms: attempt 2 failed (503 Service Unavailable); wait 70 ms
  t= 164 ms: attempt 3 failed (503 Service Unavailable); wait 125 ms
  t= 289 ms: attempt 4 failed (503 Service Unavailable); wait 228 ms
  gave up: TransientError 503 Service Unavailable
```

- Verdict: stdlib. Python has no retry function in the standard library. The loop above uses only `random` for the jitter.
- The delay before attempt n+1 is a random number between 0 and `min(cap, base * 2**n)`. This is the "full jitter" of the AWS article.
- The loop retries only `TransientError`. A `400 Bad Request` will fail again, so the loop raises it at once.
- The fake clock records the time that the loop would wait, and the seeded generator gives the same delays on each run.

Without jitter, clients that fail at the same moment retry at the same moments too. Here 100 clients back off from one failure, and each retry round arrives as one burst.

```python wrong id=no-jitter
from collections import Counter

calls = Counter()
for client in range(100):
    t = 0
    for attempt in range(4):
        calls[t] += 1
        t += 100 * 2**attempt
for t in sorted(calls):
    print(f"t={t:4} ms: {calls[t]} calls")
```

```output
t=   0 ms: 100 calls
t= 100 ms: 100 calls
t= 300 ms: 100 calls
t= 700 ms: 100 calls
```
