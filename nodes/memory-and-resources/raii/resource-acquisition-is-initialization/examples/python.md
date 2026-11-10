```python run id=exit-stack
from contextlib import ExitStack, contextmanager


@contextmanager
def opened(path):
    print("open", path)
    try:
        yield path
    finally:
        print("close", path)


def copy(paths, fail):
    with ExitStack() as stack:
        handles = [stack.enter_context(opened(path)) for path in paths]
        if fail:
            raise OSError("disk full")
        print("copy", " -> ".join(handles))


copy(["source", "target"], fail=False)
try:
    copy(["source", "target"], fail=True)
except OSError as error:
    print("caught:", error)
```

```output
open source
open target
copy source -> target
close target
close source
open source
open target
close target
close source
caught: disk full
```

- Verdict: native. The `with` statement calls `__enter__` at the start of the block and `__exit__` on every exit, also when an exception leaves the block.
- `@contextmanager` turns a generator into a context manager. The code before `yield` acquires, and the `finally` after it releases.
- `ExitStack` holds a list of managers whose number is known only at run time. It closes them in the reverse order of `enter_context`.
- In the second call, the `OSError` passes through the `with` block. Both files close before the `except` clause prints the message.
