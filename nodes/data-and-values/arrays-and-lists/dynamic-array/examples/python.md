```python run id=list-growth
import sys

# A list is a dynamic array of references. append adds at the end.
a = []
sizes = []
for i in range(1000):
    a.append(i)
    sizes.append(sys.getsizeof(a))

# The list asks for more memory only now and then, not at each append.
resizes = sum(1 for x, y in zip(sizes, sizes[1:]) if y != x)
print(len(a), resizes < 100)

# extend adds many items in one call. pop removes the last item.
a.extend([1000, 1001])
print(len(a), a.pop(), len(a))

# insert at the front moves every other item one place to the right.
b = [1, 2, 3]
b.insert(0, 0)
print(b)

# [None] * n makes all the slots at once, so the loop only assigns.
c = [None] * 5
for i in range(5):
    c[i] = i * i
print(c)
```

```output
1000 True
1002 1001 1001
[0, 1, 2, 3]
[0, 1, 4, 9, 16]
```

- Verdict: native. The built-in list is a dynamic array of references. sys.getsizeof changes far fewer times than the 1000 appends, so the list grows its array now and then. extend and pop work at the end, insert at index 0 moves all the items, and [None] * n makes all the slots at once.
