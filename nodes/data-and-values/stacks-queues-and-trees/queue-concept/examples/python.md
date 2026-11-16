```python run id=queue
from collections import deque

queue = deque()
for item in ["a", "b", "c"]:
    queue.append(item)
print(list(queue), "front", queue[0])

print(queue.popleft(), "next", queue[0], "size", len(queue))
queue.popleft()
queue.popleft()
print("empty:", "yes" if not queue else "no")

try:
    queue.popleft()
except IndexError as error:
    print("IndexError:", error)
```

```output for=queue
['a', 'b', 'c'] front a
a next b size 2
empty: yes
IndexError: pop from an empty deque
```

- Verdict: stdlib. `collections.deque` is the queue. `append` adds at the back, `popleft` removes from the front, and `queue[0]` peeks.
- `popleft` on an empty deque raises `IndexError`.
- For a queue between threads, use `queue.Queue`.
