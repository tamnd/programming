```python run id=in-place
def reverse_in_place(xs):
    i, j = 0, len(xs) - 1
    while i < j:
        xs[i], xs[j] = xs[j], xs[i]
        i += 1
        j -= 1


data = [1, 2, 3, 4, 5]
reverse_in_place(data)
print(data)

count = 0
for ch in "imperative":
    if ch in "aeiou":
        count += 1
print(count)
```

```output for=in-place
[5, 4, 3, 2, 1]
5
```

```python wrong id=alias
grid = [[0] * 3] * 2
grid[0][0] = 9
print(grid)

a = [1, 2]
b = a
b.append(3)
print(a)
```

```output for=alias
[[9, 0, 0], [9, 0, 0]]
[1, 2, 3]
```

- Verdict: native. Python has assignment, augmented assignment such as `+=`, `while` and `for` loops, and mutable lists, dictionaries and objects. The reference states that assignment statements "are used to (re)bind names to values and to modify attributes or items of mutable objects".
- A variable names an object, and assignment binds the name. It does not copy the object.
- In the wrong block, `[[0] * 3] * 2` makes a list that holds the same inner list twice, and `b = a` makes a second name for one list. A change through one name shows through the other. Use `[[0] * 3 for _ in range(2)]` and `list(a)` to make copies.
