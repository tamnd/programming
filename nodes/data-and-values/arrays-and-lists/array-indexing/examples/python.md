```python run id=negative-indices-and-indexerror
import array

a = ["a", "b", "c", "d", "e"]
# A negative index counts from the end: a[-i] is a[len(a) - i].
print(a[-1], a[-2], a[len(a) - 2], a[-0])

# An index past either end raises IndexError, but a slice cuts its bounds.
for i in (5, -6):
    try:
        a[i]
    except IndexError as e:
        print(i, "IndexError:", e)
print(a[3:99], a[-99:2])

# An index must be an integer.
try:
    a[1.0]
except TypeError as e:
    print("TypeError:", e)

# enumerate gives indices from 0, or from any start, such as 1 for people.
print(list(enumerate("xyz")), list(enumerate("xyz", start=1)))

# i % n wraps an index into 0 to n - 1, as in a ring buffer.
print([a[i % len(a)] for i in range(3, 8)])

# An array from the array module stores machine numbers one after another.
nums = array.array("i", [10, 20, 30])
print(nums.itemsize, nums[2], nums.buffer_info()[1])
```

```output
e d d a
5 IndexError: list index out of range
-6 IndexError: list index out of range
['d', 'e'] ['a', 'b']
TypeError: list indices must be integers or slices, not float
[(0, 'x'), (1, 'y'), (2, 'z')] [(1, 'x'), (2, 'y'), (3, 'z')]
['d', 'e', 'a', 'b', 'c']
4 30 3
```

- Verdict: native. A negative index counts from the end, and a[-0] is a[0]. An index past either end raises IndexError, but a slice cuts its bounds. A float index raises TypeError. enumerate gives indices from 0 or from 1, and i % n wraps an index. An array from the array module stores machine integers of 4 bytes each.
