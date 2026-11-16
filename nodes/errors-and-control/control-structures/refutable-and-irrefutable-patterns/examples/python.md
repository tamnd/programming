```python run id=refutable-python
for line in ["width=80", "oops", "height=24"]:
    match line.split("=", 1):
        case [key, value]:
            print(f"{key} -> {value}")
        case _:
            print(f"skip: {line}")
```

```output for=refutable-python
width -> 80
skip: oops
height -> 24
```

- Verdict: native. The sequence pattern `[key, value]` is refutable: it fails for a list of one item. The wildcard `_` is irrefutable, so its case block must be the last one. A plain assignment `key, value = parts` has no test; it raises an exception when the lengths differ.
