```python run id=stack
stack = []
for item in ["a", "b", "c"]:
    stack.append(item)
print(stack, "top", stack[-1])

print(stack.pop(), stack.pop(), stack)
stack.pop()
print("empty:", "yes" if not stack else "no")

try:
    stack.pop()
except IndexError as error:
    print("IndexError:", error)
```

```output for=stack
['a', 'b', 'c'] top c
c b ['a']
empty: yes
IndexError: pop from empty list
```

- Verdict: idiom. A `list` is the stack. `append` pushes, `pop()` with no index pops, and `stack[-1]` peeks.
- An empty list is false in a condition, so `not stack` tests for an empty stack.
- `pop` on an empty list raises `IndexError`.
