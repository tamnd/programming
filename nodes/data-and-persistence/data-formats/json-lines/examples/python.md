```python run id=read-write
import json
import os
import tempfile


def read_jsonl(path):
    """Yields the values of a JSON Lines file. A bad line raises ValueError with its line number."""
    with open(path, encoding="utf-8") as f:
        for number, line in enumerate(f, start=1):
            try:
                yield json.loads(line)
            except json.JSONDecodeError as error:
                raise ValueError(f"line {number}: {error.msg}") from None


fd, path = tempfile.mkstemp(suffix=".jsonl")
with os.fdopen(fd, "w", encoding="utf-8", newline="\n") as f:
    for event in [{"level": "info", "n": 1}, {"level": "warn", "n": 2}]:
        f.write(json.dumps(event) + "\n")
with open(path, "a", encoding="utf-8", newline="\n") as f:
    f.write(json.dumps({"level": "info", "n": 3}) + "\n")
    f.write("\n")

try:
    for value in read_jsonl(path):
        print(value)
except ValueError as error:
    print("ValueError:", error)
finally:
    os.remove(path)
```

```output for=read-write
{'level': 'info', 'n': 1}
{'level': 'warn', 'n': 2}
{'level': 'info', 'n': 3}
ValueError: line 4: Expecting value
```

- Verdict: stdlib. `json.dumps` writes each value on one line, and a loop over an open file gives one line at a time, so the memory holds one record. The reader keeps a line number and puts it in the error, because a JSON Lines file can have millions of lines. The file is opened again in append mode for a third record, which is the main reason to use JSON Lines: an append adds a record with no change to the lines before it. The blank line at the end is not a JSON value, so the strict reader reports it.
