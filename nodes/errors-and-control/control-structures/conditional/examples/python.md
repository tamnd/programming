```python run id=truth-chains-walrus-and-match
# A condition can be any value: empty collections, 0 and None are false.
for items in ([], [1, 2]):
    if items:
        print("has", len(items), "items")
    else:
        print("empty")

# Comparisons chain, so 0 <= x < 10 tests both bounds.
x = 7
print(0 <= x < 10, 0 <= 12 < 10)

# The walrus operator binds a name inside the condition.
data = {"port": "8080"}
if (port := data.get("port")) is not None:
    print("port", int(port))

# A dictionary can replace a long chain of elif for fixed cases.
codes = {200: "OK", 404: "Not Found", 500: "Server Error"}
for code in (200, 404, 418):
    print(code, codes.get(code, "Unknown"))

# match with a guard chooses a case by shape and condition.
for point in [(0, 0), (3, 0), (2, 2), (1, 5)]:
    match point:
        case (0, 0):
            print(point, "origin")
        case (x, 0):
            print(point, "on the x axis")
        case (x, y) if x == y:
            print(point, "on the diagonal")
        case _:
            print(point, "elsewhere")
```

```output for=truth-chains-walrus-and-match
empty
has 2 items
True False
port 8080
200 OK
404 Not Found
418 Unknown
(0, 0) origin
(3, 0) on the x axis
(2, 2) on the diagonal
(1, 5) elsewhere
```

- Verdict: native. An empty list is false in a condition and a list with items is true. `0 <= x < 10` tests both bounds. `:=` binds the value that the condition tests. A dictionary with `get` replaces a chain of `elif` for fixed codes. `match` with the guard `if x == y` finds the point on the diagonal.
