```python run id=caret-and-tilde-ranges
# Turn caret, tilde and exact constraints into a range [low, high), then test versions against it.
def parse(version):
    return tuple(int(part) for part in version.split("."))


def show(version):
    return ".".join(str(part) for part in version)


def to_range(constraint):
    op = constraint[0] if constraint[0] in "^~" else ""
    low = parse(constraint[len(op):])
    major, minor, patch = low
    if op == "^":  # keep the left-most part that is not zero
        if major > 0:
            high = (major + 1, 0, 0)
        elif minor > 0:
            high = (0, minor + 1, 0)
        else:
            high = (0, 0, patch + 1)
    elif op == "~":  # allow patch changes only
        high = (major, minor + 1, 0)
    else:  # an exact version
        high = (major, minor, patch + 1)
    return low, high


CANDIDATES = ["0.0.4", "0.0.5", "0.2.3", "0.2.4", "0.2.9", "0.3.0",
              "1.2.2", "1.2.3", "1.2.9", "1.4.0", "2.0.0"]
print("versions:", " ".join(CANDIDATES))
for constraint in ["^1.2.3", "^0.2.4", "^0.0.4", "~1.2.3", "~0.2.4", "1.2.3"]:
    low, high = to_range(constraint)
    matches = [v for v in CANDIDATES if low <= parse(v) < high]
    print(f"{constraint} means >={show(low)} <{show(high)}, matches {' '.join(matches)}")
```

```output
versions: 0.0.4 0.0.5 0.2.3 0.2.4 0.2.9 0.3.0 1.2.2 1.2.3 1.2.9 1.4.0 2.0.0
^1.2.3 means >=1.2.3 <2.0.0, matches 1.2.3 1.2.9 1.4.0
^0.2.4 means >=0.2.4 <0.3.0, matches 0.2.4 0.2.9
^0.0.4 means >=0.0.4 <0.0.5, matches 0.0.4
~1.2.3 means >=1.2.3 <1.3.0, matches 1.2.3 1.2.9
~0.2.4 means >=0.2.4 <0.3.0, matches 0.2.4 0.2.9
1.2.3 means >=1.2.3 <1.2.4, matches 1.2.3
```

- Verdict: idiom. A version is a tuple of three ints, and Python compares tuples element by element, so `low <= parse(v) < high` is the whole range test.
- The constraint `1.2.3` here has the npm meaning, one exact version. In Cargo it would mean `^1.2.3`.
- For real projects, the `packaging` library parses and compares Python versions and specifiers.
