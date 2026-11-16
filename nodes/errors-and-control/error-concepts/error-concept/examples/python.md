```python run id=error-python
for text in ["42", "x42"]:
    try:
        n = int(text)
    except ValueError as e:
        print(f"{text} -> error ({type(e).__name__})")
    else:
        print(f"{text} -> {n}")
```

```output for=error-python
42 -> 42
x42 -> error (ValueError)
```

- Verdict: native. Python reports an error at run time with an exception. `int` raises `ValueError` for a string that is not a number, and the `except` clause handles it. A syntax error is the other kind: the parser finds it before the program runs.
