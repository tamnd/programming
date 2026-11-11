```python run id=frames-and-closures
import sys


def depth():
    frame, n = sys._getframe(), 0
    while frame.f_code.co_name != "<module>":  # walk up to the module
        frame, n = frame.f_back, n + 1
    return n


def outer():
    return inner()


def inner():
    return depth()


print(depth(), outer())


def make_counter():
    count = 0  # outlives the call, because the closure holds it

    def inc():
        nonlocal count
        count += 1
        return count

    return inc


c = make_counter()
c()
print(c(), c.__closure__[0].cell_contents)

sys.setrecursionlimit(200)


def down(n):
    return 0 if n == 0 else 1 + down(n - 1)


print(down(150))
try:
    down(1000)
except RecursionError as e:
    print(type(e).__name__)
```

```output for=frames-and-closures
1 3
2 2
150
RecursionError
```

```python fails expect=exception:RecursionError id=deep-recursion
class Node:
    def __init__(self, value, next=None):
        self.value, self.next = value, next


def total(node):  # one stack frame for each node
    return 0 if node is None else node.value + total(node.next)


head = None
for i in range(100_000):
    head = Node(1, head)
print(total(head))
```

- Verdict: native. CPython keeps a record of each active call, and makes a frame object for it when code asks, as `sys._getframe` does. Each frame links to the frame of its caller, so `depth` walks the chain up to the module. Every Python object, including the frames, lives on the heap and stays alive while something refers to it, so a local variable captured by a closure outlives the call that made it.
- Python limits the depth of recursion, and raises `RecursionError` when a call would go deeper. `sys.setrecursionlimit` changes the limit, but a very high limit can crash the process when the native stack runs out.
- The fails block sums a linked list of 100,000 nodes with recursion, one frame for each node, and stops with `RecursionError`. Use a loop for long lists and deep trees.
