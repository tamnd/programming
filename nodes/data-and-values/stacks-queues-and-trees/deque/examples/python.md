```python run id=deque
from collections import deque

d = deque(["b", "c"])
d.appendleft("a")
d.append("d")
print(list(d), "front", d[0], "back", d[-1])

print(d.popleft(), d.pop(), list(d))

d.extend(["x", "y", "z"])
d.rotate(1)
print("rotate 1:", list(d))

last_three = deque(range(10), maxlen=3)
print("maxlen 3:", list(last_three))
```

```output for=deque
['a', 'b', 'c', 'd'] front a back d
a d ['b', 'c']
rotate 1: ['z', 'b', 'c', 'x', 'y']
maxlen 3: [7, 8, 9]
```

- Verdict: stdlib. `collections.deque` has `append` and `pop` at the right end, and `appendleft` and `popleft` at the left end.
- `rotate(1)` moves the last item to the front.
- With `maxlen`, the deque keeps only the newest items.
