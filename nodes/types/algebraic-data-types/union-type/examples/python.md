```python run id=either
def describe(value: int | str) -> str:
    match value:
        case int(n):
            return f"number {n}, doubled {n * 2}"
        case str(s):
            return f"text {s}, upper {s.upper()}"


for v in [21, "ok"]:
    print(describe(v))
print("union object:", int | str)
```

```output for=either
number 21, doubled 42
text ok, upper OK
union object: int | str
```

- Verdict: native. `int | str` is a union type (Python 3.10 and later). Each value still carries its own type at run time.
- A class pattern such as `case int(n)` tests the type and binds the value in one step.
- The `|` on types builds a union object that `isinstance` also accepts.
