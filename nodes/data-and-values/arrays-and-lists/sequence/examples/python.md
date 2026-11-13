```python run id=sequence-types
from collections.abc import MutableSequence, Sequence
from itertools import islice

# list, tuple, str, bytes and range are sequences: they have len and integer indices.
for value in ([1, 2], (1, 2), "ab", b"ab", range(2)):
    print(type(value).__name__, isinstance(value, Sequence), isinstance(value, MutableSequence))

# A dict also has len and [], but its keys are not positions, so it is not a sequence.
# A set has len but no order. A generator has an order but no len and no index.
gen = (x for x in range(3))
print(isinstance({}, Sequence), isinstance(set(), Sequence), isinstance(gen, Sequence))

# The common sequence operations work on every built-in sequence.
for s in ([3, 1, 3], (3, 1, 3), range(3, 0, -1)):
    print(type(s).__name__, len(s), s[0], s[-1], s.index(1), s.count(3), min(s), 1 in s)

# A str is a sequence of strings of length 1, and in searches for a substring.
print("abc".index("b"), "bc" in "abc", "abc"[::-1])

# A generator gives its items once. A second pass finds nothing.
squares = (n * n for n in range(4))
print(list(squares), list(squares))
print(list(islice(range(10**18), 3)), len(range(10**18)))
```

```output
list True True
tuple True False
str True False
bytes True False
range True False
False False False
list 3 3 3 1 2 1 True
tuple 3 3 3 1 2 1 True
range 3 3 1 2 1 1 True
1 True cba
[0, 1, 4, 9] []
[0, 1, 2] 1000000000000000000
```

- Verdict: native. list, tuple, str, bytes and range are Sequence, and only list is a MutableSequence. A dict, a set and a generator are not sequences. len, indexing, index, count, min and in work the same on every built-in sequence. A generator gives its items once, and range(10**18) has a length without storing its items.
