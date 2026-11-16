```python run id=executor-policies
import threading
from concurrent.futures import Executor, Future, ThreadPoolExecutor


class InlineExecutor(Executor):
    """An executor that runs each task at once, in the caller's thread."""

    def submit(self, fn, /, *args, **kwargs):
        future = Future()
        try:
            future.set_result(fn(*args, **kwargs))
        except BaseException as error:
            future.set_exception(error)
        return future


def threads_used(executor, count):
    # This code knows only the Executor interface: submit a task, get a Future.
    futures = [executor.submit(threading.get_ident) for _ in range(count)]
    return {future.result() for future in futures}


caller = threading.get_ident()

with InlineExecutor() as executor:
    print("inline: ran on the caller's thread only:", threads_used(executor, 4) == {caller})

with ThreadPoolExecutor(max_workers=2) as executor:
    used = threads_used(executor, 4)
    print("pool of 2: at most 2 threads:", len(used) <= 2)
    print("pool of 2: used the caller's thread:", caller in used)
```

```output for=executor-policies
inline: ran on the caller's thread only: True
pool of 2: at most 2 threads: True
pool of 2: used the caller's thread: False
```

- Verdict: stdlib. `concurrent.futures.Executor` is an abstract class with `submit`, `map` and `shutdown`. `ThreadPoolExecutor` and `ProcessPoolExecutor` are its two implementations in the standard library.
- `threads_used` submits tasks and reads their results through `Future` objects. It does not know which executor runs them.
- `InlineExecutor` shows that an executor does not have to use other threads. Its `submit` runs the task at once and returns a future that is already done.
- `asyncio` has its own model: an event loop runs coroutines, and `loop.run_in_executor` hands blocking calls to an executor.
