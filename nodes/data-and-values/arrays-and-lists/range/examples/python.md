```python run id=range-type
# range(stop) and range(start, stop) include start and exclude stop.
print(list(range(5)), list(range(2, 7)), list(range(10, 0, -3)))

# The length of a half-open range is stop - start, and an empty range has start >= stop.
r = range(2, 13)
print(len(r), r[0], r[-1], len(range(5, 5)), list(range(5, 2)))

# Two ranges that share a bound are adjacent: they cover 0..9 with no gap and no overlap.
print(list(range(0, 5)) + list(range(5, 10)) == list(range(10)))

# A range stores only start, stop and step, so a huge range is small and in is fast.
big = range(0, 10**18, 2)
print(len(big), 10**17 in big, 10**17 + 1 in big, big.index(10))

# A slice of a range is a range, and equal ranges contain the same numbers.
print(range(10)[2:8:2], range(0) == range(4, 4), range(0, 3) == range(0, 3, 1))

try:
    range(0, 10, 0)
except ValueError as e:
    print("ValueError:", e)
```

```output
[0, 1, 2, 3, 4] [2, 3, 4, 5, 6] [10, 7, 4, 1]
11 2 12 0 []
True
500000000000000000 True False 5
range(2, 8, 2) True True
ValueError: range() arg 3 must not be zero
```

- Verdict: native. range includes the start and excludes the stop, and a negative step counts down. The length is stop - start, and range(5, 5) is empty. Two ranges with a shared bound are adjacent. A range stores only its start, stop and step, so a range of 10**18 numbers has a length, a fast in and index. A slice of a range is a range. A step of 0 raises ValueError.
