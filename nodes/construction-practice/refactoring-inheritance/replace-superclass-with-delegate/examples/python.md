```python run id=superclass-to-delegate
class StackBefore(list):
    def peek(self):
        return self[-1]


class Stack:
    def __init__(self) -> None:
        self._storage: list = []

    def push(self, x) -> None:
        self._storage.append(x)

    def pop(self):
        return self._storage.pop()

    def peek(self):
        return self._storage[-1]

    def __len__(self) -> int:
        return len(self._storage)


before = StackBefore([1, 2, 3])
before.insert(1, 99)  # The list interface lets a caller insert in the middle.
assert before == [1, 99, 2, 3]

stack = Stack()
for x in (1, 2, 3):
    stack.push(x)
assert not hasattr(stack, "insert")
assert stack.pop() == 3
print(stack.peek(), len(stack))
```

```output
2 2
```
