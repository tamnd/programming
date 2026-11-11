```python run id=jobs
import sys


def run_job(job, items):
    try:
        return items[job] * 10
    finally:
        print("cleanup job", job)


items = [1, 2]
for job in [0, 5, 1]:
    try:
        print("job", job, "->", run_job(job, items))
    except Exception as exc:
        print("job", job, "failed:", type(exc).__name__, exc)

try:
    sys.exit(3)
except Exception:
    print("not reached: SystemExit is not an Exception")
except BaseException as exc:
    print("caught", type(exc).__name__, "with code", exc.code)
```

```output
cleanup job 0
job 0 -> 10
cleanup job 5
job 5 failed: IndexError list index out of range
cleanup job 1
job 1 -> 20
caught SystemExit with code 3
```

- Verdict: idiom. Python has no panic. A bug such as an index out of range raises an ordinary exception, here `IndexError`, and a boundary catches it like any other.
- `sys.exit` raises `SystemExit`. It inherits from `BaseException`, not from `Exception`, so a handler for `Exception` does not stop the exit. `finally` blocks still run on the way out.
- `os._exit` and `os.abort` are the hard stops. They end the process without the `finally` blocks.

`os._exit` ends the process with the given status. It runs no cleanup handlers and does not flush the standard I/O buffers, so the `finally` block below never prints.

```python fails expect=exit:3 id=os-exit
import os

try:
    os._exit(3)
finally:
    print("cleanup")
```

A bare `except:` catches every exception, `SystemExit` too. The program below asks to stop, and the handler keeps it alive.

```python wrong id=bare-except
import sys

for attempt in range(3):
    try:
        print("attempt", attempt)
        sys.exit(1)
    except:
        print("swallowed the exit")
print("still running")
```

```output
attempt 0
swallowed the exit
attempt 1
swallowed the exit
attempt 2
swallowed the exit
still running
```
