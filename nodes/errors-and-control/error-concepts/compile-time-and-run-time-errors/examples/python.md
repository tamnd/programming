```python run id=when-python
items = [10, 20, 30]
for i in [1, 5]:
    try:
        print(f"items[{i}] = {items[i]}")
    except IndexError as e:
        print(f"items[{i}]: run-time error ({type(e).__name__})")
```

```output for=when-python
items[1] = 20
items[5]: run-time error (IndexError)
```

- Verdict: native. Python finds a syntax error when it compiles the file, before any code runs. It finds almost all other errors at run time, such as this index past the end. A separate type checker, such as mypy, checks the types without running the code.
