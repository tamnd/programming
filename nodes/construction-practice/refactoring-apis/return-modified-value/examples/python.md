```python run id=returned-value
points = [10, 25, 20, 32]

total_ascent_before = 0


def calculate_ascent_before() -> None:
    global total_ascent_before
    for previous, current in zip(points, points[1:]):
        total_ascent_before += max(current - previous, 0)


calculate_ascent_before()


def calculate_ascent(elevations: list[int]) -> int:
    return sum(max(current - previous, 0) for previous, current in zip(elevations, elevations[1:]))


TOTAL_ASCENT = calculate_ascent(points)
assert TOTAL_ASCENT == total_ascent_before
calculate_ascent_before()
assert total_ascent_before == 2 * TOTAL_ASCENT
assert calculate_ascent(points) == TOTAL_ASCENT
print(TOTAL_ASCENT)
```

```output
27
```
