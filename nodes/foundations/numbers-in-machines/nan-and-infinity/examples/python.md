```python run id=traps
import json
import math

nan = float("nan")
other = float("nan")
print("isnan:", math.isnan(nan), "isinf:", math.isinf(-math.inf))

values = [3.0, nan, 1.0, 2.0]
print("sorted:", sorted(values))
print("max(1.0, nan):", max(1.0, nan), "max(nan, 1.0):", max(nan, 1.0))

print("same object in list:", nan in [nan], "new NaN in list:", other in [nan])
d = {nan: "a"}
d[other] = "b"
print("dict size after two NaN keys:", len(d))

print("json.dumps:", json.dumps([nan, math.inf]))
try:
    json.dumps(nan, allow_nan=False)
except ValueError as e:
    print("allow_nan=False:", e)
print("-0.0:", -0.0, "sign:", math.copysign(1.0, -0.0))
```

```output for=traps
isnan: True isinf: True
sorted: [3.0, nan, 1.0, 2.0]
max(1.0, nan): 1.0 max(nan, 1.0): nan
same object in list: True new NaN in list: False
dict size after two NaN keys: 2
json.dumps: [NaN, Infinity]
allow_nan=False: Out of range float values are not JSON compliant: nan
-0.0: -0.0 sign: -1.0
```

- Verdict: native. Python `float` has `inf`, `nan` and `-0.0`, with tests in `math`.
- `sorted` returns the list out of order, because every comparison with the NaN is false.
- `max` keeps its first argument when the comparison is false, so the place of the NaN changes the result.
- A dict finds a key by identity first, so the same NaN object is found, but a new NaN becomes a second key.
