```python fails expect=exception:ValueError id=closed-file
import io

f = io.StringIO("alice\nbob\n")
print(f.readline(), end="")
f.close()
print(f.readline(), end="")
```

```python run id=with
import io

with io.StringIO("alice\nbob\n") as f:
    for line in f:
        print(line, end="")
print("closed:", f.closed)
```

```output for=with
alice
bob
closed: True
```

- Verdict: native. Python frees an object only when no reference to it is left, so Python code cannot reach freed memory. OWASP lists Python among the memory-safe languages.
- The bug moves to resources. The documentation of `close` says that once the file is closed, any operation on it raises `ValueError`. The error is safe and visible.
- A `with` block closes the file at its end. The name `f` still exists after the block, so a later use is possible, but it fails with the error.
- C extensions and `ctypes` manage memory by hand, and a bug in them is a use after free in the interpreter process.
