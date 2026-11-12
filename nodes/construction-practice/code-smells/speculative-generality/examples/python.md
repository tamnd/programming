```python run id=remove-the-hooks
# Before: a registry, a format parameter and options that no caller uses.
_writers = {}


def register_writer(fmt, writer):
    _writers[fmt] = writer


register_writer(
    "csv",
    lambda rows, options: options.get("line_end", "\n").join(
        options.get("delimiter", ",").join(str(v) for v in r) for r in rows
    ),
)


def export_rows_before(rows, fmt="csv", options=None):
    writer = _writers.get(fmt)
    if writer is None:
        raise ValueError(f"no writer for {fmt}")
    return writer(rows, options or {})


# After: the one format that the program uses.
def to_csv(rows):
    return "\n".join(",".join(str(v) for v in r) for r in rows)


rows = [["id", "name"], [1, "Ada"], [2, "Grace"]]
assert export_rows_before(rows) == "id,name\n1,Ada\n2,Grace"
assert to_csv(rows) == export_rows_before(rows, "csv", {})
print("to_csv: tests pass")
```

```output
to_csv: tests pass
```

- `options=None` and `options or {}` avoid a shared mutable default, which is the usual Python form. In the second version the question does not come up, because there are no options.
- For real CSV with quotes and commas inside values, the standard `csv` module is the right tool. The example keeps the values plain to show the shape of the change.
