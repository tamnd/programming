```python run id=points
def describe(point):
    match point:
        case (0, 0):
            return "origin"
        case (x, 0):
            return f"on the x axis at {x}"
        case (0, y):
            return f"on the y axis at {y}"
        case (x, y) if x == y:
            return f"on the diagonal at {x}"
        case (x, y):
            return f"at {x}, {y}"


for point in [(0, 0), (3, 0), (0, -2), (4, 4), (1, 5)]:
    print(f"{point}: {describe(point)}")
```

```output for=points
(0, 0): origin
(3, 0): on the x axis at 3
(0, -2): on the y axis at -2
(4, 4): on the diagonal at 4
(1, 5): at 1, 5
```

A mapping pattern takes apart a dictionary, such as the result of `json.loads`. It matches when the listed keys are present, and it ignores other keys.

```python run id=events
import json

def handle(event):
    match event:
        case {"type": "click", "pos": [x, y]}:
            return f"click at {x}, {y}"
        case {"type": "key", "key": str(key)} if len(key) == 1:
            return f"key {key}"
        case {"type": kind}:
            return f"ignored {kind}"
        case _:
            return "not an event"


lines = [
    '{"type": "click", "pos": [10, 20], "time": 5}',
    '{"type": "key", "key": "q"}',
    '{"type": "key", "key": "Enter"}',
    '{"type": "scroll"}',
    '[1, 2]',
]
for line in lines:
    print(handle(json.loads(line)))
```

```output for=events
click at 10, 20
key q
ignored key
ignored scroll
not an event
```

- Verdict: native. The `match` statement is in Python 3.10 and later. It has literal, capture, wildcard, sequence, mapping, class, OR and AS patterns, and a guard after `if`.
- The interpreter does not check that the cases are complete. If no case matches, the statement does nothing.
- A bare name, such as `x`, always binds. To compare with a constant, use a dotted name, such as `Color.RED`. [[python/match-statement]] describes each kind of pattern.
