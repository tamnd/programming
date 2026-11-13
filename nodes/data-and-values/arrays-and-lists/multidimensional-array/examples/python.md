```python run id=lists-of-lists
# A list of lists. Each row is a separate list, so the rows can have different lengths.
grid = [[0] * 3 for _ in range(2)]
grid[0][1] = 5
print(grid, len(grid), len(grid[0]))

jagged = [[1], [2, 3], [4, 5, 6]]
print([len(row) for row in jagged], jagged[2][1])

# [[0] * 3] * 2 repeats one row object, so a write to one row shows in both.
bad = [[0] * 3] * 2
bad[0][1] = 5
print(bad, bad[0] is bad[1])

# A copy of the outer list shares the rows. A deep copy copies each row.
import copy
shallow = grid.copy()
deep = copy.deepcopy(grid)
grid[1][2] = 9
print(shallow[1], deep[1])

# zip(*m) gives the columns, which is the transpose.
m = [[1, 2, 3], [4, 5, 6]]
print([list(col) for col in zip(*m)])

# A flat list with an index formula is a dense matrix in row-major order.
rows, cols = 2, 3
flat = [0] * (rows * cols)
for r in range(rows):
    for c in range(cols):
        flat[r * cols + c] = 10 * r + c
print(flat, flat[1 * cols + 2])

# Three dimensions: a list of planes of rows.
cube = [[[x + 2 * y + 4 * z for x in range(2)] for y in range(2)] for z in range(2)]
print(cube, cube[1][0][1])
```

```output
[[0, 5, 0], [0, 0, 0]] 2 3
[1, 2, 3] 5
[[0, 5, 0], [0, 5, 0]] True
[0, 0, 9] [0, 0, 0]
[[1, 4], [2, 5], [3, 6]]
[0, 1, 2, 10, 11, 12] 12
[[[0, 1], [2, 3]], [[4, 5], [6, 7]]] 5
```

- Verdict: stdlib. A comprehension makes a new list for each row, so rows are independent and can differ in length. [[0] * 3] * 2 repeats one row object, so a write shows in both rows. A copy of the outer list shares the rows, and deepcopy does not. zip(*m) gives the transpose. A flat list with r * cols + c is a dense matrix, and a list of planes of rows has three dimensions.
