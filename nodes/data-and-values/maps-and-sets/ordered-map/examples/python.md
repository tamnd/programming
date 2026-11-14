```python run id=sorted-and-bisect
import bisect

# A dict keeps insertion order, not key order. sorted() gives the key order once.
d = {"pear": 1, "apple": 3, "fig": 2}
print(list(d), sorted(d), [v for _, v in sorted(d.items())])

# Sort by value instead of by key.
print(sorted(d.items(), key=lambda kv: kv[1]))

# A sorted list of keys with bisect gives floor, ceiling and ranges in log time.
keys = sorted([50, 10, 40, 20, 30])
print(bisect.bisect_left(keys, 25), keys[bisect.bisect_left(keys, 25)])
print(keys[bisect.bisect_left(keys, 15):bisect.bisect_right(keys, 40)])

# A key past the end has no ceiling: the index is len(keys).
print(bisect.bisect_left(keys, 99) == len(keys))

# insort keeps the list sorted, but each insert moves items, so it is linear.
bisect.insort(keys, 35)
print(keys)

# Keys must be comparable with each other. Mixed types fail.
try:
    sorted([1, "a"])
except TypeError as e:
    print(type(e).__name__)
```

```output
['pear', 'apple', 'fig'] ['apple', 'fig', 'pear'] [3, 2, 1]
[('pear', 1), ('fig', 2), ('apple', 3)]
2 30
[20, 30, 40]
True
[10, 20, 30, 35, 40, 50]
TypeError
```

- Verdict: idiom. Python has no sorted map in the standard library. sorted(d) gives the keys in order once, and a sorted list of keys with the bisect module gives floor, ceiling and ranges in logarithmic time. insort keeps the list sorted, but each insert moves items. The third-party package sortedcontainers has SortedDict.
