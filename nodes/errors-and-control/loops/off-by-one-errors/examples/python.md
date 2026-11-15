```python run id=fenceposts-slices-and-pairs
# n items need n - 1 separators.
words = ["red", "green", "blue"]
out = ""
for w in words:
    out += w + ", "
print(repr(out))  # one separator too many
print(repr(", ".join(words)))

# Chunks with half-open slices: the stop of one is the start of the next.
data = list(range(1, 11))
size = 4
chunks = [data[i:i + size] for i in range(0, len(data), size)]
print(chunks)

# n items have n - 1 pairs of neighbors.
temps = [12, 15, 11, 18]
rises = [temps[i + 1] - temps[i] for i in range(len(temps) - 1)]
print(rises)
print([b - a for a, b in zip(temps, temps[1:])])

# The last index is len - 1. A negative index counts from the end.
print(temps[len(temps) - 1], temps[-1])
```

```output for=fenceposts-slices-and-pairs
'red, green, blue, '
'red, green, blue'
[[1, 2, 3, 4], [5, 6, 7, 8], [9, 10]]
[3, -4, 7]
[3, -4, 7]
18 18
```

- Verdict: native. The loop that adds `", "` after each word leaves one separator too many, and `join` puts separators only between the items. The slices `data[i:i + size]` are half-open, so each chunk starts where the last one stopped, and the last chunk is shorter. Four temperatures give three differences, so the loop stops at `len(temps) - 1`, and `zip` with `temps[1:]` gives the same pairs with no index.
