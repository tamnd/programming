```python run id=list-and-tuple
from collections.abc import MutableSequence, Sequence

# list is the mutable sequence type. tuple is the immutable one.
nums = [3, 1, 2]
point = (3, 1, 2)
nums[0] = 30
try:
    point[0] = 30
except TypeError as e:
    print("TypeError:", e)

# Both are sequences: they have a length, indices, slices and in.
for s in (nums, point):
    print(type(s).__name__, len(s), s[-1], s[1:], 2 in s)

# The abstract base classes name the interface. list has the mutating methods.
print(isinstance(nums, MutableSequence), isinstance(point, MutableSequence),
      isinstance(point, Sequence))

# == compares items in order, but a list never equals a tuple.
print(nums == [30, 1, 2], [3, 1, 2] == [1, 2, 3], list(point) == [3, 1, 2], nums == point)
```

```output
TypeError: 'tuple' object does not support item assignment
list 3 2 [1, 2] True
tuple 3 2 (1, 2) True
True False True
True False True False
```

- Verdict: native. list is the mutable sequence and tuple the immutable one. Both have a length, indices, slices and in, but only list is a MutableSequence, and an item assignment to a tuple raises TypeError. == compares the items in order, and a list never equals a tuple with the same items.
