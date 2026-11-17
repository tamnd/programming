```python run id=two-layouts
from dataclasses import dataclass


@dataclass
class Node:
    value: int
    next: "Node | None" = None


array = [10, 20, 30, 40, 50]
head = None
for v in reversed(array):
    head = Node(v, head)


def get(node, i):
    steps = 0
    while i > 0:
        node, i, steps = node.next, i - 1, steps + 1
    return node.value, steps


print(f"array[3] = {array[3]} (0 steps)")
value, steps = get(head, 3)
print(f"linked[3] = {value} ({steps} steps)")

head = Node(5, head)
out = []
node = head
while node:
    out.append(str(node.value))
    node = node.next
print("after push_front:", " ".join(out))
```

```output for=two-layouts
array[3] = 40 (0 steps)
linked[3] = 40 (3 steps)
after push_front: 5 10 20 30 40 50
```

- Verdict: native. A Python `list` is a dynamic array. A linked list is a small class whose objects hold a value and a reference to the next object.
- Reading by index from the array takes one step. The linked list follows one reference for each position.
- Adding at the front of the linked list makes one node and moves nothing.
