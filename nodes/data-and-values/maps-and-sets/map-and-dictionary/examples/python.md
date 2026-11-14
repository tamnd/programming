```python run id=dict-basics
# Create a dict with a literal, from pairs, and from keyword arguments.
a = {"x": 1, "y": 2}
b = dict([("x", 1), ("y", 2)])
c = dict(x=1, y=2)
print(a == b == c, len(a))

# Insert, update, read and delete.
a["z"] = 3
a["x"] += 10
print(a["x"], a.get("w"), a.get("w", -1), a.pop("y"), a)

# setdefault inserts only when the key is missing.
print(a.setdefault("x", 0), a.setdefault("w", 0), a)

# Keys must be hashable: a tuple can be a key, a list cannot.
grid = {(0, 0): "start", (2, 3): "end"}
print(grid[(2, 3)])
try:
    grid[[0, 0]] = "bad"
except TypeError as e:
    print(type(e).__name__)

# Equal keys are the same key: 1, 1.0 and True are equal, so the dict keeps one entry.
d = {1: "int"}
d[1.0] = "float"
d[True] = "bool"
print(d)

# A dict keeps insertion order. Changing a dict during a loop raises RuntimeError.
try:
    for k in a:
        a[k + "!"] = 0
except RuntimeError:
    print("RuntimeError")
```

```output
True 2
11 None -1 2 {'x': 11, 'z': 3}
11 0 {'x': 11, 'z': 3, 'w': 0}
end
TypeError
{1: 'bool'}
RuntimeError
```

- Verdict: native. A dict comes from a literal, from pairs or from keyword arguments. d[key] raises KeyError for a missing key, and get and setdefault give a default. A tuple can be a key and a list cannot. 1, 1.0 and True are equal, so they are one key. A change to the keys during a loop raises RuntimeError.
