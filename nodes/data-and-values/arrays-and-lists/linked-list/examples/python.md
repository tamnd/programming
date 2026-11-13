```python run id=deque
from collections import deque

# collections.deque is a doubly linked list of fixed-size blocks.
# appendleft and popleft change the front without moving the other items.
d = deque([2, 3])
d.appendleft(1)
d.append(4)
print(d)
first, last = d.popleft(), d.pop()
print(first, last, d)

# rotate moves items from one end to the other.
d = deque([1, 2, 3, 4, 5])
d.rotate(2)
print(d)

# A deque with maxlen drops items from the other end when it is full.
last3 = deque(maxlen=3)
for x in range(6):
    last3.append(x)
print(last3)

# A plain list moves every item for an insert at index 0. deque does not.
lst = list(range(5))
lst.insert(0, -1)
dq = deque(range(5))
dq.appendleft(-1)
print(lst == list(dq), dq[0], dq[-1])
```

```output
deque([1, 2, 3, 4])
1 4 deque([2, 3])
deque([4, 5, 1, 2, 3])
deque([3, 4, 5], maxlen=3)
True -1 4
```

- Verdict: idiom. Python has no plain linked list class, so a program writes a node class, as in the Idea block. collections.deque is a doubly linked list of blocks: appendleft, popleft and pop change the ends without moving the other items, rotate moves items from one end to the other, and maxlen drops items from the far end.
