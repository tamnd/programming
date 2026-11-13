```python run id=list-and-array-module
from array import array

# A list is the usual array in Python. [0] * 5 makes five zeros.
a = [0] * 5
a[0], a[4] = 7, 9
print(a, len(a))

# A negative index counts from the end. An index past the end raises IndexError.
print(a[-1])
try:
    a[5]
except IndexError as e:
    print("IndexError:", e)

# Assignment copies the reference, not the items.
b = a
b[1] = 1
print(a[1], a is b)
c = a.copy()
c[1] = 2
print(a[1], c[1])

# The array module holds numbers of one C type. The type code "b" is a signed byte.
small = array("b", [1, 2, 3])
try:
    small.append(200)
except OverflowError as e:
    print("OverflowError:", e)
print(small.typecode, small.itemsize, small.tolist())
```

```output
[7, 0, 0, 0, 9] 5
9
IndexError: list index out of range
1 True
1 2
OverflowError: signed char is greater than maximum
b 1 [1, 2, 3]
```

- Verdict: stdlib. The core language has no fixed-size array. The built-in list is a dynamic array of references, with negative indices and an IndexError for an index past the end. The array module stores numbers of one C type, so a signed byte cannot hold 200. Assignment shares the list, and copy makes a new one.
