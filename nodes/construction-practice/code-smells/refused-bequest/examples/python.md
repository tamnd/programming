```python run id=stop-inheriting-the-array
# Before: a stack that is a list, with all of the list's methods.
class StackBefore(list):
    def peek(self):
        return self[-1]


# After: a stack that has a list, and offers four methods.
class Stack:
    __slots__ = ("_items",)

    def __init__(self):
        self._items = []

    def push(self, x):
        self._items.append(x)

    def pop(self):
        return self._items.pop()

    def peek(self):
        return self._items[-1]

    def __len__(self):
        return len(self._items)


before, after = StackBefore(), Stack()
for x in [1, 2, 3]:
    before.append(x)
    after.push(x)
assert after.peek() == before.peek() == 3
assert after.pop() == before.pop() == 3
assert len(after) == len(before) == 2

# The bequest: a client can change the bottom of the first stack.
before.insert(0, 0)
before.sort(reverse=True)
assert before == [2, 1, 0]
assert not hasattr(after, "insert") and not hasattr(after, "sort")
print("stack: tests pass")
```

```output
stack: tests pass
```

- `StackBefore` passes `isinstance(s, list)`, so any function that takes a list also takes the stack and may call `insert`, `sort` or `del s[0]` on it.
- `Stack` defines `__len__`, so `len(stack)` works. It does not define `__getitem__`, so `stack[0]` is an error, which is the point.
- The collections module offers `collections.deque`, which a stack class can hold in place of a list when it also needs fast operations at the other end.
