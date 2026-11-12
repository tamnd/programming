```python run id=around
def with_connection(name, block):
    print("open", name)
    try:
        return block(name)  # the caller's work
    finally:
        print("close", name)  # runs even if block raises


def count_rows(conn):
    rows = ["a", "b", "c"]
    print(f"query: {len(rows)} rows")
    return len(rows)


print("result:", with_connection("db", count_rows))
```

```output for=around
open db
query: 3 rows
close db
result: 3
```

- Verdict: native. A function takes the block as a function value. The `with` statement and `contextlib.contextmanager` give the same form.
