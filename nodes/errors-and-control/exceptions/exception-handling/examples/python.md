```python run id=ports
class ConfigError(Exception):
    pass


def parse_port(text):
    try:
        port = int(text)
    except ValueError as e:
        raise ConfigError(f"not a number: {text}") from e
    if not 1 <= port <= 65535:
        raise ConfigError(f"out of range: {port}")
    return port


checked = 0
for text in ["8080", "70000", "abc"]:
    try:
        print("port", parse_port(text))
    except ConfigError as e:
        cause = type(e.__cause__).__name__ if e.__cause__ else "none"
        print(f"error: {e} (cause: {cause})")
    finally:
        checked += 1
print("checked", checked)
```

```output
port 8080
error: out of range: 70000 (cause: none)
error: not a number: abc (cause: ValueError)
checked 3
```

- Verdict: native. `raise` raises an exception, and `try` with `except`, `else` and `finally` handles it.
- An `except` clause matches the class of the exception or a base class. A user-defined exception derives from `Exception`, not from `BaseException`, so that `except Exception` does not catch `KeyboardInterrupt` or `SystemExit`.
- `raise ... from e` sets `__cause__`. A `raise` inside an `except` clause without `from` sets `__context__`, so the traceback still shows the first exception.
- Python code often tries the operation and handles the failure, as `int(text)` does here, in place of a check before the operation.
- The `with` statement and context managers release resources on both paths, like `finally`. `except*` handles the parts of an `ExceptionGroup`.
