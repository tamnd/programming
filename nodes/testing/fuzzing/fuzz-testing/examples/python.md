```python run id=seeds
def parse(data: bytes) -> list[tuple[int, bytes]]:
    """Reads records of the form [tag][length][value]."""
    records = []
    i = 0
    while i < len(data):
        if i + 2 > len(data):
            raise ValueError(f"truncated header at offset {i}")
        tag, length = data[i], data[i + 1]
        if i + 2 + length > len(data):
            raise ValueError(f"truncated value at offset {i}")
        records.append((tag, data[i + 2 : i + 2 + length]))
        i += 2 + length
    return records


def encode(records):
    return b"".join(bytes([tag, len(value)]) + value for tag, value in records)


def fuzz_one(data: bytes) -> str:
    """The fuzz target: any exception other than ValueError is a bug."""
    try:
        records = parse(data)
    except ValueError as e:
        return f"rejected: {e}"
    assert encode(records) == data, "accepted input does not encode back to itself"
    return f"records={len(records)}"


# The seed corpus: valid inputs, plus the failing inputs that fuzzing found.
SEEDS = [b"", b"\x01\x02hi", b"\x01\x02hi\x02\x00", b"z", b"~\xe1"]

for seed in SEEDS:
    print(f"{seed.hex(' ') or '(empty)'}: {fuzz_one(seed)}")
print(f"seed corpus: {len(SEEDS)} inputs passed")
```

```output
(empty): records=0
01 02 68 69: records=1
01 02 68 69 02 00: records=2
7a: rejected: truncated header at offset 0
7e e1: rejected: truncated value at offset 0
seed corpus: 5 inputs passed
```

The original parser, run on the same corpus, stops at the input of one byte.

```python fails expect=exception:IndexError id=no-length-check
def parse(data: bytes) -> list[tuple[int, bytes]]:
    records = []
    i = 0
    while i < len(data):
        tag = data[i]
        length = data[i + 1]  # no check that the length byte exists
        records.append((tag, data[i + 2 : i + 2 + length]))
        i += 2 + length
    return records


SEEDS = [b"", b"\x01\x02hi", b"\x01\x02hi\x02\x00", b"z", b"~\xe1"]

for seed in SEEDS:
    print(f"{seed.hex(' ') or '(empty)'}: records={len(parse(seed))}")
```

- Verdict: library. Atheris is a coverage-guided fuzzer for Python, based on libFuzzer. It also fuzzes native extensions of CPython.
- Python checks every index, so the missing check raises `IndexError` and a fuzzer sees it. A slice never raises, so `data[i + 2 : i + 2 + length]` returns a short value with no error. Only the round-trip assertion in `fuzz_one` catches that bug.
- `fuzz_one` lets every exception except `ValueError` escape. `ValueError` is the documented way to reject bad input.
- The corpus holds the two inputs that fuzzing found, `7a` and `7e e1`. They now run on each test run as regression tests.
