```python run id=recoverable-python
def port_or_default(text, fallback):
    # A bad fallback is a bug in the caller: stop.
    assert 1 <= fallback <= 65535, "fallback out of range"
    # Bad text is an expected error: recover.
    try:
        return int(text), ""
    except ValueError:
        return fallback, " (not a number)"


for text in ["8080", "x"]:
    port, note = port_or_default(text, 80)
    print(f"{text} -> {port}{note}")
```

```output for=recoverable-python
8080 -> 8080
x -> 80 (not a number)
```

- Verdict: idiom. Python uses exceptions for both kinds. The code catches `ValueError`, an expected error, and uses an `assert` for a bug. An `AssertionError` is also an exception, and `python -O` removes `assert` statements, so the split is a convention.
