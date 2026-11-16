```python run id=shared-list
a = [1, 2, 3]
b = a  # copies the reference, not the list
b.append(4)
print(a, a is b)

c = list(a)  # a shallow copy is a new list
c.append(5)
print(a, c, a is c)

grid = [[0] * 3] * 3  # three references to one inner list
grid[0][0] = 1
print(grid)

grid = [[0] * 3 for _ in range(3)]  # three inner lists
grid[0][0] = 1
print(grid)

n = 1
m = n
m += 1  # an int cannot change, so += binds m to another object
print(n, m)
```

```output for=shared-list
[1, 2, 3, 4] True
[1, 2, 3, 4] [1, 2, 3, 4, 5] False
[[1, 0, 0], [1, 0, 0], [1, 0, 0]]
[[1, 0, 0], [0, 0, 0], [0, 0, 0]]
1 2
```

- Verdict: idiom. Every Python variable holds a reference, so `b = a` makes two names for one list, and a change through `b` shows through `a`. `[[0] * 3] * 3` repeats the reference to one inner list, so a change to one row changes all three. An `int` cannot change, so `m += 1` makes `m` refer to another object and `n` keeps its value; this is why immutable types act like value types.
