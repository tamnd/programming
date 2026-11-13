```python run id=slice-objects-and-assignment
# s[i:j:k] calls __getitem__ with a slice object.
s = slice(1, None, 2)
xs = list("abcdefg")
print(xs[s], s.indices(len(xs)))

# Assignment to a slice can change the length of the list.
ys = [0, 1, 2, 3, 4]
ys[1:3] = ["a", "b", "c", "d"]
print(ys, len(ys))
del ys[::2]
print(ys)

# An extended slice with a step must get the same number of items.
try:
    ys[::2] = [1]
except ValueError as e:
    print("ValueError:", e)

# Strings and tuples slice to new strings and tuples.
word = "programming"
print(word[3:7], word[-4:], (1, 2, 3, 4)[1:3])

# A slice copies only the outer list. The inner lists are shared.
grid = [[1, 2], [3, 4]]
top = grid[:1]
top[0].append(99)
print(grid)
```

```output
['b', 'd', 'f'] (1, 7, 2)
[0, 'a', 'b', 'c', 'd', 3, 4] 7
['a', 'c', 3]
ValueError: attempt to assign sequence of size 1 to extended slice of size 2
gram ming (2, 3)
[[1, 2, 99], [3, 4]]
```

- Verdict: native. s[i:j:k] passes a slice object to __getitem__, and indices gives the bounds for a length. An assignment to a slice can change the length of a list, and del removes every second item. An extended slice needs the same number of items, or it raises ValueError. A slice copies only the outer list, so the inner lists are shared.
