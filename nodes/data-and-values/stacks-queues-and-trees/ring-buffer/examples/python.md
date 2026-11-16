```python run id=ring
from collections import deque

ring = deque(maxlen=3)
for reading in [10, 20, 30, 40, 50]:
    ring.append(reading)
    print(f"after {reading}:", *ring)

oldest = ring.popleft()
print(f"read {oldest}, left:", *ring)
```

```output for=ring
after 10: 10
after 20: 10 20
after 30: 10 20 30
after 40: 20 30 40
after 50: 30 40 50
read 30, left: 40 50
```

- Verdict: stdlib. `deque(maxlen=3)` is a ring buffer that overwrites. When it is full, `append` drops the item at the left end.
- `popleft` reads the oldest item.
