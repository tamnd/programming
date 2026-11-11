```python run id=copy
from contextlib import ExitStack


class File:
    def __init__(self, name):
        self.name = name
        print("open", name)

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc, tb):
        print("close", self.name, "after", exc_type.__name__ if exc_type else "success")
        return False


def copy(src, dst, fail):
    with File(src) as a, File(dst) as b:
        if fail:
            raise OSError(f"cannot read {a.name}")
        print("copy", a.name, "to", b.name)


copy("a.txt", "b.txt", False)
try:
    copy("a.txt", "c.txt", True)
except OSError as exc:
    print("caught:", exc)


def label():
    text = "try"
    try:
        return text
    finally:
        text = "finally"
        print("finally ran; text is now", text)


print("label() returned", label())

with ExitStack() as stack:
    for name in ["x", "y", "z"]:
        stack.callback(print, "release", name)
    print("all acquired")
```

```output
open a.txt
open b.txt
copy a.txt to b.txt
close b.txt after success
close a.txt after success
open a.txt
open c.txt
close c.txt after OSError
close a.txt after OSError
caught: cannot read a.txt
finally ran; text is now finally
label() returned try
all acquired
release z
release y
release x
```

- Verdict: native. `try` with `finally` runs cleanup on every exit, and the `with` statement calls `__exit__` of each context manager.
- A `with` statement with two items closes them in reverse order, `b` first and then `a`. The exception reaches `__exit__` and then the caller.
- The `return` in `label` computes its value before `finally` runs. The assignment in `finally` does not change the result.
- `ExitStack` holds a number of cleanup actions that the program does not know in advance, and calls them in reverse order of registration.

A `return` in a `finally` block replaces the `return` of the `try` block. Python 3.14 warns about it with a `SyntaxWarning` when it compiles the code, but the code still runs.

```python wrong id=finally-return
def save(record):
    try:
        if not record:
            raise ValueError("empty record")
        return "saved"
    finally:
        return "done"


print(save({"id": 1}))
print(save({}))
```

```output
done
done
```
